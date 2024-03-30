run:
ifeq ($(OS),Windows_NT)
	..\..\..\..\tnasm\tnasm JNE.asm
	g++ -o run -I..\..\..\.. -I..\..\.. ..\..\..\..\teenyat.c ..\..\..\utils.cpp JNE_test.cpp
else
	../../../../tnasm/tnasm JNE.asm
	g++ -o run -I../../../.. -I../../.. ../../../../teenyat.c ../../../utils.cpp JNE_test.cpp
endif
