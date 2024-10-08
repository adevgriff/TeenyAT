#ifndef __UTIL_H__
#define __UTIL_H__

#include <cstdint>

extern uint16_t winWidthConstant;
extern uint16_t winHeightConstant;

extern uint16_t mapWidthConstant;
extern uint16_t mapHeightConstant;
extern uint16_t mapRightOffset;

extern uint16_t scale;

extern Tigr *window;

typedef struct
{
    int x, y, size;          // position and size of button
    char text[32];           // text to display on button
    bool state;              // current state of the button
    void (*onClick)(void *); // function pointer for on click events
    void *clickData;         // data to pass to the click handler
} Button;

int map(int num, int in_min, int in_max, int out_min, int out_max);
Button *createButton(int x, int y, int size, const char *text, void (*onClick)(void *), void *clickData);
void drawButton(Button *button, TPixel textColor);
void checkButtonClick(Button *button, int mouseX, int mouseY, int buttons);
void freeButton(void *button);
void initializeTigrWindow();
void tigrUtilClean();
void tigrWindowClear();

#endif /* __UTIL_H__ */