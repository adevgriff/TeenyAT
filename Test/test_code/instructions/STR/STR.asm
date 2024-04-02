.const TEST_ADDR 0x8000

set rA, 5
str [TEST_ADDR], rA ; should be 5

set rA, 4
set rB, 1   
str [rB + TEST_ADDR], rA ; should be 11

str [rA + 0], rB ; this is a store from an address inside the teenyAT not over the BUS
             ; however this will result in a binary of PC = PC + 1