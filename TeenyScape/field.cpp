#include "field.h"
#include <cmath>

Field::Field(int sec_w, int sec_h, double x_offset, double y_offset, Tigr *window)
{
    this->sections_w = sec_w;
    this->sections_h = sec_h;
    x_offset = std::fmod(x_offset, 1.0);
    y_offset = std::fmod(y_offset, 1.0);
    this->x_offset_percentage = x_offset;
    this->y_offset_percentage = y_offset;
    int win_w = window->w;
    int win_h = window->h;
    this->x_offset = win_w * x_offset;
    this->y_offset = win_h * y_offset;

    this->width = win_w - this->x_offset;
    this->height = win_h - this->y_offset;
}

void Field::update(Tigr *window)
{
    int win_w = window->w;
    int win_h = window->h;
    this->x_offset = win_w * x_offset_percentage;
    this->y_offset = win_h * y_offset_percentage;
    this->width = win_w - this->x_offset;
    this->height = win_h - this->y_offset;
}

void Field::draw(Tigr *window)
{

    tigrFillRect(window, this->x_offset, this->y_offset, this->width, this->height, tigrRGB(0xdd, 0xbb, 0xff));
}
void Field::drawGrid(Tigr *window)
{
    for (int i = 1; i < this->sections_w; i++)
    {
        int x_pos = this->x_offset + (this->width * i / this->sections_w);
        tigrLine(window, x_pos, this->y_offset,
                 x_pos, this->y_offset + this->height,
                 tigrRGB(0xff, 0xff, 0xff));
    }

    for (int i = 1; i < this->sections_h; i++)
    {
        int y_pos = this->y_offset + (this->height * i / this->sections_h);
        tigrLine(window, this->x_offset, y_pos,
                 this->x_offset + this->width, y_pos,
                 tigrRGB(0xff, 0xff, 0xff));
    }
}