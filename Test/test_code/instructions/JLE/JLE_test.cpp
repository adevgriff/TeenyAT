#include <iostream>

#include "teenyat.h"
#include "test.h"
#include "utils.h"

void bus_write(teenyat *t, tny_uword addr, tny_word data, uint16_t *delay) {

}
void bus_read(teenyat *t, tny_uword addr, tny_word *data, uint16_t *delay) {

}

int main() {
    /* set up teenyat to set */
    teenyat t;
    std::string file_string = find_relative_path("JLE.bin").generic_string();
    FILE *bin_file = fopen(file_string.c_str(), "rb");
    if(bin_file != NULL) {

        tny_init_from_file(&t, bin_file, bus_read, bus_write);
        fclose(bin_file);
    }else {
        std::cout << "Failed to init bin file (invalid path?)" << std::endl;
        printf(ERROR_MSG);
        return 0;
    }

    tny_clock(&t);
    tny_clock(&t);
    while(t.reg[TNY_REG_A].u != 1) {
        tny_clock(&t);
    }

    if(t.reg[TNY_REG_B].u == 0) {
        printf(FAILURE_MSG);
    } else {
        printf(SUCCESS_MSG);
    }

    return 0;
}