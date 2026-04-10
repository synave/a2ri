#include "opengl.h"
#include <a2ri/face.h>
#include <stdio.h>
#include <time.h>
#include <a2ri/subdivision.h>
#include <a2ri/quality.h>

vf_model *modeltodisplay;
double vpx,vpy,vpz,cx,cy,cz;
double rinc,tinc,tx,ty,tz;
GLfloat matShininess = 16.0f;
int initx,inity,rotation,deplacement;
double angle,angle2,longdiag;
int affiche_normale;
double *ar,min_ar,max_ar;
int size_ar;
int *ar_int;
int *liste_vert=NULL;
int *liste_rouge=NULL;
int size_liste_couleur=0;
double seuil;


void 
init (int argc, char **argv)
{
  
  GLfloat matR = 1.0f;
  GLfloat matG = 1.0f;
  GLfloat matB = 1.0f;
  GLfloat matShininess = 10.0f;
  
  GLfloat matGray[]= {0.1f, 0.1f, 0.1f, 1.0f};
  GLfloat mat[] = {matR, matG, matB, 1.0f};

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (WIDTH, HEIGHT);
  glutInitWindowPosition (100, 25);
  glutCreateWindow ("Visualisation de l aspect ratio des triangles");

  glClearColor (255.0, 255.0, 255.0, 0.0);
  glEnable(GL_DEPTH_TEST); 
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  glEnable(GL_LIGHT0);
  glShadeModel(GL_SMOOTH);

  GLfloat ambientlight[]={1.0f,1.0f,1.0f,1.0f};
  GLfloat diffuselight[]={1.0f,1.0f,1.0f,1.0f};
  GLfloat specularlight[]={1.0f,1.0f,1.0f,1.0f};
  GLfloat position[]={vpx,vpy,vpz+tz,1.0f};
  if(modeltodisplay->zmax<0)
    position[2]*=-1.0;

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientlight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuselight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularlight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

  glMaterialfv (GL_FRONT, GL_AMBIENT, matGray);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, mat);
  glMaterialfv (GL_FRONT, GL_SPECULAR, mat);
  glMaterialf (GL_FRONT, GL_SHININESS, matShininess);

  glViewport (0, 0, (GLsizei)WIDTH, (GLsizei)HEIGHT);

}



