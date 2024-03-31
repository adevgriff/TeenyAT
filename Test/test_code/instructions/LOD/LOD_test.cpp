#include <iostream>

#include "teenyat.h"
#include "test.h"
#include "utils.h"

#define TEST_ADDR 0x8000
#define TEST_ADDR_2 0x8001

void bus_write(teenyat *t, tny_uword addr, tny_word data, uint16_t *delay) {

}
void bus_read(teenyat *t, tny_uword addr, tny_word *data, uint16_t *delay) {
     switch(addr){
          case TEST_ADDR:{
               data->u = 10;
          }
          break;
          case TEST_ADDR_2:{
               data->u = 11;
          }
          break;
          default:
          std::cout << " Load from address " << addr << std::endl;
          break;
     }
}

int main() {
    /* set up teenyat to set*/
    bool fail = false;
    teenyat t;
    std::string file_string = "./test_code/instructions/LOD/LOD.bin";
    properPath(file_string);
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
    if(t.reg[TNY_REG_A].u != 10){
          printf(FAILURE_MSG);
          fail = true;
    }

    /* this kills the 3 dealay cycles */
    tny_clock(&t);
    tny_clock(&t);
    tny_clock(&t);

    tny_clock(&t);
    tny_clock(&t);
    if(t.reg[TNY_REG_A].u != 11){
          printf(FAILURE_MSG);
          fail = true;
    }

    /* this kills the 3 dealay cycles */
    tny_clock(&t);
    tny_clock(&t);
    tny_clock(&t);
    
    tny_clock(&t);
    tny_clock(&t);
    if(t.reg[TNY_REG_B].u != 0){
        printf(FAILURE_MSG);
        fail = true;
    }
    
    if(!fail){
          printf(SUCCESS_MSG);
    }

    return 0;
}