#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <a2ri/io.h>
#include <a2ri/subdivision.h>
#include <a2ri/geometry.h>
#include <a2ri/vector.h>

void usage(char* argv[], char* type)
{
  if(!strcmp(type,"general"))
    {
      printf("usage :\n%s <grille|boite|icosphere|uvsphere|cylindre|cone> <argument> <nom fichier>\n",argv[0]);
      printf("--> %s grille <largeur> <hauteur> <nbcase largeur> <nbcase hauteur> <nom fichier>\n",argv[0]);
      printf("--> %s cylindre <rayon> <hauteur> <nb segment> <nb anneau> <nom fichier>\n",argv[0]);
      printf("--> %s cone <rayon> <hauteur> <nb segment> <nb anneau> <nom fichier>\n",argv[0]);
      printf("--> %s uvsphere <rayon> <nb segment> <nb anneau> <nom fichier>\n",argv[0]);
      printf("--> %s icosphere <rayon> <nb subdivision> <nom fichier>\n",argv[0]);
      printf("--> %s boite <tailleX> <tailleY> <tailleZ> <nbcaseX> <nbcaseY> <nbcaseZ> <nom fichier>\n",argv[0]);
      exit(EXIT_FAILURE);
    }
  if(!strcmp(type,"grille"))
    {
      printf("usage :\n%s grille <largeur> <hauteur> <nbcase largeur> <nbcase hauteur> <nom fichier>\n",argv[0]);
      exit(EXIT_FAILURE);
    }
  if(!strcmp(type,"boite"))
    {
      printf("usage :\n%s boite <tailleX> <tailleY> <tailleZ> <nbcaseX> <nbcaseY> <nbcaseZ> <nom fichier>\n",argv[0]);
      exit(EXIT_FAILURE);
    }
  if(!strcmp(type,"icosphere"))
    {
      printf("usage :\n%s icosphere <rayon> <nb subdivision> <nom fichier>\n",argv[0]);
      exit(EXIT_FAILURE);
    }
  if(!strcmp(type,"uvsphere"))
    {
      printf("usage :\n%s uvsphere <rayon> <nb segment> <nb anneau> <nom fichier>\n",argv[0]);
      exit(EXIT_FAILURE);
    }
  if(!strcmp(type,"cylindre"))
    {
      printf("usage :\n%s cylindre <rayon> <hauteur> <nb segment> <nb anneau> <nom fichier>\n",argv[0]);
      exit(EXIT_FAILURE);
    }
  if(!strcmp(type,"cone"))
    {
      printf("usage :\n%s cone <rayon> <hauteur> <nb segment> <nb anneau> <nom fichier>\n",argv[0]);
      exit(EXIT_FAILURE);
    }
}

/*************************************************************/

void
gen_grille(char* argv[], vf_model *primitive)
{
  double tailleh=atof(argv[2]),taillev=atof(argv[3]);
  int nbcaseh=atoi(argv[4]),nbcasev=atoi(argv[5]);
  float dh=tailleh/(nbcaseh*1.),dv=taillev/(nbcasev*1.);

  for(int j=0;j<=nbcasev;j++)
    for(int i=0;i<=nbcaseh;i++)
      a2ri_vf_add_vertex(primitive,i*dh,j*dv,0);

  for(int j=0;j<nbcasev;j++)
    for(int i=0;i<nbcaseh;i++)
      {
	//printf("creation face : %d %d %d\n",j*(nbcaseh+1)+i,j*(nbcaseh+1)+i+1,(j+1)*(nbcaseh+1)+i);
	a2ri_vf_add_face(primitive,j*(nbcaseh+1)+i,j*(nbcaseh+1)+i+1,(j+1)*(nbcaseh+1)+i);
	//printf("creation face : %d %d %d\n",j*(nbcaseh+1)+i+1,(j+1)*(nbcaseh+1)+i+1,(j+1)*(nbcaseh+1)+i);
	a2ri_vf_add_face(primitive,j*(nbcaseh+1)+i+1,(j+1)*(nbcaseh+1)+i+1,(j+1)*(nbcaseh+1)+i);
      }	  
}

