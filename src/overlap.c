/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 01/03/07       */
/* Date de modification : 15/03/15   */
/* Version : 0.4                     */
/*************************************/

/*************************************/
/* Auteur : Romain Leguay            */
/*          Nguyen Haiduong          */
/*          Solange Houeto           */
/*          Marianne Fichoux         */
/* Date de modification : 06/06/09   */
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

#include "overlap.h"

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
  int **retour,
    *nbelt;
  vf_model *m;
} IToverlap_thread_argument;

void *
IFthread_process_overlap (
			void *argu)
{
  IToverlap_thread_argument *arg = (IToverlap_thread_argument *) argu;

  *(arg->retour) = NULL;
  *(arg->nbelt) = 0;
  for (int i = arg->nbtest1deb; i <= arg->nbtest1fin; i++)
    {
      //on vérifie l'intersection de la bounding ball de la face du modèle m
      //avec l'ensemble des bounding ball du modèle base
      if (intersection_bounding_ball_with_bounding_ball_list
	  (&(arg->liste_centre1[i]), arg->liste_rayon1[i], arg->liste_centre2,
	   arg->liste_rayon2, arg->nbtest2, arg->sensibility))
	{
	  //s'il y a internsetion , on ajoute les somemts de cette face à une liste en évitant les redondances
	  list_int_add (arg->retour, arg->nbelt, arg->m->fa[i].ve1,
			WITHOUT_REDUNDANCE);
	  list_int_add (arg->retour, arg->nbelt, arg->m->fa[i].ve2,
			WITHOUT_REDUNDANCE);
	  list_int_add (arg->retour, arg->nbelt, arg->m->fa[i].ve3,
			WITHOUT_REDUNDANCE);
	}
    }
  pthread_exit (0);
}

/********** MAIN FUNCTIONS **********/

/**
   Test de l'intersection d'une AABB avec une liste d'AABB
   @param min coin inférieur de la AABB
   @param max coin supérieur de la AABB
   @param listmin liste des coins inférieurs des AABB
   @param listmax liste des coins supérieurs des AABB
   @param size taille de la liste des AABB
   @return 1 si intersection, 0 sinon
**/
int
intersection_axis_aligned_bounding_box_with_axis_aligned_bounding_box_list (
									    const point3d * const min,
										const point3d * const max,
										const point3d * const listmin,
										const point3d * const listmax,
										int size)
{
  for (int i = 0; i < size; i++)
    {
      if ((min->x < listmax[i].x) && (max->x > listmin[i].x) &&
	  (min->y < listmax[i].y) && (max->y > listmin[i].y) &&
	  (min->z < listmax[i].z) && (max->z > listmin[i].z))
	return 1;
    }
  return 0;
}

/**
   Test de l'instesection d'une bounding ball avec une liste de bounding ball
   @param cx coordonnée x du centre de la bounding ball
   @param cy coordonnée y du centre de la bounding ball
   @param cz coordonnée z du centre de la bounding ball
   @param rayon rayon de la bounding ball
   @param listx liste des coordonnées x des centres des bounding ball
   @param listy liste des coordonnées y des centres des bounding ball
   @param listz liste des coordonnées z des centres des bounding ball
   @param rayon liste des rayons des bounding ball
   @param nbelt taille de la liste
   @param alpha
**/
int
intersection_bounding_ball_with_bounding_ball_list (
						    const point3d * const c,
						    double rayon,
						    const point3d * const listcentre,
						    const double * const listrayon,
						    int nbelt,
						    double alpha)
{
  for (int i = 0; i < nbelt; i++)
    if (point3d_length (c, &(listcentre[i])) <= (rayon + listrayon[i]) * alpha)
      return 1;

  return 0;
}

