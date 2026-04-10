/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 01/03/07       */
/* Date de modification : 15/03/15   */
/* Version : 0.4                     */
/*************************************/

/*************************************/
/* Auteur : Romain Leguay            */
/*          Nguyen Haiduong          */
/*          Marianne Fichoux         */
/* Date de modification : 26/05/09   */
/* Version : 0.2                     */
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

#include "model.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

/********** MAIN FUNCTIONS **********/

/**
   Initialisation d'un modèle avec un maillage vide
   @param m pointeur sur le modèle
   @return aucun
*/
void
a2ri_vf_init (
	      vf_model * m)
{
  m->ve = NULL;
  m->fa = NULL;
  m->nbvertex = 0;
  m->nbface = 0;
}

/**
   Libération de l'espace mémoire utilisé
   @param m pointeur sur le modèle
*/
void
a2ri_vf_free (
	      vf_model * m)
{
  for (int i = 0; i < m->nbvertex; i++)
    free (m->ve[i].incidentvertices);
  free (m->ve);
  free (m->fa);

}

/**
   Clonage du vf_model
   @param m pointeur sur le modèle
   @return un vf_model identique
*/
vf_model *
a2ri_vf_clone (
	       const vf_model * const m)
{
  vf_model *retour = (vf_model *) malloc (sizeof (vf_model));
  a2ri_vf_init (retour);

  retour->xmin = m->xmin;
  retour->xmax = m->xmax;
  retour->ymin = m->ymin;
  retour->ymax = m->ymax;
  retour->zmin = m->zmin;
  retour->zmax = m->zmax;

  for (int i = 0; i < m->nbvertex; i++)
    {
      a2ri_vf_add_vertex (retour, m->ve[i].x, m->ve[i].y, m->ve[i].z);
      retour->ve[i].incidentvertices = NULL;
      list_int_clone (m->ve[i].incidentvertices, m->ve[i].nbincidentvertices,
		      &(retour->ve[i].incidentvertices));
      retour->ve[i].nbincidentvertices = m->ve[i].nbincidentvertices;
    }

  for (int i = 0; i < m->nbface; i++)
    {
      a2ri_vf_add_face (retour, m->fa[i].ve1, m->fa[i].ve2, m->fa[i].ve3);
    }

  return retour;
}

/**
 //affichage des caractéristiques générales d'un modèle (nombre de sommets et de faces)
 @param m le modèle
 @return aucun
*/
void
a2ri_vf_display (
		 const vf_model * const m)
{
  printf ("********************\nModel :");
  printf ("\n\t%d vertices\n", m->nbvertex);
  printf ("\t%d faces\n", m->nbface);
  printf
    ("Bounding Box : [%4.2f , %4.2f , %4.2f] - [%4.2f , %4.2f , %4.2f]\n",
     m->xmin, m->ymin, m->zmin, m->xmax, m->ymax, m->zmax);
  printf ("********************\n");
}

/**
 //affichage des caractéristiques détaillées d'un modèle (nombre de sommets, de faces et leurs adjacences)
 @param m le modèle
 @return aucun
*/
void
a2ri_vf_display_detail (
			const vf_model * const m)
{
  int i;

  printf ("********************\nModel :");

  if (m->nbvertex)
    {
      printf ("\n\t%d vertices\n", m->nbvertex);
      for (i = 0; i < m->nbvertex; i++)
	{
	  printf ("\t\t%5d - ", i);
	  vf_vertex_display_detail (&(m->ve[i]));
	  printf ("\n");
	}
    }

  if (m->nbface)
    {
      printf ("\n\t%d faces\n", m->nbface);
      for (i = 0; i < m->nbface; i++)
	{
	  printf ("\t\t%5d - (%d , %d , %d) ",
		  i, m->fa[i].ve1, m->fa[i].ve2, m->fa[i].ve3);
	  printf ("\n\n");
	}
    }

  printf
    ("\nBounding Box : [%4.2f , %4.2f , %4.2f] - [%4.2f , %4.2f , %4.2f]\n",
     m->xmin, m->ymin, m->zmin, m->xmax, m->ymax, m->zmax);
  printf ("********************\n");
}


/**
   mis à jour du numéro d'arete en fonction de la taille de la table de hachage
   @param e arete à mettre à jour
   @param m le modele - parametre inutilisé
   @param table la table de hachage
   @return aucun
*/
/**
@TODO
*/
/*void
a2ri_vf_update_num_edge (
			 vf_edge * e,
			 const vf_model * const m,
			 const hashtable * const table)
{
  vf_model *temp;
  temp = m;
  e->att_int = hashtable_size (table);
  }*/

/**
   mis à jour de la longueur de l'arete
   @param e arete à mettre à jour
   @param m le modele
   @param table la table de hachage - parametre inutilisé
   @return aucun
*/
void
a2ri_vf_update_length_edge (
			    vf_edge * e,
			    const vf_model * const m,
			    hashtable * table)
{
  point3d p1,
    p2;
  point3d_init (&p1, m->ve[e->ve1].x, m->ve[e->ve1].y, m->ve[e->ve1].z);
  point3d_init (&p2, m->ve[e->ve2].x, m->ve[e->ve2].y, m->ve[e->ve2].z);
  e->att_double = point3d_length (&p1, &p2);
}

/**
   Construction de la table de hachage contenant toutes les aretes d'un vf_model
   @param m le modele
   @param func tableau de fonctions devant etre appliqué
   @param nbfunc taille du tableau de fonctions
   @return la table de hachage
*/
hashtable *
a2ri_vf_construction_edge_table (
				 const vf_model * const m,
				 ptf_func_hashtable * func,
				 int nbfunc)
{
  int p1,
    p2;

  hashtable *table = hashtable_new (m->nbvertex + m->nbface);
  a2ri_erreur_critique_si(table==NULL,
			  "erreur allocation memoire pour table dans a2ri_vf_construction_edge_table\n");

  for (int i = 0; i < m->nbface; i++)
    {
      p1 = m->fa[i].ve1;
      p2 = m->fa[i].ve2;
      vf_edge *edgetemp = hashtable_look_for (table, p1, p2);
      if (edgetemp == NULL)
	{
	  edgetemp = (vf_edge *) malloc (sizeof (vf_edge));
	  a2ri_erreur_critique_si (edgetemp == NULL,
				"erreur allocation memoire pour edgetemp\na2ri_vf_construction_edge_table");
	  edgetemp->ve1 = p1;
	  edgetemp->ve2 = p2;
	  edgetemp->att_int = -1;
	  edgetemp->att_double = -1;
	  for (int j = 0; j < nbfunc; j++)
	    func[j] (edgetemp, m, table);
	  edgetemp->nbsharedfaces = 0;
	  edgetemp->sharedfaces = NULL;
	  list_int_add (&(edgetemp->sharedfaces), &(edgetemp->nbsharedfaces),
			i, WITH_REDUNDANCE);
	  hashtable_add (table, edgetemp);
	}
      else
	list_int_add (&(edgetemp->sharedfaces), &(edgetemp->nbsharedfaces), i,
		      WITH_REDUNDANCE);

      p1 = m->fa[i].ve2;
      p2 = m->fa[i].ve3;
      edgetemp = hashtable_look_for (table, p1, p2);
      if (edgetemp == NULL)
	{
	  edgetemp = (vf_edge *) malloc (sizeof (vf_edge));
	  a2ri_erreur_critique_si (edgetemp == NULL,
				"erreur allocation memoire pour edgetemp\na2ri_vf_construction_edge_table");
	  edgetemp->ve1 = p1;
	  edgetemp->ve2 = p2;
	  edgetemp->att_int = -1;
	  edgetemp->att_double = -1;
	  for (int j = 0; j < nbfunc; j++)
	    func[j] (edgetemp, m, table);
	  edgetemp->nbsharedfaces = 0;
	  edgetemp->sharedfaces = NULL;
	  list_int_add (&(edgetemp->sharedfaces), &(edgetemp->nbsharedfaces),
			i, WITH_REDUNDANCE);
	  hashtable_add (table, edgetemp);
	}
      else
	list_int_add (&(edgetemp->sharedfaces), &(edgetemp->nbsharedfaces), i,
		      WITH_REDUNDANCE);


      p1 = m->fa[i].ve3;
      p2 = m->fa[i].ve1;
      edgetemp = hashtable_look_for (table, p1, p2);
      if (edgetemp == NULL)
	{
	  edgetemp = (vf_edge *) malloc (sizeof (vf_edge));
	  a2ri_erreur_critique_si (edgetemp == NULL,
				"erreur allocation memoire pour edgetemp\na2ri_vf_construction_edge_table");
	  edgetemp->ve1 = p1;
	  edgetemp->ve2 = p2;
	  edgetemp->att_int = -1;
	  edgetemp->att_double = -1;
	  for (int j = 0; j < nbfunc; j++)
	    func[j] (edgetemp, m, table);
	  edgetemp->nbsharedfaces = 0;
	  edgetemp->sharedfaces = NULL;
	  list_int_add (&(edgetemp->sharedfaces), &(edgetemp->nbsharedfaces),
			i, WITH_REDUNDANCE);
	  hashtable_add (table, edgetemp);
	}
      else
	list_int_add (&(edgetemp->sharedfaces), &(edgetemp->nbsharedfaces), i,
		      WITH_REDUNDANCE);
    }

  return table;
}

