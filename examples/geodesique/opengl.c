#include <time.h>
#include "opengl.h"

#define VERTICES 0
#define WIREFRAME 1
#define SOLID 2


vf_model *model;
double longdiag3d,longdiag2d,moy_arete;
double largeur,hauteur,ratiomodel;
int *tabselectionne,size_tabselectionne,selectionne;
int index_tabselectionne;
int *list,size;
int mp,mp2;
hashtable *table;
char *nom_modele;


double *listeanglegauche=NULL;
double *listeangledroite=NULL;
int size_listeanglegauche=0,size_listeangledroite=0;
int *listedroite=NULL,size_listedroite=0;
int *listegauche=NULL,size_listegauche=0;
int affichage_plan;
point3d points_plan[3];
int affiche_normale;
int type_affichage;


void
planarite(double *betamoy, double *betasd)
{
  vector3d v1,v2;
  double *listangle=NULL;
  int sizelistangle=0;

  for(int i=1;i<size-1;i++)
    {
      vector3d_init(&v1,
		    model->ve[list[i-1]].x-model->ve[list[i]].x,
		    model->ve[list[i-1]].y-model->ve[list[i]].y,
		    model->ve[list[i-1]].z-model->ve[list[i]].z);

      vector3d_init(&v2,
		    model->ve[list[i+1]].x-model->ve[list[i]].x,
		    model->ve[list[i+1]].y-model->ve[list[i]].y,
		    model->ve[list[i+1]].z-model->ve[list[i]].z);


      list_double_add(&listangle,&sizelistangle,vector3d_angle_degre(&v1,&v2),WITH_REDUNDANCE);
    }

  *betamoy=list_double_average(listangle,sizelistangle);
  *betasd=sqrt(list_double_variance(listangle,sizelistangle));
}


void
planarite_straightest(double *thetamoy, double *thetasd, double *kappamoy, double *kappasd)
{

  vector3d v1,v2;
  int *listfaces=NULL,size_listfaces=0;
  double anglegauche=0,angledroite=0;
  free(listeangledroite);
  listeangledroite=NULL;
  size_listeangledroite=0;
  free(listeanglegauche);
  listeanglegauche=NULL;
  size_listeanglegauche=0;

  hashtable *table=a2ri_vf_construction_edge_table(model,NULL,0);
 
  
  for(int i=1;i<size-1;i++)
    {
      int numvertex=list[i];
      free(listfaces);
      listfaces=NULL;
      size_listfaces=0;
      a2ri_vf_faces_next_vertex_with_hashtable(model,numvertex,&listfaces,&size_listfaces,table);
      anglegauche=0;
      angledroite=0;
      for(int j=0;j<size_listfaces;j++)
	{
	  int numface=listfaces[j];
	  if(list_int_contains(listedroite,size_listedroite,numface)!=-1)
	    {
	      if(model->fa[numface].ve1==numvertex)
		{
		  vector3d_init(&v1,
				model->ve[model->fa[numface].ve2].x
				-model->ve[model->fa[numface].ve1].x,
				model->ve[model->fa[numface].ve2].y
				-model->ve[model->fa[numface].ve1].y,
				model->ve[model->fa[numface].ve2].z
				-model->ve[model->fa[numface].ve1].z);
		  vector3d_init(&v2,
				model->ve[model->fa[numface].ve3].x
				-model->ve[model->fa[numface].ve1].x,
				model->ve[model->fa[numface].ve3].y
				-model->ve[model->fa[numface].ve1].y,
				model->ve[model->fa[numface].ve3].z
				-model->ve[model->fa[numface].ve1].z);
		  angledroite+=vector3d_angle_degre(&v1,&v2);
		}
	      if(model->fa[numface].ve2==numvertex)
		{
		  vector3d_init(&v1,
				model->ve[model->fa[numface].ve1].x
				-model->ve[model->fa[numface].ve2].x,
				model->ve[model->fa[numface].ve1].y
				-model->ve[model->fa[numface].ve2].y,
				model->ve[model->fa[numface].ve1].z
				-model->ve[model->fa[numface].ve2].z);
		  vector3d_init(&v2,
				model->ve[model->fa[numface].ve3].x
				-model->ve[model->fa[numface].ve2].x,
				model->ve[model->fa[numface].ve3].y
				-model->ve[model->fa[numface].ve2].y,
				model->ve[model->fa[numface].ve3].z
				-model->ve[model->fa[numface].ve2].z);
		  angledroite+=vector3d_angle_degre(&v1,&v2);
		}
	      if(model->fa[numface].ve3==numvertex)
		{
		  vector3d_init(&v1,
				model->ve[model->fa[numface].ve1].x
				-model->ve[model->fa[numface].ve3].x,
				model->ve[model->fa[numface].ve1].y
				-model->ve[model->fa[numface].ve3].y,
				model->ve[model->fa[numface].ve1].z
				-model->ve[model->fa[numface].ve3].z);
		  vector3d_init(&v2,
				model->ve[model->fa[numface].ve2].x
				-model->ve[model->fa[numface].ve3].x,
				model->ve[model->fa[numface].ve2].y
				-model->ve[model->fa[numface].ve3].y,
				model->ve[model->fa[numface].ve2].z
				-model->ve[model->fa[numface].ve3].z);
		  angledroite+=vector3d_angle_degre(&v1,&v2);
		}
	    }
	  if(list_int_contains(listegauche,size_listegauche,numface)!=-1)
	    {
	      if(model->fa[numface].ve1==numvertex)
		{
		  vector3d_init(&v1,
				model->ve[model->fa[numface].ve2].x
				-model->ve[model->fa[numface].ve1].x,
				model->ve[model->fa[numface].ve2].y
				-model->ve[model->fa[numface].ve1].y,
				model->ve[model->fa[numface].ve2].z
				-model->ve[model->fa[numface].ve1].z);
		  vector3d_init(&v2,
				model->ve[model->fa[numface].ve3].x
				-model->ve[model->fa[numface].ve1].x,
				model->ve[model->fa[numface].ve3].y
				-model->ve[model->fa[numface].ve1].y,
				model->ve[model->fa[numface].ve3].z
				-model->ve[model->fa[numface].ve1].z);
		  anglegauche+=vector3d_angle_degre(&v1,&v2);
		}
	      if(model->fa[numface].ve2==numvertex)
		{
		  vector3d_init(&v1,
				model->ve[model->fa[numface].ve1].x
				-model->ve[model->fa[numface].ve2].x,
				model->ve[model->fa[numface].ve1].y
				-model->ve[model->fa[numface].ve2].y,
				model->ve[model->fa[numface].ve1].z
				-model->ve[model->fa[numface].ve2].z);
		  vector3d_init(&v2,
				model->ve[model->fa[numface].ve3].x
				-model->ve[model->fa[numface].ve2].x,
				model->ve[model->fa[numface].ve3].y
				-model->ve[model->fa[numface].ve2].y,
				model->ve[model->fa[numface].ve3].z
				-model->ve[model->fa[numface].ve2].z);
		  anglegauche+=vector3d_angle_degre(&v1,&v2);
		}
	      if(model->fa[numface].ve3==numvertex)
		{
		  vector3d_init(&v1,
				model->ve[model->fa[numface].ve1].x
				-model->ve[model->fa[numface].ve3].x,
				model->ve[model->fa[numface].ve1].y
				-model->ve[model->fa[numface].ve3].y,
				model->ve[model->fa[numface].ve1].z
				-model->ve[model->fa[numface].ve3].z);
		  vector3d_init(&v2,
				model->ve[model->fa[numface].ve2].x
				-model->ve[model->fa[numface].ve3].x,
				model->ve[model->fa[numface].ve2].y
				-model->ve[model->fa[numface].ve3].y,
				model->ve[model->fa[numface].ve2].z
				-model->ve[model->fa[numface].ve3].z);
		  anglegauche+=vector3d_angle_degre(&v1,&v2);
		}
	    }
	}
      list_double_add(&listeangledroite,&size_listeangledroite,angledroite,WITH_REDUNDANCE);
      list_double_add(&listeanglegauche,&size_listeanglegauche,anglegauche,WITH_REDUNDANCE);
    }
  free(listfaces);

  double *difference=NULL;
  int size_difference=0;
  double *kappa=NULL;
  int size_kappa=0;

  for(int i=0;i<size_listeangledroite;i++)
    {
      list_double_add(&difference,&size_difference,fabs(listeangledroite[i]-listeanglegauche[i]),WITH_REDUNDANCE);
      list_double_add(&kappa,&size_kappa,((2.0*PI)/(listeangledroite[i]+listeanglegauche[i]))*(((listeangledroite[i]+listeanglegauche[i])/2.0)-listeanglegauche[i]),WITH_REDUNDANCE);
    }


  //printf("difference droite gauche : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",list_double_average(difference,size_difference),sqrt(list_double_variance(difference,size_difference)));
  //printf("kappa : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",list_double_average(kappa,size_kappa),sqrt(list_double_variance(kappa,size_kappa)));
  *thetamoy=list_double_average(difference,size_difference);
  *thetasd=sqrt(list_double_variance(difference,size_difference));
  *kappamoy=list_double_average(kappa,size_kappa);
  *kappasd=sqrt(list_double_variance(kappa,size_kappa));
  free(difference);
  free(kappa);

  hashtable_free(table);
  free(table);
}


