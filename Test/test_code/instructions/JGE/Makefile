run:
ifeq ($(OS),Windows_NT)
	..\..\..\..\tnasm\tnasm JGE.asm
	g++ -o run -I..\..\..\.. -I..\..\.. ..\..\..\..\teenyat.c ..\..\..\utils.cpp JGE_test.cpp
else
	../../../../tnasm/tnasm JGE.asm
	g++ -o run -I../../../.. -I../../.. ../../../../teenyat.c ../../../utils.cpp JGE_test.cpp
endif