/**
   Ajout d'un vertex (x,y,z) au modèle
   @param m pointeur sur le modèle
   @param x coordonnée x du vertex
   @param y coordonnée y
   @param z coordonnée z
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_add_vertex (
		    vf_model * m,
		    double x,
		    double y,
		    double z)
{
  if (m->ve == NULL)
    {
      m->ve = (vf_vertex *) malloc (sizeof (vf_vertex));
      a2ri_erreur_critique_si (m->ve == NULL,
			    "erreur allocation memoire pour m->ve\na2ri_vf_add_vertex");
    }
  else
    {
      vf_vertex *tempve;
      tempve =
	(vf_vertex *) realloc (m->ve, (m->nbvertex + 1) * sizeof (vf_vertex));
      a2ri_erreur_critique_si (tempve == NULL,
			    "erreur allocation memoire pour m->ve\na2ri_vf_add_vertex");
      m->ve=tempve;
    }

  m->ve[m->nbvertex].x = x;
  m->ve[m->nbvertex].y = y;
  m->ve[m->nbvertex].z = z;
  m->ve[m->nbvertex].incidentvertices = NULL;
  m->ve[m->nbvertex].nbincidentvertices = 0;
  (m->nbvertex)++;

  return 1;
}

/**
   Recherche d'un vertex dans un modèle
   @param m le modèle
   @param x coordonnée x du vertex à rechercher
   @param y coordonnée y
   @param z coordonnée z
   @return numéro du vertex dans la liste de vertex du modèle, -1 si le vertex n'est pas trouvé.
*/
int
a2ri_vf_search_vertex (
		       const vf_model * const m,
		       double x,
		       double y,
		       double z)
{
  int i;
  for (i = 0; i < m->nbvertex; i++)
    if (m->ve[i].x == x && m->ve[i].y == y && m->ve[i].z == z)
      return i;

  return -1;
}

/**
   Retrait d'un sommet du modèle. L'opération ne peut être effectué que si le sommet n'est référencée dans aucune face.
   @param m le modèle
   @param numvertex numèro du vertex à retirer
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_remove_vertex (
		       vf_model * m,
		       int numvertex)
{
  int j,
    *list1 = NULL,
    size1 = 0;
  vf_vertex *tempve;

  if (m->fa == NULL)
    {
      //si numvertex est plus grand que le nombre de sommet
      if (numvertex > m->nbvertex)
	return 0;

      //si le sommet est encore présent (utilisé) dans une face, on ne peut pas le supprimer
      for (int i = 0; i < m->nbface; i++)
	if (m->fa[i].ve1 == numvertex || m->fa[i].ve2 == numvertex
	    || m->fa[i].ve3 == numvertex)
	  return 0;

      //décalage de la liste de sommet
      for (int i = numvertex; i < (m->nbvertex) - 1; i++)
	{
	  m->ve[i].x = m->ve[i + 1].x;
	  m->ve[i].y = m->ve[i + 1].y;
	  m->ve[i].z = m->ve[i + 1].z;
	  m->ve[i].incidentvertices = m->ve[i + 1].incidentvertices;
	  m->ve[i].nbincidentvertices = m->ve[i + 1].nbincidentvertices;
	}

      tempve =
	(vf_vertex *) realloc (m->ve,
			       ((m->nbvertex) - 1) * sizeof (vf_vertex));
      a2ri_erreur_critique_si (tempve == NULL,
			    "erreur allocation memoire pour m->ve\na2ri_vf_remove_vertex");
      m->ve=tempve;
      m->nbvertex = m->nbvertex - 1;

      //suppression du vertex dans les sommets incidents et décalages pour les valeurs supérieurs à numvertex
      for (int i = 0; i < m->nbvertex; i++)
	{
	  j = 0;
	  while (j < m->ve[i].nbincidentvertices)
	    {
	      if (m->ve[i].incidentvertices[j] == numvertex)
		list_int_remove (&(m->ve[i].incidentvertices),
				 &(m->ve[i].nbincidentvertices), j);
	      else
		{
		  if (m->ve[i].incidentvertices[j] > numvertex)
		    m->ve[i].incidentvertices[j]--;
		  j++;
		}
	    }
	}
    }
  else
    {
      if (m->ve[numvertex].nbincidentvertices == 0)
	{
	  //décalage de la liste de sommet
	  for (int i = numvertex; i < (m->nbvertex) - 1; i++)
	    {
	      m->ve[i].x = m->ve[i + 1].x;
	      m->ve[i].y = m->ve[i + 1].y;
	      m->ve[i].z = m->ve[i + 1].z;
	      m->ve[i].incidentvertices = m->ve[i + 1].incidentvertices;
	      m->ve[i].nbincidentvertices = m->ve[i + 1].nbincidentvertices;
	    }

	  tempve =
	    (vf_vertex *) realloc (m->ve,
				   ((m->nbvertex) - 1) * sizeof (vf_vertex));
	  a2ri_erreur_critique_si (tempve == NULL,
				"erreur allocation memoire pour m->ve\na2ri_vf_remove_vertex");
	  m->ve=tempve;
	  m->nbvertex = m->nbvertex - 1;
	  for (int i = 0; i < m->nbface; i++)
	    {
	      if (m->fa[i].ve1 >= numvertex)
		m->fa[i].ve1 = m->fa[i].ve1 - 1;
	      if (m->fa[i].ve2 >= numvertex)
		m->fa[i].ve2 = m->fa[i].ve2 - 1;
	      if (m->fa[i].ve3 >= numvertex)
		m->fa[i].ve3 = m->fa[i].ve3 - 1;
	    }
	}
      else
	{
	  for (int i = 0; i < m->nbface; i++)
	    if (m->fa[i].ve1 == numvertex || m->fa[i].ve2 == numvertex
		|| m->fa[i].ve3 == numvertex)
	      list_int_add (&list1, &size1, i, WITH_REDUNDANCE);
	  list_int_sort (list1, size1, DESC);
	  for (int i = 0; i < size1; i++)
	    a2ri_vf_remove_face (m, list1[i]);
	  free (list1);
	  
	}
    }

  return 1;
}

/**
   Retrait d'une liste de sommets du modèle. L'opération ne peut être effectué que si le sommet n'est référencée dans aucune face.
   @param m le modèle
   @param numvertex numèro du vertex à retirer
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_remove_list_of_vertex (
			       vf_model * m,
			       int *listvertex,
			       int sizelist)
{
  int *list1 = NULL,
    size1 = 0;
  if (m->fa == NULL)
    {
      list_int_sort (listvertex, sizelist, DESC);
      for (int i = 0; i < sizelist; i++)
	a2ri_vf_remove_vertex (m, listvertex[i]);
    }
  else
    {
      for (int i = 0; i < sizelist; i++)
	for (int j = 0; j < m->nbface; j++)
	  if (m->fa[j].ve1 == listvertex[i]
	      || m->fa[j].ve2 == listvertex[i]
	      || m->fa[j].ve3 == listvertex[i])
	    list_int_add (&list1, &size1, j, WITHOUT_REDUNDANCE);
      
      list_int_sort (list1, size1, DESC);
      for (int i = 0; i < size1; i++)
	a2ri_vf_remove_face (m, list1[i]);
      free (list1);
      
    }
  return 1;
}

/**
   Ajout d'une face défini par trois points (numéros d'index dans la liste de points) au modèle
   @param m pointeur sur le modèle
   @param ve1 index du premier point
   @param ve2 index du second point
   @param ve3 index du troisième point
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_add_face (
		  vf_model * m,
		  int ve1,
		  int ve2,
		  int ve3)
{
  if (m->fa == NULL)
    {
      m->fa = (vf_face *) malloc (sizeof (vf_face));
      a2ri_erreur_critique_si (m->fa == NULL,
			    "erreur allocation memoire pour m->fa\na2ri_vf_add_face");
    }
  else
    {
      vf_face *tempfa;
      tempfa = (vf_face *) realloc (m->fa, (m->nbface + 1) * sizeof (vf_face));
      a2ri_erreur_critique_si (tempfa == NULL,
			    "erreur allocation memoire pour m->fa\na2ri_vf_add_face");
      m->fa=tempfa;
    }

  m->fa[m->nbface].ve1 = ve1;
  m->fa[m->nbface].ve2 = ve2;
  m->fa[m->nbface].ve3 = ve3;
  list_int_add (&(m->ve[ve1].incidentvertices),
		&(m->ve[ve1].nbincidentvertices), ve2, WITHOUT_REDUNDANCE);
  list_int_add (&(m->ve[ve2].incidentvertices),
		&(m->ve[ve2].nbincidentvertices), ve1, WITHOUT_REDUNDANCE);
  list_int_add (&(m->ve[ve1].incidentvertices),
		&(m->ve[ve1].nbincidentvertices), ve3, WITHOUT_REDUNDANCE);
  list_int_add (&(m->ve[ve3].incidentvertices),
		&(m->ve[ve3].nbincidentvertices), ve1, WITHOUT_REDUNDANCE);
  list_int_add (&(m->ve[ve3].incidentvertices),
		&(m->ve[ve3].nbincidentvertices), ve2, WITHOUT_REDUNDANCE);
  list_int_add (&(m->ve[ve2].incidentvertices),
		&(m->ve[ve2].nbincidentvertices), ve3, WITHOUT_REDUNDANCE);

  (m->nbface)++;

  return 1;
}

/**
   Recherche d'une face dans un modèle
   @param m le modèle
   @param ve1 index d'un point
   @param ve2 index d'un point
   @param ve3 index d'un point
   @return numéro de la face dans la liste de face du modèle, -1 si la face n'est pas trouvée.
   @warning Cette fonction est sensible à l'orientation de la face.
   Exemple pour un modèle m:
   @code
   liste de faces (index -> ve1 ve2 ve3):
   0 -> 0 1 2
   1 -> 0 2 3 
   2 -> 0 3 1
   3 -> 1 3 2
   @endcode
   le code
   @code
   a2ri_vf_search_face(m,0,3,1);
   ou
   a2ri_vf_search_face(m,3,1,0);
   ou
   a2ri_vf_search_face(m,1,0,3);
   @endcode
   retournera 2 alors que
   @code
   a2ri_vf_search_face(m,3,0,1);
   @endcode
   retournera -1
*/
int
a2ri_vf_search_face (
		     const vf_model * const m,
		     int ve1,
		     int ve2,
		     int ve3)
{
  for (int i = 0; i < m->nbface; i++)
    {
      if (m->fa[i].ve1 == ve1)
	if ((m->fa[i].ve2 == ve2 && m->fa[i].ve3 == ve3))
	  return i;
      if (m->fa[i].ve1 == ve2)
	if ((m->fa[i].ve2 == ve3 && m->fa[i].ve3 == ve1))
	  return i;
      if (m->fa[i].ve1 == ve3)
	if ((m->fa[i].ve2 == ve1 && m->fa[i].ve3 == ve2))
	  return i;
    }

  return -1;
}