/**
   Calcule le taux de recouvrement en utilisant les bounding ball de ritter
   @param base modèle de base
   @param m modèle dont on veux trouver le taux de recouvrement avec le modèle de base
   @return le taux de recouvrement trouvé
**/
double
a2ri_vf_bounding_ball_ritter_compute_overlap (
					      const vf_model * const base,
					      const vf_model * const m,
					      double sensibility)
{
  double *listrayonmodelm = NULL,
    *listrayonbase = NULL;
  int *facesmodelm = NULL,
    *facesbase = NULL,
    nbfacesmodelm,
    nbfacesbase,
    nbintersection = 0,
    *numvertex = NULL,
    nbelt = 0;
  point3d *listcentrebase = NULL,
    *listcentremodelm = NULL;


  facesmodelm = (int *) malloc (m->nbface * sizeof (int));
  a2ri_erreur_critique_si (facesmodelm == NULL,
			"erreur allocation memoire pour facesmodelm\na2ri_vf_bounding_ball_ritter_compute_overlap");
  facesbase = (int *) malloc (base->nbface * sizeof (int));
  a2ri_erreur_critique_si (facesbase == NULL,
			"erreur allocation memoire pour facesbase\na2ri_vf_bounding_ball_ritter_compute_overlap");

  //on va calculer le taux de recouvrement à partir de toutes les faces du modèle m
  for (int i = 0; i < m->nbface; i++)
    facesmodelm[i] = i;
  nbfacesmodelm = m->nbface;

  //on va calculer le taux de recouvrement à partir de toutes les faces du modèle base
  for (int i = 0; i < base->nbface; i++)
    facesbase[i] = i;
  nbfacesbase = base->nbface;

  //on demande les bounding ball de ritter pour toutes les faces
  a2ri_vf_bounding_ball_ritter (base, facesbase, nbfacesbase,
				&listcentrebase, &listrayonbase);
  a2ri_vf_bounding_ball_ritter (m, facesmodelm, nbfacesmodelm,
				&listcentremodelm, &listrayonmodelm);

  //pour toutes les bounding ball des faces du modèle m
  for (int i = 0; i < nbfacesmodelm; i++)
    {
      //on vérifie l'intersection de la bounding ball de la face du modèle m
      //avec l'ensemble des bounding ball du modèle base
      if (intersection_bounding_ball_with_bounding_ball_list
	  (&(listcentremodelm[i]), listrayonmodelm[i], listcentrebase,
	   listrayonbase, nbfacesbase, sensibility))
	{
	  //s'il y a internsetion , on ajoute les somemts de cette face à une liste en évitant les redondances
	  list_int_add (&numvertex, &nbelt, m->fa[i].ve1, WITHOUT_REDUNDANCE);
	  list_int_add (&numvertex, &nbelt, m->fa[i].ve2, WITHOUT_REDUNDANCE);
	  list_int_add (&numvertex, &nbelt, m->fa[i].ve3, WITHOUT_REDUNDANCE);
	  nbintersection++;
	}
    }

  free (listrayonmodelm);
  free (listrayonbase);
  free (facesmodelm);
  free (facesbase);
  free (numvertex);
  free (listcentrebase);
  free (listcentremodelm);

  //le taux de recouvrement est la taille de la liste de sommet divisé par le nombre de sommet du modèle
  return (nbelt * 1.0) / ((m->nbvertex) * 1.0);

}