void
affiche_plan()
{
  affichage_plan=1;  
}



void tri_gauche_droite()
{

  hashtable *table=a2ri_vf_construction_edge_table(model,NULL,0);

  int *listegenerale=NULL,size_listegenerale=0;
  int *listtemp=NULL,size_listtemp=0;
  int trouve;
  
  listegauche=NULL;
  size_listegauche=0;
  listedroite=NULL;
  size_listedroite=0;
  
  
  for(int i=0;i<size;i++)
    {
      free(listtemp);
      listtemp=NULL;
      size_listtemp=0;
      a2ri_vf_faces_next_vertex_with_hashtable(model,list[i],&listtemp,&size_listtemp,table);
      for(int j=0;j<size_listtemp;j++)
        list_int_add(&listegenerale,&size_listegenerale,listtemp[j],WITHOUT_REDUNDANCE);
    
    }
  free(listtemp);

   
  for(int i=size-1;i>=0;i--)
    {
      int ve1=list[i];
      int ve2=list[i+1];
      for(int j=size_listegenerale-1;j>=0;j--)
	{
	  trouve=0;
	  if(model->fa[listegenerale[j]].ve1==ve1)
	    {
	      if(model->fa[listegenerale[j]].ve2==ve2)
		{
		  list_int_add(&listegauche,&size_listegauche,listegenerale[j],WITHOUT_REDUNDANCE);
		  trouve++;
		}
	      else
		{
		  if(model->fa[listegenerale[j]].ve3==ve2)
		    {
		      list_int_add(&listedroite,&size_listedroite,listegenerale[j],WITHOUT_REDUNDANCE);
		      trouve++;
		    }
		}
	    }
                
	  if(model->fa[listegenerale[j]].ve2==ve1)
	    {
	      if(model->fa[listegenerale[j]].ve3==ve2)
		{
		  list_int_add(&listegauche,&size_listegauche,listegenerale[j],WITHOUT_REDUNDANCE);
		  trouve++;
		}
	      else
		{
		  if(model->fa[listegenerale[j]].ve1==ve2)
		    {
		      list_int_add(&listedroite,&size_listedroite,listegenerale[j],WITHOUT_REDUNDANCE);
		      trouve++;
		    }
		}
	    }
                
	  if(model->fa[listegenerale[j]].ve3==ve1)
	    {
	      if(model->fa[listegenerale[j]].ve1==ve2)
		{
		  list_int_add(&listegauche,&size_listegauche,listegenerale[j],WITHOUT_REDUNDANCE);
		  trouve++;
		}
	      else
		{
		  if(model->fa[listegenerale[j]].ve2==ve2)
		    {
		      list_int_add(&listedroite,&size_listedroite,listegenerale[j],WITHOUT_REDUNDANCE);
		      trouve++;
		    }
		}
	    }
	  if(trouve)
	    list_int_remove(&listegenerale,&size_listegenerale,j);
	}
    }
    

  
  for(int i=size_listegenerale-1;i>=0;i--)
    {
      if(model->fa[listegenerale[i]].ve1==vedeb || model->fa[listegenerale[i]].ve1==vefin || model->fa[listegenerale[i]].ve2==vedeb || model->fa[listegenerale[i]].ve2==vefin || model->fa[listegenerale[i]].ve3==vedeb || model->fa[listegenerale[i]].ve3==vefin)
	list_int_remove(&listegenerale,&size_listegenerale,i);
    }
    

  //prendre la star des rouges et comparer avec la listegenerale. les faces en communs sont rouges
  //prendre la star des bleus et comparer avec la listegenerale. les faces en communs sont bleus
  //recommencer jusqu'a ce que la listegenerale soit vide
  while(size_listegenerale!=0)
    {
      int *listevoisindroite=NULL,size_listevoisindroite=0;
      int *listevoisingauche=NULL,size_listevoisingauche=0;
      a2ri_vf_star(BYEDGE,model,listedroite,size_listedroite,&listevoisindroite,&size_listevoisindroite,1);
      a2ri_vf_star(BYEDGE,model,listegauche,size_listegauche,&listevoisingauche,&size_listevoisingauche,1);
      for(int i=size_listegenerale-1;i>=0;i--)
	{
	  if(list_int_contains(listevoisindroite,size_listevoisindroite,listegenerale[i])!=-1)
	    {
	      list_int_add(&listedroite,&size_listedroite,listegenerale[i],WITH_REDUNDANCE);
	      list_int_remove(&listegenerale,&size_listegenerale,i);
	    }
	  else
	    {
	      if(list_int_contains(listevoisingauche,size_listevoisingauche,listegenerale[i])!=-1)
		{
		  list_int_add(&listegauche,&size_listegauche,listegenerale[i],WITH_REDUNDANCE);
		  list_int_remove(&listegenerale,&size_listegenerale,i);
		}
	    }
	}
      free(listevoisindroite);
      free(listevoisingauche);
    }
  free(listegenerale); 

  hashtable_free(table);
  free(table);
  
}