void
gen_boite(char* argv[], vf_model *primitive)
{
  double tailleX=atof(argv[2]),tailleY=atof(argv[3]),tailleZ=atof(argv[4]);
  int nbcaseX=atoi(argv[5]),nbcaseY=atoi(argv[6]),nbcaseZ=atoi(argv[7]);
  double dX=tailleX/(nbcaseX*1.),dY=tailleY/(nbcaseY*1.),dZ=tailleZ/(nbcaseZ*1.);
  int nbpt_1,nbpt_2,nbpt_3,nbpt_4,nbpt_5;

  if(nbcaseX<1 || nbcaseY<1 || nbcaseZ<1)
    {
      printf("impossible de generer une boite dont le nombre de case suivant la direction X, Y ou Z est inférieur a 1\n");
      exit(EXIT_FAILURE);
    }

  //sommets avant/arriere
  for(int j=0;j<=nbcaseY;j++)
    for(int i=0;i<=nbcaseX;i++)
      a2ri_vf_add_vertex(primitive,i*dX,j*dY,0);

  nbpt_1=primitive->nbvertex;

  for(int j=0;j<=nbcaseY;j++)
    for(int i=0;i<=nbcaseX;i++)
      a2ri_vf_add_vertex(primitive,i*dX,j*dY,tailleZ);

  nbpt_2=primitive->nbvertex;

  //sommets dessous/dessus
  for(int k=1;k<nbcaseZ;k++)
    for(int i=0;i<=nbcaseX;i++)
      a2ri_vf_add_vertex(primitive,i*dX,0,k*dZ);

  nbpt_3=primitive->nbvertex;

  for(int k=1;k<nbcaseZ;k++)
    for(int i=0;i<=nbcaseX;i++)
      a2ri_vf_add_vertex(primitive,i*dX,tailleY,k*dZ);

  nbpt_4=primitive->nbvertex;

  //sommets gauche/droite
  for(int j=1;j<nbcaseY;j++)
    for(int k=1;k<nbcaseZ;k++)
      a2ri_vf_add_vertex(primitive,0,j*dY,k*dZ);

  nbpt_5=primitive->nbvertex;

  for(int j=1;j<nbcaseY;j++)
    for(int k=1;k<nbcaseZ;k++)
      a2ri_vf_add_vertex(primitive,tailleX,j*dY,k*dZ);

  //faces avant/arriere
  for(int j=0;j<nbcaseY;j++)
    for(int i=0;i<nbcaseX;i++)
      {
	a2ri_vf_add_face(primitive,j*(nbcaseX+1)+i,(j+1)*(nbcaseX+1)+i,j*(nbcaseX+1)+i+1);
	a2ri_vf_add_face(primitive,j*(nbcaseX+1)+i+1,(j+1)*(nbcaseX+1)+i,(j+1)*(nbcaseX+1)+i+1);
	
	a2ri_vf_add_face(primitive,j*(nbcaseX+1)+i+nbpt_1,j*(nbcaseX+1)+i+1+nbpt_1,(j+1)*(nbcaseX+1)+i+nbpt_1);
	a2ri_vf_add_face(primitive,j*(nbcaseX+1)+i+1+nbpt_1,(j+1)*(nbcaseX+1)+i+1+nbpt_1,(j+1)*(nbcaseX+1)+i+nbpt_1);
      }

  //faces dessous/dessus
  if(nbcaseZ>2)
    {
      for(int k=0;k<nbcaseZ-2;k++)
	for(int i=0;i<nbcaseX;i++)
	  {
	    a2ri_vf_add_face(primitive,k*(nbcaseX+1)+i+nbpt_2,k*(nbcaseX+1)+i+1+nbpt_2,(k+1)*(nbcaseX+1)+i+nbpt_2);
	    a2ri_vf_add_face(primitive,k*(nbcaseX+1)+i+1+nbpt_2,(k+1)*(nbcaseX+1)+i+1+nbpt_2,(k+1)*(nbcaseX+1)+i+nbpt_2);
	    
	    a2ri_vf_add_face(primitive,k*(nbcaseX+1)+i+nbpt_3,(k+1)*(nbcaseX+1)+i+nbpt_3,k*(nbcaseX+1)+i+1+nbpt_3);
	    a2ri_vf_add_face(primitive,k*(nbcaseX+1)+i+1+nbpt_3,(k+1)*(nbcaseX+1)+i+nbpt_3,(k+1)*(nbcaseX+1)+i+1+nbpt_3);
	  }
    }
      
  if(nbcaseZ>1)
    {
      for(int i=0;i<nbcaseX;i++)
	{
	  a2ri_vf_add_face(primitive,i,i+1,nbpt_2+i);
	  a2ri_vf_add_face(primitive,i+1,nbpt_2+i+1,nbpt_2+i);
	  
	  a2ri_vf_add_face(primitive,nbpt_1+i,nbpt_2+(nbcaseX+1)*(nbcaseZ-2)+i+1,nbpt_1+i+1);
	  a2ri_vf_add_face(primitive,nbpt_1+i,nbpt_2+(nbcaseX+1)*(nbcaseZ-2)+i,nbpt_2+(nbcaseX+1)*(nbcaseZ-2)+i+1);
	  
	  a2ri_vf_add_face(primitive,i+(nbcaseX+1)*(nbcaseY),nbpt_3+i,i+1+(nbcaseX+1)*(nbcaseY));
	  a2ri_vf_add_face(primitive,i+1+(nbcaseX+1)*(nbcaseY),nbpt_3+i,nbpt_3+i+1);
	  
	  a2ri_vf_add_face(primitive,nbpt_1+i+(nbcaseX+1)*(nbcaseY),nbpt_1+i+1+(nbcaseX+1)*(nbcaseY),nbpt_3+(nbcaseX+1)*(nbcaseZ-2)+i+1);
	  a2ri_vf_add_face(primitive,nbpt_1+i+(nbcaseX+1)*(nbcaseY),nbpt_3+(nbcaseX+1)*(nbcaseZ-2)+i+1,nbpt_3+(nbcaseX+1)*(nbcaseZ-2)+i);     
	  
	}
    }
  else
    {
      for(int i=0;i<nbcaseX;i++)
	{
	  a2ri_vf_add_face(primitive,i,i+1,nbpt_1+i);
	  a2ri_vf_add_face(primitive,i+1,nbpt_1+i+1,nbpt_1+i);

	  a2ri_vf_add_face(primitive,i+(nbcaseX+1)*(nbcaseY),nbpt_1+i+(nbcaseX+1)*(nbcaseY),i+1+(nbcaseX+1)*(nbcaseY));
	  a2ri_vf_add_face(primitive,i+1+(nbcaseX+1)*(nbcaseY),nbpt_1+i+(nbcaseX+1)*(nbcaseY),nbpt_1+i+1+(nbcaseX+1)*(nbcaseY));
	}
    }

  //faces gauche/droite
  if(nbcaseY>2)
    {
      for(int j=0;j<nbcaseY-2;j++)
	for(int k=0;k<nbcaseZ-2;k++)
	  {
	    a2ri_vf_add_face(primitive,j*(nbcaseZ-1)+k+nbpt_4,j*(nbcaseZ-1)+k+1+nbpt_4,(j+1)*(nbcaseZ-1)+k+nbpt_4);
	    a2ri_vf_add_face(primitive,j*(nbcaseZ-1)+k+1+nbpt_4,(j+1)*(nbcaseZ-1)+k+1+nbpt_4,(j+1)*(nbcaseZ-1)+k+nbpt_4);
	    
	    a2ri_vf_add_face(primitive,j*(nbcaseZ-1)+k+nbpt_5,(j+1)*(nbcaseZ-1)+k+nbpt_5,j*(nbcaseZ-1)+k+1+nbpt_5);
	    a2ri_vf_add_face(primitive,j*(nbcaseZ-1)+k+1+nbpt_5,(j+1)*(nbcaseZ-1)+k+nbpt_5,(j+1)*(nbcaseZ-1)+k+1+nbpt_5);
	  }
    }

  if(nbcaseY>1 && nbcaseZ>1)
    {
      for(int k=0;k<nbcaseZ-2;k++)
	{
	  a2ri_vf_add_face(primitive,nbpt_4+k,nbpt_2+(k+1)*(nbcaseX+1),nbpt_4+k+1);
	  a2ri_vf_add_face(primitive,nbpt_4+k,nbpt_2+k*(nbcaseX+1),nbpt_2+(k+1)*(nbcaseX+1));
	  
	  a2ri_vf_add_face(primitive,nbpt_4+(nbcaseZ-1)*(nbcaseY-2)+k,nbpt_4+(nbcaseZ-1)*(nbcaseY-2)+k+1,nbpt_3+k*(nbcaseX+1));
	  a2ri_vf_add_face(primitive,nbpt_3+(k+1)*(nbcaseX+1),nbpt_3+k*(nbcaseX+1),nbpt_4+(nbcaseZ-1)*(nbcaseY-2)+k+1);
	  
	  a2ri_vf_add_face(primitive,nbpt_5+k,nbpt_5+k+1,nbpt_2+(k+1)*(nbcaseX+1)+nbcaseX);
	  a2ri_vf_add_face(primitive,nbpt_5+k,nbpt_2+(k+1)*(nbcaseX+1)+nbcaseX,nbpt_2+k*(nbcaseX+1)+nbcaseX);
	  
	  a2ri_vf_add_face(primitive,nbpt_5+(nbcaseZ-1)*(nbcaseY-2)+k,nbpt_3+k*(nbcaseX+1)+nbcaseX,nbpt_5+(nbcaseZ-1)*(nbcaseY-2)+k+1);
	  a2ri_vf_add_face(primitive,nbpt_3+(k+1)*(nbcaseX+1)+nbcaseX,nbpt_5+(nbcaseZ-1)*(nbcaseY-2)+k+1,nbpt_3+k*(nbcaseX+1)+nbcaseX);
	}

      for(int j=0;j<nbcaseY-2;j++)
	{
	  a2ri_vf_add_face(primitive,(nbcaseX+1)*(j+2),(nbcaseX+1)*(j+1),nbpt_4+(nbcaseZ-1)*j);
	  a2ri_vf_add_face(primitive,(nbcaseX+1)*(j+2),nbpt_4+(nbcaseZ-1)*j,nbpt_4+(nbcaseZ-1)*(j+1));
	  
	  a2ri_vf_add_face(primitive,nbpt_1+(nbcaseX+1)*(j+2),nbpt_4+(nbcaseZ-1)*(j+2)-1,nbpt_1+(nbcaseX+1)*(j+1));
	  a2ri_vf_add_face(primitive,nbpt_1+(nbcaseX+1)*(j+1),nbpt_4+(nbcaseZ-1)*(j+2)-1,nbpt_4+(nbcaseZ-1)*(j+1)-1);
	  
	  a2ri_vf_add_face(primitive,(nbcaseX+1)*(j+2)+nbcaseX,nbpt_5+(nbcaseZ-1)*j,(nbcaseX+1)*(j+1)+nbcaseX);
	  a2ri_vf_add_face(primitive,(nbcaseX+1)*(j+2)+nbcaseX,nbpt_5+(nbcaseZ-1)*(j+1),nbpt_5+(nbcaseZ-1)*j);
	  
	  a2ri_vf_add_face(primitive,nbpt_1+(nbcaseX+1)*(j+2)+nbcaseX,nbpt_1+(nbcaseX+1)*(j+1)+nbcaseX,nbpt_5+(nbcaseZ-1)*(j+2)-1);
	  a2ri_vf_add_face(primitive,nbpt_1+(nbcaseX+1)*(j+1)+nbcaseX,nbpt_5+(nbcaseZ-1)*(j+1)-1,nbpt_5+(nbcaseZ-1)*(j+2)-1);
	}
    }
  else
    {
      if(nbcaseY==1 && nbcaseZ>1)
	{
	  a2ri_vf_add_face(primitive,nbcaseX+1,0,nbpt_2);
	  a2ri_vf_add_face(primitive,nbcaseX+1,nbpt_2,nbpt_3);

	  a2ri_vf_add_face(primitive,2*nbcaseX+1,nbpt_2+nbcaseX,nbcaseX);
	  a2ri_vf_add_face(primitive,2*nbcaseX+1,nbpt_3+nbcaseX,nbpt_2+nbcaseX);

	  for(int i=0;i<nbcaseZ-2;i++)
	    {
	      a2ri_vf_add_face(primitive,nbpt_3+i*(nbcaseX+1),nbpt_2+(i+1)*(nbcaseX+1),nbpt_3+(i+1)*(nbcaseX+1));
	      a2ri_vf_add_face(primitive,nbpt_3+i*(nbcaseX+1),nbpt_2+i*(nbcaseX+1),nbpt_2+(i+1)*(nbcaseX+1));

	      a2ri_vf_add_face(primitive,nbpt_3+i*(nbcaseX+1)+nbcaseX,nbpt_3+(i+1)*(nbcaseX+1)+nbcaseX,nbpt_2+(i+1)*(nbcaseX+1)+nbcaseX);
	      a2ri_vf_add_face(primitive,nbpt_3+i*(nbcaseX+1)+nbcaseX,nbpt_2+(i+1)*(nbcaseX+1)+nbcaseX,nbpt_2+i*(nbcaseX+1)+nbcaseX);
	    }

	  
	  a2ri_vf_add_face(primitive,nbpt_3+(nbcaseZ-2)*(nbcaseX+1),nbpt_2+(nbcaseZ-2)*(nbcaseX+1),nbpt_1);
	  a2ri_vf_add_face(primitive,nbpt_3+(nbcaseZ-2)*(nbcaseX+1),nbpt_1,nbpt_1+nbcaseX+1);

	  a2ri_vf_add_face(primitive,nbpt_3+nbcaseX+(nbcaseZ-2)*(nbcaseX+1),nbpt_1+nbcaseX,nbpt_2+nbcaseX+(nbcaseZ-2)*(nbcaseX+1));
	  a2ri_vf_add_face(primitive,nbpt_3+nbcaseX+(nbcaseZ-2)*(nbcaseX+1),nbpt_1+nbcaseX*2+1,nbpt_1+nbcaseX);
	}
      
      if(nbcaseY>1 && nbcaseZ==1)
	{
	  for(int i=0;i<nbcaseY;i++)
	    {
	      a2ri_vf_add_face(primitive,i*(nbcaseX+1),nbpt_1+i*(nbcaseX+1),(i+1)*(nbcaseX+1));
	      a2ri_vf_add_face(primitive,(i+1)*(nbcaseX+1),nbpt_1+i*(nbcaseX+1),nbpt_1+(i+1)*(nbcaseX+1));
	      
	      a2ri_vf_add_face(primitive,i*(nbcaseX+1)+nbcaseX,(i+1)*(nbcaseX+1)+nbcaseX,nbpt_1+i*(nbcaseX+1)+nbcaseX);
	      a2ri_vf_add_face(primitive,(i+1)*(nbcaseX+1)+nbcaseX,nbpt_1+(i+1)*(nbcaseX+1)+nbcaseX,nbpt_1+i*(nbcaseX+1)+nbcaseX);
	    }
	}

      if(nbcaseY==1 && nbcaseZ==1)
	{
	  a2ri_vf_add_face(primitive,nbcaseX+1,0,nbpt_1);
	  a2ri_vf_add_face(primitive,nbcaseX+1,nbpt_1,nbpt_1+nbcaseX+1);

	  a2ri_vf_add_face(primitive,2*nbcaseX+1,nbpt_1+nbcaseX,nbcaseX);
	  a2ri_vf_add_face(primitive,2*nbcaseX+1,nbpt_1+nbcaseX*2+1,nbpt_1+nbcaseX);
	}
    }
   
  
  if(nbcaseY>=2 && nbcaseZ>=2)
    {
      a2ri_vf_add_face(primitive,0,nbpt_2,nbcaseX+1);      
      a2ri_vf_add_face(primitive,nbcaseX+1,nbpt_2,nbpt_4);
 
      a2ri_vf_add_face(primitive,(nbcaseX+1)*nbcaseY,(nbcaseX+1)*(nbcaseY-1),nbpt_4+(nbcaseZ-1)*(nbcaseY-2));      
      a2ri_vf_add_face(primitive,(nbcaseX+1)*nbcaseY,nbpt_4+(nbcaseZ-1)*(nbcaseY-2),nbpt_3);
      
      a2ri_vf_add_face(primitive,nbcaseX,nbcaseX+1+nbcaseX,nbpt_2+nbcaseX);      
      a2ri_vf_add_face(primitive,nbcaseX+1+nbcaseX,nbpt_5,nbpt_2+nbcaseX);
      

      a2ri_vf_add_face(primitive,(nbcaseX+1)*nbcaseY+nbcaseX,nbpt_5+(nbcaseZ-1)*(nbcaseY-2),(nbcaseX+1)*(nbcaseY-1)+nbcaseX);      
      a2ri_vf_add_face(primitive,(nbcaseX+1)*nbcaseY+nbcaseX,nbpt_3+nbcaseX,nbpt_5+(nbcaseZ-1)*(nbcaseY-2));
  
      a2ri_vf_add_face(primitive,nbpt_4+nbcaseZ-2,nbpt_1,nbpt_1+nbcaseX+1);
      a2ri_vf_add_face(primitive,nbpt_2+(nbcaseX+1)*(nbcaseZ-2),nbpt_1,nbpt_4+nbcaseZ-2);
      
      a2ri_vf_add_face(primitive,nbpt_3+(nbcaseX+1)*(nbcaseZ-2),nbpt_4+(nbcaseZ-1)*(nbcaseY-1)-1,nbpt_1+(nbcaseX+1)*(nbcaseY-1));      
      a2ri_vf_add_face(primitive,nbpt_1+(nbcaseX+1)*(nbcaseY),nbpt_3+(nbcaseX+1)*(nbcaseZ-2),nbpt_1+(nbcaseX+1)*(nbcaseY-1));
      
      a2ri_vf_add_face(primitive,nbpt_5+nbcaseZ-2,nbpt_1+nbcaseX+1+nbcaseX,nbpt_1+nbcaseX);
      a2ri_vf_add_face(primitive,nbpt_2+(nbcaseX+1)*(nbcaseZ-2)+nbcaseX,nbpt_5+nbcaseZ-2,nbpt_1+nbcaseX);
      
      a2ri_vf_add_face(primitive,nbpt_3+(nbcaseX+1)*(nbcaseZ-2)+nbcaseX,nbpt_1+(nbcaseX+1)*(nbcaseY-1)+nbcaseX,nbpt_5+(nbcaseZ-1)*(nbcaseY-1)-1);  
      a2ri_vf_add_face(primitive,nbpt_1+(nbcaseX+1)*(nbcaseY)+nbcaseX,nbpt_1+(nbcaseX+1)*(nbcaseY-1)+nbcaseX,nbpt_3+(nbcaseX+1)*(nbcaseZ-2)+nbcaseX);
    }
}

