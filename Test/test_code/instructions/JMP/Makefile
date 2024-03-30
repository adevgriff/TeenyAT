run:
ifeq ($(OS),Windows_NT)
	..\..\..\..\tnasm\tnasm JMP.asm
	g++ -o run -I..\..\..\.. -I..\..\.. ..\..\..\..\teenyat.c ..\..\..\utils.cpp JMP_test.cpp
else
	../../../../tnasm/tnasm JMP.asm
	g++ -o run -I../../../.. -I../../.. ../../../../teenyat.c ../../../utils.cpp JMP_test.cpp
endif