/**
   Calcule le taux de recouvrement en utilisant les bounding ball minimale
   @param base modèle de base
   @param m modèle dont on veux trouver le taux de recouvrement avec le modèle de base
   @return le taux de recouvrement trouvé
**/
//Remarque : les maillages devraient etre qualifiés de const mais impossible a cause de l'utilisation de pthread
double
a2ri_vf_bounding_ball_minimale_compute_overlap (
						vf_model * base,
						vf_model * m,
						double sensibility)
{
  double *listrayonmodelm = NULL,
    *listrayonbase = NULL;
  int *facesmodelm = NULL,
    *facesbase = NULL,
    nbfacesmodelm,
    nbfacesbase,
    *numvertex = NULL,
    nbelt = 0;
  point3d *listcentrebase = NULL,
    *listcentremodelm = NULL;

  pthread_t th[A2RI_NB_THREAD];
  IToverlap_thread_argument argument[A2RI_NB_THREAD];
  void *ret[A2RI_NB_THREAD];
  int *liste_retour[A2RI_NB_THREAD],
    size_liste_retour[A2RI_NB_THREAD];

  facesmodelm = (int *) malloc (m->nbface * sizeof (int));
  a2ri_erreur_critique_si (facesmodelm == NULL,
			"erreur allocation memoire pour facesmodelm\na2ri_vf_bounding_ball_ritter_compute_overlap");
  facesbase = (int *) malloc (base->nbface * sizeof (int));
  a2ri_erreur_critique_si (facesbase == NULL,
			"erreur allocation memoire pour facesbase\na2ri_vf_bounding_ball_ritter_compute_overlap");

  //on va calculer le taux de recouvrement à partir de toutes les faces du modèle m
  for (int i = 0; i < m->nbface; i++)
    facesmodelm[i] = i;
  nbfacesmodelm = m->nbface;

  //on va calculer le taux de recouvrement à partir de toutes les faces du modèle base
  for (int i = 0; i < base->nbface; i++)
    facesbase[i] = i;
  nbfacesbase = base->nbface;

  //on demande les bounding ball minimales pour toutes les faces
  a2ri_vf_bounding_ball_minimale (base, facesbase, nbfacesbase,
				  &listcentrebase, &listrayonbase);
  a2ri_vf_bounding_ball_minimale (m, facesmodelm, nbfacesmodelm,
				  &listcentremodelm, &listrayonmodelm);

  for (int i = 0; i < A2RI_NB_THREAD - 1; i++)
    {
      argument[i].liste_centre2 = listcentrebase;
      argument[i].liste_rayon2 = listrayonbase;
      argument[i].nbtest2 = nbfacesbase;
      argument[i].nbtest1deb = (nbfacesmodelm / A2RI_NB_THREAD) * i;
      argument[i].nbtest1fin = (nbfacesmodelm / A2RI_NB_THREAD) * (i + 1) - 1;
      argument[i].liste_centre1 = listcentremodelm;
      argument[i].liste_rayon1 = listrayonmodelm;
      argument[i].sensibility = sensibility;
      argument[i].retour = &liste_retour[i];
      argument[i].nbelt = &size_liste_retour[i];
      argument[i].m = m;
    }
  argument[A2RI_NB_THREAD - 1].liste_centre2 = listcentrebase;
  argument[A2RI_NB_THREAD - 1].liste_rayon2 = listrayonbase;
  argument[A2RI_NB_THREAD - 1].nbtest2 = nbfacesbase;
  argument[A2RI_NB_THREAD - 1].nbtest1deb =
    (nbfacesmodelm / A2RI_NB_THREAD) * (A2RI_NB_THREAD - 1);
  argument[A2RI_NB_THREAD - 1].nbtest1fin = nbfacesmodelm - 1;
  argument[A2RI_NB_THREAD - 1].liste_centre1 = listcentremodelm;
  argument[A2RI_NB_THREAD - 1].liste_rayon1 = listrayonmodelm;
  argument[A2RI_NB_THREAD - 1].sensibility = sensibility;
  argument[A2RI_NB_THREAD - 1].retour = &liste_retour[A2RI_NB_THREAD - 1];
  argument[A2RI_NB_THREAD - 1].nbelt = &size_liste_retour[A2RI_NB_THREAD - 1];
  argument[A2RI_NB_THREAD - 1].m = m;

  for (int i = 0; i < A2RI_NB_THREAD; i++)
    {
      if (pthread_create (th + i, NULL, IFthread_process_overlap, argument + i)
	  < 0)
	exit (1);
    }

  for (int i = 0; i < A2RI_NB_THREAD; i++)
    (void) pthread_join (th[i], &ret[i]);

  for (int i = 1; i < A2RI_NB_THREAD; i++)
    for (int j = 0; j < size_liste_retour[i]; j++)
      list_int_add (&liste_retour[0], &size_liste_retour[0],
		    liste_retour[i][j], WITHOUT_REDUNDANCE);

  nbelt = size_liste_retour[0];

  free (listrayonmodelm);
  free (listrayonbase);
  free (facesmodelm);
  free (facesbase);
  free (numvertex);
  free (listcentrebase);
  free (listcentremodelm);

  //le taux de recouvrement est la taille de la liste de sommet divisé par le nombre de sommet du modèle
  return (nbelt * 1.0) / ((m->nbvertex) * 1.0);

}