void
gen_icosphere(char* argv[], vf_model *primitive)
{
  double rayon=atof(argv[2]);
  int subdivision=atoi(argv[3]);
  vector3d p;

  a2ri_vf_add_vertex(primitive,0.000000,-0.525731,0.850651);
  a2ri_vf_add_vertex(primitive,0.000000,0.525731,0.850651);
  a2ri_vf_add_vertex(primitive,0.000000,-0.525731,-0.850651);
  a2ri_vf_add_vertex(primitive,0.000000,0.525731,-0.850651);
  a2ri_vf_add_vertex(primitive,0.850651,0.000000,0.525731);
  a2ri_vf_add_vertex(primitive,0.850651,0.000000,-0.525731);
  a2ri_vf_add_vertex(primitive,-0.850651,0.000000,0.525731);
  a2ri_vf_add_vertex(primitive,-0.850651,0.000000,-0.525731);
  a2ri_vf_add_vertex(primitive,0.525731,0.850651,0.000000);
  a2ri_vf_add_vertex(primitive,0.525731,-0.850651,0.000000);
  a2ri_vf_add_vertex(primitive,-0.525731,0.850651,0.000000);
  a2ri_vf_add_vertex(primitive,-0.525731,-0.850651,0.000000);

  a2ri_vf_add_face(primitive,0,4,1);
  a2ri_vf_add_face(primitive,0,9,4);
  a2ri_vf_add_face(primitive,9,5,4);
  a2ri_vf_add_face(primitive,4,5,8);
  a2ri_vf_add_face(primitive,4,8,1);
  a2ri_vf_add_face(primitive,8,10,1);
  a2ri_vf_add_face(primitive,8,3,10);
  a2ri_vf_add_face(primitive,5,3,8);
  a2ri_vf_add_face(primitive,5,2,3);
  a2ri_vf_add_face(primitive,2,7,3);
  a2ri_vf_add_face(primitive,7,10,3);
  a2ri_vf_add_face(primitive,7,6,10);
  a2ri_vf_add_face(primitive,7,11,6);
  a2ri_vf_add_face(primitive,11,0,6);
  a2ri_vf_add_face(primitive,0,1,6);
  a2ri_vf_add_face(primitive,6,1,10);
  a2ri_vf_add_face(primitive,9,0,11);
  a2ri_vf_add_face(primitive,9,11,2);
  a2ri_vf_add_face(primitive,9,2,5);
  a2ri_vf_add_face(primitive,7,2,11);

  a2ri_vf_4_subdivision(primitive,subdivision-1);

  for(int i=0;i<primitive->nbvertex;i++)
    {
      vector3d_init(&p,primitive->ve[i].x,primitive->ve[i].y,primitive->ve[i].z);
      vector3d_normalize(&p);
      primitive->ve[i].x=p.dx*rayon;
      primitive->ve[i].y=p.dy*rayon;
      primitive->ve[i].z=p.dz*rayon;
    }
}

