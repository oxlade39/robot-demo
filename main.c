#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GLUT/glut.h>

GLfloat colors[][3] = {{1.0,1.0,1.0},{1.0,0.0,0.0},
{1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}, 
{1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};

GLfloat normals[][3] = { {0.0,0.0,1.0},
{0.0,0.0,-1.0},
{-1.0,0.0,0.0},	 	 	 	 	 	 
{1.0,0.0,0.0},
{0.0,1.0,0.0},
{0.0,-1.0,0.0} };

GLfloat LightAmbient[]= { 0.25, 0.25, 0.25, 1.0 }; /* some half intensity white ambient light */
GLfloat LightDiffuse[]= { 0.75, 0.75, 0.75, 1.0 }; /* A directional difues light  w=0 = directional*/
GLfloat LightPosition[][4]= { { 0.0, 0.0, 1.0, 1.0 }, /* x,y,z co-ords for pos */
{0.0, 0.9, 0.0, 1.0}  }; /* x,y,z co-ords for direction */

GLfloat zero[] = {0.0, 0.0, 0.0, 1.0};
GLfloat mat[] = {1.0, 1.0, 1.0, 1.0};
GLfloat shininess[] = {70.0};                /*(range: dull 0 100 very shiny 128)*/
GLfloat shininess2[] = {70.0};                /*(range: dull 0 100 very shiny 128)*/


GLfloat lookat[] = {0.0,1.0,0.0};
int lastx = 0;
int lasty = 0;

GLuint Textures[5];

int mode =0;
GLfloat xrot = 0;
GLfloat yrot = 0;
GLfloat zoom = 0;
GLfloat side = 0;

GLint textname[texNum];


GLfloat displacement = 0.0;

enum { HIP_JOINT = 0, KNEE_JOINT, ELBOW_JOINT, TOP_LEG_SEPARATION , SHOULDER_JOINT, SHOULDER_JOINT2, SHOULDER_JOINT3};   
GLfloat joints[7][2] = { { 0.0, 0.0 },
{ 0.0, 0.0 },
{ 10.0, -190.0 },
{ 5.0, 5.0 },
{ 15.0, 15.0}, 
{ 0.0, 0.0},
{ 0.0, 0.0}};

GLfloat robotYRot = 0.0;


int start = TRUE;
int walkingLeg = 0;
int upCycle = TRUE;
int lighting = TRUE;
int walking = TRUE;
int frontstance = FALSE;
int jointSelected = 0;

GLfloat angle=0.0,ratio;
GLfloat x=0.0,y=0.0,z=0.0;
GLfloat lx=0.0,ly=0.0,lz=-1.0;

int pos[][2] = { {0,0},
{0,0} };

GLfloat flyby = 0.0;
int displayWorldSequence = TRUE;
GLfloat flybyZoom = 0.0;
GLfloat flybyRot = 0.0;
GLfloat pictureMode = 0.0;
GLfloat currentdisplacement = 0.0;
GLfloat wave = 45.0;
int waveTurn = TRUE;

GLuint terrainDL = 1;
GLuint worldDL = 2;

void loadtexture(char *name, unsigned char *data, int w, int h, int le)
{
	FILE *f;
	int i;
	int result;
	f=fopen(name, "rb");
	i=fread(data, 1, le, f);
	printf("%d\n", i);
	
	// After We Read The Data, It's A Good Idea To Check If Everything Read Fine
	result = ferror( f );
	if (result) printf("read file error");
	
	fclose(f);
	
	
	//for(i=0; i<20; i++)
	//	  printf("%d\n", (int) data[i]);
}

void lights(float x,float y, float z){
    GLfloat pos[4] = {0.0, 0.0, 0.0, 1.0};
	GLfloat pos2[4] = {0.0, 0.0, 0.0, 1.0};	   //direction when w=0;
    GLfloat ld1[] = {0.75, 0.75, 0.75, 1.};
    GLfloat ls1[] = {1.0, 1.0, 1.0, 1.};
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    glLightfv(GL_LIGHT0, GL_POSITION, pos);/* The default light is "infinite"; a local light is important to ensure varying lighting color calculations at the vertices. */
    glEnable(GL_LIGHT0);
    pos2[0] = x;
    pos2[1] = y;
    pos2[2] = z;
    glLightfv(GL_LIGHT1, GL_POSITION, pos2);
	// must secify diffuse and specular parameters for all lights above 0
    glLightfv(GL_LIGHT1, GL_SPECULAR, ls1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, ld1);
    glEnable(GL_LIGHT1);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ld1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ls1);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);
	
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.01);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.00);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.00);
	
	// draw white cube where lights are
	glDisable(GL_TEXTURE_2D); glDisable(GL_LIGHTING);
	glColor3f(1.0,1.0,1.0);
	glutSolidCube(0.1);
	glEnable(GL_TEXTURE_2D); glEnable(GL_LIGHTING);
	
	
}

