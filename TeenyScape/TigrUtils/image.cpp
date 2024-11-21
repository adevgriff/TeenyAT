#include "image.h"
#include <iostream>
#include <string.h>

Image::Image(const char *path)
{
    this->loaded = true;
    data = stbi_load(path, &this->width, &this->height, &this->channels, 4);
    if (!data)
    {
        std::cerr << "Failed to load image at path: " << path << std::endl;
        this->loaded = false;
    }

    this->img = tigrBitmap(this->width, this->height);

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

    this->x = 0;
    this->y = 0;
    this->path = path;
}

Image::~Image()
{
    stbi_image_free(this->data);
}

void Image::resize(int width, int height)
{
    Tigr *scaled = scaleBitmap(this->img, width, height);
    tigrFree(this->img);
    this->img = scaled;
    this->width = width;
    this->height = height;
}

void Image::draw(Tigr *window)
{
    tigrBlitAlpha(window, this->img, this->x, this->y, 0, 0, this->width, this->height, 1.0);
}

void Image::draw(Tigr *window, int x, int y)
{
    tigrBlitAlpha(window, this->img, x, y, 0, 0, this->width, this->height, 1.0);
}

void Image::draw(Tigr *window, int x, int y, int widht, int height)
{
    Tigr *scaled_img = scaleBitmap(this->img, width, height);
    tigrBlitAlpha(window, scaled_img, x, y, 0, 0, width, height, 1.0);
    tigrFree(scaled_img);
}

Tigr *Image::scaleBitmap(Tigr *src, int newWidth, int newHeight)
{
    if (!src)
    {
        fprintf(stderr, "Error: Source image is null.\n");
        return nullptr;
    }

    // Create a new Tigr bitmap with the desired size
    Tigr *scaledImg = tigrBitmap(newWidth, newHeight);
    if (!scaledImg)
    {
        fprintf(stderr, "Error: Failed to allocate memory for the scaled image.\n");
        return nullptr;
    }

    // Scale pixel data
    for (int y = 0; y < newHeight; y++)
    {
        for (int x = 0; x < newWidth; x++)
        {
            // Map the destination pixel to the source image coordinates
            int srcX = x * src->w / newWidth;
            int srcY = y * src->h / newHeight;

            // Copy the pixel from the source image
            scaledImg->pix[y * newWidth + x] = src->pix[srcY * src->w + srcX];
        }
    }

    return scaledImg;
}

bool Image::isLoaded() { return this->loaded; }

void Image::setLocation(int x, int y)
{
    this->x = x;
    this->y = y;
}