void
gen_uvsphere(char* argv[], vf_model *primitive)
{
  double rayon=atof(argv[2]);
  int segment=atoi(argv[3]),anneau=atoi(argv[4]);

  if(segment<3 || anneau<3)
    {
      printf("impossible de creer une sphere avec moins de 3 anneaux segments\n");
      exit(EXIT_FAILURE);
    }
  
  for(int j=1;j<anneau;j++)
    {
      double anglecourant=j*PI/(anneau*1.);
      double rayoncourant=sin(anglecourant)*rayon;
      double hauteurcourante=-cos(anglecourant)*rayon;
      
	for(int i=0;i<segment;i++)
	  a2ri_vf_add_vertex(primitive,rayoncourant*cos(i*(2*PI/(segment*1.))),hauteurcourante,rayoncourant*sin(i*(2*PI/(segment*1.))));
    }

  for(int j=0;j<anneau-2;j++)
    for(int i=0;i<segment;i++)
      {
	//printf("creation face : %d %d %d\n",j*segment+i,(j+1)*segment+i,j*segment+((i+1)%segment));
	a2ri_vf_add_face(primitive,j*segment+i,(j+1)*segment+i,j*segment+((i+1)%segment));
	//printf("creation face : %d %d %d\n",j*segment+((i+1)%segment),(j+1)*segment+i,(j+1)*segment+((i+1)%segment));
	a2ri_vf_add_face(primitive,j*segment+((i+1)%segment),(j+1)*segment+i,(j+1)*segment+((i+1)%segment));
      }

  a2ri_vf_add_vertex(primitive,0,-rayon,0);
  a2ri_vf_add_vertex(primitive,0,rayon,0);

  for(int i=0;i<segment;i++)
    {
      a2ri_vf_add_face(primitive,i,(i+1)%segment,segment*(anneau-1));
      a2ri_vf_add_face(primitive,(anneau-2)*segment+i,segment*(anneau-1)+1,(anneau-2)*segment+(i+1)%segment);
    }
  
}