void
display_triangles()
{
  GLfloat matBlue[] = {0.7f, 0.5f, 0.1f, 1.0f};
  glMaterialf (GL_FRONT, GL_SHININESS, matShininess);
  
  for(int i=0;i<modeltodisplay->nbface;i++)
    {
      int ve1=modeltodisplay->fa[i].ve1;
      int ve2=modeltodisplay->fa[i].ve2;
      int ve3=modeltodisplay->fa[i].ve3;

      if(ar[i]<seuil)
	{
	  
	  glBegin(GL_TRIANGLES);
	  //glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	  //glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
	  glColor4f((liste_rouge[i]*1.0)/255.0, (liste_vert[i]*1.0)/255.0, 0.0f, 1.0f);
	  vector3d AB,AC,norm;
	  vector3d_init(&AB,
			modeltodisplay->ve[ve2].x-modeltodisplay->ve[ve1].x,
			modeltodisplay->ve[ve2].y-modeltodisplay->ve[ve1].y,
			modeltodisplay->ve[ve2].z-modeltodisplay->ve[ve1].z);
	  vector3d_init(&AC,
			modeltodisplay->ve[ve3].x-modeltodisplay->ve[ve1].x,
			modeltodisplay->ve[ve3].y-modeltodisplay->ve[ve1].y,
			modeltodisplay->ve[ve3].z-modeltodisplay->ve[ve1].z);
	  norm=vector3d_vectorialproduct(&AB,&AC);
	  vector3d_normalize(&norm);
	  glMaterialfv (GL_FRONT, GL_DIFFUSE, matBlue);
	  glNormal3f (norm.dx, norm.dy, norm.dz);
	  glVertex3f(modeltodisplay->ve[ve1].x,
		     modeltodisplay->ve[ve1].y,
		     modeltodisplay->ve[ve1].z);
	  glVertex3f(modeltodisplay->ve[ve2].x,
		     modeltodisplay->ve[ve2].y,
		     modeltodisplay->ve[ve2].z);
	  glVertex3f(modeltodisplay->ve[ve3].x,
		     modeltodisplay->ve[ve3].y,
		     modeltodisplay->ve[ve3].z);
	  glEnd();
	  
	  if(affiche_normale)
	    {
	      glColor4f(0.0f,0.0f,1.0f,1.0f);
	      glLineWidth(2);
	      glBegin(GL_LINES);
	      glVertex3f((modeltodisplay->ve[ve1].x+modeltodisplay->ve[ve2].x+modeltodisplay->ve[ve3].x)/3.0,
			 (modeltodisplay->ve[ve1].y+modeltodisplay->ve[ve2].y+modeltodisplay->ve[ve3].y)/3.0,
			 (modeltodisplay->ve[ve1].z+modeltodisplay->ve[ve2].z+modeltodisplay->ve[ve3].z)/3.0);
	      glVertex3f((modeltodisplay->ve[ve1].x+modeltodisplay->ve[ve2].x+modeltodisplay->ve[ve3].x)/3.0+(norm.dx*longdiag/20.0),
			 (modeltodisplay->ve[ve1].y+modeltodisplay->ve[ve2].y+modeltodisplay->ve[ve3].y)/3.0+(norm.dy*longdiag/20.0),
			 (modeltodisplay->ve[ve1].z+modeltodisplay->ve[ve2].z+modeltodisplay->ve[ve3].z)/3.0+(norm.dz*longdiag/20.0));
	      glEnd();
	    }
	  
	  glColor4f(0.0f,0.0f,0.0f,1.0f);
	  glLineWidth(2);
	  glBegin(GL_LINES);
	  glVertex3f(modeltodisplay->ve[ve1].x,
		     modeltodisplay->ve[ve1].y,
		     modeltodisplay->ve[ve1].z);
	  glVertex3f(modeltodisplay->ve[ve2].x,
		     modeltodisplay->ve[ve2].y,
		     modeltodisplay->ve[ve2].z);
	  glVertex3f(modeltodisplay->ve[ve2].x,
		     modeltodisplay->ve[ve2].y,
		     modeltodisplay->ve[ve2].z);
	  glVertex3f(modeltodisplay->ve[ve3].x,
		     modeltodisplay->ve[ve3].y,
		     modeltodisplay->ve[ve3].z);
	  glVertex3f(modeltodisplay->ve[ve3].x,
		     modeltodisplay->ve[ve3].y,
		     modeltodisplay->ve[ve3].z);
	  glVertex3f(modeltodisplay->ve[ve1].x,
		     modeltodisplay->ve[ve1].y,
		     modeltodisplay->ve[ve1].z);
	  glEnd();
	}
    }
  
  glFlush();
}


void 
display ()
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (45, WIDTH/((float)HEIGHT), 0.1, 5000);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  
  gluLookAt (vpx+tx,vpy+ty,vpz+tz,cx+tx,cy+ty,cz+tz,0,1,0);
  glTranslatef((modeltodisplay->xmin+modeltodisplay->xmax)/2.0,
	       (modeltodisplay->ymin+modeltodisplay->ymax)/2.0,
	       (modeltodisplay->zmin+modeltodisplay->zmax)/2.0);
  glRotatef(angle,0.0,1.0,0.0);
  glRotatef(angle2,1.0,0.0,0.0);
  glTranslatef(-(modeltodisplay->xmin+modeltodisplay->xmax)/2.0,
	       -(modeltodisplay->ymin+modeltodisplay->ymax)/2.0,
	       -(modeltodisplay->zmin+modeltodisplay->zmax)/2.0);
  display_triangles();

  glutSwapBuffers ();
}



void
compute_aspect_ratio()
{
  free(ar);
  ar=NULL;
  free(liste_vert);
  free(liste_rouge);
  liste_vert=NULL;
  liste_rouge=NULL;
  size_liste_couleur=0;
  size_ar=0;
  free(ar_int);
  ar_int=0;
  for(int i=0;i<modeltodisplay->nbface;i++)
    {
      double val=a2ri_vf_triangle_aspect_ratio(modeltodisplay,i);

      //val=sqrt(val);
      if(i==0)
	{
	  min_ar=val;
	  max_ar=val;
	}
      else
	{
	  if(val>max_ar)
	    max_ar=val;
	  if(val<min_ar)
	    min_ar=val;
	}
      list_double_add(&ar,&size_ar,val,WITH_REDUNDANCE);
    }

  printf("min : %lf - max : %lf\n",min_ar,max_ar);
  seuil=max_ar+0.25;

  min_ar=1.1547;
  if(max_ar>10)
  max_ar=10;

  size_ar=0;
  for(int i=0;i<modeltodisplay->nbface;i++)
    {
      int val=(int)(((ar[i]-min_ar)/(max_ar-min_ar))*65535.0);
      if(val>65535)
	val=65535;
      list_int_add(&ar_int,&size_ar,(int)(((ar[i]-min_ar)/(max_ar-min_ar))*65535.0),WITH_REDUNDANCE);
    }

  liste_vert=NULL;
  liste_rouge=NULL;
  size_liste_couleur=0;

  for(int i=0;i<modeltodisplay->nbface;i++)
    {
      if(ar_int[i]<32767)
	{
	  list_int_add(&liste_rouge,&size_liste_couleur,ar_int[i]/128,WITH_REDUNDANCE);
	  size_liste_couleur--;
	  list_int_add(&liste_vert,&size_liste_couleur,255,WITH_REDUNDANCE);
	}
      else
	{
	  list_int_add(&liste_rouge,&size_liste_couleur,255,WITH_REDUNDANCE);
	  size_liste_couleur--;
	  list_int_add(&liste_vert,&size_liste_couleur,255-((ar_int[i]-32767)/128),WITH_REDUNDANCE);
	}
    }

}

   

