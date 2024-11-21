#include <iostream>

#include "tigr.h"

#include "TigrUtils/util.h"
#include "field.h"
#include "bot.h"
#include "TigrUtils/image.h"
#include "TigrUtils/button.h"

typedef struct _button_test_struct
{
    std::string test_msg;
} button_test_struct;

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

void button_test_callback(void *button, void *data)
{
    button_test_struct *d = (button_test_struct *)data;
    Button *b = (Button *)button;

    std::cout << d->test_msg << std::endl;
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

    /* Button testing */
    button_test_struct tstruct;
    tstruct.test_msg = "this is a test message";
    Button button(10, 10, 50, 20);
    button.setData((void *)&tstruct);
    button.setCallback(button_test_callback);

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

        button.update(window);

        tigrWindowClear();
        field.draw(window);
        img.draw(window, 10, 20, 600, 300);
        button.draw(window);

        b.draw(window, &field);
        b2.draw(window, &field);
        b3.draw(window, &field);

        tigrUpdate(window);
    }

    tigrFree(window);
    return EXIT_SUCCESS;
}