void
gen_cylindre(char* argv[], vf_model *primitive)
{
  double rayon=atof(argv[2]),hauteur=atof(argv[3]);
  int segment=atoi(argv[4]),anneau=atoi(argv[5]);
  double delta=hauteur/(anneau*1.);
  
  if(segment<3)
    {
      printf("impossible de construire une cylindre avec moins de 3 segments\n");
      exit(EXIT_SUCCESS);
    }
  
  for(int j=0;j<=anneau;j++)
    for(int i=0;i<segment;i++)
      a2ri_vf_add_vertex(primitive,rayon*cos(i*(2*PI/(segment*1.))),j*delta,rayon*sin(i*(2*PI/(segment*1.))));

  for(int j=0;j<anneau;j++)
    for(int i=0;i<segment;i++)
      {
	//printf("creation face : %d %d %d\n",j*segment+i,(j+1)*segment+i,j*segment+((i+1)%segment));
	a2ri_vf_add_face(primitive,j*segment+i,(j+1)*segment+i,j*segment+((i+1)%segment));
	//printf("creation face : %d %d %d\n",j*segment+((i+1)%segment),(j+1)*segment+i,(j+1)*segment+((i+1)%segment));
	a2ri_vf_add_face(primitive,j*segment+((i+1)%segment),(j+1)*segment+i,(j+1)*segment+((i+1)%segment));
      }	

  a2ri_vf_add_vertex(primitive,0,0,0);
  a2ri_vf_add_vertex(primitive,0,hauteur,0);

  for(int i=0;i<segment;i++)
    {
      a2ri_vf_add_face(primitive,i,(i+1)%segment,segment*(anneau+1));
      a2ri_vf_add_face(primitive,anneau*segment+i,segment*(anneau+1)+1,anneau*segment+(i+1)%segment);
    }
}