void check()
{
	
	/* ErrorCode 
	 
	 #define GL_NO_ERROR                       0
	 #define GL_INVALID_ENUM                   0x0500
	 #define GL_INVALID_VALUE                  0x0501
	 #define GL_INVALID_OPERATION              0x0502
	 #define GL_STACK_OVERFLOW                 0x0503
	 #define GL_STACK_UNDERFLOW                0x0504
	 #define GL_OUT_OF_MEMORY                  0x0505
	 
	 */
	int err;
	err =glGetError();
	if(err == GL_NO_ERROR)
		return;
	printf("Gl Error %x\n", err);
	exit(0);
}

void bindAllTex()
{
	my_texture[0] = (unsigned char *) malloc(Length);
	my_texture[1] = (unsigned char *) malloc(Length2);
	my_texture[2] = (unsigned char *) malloc(Length2);
	my_texture[3] = (unsigned char *) malloc(Length2);
	my_texture[4] = (unsigned char *) malloc(Length2);
	//printf("%d %d\n", my_texture, Length);
	loadtexture("wall2.raw", my_texture[0],Width, Height, Length);
	loadtexture("woodenfloor.raw", my_texture[1], Width2, Height2, Length2);
	loadtexture("woodstrut.raw", my_texture[2], Width2, Height2, Length2);
	loadtexture("woodroof.raw", my_texture[3], Width2, Height2, Length2);
	//	  loadtexture("tex/robotmetal.raw", my_texture[4], Width2, Height2, Length2);
	loadtexture("reflective.raw", my_texture[4], Width2, Height2, Length2);
	printf("Starting Textures\n");
	
	// Create 4 textures
	glGenTextures(texNum, &textname[0]);
	
    // Create texture 1
	glBindTexture(GL_TEXTURE_2D, textname[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB,  GL_UNSIGNED_BYTE, my_texture[0]);
	check();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,Width, Height, GL_RGB, GL_UNSIGNED_BYTE, my_texture[0]);
	
	// Create texture 2
	glBindTexture(GL_TEXTURE_2D, textname[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width2, Height2, 0, GL_RGB,  GL_UNSIGNED_BYTE, my_texture[1]);
	check();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,Width2, Height2, GL_RGB, GL_UNSIGNED_BYTE, my_texture[1]);
	
	// Create texture 3
	glBindTexture(GL_TEXTURE_2D, textname[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width2, Height2, 0, GL_RGB,  GL_UNSIGNED_BYTE, my_texture[2]);
	check();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,Width2, Height2, GL_RGB, GL_UNSIGNED_BYTE, my_texture[2]);
	
	// Create texture 4
	glBindTexture(GL_TEXTURE_2D, textname[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width2, Height2, 0, GL_RGB,  GL_UNSIGNED_BYTE, my_texture[3]);
	check();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,Width2, Height2, GL_RGB, GL_UNSIGNED_BYTE, my_texture[3]);
	
	// Create texture 5
	glBindTexture(GL_TEXTURE_2D, textname[4]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width2, Height2, 0, GL_RGB,  GL_UNSIGNED_BYTE, my_texture[4]);
	check();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);;
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,Width2, Height2, GL_RGB, GL_UNSIGNED_BYTE, my_texture[4]);
	
	
	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
}

void bindMaps()
{
	my_map = (unsigned char *) malloc(HEIGHT_MAP_SIZE*HEIGHT_MAP_SIZE);
	loadtexture("terrain2.raw", my_map,HEIGHT_MAP_SIZE, HEIGHT_MAP_SIZE, HEIGHT_MAP_SIZE*HEIGHT_MAP_SIZE);
}

// Using tutorial from NeHe on HightMapping
int height(unsigned char *data, int MapX, int MapY, int MAP_SIZE)
{
	int Mapx;
	int Mapy;
	Mapx = (MapX % MAP_SIZE);
	Mapy = (MapY % MAP_SIZE);
	
	return data[Mapx + (Mapy * MAP_SIZE) ];
}

void SetVertexColor(unsigned char *data, int Mapx, int Mapy, int MAP_SIZE)	   	   	   // This Sets The Color Value For A Particular Index
{																// Depending On The Height Index
	GLfloat fColor = (-0.15 + ((height(data, Mapx, Mapy , MAP_SIZE) / 256.0)));
	// Assign This Blue Shade To The Current Vertex
	//	  printf("setting color to %f\n",fColor);
	if( fColor < 0.25) fColor*=5.0;
	glColor3f( 0.8*fColor, fColor , 0.3*fColor);
}

