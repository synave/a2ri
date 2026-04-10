#include <stdio.h>
#include <stdlib.h>
#include "opengl.h"

int
main(int argc, char* argv[]) 
{

  if(argc!=2 && argc!=4)
    {
      printf("usage: %s <fichier modele>\nou\nusage :: %s <fichier modele> <sommet1> <sommet2>\n",argv[0],argv[0]);
      return EXIT_FAILURE;
    }
    
  vf_model m;

  a2ri_vf_init(&m);
  a2ri_vf_open_file(argv[1],&m);

  printf("Sélection des points de départ et de fin à la souris\n");
  printf("clic milieu + ENTREE : sélectionner les sommets\n");
  printf("touche TAB : basculer d'un sommet à l'autre si plusieurs sont sélectionnés\n");
  printf("clic gauche : faire tourner la scène\n");
  printf("clic droit : faire translater la scène\n");
  printf("touche t : densifier le maillage\n");
  printf("touche y : recherche de chemin avec la methode approche avant\n");
  printf("touche u : recherche de chemin avec la methode approche arriere\n");
  printf("touche o : recherche de chemin avec la methode Dijkstra\n");
  printf("touche i : recherche de chemin avec la methode approche et le plan moyen\n");
  printf("touche k : recherche de chemin avec la methode approche et le plan minmax\n");
  printf("touche p : recherche de chemin avec la methode Dijkstra et le plan moyen\n");
  printf("touche m : recherche de chemin avec la methode Dijkstra et le plan minmax\n");
  printf("touche c : afficher la longueur curvilinéaire entre les deux sommets situés sur un meme bord\n");
  printf("touche r : reset\n");
  printf("touche q/Q/esc : quitter\n");
  
  if(argc==2)
    go(argc,argv,&m,-1,-1);
  if(argc==4)
    go(argc,argv,&m,atoi(argv[2]),atoi(argv[3]));
  
  return EXIT_SUCCESS;
}

