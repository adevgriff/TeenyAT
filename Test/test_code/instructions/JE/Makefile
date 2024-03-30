run:
ifeq ($(OS),Windows_NT)
	..\..\..\..\tnasm\tnasm JE.asm
	g++ -o run -I..\..\..\.. -I..\..\.. ..\..\..\..\teenyat.c ..\..\..\utils.cpp JE_test.cpp
else
	../../../../tnasm/tnasm JE.asm
	g++ -o run -I../../../.. -I../../.. ../../../../teenyat.c ../../../utils.cpp JE_test.cpp
endif