void
gen_cone(char* argv[], vf_model *primitive)
{
  double rayon=atof(argv[2]),hauteur=atof(argv[3]);
  int segment=atoi(argv[4]),anneau=atoi(argv[5]);
  double delta=hauteur/(anneau*1.),delta_rayon=rayon/(anneau*1.);
  
  if(segment<3)
    {
      printf("impossible de construire une cone avec moins de 3 segments\n");
      exit(EXIT_SUCCESS);
    }
  
  for(int j=0;j<anneau;j++)
    for(int i=0;i<segment;i++)
      a2ri_vf_add_vertex(primitive,(rayon-(j*delta_rayon))*cos(i*(2*PI/(segment*1.))),j*delta,(rayon-(j*delta_rayon))*sin(i*(2*PI/(segment*1.))));

  a2ri_vf_add_vertex(primitive,0,hauteur,0);

  for(int j=0;j<anneau-1;j++)
    for(int i=0;i<segment;i++)
      {
	//printf("creation face : %d %d %d\n",j*segment+i,(j+1)*segment+i,j*segment+((i+1)%segment));
	a2ri_vf_add_face(primitive,j*segment+i,(j+1)*segment+i,j*segment+((i+1)%segment));
	//printf("creation face : %d %d %d\n",j*segment+((i+1)%segment),(j+1)*segment+i,(j+1)*segment+((i+1)%segment));
	a2ri_vf_add_face(primitive,j*segment+((i+1)%segment),(j+1)*segment+i,(j+1)*segment+((i+1)%segment));
      }

  for(int i=0;i<segment;i++)
    a2ri_vf_add_face(primitive,(anneau-1)*segment+i,segment*anneau,(anneau-1)*segment+(i+1)%segment);

  a2ri_vf_add_vertex(primitive,0,0,0);

  for(int i=0;i<segment;i++)
    a2ri_vf_add_face(primitive,i,(i+1)%segment,segment*anneau+1);
}

