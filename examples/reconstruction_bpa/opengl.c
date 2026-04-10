#include "opengl.h"

vf_model *model;
double longdiag3d,longdiag2d,largeur,hauteur,ratiomodel;
hashtable *table;
int recharge,width,height;
GLdouble *vertices;
GLint *indices_face,*indices_arete;
int size_vertices,size_indices_face,size_indices_arete;
vector3d *normal;
double radius;
char* nomfic;


/*VARIABLE POUR LE BPA*/
bpa_edge *e;
int sigma_k,sigma_i,sigma_j,*listused=NULL,sizeused=0;
bpa_fronts *fronts=NULL;
point3d centre;

point3d pmin,pmax;
int nbpartX,nbpartY,nbpartZ;
space_partition *sp;



void 
init (int argc, char **argv)
{
  double vpx,vpy,vpz;
  GLfloat mat[]= {COUL_LUMIERE, 1.0f};
  GLfloat ambientlight[]={COUL_LUMIERE_AMBIENT,1.0f};
  GLfloat diffuselight[]={COUL_LUMIERE_DIFFUSE,1.0f};
  GLfloat specularlight[]={COUL_LUMIERE_SPECULAR,1.0f};
  GLfloat position[4];

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (WIDTH*2, HEIGHT*2);
  width=WIDTH;
  height=HEIGHT;
  glutInitWindowPosition (800, 25);
  glutCreateWindow ("titre_de_la_fenetre");

  glClearColor (255.0, 255.0, 255.0, 0.0);
  glEnable(GL_DEPTH_TEST); 
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  /*glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);*/
  glShadeModel(GL_SMOOTH);

  vpx=model->xmin;
  vpy=model->ymin;

  if(model->zmax!=0)
    vpz=model->zmax*1.5+zoom;
  else
    vpz=70;

  position[0]=vpx;position[1]=vpy;position[2]=vpz;position[3]=1.0;
  
  if(model->zmax<0)
    position[2]*=-1.0;

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientlight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuselight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularlight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

  glMaterialfv (GL_FRONT, GL_AMBIENT, mat);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, mat);
  glMaterialfv (GL_FRONT, GL_SPECULAR, mat);

  glViewport (0, 0, (GLsizei)WIDTH, (GLsizei)HEIGHT);

}

void
camera_positionner_tourner()
{
  glScalef(zoom,zoom,1.0);
  glTranslatef(tx,ty,0);
  glTranslatef((model->xmin+model->xmax)/2.0,
	       (model->ymin+model->ymax)/2.0,
	       (model->zmin+model->zmax)/2.0);
  glRotatef(angle,0.0,1.0,0.0);
  glRotatef(angle2,1.0,0.0,0.0);
  glTranslatef(-(model->xmin+model->xmax)/2.0,
	       -(model->ymin+model->ymax)/2.0,
	       -(model->zmin+model->zmax)/2.0);
}

void
liste_arete(int key, vf_edge *value, void* user_data)
{
  user_data=NULL;
  key=0;
  list_int_add(&indices_arete,&size_indices_arete,value->ve1,WITH_REDUNDANCE);
  list_int_add(&indices_arete,&size_indices_arete,value->ve2,WITH_REDUNDANCE);
}

