run:
ifeq ($(OS),Windows_NT)
	..\..\..\..\tnasm\tnasm JLE.asm
	g++ -o run -I..\..\..\.. -I..\..\.. ..\..\..\..\teenyat.c ..\..\..\utils.cpp JLE_test.cpp
else
	../../../../tnasm/tnasm JLE.asm
	g++ -o run -I../../../.. -I../../.. ../../../../teenyat.c ../../../utils.cpp JLE_test.cpp
endif
