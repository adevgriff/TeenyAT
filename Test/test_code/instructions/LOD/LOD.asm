.const TEST_ADDR 0x8000

lod rA, [TEST_ADDR] ; should be 10
set rB, 1   
lod rA, [rB + TEST_ADDR] ; should be 11

lod rB, [ra] ; this is a load from an address inside the teenyAT not over the BUS
             ; however this address [11] is outside of programers rom so the result will be zero