run:
ifeq ($(OS),Windows_NT)
	..\..\..\..\tnasm\tnasm LOD.asm
	g++ -o run -I..\..\..\.. -I..\..\.. ..\..\..\..\teenyat.c ..\..\..\utils.cpp LOD_test.cpp
else
	../../../../tnasm/tnasm LOD.asm
	g++ -o run -I../../../.. -I../../.. ../../../../teenyat.c ../../../utils.cpp LOD_test.cpp
endif
