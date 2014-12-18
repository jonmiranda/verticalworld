#ifndef __L08__Image__
#define __L08__Image__

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif

/*data structure for the image used for  texture mapping */
typedef struct Image {
	int sizeX;
	int sizeY;
	char *data;
} Image;

typedef struct RGB {
	GLubyte r;
	GLubyte g;
	GLubyte b;
} RGB;

int ImageLoad(const char *filename, Image *image);

#endif /* defined(__L08__Image__) */
