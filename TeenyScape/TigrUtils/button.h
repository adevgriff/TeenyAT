#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "../tigr.h"
#include "image.h"

typedef enum
{
    BUTTON_PRESSED,
    BUTTON_BASE,
    BUTTON_HOVERED
} ButtonState;

typedef void (*ButtonCallback)(void *button_ref, void *data);

class Button
{
public:
    Button(int x, int y, int width, int height);
    ~Button();
    void draw(Tigr *window);
    void update(Tigr *window);
    void setLocation(int x, int y, int width, int height);
    void setCallback(ButtonCallback callback);
    void setData(void *data);

private:
    ButtonState state;
    ButtonState previous_state;
    ButtonCallback callback;

    Image *pressed_img;
    Image *hovered_img;
    Image *base_img;

    int x;
    int y;
    int width;
    int height;

    std::string text;

    bool mousedOver(int x, int y);
    void *data;
};

#endif /* __BUTTON_H__ */