void renderHeightMap(unsigned char *heightMap, int MAP_SIZE)	 // This Renders The Height Map As Quads
{
	int MapX = 0, MapY = 0;	   	   	  	  	  	  	  	  	  	  // Create Some Variables To Walk The Array With.
	int Mapx, Mapy, Mapz;	 	 	 	 	 	 	 	 	 // Create Some Variables For Readability
	int STEP_SIZE = 15;								// Determines how defined the map is by stepping tru the array
	
	glPushMatrix();
	glNewList (terrainDL, GL_COMPILE);
	glDisable(GL_LIGHTING);
	glBegin( GL_QUADS );							// Render Polygons
	
	for ( MapX = 0; MapX < (MAP_SIZE-STEP_SIZE); MapX += STEP_SIZE )	 	 // loop around the x axis of the map
		for ( MapY = 0; MapY < (MAP_SIZE-STEP_SIZE); MapY += STEP_SIZE )	 // loop around the y axis of the map
		{
			Mapx = MapX;
			Mapy = height(heightMap, MapX, MapY, MAP_SIZE);
			Mapz = MapY;
			
			// bottom left
			SetVertexColor(heightMap, Mapx, Mapz, MAP_SIZE);	 	 	 // Set The Color Value Of The Current Vertex
			glTexCoord2f(MapX/MAP_SIZE,(MapY+STEP_SIZE)/MAP_SIZE ); glVertex3f(Mapx, Mapy, Mapz);	 	 	 	 	 	 	 // Send This Vertex To OpenGL To Be Rendered
			
			// Get The (X, Y, Z) Value For The Top Left Vertex
			Mapx = MapX;
			Mapy = height(heightMap, MapX, MapY + STEP_SIZE , MAP_SIZE);  
			Mapz = MapY + STEP_SIZE ;	 	 	 	 	 	 
			
			SetVertexColor(heightMap, Mapx, Mapz, MAP_SIZE); 	  	  	  // Set The Color Value Of The Current Vertex
			glTexCoord2f(MapX/MAP_SIZE,MapY/MAP_SIZE ); glVertex3f(Mapx, Mapy, Mapz);	   	   	   	   	   	   	   	   	   // Send This Vertex To OpenGL To Be Rendered
			
			// Get The (X, Y, Z) Value For The Top Right Vertex
			Mapx = MapX + STEP_SIZE; 
			Mapy = height(heightMap, MapX + STEP_SIZE, MapY + STEP_SIZE, MAP_SIZE); 
			Mapz = MapY + STEP_SIZE ;
			
			SetVertexColor(heightMap, Mapx, Mapz, MAP_SIZE);  	   	   	   // Set The Color Value Of The Current Vertex
			glTexCoord2f((MapX+STEP_SIZE)/MAP_SIZE,MapY/MAP_SIZE ); glVertex3f(Mapx, Mapy, Mapz);	   	   	   	   	   	   	   	   	   // Send This Vertex To OpenGL To Be Rendered
			
			// Get The (X, Y, Z) Value For The Bottom Right Vertex
			Mapx = MapX + STEP_SIZE; 
			Mapy = height(heightMap, MapX + STEP_SIZE, MapY, MAP_SIZE ); 
			Mapz = MapY;
			
			SetVertexColor(heightMap, Mapx, Mapz, MAP_SIZE);  	   	   	   // Set The Color Value Of The Current Vertex	  	  	  
			glTexCoord2f((MapX+STEP_SIZE)/MAP_SIZE,(MapY+STEP_SIZE)/MAP_SIZE ); glVertex3f(Mapx, Mapy, Mapz);	  	  	  	  	  	  	  	  	  // Send This Vertex To OpenGL To Be Rendered	     	 	 	 	 	 
		}
	
	glEnd();
	glEnable(GL_LIGHTING);
	glEndList ();
	glPopMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);// Reset The Color					
	
}