int
arete_appartient_a_list(int ve1,int ve2)
{
  for(int i=0;i<size-1;i++)
    if((list[i]==ve1 && list[i+1]==ve2) || (list[i]==ve2 && list[i+1]==ve1))
      return 1;

  return 0;
}

void
camera_positionner_tourner()
{

  //pour la ptite blague
  if(mp2)
    {
      angle+=rand_int(-2,2);
      angle2+=rand_int(-1,1);
    }
  glScalef(tz,tz,1.0);
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
affiche_arete(int key, vf_edge *value, void* user_data)
{
  key=0;
  user_data=NULL;
  if(arete_appartient_a_list(value->ve1,value->ve2))
    {
      glLineWidth(4);
      glColor4f(COUL_CHEMIN, 1.0f);   
    }
  else
    {
      glLineWidth(2);
      glColor4f(COUL_ARETE, 1.0f);
    }
  glBegin(GL_LINES);
  {
    glVertex3f(model->ve[value->ve1].x,
	       model->ve[value->ve1].y,
	       model->ve[value->ve1].z);
    glVertex3f(model->ve[value->ve2].x,
	       model->ve[value->ve2].y,
	       model->ve[value->ve2].z);
  }
  glEnd();
}



void
display_triangle()
{ 
  vector3d X,Y,Z,temp;
  
  if(type_affichage==SOLID)
    {
      glColor4f(COUL_TRIANGLE,1.0f); 
      glBegin(GL_TRIANGLES);
      {
        for(int i=0;i<model->nbface;i++)
          {
	    int ve1=model->fa[i].ve1;
	    int ve2=model->fa[i].ve2;
	    int ve3=model->fa[i].ve3;
	
	    glVertex3f(model->ve[ve1].x,
		       model->ve[ve1].y,
		       model->ve[ve1].z);
	    glVertex3f(model->ve[ve2].x,
		       model->ve[ve2].y,
		       model->ve[ve2].z);
	    glVertex3f(model->ve[ve3].x,
		       model->ve[ve3].y,
	               model->ve[ve3].z);
          }
      }
      glEnd();
    }

  if(type_affichage==WIREFRAME || type_affichage==SOLID)	
    hashtable_foreach(table,affiche_arete,NULL);
  
  for(int i=0;i<size_tabselectionne;i++)
    {
      glColor4f(COUL_TRIANGLE_DANS_SELEC,1.0f);
      if(i!=index_tabselectionne)
	{
	  glTranslatef(model->ve[tabselectionne[i]].x,
		       model->ve[tabselectionne[i]].y,
		       model->ve[tabselectionne[i]].z);
	  glutWireCube(longdiag3d/200.0);
	  glTranslatef(-model->ve[tabselectionne[i]].x,
		       -model->ve[tabselectionne[i]].y,
		       -model->ve[tabselectionne[i]].z);
	}
    }

  if(size_tabselectionne!=0)
    {
      glColor4f(COUL_TRIANGLE_SELEC,1.0f);
      
      glTranslatef(model->ve[selectionne].x,
		   model->ve[selectionne].y,
		   model->ve[selectionne].z);
      glutWireCube(longdiag3d/100.0);
      glTranslatef(-model->ve[selectionne].x,
		   -model->ve[selectionne].y,
		   -model->ve[selectionne].z);
    }

  if(vedeb!=-1)
    {
      glColor4f(COUL_SPHERE1,1.0f);
      glTranslatef(model->ve[vedeb].x,
		   model->ve[vedeb].y,
		   model->ve[vedeb].z);
      glutWireSphere(longdiag3d/200.0,10,10);
      glTranslatef(-model->ve[vedeb].x,
		   -model->ve[vedeb].y,
		   -model->ve[vedeb].z);
    }
  if(vefin!=-1)
    {
      glColor4f(COUL_SPHERE2,1.0f);
      glTranslatef(model->ve[vefin].x,
		   model->ve[vefin].y,
		   model->ve[vefin].z);
      glutWireSphere(longdiag3d/200.0,10,10);
      glTranslatef(-model->ve[vefin].x,
		   -model->ve[vefin].y,
		   -model->ve[vefin].z);
    }

  if(affiche_normale)
    for(int i=0;i<model->nbvertex;i++)
      {
	int *listface=NULL,size_listface=0;
	double data3_temp[3];
	gsl_matrix *mat_identity,*mat_normale,*mat_normale_trans,*mat_normale_carre,*mat_i,*mat_sous,*mat_mul;
	
	mat_identity=gsl_matrix_calloc(3,3);
	gsl_matrix_set_identity(mat_identity);
	
	
	a2ri_vf_faces_next_vertex_with_hashtable(model,i,&listface,&size_listface,table);
	vector3d_init(&Z,0,0,0);
	for(int i=0;i<size_listface;i++)
	  {
	    temp=a2ri_vf_normal_face(model,listface[i]);
	    Z.dx+=temp.dx;
	    Z.dy+=temp.dy;
	    Z.dz+=temp.dz;
	  }
	
	vector3d_normalize(&Z);
	
	data3_temp[0]=Z.dx;
	data3_temp[1]=Z.dy;
	data3_temp[2]=Z.dz;
	
      
	mat_normale=matrix_init(data3_temp,3,1);
	mat_normale_trans=matrix_transpose(mat_normale);
	mat_normale_carre=matrix_mul(mat_normale,mat_normale_trans);
	
	
	if((Z.dx==1 || Z.dx==-1) && Z.dy==0 && Z.dz==0)
	  {
	    //cas particulier
	    data3_temp[0]=0;
	    data3_temp[1]=1;
	    data3_temp[2]=0;
	  }
	else
	  {
	    data3_temp[0]=1;
	    data3_temp[1]=0;
	    data3_temp[2]=0;
	  }
	
	mat_i=matrix_init(data3_temp,3,1);
	
	mat_sous=matrix_sub(mat_identity,mat_normale_carre);
	
	mat_mul=matrix_mul(mat_sous,mat_i);
	
	X.dx=gsl_matrix_get(mat_mul,0,0);
	X.dy=gsl_matrix_get(mat_mul,1,0);
	X.dz=gsl_matrix_get(mat_mul,2,0);
	
	Y=vector3d_vectorialproduct(&X,&Z);
	
	vector3d_normalize(&X);
	vector3d_normalize(&Y);
	
	gsl_matrix_free(mat_mul);
	gsl_matrix_free(mat_sous);
	gsl_matrix_free(mat_normale_carre);
	gsl_matrix_free(mat_normale);
	gsl_matrix_free(mat_normale_trans);
	gsl_matrix_free(mat_identity);
	gsl_matrix_free(mat_i);
	
	glColor4f(0.0,0.0,1.0,1.0f);
	glBegin(GL_LINES);
	{
	  glVertex3f(model->ve[i].x,
		     model->ve[i].y,
		     model->ve[i].z);
	  glVertex3f(model->ve[i].x+(X.dx*longdiag3d/75.0),
		     model->ve[i].y+(X.dy*longdiag3d/75.0),
		     model->ve[i].z+(X.dz*longdiag3d/75.0));
	  glVertex3f(model->ve[i].x,
		     model->ve[i].y,
		     model->ve[i].z);
	  glVertex3f(model->ve[i].x+(Y.dx*longdiag3d/75.0),
		     model->ve[i].y+(Y.dy*longdiag3d/75.0),
		     model->ve[i].z+(Y.dz*longdiag3d/75.0));
	  glVertex3f(model->ve[i].x,
		     model->ve[i].y,
		     model->ve[i].z);
	  glVertex3f(model->ve[i].x+(Z.dx*longdiag3d/75.0),
		     model->ve[i].y+(Z.dy*longdiag3d/75.0),
		     model->ve[i].z+(Z.dz*longdiag3d/75.0));
	}
	glEnd();
	
	free(listface);
	
      }
  
  glFlush();
}

void
display_masque()
{  
  glColor4f(COUL_TRIANGLE,1.0f);
  for(int i=0;i<model->nbvertex;i++)
    {
      glLoadName (i);
      glBegin(GL_TRIANGLES);
      {
	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);

	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);

	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);

	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);

	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);
	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);

	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);
	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);
	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);

	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);
	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);

	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);
	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);

	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);

	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);
	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y+moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);

	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);

	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z+moy_arete/20.0);
	glVertex3f(model->ve[i].x+moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
	glVertex3f(model->ve[i].x-moy_arete/20.0,
		   model->ve[i].y-moy_arete/20.0,
		   model->ve[i].z-moy_arete/20.0);
      }
      glEnd();
    }
}


