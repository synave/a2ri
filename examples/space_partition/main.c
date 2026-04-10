#include <stdio.h>
#include <stdlib.h>
#include <a2ri/io.h>
#include <a2ri/space_partition.h>
#include <a2ri/point.h>
#include <pthread.h>

//Nombre moyen de points par cellule
//Variable non fixé car dépend fortement du nombre de points et de leur dispersion
//Laisser cette variable entre 1 et 50
//Valeur forte (20-50) lorsque les sommets sont répartis régulièrement dans l'espace
//Valeur faible (1-20) sinon (le cas le plus probable)
#define NB_MOY_PTS 10


typedef struct{
  space_partition *sp;
  vf_vertex *ve;
  double *longueur;
  int nb;
}a2ri_arg_thread;


void *thread_process (void *argu)
{
  a2ri_arg_thread *arg=(a2ri_arg_thread*)argu;
  point3d p;
  double longueur;
  
  for(int i=0;i<(int)(arg->nb);i++)
    {
      point3d ptemp;
      point3d_init(&ptemp,arg->ve[i].x,arg->ve[i].y,arg->ve[i].z);
      space_partition_nearest_point(arg->sp,&ptemp,&p,&longueur,ACCEPT_ZERO_LENGTH);
      arg->longueur[i]=longueur;
    }

  pthread_exit (0);
}