/**
   Calcule le taux de recouvrement en utilisant les axis aligned bounding box
   @param base modèle de base
   @param m modèle dont on veux trouver le taux de recouvrement avec le modèle de base
   @return le taux de recouvrement trouvé
**/
double
a2ri_vf_axis_aligned_bounding_box_compute_overlap (
						   const vf_model * const base,
						   const vf_model * const m)
{
  point3d *ptminbase = NULL,
    *ptmaxbase = NULL,
    *ptminmodelm = NULL,
    *ptmaxmodelm = NULL;
  int *facesmodelm,
    *facesbase,
    nbfacesmodelm,
    nbfacesbase,
    nbintersection = 0,
    *numvertex = NULL,
    nbelt = 0;

  facesmodelm = (int *) malloc (m->nbface * sizeof (int));
  a2ri_erreur_critique_si (facesmodelm == NULL,
			"erreur allocation memoire pour facesmodelm\na2ri_vf_axis_aligned_bounding_box_compute_overlap");
  facesbase = (int *) malloc (base->nbface * sizeof (int));
  a2ri_erreur_critique_si (facesbase == NULL,
			"erreur allocation memoire pour facesbase\na2ri_vf_axis_aligned_bounding_box_compute_overlap");

  //on va calculer le taux de recouvrement à partir de toutes les faces du modèle m
  for (int i = 0; i < m->nbface; i++)
    facesmodelm[i] = i;
  nbfacesmodelm = m->nbface;

  //on va calculer le taux de recouvrement à partir de toutes les faces du modèle base
  for (int i = 0; i < base->nbface; i++)
    facesbase[i] = i;
  nbfacesbase = base->nbface;

  //on demande les bounding box de toutes les faces
  a2ri_vf_axis_aligned_bounding_box (base, facesbase, nbfacesbase,
				     &ptminbase, &ptmaxbase);
  a2ri_vf_axis_aligned_bounding_box (m, facesmodelm, nbfacesmodelm,
				     &ptminmodelm, &ptmaxmodelm);

  //pour toutes les faces du modèle m
  for (int i = 0; i < nbfacesmodelm; i++)
    {
      //on vérifie si la bounding box de la face s'intersecte avec la liste des bounding box du modèle base
      if (intersection_axis_aligned_bounding_box_with_axis_aligned_bounding_box_list (&(ptminmodelm[i]), &(ptmaxmodelm[i]), ptminbase, ptmaxbase, nbfacesbase))
	{
	  //dans ce cas, on ajoute les trois sommets à une liste en évitant les redondances
	  list_int_add (&numvertex, &nbelt, m->fa[i].ve1, WITHOUT_REDUNDANCE);
	  list_int_add (&numvertex, &nbelt, m->fa[i].ve2, WITHOUT_REDUNDANCE);
	  list_int_add (&numvertex, &nbelt, m->fa[i].ve3, WITHOUT_REDUNDANCE);
	  nbintersection++;
	}
    }

  free (ptminbase);
  free (ptmaxbase);
  free (ptminmodelm);
  free (ptmaxmodelm);
  free (facesmodelm);
  free (facesbase);
  free (numvertex);

  //le taux de recouvrement est égale à la taille de la liste divisé par le nombre de sommets du modèle
  return (nbelt * 1.0) / ((m->nbvertex) * 1.0);

}

