/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 01/03/07       */
/* Date de modification : 15/03/15   */
/* Version : 0.4                     */
/*************************************/

/***************************************************************************/
/* This file is part of a2ri.                                              */
/*                                                                         */
/* a2ri is free software: you can redistribute it and/or modify it         */
/* under the terms of the GNU Lesser General Public License as published   */
/* by the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                     */
/*                                                                         */
/* a2ri is distributed in the hope that it will be useful,                 */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           */
/* GNU Lesser General Public License for more details.                     */
/*                                                                         */
/* You should have received a copy of the GNU Lesser General Public        */
/* License along with a2ri.                                                */
/* If not, see <http://www.gnu.org/licenses/>.                             */
/***************************************************************************/

#include "topology.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

typedef struct
{
  int *list;
  int nbelt;
} ITlistecouple;


void
IFstar_byedge (
		     const vf_model * const m,
		     int ** list,
		     int * size,
		     int depth,
		     const hashtable * const table)
{
  int sizetemp = *size;
  vf_edge *edgetemp;

  if (depth == 0)
    return;

  for (int i = 0; i < sizetemp; i++)
    {
      int ve1 = m->fa[(*list)[i]].ve1;
      int ve2 = m->fa[(*list)[i]].ve2;
      int ve3 = m->fa[(*list)[i]].ve3;

      //1ere arete
      edgetemp = hashtable_look_for (table, ve1, ve2);

      if (edgetemp->nbsharedfaces == 2)
	{
	  if (edgetemp->sharedfaces[0] == (*list)[i])
	    list_int_add (list, size, edgetemp->sharedfaces[1],
			  WITHOUT_REDUNDANCE);
	  else
	    list_int_add (list, size, edgetemp->sharedfaces[0],
			  WITHOUT_REDUNDANCE);
	}

      //2eme arete
      edgetemp = hashtable_look_for (table, ve1, ve3);

      if (edgetemp->nbsharedfaces == 2)
	{
	  if (edgetemp->sharedfaces[0] == (*list)[i])
	    list_int_add (list, size, edgetemp->sharedfaces[1],
			  WITHOUT_REDUNDANCE);
	  else
	    list_int_add (list, size, edgetemp->sharedfaces[0],
			  WITHOUT_REDUNDANCE);
	}

      //3eme arete
      edgetemp = hashtable_look_for (table, ve2, ve3);

      if (edgetemp->nbsharedfaces == 2)
	{
	  if (edgetemp->sharedfaces[0] == (*list)[i])
	    list_int_add (list, size, edgetemp->sharedfaces[1],
			  WITHOUT_REDUNDANCE);
	  else
	    list_int_add (list, size, edgetemp->sharedfaces[0],
			  WITHOUT_REDUNDANCE);
	}
    }

  IFstar_byedge (m, list, size, depth - 1, table);
}

void
IFstar_byvertex (
		       const vf_model * const m,
		       int **list,
		       int *size,
		       int depth,
		       const hashtable * const table)
{
  int sizetemp = *size;
  vf_edge *edgetemp;
  int ve[3];

  if (depth == 0)
    return;

  for (int i = 0; i < sizetemp; i++)
    {
      ve[0] = m->fa[(*list)[i]].ve1;
      ve[1] = m->fa[(*list)[i]].ve2;
      ve[2] = m->fa[(*list)[i]].ve3;

      for (int j = 0; j < 3; j++)
	{
	  for (int k = 0; k < m->ve[ve[j]].nbincidentvertices; k++)
	    {
	      edgetemp =
		hashtable_look_for (table, ve[j],
				    m->ve[ve[j]].incidentvertices[k]);
	      for (int l = 0; l < edgetemp->nbsharedfaces; l++)
		if (edgetemp->sharedfaces[l] != (*list)[i])
		  list_int_add (list, size, edgetemp->sharedfaces[l],
				WITHOUT_REDUNDANCE);
	    }
	}
    }
  IFstar_byvertex (m, list, size, depth - 1, table);
}

//on cherche les aretes qui ne partagent qu'une seule face et on ajoute leur sommet dans une liste
//résultat : la liste des sommets appartenant au bord du maillage
void
IFlook_for_border (
		int key,
		vf_edge * value,
		void *user_data)
{
  ITlistecouple *list = user_data;

  if (value->nbsharedfaces == 1)
    {
      list_int_add (&(list->list), &(list->nbelt), value->ve1,
		    WITH_REDUNDANCE);
      list_int_add (&(list->list), &(list->nbelt), value->ve2,
		    WITH_REDUNDANCE);
    }
}

