#ifndef ex1_h_
#define ex1_h_

#include "../src/Class.h"

#define DEFINING_CLASS Point

defclass(Point)
	classmethod(int, getX);
	classmethod(void, setX, int value);
	classmethod(int, getY);
	classmethod(void, setY, int value);
	
	classfunction(void, print, struct Point *p);
endclass

decconstructor(Point);
decdestructor(Point);
#undef DEFINING_CLASS

#endif /*ex1_h_*/
