/*
 *  shapes.c
 *  robot
 *
 *  Created by dan on 05/06/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <GLUT/GLUT.h>
#include "shapes.h"
#include "normals.h"


void square(GLfloat *pt1, GLfloat *pt2, GLfloat *pt3, GLfloat *pt4, GLfloat *ratio, GLfloat *normal)
{
	GLfloat STEP_SIZE = 0.5;
	GLfloat f = 0;
	
	glBegin(GL_POLYGON);;
	glNormal3fv(normal);				
	
	glTexCoord2f(0., 0.); glVertex3fv(pt1);
	
	glTexCoord2f(ratio[0], 0.); glVertex3fv(pt2);
	
	glTexCoord2f(ratio[0], ratio[1]); glVertex3fv(pt3);
	
	glTexCoord2f(0., ratio[1]); glVertex3fv(pt4);
	glEnd();
	
}

// Draw a cubiod using function square() given the top back left point and the dimensions
void rectangle(GLfloat* topBackLeft, GLfloat w, GLfloat h, GLfloat d, GLfloat* ratios)
{
	GLfloat topFrontLeft[3] = {topBackLeft[0], topBackLeft[1], topBackLeft[2]+d};
	GLfloat topFrontRight[3] = {topBackLeft[0]+w, topBackLeft[1], topBackLeft[2]+d};
	GLfloat topBackRight[3] = {topBackLeft[0]+w, topBackLeft[1], topBackLeft[2]};
	
	GLfloat bottomBackLeft[3] = {topBackLeft[0], topBackLeft[1]+h, topBackLeft[2]};
	GLfloat bottomFrontLeft[3] = {bottomBackLeft[0], bottomBackLeft[1], bottomBackLeft[2]+d};
	GLfloat bottomFrontRight[3] = {bottomBackLeft[0]+w, bottomBackLeft[1], bottomBackLeft[2]+d};	
	GLfloat bottomBackRight[3] = {bottomBackLeft[0]+w, bottomBackLeft[1], bottomBackLeft[2]};
	
	glPushMatrix();
	glRotatef(180,1.0,0.0,0.0); //compensate for fact that drawn upside down
	
	glBegin(GL_QUAD_STRIP);
	//left
	glNormal3fv(normals[LEFT_NORMAL]);	  	  	  	  	  	  	  	  	  
	glTexCoord2f(0.0, ratios[1]); glVertex3fv(bottomBackLeft);	    	
	glTexCoord2f(0.0, 0.0); glVertex3fv(topBackLeft);	     	 
	glTexCoord2f(ratios[0], ratios[1]); glVertex3fv(bottomFrontLeft);	    	 
	glTexCoord2f(ratios[0], 0.0); glVertex3fv(topFrontLeft);	     	 
	
	//front
	glNormal3fv(normals[FRONT_NORMAL]);	   	   	   	   	   	   	   	   	   
	glTexCoord2f(0., ratios[1]); glVertex3fv(bottomFrontRight);	    
	glTexCoord2f(0., 0.); glVertex3fv(topFrontRight);	 	 	 
	
	//right
	glNormal3fv(normals[RIGHT_NORMAL]);	 	 
	glTexCoord2f(ratios[0], ratios[1]); glVertex3fv(bottomBackRight);	    	
	glTexCoord2f(ratios[0], 0.); glVertex3fv(topBackRight);	 
	
	//back
	glNormal3fv(normals[BACK_NORMAL]);		
	glTexCoord2f(0., ratios[1]); glVertex3fv(bottomBackLeft);	        	
	glTexCoord2f(0., 0.); glVertex3fv(topBackLeft);		 	 
	glEnd();
	
	//bottom
	square(topFrontLeft, topFrontRight, topBackRight, topBackLeft, ratios, normals[BOTTOM_NORMAL]);
	//top
	square(bottomBackLeft, bottomFrontLeft, bottomFrontRight, bottomBackRight, ratios, normals[TOP_NORMAL]);
	
	glPopMatrix();	   
}

void triangle(GLfloat *pt1, GLfloat *pt2, GLfloat *pt3, GLfloat *col, GLfloat *normal)
{
	
	/* draw a polygon via list of vertices */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col);
	
	glBegin(GL_TRIANGLES);
	//	  	  glNormal3fv(normal);
	
	glVertex3fv(pt1);	 	 
	glVertex3fv(pt2);	 	 
	glVertex3fv(pt3);
	glEnd();
}

void textCylinder(GLfloat radius, GLfloat height)
{
	GLUquadric *cylinder =  gluNewQuadric();
	
	glPushMatrix();
	glRotatef(90,1.0,0.0,0.);
	gluQuadricTexture(cylinder, GL_TRUE);
	gluCylinder(cylinder, radius, radius*0.5, height, 20, 20);
	glPopMatrix();
}

void textSphere(GLfloat radius)
{
	GLUquadric *sphere =  gluNewQuadric();
	
	glPushMatrix();
	gluQuadricTexture(sphere, GL_TRUE);
	gluSphere(sphere, radius, 20, 20);
	glPopMatrix();
}
