#ifndef OPENGL__H
#define OPENGL__H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <a2ri/io.h>
#include <a2ri/point.h>
#include <a2ri/hashtable.h>
#include <a2ri/vector.h>
#include <a2ri/topology.h>
#include <a2ri/quality.h>
#include <a2ri/graph.h>
#include <a2ri/subdivision.h>
#include <a2ri/geodesique.h>

#define WIDTH 800
#define HEIGHT 600

#define BUFSIZE 512

#define  GLUT_WHEEL_MOUSE_UP                0x0003
#define  GLUT_WHEEL_MOUSE_DOWN              0x0004
#define  GLUT_CENTER_BUTTON                 0x0001

#define COUL_TRIANGLE 0.9,0.9,0.9
#define COUL_TRIANGLE_SELEC 0.9,0.1,0.9
#define COUL_TRIANGLE_DANS_SELEC 0.8,0.8,0.0
#define COUL_SPHERE1 1.0,0.0,0.0
#define COUL_SPHERE2 0.0,0.0,1.0
#define COUL_ARETE 0.0,0.0,0.0
#define COUL_CHEMIN 0.0,1.0,0.0

double angle,angle2;
double tx, ty, tz;
int initx,inity,rotation,deplacement;
int masque_triangle;
int vedeb,vefin;
double ratioecran;

int go(int argc, char **argv, vf_model *b, int ve1, int ve2);

#endif
