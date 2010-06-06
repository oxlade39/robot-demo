#ifndef _robot_shapes_h_
#define _robot_shapes_h_

void square(GLfloat *pt1, GLfloat *pt2, GLfloat *pt3, GLfloat *pt4, GLfloat *ratios, GLfloat *normal);

void rectangle(GLfloat *topBackLeft, GLfloat w, GLfloat h, GLfloat d, GLfloat *ratios);

void triangle(GLfloat *pt1, GLfloat *pt2, GLfloat *pt3, GLfloat *col, GLfloat *normal);

void textCylinder(GLfloat radius, GLfloat height);

void textSphere(GLfloat radius);

#endif