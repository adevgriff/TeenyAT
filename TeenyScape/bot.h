#ifndef __BOT_H__
#define __BOT_H__

#include "tigr.h"

#define BOT_SIZE (0.02)
#define BOT_BASE_BIAS (0.3) /* how far from center the base of the triangle is initially setup as a percentage of bot size */
#define HEAD_STRETCH (0.2)

#define MAX_DIR_VELOCITY (2.0) /* a scalar based on distance from goal? */
#define STEPS_PER_DEGREE (5)
#define STEPS_TO_360 (360 * STEPS_PER_DEGREE)
#define SINGLE_STEP_SIZE ( 1.0 / (double(STEPS_PER_DEGREE)))

typedef struct
{
    double x;
    double y;
} Point;

typedef struct
{
    Point head;
    Point left_shoulder;
    Point right_shoulder;
    Point center;
    TPixel color;
    double curr_dir;
    double goal_dir;
    double speed_ppf; /* pixels per frame */
    double power_used;
    unsigned int ticks_used;
    unsigned int points;
    char *name;
} Bot;

void drawBot(Bot *b);
void freeBot(Bot *b);
void rotateBotCCW(Bot *b, double degrees);
void rotateBotCW(Bot *b, double degrees);
void botUpdate(Bot *b);
double getDirection(Bot *b);
Bot *createBot(Point center, TPixel color, char *name);

#endif /* __BOT_H__ */