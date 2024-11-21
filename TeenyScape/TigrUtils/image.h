#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <iostream>

#include "../tigr.h"
#include "../stb_image.h"

class Image
{
public:
    Image(const char *path);
    ~Image();
    void draw(Tigr *window);
    void draw(Tigr *window, int x, int y);
    void draw(Tigr *window, int x, int y, int width, int height);
    /**
     * @brief This method resizes the image so that resizing does not have
     * to be done with each draw call
     *
     * @param width width in pixels to make the image
     * @param height height in pixels to make the image
     */
    void resize(int width, int height);
    bool isLoaded();
    void setLocation(int x, int y);

private:
    int x;
    int y;

    bool loaded;
    int width;
    int height;
    int channels;
    unsigned char *data;

    Tigr *img;
    std::string path;

    /**
     * @brief This method returns a scaled version of the provided
     *  image into the provided width and height dimensons in pixels
     *  this needs to be freed with tigrFree
     *
     * @param src
     * @param newWidth
     * @param newHeight
     * @return scaled tigr image
     */
    Tigr *scaleBitmap(Tigr *src, int newWidth, int newHeight);
};

#endif /* __IMAGE_H__ */