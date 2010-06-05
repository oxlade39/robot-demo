#ifndef _robot_main_h_
#define _robot_main_h_

#include <GLUT/glut.h>

#define Width 256
#define Height 256
#define Length Width*Height*3

#define Width2 128
#define Height2 128
#define Length2 Width2*Height2*3

#define X 0;
#define Y 1;
#define Z 2;
#define LINES 1;
#define POINTS 2;

#define texNum 5

#define TRUE 1
#define FALSE 0
#define DEG2RAD 6.28/360

#define HEIGHT_MAP_SIZE 1024

unsigned char *my_texture[texNum];
unsigned char *my_map;
float scaleValue = 0.15f;// Scale Value For The Terrain ( NEW )

void moveMeFlat(int direction);

#endif