void displayMenu(void)
{
	printf("\n\nHelp Menu:\n\n");	 
	
	printf("Controls - Anytime:\n");
	printf("i - top of leg up\n");
	printf("o - top of leg down\n");
	printf("y - leg to side\n");
	printf("u - reverse leg to side\n");
	printf("c - lower part of leg up\n");
	printf("x - lower part of leg down\n");
	printf("l - elbow movement\n");
	printf("k - elbow movement oposite\n");
	printf("b - shoulder back\n");
	printf("v - shoulder forward\n");
	printf("g - arm rotation\n");
	printf("f - arm rotation oposite\n");
	printf("m - shoulder rotation\n");
	printf("n - shoulder rotation oposite\n");
	printf("\n");
	printf("in some cases the capital case will cause the oposite limb to move.\n");
	printf("\n");
	printf("t - Tour mode\n");
	printf("p - display picture point\n");
	printf("r - reset all variables to initial\n");
	printf("h - print help text\n");
	printf("q - quit program\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("Controls - Non tour mode only:\n");
	printf("a - rotate view to left\n");
	printf("d - rotate view to right\n");
	printf("w - move forward\n");
	printf("s - move backward\n");
	printf("space - start robot walking\n");
	printf("] - perform taekwondo front stance\n");
	printf("\n coding by Dan Oxlade of Southampton University 2003");	
	
	printf("\n\nEnd Help Menu\n\n");		
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

void drawFloor(GLfloat *pt1, GLfloat *pt2, GLfloat *pt3, GLfloat *pt4, GLfloat *col)
{
	
	glBegin(GL_POLYGON);
	glNormal3f(0.0,1.0,0.0);
	
	glTexCoord2f(0., 0.); glVertex3fv(pt1);
	
	glTexCoord2f(50., 0.); glVertex3fv(pt2);
	
	glTexCoord2f(50., 20.); glVertex3fv(pt3);
	
	glTexCoord2f(0., 20.); glVertex3fv(pt4);
	glEnd();
	
}

void pyramid(GLfloat *point, GLfloat h, GLfloat w, GLfloat d)
{
	GLfloat p1[] = { 	  (point[0]-(w/2)),
		(point[1]-h),
		(point[2]+(d/2))
	};
	GLfloat p2[] =	{
		p1[0],
		p1[1],
		p1[2]-d
	};
	GLfloat p3[] =	{
		p2[0]+w,
		p1[1],
		p2[2]
	};
	GLfloat p4[] =	{
		p3[0],
		p3[1],
		p1[2]
	};
	GLfloat normalFront[] = {0.0,0.0,1.0};
	GLfloat normalBack[] = {0.0,0.0,-1.0};
	GLfloat normalLeft[] = {-1.0,0.0,0.0};
	GLfloat normalRight[] = {1.0,0.0,1.0};
	
	
	triangle(point, p1, p4, colors[1], normalFront);
	triangle(point, p1, p2, colors[2], normalLeft);
	triangle(point, p2, p3, colors[3], normalBack);
	triangle(point, p3, p4, colors[4], normalRight);
	square(p1,p2,p3,p4,colors[5], normalFront);
}

// Draw a cubiod using function square() given the top back left point and the dimensions
void rectangle(GLfloat *topBackLeft, GLfloat w, GLfloat h, GLfloat d, GLfloat *ratio)
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
	glTexCoord2f(0., ratio[1]); glVertex3fv(bottomBackLeft);	    	
	glTexCoord2f(0., 0.); glVertex3fv(topBackLeft);	     	 
	glTexCoord2f(ratio[0], ratio[1]); glVertex3fv(bottomFrontLeft);	    	 
	glTexCoord2f(ratio[0], 0.); glVertex3fv(topFrontLeft);	     	 
	
	//front
	glNormal3fv(normals[FRONT_NORMAL]);	   	   	   	   	   	   	   	   	   
	glTexCoord2f(0., ratio[1]); glVertex3fv(bottomFrontRight);	    
	glTexCoord2f(0., 0.); glVertex3fv(topFrontRight);	 	 	 
	
	//right
	glNormal3fv(normals[RIGHT_NORMAL]);	 	 
	glTexCoord2f(ratio[0], ratio[1]); glVertex3fv(bottomBackRight);	    	
	glTexCoord2f(ratio[0], 0.); glVertex3fv(topBackRight);	 
	
	//back
	glNormal3fv(normals[BACK_NORMAL]);		
	glTexCoord2f(0., ratio[1]); glVertex3fv(bottomBackLeft);	        	
	glTexCoord2f(0., 0.); glVertex3fv(topBackLeft);		 	 
	glEnd();
	
	//bottom
	square(topFrontLeft, topFrontRight, topBackRight, topBackLeft, ratio, normals[BOTTOM_NORMAL]);
	//top
	square(bottomBackLeft, bottomFrontLeft, bottomFrontRight, bottomBackRight, ratio, normals[TOP_NORMAL]);
	
	glPopMatrix();	   
}

void drawStrut()
{
	//	  GLfloat bottom[4][3] = {{-8.0,-1.0,8.0},{-8.0,-1.0,7.5},{-7.5,-1.0,7.5},{-7.5,-1.0,8.0}};
	//	  GLfloat top[4][3] = {{-8.0,1.0,8.0},{-8.0,1.0,7.5},{-7.5,1.0,7.5},{-7.5,1.0,8.0}};
	
	GLfloat pos[] = {0.0,-1.0,0.0};
	GLfloat ratio[] = {1.0,1.0};
	
	glPushMatrix();
	glRotatef(-180.0, 1.0, 0.0, 0.0);	 // front faces intoward the room
	rectangle(pos, 0.5, 2, 0.5, ratio);
	glPopMatrix();
}

void CreateWorld()
{
	GLfloat ratio[2] = {10.0,1.0};
	GLfloat roofratio[2] = {10.0,10.0};
	GLfloat none[] = {0.0, 0.0, 0.0, 0.0};
	GLfloat full[] = {1.0, 1.0, 1.0, 1.0};
	
	GLfloat v0[] = {-10.0,-1.0,-10.0};
	GLfloat v1[] = {-10.0,1.0,-10.0};
	GLfloat v2[] = {-10.0,-1.0,10.0};
	GLfloat v3[] = {-10.0,1.0,10.0};
	
	GLfloat v4[] = {10.0,-1.0,10.0};
	GLfloat v5[] = {10.0,1.0,10.0};
	GLfloat v6[] = {10.0,-1.0,-10.0};
	GLfloat v7[] = {10.0,1.0,-10.0};
	
	
	GLfloat WallTL[] = {-10.0,1.0,-10.0};
	GLfloat WallTR[] = {10.0,1.0,-10.0};
	GLfloat WallBR[] = {10.0,-1.0,-10.0};
	GLfloat WallBL[] = {-10.0,-1.0,-10.0};
	
	int i = -9 ;
	GLfloat j = 0.;
	
	glNewList (worldDL, GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.0,0.9,2.);	// translate to light pos
	lights(0, 0.98, 0.);
	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.005);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.005);
	
	glPopMatrix();
	
	// wall texture
	glBindTexture(GL_TEXTURE_2D, textname[0]);
	
	glBegin(GL_QUAD_STRIP);
	//left
	glNormal3fv(normals[RIGHT_NORMAL]);
	
	glTexCoord2f(0., ratio[1]); glVertex3fv(v0);	    	
	glTexCoord2f(0., 0.); glVertex3fv(v1);    	  	 	    	
	
	for(; i<=10; i++)
	{	 	 
		j+=0.5;	   	   	   	   
		glTexCoord2f(j, ratio[1]); glVertex3f(v2[0],v2[1],i);	             	  
		glTexCoord2f(j, 0.); glVertex3f(v3[0],v3[1],i);	 	 	 	 	 	 	 
	}	 	 	 	 	 
	i = -9 ;
	//front
	glNormal3fv(normals[BACK_NORMAL]);
	for(; i<=10; i++)
	{	 	 
		j+=0.5;	   	   	   	   
		glTexCoord2f(j, ratio[1]); glVertex3f(i,v4[1],v4[2]);	              	   
		glTexCoord2f(j, 0.); glVertex3f(i,v5[1],v5[2]);							
	}
	//right
	i = 9 ;
	glNormal3fv(normals[LEFT_NORMAL]);		
	for(; i>=-10; i--)
	{	 	 
		j+=0.5;	   	   	   	   
		glTexCoord2f(j, ratio[1]); glVertex3f(v6[0],v6[1],i);	             	  
		glTexCoord2f(j, 0.); glVertex3f(v7[0],v7[1],i);	 	 	 	 	 	 	 		   	   	   	   	   	   
	}  	    	
	glEnd();
	
	// restore defaults
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.01);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, full);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);
	
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.005);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0);
	
	// Floor
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, textname[1]);
	WallTL[0] = -10.0; WallTL[1] = -1.0; WallTL[2] = -10.0;
	WallTR[0] = 10.0; WallTR[1] = -1.0; WallTR[2] = -10.0;;
	WallBR[0] = 10.0; WallBR[1] = -1.0; WallBR[2] = 10.0;
	WallBL[0] = -10.0; WallBL[1] = -1.0; WallBL[2] = 10.0;;
	drawFloor(WallTL, WallTR, WallBR, WallBL, colors[1]);
	//Roof
	glTranslatef(0.0,2.0,0.0);
	glBindTexture(GL_TEXTURE_2D, textname[3]);
	square(WallTL, WallTR, WallBR, WallBL, roofratio, normals[TOP_NORMAL]);
	glPopMatrix();
	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.005);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.005);
	
	// The vertical struts
	glBindTexture(GL_TEXTURE_2D, textname[2]);
	glPushMatrix();
	glTranslatef(-8.0, 0.0, 8.0);
	i = 0;
	for(; i<5; i++)
	{
		glPushMatrix();	   
		glPushMatrix();
		glRotatef(180, 1.0, 0.0, 0.0);	// one side has oposite lighting to the other  
		glRotatef(180, 0.0, 1.0, 0.0);	// one side has oposite lighting to the other  
		drawStrut();		// right side
		glPopMatrix();
		glPushMatrix();
		glTranslatef(16.0, 0.0, 0.0);	 	 	 
		drawStrut();		// left side
		glPopMatrix();	   	   	   
		glPopMatrix();	  	  	  
		
		glTranslatef(0.0, 0.0, -4.0);
	}
	
	glPopMatrix();	//return to position
	glPushMatrix();
	glTranslatef(-10.0,-5.0,-60.0);
	glScalef(0.02, 0.02 * 1.5, 0.05);;
	//	  glCallList (terrainDL);
	glPopMatrix();
	glEndList();
	
}

