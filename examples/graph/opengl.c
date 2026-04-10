#include "opengl.h"
#include <a2ri/triangulation.h>
#include <stdio.h>
#include <time.h>

vef_model *tomove;
vef_model *todisplay;
vef_model *GG,*NNG,*EMST,*EGG;
vef_model *tetraedrisation;
int zoom=0;
float rinc;
float tinc;
int initx,inity,rotation,deplacement;
double angle,angle2,longdiag;


void 
init (int argc, char **argv)
{
  int i, j;

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (WIDTH, HEIGHT);
  glutInitWindowPosition (100, 25);
  glutCreateWindow ("Construction de graphes");

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
  point3d p1,p2;
  point3d_init(&p1,todisplay->xmin,todisplay->ymin,todisplay->zmin);
  point3d_init(&p2,todisplay->xmax,todisplay->ymax,todisplay->zmax);
  double longdiag=point3d_length(&p1,&p2);
  
  if(todisplay->nbface>0)
    for(int i=0;i<todisplay->nbface;i++)
      {
	int ve1,ve2,ve3;
	vef_face_get_vertices(&(todisplay->fa[i]),todisplay->ed,&ve1,&ve2,&ve3);
	
	glBegin(GL_POLYGON);
	
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	
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
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
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
  else
    {
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      for(int i=0;i<todisplay->nbvertex;i++)
	{
	  glTranslatef(todisplay->ve[i].x,todisplay->ve[i].y,todisplay->ve[i].z);
	  glutWireCube(longdiag/400.0);
	  glTranslatef(-todisplay->ve[i].x,-todisplay->ve[i].y,-todisplay->ve[i].z);
	}
      for(int i=0;i<todisplay->nbedge;i++)
	{
	  glBegin(GL_LINES);
	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glVertex3f(todisplay->ve[todisplay->ed[i].ve1].x,
		     todisplay->ve[todisplay->ed[i].ve1].y,
		     todisplay->ve[todisplay->ed[i].ve1].z);
	  glVertex3f(todisplay->ve[todisplay->ed[i].ve2].x,
		     todisplay->ve[todisplay->ed[i].ve2].y,
		     todisplay->ve[todisplay->ed[i].ve2].z);
	  glEnd();
	}
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
  a2ri_vef_free(tomove);
  if(GG!=NULL)
    a2ri_vef_free(GG);
  if(NNG!=NULL)
    a2ri_vef_free(NNG);
  if(EMST!=NULL)
    a2ri_vef_free(EMST);
  if(EGG!=NULL)
    a2ri_vef_free(EGG);
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
  time_t dep,fin;
      
  switch (key)
    {	
    case 'a':
    case 'A':
      if(NNG==NULL)
	{
	  dep=clock();
	  NNG=a2ri_vef_nearest_neighbour_graph(tomove);
	  fin=clock();
	  printf("temps d'execution : %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
	}
      todisplay=NNG;
      break;
    case 'z':
    case 'Z':
      if(EMST==NULL)
	{
	  dep=clock();
	  EMST=a2ri_vef_euclidean_minimal_spanning_tree(tomove);
	  fin=clock();
	  printf("temps d'execution : %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
	}
      todisplay=EMST;
      break;
    case 'e':
    case 'E':
      if(GG==NULL)
	{
	  dep=clock();
	  GG=a2ri_vef_gabriel_graph(tomove);
	  fin=clock();
	  printf("temps d'execution : %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
	}
      todisplay=GG;
      break;
    case 'r':
    case 'R':
      if(EGG==NULL)
	{
	  dep=clock();
	  EGG=a2ri_vef_extended_gabriel_hypergraph(tomove);
	  fin=clock();    
	  printf("temps d'execution : %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
	}
      todisplay=EGG;
      break;
    case 's':
    case 'S':
      todisplay=tomove;
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
go (int argc, char **argv, vef_model *m)
{
  point3d ptmin,ptmax;

  tomove=m;
  todisplay=m;
  GG=NULL;
  NNG=NULL;
  EMST=NULL;
  EGG=NULL;
  tetraedrisation=NULL;
  rinc=R_INC;
  tinc=T_INC;
  if(tomove->zmax-tomove->zmin!=0)
    tinc=(tomove->zmax-tomove->zmin)/10;

  ptmin.x=tomove->xmin;
  ptmin.y=tomove->ymin;
  ptmin.z=tomove->zmin;
  ptmax.x=tomove->xmax;
  ptmax.y=tomove->ymax;
  ptmax.z=tomove->zmax;
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
    