/*************************************************************/

int main(int argc, char* argv[])
{
  int arg_ok=0;
  vf_model primitive;

  a2ri_vf_init(&primitive);

  if(argc<4)
    usage(argv,"general");

  a2ri_vf_init(&primitive);

  if(!strcmp(argv[1],"grille"))
    {
      arg_ok=1;
      if(argc!=7)
	usage(argv,"grille");
      gen_grille(argv,&primitive);
    }
  
  if(!strcmp(argv[1],"boite"))
    {
      arg_ok=1;
      if(argc!=9)
	usage(argv,"boite");
      gen_boite(argv,&primitive);
    }
  
  if(!strcmp(argv[1],"icosphere"))
    {
      arg_ok=1;
      if(argc!=5)
	usage(argv,"icosphere");
      gen_icosphere(argv,&primitive);
    }

  if(!strcmp(argv[1],"uvsphere"))
    {
      arg_ok=1;
      if(argc!=6)
	usage(argv,"uvsphere");
      gen_uvsphere(argv,&primitive);
    }
  
  if(!strcmp(argv[1],"cylindre"))
    {
      arg_ok=1;
      if(argc!=7)
	usage(argv,"cylindre");
      gen_cylindre(argv,&primitive);
    }
  
  if(!strcmp(argv[1],"cone"))
    {
      arg_ok=1;
      if(argc!=7)
	usage(argv,"cone");
      gen_cone(argv,&primitive);
    }
  
  if(!arg_ok)
    usage(argv,"general");

  a2ri_vf_save_file(argv[argc-1],&primitive);

  exit(EXIT_SUCCESS);
}