//on cherche à retrouver un cycle dans la liste des sommets appartenant au bord du maillage
void
IFcycle (
       ITlistecouple * list,
       ITlistecouple * cycle)
{
  int trouve = 1;
  int next = (*list).list[1];
  //on ajoute le premier et le second sommet
  list_int_add (&(cycle->list), &(cycle->nbelt), (*list).list[0],
		WITH_REDUNDANCE);
  list_int_add (&(cycle->list), &(cycle->nbelt), (*list).list[1],
		WITH_REDUNDANCE);
  while (next != (*list).list[0] && trouve)
    {
      trouve = 0;
      //dans le reste de la liste, on cherche le couple dont le premier element correspond au dernier element ajouté
      for (int i = 0; i < (*list).nbelt / 2; i++)
	{
	  if ((*list).list[i * 2] == cycle->list[(cycle->nbelt) - 1])
	    {
	      //on a correspondance entre le premier element du couple et le dernier element ajouté
	      if ((*list).list[i * 2 + 1] != (*list).list[0])
		{
		  //l'element trouvé ne correpond pas au premier element
		  //on ajoute donc le sommet suivant
		  list_int_add (&(cycle->list), &(cycle->nbelt),
				(*list).list[i * 2 + 1], WITH_REDUNDANCE);

		  next = (*list).list[i * 2 + 1];
		  list_int_remove (&((*list).list), &((*list).nbelt),
				   i * 2 + 1);
		  list_int_remove (&((*list).list), &((*list).nbelt), i * 2);
		}
	      else
		{
		  //on a correpondance entre l'element à ajouter et le premier élément donc nous avons trouvé un cycle
		  next = (*list).list[0];
		  list_int_add (&(cycle->list), &(cycle->nbelt),
				-1, WITH_REDUNDANCE);
		  //on termine l'algorithme en mettant next à la valeur du premier element
		  list_int_remove (&((*list).list), &((*list).nbelt),
				   i * 2 + 1);
		  list_int_remove (&((*list).list), &((*list).nbelt), i * 2);
		}
	      trouve = 1;
	    }
	}
    }
  list_int_remove (&((*list).list), &((*list).nbelt), 1);
  list_int_remove (&((*list).list), &((*list).nbelt), 0);
}


void
IFvf_complete_partie_connexe (
			      const vf_model * const m,
			      const hashtable * const table,
			      int *list,
			      int numface,
			      int numpartie,
			      int *nbaffect,
			      int *nbrecurs)
{
  int p1,
    p2;

  printf("nbrecurs : %d\n",*nbrecurs);
  *nbrecurs=*nbrecurs+1;
  

  if (list[numface] != -1)
    return;

  //la première face est affectée du numéro de partie
  list[numface] = numpartie;
  *nbaffect = (*nbaffect) + 1;

  //pour la premiere arete de la face, 
  p1 = m->fa[numface].ve1;
  p2 = m->fa[numface].ve2;
  //printf("blop3.5 - %d %d\n",p1,p2);
  vf_edge *edgetemp = hashtable_look_for (table, p1, p2);
  //printf("blop4\n");
  //on regarde si elle partage deux faces
  if (edgetemp->nbsharedfaces == 2)
    {
      //dans ce cas, on relance la fonction avec le meme modele, les memes aretes, la meme list de parties, le numero de la face partagée, le meme numéro de parties et le meme nombre de faces affecté
      if (edgetemp->sharedfaces[0] == numface)
	IFvf_complete_partie_connexe (m, table, list,
					 edgetemp->sharedfaces[1], numpartie,
				      nbaffect,nbrecurs);
      else
	IFvf_complete_partie_connexe (m, table, list,
					 edgetemp->sharedfaces[0], numpartie,
				      nbaffect,nbrecurs);
    }

  //printf("blop5\n");
  //on recommence l'opération précédente avec la seconde et la troisième arete de la face
  p1 = m->fa[numface].ve2;
  p2 = m->fa[numface].ve3;
  //printf("blop5.5 - %d %d\n",p1,p2);
  edgetemp = hashtable_look_for (table, p1, p2);
  //printf("blop6\n");
  if (edgetemp->nbsharedfaces == 2)
    {
      if (edgetemp->sharedfaces[0] == numface)
	IFvf_complete_partie_connexe (m, table, list,
					 edgetemp->sharedfaces[1], numpartie,
				      nbaffect,nbrecurs);
      else
	IFvf_complete_partie_connexe (m, table, list,
					 edgetemp->sharedfaces[0], numpartie,
				      nbaffect,nbrecurs);
    }

  //printf("blop7\n");
  p1 = m->fa[numface].ve3;
  p2 = m->fa[numface].ve1;
  //printf("blop7.5 - %d %d\n",p1,p2);
  edgetemp = hashtable_look_for (table, p1, p2);
  //printf("blop8\n");
  if (edgetemp->nbsharedfaces == 2)
    {
      if (edgetemp->sharedfaces[0] == numface)
	IFvf_complete_partie_connexe (m, table, list,
					 edgetemp->sharedfaces[1], numpartie,
				      nbaffect,nbrecurs);
      else
	IFvf_complete_partie_connexe (m, table, list,
					 edgetemp->sharedfaces[0], numpartie,
				      nbaffect,nbrecurs);
    }
}


