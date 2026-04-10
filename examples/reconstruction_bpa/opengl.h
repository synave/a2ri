#ifndef OPENGL__H
#define OPENGL__H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <a2ri/io.h>
#include <a2ri/vector.h>
#include <a2ri/triangulation.h>
#include <a2ri/topology.h>

#define WIDTH 400
#define HEIGHT 300

#define  GLUT_WHEEL_MOUSE_UP                0x0003
#define  GLUT_WHEEL_MOUSE_DOWN              0x0004
#define  GLUT_CENTER_BUTTON                 0x0001

#define COUL_TRIANGLE 0.9,0.9,0.9
#define COUL_ARETE 0.0,0.0,0.0
#define COUL_POINT 1.0,0.0,0.0

#define COUL_MAT_OBJ 0.8f, 0.8f, 0.75f
#define COUL_LUMIERE 0.1f ,0.1f, 0.1f
#define COUL_LUMIERE_AMBIENT  1.0f, 1.0f, 1.0f
#define COUL_LUMIERE_DIFFUSE  1.0f, 1.0f, 1.0f
#define COUL_LUMIERE_SPECULAR  1.0f, 1.0f, 1.0f

#define VERTICES 0
#define WIREFRAME 1
#define SOLID 2

double angle,angle2;
double tx, ty, zoom;
int initx,inity,rotation,translation;
double ratioecran;

int go(int argc, char **argv, vf_model *b, double radius, char *ficsave);

#endif
