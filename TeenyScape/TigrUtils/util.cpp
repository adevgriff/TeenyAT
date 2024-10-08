#include <cstdint>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "../tigr.h"

#include "util.h"

uint16_t winWidthConstant = 400;
uint16_t winHeightConstant = 260;

uint16_t mapWidthConstant = 256;
uint16_t mapHeightConstant = 256;
uint16_t mapRightOffset = 2;

uint16_t scale = 4;

Tigr *window;

bool buttonLeft = true;

int map(int num, int in_min, int in_max, int out_min, int out_max)
{
  return (int)(num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Button *createButton(int x, int y, int size, const char *text, void (*onClick)(void *), void *clickData)
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
  tigrFillRect(window, button->x * scale, button->y * scale,
               button->size * scale, button->size * scale,
               tigrRGB(0xff, 0xff, 0xff));
  if (button->state)
  {
    int clickedMarkSize = button->size - 3;
    tigrFillRect(window, (button->x + 2) * scale, (button->y + 2) * scale,
                 clickedMarkSize * scale, clickedMarkSize * scale,
                 tigrRGB(0x33, 0x33, 0x33));
  }
  tigrPrint(window, tfont, (button->x + button->size) * scale, button->y * scale,
            textColor, button->text);
}

void checkButtonClick(Button *button, int mouseX, int mouseY, int buttons)
{
  bool leftMouse = buttons & 1;
  if (leftMouse && mouseX >= button->x * scale && mouseX <= (button->x + button->size) * scale &&
      mouseY >= button->y * scale && mouseY <= (button->y + button->size) * scale)
  {
    if (buttonLeft)
    {
      buttonLeft = false;
      if (button->onClick)
      {
        button->onClick(button->clickData);
      }
    }
  }
  else
  {
    buttonLeft = true;
  }
}

void freeButton(Button *button)
{
  free(button);
}

void initializeTigrWindow()
{
  window = tigrWindow(winWidthConstant * scale, winHeightConstant * scale,
                      "TeenyScape", TIGR_AUTO);
  tigrClear(window, tigrRGB(0x66, 0x66, 0x77));
  tigrFillRect(window, (winWidthConstant - mapWidthConstant - mapRightOffset) * scale,
               ((winHeightConstant - mapHeightConstant) / 2) * scale,
               mapWidthConstant * scale, mapHeightConstant * scale, tigrRGB(0xdd, 0xbb, 0xff));
  tigrUpdate(window);
}

void tigrWindowClear()
{
  tigrClear(window, tigrRGB(0x66, 0x66, 0x77));
  tigrFillRect(window, (winWidthConstant - mapWidthConstant - mapRightOffset) * scale,
               ((winHeightConstant - mapHeightConstant) / 2) * scale,
               mapWidthConstant * scale, mapHeightConstant * scale, tigrRGB(0xdd, 0xbb, 0xff));
}

void tigrUtilClean()
{
  tigrFree(window);
}
