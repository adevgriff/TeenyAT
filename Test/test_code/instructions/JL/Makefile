run:
ifeq ($(OS),Windows_NT)
	..\..\..\..\tnasm\tnasm JL.asm
	g++ -o run -I..\..\..\.. -I..\..\.. ..\..\..\..\teenyat.c ..\..\..\utils.cpp JL_test.cpp
else
	../../../../tnasm/tnasm JL.asm
	g++ -o run -I../../../.. -I../../.. ../../../../teenyat.c ../../../utils.cpp JL_test.cpp
endif