void
IFvef_complete_partie_connexe (
				  const vef_model * const m,
				  int *list,
				  int numface,
				  int numpartie,
				  int *nbaffect)
{
  int ed1 = m->fa[numface].ed1,
    ed2 = m->fa[numface].ed2,
    ed3 = m->fa[numface].ed3;
  char *temp = NULL;
  if (list[numface] != -1)
    return;

  temp = (char *) malloc (20 * sizeof (char));
  a2ri_erreur_critique_si (temp == NULL,
			"erreur allocation memoire pour temp\nIFvef_complete_partie_connexe");

  //la première face est affecté du numéro de partie
  list[numface] = numpartie;
  *nbaffect = (*nbaffect) + 1;

  //pour la premiere arete de la face, 
  //on regarde si elle partage deux faces
  if (m->ed[ed1].nbsharedfaces == 2)
    {
      //dans ce cas, on relance la fonction avec le meme modele, les memes aretes, la meme list de parties, le numero de la face partagée, le meme numéro de parties et le meme nombre de faces affecté
      if (m->ed[ed1].sharedfaces[0] == numface)
	IFvef_complete_partie_connexe (m, list, m->ed[ed1].sharedfaces[1],
					  numpartie, nbaffect);
      else
	IFvef_complete_partie_connexe (m, list, m->ed[ed1].sharedfaces[0],
					  numpartie, nbaffect);
    }

  //on recommence l'opération précédente avec la seconde et la troisième arete de la face
  if (m->ed[ed2].nbsharedfaces == 2)
    {
      if (m->ed[ed2].sharedfaces[0] == numface)
	IFvef_complete_partie_connexe (m, list, m->ed[ed2].sharedfaces[1],
					  numpartie, nbaffect);
      else
	IFvef_complete_partie_connexe (m, list, m->ed[ed2].sharedfaces[0],
					  numpartie, nbaffect);
    }


  if (m->ed[ed3].nbsharedfaces == 2)
    {
      if (m->ed[ed3].sharedfaces[0] == numface)
	IFvef_complete_partie_connexe (m, list, m->ed[ed3].sharedfaces[1],
					  numpartie, nbaffect);
      else
	IFvef_complete_partie_connexe (m, list, m->ed[ed3].sharedfaces[0],
					  numpartie, nbaffect);
    }

  free (temp);

}

/********** MAIN FUNCTIONS **********/

/**
   Recherche du voisinage pour une liste de faces et à une profondeur donnée
   @param type BYVERTEX ou BYEDGE <BR> BYVERTEX : recherche du voisinage par sommet <BR> BYEDGE : recherche du voisinage par arete.
   @param m le modèle
   @param faces tableau contenant les numéros des faces de d\303\251part
   @param nbfaces taille du tableau
   @param list pointeur sur le tableau des faces contenu dans le voisinage
   @param size pointeur sur le nombre de faces dans le voisinage
   @param depth profondeur du voisinage
   @return aucun
*/
void
a2ri_vf_star (
	      int type,
	      const vf_model * const m,
	      const int * const faces,
	      int nbfaces,
	      int **list,
	      int *size,
	      int depth)
{
  int *listclone = NULL;
  hashtable *table = a2ri_vf_construction_edge_table (m, NULL, 0);

  list_int_clone (faces, nbfaces, &listclone);
  *size = nbfaces;
  if (type == BYEDGE)
    {
      IFstar_byedge (m, &listclone, size, depth, table);
      *list = listclone;
    }
  else
    {
      IFstar_byvertex (m, &listclone, size, depth, table);
      *list = listclone;
    }
  hashtable_free (table);
  free (table);
}