void robot()
{
	GLfloat ratio[2] = {1.0,1.0};
	GLfloat ratio2[2] = {0.15,1.0};
	GLfloat upperLeg[3] = {0.05,0.25,0.05};
	GLfloat lowerLeg[3] = {0.05,0.25,0.05};
	GLfloat body[3] = {0.25,0.3,0.1};
	GLfloat upperArm[3] = {0.04,0.15,0.05};			// think of as y,x,z after rotation
	GLfloat neck[2] = {0.05, 0.03};
	GLfloat headRad = 0.07;
	
	GLfloat ld1[] = {0.3, 0.3, 0.3, 1.0};			// reduce the diffuse light for robot
	GLfloat ld2[] = {0.75, 0.75, 0.75, 1.0};
	GLfloat fullEmission[] = {0.55,0.55,0.55, 1.0};
	GLfloat noEmission[] = {0.0,0.0,0.0, 1.0};
	
	GLfloat rec[3] = {0.0,0.0,0.0};
	
	
	//	  glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);     // Enable spherical
  	glEnable(GL_TEXTURE_GEN_T);     // Environment Mapping
	glBindTexture(GL_TEXTURE_2D, textname[4]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ld1);
	glMaterialfv(GL_FRONT, GL_EMISSION, fullEmission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, noEmission);
	
	glColor3f(0.1,0.1,0.1);
	
	glPushMatrix();
	
	glTranslatef(0.0,0.0,displacement);
	
	//box body
	rectangle(rec,body[0],body[1],body[2], ratio);
	//	  	  glPushMatrix();
	//	  	  	  glScalef(1.0,1.5, 1.0);	 
	//	  	  	  textSphere(body[1]/2);
	//	  	  glPopMatrix();			
	
	glTranslatef((body[0]/2)-upperLeg[0]/2,-body[1],-(body[2]/2));	     			 // translate to bottom center of body.
	glPushMatrix();
	//left leg
	glRotatef(joints[TOP_LEG_SEPARATION][0],0.0,0.0,-1.);	  	  	  	  // separation between legs at hips
	glRotatef(joints[HIP_JOINT][0],-1.0,0.0,0.);	 	 	 	 	  	  	  // hip back/forward
	textCylinder(upperLeg[0]/2, upperLeg[1]);
	
	glTranslatef(0.,-upperLeg[1],0.);	     					// translate by just over the width of the left leg
	glRotatef(joints[KNEE_JOINT][0] - joints[HIP_JOINT][0], -1.0, 0.0, 0.0);	 	 	 	 // knee back/forward
	
	// left Lower leg
	textSphere(lowerLeg[0]/2);			
	textCylinder(lowerLeg[0]/2, lowerLeg[1]);
	
	glPopMatrix();
	
	glRotatef(joints[TOP_LEG_SEPARATION][1],0.0,0.0,1.);				// separation between legs at hips
	glRotatef(joints[HIP_JOINT][1],-1.0,0.0,0.);	 	 	 	 	  	  	  // hip back/forward
	glTranslatef(upperLeg[0],0.,0.);	    	   	   	   // translate by just over the width of the left leg
	//right leg	   
	textCylinder(upperLeg[0]/2, upperLeg[1]);
	
	glTranslatef(0.,-upperLeg[1],0.);	     				// translate by just over the width of the left leg
	glRotatef(joints[KNEE_JOINT][1] - joints[HIP_JOINT][1], -1.0, 0.0, 0.0);	 	 	 	   // knee back/forward
	
	// Right Lower leg
	textSphere(upperLeg[0]/2);	
	textCylinder(upperLeg[0]/2, upperLeg[1]);
	
	glPopMatrix();
	glPushMatrix();
	
	glTranslatef(0.0,0.0,displacement);	 	 	 	 // translate back to body pos
	glTranslatef(0.0,body[1]*-0.1,(body[2]*-0.5)+upperArm[0]/2);		// translate to arm position
	
	glRotatef(joints[SHOULDER_JOINT3][0], -1.0, 0.0, 0.0);		
	glRotatef(joints[SHOULDER_JOINT][0], 0.0, 0.0, -1.0);						// at right angle to body				
	glRotatef(joints[SHOULDER_JOINT2][0], 0.0, -1.0, 0.0);
	
	// left upper arm
	textSphere(upperArm[0]/2);			  
	textCylinder(upperArm[0]/2, upperArm[1]);
	
	glTranslatef(0.0,-upperArm[1],0.0);	  	  // translate to lower arms pos	   
	glRotatef(joints[ELBOW_JOINT][0], 0.0, 0.0, 1.0);			 
	// left lower arm
	textSphere(upperArm[0]/2);
	textCylinder(upperArm[0]/2, upperArm[1]);
	
	glPopMatrix();
	glPushMatrix();
	
	glTranslatef(0.0,0.0,displacement);	  	  	  	  // translate back to body pos
	glTranslatef(body[0],(body[1]*-0.1),(body[2]*-0.5)+upperArm[0]/2);	  	  // translate to arm position
	
	glRotatef(joints[SHOULDER_JOINT3][1], -1.0, 0.0, 0.0);		
	glRotatef(joints[SHOULDER_JOINT][1], 0.0, 0.0, 1.0);	   	   	   	   	   // at right angle to body			  	      		
	glRotatef(joints[SHOULDER_JOINT2][1], 0.0, -1.0, 0.0);
	
	
	// right upper arm
	textSphere(upperArm[0]/2);		
	textCylinder(upperArm[0]/2, upperArm[1]);
	
	glTranslatef(0.0,-upperArm[1],0.0);	      // translate to lower arms pos	   
	glRotatef(180, 1.0, 0.0, 0.0);	    
	glRotatef(joints[ELBOW_JOINT][1], 0.0, 0.0, -1.0);	  	   	   	 	 
	//right lower arm
	textSphere(upperArm[0]/2);		 	 
	textCylinder(upperArm[0]/2, upperArm[1]);
	
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,0.0,displacement);
	glTranslatef((body[0]/2), neck[1], -body[2]/2);
	textCylinder(neck[0]/2, neck[1]);
	glTranslatef(0.0, headRad*0.75, 0.0); 
	textSphere(headRad);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);     // Enable spherical
  	glDisable(GL_TEXTURE_GEN_T);     // Environment Mapping
	
	glColor3f(1.0,1.0,1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ld2);		// return to old lighting
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
	
	
	//	  glDisable(GL_TEXTURE_2D);
	
}

