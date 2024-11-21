#ifndef __BOT_H__
#define __BOT_H__

#define BOT_BASE 0.02
#define BOT_HEIGHT 0.03

#include "tigr.h"

#include "field.h"

typedef struct _point
{
    double x;
    double y;
} point;

class Bot
{

public:
    /**
     * @brief creates a new bot in the center of the play area;
     *
     */
    Bot();

    /**
     * @brief creates a new bot positioned where needed
     *
     * @param center_point this where the pivot point of the bot should be
     * @param direction this is the direction the bot should be facing in degrees 0 being top of screen
     */
    Bot(point center_point, double direction);
    ~Bot();

    /**
     * @brief draws bot where it is at
     *
     */
    void draw(Tigr *window, Field *field);

    void update();

    void incDirection(double amnt);

    void setSpeed(double amnt);

    void setColor(TPixel color);

private:
    point pivot;
    double direction;
    double speed;

    TPixel color;

    void drawFilledTriangle(Tigr *window, int x1, int y1, int x2, int y2, int x3, int y3);
    point computeVector(int ax, int ay, int bx, int by);
    double crossProduct(point a, point b);
    void moveCenter(double amnt);
};

#endif /* __BOT_H__*/