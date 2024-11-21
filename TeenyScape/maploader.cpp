/**
 * MapLoader for TeenyScape. This MapLoader is designed
 * to interpret 32-bit BMP images.
 * 
 * Canvas  = Map to be displayed on scene.
 * Overlay = Map to contain underlying information about the scene.
 */

#include "maploader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "stb_image.h"
#include "TigrUtils/util.h"

// Macros to define space information on the overlay
#define DEFAULT_SPACE 0
#define OPEN_SPACE 1
#define BLOCKED_SPACE 2

/**
 * MapLoader constructor. Initializes the canvas and overlay
 * vectors.
 * 
 * @param canvas_path Path to 32-bit BMP to be loaded as main canvas.
 * @param overlay_path Path to 32-bit BMP to be loaded as overlay.
 */
MapLoader::MapLoader(const char *canvas_path, const char *overlay_path)
{

    // Load the image using stb_image
    int width, height, channels;
    unsigned char *data = stbi_load(canvas_path, &width, &height, &channels, 4); // Force 4 channels (RGBA)
    if (!data)
    {
        fprintf(stderr, "Failed to load image: %s\n", canvas_path);
    }

    this->img = tigrBitmap(width, height);
    this->img_w = width;
    this->img_h = height;

    // Copy the RGBA data into the Tigr bitmap
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int index = (y * width + x) * 4;
            TPixel color;
            color.r = data[index];
            color.g = data[index + 1];
            color.b = data[index + 2];
            color.a = data[index + 3];
            tigrPlot(this->img, x, y, color);
        }
    }

    stbi_image_free(data);

    // Load the image using stb_image
    data = stbi_load(overlay_path, &width, &height, &channels, 4); // Force 4 channels (RGBA)
    if (!data)
    {
        fprintf(stderr, "Failed to load image: %s\n", overlay_path);
    }

    this->foreground_img = tigrBitmap(width, height);

    // Copy the RGBA data into the Tigr bitmap
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int index = (y * width + x) * 4;
            TPixel color;
            color.r = data[index];
            color.g = data[index + 1];
            color.b = data[index + 2];
            color.a = data[index + 3];
            tigrPlot(this->foreground_img, x, y, color);
        }
    }

    stbi_image_free(data);
}

/**
 * Extracts the color data from a given canvas/overlay pixel.
 * 
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @param c The color channel. Red=0, Green=1, Blue=2, Alpha=3.
 * @param is_overlay Request canvas or overlay data. Defaults as False(selects canvas).
 */
uint8_t MapLoader::extract_color(int x, int y, int c, bool is_overlay) {
    if (x < 0 || x >= CANVAS_WIDTH || y < 0 || y >= CANVAS_HEIGHT || c < 0 || c > 3) {
        std::cerr << "Error: Coordinates or color channel out of range.\n";
        return 0;
    }

    uint32_t pixel = is_overlay ? overlay_data[y][x] : canvas_data[y][x];

    uint8_t a = (pixel >> 24) & 0xFF;
    uint8_t r = (pixel >> 16) & 0xFF;
    uint8_t g = (pixel >> 8) & 0xFF;
    uint8_t b = pixel & 0xFF;

    switch (c) {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        case 3: return a;
        default: return 0;
    }
}

/**
 * Gets the coordinate data from the overlay image.
 * 
 * @param x The x-coordinate
 * @param y The y-coordinate
 */
uint8_t MapLoader::get_overlay(int x, int y) {
    if (x < 0 || x >= CANVAS_WIDTH || y < 0 || y >= CANVAS_HEIGHT) {
        std::cerr << "Error: Coordinates out of range.\n";
        return 0;
    }

    uint32_t pixel = overlay_data[y][x];
    uint8_t r = (pixel >> 16) & 0xFF;
    uint8_t g = (pixel >> 8) & 0xFF;
    uint8_t b = pixel & 0xFF;

    uint32_t color = (r << 16) | (g << 8) | b;

    switch (color) {
        case 0x000000:
            return BLOCKED_SPACE;
        /* Database fabulous lavender for our color model */
        case 0xDDBBFF:
            return OPEN_SPACE;
        default:
            return DEFAULT_SPACE;
    }
}

void MapLoader::drawBackground(Tigr *window, int x, int y, int width, int height)
{
}

void MapLoader::drawForeground(Tigr *window, int x, int y, int width, int height)
{
}