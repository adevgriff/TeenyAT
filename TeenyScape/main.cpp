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
        tigrLine(window, (winWidthConstant - mapWidthConstant - mapRightOffset) * scale,
                 (((winHeightConstant - mapHeightConstant) / 2) + i) * scale, (winWidthConstant - mapRightOffset) * scale,
                 (((winHeightConstant - mapHeightConstant) / 2) + i) * scale, tigrRGB(0xff, 0xff, 0xff));

        tigrLine(window, (winWidthConstant - mapWidthConstant - mapRightOffset + i) * scale,
                 ((winHeightConstant - mapHeightConstant) / 2) * scale, (winWidthConstant - mapWidthConstant - mapRightOffset + i) * scale,
                 (((winHeightConstant - mapHeightConstant) / 2) + mapHeightConstant) * scale, tigrRGB(0xff, 0xff, 0xff));
    }
}

int main(int argc, char *argv[])
{
    /* database fabulous lavender for our color model */
    initializeTigrWindow();
    Button *gridToggleButton = createButton(10, 10, 9, "Grid Toggle", gridToggleOnClick, nullptr);
    gridToggleButton->clickData = gridToggleButton;
    while (!tigrClosed(window) && !tigrKeyDown(window, TK_ESCAPE))
    {
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