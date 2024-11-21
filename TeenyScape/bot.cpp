#include "bot.h"
#include <cmath>

Bot::Bot()
{
    this->pivot.x = 0.5;
    this->pivot.y = 0.5;
    this->direction = 0.0;

    this->speed = 0.0;

    this->color = tigrRGB(0xAA, 0x0, 0x0);
}

Bot::Bot(point center_point, double direction)
{
    this->pivot = center_point;
    this->direction = direction;

    this->speed = 0.0;

    this->color = tigrRGB(0xA, 0x0, 0x0);
}

Bot::~Bot()
{
}

void Bot::draw(Tigr *window, Field *field)
{
    double x_offset = this->pivot.x;
    double y_offset = this->pivot.y;

    double dirRad = (this->direction + 90) * M_PI / 180.0;

    double centroidToApex = BOT_HEIGHT / 3.0;
    double centroidToBase = 2.0 * BOT_HEIGHT / 3.0;

    // Apex point relative to centroid
    double apexX = x_offset + centroidToApex * cos(dirRad);
    double apexY = y_offset - centroidToApex * sin(dirRad); // Negative sin for screen space

    // Base center relative to centroid
    double baseCenterX = x_offset - centroidToBase * cos(dirRad);
    double baseCenterY = y_offset + centroidToBase * sin(dirRad);

    // Base vertices (perpendicular to direction)
    double baseDirRad = dirRad + M_PI / 2.0; // Perpendicular to direction
    double base1X = baseCenterX + (BOT_BASE / 2.0) * cos(baseDirRad);
    double base1Y = baseCenterY - (BOT_BASE / 2.0) * sin(baseDirRad);
    double base2X = baseCenterX - (BOT_BASE / 2.0) * cos(baseDirRad);
    double base2Y = baseCenterY + (BOT_BASE / 2.0) * sin(baseDirRad);

    int x1 = apexX * field->getWidth() + field->getX();
    int y1 = apexY * field->getHeight() + field->getY();

    int x2 = base1X * field->getWidth() + field->getX();
    int y2 = base1Y * field->getHeight() + field->getY();

    int x3 = base2X * field->getWidth() + field->getX();
    int y3 = base2Y * field->getHeight() + field->getY();

    drawFilledTriangle(window, x1, y1, x2, y2, x3, y3);
}

void Bot::drawFilledTriangle(Tigr *window, int x1, int y1, int x2, int y2, int x3, int y3)
{
    int bx0 = x1;
    int by0 = y1;

    int bx1 = x1;
    int by1 = y1;

    if (bx0 > x2)
        bx0 = x2;
    if (by0 > y2)
        by0 = y2;
    if (bx0 > x3)
        bx0 = x3;
    if (by0 > y3)
        by0 = y3;

    if (bx1 < x2)
        bx1 = x2;
    if (by1 < y2)
        by1 = y2;
    if (bx1 < x3)
        bx1 = x3;
    if (by1 < y3)
        by1 = y3;

    point AB = computeVector(x1, y1, x2, y2);
    point BC = computeVector(x2, y2, x3, y3);
    point CA = computeVector(x3, y3, x1, y1);

    /* go through each horizontal scanline y coordinate */
    for (int y = by0; y <= by1; y++)
    {
        int begin = -1;
        int end = -1;
        for (int x = bx0; x <= bx1; x++)
        {
            int greater_count = 0;
            point p;
            p.x = x;
            p.y = y;
            /* is the point in the triangle */
            point AP = computeVector(x1, y1, p.x, p.y);
            point BP = computeVector(x2, y2, p.x, p.y);
            point CP = computeVector(x3, y3, p.x, p.y);

            greater_count += (crossProduct(AB, AP) > 0);
            greater_count += (crossProduct(BC, BP) > 0);
            greater_count += (crossProduct(CA, CP) > 0);

            if (greater_count % 3 == 0)
            {
                tigrPlot(window, x, y, this->color);
            }
        }
    }
}

point Bot::computeVector(int ax, int ay, int bx, int by)
{
    point result = {bx - ax, by - ay};
    return result;
}

double Bot::crossProduct(point a, point b)
{
    return a.x * b.y - a.y * b.x;
}

void Bot::incDirection(double amnt)
{
    this->direction += amnt;
    this->direction = fmod(this->direction, 360.0);

    // If the angle is negative, adjust to wrap into [0, 360)
    if (this->direction < 0)
    {
        this->direction += 360.0;
    }
}

void Bot::update()
{
    moveCenter(this->speed);
}

void Bot::moveCenter(double amnt)
{
    double dirRad = (this->direction + 90) * M_PI / 180.0;

    this->pivot.x += amnt * cos(dirRad);
    this->pivot.y -= amnt * sin(dirRad);
}

void Bot::setSpeed(double amnt)
{
    this->speed = amnt;
}

void Bot::setColor(TPixel color)
{
    this->color = color;
}