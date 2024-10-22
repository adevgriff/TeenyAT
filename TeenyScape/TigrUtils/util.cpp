#include <cstdint>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include "../tigr.h"

#include "util.h"

int winWidth = 400;
int winHeight = 260;

int mapWidth = 256;
int mapHeight = 256;
int mapLeftOffset = 0;
int mapTopOffset = 0;

Tigr *window;

bool buttonNotClicked = true;

int map(int num, int in_min, int in_max, int out_min, int out_max)
{
  return (int)(num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Button *createButton(double x, double y, int size, const char *text, void (*onClick)(void *), void *clickData)
{
  Button *button = (Button *)malloc(sizeof(Button));
  button->x = x;
  button->y = y;
  button->size = size;
  strncpy(button->text, text, sizeof(button->text));
  button->state = false;
  button->onClick = onClick;
  button->clickData = clickData;
  return button;
}

void drawButton(Button *button, TPixel textColor)
{
  tigrFillRect(window, winWidth * button->x, winHeight * button->y,
               button->size, button->size,
               tigrRGB(0xff, 0xff, 0xff));
  if (button->state)
  {
    int clickedMarkSize = button->size - 3;
    tigrFillRect(window, winWidth * button->x + (button->size / 8),
                 winHeight * button->y + (button->size / 8), button->size * 6 / 8, button->size * 6 / 8,
                 tigrRGB(0x33, 0x33, 0x33));
  }
  tigrPrint(window, tfont, winWidth * button->x + button->size, winHeight * button->y,
            textColor, button->text);
}

void checkButtonClick(Button *button, int mouseX, int mouseY, int buttons)
{
  bool leftMouse = buttons & 1;
  int buttonLeft = winWidth * button->x;
  int buttonRight = buttonLeft + button->size;
  int buttonTop = winHeight * button->y;
  int buttonBottom = buttonTop + button->size;
  if (leftMouse && mouseX >= buttonLeft && mouseX <= buttonRight &&
      mouseY >= buttonTop && mouseY <= buttonBottom)
  {
    if (buttonNotClicked)
    {
      buttonNotClicked = false;
      if (button->onClick)
      {
        button->onClick(button->clickData);
      }
    }
  }
  else
  {
    buttonNotClicked = true;
  }
}

void freeButton(Button *button)
{
  free(button);
}

void initializeTigrWindow()
{
  /*Calculate useful sizes and offsets for current opening window size*/
  mapWidth = winWidth * MAP_SCREEN_WIDTH_RATIO;
  mapHeight = winHeight * MAP_SCREEN_HEIGHT_RATIO;
  mapLeftOffset = winWidth - mapWidth - MAP_SCREEN_RIGHT_OFFSET;
  mapTopOffset = (winHeight - mapHeight) / 2;

  std::cout << mapWidth << " : " << mapHeight << " : " << mapLeftOffset << " : " << mapTopOffset << std::endl;
  std::cout << MAP_SCREEN_WIDTH_RATIO << " : " << MAP_SCREEN_HEIGHT_RATIO << std::endl;

  window = tigrWindow(winWidth, winHeight,
                      "TeenyScape", TIGR_AUTO);
  tigrClear(window, tigrRGB(0x66, 0x66, 0x77));
  tigrFillRect(window, mapLeftOffset, mapTopOffset, mapWidth, mapHeight, tigrRGB(0xdd, 0xbb, 0xff));
  tigrUpdate(window);
}

void tigrWindowClear()
{
  /*Calculate useful sizes and offsets for current opening window size*/
  mapWidth = winWidth * MAP_SCREEN_WIDTH_RATIO;
  mapHeight = winHeight * MAP_SCREEN_HEIGHT_RATIO;
  mapLeftOffset = winWidth - mapWidth - MAP_SCREEN_RIGHT_OFFSET;
  mapTopOffset = (winHeight - mapHeight) / 2;

  tigrClear(window, tigrRGB(0x66, 0x66, 0x77));
  tigrFillRect(window, mapLeftOffset, mapTopOffset, mapWidth, mapHeight, tigrRGB(0xdd, 0xbb, 0xff));
}

void tigrUtilClean()
{
  tigrFree(window);
}

void drawMapFromLoader(MapLoader mapLoader)
{
  int mapStartX = mapLeftOffset;
  int mapStartY = mapTopOffset;

  for (int y = 0; y < mapLoader.CANVAS_HEIGHT; y++)
  {
    for (int x = 0; x < mapLoader.CANVAS_HEIGHT; x++)
    {
      uint8_t r = mapLoader.extract_color(x, y, 0); // red
      uint8_t g = mapLoader.extract_color(x, y, 1); // green
      uint8_t b = mapLoader.extract_color(x, y, 2); // blue
      uint8_t a = mapLoader.extract_color(x, y, 3); // alpha
      tigrFillRect(window, mapStartX + x * mapWidth / mapLoader.CANVAS_WIDTH, mapStartY + y * mapHeight / mapLoader.CANVAS_HEIGHT, 15, 15, tigrRGBA(r, g, b, a));
    }
  }
}
