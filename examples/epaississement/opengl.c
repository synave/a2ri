#include "opengl.h"
#include <a2ri/epaississement.h>
#include <stdio.h>
#include <time.h>

vf_model *original;
vf_model *todisplay;
int zoom=0;
float rinc;
float tinc;
int initx,inity,rotation,deplacement;
double angle,angle2,longdiag;
int affiche_normale;
double epaisseur;


void 
init (int argc, char **argv)
{
  int i, j;

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (WIDTH, HEIGHT);
  glutInitWindowPosition (100, 25);
  glutCreateWindow ("Epaississement de surface");

  glClearColor (0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glViewport (0, 0, (GLsizei)WIDTH, (GLsizei)HEIGHT);

  rx = ry = rz = tx = ty = tz = 0;
  for (i=0; i < NB_VAL; i++)
    for (j=0; j < 6; j++)
      hist [i][j] = 0;
}



void
display_triangles()
{
  for(int i=0;i<todisplay->nbface;i++)
    {
      int ve1=todisplay->fa[i].ve1;
      int ve2=todisplay->fa[i].ve2;
      int ve3=todisplay->fa[i].ve3;


      glBegin(GL_TRIANGLES);
      glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
      vector3d AB,AC,norm;
      vector3d_init(&AB,
		    todisplay->ve[ve2].x-todisplay->ve[ve1].x,
		    todisplay->ve[ve2].y-todisplay->ve[ve1].y,
		    todisplay->ve[ve2].z-todisplay->ve[ve1].z);
      vector3d_init(&AC,
		    todisplay->ve[ve3].x-todisplay->ve[ve1].x,
		    todisplay->ve[ve3].y-todisplay->ve[ve1].y,
		    todisplay->ve[ve3].z-todisplay->ve[ve1].z);
      norm=vector3d_vectorialproduct(&AB,&AC);
      vector3d_normalize(&norm);
      glNormal3f (norm.dx, norm.dy, norm.dz);
      glVertex3f(todisplay->ve[ve1].x,
		 todisplay->ve[ve1].y,
		 todisplay->ve[ve1].z);
      glVertex3f(todisplay->ve[ve2].x,
		 todisplay->ve[ve2].y,
		 todisplay->ve[ve2].z);
      glVertex3f(todisplay->ve[ve3].x,
		 todisplay->ve[ve3].y,
		 todisplay->ve[ve3].z);
      glEnd();

      if(affiche_normale)
	{
	  glColor4f(0.0f,0.0f,1.0f,1.0f);
	  glLineWidth(2);
	  glBegin(GL_LINES);
	  glVertex3f((todisplay->ve[ve1].x+todisplay->ve[ve2].x+todisplay->ve[ve3].x)/3.0,
		     (todisplay->ve[ve1].y+todisplay->ve[ve2].y+todisplay->ve[ve3].y)/3.0,
		     (todisplay->ve[ve1].z+todisplay->ve[ve2].z+todisplay->ve[ve3].z)/3.0);
	  glVertex3f((todisplay->ve[ve1].x+todisplay->ve[ve2].x+todisplay->ve[ve3].x)/3.0+(norm.dx*longdiag/20.0),
		     (todisplay->ve[ve1].y+todisplay->ve[ve2].y+todisplay->ve[ve3].y)/3.0+(norm.dy*longdiag/20.0),
		     (todisplay->ve[ve1].z+todisplay->ve[ve2].z+todisplay->ve[ve3].z)/3.0+(norm.dz*longdiag/20.0));
	  glEnd();
	}

      glColor4f(0.0f,0.0f,0.0f,1.0f);
      glLineWidth(2);
      glBegin(GL_LINES);
      glVertex3f(todisplay->ve[ve1].x,
		 todisplay->ve[ve1].y,
		 todisplay->ve[ve1].z);
      glVertex3f(todisplay->ve[ve2].x,
		 todisplay->ve[ve2].y,
		 todisplay->ve[ve2].z);
      glVertex3f(todisplay->ve[ve2].x,
		 todisplay->ve[ve2].y,
		 todisplay->ve[ve2].z);
      glVertex3f(todisplay->ve[ve3].x,
		 todisplay->ve[ve3].y,
		 todisplay->ve[ve3].z);
      glVertex3f(todisplay->ve[ve3].x,
		 todisplay->ve[ve3].y,
		 todisplay->ve[ve3].z);
      glVertex3f(todisplay->ve[ve1].x,
		 todisplay->ve[ve1].y,
		 todisplay->ve[ve1].z);
      glEnd();
    }
  
  glFlush();
}



void 
transform (int i)
{
  glTranslatef((todisplay->xmin+todisplay->xmax)/2.0,
	       (todisplay->ymin+todisplay->ymax)/2.0,
	       (todisplay->zmin+todisplay->zmax)/2.0);
  glRotatef (hist[i][0], 1.0, 0.0, 0.0);
  glRotatef (hist[i][1], 0.0, 1.0, 0.0);
  glRotatef (hist[i][2], 0.0, 0.0, 1.0);
  glTranslatef(-(todisplay->xmin+todisplay->xmax)/2.0,
	       -(todisplay->ymin+todisplay->ymax)/2.0,
	       -(todisplay->zmin+todisplay->zmax)/2.0);
}

void 
display ()
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (45, WIDTH/((float)HEIGHT), 0.1, 900);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  if(todisplay->zmax==0)
    gluLookAt (((todisplay->xmin+todisplay->xmax)/2.0)+tx,
	       ((todisplay->ymin+todisplay->ymax)/2.0)+ty, 
	       70+tz,
	       ((todisplay->xmin+todisplay->xmax)/2.0)+tx,
	       ((todisplay->ymin+todisplay->ymax)/2.0)+ty,
	       ((todisplay->zmin+todisplay->zmax)/2.0)+tz,
	       0, 1, 0);
  else
    gluLookAt (((todisplay->xmin+todisplay->xmax)/2.0)+tx,
	       ((todisplay->ymin+todisplay->ymax)/2.0)+ty,
	       todisplay->zmax*1.5+tz,
	       ((todisplay->xmin+todisplay->xmax)/2.0)+tx,
	       ((todisplay->ymin+todisplay->ymax)/2.0)+ty,
	       ((todisplay->zmin+todisplay->zmax)/2.0)+tz,
	       0, 1, 0);
  
  transform (0);

  glTranslatef((todisplay->xmin+todisplay->xmax)/2.0,
	       (todisplay->ymin+todisplay->ymax)/2.0,
	       (todisplay->zmin+todisplay->zmax)/2.0);
  glRotatef(angle,0.0,1.0,0.0);
  glRotatef(angle2,1.0,0.0,0.0);
  glTranslatef(-(todisplay->xmin+todisplay->xmax)/2.0,
	       -(todisplay->ymin+todisplay->ymax)/2.0,
	       -(todisplay->zmin+todisplay->zmax)/2.0);

  display_triangles();

  glutSwapBuffers ();
}
   

