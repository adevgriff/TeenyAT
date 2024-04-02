run:
ifeq ($(OS),Windows_NT)
	..\..\..\..\tnasm\tnasm STR.asm
	g++ -o run -I..\..\..\.. -I..\..\.. ..\..\..\..\teenyat.c ..\..\..\utils.cpp STR_test.cpp
else
	../../../../tnasm/tnasm STR.asm
	g++ -o run -I../../../.. -I../../.. ../../../../teenyat.c ../../../utils.cpp STR_test.cpp
endif
