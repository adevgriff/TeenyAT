#include <iostream>

#include "tigr.h"

#include "TigrUtils/util.h"
#include "field.h"
#include "bot.h"
#include "TigrUtils/image.h"

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
    Field field(256, 256, 0.20, 0.01, window);
    Bot b;
    Bot b2;
    b2.setColor(tigrRGB(0x0, 0xAA, 0xBB));
    Bot b3;
    b3.setColor(tigrRGB(0XFF, 0x66, 0x22));

    b3.setSpeed(0.0003);
    b.setSpeed(0.0006);

    /* Temp image testing */
    Image img("maps/canvas.bmp");
    std::cout << img.isLoaded() << std::endl;
    img.resize(500, 500);

    while (!tigrClosed(window) && !tigrKeyDown(window, TK_ESCAPE))
    {
        if (winWidth != window->w || winHeight != window->h)
        {
            winWidth = window->w;
            winHeight = window->h;
            field.update(window);
        }

        b.incDirection(0.2);
        b2.incDirection(0.5);
        b.update();
        b2.update();
        b3.update();

        int mouseX;
        int mouseY;
        int buttons;
        tigrMouse(window, &mouseX, &mouseY, &buttons);
        tigrWindowClear();
        field.draw(window);
        img.draw(window, 10, 20, 600, 300);

        b.draw(window, &field);
        b2.draw(window, &field);
        b3.draw(window, &field);

        tigrUpdate(window);
    }

    tigrFree(window);
    return EXIT_SUCCESS;
}