void walk(GLfloat speed)
{
	GLfloat KNEEMAX = 2.0; GLfloat KNEEMIN = 0.0;
	GLfloat HIPMAX = 25; GLfloat HIPMIN = 0.0;
	
	if(walkingLeg>1) walkingLeg = 0;
	
	//	  if(upCycle && knee[walkingLeg] < KNEEMAX) knee[walkingLeg]++;	  	  // turn to move knee up
	//	  else if(upCycle) upCycle = FALSE;	  	  	  	  	  	  	  	  	  // end of move knee up
	//	  else if(knee[walkingLeg] > KNEEMIN) knee[walkingLeg]--;	  	  	  // turn to move down
	//	  else if(knee[walkingLeg] == KNEEMIN) { upCycle = TRUE;}
	
	if(upCycle && joints[HIP_JOINT][walkingLeg] < HIPMAX)	  	  	  	  	  	  	    // turn to move knee up
	{
		joints[HIP_JOINT][walkingLeg] += speed;
		if(joints[KNEE_JOINT][walkingLeg]<0) joints[KNEE_JOINT][walkingLeg] += speed/2;
	}	 
	else if(upCycle) { upCycle = FALSE;	}   	// end of move knee up
	else if(joints[HIP_JOINT][walkingLeg] > HIPMIN) 										// turn to move down
	{ 
		joints[HIP_JOINT][walkingLeg] -= speed;
		displacement+= 0.008*speed;
		joints[HIP_JOINT][!walkingLeg] -= speed;
		joints[KNEE_JOINT][!walkingLeg] -= speed;
	}	 	 	 
	else if(joints[HIP_JOINT][walkingLeg] == HIPMIN) { upCycle = TRUE; walkingLeg++; }
	
}