void
display_triangle()
{ 
  glPolygonMode(GL_FRONT, GL_FILL); 
  if(recharge)
    {
      // on ne rechage le tableau de sommets, d'arete et de faces 
      // qu'au premier affichage ou si les sommets/artetes ou faces 
      // ont été modifiés. ne pas oublier de mettre la variable recharge
      // à 1 à l'endroit où les modifications sont faites...
      //
      // on peut meme faire mieux ! si on ne fait que de la translation 
      // de sommets, on peut répercuter les modifications directement 
      // dans le tableau concerné sans tout recharger.
      //
      // par contre si on supprimer un sommet ou une face, la il faut tout recharger !
      free(vertices);
      free(indices_face);
      free(indices_arete);
      vertices=NULL;
      indices_face=NULL;
      indices_arete=NULL;
      size_vertices=0;
      size_indices_face=0;
      size_indices_arete=0;
      vertices=(double*)malloc(3*model->nbvertex*sizeof(double));
      for(int i=0;i<model->nbvertex;i++)
	{
	  /*list_double_add(&vertices,&size_vertices,model->ve[i].x,WITH_REDUNDANCE);
	  list_double_add(&vertices,&size_vertices,model->ve[i].y,WITH_REDUNDANCE);
	  list_double_add(&vertices,&size_vertices,model->ve[i].z,WITH_REDUNDANCE);*/
	  vertices[i*3]=model->ve[i].x;
	  vertices[i*3+1]=model->ve[i].y;
	  vertices[i*3+2]=model->ve[i].z;
	}
      indices_face=(int*)malloc(3*model->nbface*sizeof(int));
      for(int i=0;i<model->nbface;i++)
	{
	  /*list_int_add(&indices_face,&size_indices_face,model->fa[i].ve1,WITH_REDUNDANCE);
	  list_int_add(&indices_face,&size_indices_face,model->fa[i].ve2,WITH_REDUNDANCE);
	  list_int_add(&indices_face,&size_indices_face,model->fa[i].ve3,WITH_REDUNDANCE);*/
	  indices_face[i*3]=model->fa[i].ve1;
	  indices_face[i*3+1]=model->fa[i].ve2;
	  indices_face[i*3+2]=model->fa[i].ve3;
	}
      size_indices_face=model->nbface*3;
      
      hashtable_free(table);
      free(table);
      table=NULL;
      table=a2ri_vf_construction_edge_table(model,NULL,0);
      hashtable_foreach(table,liste_arete,NULL);
      
      recharge=0;
    }
  
  //affichage des polygones
  glColor4f(COUL_TRIANGLE, 1.0f);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0, 1.0);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0, vertices);
  glDrawElements(GL_TRIANGLES, size_indices_face , GL_UNSIGNED_INT, indices_face);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisable(GL_POLYGON_OFFSET_FILL);
  
  //affichage des aretes
  glColor4f(COUL_ARETE, 1.0f);
  glPolygonMode(GL_FRONT, GL_LINE);
  glLineWidth(0.5);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0, vertices);
  glDrawElements(GL_LINES, size_indices_arete , GL_UNSIGNED_INT, indices_arete);
  glDisableClientState(GL_VERTEX_ARRAY);
  
  //affichage des sommets
  
  glColor4f(COUL_POINT, 1.0f);
  glPointSize(4.0);
  glBegin(GL_POINTS);
  for(int i=0;i<model->nbvertex;i++)
    glVertex3f(model->ve[i].x,model->ve[i].y,model->ve[i].z);
  glEnd();

  /*glColor4f(0.0,0.0,1.0, 1.0f);
  glLineWidth(1);
  glBegin(GL_LINES);
  for(int i=0;i<model->nbvertex;i++)
    {
      glVertex3f(model->ve[i].x,model->ve[i].y,model->ve[i].z);
      glVertex3f(model->ve[i].x+normal[i].dx,model->ve[i].y+normal[i].dy,model->ve[i].z+normal[i].dz);
    }
    glEnd();*/

  glColor4f(0.0,1.0,0.0, 1.0f);
  if(e!=NULL)
    {
    glTranslatef(e->cijo.x,e->cijo.y,e->cijo.z);
    glutSolidSphere(radius,10,10);
    glTranslatef(-e->cijo.x,-e->cijo.y,-e->cijo.z);
    }
  
  
  glFlush();
}



void mouse(int button, int state, int x, int y)
{
  if(button==GLUT_WHEEL_MOUSE_UP)
    {
      zoom*=1.1;
      glutPostRedisplay();
      return;
    }

  if(button==GLUT_WHEEL_MOUSE_DOWN)
    {
      zoom/=1.1;
      glutPostRedisplay();
      return ;
    } 

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
      translation=1;
      initx=x;
      inity=y;
    }

  if(button==GLUT_RIGHT_BUTTON && state==GLUT_UP)
    translation=0;
      
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

  if(translation)
    {
      tx+=(x-initx)*0.001*longdiag2d/zoom;
      ty-=(y-inity)*0.001*longdiag2d/zoom;
      initx=x;
      inity=y;
      glutPostRedisplay();
    }
}

void display()
{
  glClearColor(255.0,255.0,255.0,0.0);
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  camera_positionner_tourner();
  display_triangle();
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  ratiomodel=largeur/hauteur;
  ratioecran=(w*1.0)/(h*1.0);
  width=w;
  height=h;
  
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  if(ratioecran>ratiomodel)
    glOrtho(model->xmin-(((ratioecran*hauteur)-largeur)/2.0)-(longdiag2d/20.0),model->xmax+(((ratioecran*hauteur)-largeur)/2.0)+(longdiag2d/20.0),
	    model->ymin-(longdiag2d/20.0),model->ymax+(longdiag2d/20.0),
	    model->zmin-longdiag3d,model->zmax+longdiag3d);
  else
    glOrtho(model->xmin-(longdiag2d/20.0),model->xmax+(longdiag2d/20.0),
	    model->ymin-(((largeur/ratioecran)-hauteur)/2.0)-(longdiag2d/20.0),model->ymax+(((largeur/ratioecran)-hauteur)/2.0)+(longdiag2d/20.0),
	    model->zmin-longdiag3d,model->zmax+longdiag3d);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  camera_positionner_tourner();
  glutPostRedisplay();
}

