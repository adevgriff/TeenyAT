#include <iostream>

#include "teenyat.h"
#include "test.h"
#include "utils.h"

#define TEST_ADDR 0x8000
#define TEST_ADDR_2 0x8001

bool fail = false;
void bus_write(teenyat *t, tny_uword addr, tny_word data, uint16_t *delay) {
     switch(addr){
          case TEST_ADDR:{  
            if(data.u != 5){
                 printf(FAILURE_MSG);
                 fail = true;
            }
          }
          break;
          case TEST_ADDR_2:{
               if(data.u != 4){
                 printf(FAILURE_MSG);
                 fail = true;
                }
          }
          break;
          default:
          std::cout << " Store to address " << addr << std::endl;
          break;
     }
}
void bus_read(teenyat *t, tny_uword addr, tny_word *data, uint16_t *delay) {
}

int main() {
    /* set up teenyat to set*/
    teenyat t;
    std::string file_string = "./test_code/instructions/STR/STR.bin";
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

    /* Kill 4 delay cycles */
    tny_clock(&t);
    tny_clock(&t);
    tny_clock(&t);
    tny_clock(&t);

    tny_clock(&t);
    tny_clock(&t);
    tny_clock(&t);

     /* Kill 4 delay cycles */
    tny_clock(&t);
    tny_clock(&t);
    tny_clock(&t);
    tny_clock(&t);

    tny_clock(&t);
    if(t.ram[t.reg[TNY_REG_A].u].u != t.reg[TNY_REG_B].u){
            printf(FAILURE_MSG);
            fail = true;
    }

    //std::cout << t.delay_cycles << std::endl;

    if(!fail) printf(SUCCESS_MSG);

    return 0;
}