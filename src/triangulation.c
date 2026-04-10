/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 15/05/08       */
/* Date de modification : 15/03/15   */
/* Version : 0.4                     */
/*************************************/

/*************************************/
/* Auteur : Romain Leguay            */
/*          Nguyen Haiduong          */
/*          Solange Houeto           */
/*          Marianne Fichoux         */
/* Date de modification : 08/06/09   */
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

#include "triangulation.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

typedef struct
{
  point3d *liste_centre1,
    *liste_centre2;
  double *liste_rayon1,
    *liste_rayon2;
  double sensibility;
  int nbtest1deb,
    nbtest1fin,
    nbtest2;
  int **asupprimer,
    *nbelt;
} ITnettoyage_thread_argument;

typedef struct
{
  int *list;
  int nbelt;
} IT_bpa_liste;


void *
IFthread_process_nettoyage (
			  void *argu)
{
  ITnettoyage_thread_argument *arg =
    (ITnettoyage_thread_argument *) argu;

  *(arg->asupprimer) = NULL;
  *(arg->nbelt) = 0;

  for (int i = arg->nbtest1deb; i <= arg->nbtest1fin; i++)
    {
      if (!intersection_bounding_ball_with_bounding_ball_list
	  (&(arg->liste_centre1[i]), arg->liste_rayon1[i], arg->liste_centre2,
	   arg->liste_rayon2, arg->nbtest2, arg->sensibility))
	list_int_add (arg->asupprimer, arg->nbelt, i, WITH_REDUNDANCE);
    }

  pthread_exit (0);
}