void 
exitFunc ()
{
  hashtable_free(table);
  free(table);
  a2ri_vf_free(model);
  exit(EXIT_SUCCESS);
}

void a2ri_bpa_step()
{
  e=a2ri_bpa_get_active_edge_in_fronts(fronts);
  if(e!=NULL)
    {
      a2ri_bpa_ball_pivot(model,sp,normal,radius,e,listused,sizeused,fronts,&sigma_k,&centre);
      if(sigma_k!=-1)
	{
	  list_int_add(&listused,&sizeused,sigma_k,WITH_REDUNDANCE);
	  //printf("Ajout triangle %d : %d %d %d par pivot\n",model->nbface,e->sigma_i,sigma_k,e->sigma_j);
	  a2ri_vf_add_face(model,e->sigma_i,sigma_k,e->sigma_j);
	  sigma_i=e->sigma_i;sigma_j=e->sigma_j;
	  //a2ri_bpa_display_fronts(fronts);
	  a2ri_bpa_join(&e,sigma_k,centre);
	  //a2ri_bpa_display_fronts(fronts);
	  a2ri_bpa_regularization(&fronts,sigma_i,sigma_j,sigma_k);
	  //a2ri_bpa_display_fronts(fronts);
	  e=(bpa_edge*)malloc(sizeof(bpa_edge));
	  e->sigma_i=sigma_i;e->sigma_j=sigma_j;e->cijo=centre;
	}
      else
	{
	  e->state=BOUNDARY;
	  //printf("arete frontiere %d %d\n",e->sigma_i,e->sigma_j);
	}
      //a2ri_bpa_display_fronts(fronts);
    }
  else
    {
      if(a2ri_bpa_find_seed_triangle(model,sp,normal,radius,listused,sizeused,&sigma_i,&sigma_j,&sigma_k,&centre,0))
	{
	  e=(bpa_edge*)malloc(sizeof(bpa_edge));
	  e->sigma_i=sigma_i;e->sigma_j=sigma_j;e->cijo=centre;
	  //printf("Ajout triangle %d : %d %d %d par seed triangle\n",model->nbface,sigma_i,sigma_j,sigma_k);
	  a2ri_vf_add_face(model,sigma_i,sigma_j,sigma_k);
	  a2ri_bpa_free_fronts(&fronts);
	  fronts=NULL;
	  a2ri_bpa_new_front(&fronts,sigma_i,sigma_j,sigma_k,centre);
	  //a2ri_bpa_display_fronts(fronts);
	  list_int_add(&listused,&sizeused,sigma_i,WITH_REDUNDANCE);
	  list_int_add(&listused,&sizeused,sigma_j,WITH_REDUNDANCE);
	  list_int_add(&listused,&sizeused,sigma_k,WITH_REDUNDANCE);
	}
      else
	{
	  printf("plus de nouveau triangle trouve...\n");
	  return;
	}
    }
  //a2ri_bpa_display_fronts(fronts);
  //printf("\n\n");

  recharge=1;
  return;
}


