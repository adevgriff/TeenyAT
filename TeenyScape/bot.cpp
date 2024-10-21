#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include "bot.h"
#include "TigrUtils/util.h"

Point computeVector(int ax, int ay, int bx, int by)
{
    Point result = {bx - ax, by - ay};
    return result;
}

double crossProduct(Point A, Point B)
{
    return A.x * B.y - A.y * B.x;
}

void drawBot(Bot *b)
{
    /* First need to get the screen pixels of the verticies */
    int hx = b->head.x * mapWidth + mapLeftOffset;
    int lx = b->left_shoulder.x * mapWidth + mapLeftOffset;
    int rx = b->right_shoulder.x * mapWidth + mapLeftOffset;

    int hy = b->head.y * mapHeight + mapTopOffset;
    int ly = b->left_shoulder.y * mapHeight + mapTopOffset;
    int ry = b->right_shoulder.y * mapHeight + mapTopOffset;

    /* Calculate box around the triangle */
    int bx0 = hx;
    int by0 = hy;

    int bx1 = hx;
    int by1 = hy;

    if (bx0 > lx)
        bx0 = lx;
    if (by0 > ly)
        by0 = ly;
    if (bx0 > rx)
        bx0 = rx;
    if (by0 > ry)
        by0 = ry;

    if (bx1 < lx)
        bx1 = lx;
    if (by1 < ly)
        by1 = ly;
    if (bx1 < rx)
        bx1 = rx;
    if (by1 < ry)
        by1 = ry;

    Point AB = computeVector(hx, hy, lx, ly);
    Point BC = computeVector(lx, ly, rx, ry);
    Point CA = computeVector(rx, ry, hx, hy);

    /* go through each horizontal scanline y coordinate */
    for (int y = by0; y <= by1; y++)
    {
        int begin = -1;
        int end = -1;
        for (int x = bx0; x <= bx1; x++)
        {
            int greater_count = 0;
            Point p;
            p.x = x;
            p.y = y;
            /* is the point in the triangle */
            Point AP = computeVector(hx, hy, p.x, p.y);
            Point BP = computeVector(lx, ly, p.x, p.y);
            Point CP = computeVector(rx, ry, p.x, p.y);

            greater_count += (crossProduct(AB, AP) > 0);
            greater_count += (crossProduct(BC, BP) > 0);
            greater_count += (crossProduct(CA, CP) > 0);

            if (greater_count % 3 == 0)
            {
                tigrPlot(window, x, y, b->color);
            }
        }
    }
    tigrPlot(window, b->center.x * mapWidth + mapLeftOffset, b->center.y * mapHeight + mapTopOffset, tigrRGB(0xFF, 0x11, 0x11));
    char degree_string[20];
    sprintf(degree_string, "%lf", b->curr_dir);
    tigrPrint(window, tfont, mapLeftOffset, mapTopOffset + mapHeight, tigrRGB(0x00, 0x00, 0x00), degree_string);
}

void rotateBotCCW(Bot *b, double degrees)
{
    double radians = degrees * (M_PI / 180);

    b->head.x -= b->center.x;
    b->head.y -= b->center.y;

    b->left_shoulder.x -= b->center.x;
    b->left_shoulder.y -= b->center.y;

    b->right_shoulder.x -= b->center.x;
    b->right_shoulder.y -= b->center.y;

    b->head.x = b->head.x * cos(radians) - b->head.y * sin(radians);
    b->head.y = b->head.x * sin(radians) + b->head.y * cos(radians);

    b->right_shoulder.x = b->right_shoulder.x * cos(radians) - b->right_shoulder.y * sin(radians);
    b->right_shoulder.y = b->right_shoulder.x * sin(radians) + b->right_shoulder.y * cos(radians);

    b->left_shoulder.x = b->left_shoulder.x * cos(radians) - b->left_shoulder.y * sin(radians);
    b->left_shoulder.y = b->left_shoulder.x * sin(radians) + b->left_shoulder.y * cos(radians);

    b->head.x += b->center.x;
    b->head.y += b->center.y;

    b->left_shoulder.x += b->center.x;
    b->left_shoulder.y += b->center.y;

    b->right_shoulder.x += b->center.x;
    b->right_shoulder.y += b->center.y;

    b->curr_dir += degrees;
    while (b->curr_dir > 360.0)
    {
        b->curr_dir -= 360;
    }

    while (b->curr_dir < 0.0)
    {
        b->curr_dir += 360;
    }
}

void rotateBotCW(Bot *b, double degrees)
{
    rotateBotCCW(b, -1 * degrees);
}

Bot *createBot(Point center, TPixel color, char *name)
{
    Bot *bot = (Bot *)malloc(sizeof(Bot));
    bot->center = center;
    bot->head.x = bot->center.x;
    bot->head.y = bot->center.y - (BOT_SIZE * (1 - BOT_BASE_BIAS)) - (HEAD_STRETCH * BOT_SIZE);
    bot->left_shoulder.x = bot->center.x - (BOT_SIZE / 2);
    bot->right_shoulder.x = bot->center.x + (BOT_SIZE / 2);
    bot->left_shoulder.y = bot->right_shoulder.y = bot->center.y + (BOT_SIZE * BOT_BASE_BIAS);
    bot->color = color;
    bot->curr_dir = 0;
    bot->goal_dir = 0;
    bot->dir_velocity = 0;
    bot->speed_ppf = 0;
    bot->power_used = 0;
    bot->ticks_used = 0;
    bot->points = 0;
    bot->name = strndup(name, 10);
    return bot;
}

void freeBot(Bot *b)
{
    free(b->name);
    free(b);
}

void directionUpdate(Bot *b)
{
    /* Direction */
    double diff = b->goal_dir - b->curr_dir;

    /* Normalize `diff` to be within -180 to 180 */
    if (diff > 180)
    {
        diff -= 360;
    }
    else if (diff < -180)
    {
        diff += 360;
    }

    double absolute = fabs(diff);
    int direction = (diff < 0 ? 1 : -1);

    std::cout << b->dir_velocity << std::endl;

    /* Decelerate when close to the goal and stop when very close */
    if (absolute < DECELERATION_DISTANCE && absolute > MIN_DIR_VELOCITY)
    {
        double scale = absolute / DECELERATION_DISTANCE;
        b->dir_velocity *= scale;
        if (b->dir_velocity < MIN_DIR_VELOCITY)
            b->dir_velocity = MIN_DIR_VELOCITY;
    }
    else if (absolute <= MIN_DIR_VELOCITY)
    {
        b->dir_velocity = 0;
        rotateBotCW(b, absolute * direction);
    }
    else
    {
        /* Apply acceleration */
        b->dir_velocity += DEFAULT_DIR_ACCELERATION;

        if (b->dir_velocity > MAX_DIR_VELOCITY)
            b->dir_velocity = MAX_DIR_VELOCITY;
    }

    rotateBotCW(b, b->dir_velocity * direction);
}

void botUpdate(Bot *b)
{
    directionUpdate(b);
}