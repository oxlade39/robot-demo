#ifndef _robot_normals_h_
#define _robot_normals_h_

GLfloat normals[6][3]; /* = { {0.0,0.0,1.0},
{0.0,0.0,-1.0},
{-1.0,0.0,0.0},	 	 	 	 	 	 
{1.0,0.0,0.0},
{0.0,1.0,0.0},
{0.0,-1.0,0.0} }; */


enum { FRONT_NORMAL, BACK_NORMAL, LEFT_NORMAL, RIGHT_NORMAL, TOP_NORMAL, BOTTOM_NORMAL };

#endif