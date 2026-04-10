#include <stdio.h>
#include <stdlib.h>

#include <a2ri/io.h>
#include <a2ri/graph.h>
#include "opengl.h"



int main(int argc, char *argv[])
{
  vef_model m;

  if(argc!=2)
    {
      printf("usage : %s <modele>\n",argv[0]);
      return 1;
    }


  a2ri_vef_init(&m);

  if(!a2ri_vef_open_file(argv[1],&m))
  return 0;


  printf("touche a : calculer et afficher le Nearest Neighbour Graph\n");
  printf("touche z : calculer et afficher le graph couvrant minimal\n");
  printf("touche e : calculer et afficher le Gabriel graph\n");
  printf("touche r : calculer et afficher le Gabriel graph etendu\n");
  printf("touche s : afficher le modele de depart\n");
  printf("touche q/Q/esc : quitter\n");
  go(argc,argv,&m);

  
    
  return 0;
}