void
traite_tabselectionne()
{
  if(size_tabselectionne==0)
    return;

  if(size_tabselectionne==1)
    {
      selectionne=tabselectionne[0];
      index_tabselectionne=0;
      return;
    }

  index_tabselectionne=(index_tabselectionne+1)%size_tabselectionne;
  selectionne=tabselectionne[index_tabselectionne];
  printf("element %d / %d selectionne\n",index_tabselectionne+1,size_tabselectionne);
}

void processHits (GLuint hits, GLuint buffer[])
{
  GLuint names, *ptr;

  free(tabselectionne);
  tabselectionne=NULL;
  size_tabselectionne=0;
  selectionne=-1;

  printf("\n\nnombre d'elements sélectionnés = %d\n", hits);
  ptr=(GLuint*)buffer;
  for(unsigned int i=0;i<hits;i++)
    {
      names=*ptr;
      ptr+=3;
      for (unsigned int j=0;j<names;j++)
	list_int_add(&tabselectionne,&size_tabselectionne,*(ptr++),WITH_REDUNDANCE);
    }
  
  index_tabselectionne=-1;
  traite_tabselectionne();
}

void mouse(int button, int state, int x, int y)
{
  GLuint selectBuf[BUFSIZE];
  GLint hits;
  GLint viewport[4];


  if(button==GLUT_WHEEL_MOUSE_UP)
    {
      tz*=1.1;
      glutPostRedisplay();
      return;
    }

  if(button==GLUT_WHEEL_MOUSE_DOWN)
    {
      tz/=1.1;
      glutPostRedisplay();
      return ;
    } 

  if(button==GLUT_CENTER_BUTTON && state==GLUT_DOWN)
    {
      
      glGetIntegerv (GL_VIEWPORT, viewport);
      
      glSelectBuffer (BUFSIZE, selectBuf);
      (void) glRenderMode (GL_SELECT);
            
      glInitNames();
      glPushName(0);
      
      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();
      gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y), 5.0, 5.0, viewport);


      if(ratioecran>ratiomodel)
	glOrtho(model->xmin-(((ratioecran*hauteur)-largeur)/2.0)-(longdiag2d/20.0),model->xmax+(((ratioecran*hauteur)-largeur)/2.0)+(longdiag2d/20.0),
		model->ymin-(longdiag2d/20.0),model->ymax+(longdiag2d/20.0),
		model->zmin-longdiag3d,model->zmax+longdiag3d);
      else
	glOrtho(model->xmin-(longdiag2d/20.0),model->xmax+(longdiag2d/20.0),
		model->ymin-(((largeur/ratioecran)-hauteur)/2.0)-(longdiag2d/20.0),model->ymax+(((largeur/ratioecran)-hauteur)/2.0)+(longdiag2d/20.0),
		model->zmin-longdiag3d,model->zmax+longdiag3d);
      
      
      display_masque();
      
      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();
           

      if(ratioecran>ratiomodel)
    glOrtho(model->xmin-(((ratioecran*hauteur)-largeur)/2.0)-(longdiag2d/20.0),model->xmax+(((ratioecran*hauteur)-largeur)/2.0)+(longdiag2d/20.0),
	    model->ymin-(longdiag2d/20.0),model->ymax+(longdiag2d/20.0),
	    model->zmin-longdiag3d,model->zmax+longdiag3d);
  else
    glOrtho(model->xmin-(longdiag2d/20.0),model->xmax+(longdiag2d/20.0),
	    model->ymin-(((largeur/ratioecran)-hauteur)/2.0)-(longdiag2d/20.0),model->ymax+(((largeur/ratioecran)-hauteur)/2.0)+(longdiag2d/20.0),
	    model->zmin-longdiag3d,model->zmax+longdiag3d);
      

      glFlush ();
      
      hits = glRenderMode (GL_RENDER);
      processHits (hits, selectBuf);
      glutPostRedisplay();
    }

  if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
      rotation=1;
      if(mp)
	mp2=1;
      initx=x;
      inity=y;
    }

  if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
    {
      mp2=0;
      rotation=0;
    }

  if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    {
      deplacement=1;
      initx=x;
      inity=y;
    }

  if(button==GLUT_RIGHT_BUTTON && state==GLUT_UP)
    deplacement=0;
      
} 

