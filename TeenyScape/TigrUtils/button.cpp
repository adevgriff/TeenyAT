#include "button.h"

Button::Button(int x, int y, int width, int height)
{
    this->state = BUTTON_BASE;
    this->previous_state = BUTTON_BASE;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->pressed_img = new Image("assets/buttons/button_pressed.png");
    this->pressed_img->setLocation(this->x, this->y);
    this->pressed_img->resize(this->width, this->height);

    this->hovered_img = new Image("assets/buttons/button_hovered.png");
    this->hovered_img->setLocation(this->x, this->y);
    this->hovered_img->resize(this->width, this->height);

    this->base_img = new Image("assets/buttons/button_base.png");
    this->base_img->setLocation(this->x, this->y);
    this->base_img->resize(this->width, this->height);
}

Button::~Button()
{
    delete (this->pressed_img);
    delete (this->hovered_img);
    delete (this->base_img);
}

void Button::draw(Tigr *window)
{
    if (this->state == BUTTON_BASE)
    {
        this->base_img->draw(window);
    }
    else if (this->state == BUTTON_HOVERED)
    {
        this->hovered_img->draw(window);
    }
    else
    {
        this->pressed_img->draw(window);
    }
}

void Button::setLocation(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->pressed_img->setLocation(this->x, this->y);
    this->pressed_img->resize(this->width, this->height);

    this->hovered_img->setLocation(this->x, this->y);
    this->hovered_img->resize(this->width, this->height);

    this->base_img->setLocation(this->x, this->y);
    this->base_img->resize(this->width, this->height);
}

bool Button::mousedOver(int x, int y)
{
    return (x > this->x && x < this->x + this->width && y > this->y && y < this->y + this->height);
}

void Button::update(Tigr *window)
{
    int mouse_x = -1;
    int mouse_y = -1;
    int button = -1;

    tigrMouse(window, &mouse_x, &mouse_y, &button);

    state = BUTTON_BASE;

    if (mousedOver(mouse_x, mouse_y))
    {
        state = BUTTON_HOVERED;
        if (button & 1)
        {
            state = BUTTON_PRESSED;
        }
        else if (previous_state == BUTTON_PRESSED)
        {
            /* this says that the button was pressed down and released while hovering over button*/
            this->callback((void *)this, data);
        }
    }

    this->previous_state = this->state;
}

void Button::setCallback(ButtonCallback callback)
{
    this->callback = callback;
}

void Button::setData(void *data)
{
    this->data = data;
}