void frontStance()
{
	joints[HIP_JOINT][0] = (GLfloat)60.0;	 	 // left leg forward
	joints[HIP_JOINT][1] = (GLfloat)-45.0;	 	 // right leg back
	joints[KNEE_JOINT][1] = (GLfloat)-45.0;
	joints[ELBOW_JOINT][1] = (GLfloat)55.0;			//right elbow 45 deg
	joints[SHOULDER_JOINT3][1] = (GLfloat)-65.0;	// right arm back
	joints[SHOULDER_JOINT2][1] = (GLfloat)-100.0;	// right arm back
	joints[SHOULDER_JOINT][1] = (GLfloat)20.0;		// right arm back
}

void showWorld(void)
{
	// flyZoom-50 is roughly center of room
	
	if( ((flybyZoom-55)+displacement)<=0) flybyZoom+=0.1;
	else
	{
		flybyRot+=0.5;		// rotate around the robot
		joints[SHOULDER_JOINT3][1] = -90;
		joints[SHOULDER_JOINT2][1] = wave;	
		joints[SHOULDER_JOINT][1] = 180;
		joints[ELBOW_JOINT][1] = 90;
		joints[SHOULDER_JOINT3][0] = -wave;
		joints[SHOULDER_JOINT2][0] = -wave;	 
		joints[SHOULDER_JOINT][0] = 180;
		joints[ELBOW_JOINT][0] = wave/2;	     
		
		if(wave<180 && waveTurn==TRUE){ wave+=5;}
		else if(wave == 180 && waveTurn == TRUE) { waveTurn = FALSE;}
		else if(wave > 0 &&  waveTurn==FALSE) { wave-=5;} 
		else if(wave == 0) { waveTurn = TRUE;}	    
	}	 
	
	if(displacement >= 5) { displayWorldSequence = FALSE; walking = FALSE; }
}