void motion(int x, int y)
{
  if(rotation)
    {
      angle+=(x-initx)*0.5;
      angle2+=(y-inity)*0.5;
      initx=x;
      inity=y;
      
      //pour la ptite blague
      if(mp2)
	{
	  angle+=rand_int(-2,2);
	  angle2+=rand_int(-1,1);
	}
      
      glutPostRedisplay();
    }

  if(deplacement)
    {
      tx+=(x-initx)*0.001*longdiag2d/tz;
      ty-=(y-inity)*0.001*longdiag2d/tz;
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
  if(masque_triangle==0)
    display_triangle();
  else
    display_masque();
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  ratiomodel=largeur/hauteur;
  ratioecran=(w*1.0)/(h*1.0);
  
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


void keyboard(unsigned char key, int x, int y)
{
  point3d p1,p2;
  int index1,index2,*temp=NULL,sizetemp=0,echang;
  double longgeo,longgeo2,betamoy,betasd,thetamoy,thetasd,kappamoy,kappasd;
  double *gauss=NULL,*levi=NULL,*list_mean=NULL;
  //int size_gauss=0,size_levi=0,size_list_mean=0;
  vector3d *mean=NULL;
  //int size_mean=0;
  double H,K,*listk=NULL,*listh=NULL;
  int size_listk=0,size_listh=0;

  int nbsomm;
  time_t dep,fin;

  ptf_func_hashtable func[1];
  func[0]=a2ri_vf_update_length_edge;
  
  x=0;
  y=0;
  switch (key)
    {
    case 'w':
      type_affichage=(type_affichage+1)%(SOLID+1);
      break;
    case 'b':
    case 'B':
      if(vedeb!=-1)
	{
	  a2ri_vf_garimella(model,vedeb,&H,&K);
	  printf("H : %g - K : %g\n",H,K);
	  vefin=-1;
	}
      break;
    case 'v':
    case 'V':
      for(int i=0;i<model->nbvertex;i++)
	{
	  a2ri_vf_garimella(model,i,&H,&K);
	  list_double_add(&listk,&size_listk,K,WITH_REDUNDANCE);
	  list_double_add(&listh,&size_listh,H,WITH_REDUNDANCE);
	}
      printf("H : moyenne : %g - SD : %g\n",list_double_average(listh,size_listh),sqrt(list_double_variance(listh,size_listh)));
      printf("K : moyenne : %g - SD : %g\n",list_double_average(listk,size_listk),sqrt(list_double_variance(listk,size_listk)));
      vefin=-1;
      break;
    case 'n':
    case 'N':
      affiche_normale=(affiche_normale+1)%2;
      break;
    case 'a':
    case 'A':
      mp=(mp+1)%2;
      if(mp==0)
	mp2=0;
      break;
    case 'r':
    case 'R':
      vedeb=-1;
      vefin=-1;
      selectionne=-1;
      free(tabselectionne);
      tabselectionne=0;
      size_tabselectionne=0;
      free(list);
      list=NULL;
      size=0;
      break;
    case 13:
      if(selectionne!=-1)
	{
	  if(vedeb==-1)
	    vedeb=selectionne;
	  else
	    {
	      if(vefin==-1)
		vefin=selectionne;
	    }
	}
      selectionne=-1;
      free(tabselectionne);
      tabselectionne=0;
      size_tabselectionne=0;
      break;
      //case 'm':
      //case 'M':
      //masque_triangle=(masque_triangle+1)%2;
      //break;
    case 9:
      //touche TAB
      traite_tabselectionne();
      break;
    case 'l':
    case 'L':
      //LONGUEUR LINEAIRE
      point3d_init(&p1,model->ve[vedeb].x,model->ve[vedeb].y,model->ve[vedeb].z);
      point3d_init(&p2,model->ve[vefin].x,model->ve[vefin].y,model->ve[vefin].z);
      printf("longueur euclidienne %d -> %d : %6.3lf unités\n",vedeb,vefin,point3d_length(&p1,&p2));
      break;

    case 'c':
    case 'C':
      //LONGUEUR CURVILINEAIRE
      free(list);
      list=NULL;
      size=0;
      a2ri_vf_search_hole_contains(model,vedeb,vefin,&temp,&sizetemp);
      if(temp==NULL)
	{
	  printf("pas de chemin curvilinéaire disponible : verifier que les deux sommets soient bien sur un meme bord\n");
	  break;
	}
      index1=list_int_contains(temp,sizetemp,vedeb);
      index2=list_int_contains(temp,sizetemp,vefin);
      if(index1>index2)
	{
	  echang=index1;
	  index1=index2;
	  index2=echang;
	}
      longgeo=0;
      longgeo2=0;
      for(int i=index1;i<index2;i++)
	{
	  point3d_init(&p1,model->ve[temp[i]].x,model->ve[temp[i]].y,model->ve[temp[i]].z);
	  point3d_init(&p2,model->ve[temp[i+1]].x,model->ve[temp[i+1]].y,model->ve[temp[i+1]].z);
	  longgeo+=point3d_length(&p1,&p2);
	}
      for(int i=index2;i<index1+sizetemp;i++)
	{
	  point3d_init(&p1,model->ve[temp[i%sizetemp]].x,model->ve[temp[i%sizetemp]].y,model->ve[temp[i%sizetemp]].z);
	  point3d_init(&p2,model->ve[temp[(i+1)%sizetemp]].x,model->ve[temp[(i+1)%sizetemp]].y,model->ve[temp[(i+1)%sizetemp]].z);
	  longgeo2+=point3d_length(&p1,&p2);
	}
      if(longgeo<longgeo2)
	for(int i=index1;i<=index2;i++)
	  list_int_add(&list,&size,temp[i],WITH_REDUNDANCE);
      else
	{
	  for(int i=index2;i<=index1+sizetemp;i++)
	    list_int_add(&list,&size,temp[i%sizetemp],WITH_REDUNDANCE);
	  longgeo=longgeo2;
	}
      printf("distance curvilinéaire %d -> %d : %6.3lf unités\n",vedeb,vefin,longgeo);
      break;






    case 't':
    case 'T':
      printf("*********************************\n");
      printf("Subdivision homogene/reguliere 1:6\n");
      printf("nombre de sommets depart: %d\n",model->nbvertex);
      dep=clock();
      a2ri_vf_6_subdivision(model,1);
      fin=clock();
      printf("nombre de sommets fin: %d\n",model->nbvertex);
      printf("temps d'execution pour la subdivision reguliere 1:6 : %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
      printf("*********************************\n\n\n");
      vedeb=-1;
      vefin=-1;
      selectionne=-1;
      free(tabselectionne);
      tabselectionne=0;
      size_tabselectionne=0;
     
      hashtable_free(table);
      free(table);
      
      table=a2ri_vf_construction_edge_table(model,func,1);

      break;
      break;
    case 'o':
    case 'O':
      printf("*********************************\n");
      printf("méthode de Dijkstra\n");
      printf("nombre de sommets depart: %d\n",nbsomm=model->nbvertex);
      dep=clock();
      free(list);
      list=NULL;
      size=0;
      p1.x=model->ve[vedeb].x;
      p1.y=model->ve[vedeb].y;
      p1.z=model->ve[vedeb].z;
      p2.x=model->ve[vefin].x;
      p2.y=model->ve[vefin].y;
      p2.z=model->ve[vefin].z;
      printf("\nsommet de depart : %d -> sommet de fin : %d\n",vedeb,vefin);
      printf("longueur lineaire : %6.3lf\n",point3d_length(&p1,&p2));
      longgeo=a2ri_vf_dijkstra(model,vedeb,vefin,&list,&size);   
      printf("longueur geodesique : %6.3lf\n",longgeo);
      planarite(&betamoy,&betasd);
      printf("beta : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",betamoy,betasd);
      tri_gauche_droite();
      planarite_straightest(&thetamoy,&thetasd,&kappamoy,&kappasd);
      printf("difference droite gauche : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",thetamoy,thetasd);
      printf("kappa : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",kappamoy,kappasd);
      fin=clock();
      printf("temps d'execution: %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
      

      printf("*********************************\n\n\n");
      break;

    case 'y':
    case 'Y':
      printf("*********************************\n");
      printf("méthode approche_avant\n");
      printf("nombre de sommets depart: %d\n",nbsomm=model->nbvertex);
      dep=clock();
      free(list);
      list=NULL;
      size=0;
      p1.x=model->ve[vedeb].x;
      p1.y=model->ve[vedeb].y;
      p1.z=model->ve[vedeb].z;
      p2.x=model->ve[vefin].x;
      p2.y=model->ve[vefin].y;
      p2.z=model->ve[vefin].z;
      printf("\nsommet de depart : %d -> sommet de fin : %d\n",vedeb,vefin);
      printf("longueur lineaire : %6.3lf\n",point3d_length(&p1,&p2));
      longgeo=a2ri_vf_approche(model,vedeb,vefin,&list,&size);   
      printf("longueur geodesique : %6.3lf\n",longgeo);
      planarite(&betamoy,&betasd);
      printf("beta : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",betamoy,betasd);
      tri_gauche_droite();
      planarite_straightest(&thetamoy,&thetasd,&kappamoy,&kappasd);
      printf("difference droite gauche : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",thetamoy,thetasd);
      printf("kappa : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",kappamoy,kappasd);
      fin=clock();
      printf("temps d'execution: %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
      hashtable_free(table);
      free(table);
      
      table=a2ri_vf_construction_edge_table(model,func,1);
            
      printf("*********************************\n\n\n");
      break;

    case 'u':
    case 'U':
      printf("méthode approche_arriere\n");
      printf("nombre de sommets depart: %d\n",nbsomm=model->nbvertex);
      dep=clock();
      free(list);
      list=NULL;
      size=0;
      p1.x=model->ve[vedeb].x;
      p1.y=model->ve[vedeb].y;
      p1.z=model->ve[vedeb].z;
      p2.x=model->ve[vefin].x;
      p2.y=model->ve[vefin].y;
      p2.z=model->ve[vefin].z;
      printf("\nsommet de depart : %d -> sommet de fin : %d\n",vedeb,vefin);
      printf("longueur lineaire : %6.3lf\n",point3d_length(&p1,&p2));
      longgeo=a2ri_vf_approche(model,vefin,vedeb,&list,&size);   
      printf("longueur geodesique : %6.3lf\n",longgeo);
      planarite(&betamoy,&betasd);
      printf("beta : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",betamoy,betasd);
      tri_gauche_droite();
      planarite_straightest(&thetamoy,&thetasd,&kappamoy,&kappasd);
      printf("difference droite gauche : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",thetamoy,thetasd);
      printf("kappa : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",kappamoy,kappasd);
      fin=clock();
      printf("temps d'execution: %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
      hashtable_free(table);
      free(table);
      
      table=a2ri_vf_construction_edge_table(model,func,1);
            
      printf("*********************************\n\n\n");
      break;


    case 'i':
    case 'I':
      printf("*********************************\n");
      printf("méthode de approche + plan moyen\n");
      printf("nombre de sommets depart: %d\n",nbsomm=model->nbvertex);
      dep=clock();
      free(list);
      list=NULL;
      size=0;
      p1.x=model->ve[vedeb].x;
      p1.y=model->ve[vedeb].y;
      p1.z=model->ve[vedeb].z;
      p2.x=model->ve[vefin].x;
      p2.y=model->ve[vefin].y;
      p2.z=model->ve[vefin].z;
      printf("\nsommet de depart : %d -> sommet de fin : %d\n",vedeb,vefin);
      printf("longueur lineaire : %6.3lf\n",point3d_length(&p1,&p2));
      a2ri_vf_geodesic_path_approche_plan_moyen(model,vedeb,vefin,&list,&size,&longgeo,GEODESIC_DENSIFICATION_NB_SUBDIV);
      printf("longueur geodesique : %6.3lf\n",longgeo);
      planarite(&betamoy,&betasd);
      printf("beta : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",betamoy,betasd);   
      tri_gauche_droite(); 
      planarite_straightest(&thetamoy,&thetasd,&kappamoy,&kappasd);
      printf("difference droite gauche : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",thetamoy,thetasd);
      printf("kappa : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",kappamoy,kappasd);
      fin=clock();
      printf("nombre de sommets fin: %d\n",model->nbvertex);  
      printf("temps d'execution: %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
      hashtable_free(table);
      free(table);
      
      table=a2ri_vf_construction_edge_table(model,func,1);
      
      
      printf("*********************************\n\n\n");
      break;
    case 'p':
    case 'P':
      printf("*********************************\n");
      printf("méthode de Dijkstra + plan moyen\n");
      printf("nombre de sommets depart: %d\n",nbsomm=model->nbvertex);
      dep=clock();
      free(list);
      list=NULL;
      size=0;
      p1.x=model->ve[vedeb].x;
      p1.y=model->ve[vedeb].y;
      p1.z=model->ve[vedeb].z;
      p2.x=model->ve[vefin].x;
      p2.y=model->ve[vefin].y;
      p2.z=model->ve[vefin].z;
      printf("\nsommet de depart : %d -> sommet de fin : %d\n",vedeb,vefin);
      printf("longueur lineaire : %6.3lf\n",point3d_length(&p1,&p2));
      a2ri_vf_geodesic_path_dijkstra_plan_moyen(model,vedeb,vefin,&list,&size,&longgeo,GEODESIC_DENSIFICATION_NB_SUBDIV);
      printf("longueur geodesique : %6.3lf\n",longgeo);
      planarite(&betamoy,&betasd);
      printf("beta : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",betamoy,betasd);   
      tri_gauche_droite(); 
      planarite_straightest(&thetamoy,&thetasd,&kappamoy,&kappasd);
      printf("difference droite gauche : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",thetamoy,thetasd);
      printf("kappa : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",kappamoy,kappasd);
      fin=clock();
      printf("nombre de sommets fin: %d\n",model->nbvertex);  
      printf("temps d'execution: %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
      hashtable_free(table);
      free(table);
      
      table=a2ri_vf_construction_edge_table(model,func,1);

      printf("*********************************\n\n\n");
      break;
    case 'k':
    case 'K':
      printf("*********************************\n");
      printf("méthode de approche + plan minmax\n");
      printf("nombre de sommets depart: %d\n",nbsomm=model->nbvertex);
      dep=clock();
      free(list);
      list=NULL;
      size=0;
      p1.x=model->ve[vedeb].x;
      p1.y=model->ve[vedeb].y;
      p1.z=model->ve[vedeb].z;
      p2.x=model->ve[vefin].x;
      p2.y=model->ve[vefin].y;
      p2.z=model->ve[vefin].z;
      printf("\nsommet de depart : %d -> sommet de fin : %d\n",vedeb,vefin);
      printf("longueur lineaire : %6.3lf\n",point3d_length(&p1,&p2));
      a2ri_vf_geodesic_path_approche_plan_minmax(model,vedeb,vefin,&list,&size,&longgeo,GEODESIC_DENSIFICATION_NB_SUBDIV);
      printf("longueur geodesique : %6.3lf\n",longgeo);
      planarite(&betamoy,&betasd);
      printf("beta : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",betamoy,betasd);   
      tri_gauche_droite(); 
      planarite_straightest(&thetamoy,&thetasd,&kappamoy,&kappasd);
      printf("difference droite gauche : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",thetamoy,thetasd);
      printf("kappa : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",kappamoy,kappasd);
      fin=clock();
      printf("nombre de sommets fin: %d\n",model->nbvertex);  
      printf("temps d'execution: %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
      hashtable_free(table);
      free(table);
      
      table=a2ri_vf_construction_edge_table(model,func,1);





      printf("*********************************\n\n\n");
      break;
    case 'm':
    case 'M':
      printf("*********************************\n");
      printf("méthode de Dijkstra + plan minamx\n");
      printf("nombre de sommets depart: %d\n",nbsomm=model->nbvertex);
      dep=clock();
      free(list);
      list=NULL;
      size=0;
      p1.x=model->ve[vedeb].x;
      p1.y=model->ve[vedeb].y;
      p1.z=model->ve[vedeb].z;
      p2.x=model->ve[vefin].x;
      p2.y=model->ve[vefin].y;
      p2.z=model->ve[vefin].z;
      printf("\nsommet de depart : %d -> sommet de fin : %d\n",vedeb,vefin);
      printf("longueur lineaire : %6.3lf\n",point3d_length(&p1,&p2));
      a2ri_vf_geodesic_path_dijkstra_plan_minmax(model,vedeb,vefin,&list,&size,&longgeo,GEODESIC_DENSIFICATION_NB_SUBDIV);
      printf("longueur geodesique : %6.3lf\n",longgeo);
      planarite(&betamoy,&betasd);
      printf("beta : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",betamoy,betasd);   
      tri_gauche_droite(); 
      planarite_straightest(&thetamoy,&thetasd,&kappamoy,&kappasd);
      printf("difference droite gauche : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",thetamoy,thetasd);
      printf("kappa : moyenne --> %6.3lf  --  ecart type : %6.3lf\n",kappamoy,kappasd);
      fin=clock();
      printf("nombre de sommets fin: %d\n",model->nbvertex);  
      printf("temps d'execution: %lf\n",((fin-dep)*1.0)/(CLOCKS_PER_SEC*1.0));
      hashtable_free(table);
      free(table);
      
      table=a2ri_vf_construction_edge_table(model,func,1);




      printf("*********************************\n\n\n");
      break;


    case 27:
    case 'q':
    case 'Q':
      exitFunc();
      break;
    }
  
  glutPostRedisplay();
  free(levi);
  free(mean);
  free(list_mean);
  free(gauss);
}


void 
idleFunc ()
{ 
  glutPostRedisplay ();
}

void
somme_longueur(int key, vf_edge *value, void* user_data)
{
  double *somme=user_data;
  key=0;
  *somme=(*somme)+value->att_double;
}


int go(int argc, char** argv,vf_model *b, int ve1, int ve2)
{
  point3d ptmin,ptmax;

  point3d infgauche,supgauche,infdroit;

  model=b;

  a2ri_vf_center(model);

  ptf_func_hashtable func[1];
  func[0]=a2ri_vf_update_length_edge;
  table=a2ri_vf_construction_edge_table(model,func,1);


  moy_arete=0;

  hashtable_foreach(table,somme_longueur,&moy_arete);

  moy_arete=moy_arete/(hashtable_size(table)*1.0);

  vedeb=ve1;
  vefin=ve2;

  tx=0;ty=0;tz=1;
  tabselectionne=NULL;
  size_tabselectionne=0;
  selectionne=-1;
  masque_triangle=0;
  list=NULL;
  size=0;

  mp=0;
  mp2=0;
  srand(time(NULL));

  ptmin.x=model->xmin;
  ptmin.y=model->ymin;
  ptmin.z=model->zmin;
  ptmax.x=model->xmax;
  ptmax.y=model->ymax;
  ptmax.z=model->zmax;
  longdiag3d=point3d_length(&ptmin,&ptmax);

  point3d_init(&infgauche,model->xmin,model->ymin,0);
  point3d_init(&supgauche,model->xmin,model->ymax,0);
  point3d_init(&infdroit,model->xmax,model->ymin,0);

  largeur=point3d_length(&infgauche,&infdroit);
  hauteur=point3d_length(&infgauche,&supgauche);
  longdiag2d=point3d_length(&supgauche,&infdroit);
  affiche_normale=0;
  type_affichage=SOLID;

  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
  glutInitWindowSize (WIDTH, HEIGHT);
  glutInitWindowPosition (0, 0);
  glutCreateWindow (argv[1]);
  nom_modele=argv[1];
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glutReshapeFunc (reshape);
  glutDisplayFunc(display); 
  glutMouseFunc (mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc (keyboard);
  glutIdleFunc (idleFunc);
  glutMainLoop();
  return 0; 
}

