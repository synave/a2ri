#include <stdio.h>
#include <stdlib.h>
#include <a2ri/io.h>
#include <a2ri/quality.h>

int
main(int argc, char* argv[]) 
{
  
  if(argc!=3)
    {
      printf("usage: ./precision_squelette <fichier modele> <fichier squelette>\n");
      return EXIT_FAILURE;
    }

  vf_model m;
  skeleton s;

  //initialisation d'un nouveau modèle
  a2ri_vf_init(&m);
  a2ri_skeleton_init(&s);

  //ouverture du modèle input
  a2ri_vf_open_file(argv[1],&m);
  a2ri_skeleton_open_file(argv[2],&s);

  printf("precision du squelette : %d\n",a2ri_vf_model_skeleton_accuracy(&m,&s));
  
  return EXIT_SUCCESS;
}