/**
   Calcul du nombre de trous/nombre de limites dans le modèle
   @param m le modèle
   @return nombre de trous/limites
*/
int
a2ri_vf_nb_hole (
		 const vf_model * const m)
{
  int nbtrou = 0;
  ITlistecouple lescouples,
    lescycles;
  lescouples.list = NULL;
  lescouples.nbelt = 0;
  lescycles.list = NULL;
  lescycles.nbelt = 0;
  hashtable *table = a2ri_vf_construction_edge_table (m, NULL, 0);

  hashtable_foreach (table, IFlook_for_border, &lescouples);
  //si on ne trouve aucun sommet sur le bord du maillage
  if (lescouples.nbelt == 0)
    {
      hashtable_free (table);
      free (table);
      free (lescouples.list);
      free (lescycles.list);
      //il n'y a pas de trou
      return nbtrou;
    }

#ifdef _DEBUG
  printf("--> lescouples\n");
  list_int_display(lescouples.list,lescouples.nbelt);
#endif

  //sinon tant qu'on a des elements dans la liste, on cherche un nouveau cycle
  while (lescouples.nbelt != 0)
    {
      IFcycle (&lescouples, &lescycles);
      //le nombre de cycle correspond au nombre de trou
      nbtrou++;
    }

#ifdef _DEBUG
      printf("--> lescycles\n");
      list_int_display(lescycles.list,lescycles.nbelt);
#endif

  hashtable_free (table);
  free (table);
  free (lescouples.list);
  free (lescycles.list);

  return nbtrou;
}

/**
   Calcul du nombre de trous/nombre de limites dans le modèle
   @param m le modèle
   @return nombre de trous/limites
*/
int
a2ri_vef_nb_hole (
		  const vef_model * const m)
{
  int nbtrou = 0;
  ITlistecouple lescouples,
    lescycles;
  lescouples.list = NULL;
  lescouples.nbelt = 0;
  lescycles.list = NULL;
  lescycles.nbelt = 0;

  for (int i = 0; i < m->nbedge; i++)
    if (m->ed[i].nbsharedfaces == 1)
      {
	list_int_add (&(lescouples.list), &(lescouples.nbelt), m->ed[i].ve1,
		      WITH_REDUNDANCE);
	list_int_add (&(lescouples.list), &(lescouples.nbelt), m->ed[i].ve2,
		      WITH_REDUNDANCE);
      }

  //si on ne trouve aucun sommet sur le bord du maillage
  if (lescouples.nbelt == 0)
    //il n'y a pas de trou
    return nbtrou;

  //sinon tant qu'on a des elements dans la liste, on cherche un nouveau cycle
  while (lescouples.nbelt != 0)
    {
      IFcycle (&lescouples, &lescycles);
      //le nombre de cycle correspond au nombre de trou
      nbtrou++;
    }

  free (lescouples.list);
  free (lescycles.list);
  return nbtrou;
}

/**
   Calcul du nombre de parties connexes contenu dans le modèle
   @param m le modèle
   @param list tableau d'entier représentant les faces du modèle et contenant le numéro de la partie à laquelle la face appartient.
   @return nombre de parties connexes
*/
int
a2ri_vf_nb_connected_part (
			   const vf_model * const m,
			   int **list)
{
  int nbpart = 0,
    nbaffect = 0,
    nbrecurs=0,
    j;
  int *parties = NULL;

  parties = (int *) malloc (m->nbface * sizeof (int));
  a2ri_erreur_critique_si (parties == NULL,
			"erreur allocation memoire pour parties\na2ri_vf_nb_connected_part");

  //parties sera le tableau qui donnera le numero de la parties à laquelle apparient la face du meme index
  // les parties sont initialisées à -1

  for (int i = 0; i < m->nbface; i++)
    parties[i] = -1;

  hashtable *table = a2ri_vf_construction_edge_table (m, NULL, 0);

  //tant que toutes les faces n'ont pas trouvées de numéro de parties, on continu
  while (nbaffect != m->nbface)
    {
      nbrecurs=0;
      j = 0;
      //on cherche la première face n'ayant pas de numéro de parties donc ne faisant parties d'aucun groupe
      while (parties[j] != -1)
	j++;
      //on appelle la fonction avec le modèle, les aretes, le tableau des parties, le numéro de la première face, le numéro de la nouvelle partie et enfin le nombre de face deja affecté d'un numéro de parties
      IFvf_complete_partie_connexe (m, table, parties, j, nbpart,
				    &nbaffect,&nbrecurs);
      //nous avons trouvé une nouvelle partie
      nbpart++;
    }

  *list = parties;

  hashtable_free (table);
  free (table);


  return nbpart;
}

