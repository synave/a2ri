#include <stdio.h>
#include <stdlib.h>

#include <a2ri/io.h>
#include <a2ri/graph.h>
#include "opengl.h"



int main(int argc, char *argv[])
{
  vf_model m;

  if(argc!=3)
    {
      printf("usage : %s <modele> <epaisseur>\n",argv[0]);
      return 1;
    }


  a2ri_vf_init(&m);

  if(!a2ri_vf_open_file(argv[1],&m))
  return 0;


  printf("touche e : epaissir le maillage\n");
  printf("touche o : afficher le modele de depart\n");
  printf("touche d : afficher le modele\n");
  printf("touche f : afficher le modele en detail\n");
  printf("touche s : sauver le maillage courant\n");
  printf("touche q/Q/esc : quitter\n");
  go(argc,argv,&m,atof(argv[2]));

  
    
  return 0;
}


