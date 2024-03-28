#include <iostream>

#include "utils.h"

std::filesystem::path find_relative_path(std::string name) {
    std::filesystem::path p;
    std::filesystem::path current = "./";
    // Recursively iterate over the directory and its subdirectories
    for (const auto& entry : std::filesystem::recursive_directory_iterator(current)) {
        // Check if the current entry is a file and matches the specified filename
        if (std::filesystem::is_regular_file(entry) && entry.path().filename() == name) {
            std::cout << "Found: " << entry.path() << std::endl;
            p = entry;
        }
    }

    return p;
}