/**
   Calcul du nombre de parties connexes contenu dans le modèle
   @param m le modèle
   @param list tableau d'entier représentant les faces du modèle et contenant le numéro de la partie à laquelle la face appartient.
   @return nombre de parties connexes
*/
int
a2ri_vef_nb_connected_part (
			    const vef_model * const m,
			    int **list)
{
  int nbpart = 0,
    nbaffect = 0,
    j;
  int *parties = NULL;

  parties = (int *) malloc (m->nbface * sizeof (int));
  a2ri_erreur_critique_si (parties == NULL,
			"erreur allocation memoire pour parties\na2ri_vef_nb_connected_part");


  //parties sera le tableau qui donnera le numero de la parties à laquelle apparient la face du meme index
  // les parties sont initialisées à -1

  for (int i = 0; i < m->nbface; i++)
    parties[i] = -1;

  //tant que toutes les faces n'ont pas trouvées de numéro de parties, on continu
  while (nbaffect != m->nbface)
    {
      j = 0;
      //on cherche la première face n'ayant pas de numéro de parties donc ne faisant parties d'aucun groupe
      while (parties[j] != -1)
	j++;
      //on appelle la fonction avec le modèle, les aretes, le tableau des parties, le numéro de la première face, le numéro de la nouvelle partie et enfin le nombre de face deja affecté d'un numéro de parties
      IFvef_complete_partie_connexe (m, parties, j, nbpart, &nbaffect);
      //nous avons trouvé une nouvelle partie
      nbpart++;
    }

  *list = parties;

  return nbpart;
}


/**
   Cherche un trou/cycle dans le maillage contenant les deux sommets
   @param m le modele
   @param ve1 premier sommet qui doit etre contenu dans le cycle
   @param ve2 second sommet qui doit etre contenu dans le cycle
   @param list liste des sommets formant le cycle
   @param size taille de la liste
**/
void
a2ri_vf_search_hole_contains (
			      const vf_model * const m,
			      int ve1,
			      int ve2,
			      int ** list,
			      int * size)
{
  ITlistecouple lescouples,
    lescycles;
  lescouples.list = NULL;
  lescouples.nbelt = 0;
  lescycles.list = NULL;
  lescycles.nbelt = 0;
  hashtable *table = a2ri_vf_construction_edge_table (m, NULL, 0);

  hashtable_foreach (table, IFlook_for_border, &lescouples);
  //si on ne trouve aucun sommet sur le bord du maillage
  if (lescouples.nbelt == 0)
    {
      hashtable_free (table);
      free (table);
      free (lescouples.list);
      free (lescycles.list);
      //il n'y a pas de trou
      *list = NULL;
      *size = 0;
      return;
    }

  //sinon tant qu'on a des elements dans la liste, on cherche un nouveau cycle
  while (lescouples.nbelt != 0)
    {
      IFcycle (&lescouples, &lescycles);
      //le nombre de cycle correspond au nombre de trou
      if (list_int_contains (lescycles.list, lescycles.nbelt, ve1) != -1 &&
	  list_int_contains (lescycles.list, lescycles.nbelt, ve2) != -1)
	{
	  *list = lescycles.list;
	  *size = lescycles.nbelt;
	  free (lescouples.list);
	  hashtable_free (table);
	  free (table);
	  return;
	}
      free (lescycles.list);
      lescycles.list = NULL;
      lescycles.nbelt = 0;
    }

  hashtable_free (table);
  free (table);
  free (lescouples.list);
  free (lescycles.list);
  *list = NULL;
  *size = 0;

  return;
}
