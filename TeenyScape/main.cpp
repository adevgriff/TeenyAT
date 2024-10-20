#include <iostream>

#include "tigr.h"

#include "TigrUtils/util.h"
#include "bot.h"

void gridToggleOnClick(void *data)
{
    std::cout << "button clicked" << std::endl;
    Button *button = (Button *)data;
    button->state = !button->state;
}

void drawMapGrid()
{
    for (int i = 1; i < 256; i++)
    {
        tigrLine(window, mapLeftOffset + (mapWidth * i / 256), mapTopOffset,
                 mapLeftOffset + (mapWidth * i / 256), mapTopOffset + mapHeight,
                 tigrRGB(0xff, 0xff, 0xff));

        tigrLine(window, mapLeftOffset, mapTopOffset + (mapHeight * i / 256),
                 mapLeftOffset + mapWidth, mapTopOffset + (mapHeight * i / 256),
                 tigrRGB(0xff, 0xff, 0xff));
    }
}

int main(int argc, char *argv[])
{
    /* database fabulous lavender for our color model */
    initializeTigrWindow();
    Button *gridToggleButton = createButton(0.01, 0.01, 9, "Grid Toggle", gridToggleOnClick, nullptr);
    gridToggleButton->clickData = gridToggleButton;
    Bot *b = createBot({0.5, 0.5}, tigrRGB(0x00, 0xAA, 0x66), "A LARGER TEST NAME THAT SHOULD GET CUT OFF");
    b->goal_dir = 90;
    while (!tigrClosed(window) && !tigrKeyDown(window, TK_ESCAPE))
    {
        winWidth = window->w;
        winHeight = window->h;
        int mouseX;
        int mouseY;
        int buttons;
        tigrMouse(window, &mouseX, &mouseY, &buttons);
        checkButtonClick(gridToggleButton, mouseX, mouseY, buttons);

        botUpdate(b);

        tigrWindowClear();
        drawButton(gridToggleButton, tigrRGB(0x00, 0x00, 0x00));
        if (gridToggleButton->state)
        {
            drawMapGrid();
        }
        drawBot(b);
        tigrUpdate(window);
    }

    freeBot(b);
    tigrFree(window);
    return EXIT_SUCCESS;
}