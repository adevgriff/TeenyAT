#include <iostream>

#include "tigr.h"

#include "TigrUtils/util.h"

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
    while (!tigrClosed(window) && !tigrKeyDown(window, TK_ESCAPE))
    {

        winWidth = window->w;
        winHeight = window->h;
        int mouseX;
        int mouseY;
        int buttons;
        tigrMouse(window, &mouseX, &mouseY, &buttons);
        checkButtonClick(gridToggleButton, mouseX, mouseY, buttons);

        tigrWindowClear();
        drawButton(gridToggleButton, tigrRGB(0x00, 0x00, 0x00));
        if (gridToggleButton->state)
        {
            drawMapGrid();
        }
        tigrUpdate(window);
    }

    tigrFree(window);
    return EXIT_SUCCESS;
}