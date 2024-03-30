#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sstream>
#include <filesystem>

#include "teenyat.h"
#include "test.h"


typedef struct _test {
    std::string test_path;
    std::string test_name;
} test;


#if defined(_WIN64) || defined(_WIN32)
bool isWindows = true;
#define popen _popen
#define pclose _pclose
#else
bool isWindows = false;  
#endif



void setup_tests(std::vector<test> *tests) {
    std::ifstream test_config;
    test_config.open("TESTS.config");
    std::string line;
    while(test_config.is_open() && getline(test_config,line)) {
        test temp;
        temp.test_name = line.substr(0, line.find_first_of(','));
        temp.test_path = line.substr(line.find(',') + 1, line.size());
        std::string command = "make -C " + temp.test_path;
        std::system(command.c_str());
        tests->push_back(temp);
    }

}

void replaceForwardSlash(std::string& str) {
    size_t pos = 0;
    while ((pos = str.find('/', pos)) != std::string::npos) {
        str.replace(pos, 1, "\\");
        pos += 1;
    }
}

/* Chat gpt generated thank you Aero :) */
std::string getCommandOutput(const std::string& command) {
    char buffer[128];
    std::string result = "";
    // Execute the command and read results
    std::cout << command.c_str() << std::endl;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed! uh oh");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

int main(int argc, char* argv[]) {
    bool success = true;
    std::vector<std::string> failed_tests;
    std::vector<std::string> broken_tests;

    std::vector<test> tests;
    setup_tests(&tests);
    for(test t : tests) {
        printf("Name: %s     Path: %s\n", t.test_name.c_str(), t.test_path.c_str());
        std::string command = t.test_path + "/run";
        if(isWindows)  replaceForwardSlash(command);
        std::string output = getCommandOutput(command.c_str());
        std::cout << "Output: " << output << std::endl;
        if(output.find(FAILURE_MSG) != std::string::npos) {
            success = false;
            failed_tests.push_back(t.test_name);
        } else if(output.find(ERROR_MSG) != std::string::npos) {
            broken_tests.push_back(t.test_name);
        }
    }

    if(broken_tests.size() > 0) {
        std::cout << "One or more tests contain errors\n" << std::endl;
        for(std::string test : broken_tests) {
            printf("\t%s\n", test.c_str());
        }
    }

    if(!success) {
        std::cout << "One or more tests failed\n" << std::endl;
        for(std::string test : failed_tests) {
            printf("\t%s\n", test.c_str());
        }
    } else {
        std::cout << "GREEN BUILD!" << std::endl;
    }
    return 0;
}