/**
   Calcule la liste des faces du modèle m s'intersectant avec le modèle base en utilisant les bounding ball de ritter.
   @param base modèle servant de base
   @param m modèle dont on cherche les faces s'intersectant avec le modèle de base
   @param list liste des faces s'intersectant
   @param size taille de la liste
   @param alpha variable alpha servant lors du test d'intesection
   @return aucun
**/
void
a2ri_vf_bounding_ball_ritter_find_face_overlapping (
						    const vf_model * const base,
						    const vf_model * const m,
						    int **list,
						    int *size,
						    double alpha)
{
  *list = NULL;
  *size = 0;

  double *listrayonmodelm = NULL,
    *listrayonbase = NULL;
  int *facesmodelm,
    *facesbase,
    nbfacesmodelm,
    nbfacesbase;
  point3d *listcentrebase = NULL,
    *listcentremodelm = NULL;

  //on va demander les bounding ball de toutes les faces des deux modèles
  facesmodelm = (int *) malloc (m->nbface * sizeof (int));
  a2ri_erreur_critique_si (facesmodelm == NULL,
			"erreur allocation memoire pour facesmodelm\na2ri_vf_bounding_ball_ritter_find_face_overlapping");
  facesbase = (int *) malloc (base->nbface * sizeof (int));
  a2ri_erreur_critique_si (facesbase == NULL,
			"erreur allocation memoire pour facesbase\na2ri_vf_bounding_ball_ritter_find_face_overlapping");

  for (int i = 0; i < m->nbface; i++)
    facesmodelm[i] = i;
  nbfacesmodelm = m->nbface;

  for (int i = 0; i < base->nbface; i++)
    facesbase[i] = i;
  nbfacesbase = base->nbface;

  a2ri_vf_bounding_ball_ritter (base, facesbase, nbfacesbase,
				&listcentrebase, &listrayonbase);
  a2ri_vf_bounding_ball_ritter (m, facesmodelm, nbfacesmodelm,
				&listcentremodelm, &listrayonmodelm);

  //pour toutes les faces du modèles m
  for (int i = 0; i < nbfacesmodelm; i++)
    {
      //on vérifie s'il y a intersection entre la bounding ball et l'ensemble des bounding ball des faces de l'autre modèle
      if (intersection_bounding_ball_with_bounding_ball_list
	  (&(listcentremodelm[i]), listrayonmodelm[i], listcentrebase,
	   listrayonbase, nbfacesbase, alpha))
	//dans ce cas, on ajoute la face à la liste
	list_int_add (list, size, i, WITH_REDUNDANCE);
    }

  free (listrayonmodelm);
  free (listrayonbase);
  free (facesmodelm);
  free (facesbase);
  free (listcentrebase);
  free (listcentremodelm);

}

/**
   Calcule la liste des faces du modèle m s'intersectant avec le modèle base en utilisant les bounding ball de ritter.
   @param base modèle servant de base
   @param m modèle dont on cherche les faces s'intersectant avec le modèle de base
   @param list liste des faces s'intersectant
   @param size taille de la liste
   @param alpha variable alpha servant lors du test d'intesection
   @return aucun
**/
void
a2ri_vf_bounding_ball_minimale_find_face_overlapping (
						      const vf_model * const base,
						      const vf_model * const m,
						      int **list,
						      int *size,
						      double alpha)
{
  *list = NULL;
  *size = 0;

  double *listrayonmodelm = NULL,
    *listrayonbase = NULL;
  int *facesmodelm,
    *facesbase,
    nbfacesmodelm,
    nbfacesbase;
  point3d *listcentrebase = NULL,
    *listcentremodelm = NULL;

  //on va demander les bounding ball de toutes les faces des deux modèles
  facesmodelm = (int *) malloc (m->nbface * sizeof (int));
  a2ri_erreur_critique_si (facesmodelm == NULL,
			"erreur allocation memoire pour facesmodelm\na2ri_vf_bounding_ball_ritter_find_face_overlapping");
  facesbase = (int *) malloc (base->nbface * sizeof (int));
  a2ri_erreur_critique_si (facesbase == NULL,
			"erreur allocation memoire pour facesbase\na2ri_vf_bounding_ball_ritter_find_face_overlapping");

  for (int i = 0; i < m->nbface; i++)
    facesmodelm[i] = i;
  nbfacesmodelm = m->nbface;

  for (int i = 0; i < base->nbface; i++)
    facesbase[i] = i;
  nbfacesbase = base->nbface;

  a2ri_vf_bounding_ball_minimale (base, facesbase, nbfacesbase,
				  &listcentrebase, &listrayonbase);
  a2ri_vf_bounding_ball_minimale (m, facesmodelm, nbfacesmodelm,
				  &listcentremodelm, &listrayonmodelm);

  //pour toutes les faces du modèles m
  for (int i = 0; i < nbfacesmodelm; i++)
    {
      //on vérifie s'il y a intersection entre la bounding ball et l'ensemble des bounding ball des faces de l'autre modèle
      if (intersection_bounding_ball_with_bounding_ball_list
	  (&(listcentremodelm[i]), listrayonmodelm[i], listcentrebase,
	   listrayonbase, nbfacesbase, alpha))
	//dans ce cas, on ajoute la face à la liste
	list_int_add (list, size, i, WITH_REDUNDANCE);
    }

  free (listrayonmodelm);
  free (listrayonbase);
  free (facesmodelm);
  free (facesbase);
  free (listcentrebase);
  free (listcentremodelm);

}

