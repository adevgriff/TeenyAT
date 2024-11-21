#ifndef __FIELD_H__
#define __FIELD_H__

#include "tigr.h"

class Field
{
public:
    /**
     * @brief Construct a new field object
     *
     * @param sec_w sections the field should be wide
     * @param sec_h sections the field should be in height
     * @param x_offset percentage of the screen width the field should be offset
     * @param y_offset percentage of the screen height the field should be offset
     * @param window window the field should exist on
     */
    Field(int sec_w, int sec_h, double x_offset, double y_offset, Tigr *window);

    /**
     * @brief This should be called every time the window dimensions change
     *
     * @param window
     */
    void update(Tigr *window);

    /**
     * @brief Draws a specifically colored rectangle used to clear and debug
     * the location of the play field
     *
     * @param window tigr window field is being added to
     */
    void draw(Tigr *window);

    /**
     * @brief Draws white grid lines used to debug grid locations on
     * the play field
     *
     * @param window tigr window to draw grid on
     */
    void drawGrid(Tigr *window);

    inline int getWidth() { return this->width; }
    inline int getHeight() { return this->height; }
    inline int getX() { return this->x_offset; }
    inline int getY() { return this->y_offset; }

private:
    int width;
    int height;

    int x_offset;
    int y_offset;
    double x_offset_percentage;
    double y_offset_percentage;

    int sections_w;
    int sections_h;
};

#endif /* __FIELD_H__ */