void keyboard(unsigned char key, int x, int y)
{
  FILE *screenshot;
  unsigned short *pixels=NULL;
  char nom_fichier[100];
  double min,max,moyenne,sd;
  bpa_fronts *temp1;
  int *temp2, temp3;
  
  x=0;
  y=0;
  switch (key)
    {
      //touche entrée
    case 13:
      pixels=(unsigned short*)malloc(width*height*3*sizeof(unsigned short));
      glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_SHORT,pixels);
      printf("entrer le nom du fichier (sans extension) : ");
      a2ri_erreur_critique_si(!scanf("%s",nom_fichier),"erreur nom fichier\n");
      
      sprintf(nom_fichier,"%s.ppm",nom_fichier);
      screenshot=fopen(nom_fichier,"w");
      fprintf(screenshot,"P3\n %d %d\n65535\n",width,height);
      for(int j=height-1;j>=0;j--)
	for(int i=0;i<width;i++)
	  fprintf(screenshot,"%d %d %d ",pixels[(j*width+i)*3],pixels[(j*width+i)*3+1],pixels[(j*width+i)*3+2]);
      fclose(screenshot);
      break;
      //touche TAB
    case 't':
    case'T':
      a2ri_vf_bpa(model,normal,radius);
      a2ri_vf_save_file(nomfic,model);
      recharge=1;
      break;
    case 9:
      a2ri_bpa_step();
      break;
    case 'r':
      glutPostRedisplay();
      break;
    case 'f':
      a2ri_bpa_display_fronts(fronts);
      break;
    case 'p':
      a2ri_bpa_average_radius_suggestion(model,&min,&max,&moyenne,&sd);
      printf("rayon propose :\nmin : %lf - max : %lf - moyenne : %lf - ecart-type : %lf\n",min,max,moyenne,sd);
      radius=moyenne;
      printf("Le rayon de la boule a ete modifiee. rayon = %lf\n",radius);
      break;
    case 'h':
      a2ri_bpa_initialisation(model,&temp1,&temp2,&temp3);
      break;
    case 's':
    case 'S':
      a2ri_vf_save_file(nomfic,model);
      break;
      //touche echap
    case 27:
    case 'q':
    case 'Q':
      exitFunc();
      break;
    }
  glutPostRedisplay();
}



int go(int argc, char** argv, vf_model * b, double my_radius, char* ficsave)
{
  point3d ptmin,ptmax,infgauche,supgauche,infdroit;

  printf("radius : %lf\n",my_radius);

  //a2ri_bpa_initialisation(&b);

  recharge=1;
  vertices=NULL;
  indices_face=NULL;
  indices_arete=NULL;
  size_vertices=0;
  size_indices_face=0;
  size_indices_arete=0;

  /*vector3d trans;
    vector3d_init(&trans,0.5,0.5,0.5);*/

  model=b;
  radius=my_radius;
  nomfic=ficsave;

  table=a2ri_vf_construction_edge_table(model,NULL,0);

  if(model->nbface!=0)
    {
      /*Calcul des normales aux sommets*/
      normal=(vector3d*)malloc(model->nbvertex*sizeof(vector3d));
      for(int i=0;i<model->nbvertex;i++)
	{
	  normal[i]=a2ri_vf_normal_vertex_with_hashtable(model,i,table);
	  vector3d_normalize(&normal[i]);
	}
      /*Suppression des faces*/
      free(model->fa);
      model->nbface=0;
      model->fa=NULL;
    }

  //a2ri_vf_center(model);

  tx=0;ty=0;zoom=1;

  /*longdiag3d*/
  ptmin.x=model->xmin;
  ptmin.y=model->ymin;
  ptmin.z=model->zmin;
  ptmax.x=model->xmax;
  ptmax.y=model->ymax;
  ptmax.z=model->zmax;
  longdiag3d=point3d_length(&ptmin,&ptmax);


  /*INITIALISATION DE L'ALGORITHME*/
  /*CREATION DE LA SPACE PARTITION POUR OPTIMISATION DES RECHERCHES*/
  sp=(space_partition*)malloc(sizeof(space_partition));
  a2ri_erreur_critique_si(sp==NULL,
			  "Erreur allocation memoire pour sp\na2ri_bpa_find_seed_triangle\n");
  
  nbpartX=(int)((model->xmax-model->xmin)/(2*radius));
  if(nbpartX==0)
    nbpartX++;
  nbpartY=(int)((model->ymax-model->ymin)/(2*radius));
  if(nbpartY==0)
    nbpartY++;
  nbpartZ=(int)((model->zmax-model->zmin)/(2*radius));
  if(nbpartZ==0)
    nbpartZ++;
  space_partition_new(sp,&ptmin,&ptmax,nbpartX,nbpartY,nbpartZ);
  a2ri_vf_space_partition(model,sp);
  //space_partition_display(sp);

  /*largeur, hauteur, longdiag2d*/
  point3d_init(&infgauche,model->xmin,model->ymin,0);
  point3d_init(&supgauche,model->xmin,model->ymax,0);
  point3d_init(&infdroit,model->xmax,model->ymin,0);
  largeur=point3d_length(&infgauche,&infdroit);
  hauteur=point3d_length(&infgauche,&supgauche);
  longdiag2d=point3d_length(&supgauche,&infdroit);

  init(argc,argv);
  glutReshapeFunc (reshape);
  glutDisplayFunc(display); 
  glutMouseFunc (mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc (keyboard);
  glutMainLoop();
  return 0; 
}