/**
   Retrait d'une face du modèle. La fonction enlève également les sommets qui ne sont plus utilisés.
   @param m le modèle
   @param numface numèro de la face à retirer
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_remove_face (
		     vf_model * m,
		     int numface)
{
  int ve[3],
    k;
  vf_face *tempfa;
  vf_vertex *tempve;

  //le numero de la face est supérieur au nombre de faces du modèle
  if (numface > m->nbface)
    return 0;

  //on récupére les numéros des sommets pour ensuite essayer de les supprimer
  ve[0] = m->fa[numface].ve1;
  ve[1] = m->fa[numface].ve2;
  ve[2] = m->fa[numface].ve3;

  list_int_sort (ve, 3, DESC);

  //décalage des faces
  for (int i = numface; i < (m->nbface) - 1; i++)
    {
      m->fa[i].ve1 = m->fa[i + 1].ve1;
      m->fa[i].ve2 = m->fa[i + 1].ve2;
      m->fa[i].ve3 = m->fa[i + 1].ve3;
    }

  tempfa =
    (vf_face *) realloc (m->fa, ((m->nbface) - 1) * sizeof (vf_face));
  a2ri_erreur_critique_si (tempfa == NULL,
			   "erreur allocation memoire pour m->fa\na2ri_vf_remove_face");
  m->fa=tempfa;
  m->nbface = m->nbface - 1;

  //on essaye de supprimer les trois sommets
  /****************************/
  for (int i = 0; i < 3; i++)
    {
      int numvertex = ve[i];
      //si le sommet est encore présent (utilisé) dans une face, on ne peut pas le supprime
      k = 0;
      for (int j = 0; j < m->nbface; j++)
	if (m->fa[j].ve1 == numvertex || m->fa[j].ve2 == numvertex
	    || m->fa[j].ve3 == numvertex)
	  k++;

      if (!k)
	{
	  free (m->ve[numvertex].incidentvertices);
	  //décalage de la liste de sommet
	  for (int j = numvertex; j < (m->nbvertex) - 1; j++)
	    {
	      m->ve[j].x = m->ve[j + 1].x;
	      m->ve[j].y = m->ve[j + 1].y;
	      m->ve[j].z = m->ve[j + 1].z;
	      m->ve[j].incidentvertices = m->ve[j + 1].incidentvertices;
	      m->ve[j].nbincidentvertices =
		m->ve[j + 1].nbincidentvertices;
	    }

	  tempve =
	    (vf_vertex *) realloc (m->ve,
				   ((m->nbvertex) -
				    1) * sizeof (vf_vertex));
	  a2ri_erreur_critique_si (tempve == NULL,
				   "erreur allocation memoire pour m->ve\na2ri_vf_remove_face");
	  m->ve=tempve;
	  m->nbvertex = m->nbvertex - 1;

	  //suppression du vertex dans les sommets incidents et décalages pour les valeurs supérieurs à numvertex
	  for (int j = 0; j < m->nbvertex; j++)
	    {
	      k = 0;
	      while (k < m->ve[j].nbincidentvertices)
		{
		  if (m->ve[j].incidentvertices[k] == numvertex)
		    list_int_remove (&(m->ve[j].incidentvertices),
				     &(m->ve[j].nbincidentvertices), k);
		  else
		    {
		      if (m->ve[j].incidentvertices[k] > numvertex)
			m->ve[j].incidentvertices[k]--;
		      k++;
		    }
		}
	    }

	  //mis à jour des numéro de sommet dans les faces
	  for (int j = 0; j < m->nbface; j++)
	    {
	      if (m->fa[j].ve1 > numvertex)
		m->fa[j].ve1 = m->fa[j].ve1 - 1;
	      if (m->fa[j].ve2 > numvertex)
		m->fa[j].ve2 = m->fa[j].ve2 - 1;
	      if (m->fa[j].ve3 > numvertex)
		m->fa[j].ve3 = m->fa[j].ve3 - 1;
	    }
	}
    }
  /****************************/
  
  

  return 1;

}

/**
   Retrait d'une liste de faces du modèle. La fonction enlève également les sommets qui ne sont plus utilisés.
   @param m le modèle
   @param numface numèro de la face à retirer
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_remove_list_of_face (
			     vf_model * m,
			     int *listface,
			     int sizelist)
{

  list_int_sort (listface, sizelist, DESC);

  for (int i = 0; i < sizelist; i++)
    a2ri_vf_remove_face (m, listface[i]);
  
  return 1;
}

/**
   Translation du modèle
   @param m pointeur sur le modèle à translater
   @param delta pointeur sur le vecteur de translation
   @return aucun
*/
void
a2ri_vf_translate (
		   vf_model * m,
		   const vector3d * const delta)
{
  //translation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    vf_vertex_translate (&((*m).ve[i]), delta);

  //translation de la bounding box
  m->xmin += delta->dx;
  m->xmax += delta->dx;
  m->ymin += delta->dy;
  m->ymax += delta->dy;
  m->zmin += delta->dz;
  m->zmax += delta->dz;

}

/**
   Translation d'un modele afin qu'il se trouve centré sur l'origine (0,0,0)
   @param m le modele a centrer
   @return aucun
**/
void
a2ri_vf_center (
		vf_model * m)
{
  vector3d delta;

  vector3d_init (&delta, -((m->xmin + m->xmax)) / 2.0,
		 -((m->ymin + m->ymax) / 2.0), -((m->zmin + m->zmax) / 2.0));

  a2ri_vf_translate (m, &delta);
}