//on cherche les aretes qui ne partagent qu'une seule face et on ajoute leur sommet dans une liste
//résultat : la liste des sommets appartenant au bord du maillage
void
IF_bpa_look_for_border (
		int key,
		vf_edge * value,
		void *user_data)
{
  IT_bpa_liste *list = user_data;

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
IF_bpa_cycle (
       IT_bpa_liste * list,
       IT_bpa_liste * cycle)
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

//recherche de cycle dans les arêtes du bord du modèle
int IF_bpa_vf_hole (vf_model * m, IT_bpa_liste *lescycles)
{
  int nbtrou = 0;
  IT_bpa_liste lescouples;
  lescouples.list = NULL;
  lescouples.nbelt = 0;
  lescycles->list = NULL;
  lescycles->nbelt = 0;
  hashtable *table = a2ri_vf_construction_edge_table (m, NULL, 0);

  hashtable_foreach (table, IF_bpa_look_for_border, &lescouples);
  //si on ne trouve aucun sommet sur le bord du maillage
  if (lescouples.nbelt == 0)
    {
      hashtable_free (table);
      free (table);
      free (lescouples.list);
      //il n'y a pas de trou
      return nbtrou;
    }

  //sinon tant qu'on a des elements dans la liste, on cherche un nouveau cycle
  while (lescouples.nbelt != 0)
    {
      IF_bpa_cycle (&lescouples, lescycles);
      //le nombre de cycle correspond au nombre de trou
      nbtrou++;
    }

  hashtable_free (table);
  free (table);
  free (lescouples.list);

  return nbtrou;
}

/********** MAIN FUNCTIONS **********/

/**
   Nettoyage par suppression des faces dans la zone de recouvrement
   @param base,m 2 vf_model
   @return aucun 
**/
void
nettoyage_delete_face (
		       vf_model * base,
		       vf_model * m,
		       double sensibility)
{
  hashtable *table = NULL;
  int recouvrement = 1;
  int tour = -1;
  int *candidat_m = NULL,
    size_candidat_m = 0,
    *candidat_base = NULL,
    size_candidat_base = 0;

  double *listrayonmodelm = NULL,
    *listrayonbase = NULL;
  point3d *listcentrebase = NULL,
    *listcentremodelm = NULL;

  a2ri_time deptotal,
    fintotal,
    depinter,
    fininter,
    depMAJ,
    finMAJ,
    depBB,
    finBB;
  double totalinter = 0,
    totalMAJ = 0;

  deptotal = a2ri_get_time ();

  pthread_t th[A2RI_NB_THREAD];
  ITnettoyage_thread_argument argument[A2RI_NB_THREAD];
  void *ret[A2RI_NB_THREAD];
  int *liste_a_supprimer[A2RI_NB_THREAD],
    size_liste_a_supprimer[A2RI_NB_THREAD];

  for (int i = 0; i < A2RI_NB_THREAD; i++)
    {
      liste_a_supprimer[i] = NULL;
      size_liste_a_supprimer[i] = 0;
    }

  //on va demander les bounding ball de toutes les faces des deux modèles
  candidat_m = (int *) malloc (m->nbface * sizeof (int));
  a2ri_erreur_critique_si (candidat_m == NULL,
			"erreur allocation memoire pour candidat_m\nnettoyage_delete_face");
  candidat_base = (int *) malloc (base->nbface * sizeof (int));
  a2ri_erreur_critique_si (candidat_base == NULL,
			"erreur allocation memoire pour candidat_base\nnettoyage_delete_face");

  for (int i = 0; i < m->nbface; i++)
    candidat_m[i] = i;
  size_candidat_m = m->nbface;

  for (int i = 0; i < base->nbface; i++)
    candidat_base[i] = i;
  size_candidat_base = base->nbface;

  //récupère toutes les bounding sphere de toutes les faces des modeles base et m
  depBB = a2ri_get_time ();
  a2ri_vf_bounding_ball_minimale (base, 
				  candidat_base, 
				  size_candidat_base,
				  &listcentrebase, 
				  &listrayonbase);
  a2ri_vf_bounding_ball_minimale (m, 
				  candidat_m, 
				  size_candidat_m,
				  &listcentremodelm, 
				  &listrayonmodelm);
  finBB = a2ri_get_time ();

  while (recouvrement)
    {
      tour++;

      for (int i = 0; i < A2RI_NB_THREAD; i++)
	{
	  free (liste_a_supprimer[i]);
	  liste_a_supprimer[i] = NULL;
	  size_liste_a_supprimer[i] = 0;
	}

      /*mettre le numero des faces dans la zone de recouvrement de 
	2 vf_model dans 2 listes */
      if (tour == 0)
	{
	  depinter = a2ri_get_time ();
	  for (int i = 0; i < A2RI_NB_THREAD - 1; i++)
	    {
	      argument[i].liste_centre2 = listcentrebase;
	      argument[i].liste_rayon2 = listrayonbase;
	      argument[i].nbtest2 = size_candidat_base;
	      argument[i].nbtest1deb = (size_candidat_m / A2RI_NB_THREAD) * i;
	      argument[i].nbtest1fin =
		(size_candidat_m / A2RI_NB_THREAD) * (i + 1) - 1;
	      argument[i].liste_centre1 = listcentremodelm;
	      argument[i].liste_rayon1 = listrayonmodelm;
	      argument[i].sensibility = sensibility;
	      argument[i].asupprimer = &liste_a_supprimer[i];
	      argument[i].nbelt = &size_liste_a_supprimer[i];
	    }
	  argument[A2RI_NB_THREAD - 1].liste_centre2 = listcentrebase;
	  argument[A2RI_NB_THREAD - 1].liste_rayon2 = listrayonbase;
	  argument[A2RI_NB_THREAD - 1].nbtest2 = size_candidat_base;
	  argument[A2RI_NB_THREAD - 1].nbtest1deb =
	    (size_candidat_m / A2RI_NB_THREAD) * (A2RI_NB_THREAD - 1);
	  argument[A2RI_NB_THREAD - 1].nbtest1fin = size_candidat_m - 1;
	  argument[A2RI_NB_THREAD - 1].liste_centre1 = listcentremodelm;
	  argument[A2RI_NB_THREAD - 1].liste_rayon1 = listrayonmodelm;
	  argument[A2RI_NB_THREAD - 1].sensibility = sensibility;
	  argument[A2RI_NB_THREAD - 1].asupprimer =
	    &liste_a_supprimer[A2RI_NB_THREAD - 1];
	  argument[A2RI_NB_THREAD - 1].nbelt =
	    &size_liste_a_supprimer[A2RI_NB_THREAD - 1];

	  for (int i = 0; i < A2RI_NB_THREAD; i++)
	    {
	      if (pthread_create
		  (th + i, NULL, IFthread_process_nettoyage, argument + i) < 0)
		exit (1);
	    }

	  for (int i = 0; i < A2RI_NB_THREAD; i++)
	    (void) pthread_join (th[i], &ret[i]);

	  for (int i = A2RI_NB_THREAD - 1; i >= 0; i--)
	    {
	      for (int j = size_liste_a_supprimer[i] - 1; j >= 0; j--)
		{
		  list_int_remove (&candidat_m, &size_candidat_m,
				   liste_a_supprimer[i][j]);
		  size_candidat_m++;
		  list_double_remove (&listrayonmodelm, &size_candidat_m,
				      liste_a_supprimer[i][j]);
		  size_candidat_m++;
		  list_point3d_remove (&listcentremodelm, &size_candidat_m,
				       liste_a_supprimer[i][j]);
		}
	      free (liste_a_supprimer[i]);
	      liste_a_supprimer[i] = NULL;
	      size_liste_a_supprimer[i] = 0;
	    }


	  for (int i = 0; i < A2RI_NB_THREAD - 1; i++)
	    {
	      argument[i].liste_centre2 = listcentremodelm;
	      argument[i].liste_rayon2 = listrayonmodelm;
	      argument[i].nbtest2 = size_candidat_m;
	      argument[i].nbtest1deb =
		(size_candidat_base / A2RI_NB_THREAD) * i;
	      argument[i].nbtest1fin =
		(size_candidat_base / A2RI_NB_THREAD) * (i + 1) - 1;
	      argument[i].liste_centre1 = listcentrebase;
	      argument[i].liste_rayon1 = listrayonbase;
	      argument[i].sensibility = sensibility;
	      argument[i].asupprimer = &liste_a_supprimer[i];
	      argument[i].nbelt = &size_liste_a_supprimer[i];
	    }
	  argument[A2RI_NB_THREAD - 1].liste_centre2 = listcentremodelm;
	  argument[A2RI_NB_THREAD - 1].liste_rayon2 = listrayonmodelm;
	  argument[A2RI_NB_THREAD - 1].nbtest2 = size_candidat_m;
	  argument[A2RI_NB_THREAD - 1].nbtest1deb =
	    (size_candidat_base / A2RI_NB_THREAD) * (A2RI_NB_THREAD - 1);
	  argument[A2RI_NB_THREAD - 1].nbtest1fin = size_candidat_base - 1;
	  argument[A2RI_NB_THREAD - 1].liste_centre1 = listcentrebase;
	  argument[A2RI_NB_THREAD - 1].liste_rayon1 = listrayonbase;
	  argument[A2RI_NB_THREAD - 1].sensibility = sensibility;
	  argument[A2RI_NB_THREAD - 1].asupprimer =
	    &liste_a_supprimer[A2RI_NB_THREAD - 1];
	  argument[A2RI_NB_THREAD - 1].nbelt =
	    &size_liste_a_supprimer[A2RI_NB_THREAD - 1];

	  for (int i = 0; i < A2RI_NB_THREAD; i++)
	    {
	      if (pthread_create
		  (th + i, NULL, IFthread_process_nettoyage, argument + i) < 0)
		exit (1);
	    }

	  for (int i = 0; i < A2RI_NB_THREAD; i++)
	    (void) pthread_join (th[i], &ret[i]);

	  for (int i = A2RI_NB_THREAD - 1; i >= 0; i--)
	    {
	      for (int j = size_liste_a_supprimer[i] - 1; j >= 0; j--)
		{
		  list_int_remove (&candidat_base, &size_candidat_base,
				   liste_a_supprimer[i][j]);
		  size_candidat_base++;
		  list_double_remove (&listrayonbase, &size_candidat_base,
				      liste_a_supprimer[i][j]);
		  size_candidat_base++;
		  list_point3d_remove (&listcentrebase, &size_candidat_base,
				       liste_a_supprimer[i][j]);
		}
	      free (liste_a_supprimer[i]);
	      liste_a_supprimer[i] = NULL;
	      size_liste_a_supprimer[i] = 0;
	    }
	  fininter = a2ri_get_time ();
	  totalinter += a2ri_time_interval_to_double (depinter, fininter);
	}
      else
	{
	  if (tour % 2 == 0)
	    {
	      depinter = a2ri_get_time ();
	      for (int i = 0; i < A2RI_NB_THREAD - 1; i++)
		{
		  argument[i].liste_centre2 = listcentrebase;
		  argument[i].liste_rayon2 = listrayonbase;
		  argument[i].nbtest2 = size_candidat_base;
		  argument[i].nbtest1deb =
		    (size_candidat_m / A2RI_NB_THREAD) * i;
		  argument[i].nbtest1fin =
		    (size_candidat_m / A2RI_NB_THREAD) * (i + 1) - 1;
		  argument[i].liste_centre1 = listcentremodelm;
		  argument[i].liste_rayon1 = listrayonmodelm;
		  argument[i].sensibility = sensibility;
		  argument[i].asupprimer = &liste_a_supprimer[i];
		  argument[i].nbelt = &size_liste_a_supprimer[i];
		}
	      argument[A2RI_NB_THREAD - 1].liste_centre2 = listcentrebase;
	      argument[A2RI_NB_THREAD - 1].liste_rayon2 = listrayonbase;
	      argument[A2RI_NB_THREAD - 1].nbtest2 = size_candidat_base;
	      argument[A2RI_NB_THREAD - 1].nbtest1deb =
		(size_candidat_m / A2RI_NB_THREAD) * (A2RI_NB_THREAD - 1);
	      argument[A2RI_NB_THREAD - 1].nbtest1fin = size_candidat_m - 1;
	      argument[A2RI_NB_THREAD - 1].liste_centre1 = listcentremodelm;
	      argument[A2RI_NB_THREAD - 1].liste_rayon1 = listrayonmodelm;
	      argument[A2RI_NB_THREAD - 1].sensibility = sensibility;
	      argument[A2RI_NB_THREAD - 1].asupprimer =
		&liste_a_supprimer[A2RI_NB_THREAD - 1];
	      argument[A2RI_NB_THREAD - 1].nbelt =
		&size_liste_a_supprimer[A2RI_NB_THREAD - 1];

	      for (int i = 0; i < A2RI_NB_THREAD; i++)
		{
		  if (pthread_create
		      (th + i, NULL, IFthread_process_nettoyage,
		       argument + i) < 0)
		    exit (1);
		}

	      for (int i = 0; i < A2RI_NB_THREAD; i++)
		(void) pthread_join (th[i], &ret[i]);

	      for (int i = A2RI_NB_THREAD - 1; i >= 0; i--)
		{
		  for (int j = size_liste_a_supprimer[i] - 1; j >= 0; j--)
		    {
		      list_int_remove (&candidat_m, &size_candidat_m,
				       liste_a_supprimer[i][j]);
		      size_candidat_m++;
		      list_double_remove (&listrayonmodelm, &size_candidat_m,
					  liste_a_supprimer[i][j]);
		      size_candidat_m++;
		      list_point3d_remove (&listcentremodelm,
					   &size_candidat_m,
					   liste_a_supprimer[i][j]);
		    }
		  free (liste_a_supprimer[i]);
		  liste_a_supprimer[i] = NULL;
		  size_liste_a_supprimer[i] = 0;
		}
	      fininter = a2ri_get_time ();
	      totalinter += a2ri_time_interval_to_double (depinter, fininter);
	    }
	  if (tour % 2 == 1)
	    {
	      depinter = a2ri_get_time ();
	      for (int i = 0; i < A2RI_NB_THREAD - 1; i++)
		{
		  argument[i].liste_centre2 = listcentremodelm;
		  argument[i].liste_rayon2 = listrayonmodelm;
		  argument[i].nbtest2 = size_candidat_m;
		  argument[i].nbtest1deb =
		    (size_candidat_base / A2RI_NB_THREAD) * i;
		  argument[i].nbtest1fin =
		    (size_candidat_base / A2RI_NB_THREAD) * (i + 1) - 1;
		  argument[i].liste_centre1 = listcentrebase;
		  argument[i].liste_rayon1 = listrayonbase;
		  argument[i].sensibility = sensibility;
		  argument[i].asupprimer = &liste_a_supprimer[i];
		  argument[i].nbelt = &size_liste_a_supprimer[i];
		}
	      argument[A2RI_NB_THREAD - 1].liste_centre2 = listcentremodelm;
	      argument[A2RI_NB_THREAD - 1].liste_rayon2 = listrayonmodelm;
	      argument[A2RI_NB_THREAD - 1].nbtest2 = size_candidat_m;
	      argument[A2RI_NB_THREAD - 1].nbtest1deb =
		(size_candidat_base / A2RI_NB_THREAD) * (A2RI_NB_THREAD - 1);
	      argument[A2RI_NB_THREAD - 1].nbtest1fin =
		size_candidat_base - 1;
	      argument[A2RI_NB_THREAD - 1].liste_centre1 = listcentrebase;
	      argument[A2RI_NB_THREAD - 1].liste_rayon1 = listrayonbase;
	      argument[A2RI_NB_THREAD - 1].sensibility = sensibility;
	      argument[A2RI_NB_THREAD - 1].asupprimer =
		&liste_a_supprimer[A2RI_NB_THREAD - 1];
	      argument[A2RI_NB_THREAD - 1].nbelt =
		&size_liste_a_supprimer[A2RI_NB_THREAD - 1];

	      for (int i = 0; i < A2RI_NB_THREAD; i++)
		{
		  if (pthread_create
		      (th + i, NULL, IFthread_process_nettoyage,
		       argument + i) < 0)
		    exit (1);
		}

	      for (int i = 0; i < A2RI_NB_THREAD; i++)
		(void) pthread_join (th[i], &ret[i]);

	      for (int i = A2RI_NB_THREAD - 1; i >= 0; i--)
		{
		  for (int j = size_liste_a_supprimer[i] - 1; j >= 0; j--)
		    {
		      list_int_remove (&candidat_base, &size_candidat_base,
				       liste_a_supprimer[i][j]);
		      size_candidat_base++;
		      list_double_remove (&listrayonbase, &size_candidat_base,
					  liste_a_supprimer[i][j]);
		      size_candidat_base++;
		      list_point3d_remove (&listcentrebase,
					   &size_candidat_base,
					   liste_a_supprimer[i][j]);
		    }
		  free (liste_a_supprimer[i]);
		  liste_a_supprimer[i] = NULL;
		  size_liste_a_supprimer[i] = 0;
		}
	      fininter = a2ri_get_time ();
	      totalinter += a2ri_time_interval_to_double (depinter, fininter);
	    }
	}

      if ((size_candidat_m == 0) || (size_candidat_base == 0))
	recouvrement = 0;
      else
	{
	  if (tour % 2 == 0)
	    {
	      //supprimer les faces se trouvant sur les bords du maillage
	      //pour m
	      depMAJ = a2ri_get_time ();
	      table = a2ri_vf_construction_edge_table (m, NULL, 0);
	      for (int i = 0; i < size_candidat_m; i++)
		{
		  vf_edge *edgetemp1 =
		    hashtable_look_for (table, m->fa[candidat_m[i]].ve1,
					m->fa[candidat_m[i]].ve2);
		  vf_edge *edgetemp2 =
		    hashtable_look_for (table, m->fa[candidat_m[i]].ve1,
					m->fa[candidat_m[i]].ve3);
		  vf_edge *edgetemp3 =
		    hashtable_look_for (table, m->fa[candidat_m[i]].ve2,
					m->fa[candidat_m[i]].ve3);
		  if (edgetemp1->nbsharedfaces == 1
		      || edgetemp2->nbsharedfaces == 1
		      || edgetemp3->nbsharedfaces == 1)
		    list_int_add (&liste_a_supprimer[0],
				  &size_liste_a_supprimer[0], candidat_m[i],
				  WITH_REDUNDANCE);
		}

	      a2ri_vf_remove_list_of_face (m, liste_a_supprimer[0],
					   size_liste_a_supprimer[0]);
	      for (int i = 0; i < size_liste_a_supprimer[0]; i++)
		{
		  int index;
		  list_int_remove (&candidat_m, &size_candidat_m, index =
				   list_int_contains (candidat_m,
						      size_candidat_m,
						      liste_a_supprimer[0]
						      [i]));
		  for (int j = index; j < size_candidat_m; j++)
		    candidat_m[j] = candidat_m[j] - 1;
		  size_candidat_m++;
		  list_double_remove (&listrayonmodelm, &size_candidat_m,
				      index);
		  size_candidat_m++;
		  list_point3d_remove (&listcentremodelm, &size_candidat_m,
				       index);
		}
	      if (size_liste_a_supprimer[0] == 0)
		recouvrement = 0;
	      else
		recouvrement = 1;

	      free (liste_a_supprimer[0]);
	      liste_a_supprimer[0] = NULL;
	      size_liste_a_supprimer[0] = 0;

	      hashtable_free (table);
	      table = NULL;
	      finMAJ = a2ri_get_time ();
	      totalMAJ += a2ri_time_interval_to_double (depMAJ, finMAJ);
	    }

	  if (tour % 2 == 1)
	    {
	      //pour base
	      depMAJ = a2ri_get_time ();
	      table = a2ri_vf_construction_edge_table (base, NULL, 0);
	      for (int i = 0; i < size_candidat_base; i++)
		{
		  vf_edge *edgetemp1 =
		    hashtable_look_for (table, base->fa[candidat_base[i]].ve1,
					base->fa[candidat_base[i]].ve2);
		  vf_edge *edgetemp2 =
		    hashtable_look_for (table, base->fa[candidat_base[i]].ve1,
					base->fa[candidat_base[i]].ve3);
		  vf_edge *edgetemp3 =
		    hashtable_look_for (table, base->fa[candidat_base[i]].ve2,
					base->fa[candidat_base[i]].ve3);
		  if (edgetemp1->nbsharedfaces == 1
		      || edgetemp2->nbsharedfaces == 1
		      || edgetemp3->nbsharedfaces == 1)
		    list_int_add (&liste_a_supprimer[0],
				  &size_liste_a_supprimer[0],
				  candidat_base[i], WITH_REDUNDANCE);
		}

	      a2ri_vf_remove_list_of_face (base, liste_a_supprimer[0],
					   size_liste_a_supprimer[0]);

	      for (int i = 0; i < size_liste_a_supprimer[0]; i++)
		{
		  int index;
		  list_int_remove (&candidat_base, &size_candidat_base,
				   index =
				   list_int_contains (candidat_base,
						      size_candidat_base,
						      liste_a_supprimer[0]
						      [i]));
		  for (int j = index; j < size_candidat_base; j++)
		    candidat_base[j] = candidat_base[j] - 1;
		  size_candidat_base++;
		  list_double_remove (&listrayonbase, &size_candidat_base,
				      index);
		  size_candidat_base++;
		  list_point3d_remove (&listcentrebase, &size_candidat_base,
				       index);

		}

	      if (size_liste_a_supprimer[0] == 0 && !recouvrement)
		recouvrement = 0;
	      else
		recouvrement = 1;

	      free (liste_a_supprimer[0]);
	      liste_a_supprimer[0] = NULL;
	      size_liste_a_supprimer[0] = 0;

	      hashtable_free (table);
	      table = NULL;
	      finMAJ = a2ri_get_time ();
	      totalMAJ += a2ri_time_interval_to_double (depMAJ, finMAJ);
	    }
	}
    }
  tour++;

  fintotal = a2ri_get_time ();

  a2ri_display_interval_time ("temps TOTAL nettoyage : ", deptotal, fintotal);
  printf ("\ttemps calcul BB : %lf -> %lf %c\n",
	  a2ri_time_interval_to_double (depBB, finBB),
	  a2ri_time_interval_to_double (depBB,
					finBB) /
	  a2ri_time_interval_to_double (deptotal, fintotal) * 100, 37);
  printf ("\ttemps calcul intersection : %lf -> %lf %c\n", totalinter,
	  totalinter / a2ri_time_interval_to_double (deptotal,
						     fintotal) * 100, 37);
  printf ("\ttemps MAJ : %lf -> %lf %c\n", totalMAJ,
	  totalMAJ / a2ri_time_interval_to_double (deptotal, fintotal) * 100,
	  37);

}

/**
   Création d'un vf_model avec une triangulation de delaunay
   @param list liste de point
   @param nbpoint nombre de points
   @return aucun
*/
vf_model *
a2ri_vf_delaunay_triangulation (
				const point3d * const list,
				int nbpoint)
{
  /*TODO*/
  vf_model *m = (vf_model *) malloc (sizeof (vf_model));
  a2ri_erreur_critique_si (m == NULL,
			"erreur allocation memoire pour m\na2ri_vf_delaunay_triangulation");

  a2ri_vf_init (m);

  return m;
}


/**
   Création d'un vef_model avec une triangulation de delaunay
   @param list liste de point
   @param nbpoint nombre de points
   @return aucun
*/
vef_model *
a2ri_vef_delaunay_triangulation (
				 const point3d * const list,
				 int nbpoint)
{
  /*TODO*/
  vef_model *m = (vef_model *) malloc (sizeof (vef_model));
  a2ri_erreur_critique_si (m == NULL,
			"erreur allocation memoire pour m\na2ri_vef_delaunay_triangulation");

  a2ri_vef_init (m);
  
  return m;
}



/*********************/
/*algorithme BPA 2010*/
/*********************/

void a2ri_bpa_insert_edge(bpa_edge *front, bpa_edge e)
{
  bpa_edge *newedge=(bpa_edge*)malloc(sizeof(bpa_edge));
  a2ri_erreur_critique_si (newedge == NULL,
			   "erreur allocation memoire pour newedge\na2ri_bpa_insert_edge");
  newedge->sigma_i=e.sigma_i;
  newedge->sigma_j=e.sigma_j;
  newedge->sigma_o=e.sigma_o;
  newedge->cijo=e.cijo;
  newedge->state=e.state;
  newedge->prev=front->prev;
  newedge->next=front;
  newedge->front=front->prev->front;
  front->prev->next=newedge;
  front->prev=newedge;
  front->front->front=newedge;
}

void a2ri_bpa_delete_edge(bpa_edge **front)
{
  a2ri_erreur_critique_si ((*front)->prev == NULL,
			   "Erreur algorithmique. Arete impossible a supprimer car non boucle 1.\na2ri_bpa_delete_edge");
  a2ri_erreur_critique_si ((*front)->next == NULL,
			   "Erreur algorithmique. Arete impossible a supprimer car non boucle 2.\na2ri_bpa_delete_edge");
  (*front)->front->front=(*front)->prev;
  (*front)->prev->next=(*front)->next;
  (*front)->next->prev=(*front)->prev;
  free((*front));
}

void a2ri_bpa_free_front(bpa_edge **front)
{
  bpa_edge *courant=*front,*suivant=NULL;
  if((*front)==NULL)
    return;

  if(courant->prev!=NULL)
    courant->prev->next=NULL;

  suivant=courant->next;

  while(courant!=NULL)
    {
      a2ri_erreur_critique_si (courant->state == ACTIVE,
			       "Erreur algorithmique. Arete active impossible a liberer.\na2ri_bpa_free_front");
      free(courant);
      courant=suivant;
      if(suivant!=NULL)
	suivant=suivant->next;
    }
  
}

void a2ri_bpa_free_fronts(bpa_fronts **fronts)
{
  bpa_fronts *courant=*fronts,*suivant=NULL;
  if((*fronts)==NULL)
    return;

  suivant=courant->next;

  while(courant!=NULL)
    {
      a2ri_bpa_free_front(&(courant->front));
      courant->front=NULL;
      free(courant);
      courant=suivant;
      if(suivant!=NULL)
	suivant=suivant->next;
    }
}

void a2ri_bpa_new_front(bpa_fronts **fronts, int sigma_i, int sigma_j, int sigma_k, point3d centre)
{
  bpa_edge *parc;
  (*fronts)=(bpa_fronts*)malloc(sizeof(bpa_fronts));
  a2ri_erreur_critique_si (*fronts == NULL,
			   "erreur allocation memoire pour fronts\na2ri_bpa_new_front");
  (*fronts)->next=NULL;
  (*fronts)->front=(bpa_edge*)malloc(sizeof(bpa_edge));
  a2ri_erreur_critique_si ((*fronts)->front == NULL,
			   "erreur allocation memoire pour *fronts->front - 1ere arete\na2ri_bpa_insert_edge");
  parc=(*fronts)->front;

  /*premiere arete du front*/
  parc->sigma_i=sigma_i;parc->sigma_j=sigma_j;parc->sigma_o=sigma_k;parc->cijo=centre;parc->state=ACTIVE;parc->front=(*fronts);
  parc->next=(bpa_edge*)malloc(sizeof(bpa_edge));
  a2ri_erreur_critique_si (parc->next == NULL,
			   "erreur allocation memoire pour parc->next - 2eme arete\na2ri_bpa_new_front");
  parc->next->prev=parc;
  parc=parc->next;
  /*seconde arete du front*/
  parc->sigma_i=sigma_j;parc->sigma_j=sigma_k;parc->sigma_o=sigma_i;parc->cijo=centre;parc->state=ACTIVE;parc->front=(*fronts);
  parc->next=(bpa_edge*)malloc(sizeof(bpa_edge));
  a2ri_erreur_critique_si (parc->next == NULL,
			   "erreur allocation memoire pour parc->next - 3eme arete\na2ri_bpa_new_front");
  parc->next->prev=parc;
  parc=parc->next;
  /*troisieme arete du front*/
  parc->sigma_i=sigma_k;parc->sigma_j=sigma_i;parc->sigma_o=sigma_j;parc->cijo=centre;parc->state=ACTIVE;parc->front=(*fronts);
  parc->next=(*fronts)->front;
  parc->next->prev=parc;
}

bpa_edge* a2ri_bpa_get_active_edge_in_front(bpa_edge *front)
{
  //printf("recherche d'arete active dans un front\n");
  int tour=0;
  while(tour<2)
    {
      if(front==front->front->front)
	tour++;
      if(front->state==ACTIVE)
	return front;
      else
	front=front->next;
    }
  return NULL;
}

bpa_edge* a2ri_bpa_get_active_edge_in_fronts(bpa_fronts *fronts)
{
  bpa_edge *temp;
  //printf("recherche d'arete active dans les fronts\n");
  while(fronts!=NULL)
    if((temp=a2ri_bpa_get_active_edge_in_front(fronts->front))!=NULL)
      return temp;
    else
      fronts=fronts->next;
  return NULL;
}

char a2ri_bpa_front_contains_point_in_front(bpa_edge *front, int sigma_k)
{
  int tour=0;
  while(tour<2)
    {
      if(front==front->front->front)
	tour++;
      if(front->sigma_i==sigma_k || front->sigma_j==sigma_k)
	return 1;
      else
	front=front->next;
    }
  return 0;
}

char a2ri_bpa_front_contains_point_in_fronts(bpa_fronts *fronts, int sigma_k)
{
  while(fronts!=NULL)
    if(a2ri_bpa_front_contains_point_in_front(fronts->front,sigma_k))
      return 1;
    else
      fronts=fronts->next;
  return 0;
}

bpa_edge* a2ri_bpa_front_contains_edge_in_front(bpa_edge *front, bpa_edge e)
{
  int tour=0;
  while(tour<2)
    {
      if(front==front->front->front)
	tour++;
      if(front->sigma_i==e.sigma_i && front->sigma_j==e.sigma_j)
	return front;
      else
	front=front->next;

    }
  return NULL;
}

bpa_edge* a2ri_bpa_front_contains_edge_in_fronts(bpa_fronts *fronts, bpa_edge e)
{
  bpa_edge *temp;
  while(fronts!=NULL)
    {
      temp=a2ri_bpa_front_contains_edge_in_front(fronts->front,e);
      if(temp!=NULL)
	return temp;
      else
	fronts=fronts->next;
    }
  return NULL;
}

void a2ri_bpa_display_front(bpa_edge *front)
{
  int tour=0;
  while(tour==0 || front!=front->front->front)
    {
      tour=1;
      //printf("\tarete : (%d %d) - point oppose : %d - centre : (%lf %lf %lf) - etat : ",front->sigma_i,front->sigma_j,front->sigma_o,front->cijo.x,front->cijo.y,front->cijo.z);
      printf("\tarete : (%4d %4d) - point oppose : %4d - ",front->sigma_i,front->sigma_j,front->sigma_o);
      if(front->state==ACTIVE)
	printf("ACTIVE\n");
      if(front->state==BOUNDARY)
	printf("BOUNDARY\n");
      //printf("\tfront : %p - front->front : %p - front->front->front : %p\n",front,front->front,front->front->front);
      front=front->next;
    }
}

void a2ri_bpa_display_fronts(bpa_fronts *fronts)
{
  int index=0;
  
  while(fronts!=NULL)
    {
      printf("front %d :\n",index++);
      a2ri_bpa_display_front(fronts->front);
      fronts=fronts->next;
    }
}

int a2ri_bpa_front_distance(bpa_edge *front, int index)
{
  if(!a2ri_bpa_front_contains_point_in_front(front, index))
    return -1;
  int tour=0,taille=-1,distance=0,trouve=0;
  while(tour<2)
    {
      taille++;
      if(front==front->front->front)
	  tour++;
      if(front->sigma_i==index || front->sigma_j==index)
	trouve=1;
      if(!trouve)
	distance++;
      front=front->next;
    }
  if((distance*1.0)>=(taille/2.0))
    distance=taille-distance-1;
  return distance;
}

void a2ri_bpa_join(bpa_edge **e, int sigma_k, point3d centre)
{
  bpa_edge temp;
  
  temp.prev=NULL;
  temp.next=NULL;

  temp.sigma_i=(*e)->sigma_i;
  temp.sigma_j=sigma_k;
  temp.sigma_o=(*e)->sigma_j;
  temp.cijo=centre;
  temp.state=ACTIVE;
  a2ri_bpa_insert_edge(*e,temp);
  temp.sigma_i=sigma_k;
  temp.sigma_j=(*e)->sigma_j;
  temp.sigma_o=(*e)->sigma_i;
  a2ri_bpa_insert_edge(*e,temp);
  a2ri_bpa_delete_edge(e);
}

void a2ri_bpa_glue(bpa_fronts **fronts, bpa_edge *ed1, bpa_edge *ed2)
{
  bpa_edge *temp;
  bpa_fronts *parc=*fronts,*todelete;
  //printf("GLUE - arete %d %d - arete %d %d\n",ed1->sigma_i,ed1->sigma_j,ed2->sigma_i,ed2->sigma_j);
  //a2ri_bpa_display_fronts(*fronts);
  if(ed1->next==ed2 || ed1->prev==ed2)
    {
      /*SONT ADJACENTS*/
      if(ed1->next==ed2 && ed1->prev==ed2)
	{
	  /*FORMENT UNE BOUCLE A EUX DEUX*/
	  /*SURPPRIMER LE FRONT*/
	  //printf("deux aretes adjacentes qui forment une boucle ");
	  if(ed1->front==*fronts)
	    {
	      //printf("cas 1\n");
	      /*L'ARETE APPARTIENT AU PREMIER FRONT*/
	      *fronts=(*fronts)->next;
	      parc->front->state=BOUNDARY;
	      parc->front->next->state=BOUNDARY;
	      a2ri_bpa_free_front(&(parc->front));
	      parc->front=NULL;
	      free(parc);
	    }
	  else
	    {
	      /*ON RECHERCHE LE FRONT POUR LE SUPPRIMER*/
	      //printf("cas 2\n");
	      while(parc->next!=ed1->front)
		parc=parc->next;
	      todelete=parc->next;
	      parc->next=todelete->next;
	      todelete->front->state=BOUNDARY;
	      todelete->front->next->state=BOUNDARY;
	      a2ri_bpa_free_front(&(todelete->front));
	      todelete->front=NULL;
	      free(todelete);
	    }
	}
      else
	{
	  //printf("deux aretes adjacentes\n");
	  /*NE FORMENT PAS UNE BOUCLE A EUX DEUX*/
	  a2ri_bpa_delete_edge(&ed1);
	  a2ri_bpa_delete_edge(&ed2);
	}
    }
  else
    {
      /*NE SONT PAS ADJACENTS*/
      if(ed1->front==ed2->front)
	{
	  //printf("deux arete non adjacentes appartenant au meme front\n");
	  //a2ri_bpa_display_fronts(*fronts);
	  /*APPARTIENNENT AU MEME FRONT*/
	  /*SPLIT DU FRONT EN DEUX*/
	  ed1->front->front=ed1->prev;

	  ed2->next->prev=ed1->prev;
	  ed1->prev->next=ed2->next;
	  ed1->next->prev=ed2->prev;
	  ed2->prev->next=ed1->next;

	  while(parc->next!=NULL)
	    parc=parc->next;
	  parc->next=(bpa_fronts*)malloc(sizeof(bpa_fronts));
	  parc=parc->next;
	  parc->next=NULL;
	  parc->front=ed2->prev;
	  temp=parc->front->next;
	  parc->front->front=parc;
	  while(temp!=parc->front)
	    {
	      temp->front=parc;
	      temp=temp->next;
	    }
	  //printf("\n\n");
	  //a2ri_bpa_display_fronts(*fronts);
	}
      else
	{
	  //a2ri_bpa_display_fronts(*fronts);
	  //printf("deux aretes non adjacentes qui n'appartiennent pas au meme front\n");
	  /*N'APPARTIENNENT PAS AU MEME FRONT*/
	  /*FUSION DES DEUX FRONTS*/
	  if(ed1->front!=*fronts)
	    todelete=ed1->front;
	  else
	    todelete=ed2->front;
	  ed1->front->front=ed1->prev;
	  ed2->front->front=ed2->prev;
	  while(parc->next!=todelete)
	    parc=parc->next;
	  parc->next=todelete->next;
	  
	  ed1->prev->next=ed2->next;
	  ed2->next->prev=ed1->prev;
	  ed1->next->prev=ed2->prev;
	  ed2->prev->next=ed1->next;

	  if(todelete==ed1->front)
	    {
	      temp=ed2->front->front;
	      temp->front=ed2->front;
	      temp=temp->next;
	      while(temp!=ed2->front->front)
		{
		  temp->front=ed2->front;
		  temp=temp->next;
		}
	    }
	  else
	  {
	    temp=ed1->front->front;
	    temp->front=ed1->front;
	    temp=temp->next;
	    while(temp!=ed1->front->front)
	      {
		temp->front=ed1->front;
		temp=temp->next;
	      }
	  }
	  
	  free(todelete);
	}
      free(ed1);
      free(ed2);
    }
  return;
}

void a2ri_bpa_regularization(bpa_fronts **fronts, int sigma_i, int sigma_j, int sigma_k)
{
  bpa_edge *ed1,*ed2,temp;
  /*IK KI*/
  temp.sigma_i=sigma_i;
  temp.sigma_j=sigma_k;
  //printf("arete recherche : %d %d\n",temp.sigma_i,temp.sigma_j);
  //a2ri_bpa_display_fronts(*fronts);
  ed1=a2ri_bpa_front_contains_edge_in_fronts(*fronts,temp);
  a2ri_erreur_critique_si (ed1 == NULL,
			   "Erreur algorithmique. L'arete vient d''etre creee. IK KI ed1 ne peut etre NULL.\na2ri_bpa_regularization");
  temp.sigma_i=sigma_k;
  temp.sigma_j=sigma_i;
  ed2=a2ri_bpa_front_contains_edge_in_fronts(*fronts,temp);
  if(ed2!=NULL)
    a2ri_bpa_glue(fronts,ed1,ed2);

  
  /*KJ JK*/
  temp.sigma_i=sigma_k;
  temp.sigma_j=sigma_j;
  ed1=a2ri_bpa_front_contains_edge_in_fronts(*fronts,temp);
  a2ri_erreur_critique_si (ed1 == NULL,
			   "Erreur algorithmique. L'arete vient d''etre cree. KJ JK ed1 ne peut etre NULL.\na2ri_bpa_regularization");
  temp.sigma_i=sigma_j;
  temp.sigma_j=sigma_k;
  ed2=a2ri_bpa_front_contains_edge_in_fronts(*fronts,temp);
  if(ed2!=NULL)
    a2ri_bpa_glue(fronts,ed1,ed2);
}

char a2ri_bpa_compute_ball(point3d p1, point3d p2, point3d p3, vector3d normaleface, double radius, point3d *centre)
{
  double longueur;
  
  *centre=circumcircle_center(&p1,&p2,&p3);
  longueur=point3d_length(centre,&p1);
  if(longueur<radius)
    {
      longueur=sqrt(radius*radius-longueur*longueur);
      normaleface.dx*=longueur;
      normaleface.dy*=longueur;
      normaleface.dz*=longueur;
      centre->x=centre->x+normaleface.dx;
      centre->y=centre->y+normaleface.dy;
      centre->z=centre->z+normaleface.dz;
      return 1;
    }
  else
    return 0;
}

char a2ri_bpa_verif_contraintes(pt_point3d *listpoint, int sizelistpoint, point3d p1, point3d p2, point3d p3, vector3d normalp1, vector3d normalp2, vector3d normalp3, double radius, point3d *centre)
{
  vector3d AB,AC,normaleface;
  double prodscal1,prodscal2,prodscal3,distance;
  int index,trouve;

  /*printf("----DEBUT A2RI BPA VERIF CONTAINTES\n");
  printf("points :\n");
  point3d_display(p1);point3d_display(p2);point3d_display(p3);
  printf("normales aux trois points :\n");
  vector3d_display(normalp1);
  vector3d_display(normalp2);
  vector3d_display(normalp3);*/
  vector3d_init (&AB, p2.x - p1.x,p2.y - p1.y,p2.z - p1.z);
  vector3d_init (&AC, p3.x - p1.x,p3.y - p1.y,p3.z - p1.z);
  normaleface=vector3d_vectorialproduct (&AB, &AC);
  vector3d_normalize(&normaleface);
  /*printf("normale au triangle candidat :\n");
    vector3d_display(normaleface);*/
  prodscal1=vector3d_scalarproduct(&normaleface,&normalp1);
  prodscal2=vector3d_scalarproduct(&normaleface,&normalp2);
  prodscal3=vector3d_scalarproduct(&normaleface,&normalp3);
  if(prodscal1>0 && prodscal2>0 && prodscal3>0)
    {
      /*printf("test des normales OK\n");*/
      /*VERIFIER QUE LA BOULE PASSANT PAR LES TROIS POINTS NE CONTIENNENT PAS D'AUTRE POINT DE LA LISTE*/
      /*CALCUL DU CENTRE DE LA BOULE*/
      /*SI ON ARRIVE A TROUVER UNE BOULE TOUCHANT LES TROIS POINTS*/
      if(a2ri_bpa_compute_ball(p1,p2,p3,normaleface,radius,centre))
	{
	  /*printf("calcul de la boule OK\ncentre :\n");*/
	  /*point3d_display(*centre);*/
	  /*VERIFICATION DE NON PRESENCE D'AUTRE POINT DANS LA BOULE*/
	  index=0;
	  trouve=0;
	  /*printf("recherche de points se trouvant dans cette sphere\n");*/
	  while(!trouve && index<sizelistpoint)
	    {
	      /*printf("point index : %d\n",index);
	      point3d_display(*(listpoint[index]));*/
	      if(!point3d_equal(&p1,listpoint[index]) && !point3d_equal(&p2,listpoint[index]) && !point3d_equal(&p3,listpoint[index]))
		{
		  /*printf("longueur au centre de la sphere : %lf\n",point3d_length(*centre,*(listpoint[index])));*/
		  if((distance=point3d_length(centre,listpoint[index]))<radius && !egalite(distance-radius,0))
		    {
		      /*printf("le point se trouve dans la sphere\n");*/
		      trouve=1;
		    }
		  /*else
		  {
		  if(egalite(distance-radius,0))
		  printf("le point ne se trouve pas dans la sphere mais presque, distance : %lf\n",distance);
		  else
		  printf("le point ne se trouve pas dans la sphere\n");
		  }*/
		}
	      /*else
		printf("le point est egal a l'un des points du triangle\n");*/
	      index++;
	    }
	  if(!trouve)
	    {
	      /*printf("pas de point trouve dans la sphere\n");*/
	      return 1;
	    }
	}
      /*else
        printf("impossible de calculer une boule passant par ces trois points et de rayon %lf\n",radius);*/
      }
  /*else
    printf("probleme dans le test des normales\n----FIN A2RI BPA VERIF CONTAINTES\n");*/
  return 0;
}

char a2ri_bpa_find_seed_triangle(vf_model *m, space_partition *sp, vector3d *normal, double radius, int *listused, int sizeused, int *sigma_i, int *sigma_j, int *sigma_k, point3d *centre, int imin)
{
  point3d p1,p2,p3;
  pt_sp_depth *listcellule=NULL;
  int sizelistcellule=0;
  pt_point3d *listpoint=NULL;
  int sizelistpoint=0;
  int index,index2;
  
  //printf("----DEBUT A2RI BPA FIND SEED TRIANGLE\n");
  for(int i=imin;i<m->nbvertex;i++)
    {
      if(list_int_contains(listused,sizeused,i)==-1)
	{
	  *sigma_i=i;
	  point3d_init(&p1,m->ve[i].x,m->ve[i].y,m->ve[i].z);
	  free(listcellule);
	  listcellule=NULL;
	  sizelistcellule=0;
	  sizelistpoint=0;
	  space_partition_get_neighbour(sp,&p1,1,&listcellule,&sizelistcellule);
	  for(int j=0;j<sizelistcellule;j++)
	    sizelistpoint+=listcellule[j]->nb_point;
	  free(listpoint);
	  listpoint=NULL;
	  listpoint=(pt_point3d*)malloc(sizelistpoint*sizeof(pt_point3d));
	  index=0;
	  /*CREATION DE LA LISTE DE POINT SE TROUVANT DANS LES CASES ADJACENTES AU POINT D'INDEX I*/
	  for(int j=0;j<sizelistcellule;j++)
	    for(int k=0;k<listcellule[j]->nb_point;k++)
	      listpoint[index++]=&(listcellule[j]->list_point[k]);
	  
	  /*PRISE DES PAIRES DE POINTS DISCTINCTES*/
	  //printf("nombre de point : %d\n",sizelistpoint);
	  for(int j=0;j<sizelistpoint;j++)
	    for(int k=j+1;k<sizelistpoint;k++)
	      {
		/*printf("%5d - a2ri_bpa_find_seed_triangle - test du triangle : %d %d %d\n",m->nbface,i,listpoint[j]->att_int,listpoint[k]->att_int);*/
		index=list_int_contains(listused,sizeused,listpoint[j]->att_int);
		index2=list_int_contains(listused,sizeused,listpoint[k]->att_int);
		if(i!=listpoint[j]->att_int && i!=listpoint[k]->att_int && listpoint[j]->att_int!=listpoint[k]->att_int && index==-1 && index2==-1)
		  {
		    /*VERIFIER SI LES TROIS POINTS VERIFIENT LES CONTRAINTES BPA*/
		    /*VERIFIER L'ORIENTATION DE LA NORMALE*/
		    point3d_init(&p2,
				 m->ve[listpoint[j]->att_int].x,
				 m->ve[listpoint[j]->att_int].y,
				 m->ve[listpoint[j]->att_int].z);
		    point3d_init(&p3,
				 m->ve[listpoint[k]->att_int].x,
				 m->ve[listpoint[k]->att_int].y,
				 m->ve[listpoint[k]->att_int].z);
		    if(a2ri_bpa_verif_contraintes(listpoint,
						  sizelistpoint,
						  p1,p2,p3,
						  normal[i],
						  normal[listpoint[j]->att_int],
						  normal[listpoint[k]->att_int],
						  radius,centre))
		      {
			//printf("triangle OK\n");
			*sigma_i=i;
			*sigma_j=listpoint[j]->att_int;
			*sigma_k=listpoint[k]->att_int;
			//printf("----FIN A2RI BPA FIND SEED TRIANGLE\n");
			return 1;
		      }
		    /*else
		     printf("le triangle ne respecte pas les contraintes bpa\n");*/
		  }
		/*else
		  printf("triangle degenere ou un point est deja utilise\n");*/
	      }
	}
    }
  free(listpoint);
  free(listcellule);
  *sigma_i=-1;
  *sigma_j=-1;
  *sigma_k=-1;
  //printf("pas de triangle trouve\n----FIN A2RI BPA FIND SEED TRIANGLE\n");
  return 0;
}

void a2ri_bpa_ball_pivot(vf_model *m, space_partition *sp, vector3d *normal, double radius, bpa_edge *e, int *listused, int sizeused, bpa_fronts *fronts, int *sigma_k, point3d *centre)
{
  point3d milieu,p1,p2,p3;
  pt_sp_depth *listcellule=NULL;
  int sizelistcellule=0;
  pt_point3d *listpoint=NULL;
  int sizelistpoint=0;
  int index=0,index2=0;
  pt_point3d *listcentre=NULL;
  double prodscal,max;
  vector3d AB,AC;

  //printf("----DEBUT A2RI BPA BALL PIVOT\n");
  //printf("pivot sur l'arete %d %d - point oppose : %d\n",e->sigma_i,e->sigma_j,e->sigma_o);
  //printf("centre : ");point3d_display(e->cijo);

  point3d_init(&p1,m->ve[e->sigma_i].x,m->ve[e->sigma_i].y,m->ve[e->sigma_i].z);
  point3d_init(&p3,m->ve[e->sigma_j].x,m->ve[e->sigma_j].y,m->ve[e->sigma_j].z);

  point3d_init(&milieu,(p1.x+p3.x)/2.0,(p1.y+p3.y)/2.0,(p1.z+p3.z)/2.0);

  listcellule=NULL;
  sizelistcellule=0;
  space_partition_get_neighbour(sp,&milieu,1,&listcellule,&sizelistcellule);
  for(int j=0;j<sizelistcellule;j++)
    sizelistpoint+=listcellule[j]->nb_point;
  listpoint=(pt_point3d*)malloc(sizelistpoint*sizeof(pt_point3d));
  a2ri_erreur_critique_si(listpoint==NULL,
			  "erreur allocation memoire pour listpoint\na2ri_bpa_ball_pivot");
  listcentre=(pt_point3d*)malloc(sizelistpoint*sizeof(pt_point3d));
  a2ri_erreur_critique_si(listcentre==NULL,
			  "erreur allocation memoire pour listcentre\na2ri_bpa_ball_pivot");
  index=0;
  /*CREATION DE LA LISTE DE POINT SE TROUVANT DANS LES CASES ADJACENTES AU POINT D'INDEX I*/
  for(int j=0;j<sizelistcellule;j++)
    for(int k=0;k<listcellule[j]->nb_point;k++)
      listpoint[index++]=&(listcellule[j]->list_point[k]);

  /*CONSTRUCTION DES BOULES CANDIDATES*/
  //int nbcandidat=0;
  for(int i=0;i<sizelistpoint;i++)
    {
      if(listpoint[i]->att_int!=e->sigma_i && listpoint[i]->att_int!=e->sigma_j && listpoint[i]->att_int!=e->sigma_o)
	{
	  //printf("test triangle %d %d %d\n",e->sigma_i,listpoint[i]->att_int,e->sigma_j);
	  point3d_init(&p2,
		       m->ve[listpoint[i]->att_int].x,
		       m->ve[listpoint[i]->att_int].y,
		       m->ve[listpoint[i]->att_int].z);
	  if(a2ri_bpa_verif_contraintes(listpoint,
					sizelistpoint,
					p1,p2,p3,
					normal[e->sigma_i],
					normal[listpoint[i]->att_int],
					normal[e->sigma_j],
					radius,centre))
	    {
	      listcentre[i]=(point3d*)malloc(sizeof(point3d));
	      point3d_init(listcentre[i],centre->x,centre->y,centre->z);
	      //nbcandidat++;
	      //printf("centre OK, %d candidat\n",nbcandidat);
	    }
	  else
	    {
	      //printf("centre pas ok\n");
	      listcentre[i]=NULL;
	    }
	}
      else
	listcentre[i]=NULL;
    }

  //printf("%d candidats sur %d\n",nbcandidat,sizelistpoint);

  /*REGARDER LES BOULES ET CHOISIR LA BONNE*/
  vector3d_init(&AB,e->cijo.x-milieu.x,e->cijo.y-milieu.y,e->cijo.z-milieu.z);
  vector3d_normalize(&AB);
  index=-1;
  max=-1;
  for(int i=0;i<sizelistpoint;i++)
    if(listcentre[i]!=NULL)
      {
	vector3d_init(&AC,listcentre[i]->x-milieu.x,listcentre[i]->y-milieu.y,listcentre[i]->z-milieu.z);
	vector3d_normalize(&AC);
	prodscal=vector3d_scalarproduct(&AB,&AC);
	/*DANS LE CAS OU NOUS N'AVONS PAS ENCORE DE CANDIDAT*/
	if(index==-1)
	  {
	    index=listpoint[i]->att_int;
	    max=prodscal;
	    index2=i;
	  }
	else
	  /*DANS LE CAS OU LA BOULE TESTEE EST MEILLEURE QUE LA PRECEDENTE RETENUE*/
	  if(prodscal>max)
	    {
	      index2=i;
	      index=listpoint[i]->att_int;
	      max=prodscal;
	    }
	  else
	    {
	      /*DANS LE CAS OU LA BOULE TESTEE EST AUSSI BONNE QUE LA PRECEDENTE RETENUE*/
	      if(prodscal==max)
		{
		  //printf("ATTENTION, NOUS AVONS UNE EGALITE entre les points :\n%d de distance %d\n%d de distance %d\n",index,a2ri_bpa_front_distance(e,index),listpoint[i]->att_int,a2ri_bpa_front_distance(e,listpoint[i]->att_int));
		  if(a2ri_bpa_front_distance(e,index)>a2ri_bpa_front_distance(e,listpoint[i]->att_int))
		    index=listpoint[i]->att_int;
		}
	    }
      }
  
  if(index!=-1)
    {
      *centre=*(listcentre[index2]);
      point3d_init(&p2,m->ve[index].x,m->ve[index].y,m->ve[index].z);
      /*printf("longueurs : %lf %lf %lf\n",
	     point3d_length(p1,*centre),
	     point3d_length(p2,*centre),
	     point3d_length(p3,*centre));*/
      a2ri_erreur_critique_si(!egalite(point3d_length(&p1,centre),radius),
			      "la boule n'est pas bien definie\na2ri_bpa_ball_pivot\n");
      a2ri_erreur_critique_si(!egalite(point3d_length(&p2,centre),radius),
			      "la boule n'est pas bien definie\na2ri_bpa_ball_pivot\n");
      a2ri_erreur_critique_si(!egalite(point3d_length(&p3,centre),radius),
			      "la boule n'est pas bien definie\na2ri_bpa_ball_pivot\n");
    }
  
  free(listpoint);
  free(listcellule);
  free(listcentre);
  *sigma_k=index;
  if(list_int_contains(listused,sizeused,*sigma_k)!=-1 && !a2ri_bpa_front_contains_point_in_fronts(fronts,*sigma_k))
    *sigma_k=-1;
  
  //printf("-- FIN A2RI BPA BALL PIVOT\n");
}

void a2ri_vf_bpa(vf_model *m, vector3d *normal, double radius)
{
  bpa_edge *e;
  int sigma_k,sigma_i,sigma_j,*listused=NULL,sizeused=0;
  bpa_fronts *fronts=NULL;
  point3d centre;
  int imin=0;

  point3d pmin,pmax;
  int nbpartX,nbpartY,nbpartZ;
  space_partition *sp=(space_partition*)malloc(sizeof(space_partition));
  
  /*INITIALISATION DE L'ALGORITHME*/
  /*CREATION DE LA SPACE PARTITION POUR OPTIMISATION DES RECHERCHES*/
  a2ri_erreur_critique_si(sp==NULL,
			  "Erreur allocation memoire pour sp\na2ri_bpa_find_seed_triangle\n");

  point3d_init(&pmin,m->xmin,m->ymin,m->zmin);
  point3d_init(&pmax,m->xmax,m->ymax,m->zmax);
  nbpartX=(int)((m->xmax-m->xmin)/(2*radius));
  if(nbpartX==0)
    nbpartX++;
  nbpartY=(int)((m->ymax-m->ymin)/(2*radius));
  if(nbpartY==0)
    nbpartY++;
  nbpartZ=(int)((m->zmax-m->zmin)/(2*radius));
  if(nbpartZ==0)
    nbpartZ++;
  space_partition_new(sp,&pmin,&pmax,nbpartX,nbpartY,nbpartZ);
  a2ri_vf_space_partition(m,sp);

  /*REVOIR L'ALGO*/
  while(1)
    {
      while((e=a2ri_bpa_get_active_edge_in_fronts(fronts))!=NULL)
	{
	  a2ri_bpa_ball_pivot(m,sp,normal,radius,e,listused,sizeused,fronts,&sigma_k,&centre);
	  if(sigma_k!=-1)
	    {
	      list_int_add(&listused,&sizeused,sigma_k,WITHOUT_REDUNDANCE);
#ifdef _DEBUG
	      printf("%c%c%c%c%c%c%c%c%c%c%c%c%lf%c",8,8,8,8,8,8,8,8,8,8,8,8,sizeused*100.0/(m->nbvertex*1.0),37);
#endif
	      //printf("Ajout triangle %d : %d %d %d par pivot\n",m->nbface,e->sigma_i,sigma_k,e->sigma_j);
	      a2ri_vf_add_face(m,e->sigma_i,sigma_k,e->sigma_j);
	      sigma_i=e->sigma_i;sigma_j=e->sigma_j;
	      //a2ri_bpa_display_fronts(fronts);
	      a2ri_bpa_join(&e,sigma_k,centre);
	      //a2ri_bpa_display_fronts(fronts);
	      a2ri_bpa_regularization(&fronts,sigma_i,sigma_j,sigma_k);
	      //a2ri_bpa_display_fronts(fronts);
	    }
	  else
	    {
	      //printf("arete frontiere\n");
	      e->state=BOUNDARY;
	    }
	  //a2ri_bpa_display_fronts(fronts);
	}
      
      if(a2ri_bpa_find_seed_triangle(m,sp,normal,radius,listused,sizeused,&sigma_i,&sigma_j,&sigma_k,&centre,imin))
	{
#ifdef _DEBUG
	  printf("%c%c%c%c%c%c%c%c%c%c%c%c%lf%c",8,8,8,8,8,8,8,8,8,8,8,8,sizeused*100.0/(m->nbvertex*1.0),37);
#endif
	  //printf("Ajout triangle %d : %d %d %d par seed triangle\n",m->nbface,sigma_i,sigma_j,sigma_k);
	  imin=sigma_i;
	  a2ri_vf_add_face(m,sigma_i,sigma_j,sigma_k);
	  a2ri_bpa_free_fronts(&fronts);
	  fronts=NULL;
	  a2ri_bpa_new_front(&fronts,sigma_i,sigma_j,sigma_k,centre);
	  //a2ri_bpa_display_fronts(fronts);
	  list_int_add(&listused,&sizeused,sigma_i,WITH_REDUNDANCE);
	  list_int_add(&listused,&sizeused,sigma_j,WITH_REDUNDANCE);
	  list_int_add(&listused,&sizeused,sigma_k,WITH_REDUNDANCE);
	}
      else
	{
	  printf("\nBPA termine...\n");
	  return;
	}
    }
  return;
}

/*
void a2ri_vf_bpa_multipass(vf_model *m, vector3d *normal, double *listradius, int listsize)
{
  return;
  }*/

void a2ri_vf_bpa_without_normal(vf_model *m, double radius)
{
  vector3d *list=NULL;

  list=(vector3d*)malloc(m->nbvertex*sizeof(vector3d));
  a2ri_erreur_critique_si(list==NULL,
			  "erreur allocation memoire pour list\na2ri_vf_bpa_without_normal\n");
  
  for(int i=0;i<m->nbvertex;i++)
    {
      vector3d_init(&(list[i]),m->ve[i].x-((m->xmin+m->xmax)/2.0),m->ve[i].y-((m->ymin+m->ymax)/2.0),m->ve[i].z-((m->zmin+m->zmax)/2.0));
      vector3d_normalize(&(list[i]));
    }  
  a2ri_vf_bpa(m,list,radius);
  free(list);
}

void a2ri_bpa_average_radius_suggestion(vf_model *m, double *min, double *max, double *average, double *sd)
{
  /*TODO optimisation multi thread*/
  space_partition sp;
  point3d ptmin,ptmax;
  double *listelongueur;

  listelongueur=(double*)malloc(sizeof(double)*m->nbvertex);
  a2ri_erreur_critique_si(listelongueur==NULL,"erreur allocation memoire a2ri_bpa_average_radius_suggestion pour listelongueur\n");
  
  ptmin.x=m->xmin;ptmin.y=m->ymin;ptmin.z=m->zmin;
  ptmax.x=m->xmax;ptmax.y=m->ymax;ptmax.z=m->zmax;
  
  if(m->nbvertex>=24)
    
    space_partition_new(&sp,&ptmin,&ptmax,
			(int)(pow((m->nbvertex/24.0),1.0/3.0)),
			(int)(pow((m->nbvertex/24.0),1.0/3.0)),
			((int)pow((m->nbvertex/24.0),1.0/3.0)));
  else
    space_partition_new(&sp,&ptmin,&ptmax,1,1,1);
   
  a2ri_vf_space_partition(m,&sp);

   for(int i=0;i<m->nbvertex;i++)
     {
       point3d p1,p2;
       double longueur;
       point3d_init(&p1,m->ve[i].x,m->ve[i].y,m->ve[i].z);
       space_partition_nearest_point(&sp,&p1,&p2,&longueur,REJECT_ZERO_LENGTH);
       listelongueur[i]=longueur; 
     }

   *min=list_double_min(listelongueur,m->nbvertex);
   *max=list_double_max(listelongueur,m->nbvertex);
   *average=list_double_average(listelongueur,m->nbvertex);
   *sd=sqrt(list_double_variance(listelongueur,m->nbvertex));

   space_partition_free(&sp);
}

void a2ri_bpa_initialisation(vf_model *m, bpa_fronts **fronts, int **listused, int *sizelistused)
{
  IT_bpa_liste temp;
  int nbtrou=IF_bpa_vf_hole(m,&temp);
  //printf("\n\n%d trous\n",nbtrou);

  //list_int_display(temp.list,temp.nbelt);

  IT_bpa_liste *cycle;
  cycle=(IT_bpa_liste*)malloc(nbtrou*sizeof(IT_bpa_liste));
  
  int index=0;
  for(int i=0;i<nbtrou;i++)
    {
      cycle[i].list=NULL;
      cycle[i].nbelt=0;
      while(temp.list[index]!=-1)
	{
	  list_int_add(&(cycle[i].list),&(cycle[i].nbelt),temp.list[index],WITH_REDUNDANCE);
	  index++;
	}
      index++;
      /*printf("cycle %d\n",i);
	list_int_display(cycle[i].list,cycle[i].nbelt);*/
    }

  //cycles trouvés. On va essayer de les rassembler si il y a un point en commun

  if(nbtrou>1)
    {
      int i=0,j,commun=-1;
      while(i<nbtrou)
	{
	  if(commun!=-1)
	    {
	      i=0;
	      commun=-1;
	    }
	  j=i+1;
	  while(j<nbtrou && commun==-1)
	    {
	      int indexi=0,indexj;
	      while(indexi<cycle[i].nbelt && commun==-1)
		{
		  if((indexj=list_int_contains(cycle[j].list,cycle[j].nbelt,cycle[i].list[indexi]))!=-1)
		    commun=cycle[i].list[indexi];
		  else
		    indexi++;
		}
	      if(commun!=-1)
		{
		  //fusion des deux listes et mise à 0 de i et 1 de j
		  /*printf("attention, conflit entre les cycles %d et %d sur l'element %d\n",i,j,commun);*/
		  while(cycle[i].list[cycle[i].nbelt-1]!=commun)
		    list_int_shift_right(cycle[i].list,cycle[i].nbelt,1);
		  while(cycle[j].list[0]!=commun)
		    list_int_shift_left(cycle[j].list,cycle[j].nbelt,1);
		  for(int k=1;k<cycle[j].nbelt;k++)
		    list_int_add(&(cycle[i].list),&(cycle[i].nbelt),cycle[j].list[k],WITH_REDUNDANCE);
		  list_int_add(&(cycle[i].list),&(cycle[i].nbelt),commun,WITH_REDUNDANCE);
		  free(cycle[j].list);
		  for(int k=j+1;k<nbtrou;k++)
		    {
		      cycle[k-1].list=cycle[k].list;
		      cycle[k-1].nbelt=cycle[k].nbelt;
		    }
		  nbtrou--;
		}
	      else
		j++;
	    }
	  i++;
	}
    }
  
  printf("\n\n%d trous\n",nbtrou);
  for(int i=0;i<nbtrou;i++)
    {
      printf("cycle %d\n",i);
      list_int_display(cycle[i].list,cycle[i].nbelt);
    }

  for(int i=0;i<nbtrou;i++)
    free(cycle[i].list);
  free(cycle);

  /*CREER LES FRONTS EN FONCTION DES CYCLES*/
}
