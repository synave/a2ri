#include <stdio.h>
#include <stdlib.h>
#include <a2ri/conversion.h>

int
main(int argc, char* argv[]) 
{
  
  if(argc!=3 && argc!=4)
    {
      printf("usage: ./conversion <fichier modele> <fichier sortie>\nou\nusage : argv[0] -tovrml1 <fichier modele> <fichiersortie>\n");
      return EXIT_FAILURE;
    }

  if(argc==3)
    {
      a2ri_vf_conversion(argv[1],argv[2]);
      return EXIT_SUCCESS;
    }

  vf_model m;

  //initialisation d'un nouveau modèle
  a2ri_vf_init(&m);

  //ouverture du modèle input
  a2ri_vf_open_file(argv[2],&m);

  //enregistrement du modèle output
  a2ri_vf_save_ascii_stl(argv[3],&m);

  a2ri_vf_free(&m);
  
  return EXIT_SUCCESS;
}
