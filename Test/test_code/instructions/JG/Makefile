run:
ifeq ($(OS),Windows_NT)
	..\..\..\..\tnasm\tnasm JG.asm
	g++ -o run -I..\..\..\.. -I..\..\.. ..\..\..\..\teenyat.c ..\..\..\utils.cpp JG_test.cpp
else
	../../../../tnasm/tnasm JG.asm
	g++ -o run -I../../../.. -I../../.. ../../../../teenyat.c ../../../utils.cpp JG_test.cpp
endif