void display(void)
{
	/* display callback, clear frame buffer and z buffer,
	 rotate cube and draw, swap buffers */
	
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glScalef(1.0, 1.0, 1.0);
	glColor3f(1.0,1.0,1.0);
	
	gluLookAt(x, y, z, 
			  x + lx,y + ly,z + lz,
			  0.0,1.0,0.0);
	
	//	  printf("(%f,%f,%f, %f,%f,%f)\n",x, y, z, 
	//      x + lx,y + ly,z + lz);
	
	if(displayWorldSequence)
	{
		
		showWorld();
		currentdisplacement = displacement;	   
	}	 
	
	glTranslatef(0.0, 0.0, -3.0);
	glRotatef(flybyRot, 0.0, 1.0, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	glTranslatef(0.0, flyby, -flybyZoom);
	glTranslatef(0.0, 0.0, currentdisplacement);
	
	glTranslatef(0.0, 0.0, 50.0 );
	
	if(walking) walk(1.0);
	else if(frontstance) frontStance();
	
 	glCallList (worldDL);
	glPushMatrix();
	glTranslatef(-10.0,-5.0,-60.0);
	glScalef(0.02, 0.02 * 1.5, 0.05);;
	glCallList (terrainDL);
	glPopMatrix();
	
	glRotatef(180, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -0.2, -5.0 );
	robot();
	glPopMatrix();
	
	glFlush();
	glutSwapBuffers();
}

void spinCube()
{	 
	display();
}


void orientMe(float ang) {
	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	gluLookAt(x, y, z, 
			  x + lx,y + ly,z + lz,
			  0.0,1.0,0.0);
}

void moveMeFlat(int direction) {
	x = x + direction*(lx)*0.1;
	z = z + direction*(lz)*0.1;
	glLoadIdentity();
	gluLookAt(x, y, z, 
			  x + lx,y + ly,z + lz,
			  0.0,1.0,0.0);
}

void mykey(unsigned char key, int localx, int localy)
{
	
	switch(key){
		case 'i' : joints[HIP_JOINT][0]+= 1; break;
		case 'o' : joints[HIP_JOINT][0]-= 1; break;
		case 'I' : joints[HIP_JOINT][1]+= 1; break;
		case 'O' : joints[HIP_JOINT][1]-= 1; break;
		case 'y' : joints[TOP_LEG_SEPARATION][0]+= 1; break;
		case 'u' : joints[TOP_LEG_SEPARATION][0]-= 1; break;
		case 'Y' : joints[TOP_LEG_SEPARATION][1]+= 1; break;
		case 'U' : joints[TOP_LEG_SEPARATION][1]-= 1; break;
		case 'c' : joints[KNEE_JOINT][0]+= 1; break;
		case 'x' : joints[KNEE_JOINT][0]-= 1; break;
		case 'C' : joints[KNEE_JOINT][1]+= 1; break;
		case 'X' : joints[KNEE_JOINT][1]-= 1; break;
		case 'l' : joints[ELBOW_JOINT][1]+= 1; break;
		case 'k' : joints[ELBOW_JOINT][1]-= 1; break;
		case 'L' : joints[ELBOW_JOINT][0]-= 1; break;
		case 'K' : joints[ELBOW_JOINT][0]+= 1; break;
		case 'b' : joints[SHOULDER_JOINT][1]-= 1; break;
		case 'v' : joints[SHOULDER_JOINT][1]+= 1; break;
		case 'g' : joints[SHOULDER_JOINT2][1]-= 1; break;
		case 'f' : joints[SHOULDER_JOINT2][1]+= 1; break;	   	   	    	  	    	  
		case 'm' : joints[SHOULDER_JOINT3][1]-= 1; break;
		case 'n' : joints[SHOULDER_JOINT3][1]+= 1; break;
		case 't' : 
			reset();
			walking = TRUE;
			displayWorldSequence = TRUE;
			flybyZoom = 0.0;
			flybyRot = 0.0;		
			x=0.0;
			y=0.0;
			z=0.0;
			lx=0.0;
			ly=0.0;
			lz=-1.0;
			currentdisplacement = 0;		
			break;
		case 'p' : 
			reset();
			displayWorldSequence = FALSE;
			flybyZoom = 30.0;
			flybyRot = 0.0;
			walking = FALSE;
			x=0.0;
			y=0.0;
			z=0.0;
			lx=0.0;
			ly=0.0;
			lz=-1.0;
			currentdisplacement = 0;
			break;	   
		case 'r' : reset(); break;
		case 'h' : displayMenu(); break;		
		case 'q' : clearmem(); exit(0);
		default : break;
			break;
	}
	
	// can olny use these keys while not in flyby mode
	if(displayWorldSequence == FALSE)
	{
		switch(key){
		  	case 'a' : 
				angle -= 0.05;
				orientMe(angle);break;
			case 'd' : 
				angle +=0.05;
				orientMe(angle);break;
			case 'w' : 
				moveMeFlat(1);break;
			case 'W' : 
				moveMeFlat(5);break;	   	   
			case 's' : 
				moveMeFlat(-1);break;
			case ' ' : walking=!walking; break;
			case ']' : frontstance=!frontstance; break;	 	   	   	   
			default : break;
				break;
	 	}	 	 
	}
}

int clearmem()
{
	int i = 0;
	
	printf("clearing texture memory\n");
	for(;i<texNum; i++)
	{
		free(my_texture[i]);
		my_texture[i]= 0;
	}	 
}

int reset()
{
	
	lastx = 0;
	lasty = 0;
	
	mode =0;
	xrot = 0;
	yrot = 0;
	zoom = 0;
	
	displacement = 0.0;
	
	robotYRot = 0.0;
	
	
	start = TRUE;
	walkingLeg = 0;
	upCycle = TRUE;
	
	joints[0][0] = joints[0][1] = joints[1][0] = joints[1][1] = joints[5][0] = joints[5][1] = joints[6][0] = joints[6][1] = 0;
	joints[2][0] = 10.0; joints[2][1] = -190.0;
	joints[3][0] = joints[3][1] = 5.0;
	joints[4][0] = joints[4][1] = 15.0;
}

void myReshape(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport
	
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0,(GLfloat)width/(GLfloat)height,0.1,500.0);
	
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}


main(int argc, char **argv)
{
	int i;
    glutInit(&argc, argv);
	
	/* need both double buffering and z buffer */
	
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//	  glutGameModeString("1280x1024:32@60");
	//	  glutEnterGameMode();
    glutInitWindowSize(640, 480);
    glutCreateWindow("djo101 coursework submission 2");	   
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
	glutIdleFunc(spinCube);
	glutKeyboardFunc(mykey);
	
	bindAllTex();
	
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0, 0.0, 0.0, 0.5);				// Black Background
	glClearDepth(1.0);	   	   	   	   	   	   	   	   // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable (GL_NORMALIZE);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	bindMaps();
	renderHeightMap(my_map, HEIGHT_MAP_SIZE);			// create heightmap display list
	CreateWorld();										// create world display list
	free(my_map);										// free the map data from memory
	
	glutMainLoop();
}

