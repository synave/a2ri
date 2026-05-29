#include <stdio.h>
#include <time.h>

#include "io.h"
#include "curvature.h"
//#include "opengl.h"

void
usage(char *argv[])
{
  printf("usage : %s <modele> <num_sommet>\n",argv[0]);
}


int main(int argc, char *argv[])
{
  vf_model m;  

  if(argc!=3)
    {
      usage(argv);
      return 0;
    }



  a2ri_vf_init(&m);

  if(!a2ri_vf_open_file(argv[1],&m))
    {
      printf("erreur a l ouverture\n");
      return 1;
    }

  int num_sommet = atoi(argv[2]);

  printf("somme des angles : %lf\n",a2ri_vf_courbure_gaussienne_discrete(&m, num_sommet));


  /*printf("clic gauche : faire tourner l'objet\n");
  printf("molette : avancer/reculer\n");
  printf("clic droit : translater l'objet\n");
  printf("touche s : fixer manuellement un seuil\n");
  printf("touche +/- : augmenter/diminuer le seuil de filtrage\n");
  printf("touche q : quitter\n");
  go(argc,argv,&m);*/

  return 0;
}
