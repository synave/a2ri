#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <time.h>

#include <a2ri/io.h>
#include "opengl.h"
#include <a2ri/overlap.h>
#include <a2ri/icp.h>

int main(int argc, char *argv[])
{
  vf_model m,base;

  if(argc!=5 && argc!=11)
    {
      printf("usage : ./icp -v <modele de base> <modele a recaler> <critere d'arret Dkn>\n");
      printf("ou\t./icp <modele de base> <modele a recaler> <critere d'arret Dkn> <rotation suivant X> <rotation suivant Y> >rotation suivant Z> <translation suivant X> <translation suivant Y> <translation suivant Z> <AABB|ritter>\n");
      return 1;
    }    

  a2ri_vf_init(&base);
  a2ri_vf_init(&m);
  if(!a2ri_vf_open_file(argv[2],&base))
    {
      printf("modele introuvable\n");
      return 0;
    }
  if(!a2ri_vf_open_file(argv[3],&m))
    {
      printf("modele introuvable\n");
      return 0;
    }
  printf("clic gauche : faire tourner la scene\n");
  printf("clic droit : faire translater la scene\n");
  printf("molette : se rapprocher/s eloigner de la scene\n");
  printf("touche w/x/c : faire tourner l'objet a recaler\n");
  printf("touche v/b/n : faire translater l'objet a recaler\n");
  printf("touche i : lancer l'algorithme ICP\n");
  printf("touche r : entrer manuellement un taux de recouvrement (100 %d par defaut)\n",37);
  printf("touche t : lance trimmed ICP - ne pas oublier d'entrer le taux de recouvrement avant\n");
  printf("touche e : regler la sensibilite de l'algorithme Automated Trimmed ICP (0.3 par defaut)\n");
  printf("touche a : lance Automated Trimmed ICP\n");
  printf("touche s : sauver le maillage recale\n");
  printf("touche q/Q/esc : quitter\n");
  go(argc,argv,&base,&m,atof(argv[4]));  
  
  return 0;
}


