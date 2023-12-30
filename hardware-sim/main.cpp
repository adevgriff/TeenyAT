#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

#include "tigr.h"
#include "teenyat.h"

#define CLOCK_PIN (16) //this pin indicates clock cycles
#define LOW (0)
#define HIGH (1)

#define PIXEL_SIZE (10)
#define WIND_WIDTH (640)
#define WIND_HEIGHT (170) //16 led height for matrix + 1 for clock indicator

// Define a no-op instruction
#ifdef _WIN32
#define NOP __asm nop
#else
#define NOP __asm__("nop")
#endif

void platformIndependentDelay(int milliseconds)
{
   std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void bus_read(teenyat *t, tny_uword addr, tny_word *data, uint16_t *delay);
void bus_write(teenyat *t, tny_uword addr, tny_word data, uint16_t *delay);

/*pin here is 0-15 since they are not real pins on a pico board*/
bool pins[17];

/*Depending on how the system is put together this exists but could be hardware shift register*/
bool pin_matrix[64][16];

void digitalWrite(int pin, int value) {
   if(pin > 16) {
      std::cerr << "Write to invalid pin: " << pin << std::endl;
   }
   else {
      pins[pin] = static_cast <bool> (value > 0);
   }
}

void shiftRegisterClock() {
   for(int i = 62; i >= 0; i--) {
      for(int j = 0; j < 16; j++) {
         pin_matrix[i + 1][j] = pin_matrix[i][j];
      }
   }

   for(int i = 0; i < 16; i++) {
      pin_matrix[0][i] = pins[i];
   }
}

void drawSystem(Tigr *window) {

   TPixel offColor = tigrRGBA(0, 0, 0, 255); // black background
   TPixel onColor = tigrRGBA(100, 100, 255, 255); //light blue lights

   /*Draw clock status*/
   tigrFill(window, 0, 0, PIXEL_SIZE, PIXEL_SIZE, (pins[CLOCK_PIN])? onColor : offColor);

   for (int i = 63; i >= 0; i--) {
      /*draw individual pixels in matrix in decending order*/
      for(int j = 0; j < 16; j++) {
         tigrFill(window, (63 - i) * PIXEL_SIZE, j * PIXEL_SIZE + PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, (pin_matrix[i][j]) ? onColor : offColor);
      }
   }

   return;
}

int main(int argc, char *argv[]) {

   Tigr *window;
   TPixel clearColor = tigrRGBA(0xdd, 0xbb, 0xff, 255);//database fab

   for(int i = 0; i < 64; i++) {
      for(int j = 0; j < 16; j++) {
         pin_matrix[i][j] = rand() % 2;
      }
   }

   srand(time(nullptr));
   window = tigrWindow(WIND_WIDTH, WIND_HEIGHT, "Hardware Sim", 0);
   tigrClear(window, clearColor);
   tigrUpdate(window);

   bool quit = false;

   if(argc < 2) {
      std::cerr << "Please provide a tnyAt binary file." << std::endl;
      return EXIT_FAILURE;
   }

   std::string fileName = argv[1];
   FILE *bin_file = fopen(fileName.c_str(), "rb");
   if (bin_file == NULL)
   {
      std::cerr << "Please provide a valid tnyAt binary file." << std::endl;
      return EXIT_FAILURE;
   }

   teenyat tnyAt;
   tny_init_from_file(&tnyAt, bin_file, bus_read, bus_write);

   while (!tigrClosed(window) && !tigrKeyDown(window, TK_ESCAPE))
   {
      digitalWrite(CLOCK_PIN, static_cast<int>(!pins[CLOCK_PIN]));
      tny_clock(&tnyAt);
      tigrClear(window, clearColor);
      drawSystem(window);
      tigrUpdate(window);
      platformIndependentDelay(5);
   }

   tigrFree(window);

   return EXIT_SUCCESS;
}

void bus_read(teenyat *t, tny_uword addr, tny_word *data, uint16_t *delay)
{
   data->u = 0;
   for(int i = 15; i > 0; i--) {
      data->u += static_cast <uint16_t> (pins[i]);
      data->u <<= 1;
   }
   data->u += static_cast<uint16_t>(pins[0]);

   return;
}

void bus_write(teenyat *t, tny_uword addr, tny_word data, uint16_t *delay)
{
   std::cout << "write to address " << addr << " with data " << data.u << std::endl;
   for (int i = 0; i < 16; i++)
   {
      if ((addr >> i) % 2)
      {
         digitalWrite(i, HIGH);
      }
      else
      {
         digitalWrite(i, LOW);
      }
   }

   shiftRegisterClock();

   return;
}