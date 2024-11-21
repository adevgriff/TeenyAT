#ifndef __UTIL_H__
#define __UTIL_H__

#include <cstdint>

#include "../maploader.h"

#define SCREEN_HORIZONTAL_PARTION_AMOUNT (5)

#define MAP_SCREEN_HEIGHT_RATIO (((1 << SCREEN_HORIZONTAL_PARTION_AMOUNT) - 1) / (double)(1 << SCREEN_HORIZONTAL_PARTION_AMOUNT))
#define MAP_SCREEN_WIDTH_RATIO (2.0 / 3.0)
#define MAP_SCREEN_RIGHT_OFFSET (((1 << (SCREEN_HORIZONTAL_PARTION_AMOUNT + 1)) - 1) / (double)(1 << (SCREEN_HORIZONTAL_PARTION_AMOUNT + 1)))
#define WINDOW_OPTIONS_LEFT_OFFSET (1.0 - MAP_SCREEN_RIGHT_OFFSET)

extern int winWidth;
extern int winHeight;

extern int mapWidth;
extern int mapHeight;
extern int mapLeftOffset;
extern int mapTopOffset;

extern Tigr *window;

int map(int num, int in_min, int in_max, int out_min, int out_max);
void freeButton(void *button);
void initializeTigrWindow();
void tigrUtilClean();
void tigrWindowClear();
void drawMapFromLoader(MapLoader mapLoader);

#endif /* __UTIL_H__ */