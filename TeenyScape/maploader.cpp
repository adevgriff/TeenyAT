/**
 * MapLoader for TeenyScape. This MapLoader is designed
 * to interpret 32-bit BMP images.
 * 
 * Canvas  = Map to be displayed on scene.
 * Overlay = Map to contain underlying information about the scene.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

// Macros to define space information on the overlay
#define DEFAULT_SPACE 0
#define OPEN_SPACE 1
#define BLOCKED_SPACE 2

class MapLoader {
public:
    MapLoader(const char* canvas_path, const char* overlay_path);

    uint8_t extract_color(int x, int y, int c, bool is_overlay = false);
    uint8_t get_overlay(int x, int y);

    static const int CANVAS_WIDTH = 256;
    static const int CANVAS_HEIGHT = 256;

private:
    void process_image(const char* image_path, std::vector<std::vector<uint32_t>>& image_data);

    std::vector<std::vector<uint32_t>> canvas_data;
    std::vector<std::vector<uint32_t>> overlay_data;
};

/**
 * MapLoader constructor. Initializes the canvas and overlay
 * vectors.
 * 
 * @param canvas_path Path to 32-bit BMP to be loaded as main canvas.
 * @param overlay_path Path to 32-bit BMP to be loaded as overlay.
 */
MapLoader::MapLoader(const char* canvas_path, const char* overlay_path) {
    process_image(canvas_path, canvas_data);
    process_image(overlay_path, overlay_data);
}

/**
 * Process the image and load data into given vector(image_data).
 * 
 * @param image_path Path to 32-bit BMP to be processed.
 * @param image_data Vector to store pixel data.
 */
void MapLoader::process_image(const char* image_path, std::vector<std::vector<uint32_t>>& image_data) {
    std::ifstream file(image_path, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open file.\n";
        return;
    }

    // Read the BMP header to get the offset to the pixel data
    uint8_t header[54]; // Standard BMP header is 54 bytes
    file.read(reinterpret_cast<char*>(header), sizeof(header));

    // Extract the offset to pixel data from the header (bytes 10-13)
    uint32_t dataOffset = *reinterpret_cast<uint32_t*>(&header[10]);

    // Move to the pixel data
    file.seekg(dataOffset, std::ios::beg);

    const int bytesPerPixel = 4; // 32 bits per pixel

    image_data.resize(CANVAS_HEIGHT, std::vector<uint32_t>(CANVAS_WIDTH));

    const int rowSize = CANVAS_WIDTH * bytesPerPixel;
    std::vector<uint8_t> rowData(rowSize);

    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        file.read(reinterpret_cast<char*>(rowData.data()), rowSize);
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            uint8_t* pixelPtr = rowData.data() + x * bytesPerPixel;
            uint8_t b = pixelPtr[0];
            uint8_t g = pixelPtr[1];
            uint8_t r = pixelPtr[2];
            uint8_t a = pixelPtr[3];

            uint32_t pixel = (a << 24) | (r << 16) | (g << 8) | b;
            image_data[CANVAS_HEIGHT - y - 1][x] = pixel; // Flip vertically
        }
    }
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
