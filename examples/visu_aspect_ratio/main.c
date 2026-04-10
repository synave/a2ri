#include <stdio.h>
#include <time.h>

#include <a2ri/io.h>
#include "opengl.h"

void
usage(char *argv[])
{
  printf("usage : %s <modele>\n",argv[0]);
}


int main(int argc, char *argv[])
{
  vf_model m;  

  if(argc!=2)
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


  printf("clic gauche : faire tourner l'objet\n");
  printf("molette : avancer/reculer\n");
  printf("clic droit : translater l'objet\n");
  printf("touche s : fixer manuellement un seuil\n");
  printf("touche +/- : augmenter/diminuer le seuil de filtrage\n");
  printf("touche q : quitter\n");
  go(argc,argv,&m);

  return 0;
}
