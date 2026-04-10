#include <stdio.h>
#include <stdlib.h>
#include <a2ri/io.h>
#include "opengl.h"

void
usage(char* argv[])
{
  printf("usage: %s <modele a ouvrir> <fichier a enregistrer> <taille de la boule>\n",argv[0]);
}

int
main(int argc, char* argv[]) 
{
  vf_model m;

  if(argc!=4)
    {
      usage(argv);
      return EXIT_FAILURE;
    }
  
  a2ri_vf_init(&m);
  
  if(!(a2ri_vf_open_file(argv[1],&m)))
    {
      printf("fichier %s introuvable\n",argv[1]);
      return EXIT_FAILURE;
    }

  a2ri_vf_center(&m);
  
  a2ri_vf_display(&m);
  
  printf("clic gauche : faire tourner la scène\n");
  printf("clic droit : faire translater la scène\n");
  printf("touche entree : screenshot\n");
  printf("touche TAB : Faire un pas de la triangulation\n");
  printf("touche p : calcul automatique de la taille optimale de la boule\n");
  printf("touche t : trianguler totalement le nuage de points et enregistrer\n");
  printf("touche f : afficher les fronts\n");
  printf("touche s : enregistrer le maillage\n");
  printf("touche q/Q/esc : quitter\n");
  setlocale(LC_NUMERIC,"C");
  go(argc,argv,&m,atof(argv[3]),argv[2]);
  
  return EXIT_SUCCESS;
  }
