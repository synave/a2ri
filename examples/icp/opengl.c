#include <stdio.h>
#include <time.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>

#include "opengl.h"
#include <a2ri/subdivision.h>
#include <a2ri/icp.h>
#include <a2ri/io.h>
#include <a2ri/boundingbox.h>
#include <a2ri/overlap.h>
#include <time.h>

vf_model *modeltodisplay,*tomove;
int zoom=0;
float rinc;
float tinc;
double recouvrement,sensibilite;
int *facesmodeltodisplay, *facestomove;
point3d *ptmin_aabb,*ptmax_aabb,*ptmin_obb,*ptmax_obb;
vector3d *nvbasex,*nvbasey,*nvbasez;
int affich_aabb,affich_obb,affich_ball_exacte,affich_ball_ritter;
double dkarret;
double perttx=0.0,pertty=0.0,perttz=0.0,pertrotx=0.0,pertroty=0.0,pertrotz=0.0;
int initx,inity,rotation,deplacement;
double angle,angle2,longdiag;

int *liste_vert=NULL;
int *liste_rouge=NULL;
int size_liste_couleur=0;
int affich_tomove,affich_display;


void 
init (int argc, char **argv)
{
  int i, j;

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (WIDTH, HEIGHT);
  glutInitWindowPosition (100, 25);
  glutCreateWindow ("Automated Trimmed Iterative Closest Point");

  glClearColor (255.0, 255.0, 255.0, 0.0);
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
  if(affich_tomove)
    {
      for(int i=0;i<tomove->nbface;i++)
	{
	  int ve1=tomove->fa[i].ve1;
	  int ve2=tomove->fa[i].ve2;
	  int ve3=tomove->fa[i].ve3;
	  
	  glBegin(GL_POLYGON);
	  
	  glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	  if(liste_vert!=NULL)
	    glColor4f((liste_rouge[ve1]*1.0)/255.0, (liste_vert[ve1]*1.0)/255.0, 0.0f, 1.0f);
	  
	  glVertex3f(tomove->ve[ve1].x,
		     tomove->ve[ve1].y,
		     tomove->ve[ve1].z);
	  if(liste_vert!=NULL)
	    glColor4f((liste_rouge[ve2]*1.0)/255.0, (liste_vert[ve2]*1.0)/255.0, 0.0f, 1.0f);
	  glVertex3f(tomove->ve[ve2].x,
		     tomove->ve[ve2].y,
		     tomove->ve[ve2].z);
	  if(liste_vert!=NULL)
	    glColor4f((liste_rouge[ve3]*1.0)/255.0, (liste_vert[ve3]*1.0)/255.0, 0.0f, 1.0f);
	  glVertex3f(tomove->ve[ve3].x,
		     tomove->ve[ve3].y,
		     tomove->ve[ve3].z);
	  glEnd();
	  glLineWidth(2);
	  glBegin(GL_LINES);
	  glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	  glVertex3f(tomove->ve[ve1].x,
		     tomove->ve[ve1].y,
		     tomove->ve[ve1].z);
	  glVertex3f(tomove->ve[ve2].x,
		     tomove->ve[ve2].y,
		     tomove->ve[ve2].z);
	  glVertex3f(tomove->ve[ve2].x,
		     tomove->ve[ve2].y,
		     tomove->ve[ve2].z);
	  glVertex3f(tomove->ve[ve3].x,
		     tomove->ve[ve3].y,
		     tomove->ve[ve3].z);
	  glVertex3f(tomove->ve[ve3].x,
		     tomove->ve[ve3].y,
		     tomove->ve[ve3].z);
	  glVertex3f(tomove->ve[ve1].x,
		     tomove->ve[ve1].y,
		     tomove->ve[ve1].z);
	  glEnd();
	}
    }

  if(affich_display)
    {
      for(int i=0;i<modeltodisplay->nbface;i++)
	{
	  int ve1=modeltodisplay->fa[i].ve1;
	  int ve2=modeltodisplay->fa[i].ve2;
	  int ve3=modeltodisplay->fa[i].ve3;
	  
	  glBegin(GL_POLYGON);
	  
	  glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
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
	  glLineWidth(2);
	  glBegin(GL_LINES);
	  glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
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
display_aabb()
{
  double xmin,ymin,zmin,xmax,ymax,zmax;
  a2ri_vf_axis_aligned_bounding_box(modeltodisplay,facesmodeltodisplay,modeltodisplay->nbface,&ptmin_aabb,&ptmax_aabb);
  for(int i=0;i<modeltodisplay->nbface;i++)
    {
      xmin=ptmin_aabb[i].x;
      ymin=ptmin_aabb[i].y;
      zmin=ptmin_aabb[i].z;

      xmax=ptmax_aabb[i].x;
      ymax=ptmax_aabb[i].y;
      zmax=ptmax_aabb[i].z;
      
      
      glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
      glLineWidth(1);
      glBegin(GL_LINES);

      glVertex3f(xmin,ymin,zmin);
      glVertex3f(xmax,ymin,zmin);
      glVertex3f(xmax,ymin,zmin);
      glVertex3f(xmax,ymax,zmin);
      glVertex3f(xmax,ymax,zmin);
      glVertex3f(xmin,ymax,zmin);
      glVertex3f(xmin,ymax,zmin);
      glVertex3f(xmin,ymin,zmin);

      glVertex3f(xmax,ymin,zmin);
      glVertex3f(xmax,ymin,zmax);
      glVertex3f(xmax,ymin,zmax);
      glVertex3f(xmax,ymax,zmax);
      glVertex3f(xmax,ymax,zmax);
      glVertex3f(xmax,ymax,zmin);
      glVertex3f(xmax,ymax,zmin);
      glVertex3f(xmax,ymin,zmin);

      glVertex3f(xmin,ymin,zmin);
      glVertex3f(xmax,ymin,zmin);
      glVertex3f(xmax,ymin,zmin);
      glVertex3f(xmax,ymin,zmax);
      glVertex3f(xmax,ymin,zmax);
      glVertex3f(xmin,ymin,zmax);
      glVertex3f(xmin,ymin,zmax);
      glVertex3f(xmin,ymin,zmin);

      glVertex3f(xmin,ymin,zmax);
      glVertex3f(xmax,ymin,zmax);
      glVertex3f(xmax,ymin,zmax);
      glVertex3f(xmax,ymax,zmax);
      glVertex3f(xmax,ymax,zmax);
      glVertex3f(xmin,ymax,zmax);
      glVertex3f(xmin,ymax,zmax);
      glVertex3f(xmin,ymin,zmax);

      glVertex3f(xmin,ymin,zmin);
      glVertex3f(xmin,ymin,zmax);
      glVertex3f(xmin,ymin,zmax);
      glVertex3f(xmin,ymax,zmax);
      glVertex3f(xmin,ymax,zmax);
      glVertex3f(xmin,ymax,zmin);
      glVertex3f(xmin,ymax,zmin);
      glVertex3f(xmin,ymin,zmin);

      glVertex3f(xmin,ymax,zmin);
      glVertex3f(xmax,ymax,zmin);
      glVertex3f(xmax,ymax,zmin);
      glVertex3f(xmax,ymax,zmax);
      glVertex3f(xmax,ymax,zmax);
      glVertex3f(xmin,ymax,zmax);
      glVertex3f(xmin,ymax,zmax);
      glVertex3f(xmin,ymax,zmin);
      glEnd();
    }

  free(ptmin_aabb);
  free(ptmax_aabb);


  ptmin_aabb=NULL;
  ptmax_aabb=NULL;


  a2ri_vf_axis_aligned_bounding_box(tomove,facestomove,tomove->nbface,&ptmin_aabb,&ptmax_aabb);
  for(int i=0;i<tomove->nbface;i++)
    {
      xmin=ptmin_aabb[i].x;
      ymin=ptmin_aabb[i].y;
      zmin=ptmin_aabb[i].z;

      xmax=ptmax_aabb[i].x;
      ymax=ptmax_aabb[i].y;
      zmax=ptmax_aabb[i].z;
      
      
      glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
      glLineWidth(1);
      glBegin(GL_LINES);

      glVertex3f(xmin,ymin,zmin);
      glVertex3f(xmax,ymin,zmin);
      glVertex3f(xmax,ymin,zmin);
      glVertex3f(xmax,ymax,zmin);
      glVertex3f(xmax,ymax,zmin);
      glVertex3f(xmin,ymax,zmin);
      glVertex3f(xmin,ymax,zmin);
      glVertex3f(xmin,ymin,zmin);

      glVertex3f(xmax,ymin,zmin);
      glVertex3f(xmax,ymin,zmax);
      glVertex3f(xmax,ymin,zmax);
      glVertex3f(xmax,ymax,zmax);
      glVertex3f(xmax,ymax,zmax);
      glVertex3f(xmax,ymax,zmin);
      glVertex3f(xmax,ymax,zmin);
      glVertex3f(xmax,ymin,zmin);

      glVertex3f(xmin,ymin,zmin);
      glVertex3f(xmax,ymin,zmin);
      glVertex3f(xmax,ymin,zmin);
      glVertex3f(xmax,ymin,zmax);
      glVertex3f(xmax,ymin,zmax);
      glVertex3f(xmin,ymin,zmax);
      glVertex3f(xmin,ymin,zmax);
      glVertex3f(xmin,ymin,zmin);

      glVertex3f(xmin,ymin,zmax);
      glVertex3f(xmax,ymin,zmax);
      glVertex3f(xmax,ymin,zmax);
      glVertex3f(xmax,ymax,zmax);
      glVertex3f(xmax,ymax,zmax);
      glVertex3f(xmin,ymax,zmax);
      glVertex3f(xmin,ymax,zmax);
      glVertex3f(xmin,ymin,zmax);

      glVertex3f(xmin,ymin,zmin);
      glVertex3f(xmin,ymin,zmax);
      glVertex3f(xmin,ymin,zmax);
      glVertex3f(xmin,ymax,zmax);
      glVertex3f(xmin,ymax,zmax);
      glVertex3f(xmin,ymax,zmin);
      glVertex3f(xmin,ymax,zmin);
      glVertex3f(xmin,ymin,zmin);

      glVertex3f(xmin,ymax,zmin);
      glVertex3f(xmax,ymax,zmin);
      glVertex3f(xmax,ymax,zmin);
      glVertex3f(xmax,ymax,zmax);
      glVertex3f(xmax,ymax,zmax);
      glVertex3f(xmin,ymax,zmax);
      glVertex3f(xmin,ymax,zmax);
      glVertex3f(xmin,ymax,zmin);
      glEnd();
    }

  free(ptmin_aabb);
  free(ptmax_aabb);
  
  glFlush();
}


void display_ball_ritter()
{

  point3d *listcentre=NULL;
  double *listray=NULL;

  a2ri_vf_bounding_ball_ritter(modeltodisplay,facesmodeltodisplay,modeltodisplay->nbface,&listcentre,&listray);
  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
  for(int i=0;i<modeltodisplay->nbface;i++)
    {
      glTranslatef(listcentre[i].x,listcentre[i].y,listcentre[i].z);
      glutWireSphere(listray[i],10,10);
      glTranslatef(-listcentre[i].x,-listcentre[i].y,-listcentre[i].z);
    }
 
  free(listcentre);
  free(listray);

  listcentre=NULL;
  listray=NULL;


  a2ri_vf_bounding_ball_ritter(tomove,facestomove,tomove->nbface,&listcentre,&listray);
  glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
  for(int i=0;i<tomove->nbface;i++)
    {
      glTranslatef(listcentre[i].x,listcentre[i].y,listcentre[i].z);
      glutWireSphere(listray[i],10,10);
      glTranslatef(-listcentre[i].x,-listcentre[i].y,-listcentre[i].z);
    }
 
  free(listcentre);
  free(listray);

}


void 
transform (int i)
{
  glTranslatef((modeltodisplay->xmin+modeltodisplay->xmax)/2.0,
	       (modeltodisplay->ymin+modeltodisplay->ymax)/2.0,
	       (modeltodisplay->zmin+modeltodisplay->zmax)/2.0);
  glRotatef (hist[i][0], 1.0, 0.0, 0.0);
  glRotatef (hist[i][1], 0.0, 1.0, 0.0);
  glRotatef (hist[i][2], 0.0, 0.0, 1.0);
  glTranslatef(-(modeltodisplay->xmin+modeltodisplay->xmax)/2.0,
	       -(modeltodisplay->ymin+modeltodisplay->ymax)/2.0,
	       -(modeltodisplay->zmin+modeltodisplay->zmax)/2.0);
}

void 
display ()
{
  point3d ptmin,ptmax;
  if(modeltodisplay->xmin<tomove->xmin)
    ptmin.x=modeltodisplay->xmin;
  else
    ptmin.x=tomove->xmin;
  if(modeltodisplay->ymin<tomove->ymin)
    ptmin.y=modeltodisplay->ymin;
  else
    ptmin.y=tomove->ymin;
  if(modeltodisplay->zmin<tomove->zmin)
    ptmin.z=modeltodisplay->zmin;
  else
    ptmin.z=tomove->zmin;

  if(modeltodisplay->xmax>tomove->xmax)
    ptmax.x=modeltodisplay->xmax;
  else
    ptmax.x=tomove->xmax;
  if(modeltodisplay->ymax>tomove->ymax)
    ptmax.y=modeltodisplay->ymax;
  else
    ptmax.y=tomove->ymax;
  if(modeltodisplay->zmax>tomove->zmax)
    ptmax.z=modeltodisplay->zmax;
  else
    ptmax.z=tomove->zmax;


  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (45, WIDTH/((float)HEIGHT), 0.1, 900);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  if(modeltodisplay->zmax==0)
    gluLookAt (((modeltodisplay->xmin+modeltodisplay->xmax)/2.0)+tx,
	       ((modeltodisplay->ymin+modeltodisplay->ymax)/2.0)+ty, 
	       70+tz,
	       ((modeltodisplay->xmin+modeltodisplay->xmax)/2.0)-tx,
	       ((modeltodisplay->ymin+modeltodisplay->ymax)/2.0)+ty,
	       ((modeltodisplay->zmin+modeltodisplay->zmax)/2.0)+tz,
	       0, 1, 0);
  else
    gluLookAt (((modeltodisplay->xmin+modeltodisplay->xmax)/2.0)+tx,
	       ((modeltodisplay->ymin+modeltodisplay->ymax)/2.0)+ty,
	       modeltodisplay->zmax*1.5+tz,
	       ((modeltodisplay->xmin+modeltodisplay->xmax)/2.0)+tx,
	       ((modeltodisplay->ymin+modeltodisplay->ymax)/2.0)+ty,
	       ((modeltodisplay->zmin+modeltodisplay->zmax)/2.0)+tz,
	       0, 1, 0);
  
  transform (0);

  glTranslatef((ptmin.x+ptmax.x)/2.0,
	       (ptmin.y+ptmax.y)/2.0,
	       (ptmin.z+ptmax.z)/2.0);
  glRotatef(angle,0.0,1.0,0.0);
  glRotatef(angle2,1.0,0.0,0.0);
  glTranslatef(-(ptmin.x+ptmax.x)/2.0,
	       -(ptmin.y+ptmax.y)/2.0,
	       -(ptmin.z+ptmax.z)/2.0);

  display_triangles();
  if(affich_aabb)
    display_aabb();
  if(affich_ball_ritter)
    display_ball_ritter();

  glutSwapBuffers ();
}
   

void 
exitFunc ()
{
  a2ri_vf_free(modeltodisplay);
  a2ri_vf_free(tomove);
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
calcul_precision()
{
  double *distance=NULL,longueurmin,longueur;
  int size=0,*distance_int=NULL;

  free(liste_vert);
  liste_vert=NULL;
  free(liste_rouge);
  liste_rouge=NULL;
  size_liste_couleur=0;

  for(int i=0;i<tomove->nbvertex;i++)
    {
      point3d M,A;
      point3d_init(&M,tomove->ve[i].x,tomove->ve[i].y,tomove->ve[i].z);
      longueurmin=-1;

      for(int j=0;j<modeltodisplay->nbvertex;j++)
	{
	  point3d_init(&A,modeltodisplay->ve[j].x,modeltodisplay->ve[j].y,modeltodisplay->ve[j].z);
	  longueur=point3d_length(&M,&A);
	  if(longueur<longueurmin || longueurmin<0)
	    longueurmin=longueur;
	}
      list_double_add(&distance,&size,sqrt(longueurmin),WITH_REDUNDANCE);
    }


  double min=list_double_min(distance,size);
  double max=list_double_max(distance,size);
  
  size=0;
  for(int i=0;i<tomove->nbvertex;i++)
    {
      list_int_add(&distance_int,&size,(int)(((distance[i]-min)/(max-min))*65535.0),WITH_REDUNDANCE);
    }

  size=0;
  liste_vert=NULL;
  liste_rouge=NULL;
  size_liste_couleur=0;

  for(int i=0;i<tomove->nbvertex;i++)
    {
      if(distance_int[i]<32767)
	{
	  list_int_add(&liste_rouge,&size_liste_couleur,distance_int[i]/128,WITH_REDUNDANCE);
	  size_liste_couleur--;
	  list_int_add(&liste_vert,&size_liste_couleur,255,WITH_REDUNDANCE);
	}
      else
	{
	  list_int_add(&liste_rouge,&size_liste_couleur,255,WITH_REDUNDANCE);
	  size_liste_couleur--;
	  list_int_add(&liste_vert,&size_liste_couleur,255-((distance_int[i]-32767)/128),WITH_REDUNDANCE);
	}
    }
}

void 
keyboard (unsigned char key, int x, int y)
{
  int i, j;
  char fic[50];
  time_t dep,fin;

  point3d centre;
  centre.x=(tomove->xmin+tomove->xmax)/2.0;
  centre.y=(tomove->ymin+tomove->ymax)/2.0;
  centre.z=(tomove->zmin+tomove->zmax)/2.0;

  vector3d vtrans;
      
  switch (key)
    {
    case 'w':
      a2ri_vf_rotateX_degre_center(tomove,1,&centre);
      pertrotx+=1.0;
      break;
    case 'W':
      a2ri_vf_rotateX_degre_center(tomove,-1,&centre);
      pertrotx-=1.0;
      break;
    case 'x':
      a2ri_vf_rotateY_degre_center(tomove,1,&centre);
      pertroty+=1.0;
      break;
    case 'X':
      a2ri_vf_rotateY_degre_center(tomove,-1,&centre);
      pertroty-=1.0;
      break;
    case 'c':
      a2ri_vf_rotateZ_degre_center(tomove,1,&centre);
      pertrotz+=1.0;
      break;
    case 'C':
      a2ri_vf_rotateZ_degre_center(tomove,-1,&centre);
      pertrotz-=1.0;
      break;
    case 'v':
      vtrans.dx=(sqrt((tomove->xmin-tomove->xmax)*(tomove->xmin-tomove->xmax)+(tomove->ymin-tomove->ymax)*(tomove->ymin-tomove->ymax)+(tomove->zmin-tomove->zmax)*(tomove->zmin-tomove->zmax))/50.0);
      vtrans.dy=0;
      vtrans.dz=0;
      a2ri_vf_translate(tomove,&vtrans);
      perttx+=2.0;
      break;
    case 'V':
      vtrans.dx=-(sqrt((tomove->xmin-tomove->xmax)*(tomove->xmin-tomove->xmax)+(tomove->ymin-tomove->ymax)*(tomove->ymin-tomove->ymax)+(tomove->zmin-tomove->zmax)*(tomove->zmin-tomove->zmax))/50.0);
      vtrans.dy=0;
      vtrans.dz=0;
      a2ri_vf_translate(tomove,&vtrans);
      perttx-=2.0;
      break;
    case 'b':
      vtrans.dx=0;
      vtrans.dy=(sqrt((tomove->xmin-tomove->xmax)*(tomove->xmin-tomove->xmax)+(tomove->ymin-tomove->ymax)*(tomove->ymin-tomove->ymax)+(tomove->zmin-tomove->zmax)*(tomove->zmin-tomove->zmax))/50.0);
      vtrans.dz=0;
      a2ri_vf_translate(tomove,&vtrans);
      pertty+=2.0;
      break;
    case 'B':
      vtrans.dx=0;
      vtrans.dy=-(sqrt((tomove->xmin-tomove->xmax)*(tomove->xmin-tomove->xmax)+(tomove->ymin-tomove->ymax)*(tomove->ymin-tomove->ymax)+(tomove->zmin-tomove->zmax)*(tomove->zmin-tomove->zmax))/50.0);
      vtrans.dz=0;
      a2ri_vf_translate(tomove,&vtrans);
      pertty-=2.0;
      break;
    case 'n':
      vtrans.dx=0;
      vtrans.dy=0;
      vtrans.dz=(sqrt((tomove->xmin-tomove->xmax)*(tomove->xmin-tomove->xmax)+(tomove->ymin-tomove->ymax)*(tomove->ymin-tomove->ymax)+(tomove->zmin-tomove->zmax)*(tomove->zmin-tomove->zmax))/50.0);
      a2ri_vf_translate(tomove,&vtrans);
      perttz+=2.0;
      break;
    case 'N':
      vtrans.dx=0;
      vtrans.dy=0;
      vtrans.dz=-(sqrt((tomove->xmin-tomove->xmax)*(tomove->xmin-tomove->xmax)+(tomove->ymin-tomove->ymax)*(tomove->ymin-tomove->ymax)+(tomove->zmin-tomove->zmax)*(tomove->zmin-tomove->zmax))/50.0);
      a2ri_vf_translate(tomove,&vtrans);
      perttz-=2.0;
      break;
    case 's':
    case 'S' :
      printf("nom du fichier : ");
      a2ri_erreur_critique_si(!scanf("%s",fic),"erreur nom fichier");
      a2ri_vf_save_file(fic,tomove);
      break;
    case 'r':
    case 'R':
      printf("entrer le pourcentage de recouvrement (entre 0.0 et 1.0): ");
      a2ri_erreur_critique_si(!scanf("%lf",&recouvrement),"erreur recouvrement");
      break;
    case 'e':
    case 'E':
      printf("regler la sensibilite de l'algorithme automated_trimmed_cip (entre 0.0 et 1.0): ");
      a2ri_erreur_critique_si(!scanf("%lf",&sensibilite),"erreur sensibilite");
      break;
    case 'i':
    case 'I':
      dep=clock();
      a2ri_vf_icp(tomove,modeltodisplay,dkarret);
      fin=clock();
      printf("temps calcul recalage : %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
      break;
    case 't':
    case 'T':
      dep=clock();
      a2ri_vf_trimmed_icp(tomove,modeltodisplay,recouvrement,dkarret);
      fin=clock();
      printf("temps calcul recalage : %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
      break;
    case 'a':
    case 'A':
      dep=clock();
      a2ri_vf_automated_trimmed_icp(tomove,modeltodisplay,dkarret,sensibilite);
      fin=clock();
      printf("temps calcul recalage : %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
      break;
    case 'q':
    case 'Q':
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
  printf("vue:\nrotation : %lf , %lf , %lf / translation : %lf , %lf , %lf\n",rx,ry,rz,tx,ty,tz);
  printf("perturbation:\nrotation : %lf , %lf , %lf / translation : %lf , %lf , %lf\n",pertrotx,pertroty,pertrotz,perttx,pertty,perttz);
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
go (int argc, char **argv, vf_model *b, vf_model *m, double dkarr)
{
  point3d ptmin,ptmax;

  dkarret=dkarr;
  modeltodisplay=b;
  tomove=m;


  affich_aabb=0;
  affich_ball_ritter=0;
  
  recouvrement=1.0;
  sensibilite=0.3;

  facesmodeltodisplay=(int*)malloc(modeltodisplay->nbface*sizeof(int));
  for(int i=0;i<modeltodisplay->nbface;i++)
    facesmodeltodisplay[i]=i;

  facestomove=(int*)malloc(tomove->nbface*sizeof(int));
  for(int i=0;i<tomove->nbface;i++)
    facestomove[i]=i;


  rinc=R_INC;
  tinc=T_INC;
  if(modeltodisplay->zmax-modeltodisplay->zmin!=0)
    tinc=(modeltodisplay->zmax-modeltodisplay->zmin)/10;

  ptmin.x=modeltodisplay->xmin;
  ptmin.y=modeltodisplay->ymin;
  ptmin.z=modeltodisplay->zmin;
  ptmax.x=modeltodisplay->xmax;
  ptmax.y=modeltodisplay->ymax;
  ptmax.z=modeltodisplay->zmax;
  longdiag=point3d_length(&ptmin,&ptmax);

  affich_tomove=1;
  affich_display=1;

  init (argc, argv);
  
  glutDisplayFunc (display);
  glutKeyboardFunc (keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutIdleFunc (idleFunc);
  glutMainLoop ();
  
  return EXIT_SUCCESS;
}
    
