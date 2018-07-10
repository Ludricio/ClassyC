#include "ex1.h"
#include <stdio.h>

int main()
{
	Point *p = Point_new(1, 5);
	
	printf("Point value after construction:\n");
	p->print(p);
	
	int oldX = p->getX(p);
	int oldY = p->getY(p);

	

	p->setX(p, oldY);
	p->setY(p, oldX);

	printf("Point value after new values set\n");
	p->print(p);

	Point_destroy(p);
	return 0;
}