void 
exitFunc ()
{
  a2ri_vf_free(modeltodisplay);
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
  switch (key)
    {
    case 'l':
    case 'L':
      a2ri_vf_loop(modeltodisplay,1);
      compute_aspect_ratio();
      break;
    case 's':
    case 'S':
      printf("entrez le nouveau seuil :");
      a2ri_erreur_critique_si(!scanf("%lf",&seuil),"erreur seuil\n");
      break;
    case 't':
    case 'T':
      if(glIsEnabled(GL_LIGHTING))
	glDisable(GL_LIGHTING);
      else
	glEnable(GL_LIGHTING);
      break;
    case 'n':
    case 'N':
      affiche_normale=(affiche_normale+1)%2;
      break;
    case '+':
      if(glIsEnabled(GL_LIGHTING))
	{
	  matShininess/=2.0;
	  if(matShininess<1.0)
	    matShininess=1.0;
	}
      else
	{
	  seuil+=0.25;
	  printf("seuil : %lf\n",seuil);
	}
      break;
    case ':':
      a2ri_vf_display(modeltodisplay);
      break;
    case '-':
      if(glIsEnabled(GL_LIGHTING))
	{
	  matShininess*=2.0;
	  if(matShininess>128.0)
	    matShininess=128.0;
	}
      else
	{
	  seuil-=0.25;
	  if(seuil<1.1547)
	    seuil+=0.25;
	  printf("seuil : %lf\n",seuil);
	}
      break;
    case 'q':
    case 'Q':
    case 27:
      exitFunc ();
      break;
    }
}


void 
idleFunc ()
{  
  glutPostRedisplay ();
}

int 
go (int argc, char **argv, vf_model *m)
{
  point3d ptmin,ptmax;

  modeltodisplay=m;

  ar=NULL;
  ar_int=NULL;
  size_ar=0;
  liste_vert=NULL;
  liste_rouge=NULL;
  size_liste_couleur=0;
  compute_aspect_ratio();

  rinc=R_INC;
  tinc=T_INC;
  if(modeltodisplay->zmax-modeltodisplay->zmin!=0)
      tinc=(modeltodisplay->zmax-modeltodisplay->zmin)/10;
  if(modeltodisplay->zmax!=0)
    vpz=modeltodisplay->zmax*1.5+tz;
  else
    vpz=70;

  ptmin.x=modeltodisplay->xmin;
  ptmin.y=modeltodisplay->ymin;
  ptmin.z=modeltodisplay->zmin;
  ptmax.x=modeltodisplay->xmax;
  ptmax.y=modeltodisplay->ymax;
  ptmax.z=modeltodisplay->zmax;
  longdiag=point3d_length(&ptmin,&ptmax);
  
  vpx=(modeltodisplay->xmin+modeltodisplay->xmax)/2.0;
  vpy=(modeltodisplay->ymin+modeltodisplay->ymax)/2.0;
  cx=(modeltodisplay->xmin+modeltodisplay->xmax)/2.0;
  cy=(modeltodisplay->ymin+modeltodisplay->ymax)/2.0;
  cz=(modeltodisplay->zmin+modeltodisplay->zmax)/2.0;
  angle=0;
  angle2=0;
  tx = ty = tz = 0;
  affiche_normale=0;
  init (argc, argv);
  
  glutDisplayFunc (display);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc (keyboard);
  glutIdleFunc (idleFunc);
  glutMainLoop ();
  
  return EXIT_SUCCESS;
}
    
