#include <stdio.h>
#include <stdlib.h>
#include <a2ri/io.h>
#include <a2ri/topology.h>

int
main(int argc, char* argv[]) 
{

  vf_model m;
  
  if(argc!=2)
    {
      printf("usage: %s <fichier modele>\n",argv[0]);
      return EXIT_FAILURE;
    }



  a2ri_vf_init(&m);
  a2ri_vf_open_file(argv[1],&m);

  int *list=NULL,size=0;

  //printf("nombre de trou : %d\n",a2ri_vf_nb_hole(&m));
  printf("nombre de partie connexes : %d\n",a2ri_vf_nb_connected_part(&m,&list));
  free(list);
  list=NULL;

  for(int i=0;i<m.nbface;i++)
    {
      list_int_add(&list,&size,m.fa[i].ve1,WITHOUT_REDUNDANCE);
      list_int_add(&list,&size,m.fa[i].ve2,WITHOUT_REDUNDANCE);
      list_int_add(&list,&size,m.fa[i].ve3,WITHOUT_REDUNDANCE);
    }
  list_int_sort(list,size,ASC);

  printf("nombre de sommets isoles : %d\n",(list[size-1]-size)+1);

  free(list);

  a2ri_vf_free(&m);

  
  return EXIT_SUCCESS;
}
