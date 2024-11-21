#include <cstdint>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include "../tigr.h"

#include "util.h"
#include "../field.h"

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
  tigrClear(window, tigrRGB(0x66, 0x66, 0x77));
}

void tigrUtilClean()
{
  tigrFree(window);
}

void drawMapFromLoader(MapLoader mapLoader, Field field)
{
  return;
}
