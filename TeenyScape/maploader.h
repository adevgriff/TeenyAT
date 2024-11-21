#ifndef MAPLOADER_H
#define MAPLOADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#include "tigr.h"
#include "TigrUtils/image.h"

#define DEFAULT_SPACE 0
#define OPEN_SPACE 1
#define BLOCKED_SPACE 2

class MapLoader {
public:
    MapLoader(const char* canvas_path, const char* overlay_path);

    uint8_t extract_color(int x, int y, int c, bool is_overlay=false);
    uint8_t get_overlay(int x, int y);

    static const int CANVAS_WIDTH = 256;
    static const int CANVAS_HEIGHT = 256;

    void drawBackground(Tigr *window, int x, int y, int w, int h);
    void drawForeground(Tigr *window, int x, int y, int w, int h);

private:
    std::vector<std::vector<uint16_t>> canvas_data;
    std::vector<std::vector<uint16_t>> overlay_data;
    Tigr *img;
    Tigr *foreground_img;
    int img_w;
    int img_h;
};

#endif // MAPLOADER_H
