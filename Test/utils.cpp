#include <iostream>
#include <string>

#include "utils.h"

#if defined(_WIN64) || defined(_WIN32)
bool isWindows = true;
#define popen _popen
#define pclose _pclose
#else
bool isWindows = false;  
#endif

void replaceForwardSlash(std::string& str) {
    size_t pos = 0;
    while ((pos = str.find('/', pos)) != std::string::npos) {
        str.replace(pos, 1, "\\");
        pos += 1;
    }
}

// All this does is convert the slashes so you can write them however yah want
void properPath(std::string& str){
    if(isWindows) replaceForwardSlash(str);
    return;
}