/**
   Calcule la liste des faces du modèle m s'intersectant avec le modèle base en utilisant les AABB
   @param base modèle servant de base
   @param m modèle dont on cherche les faces s'intersectant avec le modèle de base
   @param list liste des faces s'intersectant
   @param size taille de la liste
   @return aucun
**/
void
a2ri_vf_axis_aligned_bounding_box_find_face_overlapping (
							 const vf_model * const base,
							 const vf_model * const m,
							 int **listface,
							 int *size)
{
  *listface = NULL;
  *size = 0;

  int *facesmodelm,
    *facesbase,
    nbfacesmodelm,
    nbfacesbase;
  point3d *listptminbase = NULL,
    *listptminm = NULL,
    *listptmaxbase = NULL,
    *listptmaxm = NULL;

  //on va demander les bounding box de toutes les faces des deux modèles
  facesmodelm = (int *) malloc (m->nbface * sizeof (int));
  a2ri_erreur_critique_si (facesmodelm == NULL,
			"erreur allocation memoire pour facesmodelm\na2ri_vf_axis_aligned_bounding_box_find_face_overlapping");
  facesbase = (int *) malloc (base->nbface * sizeof (int));
  a2ri_erreur_critique_si (facesbase == NULL,
			"erreur allocation memoire pour facesbase\na2ri_vf_axis_aligned_bounding_box_find_face_overlapping");

  for (int i = 0; i < m->nbface; i++)
    facesmodelm[i] = i;
  nbfacesmodelm = m->nbface;

  for (int i = 0; i < base->nbface; i++)
    facesbase[i] = i;
  nbfacesbase = base->nbface;

  a2ri_vf_axis_aligned_bounding_box (base, facesbase, nbfacesbase,
				     &listptminbase, &listptmaxbase);
  a2ri_vf_axis_aligned_bounding_box (m, facesmodelm, nbfacesmodelm,
				     &listptminm, &listptmaxm);

  //pour toutes les faces du modèles m
  for (int i = 0; i < nbfacesmodelm; i++)
    {
      //on vérifie s'il y a intersection entre la bounding box et l'ensemble des bounding box des faces de l'autre modèle
      if (intersection_axis_aligned_bounding_box_with_axis_aligned_bounding_box_list (&(listptminm[i]), &(listptmaxm[i]), listptminbase, listptmaxbase, nbfacesbase))
	//dans ce cas, on ajoute la face à la liste
	list_int_add (listface, size, i, WITH_REDUNDANCE);
    }

  free (listptminbase);
  free (listptmaxbase);
  free (facesmodelm);
  free (facesbase);
  free (listptminm);
  free (listptmaxm);

}
