#ifndef OPENGL__H
#define OPENGL__H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <a2ri/io.h>

#define WIDTH 640
#define HEIGHT 480

#define  GLUT_WHEEL_MOUSE_UP                0x0003
#define  GLUT_WHEEL_MOUSE_DOWN              0x0004


#define R_INC 5
#define T_INC 1

void init(int argc, char **argv);
void transform(int i);
void display_triangles();
void display();
void exitFunc();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void idleFunc();
int go(int argc, char **argv, vf_model *m);

#endif