int
main(int argc, char* argv[]) 
{

   if(argc!=3)
    {
      printf("usage: %s <modele m1> <modele m2>\n",argv[0]);
      return EXIT_FAILURE;
    }
   
      
   space_partition sp1,sp2;
   point3d ptmin,ptmax;
   a2ri_time depgen,fingen,depinter,fininter;
   double *listelongueur1=NULL,*listelongueur2=NULL;
   int nbpartX,nbpartY,nbpartZ;

   vf_model m1,m2;
   
   pthread_t th[A2RI_NB_THREAD];
   a2ri_arg_thread argument[A2RI_NB_THREAD];
   void *ret[A2RI_NB_THREAD];

   printf("ouverture des deux modeles\n");

   a2ri_vf_init(&m1);
   a2ri_vf_init(&m2);

   a2ri_vf_open_file(argv[1],&m1);
   a2ri_vf_open_file(argv[2],&m2);

   printf("M1 :\n");
   a2ri_vf_display(&m1);
   printf("M2 :\n");
   a2ri_vf_display(&m2);
   
   
   depgen=a2ri_get_time();

   printf("\n**************************\n\n");

   printf("creation des deux space_partition \n");
   depinter=a2ri_get_time();

   ptmin.x=m1.xmin;ptmin.y=m1.ymin;ptmin.z=m1.zmin;
   ptmax.x=m1.xmax;ptmax.y=m1.ymax;ptmax.z=m1.zmax;

   int nbpart=(int)(pow((m1.nbvertex/(NB_MOY_PTS*1.0)),1.0/3.0));
   double dx=m1.xmax-m1.xmin,dy=m1.ymax-m1.ymin,dz=m1.zmax-m1.zmin;
   
   //pour obtenir des cellules cubiques (ou presque)
   if(dx<=dy && dx<=dz)
     {
       //dx mini
       nbpartX=nbpart;
       nbpartY=(int)(nbpartX*(dy/dx));
       nbpartZ=(int)(nbpartX*(dz/dx));
     }
   else
     {
       if(dy<=dx && dy<=dz)
	 {
	   //dy mini
	   nbpartY=nbpart;
	   nbpartX=(int)(nbpartY*(dx/dy));
	   nbpartZ=(int)(nbpartY*(dz/dy));
	 }
       else
	 {
	   //dz mini
	   nbpartZ=nbpart;
	   nbpartX=(int)(nbpartZ*(dx/dz));
	   nbpartY=(int)(nbpartZ*(dy/dz));
	 }
     }

   space_partition_new(&sp1,&ptmin,&ptmax,nbpartX,nbpartY,nbpartZ);

   ptmin.x=m2.xmin;ptmin.y=m2.ymin;ptmin.z=m2.zmin;
   ptmax.x=m2.xmax;ptmax.y=m2.ymax;ptmax.z=m2.zmax;

   nbpart=(int)(pow((m1.nbvertex/(NB_MOY_PTS*1.0)),1.0/3.0));
   dx=m2.xmax-m2.xmin;dy=m2.ymax-m2.ymin;dz=m2.zmax-m2.zmin;

   if(dx<=dy && dx<=dz)
     {
       //dx mini
       nbpartX=nbpart;
       nbpartY=(int)(nbpartX*(dy/dx));
       nbpartZ=(int)(nbpartX*(dz/dx));
     }
   else
     {
       if(dy<=dx && dy<=dz)
	 {
	   //dy mini
	   nbpartY=nbpart;
	   nbpartX=(int)(nbpartY*(dx/dy));
	   nbpartZ=(int)(nbpartY*(dz/dy));
	 }
       else
	 {
	   //dz mini
	   nbpartZ=nbpart;
	   nbpartX=(int)(nbpartZ*(dx/dz));
	   nbpartY=(int)(nbpartZ*(dy/dz));
	 }
     }

   space_partition_new(&sp2,&ptmin,&ptmax,nbpartX,nbpartY,nbpartZ);

  
   a2ri_vf_space_partition(&m1,&sp1);
   a2ri_vf_space_partition(&m2,&sp2);


   fininter=a2ri_get_time();
   a2ri_display_interval_time("temps de creation : ",depinter,fininter);
   
   printf("\n**************************\n\n");
   
   printf("recherche du point le plus proche\n");

   printf("M1->M2\n");
   depinter=a2ri_get_time();
   
   listelongueur1=(double*)malloc(m1.nbvertex*sizeof(double));

   for(int i=0;i<A2RI_NB_THREAD-1;i++)
     {
       argument[i].sp=&sp2;
       argument[i].nb=m1.nbvertex/A2RI_NB_THREAD;
       argument[i].longueur=listelongueur1+i*argument[i].nb;
       argument[i].ve=m1.ve+i*argument[i].nb;
     }
   argument[A2RI_NB_THREAD-1].sp=&sp2;
   argument[A2RI_NB_THREAD-1].nb=m1.nbvertex-(A2RI_NB_THREAD-1)*(argument[0].nb);
   argument[A2RI_NB_THREAD-1].longueur=listelongueur1+(A2RI_NB_THREAD-1)*(argument[0].nb);
   argument[A2RI_NB_THREAD-1].ve=m1.ve+(A2RI_NB_THREAD-1)*(argument[0].nb);
   
   for(int i=0;i<A2RI_NB_THREAD;i++)
     {
       if (pthread_create (th+i, NULL, thread_process, argument+i) < 0) {
	 fprintf (stderr, "pthread_create error for thread 2\n");
	 exit (1);
       }
     }
   
   for(int i=0;i<A2RI_NB_THREAD;i++)
   (void)pthread_join (th[i], &ret[i]);

   fininter=a2ri_get_time();
   a2ri_display_interval_time("temps de calcul M1->M2 : ",depinter,fininter);

   printf("M2->M1\n\n");
   depinter=a2ri_get_time();

   listelongueur2=(double*)malloc(m2.nbvertex*sizeof(double));

   for(int i=0;i<A2RI_NB_THREAD-1;i++)
     {
       argument[i].sp=&sp1;
       argument[i].nb=m2.nbvertex/A2RI_NB_THREAD;
       argument[i].longueur=listelongueur2+i*argument[i].nb;
       argument[i].ve=m2.ve+i*argument[i].nb;
     }
   argument[A2RI_NB_THREAD-1].sp=&sp1;
   argument[A2RI_NB_THREAD-1].nb=m2.nbvertex-(A2RI_NB_THREAD-1)*(argument[0].nb);
   argument[A2RI_NB_THREAD-1].longueur=listelongueur2+(A2RI_NB_THREAD-1)*(argument[0].nb);
   argument[A2RI_NB_THREAD-1].ve=m2.ve+(A2RI_NB_THREAD-1)*(argument[0].nb);
   
   for(int i=0;i<A2RI_NB_THREAD;i++)
     {
       if (pthread_create (th+i, NULL, thread_process, argument+i) < 0) {
	 fprintf (stderr, "pthread_create error for thread 2\n");
	 exit (1);
       }
     }
   
   for(int i=0;i<A2RI_NB_THREAD;i++)
   (void)pthread_join (th[i], &ret[i]);

   fininter=a2ri_get_time();
   a2ri_display_interval_time("temps de calcul M2->M1 : ",depinter,fininter);

   fingen=a2ri_get_time();
   a2ri_display_interval_time("temps total : ",depgen,fingen);
   
   printf("\n**************************\n\n");
   
   printf("RESUTLATS :\n");
   printf("max M1->M2 : %lf\n",list_double_max(listelongueur1,m1.nbvertex));
   printf("max M2->M1 : %lf\n",list_double_max(listelongueur2,m2.nbvertex));
   
   space_partition_free(&sp1);
   space_partition_free(&sp2);

   free(listelongueur1);
   free(listelongueur2);

   a2ri_vf_free(&m1);
   a2ri_vf_free(&m2);

  
   return EXIT_SUCCESS;
}
