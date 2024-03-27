#include <iostream>
#include <vector>
#include <fstream>
#include <boost/process.hpp>
#include <sstream>

#include "teenyat.h"
#include "test.h"

typedef struct _test {
    std::string test_path;
    std::string test_name;
} test;

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

/* Chat gpt generated */
std::string getCommandOutput(const std::string& command) {
    boost::process::ipstream pipe_stream; // Stream for reading the child process' output
    std::stringstream result;

    // Execute the command and create a child process
    boost::process::child c(command, boost::process::std_out > pipe_stream);

    // Read the output
    std::string line;
    while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
        result << line << std::endl;

    c.wait(); // Wait for the process to exit

    return result.str();
}

int main(int argc, char* argv[]) {
    bool success = true;
    std::vector<std::string> failed_tests;
    std::vector<std::string> broken_tests;

    std::vector<test> tests;
    setup_tests(&tests);
    for(test t : tests) {
        printf("Name: %s     Path: %s\n", t.test_name.c_str(), t.test_path.c_str());
        std::string command = "./" + t.test_path + "/run";
        std::string output = getCommandOutput(command.c_str());
        std::cout << output << std::endl;
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