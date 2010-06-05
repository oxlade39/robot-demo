Controls - Anytime:
i - top of leg up
o - top of leg down
y - leg to side
u - reverse leg to side
c - lower part of leg up
x - lower part of leg down
l - elbow movement
k - elbow movement oposite
b - shoulder back
v - shoulder forward
g - arm rotation
f - arm rotation oposite
m - shoulder rotation
n - shoulder rotation oposite

in some cases the capital case will cause the oposite limb to move.

t - Tour mode
p - display picture point
r - reset all variables to initial
h - print help text
q - quit program
--------------------------------------------------------------------------------
Controls - Non tour mode only:
a - rotate view to left
d - rotate view to right
w - move forward
s - move backward
space - start robot walking
] - perform taekwondo front stance

--------------------------------------------------------------------------------
Mouse - no use
--------------------------------------------------------------------------------
File contents:

--------------------------------------------------------------------------------
Build instruction:
Open .prj file in quincy, compile build and run using quincy menu.
--------------------------------------------------------------------------------
Description:
The world is built of 2 parts. The room and the landscape.
Room:
The room is 3 walls with brick tecture, roof with wooden texture and floor with
 different wooden texture. The walls should be lit in a gradualy
decreasing light as distance from the light increases, this has not been implemented
on the floor or roof. The light source is in the center of the room indicated by
the white cube on the roof.
Terrain:
The terrain is a heightmap built from the "terrain2.raw" file and shaded green.

The robot:
The robot is built using cylinders and a sphere for head and box for body.
Shoulders, elbows, knees and hips can all be rotated in a human like manor.
The robot can walk backwards and forwards along a line.
The robot has a single texture which is sphere mapped to give it a shiny metallic
look the robot also has high emission.
As the robot walks under and passed the light the shading will match.
---------------------------------------------------------------------------------
Code Re-use:
I took one of the gcc.prj files as a template and heavily modified and stripped out
unused code. I started with the lighting from the framework and modified this to get
my desired lighting. I used the loadtexture function from the lena gcc example.
My heightmap creation was done after studying the NeHe tutorial on heightmaps and the
same code structure was used (ported to glut and non windows environment).