/**
   Rotation d'un modèle en radian autour de l'axe X
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @return aucun
*/
void
a2ri_vf_rotateX_radian (
			vf_model * m,
			double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotateX_radian (&(m->ve[i]), angle);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en degré autour de l'axe X
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degré
   @return aucun
*/
void
a2ri_vf_rotateX_degre (
		       vf_model * m,
		       double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotateX_degre (&(m->ve[i]), angle);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en radian suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void
a2ri_vf_rotateX_radian_center (
			       vf_model * m,
			       double angle,
			       const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotateX_center_radian (&(m->ve[i]), angle, centre);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en degre suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void
a2ri_vf_rotateX_degre_center (
			      vf_model * m,
			      double angle,
			      const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotateX_center_degre (&(m->ve[i]), angle, centre);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en radian autour de l'axe Y
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @return aucun
*/
void
a2ri_vf_rotateY_radian (
			vf_model * m,
			double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotateY_radian (&(m->ve[i]), angle);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}

    }
}

/**
   Rotation d'un modèle en degré autour de l'axe Y
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degré
   @return aucun
*/
void
a2ri_vf_rotateY_degre (
		       vf_model * m,
		       double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotateY_degre (&(m->ve[i]), angle);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en radian suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void
a2ri_vf_rotateY_radian_center (
			       vf_model * m,
			       double angle,
			       const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotateY_center_radian (&(m->ve[i]), angle, centre);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en degre suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void
a2ri_vf_rotateY_degre_center (
			      vf_model * m,
			      double angle,
			      const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotateY_center_degre (&(m->ve[i]), angle, centre);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en radian autour de l'axe Z
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @return aucun
*/
void
a2ri_vf_rotateZ_radian (
			vf_model * m,
			double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotateZ_radian (&(m->ve[i]), angle);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en degré autour de l'axe Z
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degré
   @return aucun
*/
void
a2ri_vf_rotateZ_degre (
		       vf_model * m,
		       double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotateZ_degre (&(m->ve[i]), angle);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en radian suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void
a2ri_vf_rotateZ_radian_center (
			       vf_model * m,
			       double angle,
			       const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotateZ_center_radian (&(m->ve[i]), angle, centre);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en degre suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void
a2ri_vf_rotateZ_degre_center (
			      vf_model * m,
			      double angle,
			      const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotateZ_center_degre (&(m->ve[i]), angle, centre);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en degré suivant l'axe donnée
   @param m pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param axe l'axe de rotation
   @return aucun
*/
void
a2ri_vf_rotate_axe_radian (
			   vf_model * m,
			   double angle,
			   const vector3d * const axe)
{
  gsl_matrix *identity3,
    *A,
    *B,
    *M;
  double x = axe->dx;
  double y = axe->dy;
  double z = axe->dz;

  identity3 = gsl_matrix_alloc (3, 3);
  gsl_matrix_set_identity (identity3);

  A = gsl_matrix_alloc (3, 3);
  gsl_matrix_set (A, 0, 0, x * x);
  gsl_matrix_set (A, 0, 1, x * y);
  gsl_matrix_set (A, 0, 2, x * z);
  gsl_matrix_set (A, 1, 0, x * y);
  gsl_matrix_set (A, 1, 1, y * y);
  gsl_matrix_set (A, 1, 2, y * z);
  gsl_matrix_set (A, 2, 0, x * z);
  gsl_matrix_set (A, 2, 1, y * z);
  gsl_matrix_set (A, 2, 2, z * z);


  B = gsl_matrix_alloc (3, 3);
  gsl_matrix_set (B, 0, 0, 0);
  gsl_matrix_set (B, 0, 1, -z);
  gsl_matrix_set (B, 0, 2, y);
  gsl_matrix_set (B, 1, 0, z);
  gsl_matrix_set (B, 1, 1, 0);
  gsl_matrix_set (B, 1, 2, -x);
  gsl_matrix_set (B, 2, 0, -y);
  gsl_matrix_set (B, 2, 1, x);
  gsl_matrix_set (B, 2, 2, 0);


  M =
    matrix_add (matrix_add
		(matrix_mul_scale (identity3, cos (angle)),
		 matrix_mul_scale (A, 1 - cos (angle))), matrix_mul_scale (B,
									   sin
									   (angle)));


  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vf_vertex_rotate_axe_radian (&(m->ve[i]), M);

      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
  gsl_matrix_free (A);
  gsl_matrix_free (B);
  gsl_matrix_free (M);
  gsl_matrix_free (identity3);
}


/**
   Calcul de l'aire totale d'un modèle
   @param m le modèle
   @return aire totale du modèle
*/
double
a2ri_vf_area (
	      const vf_model * const m)
{
  int ve1,
    ve2,
    ve3;
  double sumarea = 0.0;
  point3d A,
    B,
    C;

  //somme des aires de toutes les faces
  for (int i = 0; i < m->nbface; i++)
    {
      ve1 = m->fa[i].ve1;
      ve2 = m->fa[i].ve2;
      ve3 = m->fa[i].ve3;

      point3d_init (&A, m->ve[ve1].x, m->ve[ve1].y, m->ve[ve1].z);
      point3d_init (&B, m->ve[ve2].x, m->ve[ve2].y, m->ve[ve2].z);
      point3d_init (&C, m->ve[ve3].x, m->ve[ve3].y, m->ve[ve3].z);
      sumarea += point3d_area (&A, &B, &C);
    }

  return sumarea;
}

/**
   Trouve les faces entourant un sommet
   @param m le modèle
   @param numve numéro du sommet
   @param list liste des numéros de faces
   @param size nombre de faces
   @return aucun
**/
void
a2ri_vf_faces_next_vertex (
			   const vf_model * const m,
			   int numve,
			   int **list,
			   int *size)
{
  vf_edge *edgetemp;

  hashtable *table = a2ri_vf_construction_edge_table (m, NULL, 0);

  *list = NULL;
  *size = 0;

  for (int i = 0; i < m->ve[numve].nbincidentvertices; i++)
    {
      edgetemp =
	hashtable_look_for (table, numve, m->ve[numve].incidentvertices[i]);
      for (int j = 0; j < edgetemp->nbsharedfaces; j++)
	list_int_add (list, size, edgetemp->sharedfaces[j],
		      WITHOUT_REDUNDANCE);
    }
  hashtable_free (table);
  free (table);
}


/**
   Trouve les faces entourant un sommet avec une hashtable contenant les aretes fournie
   @param m le modèle
   @param numve numéro du sommet
   @param list liste des numéros de faces
   @param size nombre de faces
   @param table la hashtable
   @return aucun
**/
void
a2ri_vf_faces_next_vertex_with_hashtable (
					  const vf_model * const m,
					  int numve,
					  int **list,
					  int *size,
					  const hashtable * const table)
{

  vf_edge *edgetemp;

  *list = NULL;
  *size = 0;

  for (int i = 0; i < m->ve[numve].nbincidentvertices; i++)
    {
      edgetemp =
	hashtable_look_for (table, numve, m->ve[numve].incidentvertices[i]);
      for (int j = 0; j < edgetemp->nbsharedfaces; j++)
	list_int_add (list, size, edgetemp->sharedfaces[j],
		      WITHOUT_REDUNDANCE);
    }
}

/**
   calcul d'une normale à la face
   @param m le modele
   @param numfa numéro de la face
   @return vecteur normal à la face
*/
vector3d
a2ri_vf_normal_face (
		     const vf_model * const m,
		     int numfa)
{
  vector3d AB,
    AC;

  vector3d_init (&AB, m->ve[m->fa[numfa].ve2].x - m->ve[m->fa[numfa].ve1].x,
		 m->ve[m->fa[numfa].ve2].y - m->ve[m->fa[numfa].ve1].y,
		 m->ve[m->fa[numfa].ve2].z - m->ve[m->fa[numfa].ve1].z);
  vector3d_init (&AC, m->ve[m->fa[numfa].ve3].x - m->ve[m->fa[numfa].ve1].x,
		 m->ve[m->fa[numfa].ve3].y - m->ve[m->fa[numfa].ve1].y,
		 m->ve[m->fa[numfa].ve3].z - m->ve[m->fa[numfa].ve1].z);

  return vector3d_vectorialproduct (&AB, &AC);
}


/**
   calcul d'une normale au sommet comme étant la moyenne des normales des faces adjacentes au sommet
   @param m le modele
   @param numve numéro du sommet
   @return vecteur normal au sommet
*/
vector3d
a2ri_vf_normal_vertex (
		       const vf_model * const m,
		       int numve)
{
  int *listface = NULL,
    sizelistface = 0;
  vector3d normale_sommet;

  a2ri_vf_faces_next_vertex (m, numve, &listface, &sizelistface);
  vector3d_init (&normale_sommet, 0.0, 0.0, 0.0);
  //calcul de la normale au sommet
  for (int i = 0; i < sizelistface; i++)
    {
      vector3d vtemp=a2ri_vf_normal_face (m, listface[i]);
      normale_sommet = vector3d_add (&normale_sommet, &vtemp);
    }
  vector3d_init (&normale_sommet, normale_sommet.dx / sizelistface,
		 normale_sommet.dy / sizelistface,
		 normale_sommet.dz / sizelistface);
  vector3d_normalize (&normale_sommet);

  return normale_sommet;
}

/**
   calcul d'une normale au sommet comme étant la moyenne des normales des faces adjacentes au sommet
   @param m le modele
   @param numve numéro du sommet
   @param table table de hachage contenant les aretes
   @return vecteur normal au sommet
*/
vector3d
a2ri_vf_normal_vertex_with_hashtable (
				      const vf_model * const m,
				      int numve,
				      const hashtable * const table)
{
  int *listface = NULL,
    sizelistface = 0;
  vector3d normale_sommet;

  a2ri_vf_faces_next_vertex_with_hashtable (m, numve, &listface,
					    &sizelistface, table);
  vector3d_init (&normale_sommet, 0.0, 0.0, 0.0);
  //calcul de la normale au sommet
  for (int i = 0; i < sizelistface; i++)
    {
      vector3d vtemp=a2ri_vf_normal_face (m, listface[i]);
      normale_sommet = vector3d_add (&normale_sommet, &vtemp);
    }
  vector3d_init (&normale_sommet, normale_sommet.dx / sizelistface,
		 normale_sommet.dy / sizelistface,
		 normale_sommet.dz / sizelistface);
  vector3d_normalize (&normale_sommet);

  free (listface);

  return normale_sommet;
}


/**
   Concaténation de plusieurs maillages dans un seul modèle
   @param m tableau de modèle
   @param size nombre de modele numérique
   @return un maillage content tous les maillages.
*/
vf_model *
a2ri_vf_concat (
		const pt_vf_model * const m,
		int size)
{
  vf_model *retour = NULL;
  int ancienvertex = 0;

  retour = (vf_model *) malloc (sizeof (vf_model));
  a2ri_erreur_critique_si (retour == NULL,
			"erreur d'allocation memoire dans la fonciton a2ri_vf_concat");

  a2ri_vf_init (retour);

  retour->xmin = retour->xmax = m[0]->ve[0].x;
  retour->ymin = retour->ymax = m[0]->ve[0].y;
  retour->zmin = retour->zmax = m[0]->ve[0].z;

  for (int i = 0; i < size; i++)
    {
      for (int j = 0; j < m[i]->nbvertex; j++)
	{
	  a2ri_vf_add_vertex (retour, m[i]->ve[j].x, m[i]->ve[j].y,
			      m[i]->ve[j].z);
	  if (m[i]->ve[j].x < retour->xmin)
	    retour->xmin = m[i]->ve[j].x;
	  if (m[i]->ve[j].y < retour->ymin)
	    retour->ymin = m[i]->ve[j].y;
	  if (m[i]->ve[j].z < retour->zmin)
	    retour->zmin = m[i]->ve[j].z;
	  if (m[i]->ve[j].x > retour->xmax)
	    retour->xmax = m[i]->ve[j].x;
	  if (m[i]->ve[j].y > retour->ymax)
	    retour->ymax = m[i]->ve[j].y;
	  if (m[i]->ve[j].z > retour->zmax)
	    retour->zmax = m[i]->ve[j].z;
	}

      for (int j = 0; j < m[i]->nbface; j++)
	a2ri_vf_add_face (retour, m[i]->fa[j].ve1 + ancienvertex,
			  m[i]->fa[j].ve2 + ancienvertex,
			  m[i]->fa[j].ve3 + ancienvertex);

      ancienvertex += m[i]->nbvertex;
    }

  return retour;

}

/**
   Ajout des points d'un vf_model dans une partition de l'espace
   @param m le modèle
   @param sp la partition de l'espace
   @return aucun
*/
void
a2ri_vf_space_partition (
			 const vf_model * const m,
			 space_partition * sp)
{
  point3d p;
  for (int i = 0; i < m->nbvertex; i++)
    {
      point3d_init (&p, m->ve[i].x, m->ve[i].y, m->ve[i].z);
      p.att_int = i;
      space_partition_add_point (sp, &p);
    }
}

/**
   Conversion d'un vf_model en liste de point3d
   @param m le modèle
   @return la liste de point3d
**/
point3d *
a2ri_vf_to_list_point3d(
			const vf_model * const m)
{
  point3d *retour=(point3d*)malloc(m->nbvertex*sizeof(point3d));
  a2ri_erreur_critique_si(retour==NULL,"(a2ri_vf_to_list_point3d) - erreur allocation mémoire pour retour\n");

  for(int i=0;i<m->nbvertex;i++)
    point3d_init(&retour[i],m->ve[i].x,m->ve[i].y,m->ve[i].z);
  
  return retour;
}

/**
   Conversion d'un vf_model en vef_model
   @param le modèle à convertir
   @return le vef_model
 **/
vef_model *
a2ri_vf_to_vef (
		const vf_model * const m)
{
   int p1,
     p2,ar1,ar2,ar3;
   
  vef_model *retour=(vef_model*)malloc(sizeof(vef_model));
  a2ri_erreur_critique_si(retour==NULL,"erreur allocation mémoire pour retour dans a2ri_vf_to_vef");

  a2ri_vef_init(retour);

  for(int i=0;i<m->nbvertex;i++)
    a2ri_vef_add_vertex(retour,m->ve[i].x,m->ve[i].y,m->ve[i].z);

  hashtable *table=hashtable_new (m->nbvertex + m->nbface);

  for (int i = 0; i < m->nbface; i++)
    {
      p1 = m->fa[i].ve1;
      p2 = m->fa[i].ve2;
      vf_edge *edgetemp = hashtable_look_for (table, p1, p2);
      if (edgetemp == NULL)
	{
	  edgetemp = (vf_edge *) malloc (sizeof (vf_edge));
	  a2ri_erreur_critique_si (edgetemp == NULL,
				"erreur allocation memoire pour edgetemp\na2ri_vf_to_vef");
	  edgetemp->ve1 = p1;
	  edgetemp->ve2 = p2;
	  edgetemp->att_int = retour->nbedge;
	  hashtable_add (table, edgetemp);
	  ar1 = retour->nbedge;
	  a2ri_vef_add_edge(retour,p1,p2,0);
	}
      else
	ar1=edgetemp->att_int;

      p1 = m->fa[i].ve2;
      p2 = m->fa[i].ve3;
      edgetemp = hashtable_look_for (table, p1, p2);
      if (edgetemp == NULL)
	{
	  edgetemp = (vf_edge *) malloc (sizeof (vf_edge));
	  a2ri_erreur_critique_si (edgetemp == NULL,
				"erreur allocation memoire pour edgetemp\na2ri_vf_to_vef");
	  edgetemp->ve1 = p1;
	  edgetemp->ve2 = p2;
	  edgetemp->att_int = retour->nbedge;
	  hashtable_add (table, edgetemp);
	  ar2 = retour->nbedge;
	  a2ri_vef_add_edge(retour,p1,p2,0);
	}
      else
	ar2=edgetemp->att_int;

      p1 = m->fa[i].ve3;
      p2 = m->fa[i].ve1;
      edgetemp = hashtable_look_for (table, p1, p2);
      if (edgetemp == NULL)
	{
	  edgetemp = (vf_edge *) malloc (sizeof (vf_edge));
	  a2ri_erreur_critique_si (edgetemp == NULL,
				"erreur allocation memoire pour edgetemp\na2ri_vf_to_vef");
	  edgetemp->ve1 = p1;
	  edgetemp->ve2 = p2;
	  edgetemp->att_int = retour->nbedge;
	  hashtable_add (table, edgetemp);
	  ar3 = retour->nbedge;
	  a2ri_vef_add_edge(retour,p1,p2,0);
	}
      else
	ar3=edgetemp->att_int;

      a2ri_vef_add_face(retour,ar1,ar2,ar3);
    }

  retour->xmin=m->xmin;
  retour->xmax=m->xmax;
  retour->ymin=m->ymin;
  retour->ymax=m->ymax;
  retour->zmin=m->zmin;
  retour->zmax=m->zmax;
  

  return retour;
}






/**
   Initialisation d'un modèle avec un maillage vide
   @param m pointeur sur le modèle
   @return aucun
*/
void
a2ri_vef_init (
	       vef_model * m)
{
  m->ve = NULL;
  m->ed = NULL;
  m->fa = NULL;
  m->nbvertex = 0;
  m->nbedge = 0;
  m->nbface = 0;
}

/**
   Libération de l'espace mémoire utilisé
   @param m pointeur sur le modèle
*/
void
a2ri_vef_free (
	       vef_model * m)
{
  int i;
  for (i = 0; i < m->nbvertex; i++)
    free (m->ve[i].sharededges);
  free (m->ve);

  for (i = 0; i < m->nbedge; i++)
    free (m->ed[i].sharedfaces);
  free (m->ed);

  free (m->fa);

}

/**
 affichage des caractéristiques générales d'un modèle (nombre de sommets et de faces)
 @param m le modèle
 @return aucun
*/
void
a2ri_vef_display (
		  const vef_model * const m)
{
  printf ("********************\nModel :");
  printf ("\n\t%d vertices\n", m->nbvertex);
  printf ("\t%d edges\n", m->nbedge);
  printf ("\t%d faces\n", m->nbface);
  printf
    ("Bounding Box : [%4.2f , %4.2f , %4.2f] - [%4.2f , %4.2f , %4.2f]\n",
     m->xmin, m->ymin, m->zmin, m->xmax, m->ymax, m->zmax);
  printf ("********************\n");
}

/**
 affichage des caractéristiques détaillées d'un modèle (nombre de sommets, de faces et leurs adjacences)
 @param m le modèle
 @return aucun
*/
void
a2ri_vef_display_detail (
			 const vef_model * const m)
{
  int i,
    j;
  printf ("********************\nModel :");

  if (m->nbvertex)
    {
      printf ("\n\t%d vertices\n", m->nbvertex);
      for (i = 0; i < m->nbvertex; i++)
	{
	  printf
	    ("\t\t%5d - [%5.2f , %5.2f , %5.2f]\n\t\t\tvalency : %d - list : ",
	     i, m->ve[i].x, m->ve[i].y, m->ve[i].z, m->ve[i].nbsharededges);
	  if (m->ve[i].nbsharededges > 0)
	    printf ("%d", m->ve[i].sharededges[0]);
	  for (j = 1; j < m->ve[i].nbsharededges; j++)
	    printf (" , %d", m->ve[i].sharededges[j]);
	  printf ("\n\n");
	}
    }

  if (m->nbedge)
    {
      printf ("\n\t%d edges\n", m->nbedge);
      for (i = 0; i < m->nbedge; i++)
	{
	  printf ("\t\t%5d - (%d , %d)\n\t\t\tvalency : %d - list : ",
		  i, m->ed[i].ve1, m->ed[i].ve2, m->ed[i].nbsharedfaces);
	  if (m->ed[i].nbsharedfaces > 0)
	    printf ("%d", m->ed[i].sharedfaces[0]);
	  for (j = 1; j < m->ed[i].nbsharedfaces; j++)
	    printf (" , %d", m->ed[i].sharedfaces[j]);
	  printf ("\n\n");
	}
    }

  if (m->nbface)
    {
      printf ("\n\t%d faces\n", m->nbface);
      for (i = 0; i < m->nbface; i++)
	{
	  printf ("\t\t%5d - (%d , %d , %d)",
		  i, m->fa[i].ed1, m->fa[i].ed2, m->fa[i].ed3);
	  printf ("\n\n");
	}
    }

  printf
    ("\nBounding Box : [%4.2f , %4.2f , %4.2f] - [%4.2f , %4.2f , %4.2f]\n",
     m->xmin, m->ymin, m->zmin, m->xmax, m->ymax, m->zmax);
  printf ("********************\n");
}

/**
   Ajout d'un vertex (x,y,z) au modèle
   @param m pointeur sur le modèle
   @param x coordonnée x du vertex
   @param y coordonnée y
   @param z coordonnée z
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_add_vertex (
		     vef_model * m,
		     double x,
		     double y,
		     double z)
{
  //allocation de l'espace mémoire ou réallocation
  if (m->ve == NULL)
    {
      m->ve = (vef_vertex *) malloc (sizeof (vef_vertex));
      a2ri_erreur_critique_si (m->ve == NULL,
			    "erreur allocation memoire pour m->ve\na2ri_vef_add_vertex");
    }
  else
    {
      vef_vertex *tempve;
      tempve =
	(vef_vertex *) realloc (m->ve,
				(m->nbvertex + 1) * sizeof (vef_vertex));
      a2ri_erreur_critique_si (tempve == NULL,
			    "erreur allocation memoire pour m->ve\na2ri_vef_add_vertex");
      m->ve=tempve;
    }
  m->ve[m->nbvertex].x = x;
  m->ve[m->nbvertex].y = y;
  m->ve[m->nbvertex].z = z;
  m->ve[m->nbvertex].sharededges = NULL;
  m->ve[m->nbvertex].nbsharededges = 0;
  (m->nbvertex)++;

  return 1;
}

/**
   Recherche d'un vertex dans un modèle
   @param m le modèle
   @param x coordonnée x du vertex à rechercher
   @param y coordonnée y
   @param z coordonnée z
   @return numéro du vertex dans la liste de vertex du modèle, -1 si le vertex n'est pas trouvé.
*/
int
a2ri_vef_search_vertex (
			const vef_model * const m,
			double x,
			double y,
			double z)
{
  int i;
  for (i = 0; i < m->nbvertex; i++)
    if (m->ve[i].x == x && m->ve[i].y == y && m->ve[i].z == z)
      return i;

  return -1;
}

/**
   Retrait d'un sommet du modèle. L'opération ne peut être effectué que si le sommet n'est référencée dans aucune face.
   @param m le modèle
   @param numvertex numèro du vertex à retirer
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_remove_vertex (
			vef_model * m,
			int numvertex)
{
  if (m->ed == NULL)
    {
      //on vérifie que le sommet existe bien
      if (numvertex > m->nbvertex)
	return 0;

      //on décale les sommets de rang plus élevé
      for (int i = numvertex; i < (m->nbvertex) - 1; i++)
	{
	  m->ve[i].x = m->ve[i + 1].x;
	  m->ve[i].y = m->ve[i + 1].y;
	  m->ve[i].z = m->ve[i + 1].z;
	  m->ve[i].sharededges = m->ve[i + 1].sharededges;
	  m->ve[i].nbsharededges = m->ve[i + 1].nbsharededges;
	}


      //reallocation de l'espace memoire
      vef_vertex *tempve;
      tempve =
	(vef_vertex *) realloc (m->ve,
				((m->nbvertex) - 1) * sizeof (vef_vertex));
      a2ri_erreur_critique_si (tempve == NULL,
			    "erreur allocation memoire pour m->ve\na2ri_vef_remove_vertex");
      m->ve=tempve;
      m->nbvertex = m->nbvertex - 1;

    }
  else
    {
      a2ri_vef_remove_list_of_vertex (m, &numvertex, 1);
    }
  return 1;
}

/**
   Retrait d'un sommet du modèle. L'opération ne peut être effectué que si le sommet n'est référencée dans aucune arete.
   @param m le modèle
   @param numvertex numèro du vertex à retirer
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_remove_list_of_vertex (
				vef_model * m,
				int *listvertex,
				int size)
{
  int *list1 = NULL,
    *list2 = NULL,
    size1 = 0,
    size2 = 0;

  list_int_sort (listvertex, size, DESC);

  if (m->ed == NULL)
    {
      for (int i = 0; i < size; i++)
	a2ri_vef_remove_vertex (m, listvertex[i]);
      return 1;
    }
  list1 = NULL;
  size1 = 0;
  for (int i = 0; i < size; i++)
    for (int j = 0; j < m->ve[listvertex[i]].nbsharededges; j++)
      list_int_add (&list1, &size1, m->ve[listvertex[i]].sharededges[j],
		    WITHOUT_REDUNDANCE);
  list_int_sort (list1, size1, DESC);
  if (m->fa == NULL)
    {
      for (int i = 0; i < size1; i++)
	a2ri_vef_remove_edge (m, list1[i]);
      free (list1);
      return 1;
    }
  list2 = NULL;
  size2 = 0;
  for (int i = 0; i < size1; i++)
    for (int j = 0; j < m->ed[list1[i]].nbsharedfaces; j++)
      list_int_add (&list2, &size2, m->ed[list1[i]].sharedfaces[j],
		    WITHOUT_REDUNDANCE);
  list_int_sort (list2, size2, DESC);

  for (int i = 0; i < size2; i++)
    a2ri_vef_remove_face (m, list2[i]);
  free (list1);
  free (list2);
  return 1;



  return 1;
}

/**
   Ajout d'une arete (ve1,ve2) au modèle
   @param m pointeur sur le modèle
   @param ve1 index du premier point
   @param ve2 index du second point
   @param verif 1 s'il faut vérifier si l'arete existe deja, 0 sinon
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_add_edge (
		   vef_model * m,
		   int ve1,
		   int ve2,
		   int verif)
{
  //si vérification, on regarde si l'arete existe deja
  //sinon on est sur d'ajouter une arete inexistante
  if (verif)
    if (ve1 >= m->nbvertex || ve2 >= m->nbvertex)
      return 0;

  if (verif)
    if (a2ri_vef_search_edge (m, ve1, ve2) != -1)
      return 0;

  //allocation ou reallocatin de l'espace memoire
  if (m->ed == NULL)
    {
      m->ed = (vef_edge *) malloc (sizeof (vef_edge));
      a2ri_erreur_critique_si (m->ed == NULL,
			    "erreur allocation memoire pour m->ed\na2ri_vef_add_edge");
    }
  else
    {
      vef_edge *temped;
      temped =
	(vef_edge *) realloc (m->ed, (m->nbedge + 1) * sizeof (vef_edge));
      a2ri_erreur_critique_si (temped == NULL,
			    "erreur allocation memoire pour m->ed\na2ri_vef_add_vertex");
      m->ed=temped;
    }

  m->ed[m->nbedge].ve1 = ve1;
  m->ed[m->nbedge].ve2 = ve2;
  m->ed[m->nbedge].sharedfaces = NULL;
  m->ed[m->nbedge].nbsharedfaces = 0;
  (m->nbedge)++;

  list_int_add (&(m->ve[ve1].sharededges),
		&(m->ve[ve1].nbsharededges),
		(m->nbedge) - 1, WITH_REDUNDANCE);
  list_int_add (&(m->ve[ve2].sharededges),
		&(m->ve[ve2].nbsharededges),
		(m->nbedge) - 1, WITH_REDUNDANCE);

  return 1;
}

/**
   Recherche d'une arete dans un modèle
   @param m le modèle
   @param ve1 indice d'un point
   @param ve2 indice d'un point
   @return index de l'arete, -1 si l'arete n'est pas trouvée.
   @warning a2ri_vef_search_edge(m,0,1) retournera le meme resultat que a2ri_vef_search_edge(m,1,0).
*/
int
a2ri_vef_search_edge (
		      const vef_model * const m,
		      int ve1,
		      int ve2)
{
  int i;
  for (i = 0; i < m->nbedge; i++)
    if ((m->ed[i].ve1 == ve1 && m->ed[i].ve2 == ve2)
	|| (m->ed[i].ve1 == ve2 && m->ed[i].ve2 == ve1))
      return i;

  return -1;
}

/**
   Retrait d'une arete du modèle. L'opération ne peut être effectué que si l'arete n'est référencée dans aucune face.
   @param m le modèle
   @param numvertex numèro du vertex à retirer
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_remove_edge (
		      vef_model * m,
		      int numedge)
{
  int k,
    ve[2];

  vef_vertex *tempve;
  vef_edge *temped;

  if (m->fa == NULL)
    {

      //on vérifie l'existence de l'arete
      if (numedge > m->nbedge)
	return 0;

      if (m->ed[numedge].ve1 > m->ed[numedge].ve2)
	{
	  ve[0] = m->ed[numedge].ve1;
	  ve[1] = m->ed[numedge].ve2;
	}
      else
	{
	  ve[0] = m->ed[numedge].ve2;
	  ve[1] = m->ed[numedge].ve1;
	}

      //on décale les aretes de rang plus élevé
      for (int i = numedge; i < (m->nbedge) - 1; i++)
	{
	  m->ed[i].ve1 = m->ed[i + 1].ve1;
	  m->ed[i].ve2 = m->ed[i + 1].ve2;
	  m->ed[i].sharedfaces = m->ed[i + 1].sharedfaces;
	  m->ed[i].nbsharedfaces = m->ed[i + 1].nbsharedfaces;
	}

      //reallocation memoire
      temped =
	(vef_edge *) realloc (m->ed, ((m->nbedge) - 1) * sizeof (vef_edge));
      a2ri_erreur_critique_si (temped == NULL,
			    "erreur allocation memoire pour m->ed\na2ri_vef_remove_edge");
      m->ed=temped;
      m->nbedge = m->nbedge - 1;

      // on decale les numeros d'aretes dans les adjacences des sommets
      for (int i = 0; i < m->nbvertex; i++)
	{
	  k = 0;
	  while (k < m->ve[i].nbsharededges)
	    {
	      if (m->ve[i].sharededges[k] == numedge)
		list_int_remove (&(m->ve[i].sharededges),
				 &(m->ve[i].nbsharededges), k);
	      else
		{
		  if (m->ve[i].sharededges[k] > numedge)
		    (m->ve[i].sharededges[k])--;
		  k++;
		}
	    }
	}

      //on essaye de supprimer les sommets
      for (int i = 0; i < 2; i++)
	if (m->ve[ve[i]].nbsharededges == 0)
	  {
	    int numvertex = ve[i];
	    for (int j = numvertex; j < (m->nbvertex) - 1; j++)
	      {
		m->ve[j].x = m->ve[j + 1].x;
		m->ve[j].y = m->ve[j + 1].y;
		m->ve[j].z = m->ve[j + 1].z;
		m->ve[j].sharededges = m->ve[j + 1].sharededges;
		m->ve[j].nbsharededges = m->ve[j + 1].nbsharededges;
	      }

	    //reallocation memoire
	    tempve =
	      (vef_vertex *) realloc (m->ve,
				      ((m->nbvertex) -
				       1) * sizeof (vef_vertex));
	    a2ri_erreur_critique_si (tempve == NULL,
				  "erreur allocation memoire pour m->ve\na2ri_vef_remove_edge");
	    m->ve=tempve;
	    m->nbvertex = m->nbvertex - 1;

	    for (int j = 0; j < m->nbedge; j++)
	      {
		if (m->ed[j].ve1 > numvertex)
		  m->ed[j].ve1 = m->ed[j].ve1 - 1;
		if (m->ed[j].ve2 > numvertex)
		  m->ed[j].ve2 = m->ed[j].ve2 - 1;
	      }
	  }
    }
  else
    {
      a2ri_vef_remove_list_of_edge (m, &numedge, 1);
    }

  return 1;
}

/**
   Retrait d'une liste d'arete du modèle. L'opération ne peut être effectué que si l'arete n'est référencée dans aucune face.
   @param m le modèle
   @param numvertex numèro du vertex à retirer
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_remove_list_of_edge (
			      vef_model * m,
			      int *listedge,
			      int size)
{
  int *list1 = NULL,
    size1 = 0;

  list_int_sort (listedge, size, DESC);

  if (m->fa == NULL)
    {
      for (int i = 0; i < size; i++)
	a2ri_vef_remove_edge (m, listedge[i]);
      return 1;
    }
  list1 = NULL;
  size1 = 0;
  for (int i = 0; i < size; i++)
    for (int j = 0; j < m->ed[listedge[i]].nbsharedfaces; j++)
      list_int_add (&list1, &size1, m->ed[listedge[i]].sharedfaces[j],
		    WITHOUT_REDUNDANCE);
  list_int_sort (list1, size1, DESC);

  for (int i = 0; i < size1; i++)
    a2ri_vef_remove_face (m, list1[i]);
  free (list1);
  return 1;

}

/**
   Ajout d'une face défini par trois points (numéros d'index dans la liste de points) au modèle
   @param m pointeur sur le modèle
   @param ve1 index du premier point
   @param ve2 index du second point
   @param ve3 index du troisième point
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_add_face (
		   vef_model * m,
		   int ed1,
		   int ed2,
		   int ed3)
{
  //allocation ou reallocation de l'espace memoire
  if (m->fa == NULL)
    {
      m->fa = (vef_face *) malloc (sizeof (vef_face));
      a2ri_erreur_critique_si (m->fa == NULL,
			    "erreur allocation memoire pour m->fa\na2ri_vef_add_face");
    }
  else
    {
      vef_face *tempfa;
      tempfa =
	(vef_face *) realloc (m->fa, (m->nbface + 1) * sizeof (vef_face));
      a2ri_erreur_critique_si (tempfa == NULL,
			    "erreur allocation memoire pour m->fa\na2ri_vef_add_face");
      m->fa=tempfa;
    }

  m->fa[m->nbface].ed1 = ed1;
  m->fa[m->nbface].ed2 = ed2;
  m->fa[m->nbface].ed3 = ed3;
  (m->nbface)++;

  list_int_add (&(m->ed[ed1].sharedfaces),
		&(m->ed[ed1].nbsharedfaces),
		(m->nbface) - 1, WITH_REDUNDANCE);
  list_int_add (&(m->ed[ed2].sharedfaces),
		&(m->ed[ed2].nbsharedfaces),
		(m->nbface) - 1, WITH_REDUNDANCE);
  list_int_add (&(m->ed[ed3].sharedfaces),
		&(m->ed[ed3].nbsharedfaces),
		(m->nbface) - 1, WITH_REDUNDANCE);

  return 1;
}

/**
   Recherche d'une face dans un modèle
   @param m le modèle
   @param ve1 index d'un point
   @param ve2 index d'un point
   @param ve3 index d'un point
   @return numéro de la face dans la liste de face du modèle, -1 si la face n'est pas trouvée.
   @warning Cette fonction est sensible à l'orientation de la face.
   Exemple pour un modèle m:
   A REECRIRE
   @code
   liste de faces (index -> ve1 ve2 ve3):
   0 -> 0 1 2
   1 -> 0 2 3 
   2 -> 0 3 1
   3 -> 1 3 2
   @endcode
   le code
   @code
   a2ri_vef_search_face(m,0,3,1);
   ou
   a2ri_vef_search_face(m,3,1,0);
   ou
   a2ri_vef_search_face(m,1,0,3);
   @endcode
   retournera 2 alors que
   @code
   a2ri_vef_search_face(m,3,0,1);
   @endcode
   retournera -1
*/
int
a2ri_vef_search_face (
		      const vef_model * const m,
		      int ed1,
		      int ed2,
		      int ed3)
{
  int i;
  for (i = 0; i < m->nbface; i++)
    {
      if (m->fa[i].ed1 == ed1)
	if ((m->fa[i].ed2 == ed2 && m->fa[i].ed3 == ed3)
	    || (m->fa[i].ed2 == ed3 && m->fa[i].ed3 == ed2))
	  return i;
      if (m->fa[i].ed2 == ed1)
	if ((m->fa[i].ed1 == ed3 && m->fa[i].ed3 == ed2))
	  return i;
      if (m->fa[i].ed3 == ed1)
	if ((m->fa[i].ed2 == ed3 && m->fa[i].ed1 == ed2))
	  return i;
    }

  return -1;
}

/**
   Retrait d'une face du modèle. La fonction enlève également les sommets qui ne sont plus utilisés.
   @param m le modèle
   @param numface numèro de la face à retirer
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_remove_face (
		      vef_model * m,
		      int numface)
{
  int k,
    ed[3],
    ve[3];
  vef_vertex *tempve;
  vef_edge *temped;
  vef_face *tempfa;

  //on verifie l'existence de la face
  if (numface > m->nbface)
    return 0;

  //on garde les numeros d'aretes et de sommet pour esayer de les supprimer ensuite
  ed[0] = m->fa[numface].ed1;
  ed[1] = m->fa[numface].ed2;
  ed[2] = m->fa[numface].ed3;

  list_int_sort (ed, 3, DESC);

  vef_face_get_vertices (&(m->fa[numface]), m->ed, &ve[0], &ve[1], &ve[2]);

  list_int_sort (ve, 3, DESC);

  //décalage des faces de rang supérieur
  for (int i = numface; i < (m->nbface) - 1; i++)
    {
      m->fa[i].ed1 = m->fa[i + 1].ed1;
      m->fa[i].ed2 = m->fa[i + 1].ed2;
      m->fa[i].ed3 = m->fa[i + 1].ed3;
    }

  //reallocation de l'espace memoire
  tempfa =
    (vef_face *) realloc (m->fa, ((m->nbface) - 1) * sizeof (vef_face));
  a2ri_erreur_critique_si (tempfa == NULL,
			   "erreur allocation memoire pour m->fa\na2ri_vef_remove_face");
  m->fa=tempfa;
  m->nbface = m->nbface - 1;

  //decalage et suppression dans les adjacences d'aretes
  for (int i = 0; i < m->nbedge; i++)
    {
      k = 0;
      while (k < m->ed[i].nbsharedfaces)
	{
	  if (m->ed[i].sharedfaces[k] == numface)
	    list_int_remove (&(m->ed[i].sharedfaces),
			     &(m->ed[i].nbsharedfaces), k);
	  else
	    {
	      if (m->ed[i].sharedfaces[k] > numface)
		m->ed[i].sharedfaces[k] = m->ed[i].sharedfaces[k] - 1;
	      k++;
	    }
	}
    }

  //on essaye de supprimer les aretes inutilisées
  for (int i = 0; i < 3; i++)
    if (m->ed[ed[i]].nbsharedfaces == 0)
      {
	int numedge = ed[i];

	//on décale les aretes de rang plus élevé
	for (int j = numedge; j < (m->nbedge) - 1; j++)
	  {
	    m->ed[j].ve1 = m->ed[j + 1].ve1;
	    m->ed[j].ve2 = m->ed[j + 1].ve2;
	    m->ed[j].sharedfaces = m->ed[j + 1].sharedfaces;
	    m->ed[j].nbsharedfaces = m->ed[j + 1].nbsharedfaces;
	  }

	//reallocation memoire
	temped =
	  (vef_edge *) realloc (m->ed,
				((m->nbedge) - 1) * sizeof (vef_edge));
	a2ri_erreur_critique_si (temped == NULL,
				 "erreur allocation memoire pour m->ed\na2ri_vef_remove_face");
	m->ed=temped;
	m->nbedge = m->nbedge - 1;

	// on decale les numeros d'aretes dans les adjacences des sommets
	for (int j = 0; j < m->nbvertex; j++)
	  {
	    k = 0;
	    while (k < m->ve[j].nbsharededges)
	      {
		if (m->ve[j].sharededges[k] == numedge)
		  list_int_remove (&(m->ve[j].sharededges),
				   &(m->ve[j].nbsharededges), k);
		else
		  {
		    if (m->ve[j].sharededges[k] > numedge)
		      (m->ve[j].sharededges[k])--;
		    k++;
		  }
	      }
	  }

	// on decale les numeros d'aretes dans les faces
	for (int j = 0; j < m->nbface; j++)
	  {
	    if (m->fa[j].ed1 > numedge)
	      m->fa[j].ed1--;
	    if (m->fa[j].ed2 > numedge)
	      m->fa[j].ed2--;
	    if (m->fa[j].ed3 > numedge)
	      m->fa[j].ed3--;
	  }
      }

  //on essaye de supprimer les sommets
  for (int i = 0; i < 3; i++)
    if (m->ve[ve[i]].nbsharededges == 0)
      {
	int numvertex = ve[i];
	//on décale les sommets de rang plus élevé
	for (int j = numvertex; j < (m->nbvertex) - 1; j++)
	  {
	    m->ve[j].x = m->ve[j + 1].x;
	    m->ve[j].y = m->ve[j + 1].y;
	    m->ve[j].z = m->ve[j + 1].z;
	    m->ve[j].sharededges = m->ve[j + 1].sharededges;
	    m->ve[j].nbsharededges = m->ve[j + 1].nbsharededges;
	  }


	//reallocation de l'espace memoire
	tempve =
	  (vef_vertex *) realloc (m->ve,
				  ((m->nbvertex) -
				   1) * sizeof (vef_vertex));
	a2ri_erreur_critique_si (tempve == NULL,
				 "erreur allocation memoire pour m->ve\na2ri_vef_remove_face");
	m->ve=tempve;
	m->nbvertex = m->nbvertex - 1;

	//on met à jour les numéros de sommets dans les aretes
	for (int j = 0; j < m->nbedge; j++)
	  {
	    if (m->ed[j].ve1 > numvertex)
	      m->ed[j].ve1 = m->ed[j].ve1 - 1;
	    if (m->ed[j].ve2 > numvertex)
	      m->ed[j].ve2 = m->ed[j].ve2 - 1;
	  }
      }

  return 1;
}

/**
   Retrait d'une liste de face du modèle. La fonction enlève également les sommets qui ne sont plus utilisés.
   @param m le modèle
   @param numface numèro de la face à retirer
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_remove_list_of_face (
			      vef_model * m,
			      int *listface,
			      int size)
{
  list_int_sort (listface, size, DESC);

  for (int i = 0; i < size; i++)
    a2ri_vef_remove_face (m, listface[i]);
  return 1;
}

/**
   Translation du modèle
   @param m pointeur sur le modèle à translater
   @param delta pointeur sur le vecteur de translation
   @return aucun
*/
void
a2ri_vef_translate (
		    vef_model * m,
		    const vector3d * const delta)
{
  //translation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    vef_vertex_translate (&(m->ve[i]), delta);

  //translation de la bounding box
  m->xmin += delta->dx;
  m->xmax += delta->dx;
  m->ymin += delta->dy;
  m->ymax += delta->dy;
  m->zmin += delta->dz;
  m->zmax += delta->dz;
}

/**
   Rotation d'un modèle en radian autour de l'axe X
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @return aucun
*/
void
a2ri_vef_rotateX_radian (
			 vef_model * m,
			 double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotateX_radian (&(m->ve[i]), angle);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en degré autour de l'axe X
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degré
   @return aucun
*/
void
a2ri_vef_rotateX_degre (
			vef_model * m,
			double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotateX_degre (&(m->ve[i]), angle);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en radian suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void
a2ri_vef_rotateX_radian_center (
				vef_model * m,
				double angle,
				const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotateX_center_radian (&(m->ve[i]), angle, centre);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en degre suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void
a2ri_vef_rotateX_degre_center (
			       vef_model * m,
			       double angle,
			       const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotateX_center_degre (&(m->ve[i]), angle, centre);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en radian autour de l'axe Y
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @return aucun
*/
void
a2ri_vef_rotateY_radian (
			 vef_model * m,
			 double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotateY_radian (&(m->ve[i]), angle);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}

    }
}

/**
   Rotation d'un modèle en degré autour de l'axe Y
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degré
   @return aucun
*/
void
a2ri_vef_rotateY_degre (
			vef_model * m,
			double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotateY_degre (&(m->ve[i]), angle);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en radian suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void
a2ri_vef_rotateY_radian_center (
				vef_model * m,
				double angle,
				const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotateY_center_radian (&(m->ve[i]), angle, centre);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en degre suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void
a2ri_vef_rotateY_degre_center (
			       vef_model * m,
			       double angle,
			       const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotateY_center_degre (&(m->ve[i]), angle, centre);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en radian autour de l'axe Z
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @return aucun
*/
void
a2ri_vef_rotateZ_radian (
			 vef_model * m,
			 double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotateZ_radian (&(m->ve[i]), angle);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en degré autour de l'axe Z
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degré
   @return aucun
*/
void
a2ri_vef_rotateZ_degre (
			vef_model * m,
			double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotateZ_degre (&(m->ve[i]), angle);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en radian suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void
a2ri_vef_rotateZ_radian_center (
				vef_model * m,
				double angle,
				const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotateZ_center_radian (&(m->ve[i]), angle, centre);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en degre suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void
a2ri_vef_rotateZ_degre_center (
			       vef_model * m,
			       double angle,
			       const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotateZ_center_degre (&(m->ve[i]), angle, centre);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
}

/**
   Rotation d'un modèle en degré suivant l'axe donnée
   @param m pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param axe l'axe de rotation
   @return aucun
*/
void
a2ri_vef_rotate_axe_radian (
			    vef_model * m,
			    double angle,
			    const vector3d * const axe)
{
  gsl_matrix *identity3,
    *A,
    *B,
    *M;
  double x = axe->dx;
  double y = axe->dy;
  double z = axe->dz;

  identity3 = gsl_matrix_alloc (3, 3);
  gsl_matrix_set_identity (identity3);

  A = gsl_matrix_alloc (3, 3);
  gsl_matrix_set (A, 0, 0, x * x);
  gsl_matrix_set (A, 0, 1, x * y);
  gsl_matrix_set (A, 0, 2, x * z);
  gsl_matrix_set (A, 1, 0, x * y);
  gsl_matrix_set (A, 1, 1, y * y);
  gsl_matrix_set (A, 1, 2, y * z);
  gsl_matrix_set (A, 2, 0, x * z);
  gsl_matrix_set (A, 2, 1, y * z);
  gsl_matrix_set (A, 2, 2, z * z);


  B = gsl_matrix_alloc (3, 3);
  gsl_matrix_set (B, 0, 0, 0);
  gsl_matrix_set (B, 0, 1, -z);
  gsl_matrix_set (B, 0, 2, y);
  gsl_matrix_set (B, 1, 0, z);
  gsl_matrix_set (B, 1, 1, 0);
  gsl_matrix_set (B, 1, 2, -x);
  gsl_matrix_set (B, 2, 0, -y);
  gsl_matrix_set (B, 2, 1, x);
  gsl_matrix_set (B, 2, 2, 0);


  M =
    matrix_add (matrix_add
		(matrix_mul_scale (identity3, cos (angle)),
		 matrix_mul_scale (A, 1 - cos (angle))), matrix_mul_scale (B,
									   sin
									   (angle)));


  //rotation de tous les sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      vef_vertex_rotate_axe_radian (&(m->ve[i]), M);

      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = m->ve[i].x;
	  m->xmax = m->ve[i].x;
	  m->ymin = m->ve[i].y;
	  m->ymax = m->ve[i].y;
	  m->zmin = m->ve[i].z;
	  m->zmax = m->ve[i].z;
	}
      else
	{
	  if (m->xmin > m->ve[i].x)
	    m->xmin = m->ve[i].x;
	  if (m->xmax < m->ve[i].x)
	    m->xmax = m->ve[i].x;
	  if (m->ymin > m->ve[i].y)
	    m->ymin = m->ve[i].y;
	  if (m->ymax < m->ve[i].y)
	    m->ymax = m->ve[i].y;
	  if (m->zmin > m->ve[i].z)
	    m->zmin = m->ve[i].z;
	  if (m->zmax < m->ve[i].z)
	    m->zmax = m->ve[i].z;
	}
    }
  gsl_matrix_free (A);
  gsl_matrix_free (B);
  gsl_matrix_free (M);
  gsl_matrix_free (identity3);
}

/**
   Calcul de l'aire totale d'un modèle
   @param m le modèle
   @return aire totale du modèle
*/
double
a2ri_vef_area (
	       const vef_model * const m)
{
  int ve1,
    ve2,
    ve3;
  double sumarea = 0.0;
  point3d A,
    B,
    C;

  //somme des aires de toutes les faces
  for (int i = 0; i < m->nbface; i++)
    {
      vef_face_get_vertices (&(m->fa[i]), m->ed, &ve1, &ve2, &ve3);

      point3d_init (&A, m->ve[ve1].x, m->ve[ve1].y, m->ve[ve1].z);
      point3d_init (&B, m->ve[ve2].x, m->ve[ve2].y, m->ve[ve2].z);
      point3d_init (&C, m->ve[ve3].x, m->ve[ve3].y, m->ve[ve3].z);
      sumarea += point3d_area (&A, &B, &C);
    }

  return sumarea;
}

/**
   Ajout des points d'un vef_model dans une partition de l'espace
   @param m le modèle
   @param sp la partition de l'espace
   @return aucun
*/
void
a2ri_vef_space_partition (
			  const vef_model * const m,
			  space_partition * sp)
{
  point3d p;
  for (int i = 0; i < m->nbvertex; i++)
    {
      point3d_init (&p, m->ve[i].x, m->ve[i].y, m->ve[i].z);
      p.att_int = i;
      space_partition_add_point (sp, &p);
    }
}

/**
   Conversion d'un vef_model en liste de point3d
   @param m le modèle
   @return la liste de point3d
**/
point3d *
a2ri_vef_to_list_point3d(
			 const vef_model * const m)
{
  point3d *retour=(point3d*)malloc(m->nbvertex*sizeof(point3d));
  a2ri_erreur_critique_si(retour==NULL,"(a2ri_vf_to_list_point3d) - erreur allocation mémoire pour retour\n");

  for(int i=0;i<m->nbvertex;i++)
    point3d_init(&retour[i],m->ve[i].x,m->ve[i].y,m->ve[i].z);
  
  return retour;
}

/**
   Conversion d'un vef_model en vf_model
   @param m le modèle à convertir
   @return un pointeur sur le vf_model
 **/
vf_model *
a2ri_vef_to_vf (
		const vef_model * const m)
{
  vf_model *retour=(vf_model*)malloc(sizeof(vf_model));
  a2ri_erreur_critique_si(retour==NULL,"erreur allocation mémoire pour retour dans a2ri_vef_to_vf");

  a2ri_vf_init(retour);

  for(int i=0;i<m->nbvertex;i++)
    a2ri_vf_add_vertex(retour,m->ve[i].x,m->ve[i].y,m->ve[i].z);

  int ve1,ve2,ve3;

  for(int i=0;i<m->nbface;i++)
    {
      vef_face_get_vertices(&(m->fa[i]),m->ed,&ve1,&ve2,&ve3);
      a2ri_vf_add_face(retour,ve1,ve2,ve3);
    }

  retour->xmin=m->xmin;
  retour->xmax=m->xmax;
  retour->ymin=m->ymin;
  retour->ymax=m->ymax;
  retour->zmin=m->zmin;
  retour->zmax=m->zmax;

  return retour;
}