void 
exitFunc ()
{
  a2ri_vf_free(original);
  exit (EXIT_SUCCESS);
}


void mouse(int button, int state, int x, int y)
{
  if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
      rotation=1;
      initx=x;
      inity=y;
    }

  if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
    rotation=0;

  if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    {
      deplacement=1;
      initx=x;
      inity=y;
    }

  if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
    deplacement=0;

  if(button==GLUT_WHEEL_MOUSE_UP)
    tz+=longdiag/50;

  if(button==GLUT_WHEEL_MOUSE_DOWN)
    tz-=longdiag/50;
}

void motion(int x, int y)
{
  if(rotation)
    {
      angle+=(x-initx)*0.5;
      angle2+=(y-inity)*0.5;
      initx=x;
      inity=y;
      glutPostRedisplay();
    }

  if(deplacement)
    {
      tx-=(x-initx)*0.001*longdiag;
      ty+=(y-inity)*0.001*longdiag;
      initx=x;
      inity=y;
      glutPostRedisplay();
    }
}


void 
keyboard (unsigned char key, int x, int y)
{
  int i, j;
  char file[100];

  //evite les warning
  x=y;
  y=x;

  switch (key)
    {	
    case 'd':
    case 'D':
      a2ri_vf_display(todisplay);
      break;
    case 'f':
    case 'F':
      a2ri_vf_display_detail(todisplay);
      break;
    case 'o':
    case 'O':
      todisplay=original;
      break;
    case 'e':
    case 'E':
      a2ri_vf_epaissi_surface(original,epaisseur);
      break;
    case 'n':
    case 'N':
      affiche_normale=(affiche_normale+1)%2;
      break;
    case 's':
    case 'S':
      printf("entrer le nom du fichier :\n");
      a2ri_erreur_critique_si(!scanf("%s",file),"erreur\n");
      a2ri_vf_save_file(file,todisplay);
      break;
    case 'Q':
    case 'q':
    case 27:
      exitFunc ();
      break;
    }


  for (i=NB_VAL-1; i > 0; i--)
    for (j=0; j < 6; j++)
      hist [i][j] = hist [i-1][j];
  hist [0][0] = rx;
  hist [0][1] = ry;
  hist [0][2] = rz; 
  hist [0][3] = tx;
  hist [0][4] = ty;
  hist [0][5] = tz;
}


void 
idleFunc ()
{
  int i, j;
  for (i=NB_VAL-1; i > 0; i--)
    for (j=0; j < 6; j++)
      hist [i][j] = hist [i-1][j];
  
  glutPostRedisplay ();
}


int 
go (int argc, char **argv, vf_model *m, double epais)
{
  point3d ptmin,ptmax;

  epaisseur=epais;

  original=m;
  todisplay=original;
  affiche_normale=0;
  rinc=R_INC;
  tinc=T_INC;
  if(todisplay->zmax-todisplay->zmin!=0)
    tinc=(todisplay->zmax-todisplay->zmin)/10;

  ptmin.x=todisplay->xmin;
  ptmin.y=todisplay->ymin;
  ptmin.z=todisplay->zmin;
  ptmax.x=todisplay->xmax;
  ptmax.y=todisplay->ymax;
  ptmax.z=todisplay->zmax;
  longdiag=point3d_length(&ptmin,&ptmax);

  init (argc, argv);
  
  glutDisplayFunc (display);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc (keyboard);
  glutIdleFunc (idleFunc);
  glutMainLoop ();
  
  return EXIT_SUCCESS;
}
    
