#include "ex1.h"
#include <stdio.h>

#define DEFINING_CLASS Point

defclass_private
	int x;
	int y;
endclass_private

decmethod(int, getX);
decmethod(void, setX, int value);
decmethod(int, getY);
decmethod(void, setY, int value);
decfunction(void, print, Point *p);

defconstructor(int x, int y)
	private->x = x;
	private->y = y;
	
	this->getX = Point_getX;
	this->setX = Point_setX;
	this->getY = Point_getY;
	this->setY = Point_setY;
	this->print = Point_print;
endconstructor

defmethod(int, getX)
	return private->x;
endmethod

defmethod(void, setX, int value)
	private->x = value;
endmethod

defmethod(int, getY)
	return private->y;
endmethod

defmethod(void, setY, int value)
	private->y = value;
endmethod

deffunction(void, print, Point *p)
	printf("X: %d, Y: %d\n", 
		p->private->x, 
		p->private->y);
endfunction

defdestructor
enddestructor

#undef DEFINING_CLASS
