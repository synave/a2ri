/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 05/01/08       */
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

#include "graph.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

int IFlist_edge_etape_tri_rapide (
				double *listlong,
				int *listnumedge,
				int min,
				int max)
{
  double temp = listlong[max];
  int temp2 = listnumedge[max];
  
  while (max > min)
    {
      
      while (max > min && listlong[min] <= temp)
	min++;
      
      if (max > min)
	{
	  listlong[max] = listlong[min];
	  listnumedge[max] = listnumedge[min];
	  max--;
	  
	  while (max > min && listlong[max] >= temp)
	    max--;
	  
	  if (max > min)
	    {
	      listlong[min] = listlong[max];
	      listnumedge[min] = listnumedge[max];
	      min++;
	    }
	}
    }
  listlong[max] = temp;
  listnumedge[max] = temp2;
  return (max);
}

void IFlist_edge_tri_rapide (
			   double *listlong,
			   int *listnumedge,
			   int deb,
			   int fin)
{
  int mil;
  
  if (deb < fin)
    {
      mil = IFlist_edge_etape_tri_rapide (listlong, listnumedge, deb, fin);
      if (mil - deb > fin - mil)
	{
	  IFlist_edge_tri_rapide (listlong, listnumedge, mil + 1, fin);
	  IFlist_edge_tri_rapide (listlong, listnumedge, deb, mil - 1);
	}
      else
	{
	  IFlist_edge_tri_rapide (listlong, listnumedge, deb, mil - 1);
	  IFlist_edge_tri_rapide (listlong, listnumedge, mil + 1, fin);
	}
    }
}

void
IFlist_edge_sort (
		double *listlong,
		int *listnumedge,
		int size)
{
  IFlist_edge_tri_rapide (listlong, listnumedge, 0, size - 1);
}

/********** MAIN FUNCTIONS **********/

/**
   Calcul du graphe Nearest Neighbour Graph (NNG)
   @param m le modele
   @return un modele contenant les aretes
*/
vef_model *
a2ri_vef_nearest_neighbour_graph (
				  const vef_model * const m)
{
  point3d p1,
    p2,
    ptmin,
    ptmax;
  double distmin;
  space_partition sp;
  vef_model *retour = (vef_model *) malloc (sizeof (vef_model));
  a2ri_erreur_critique_si (retour == NULL,
			"erreur allocation memoire pour retour\na2ri_vef_nearest_neighbour_graph");

  a2ri_vef_init (retour);

  for (int i = 0; i < m->nbvertex; i++)
    a2ri_vef_add_vertex (retour, m->ve[i].x, m->ve[i].y, m->ve[i].z);

  point3d_init (&ptmin, m->xmin, m->ymin, m->zmin);
  point3d_init (&ptmax, m->xmax, m->ymax, m->zmax);
  space_partition_new (&sp, &ptmin, &ptmax, 8, 8, 8);

  a2ri_vef_space_partition (m, &sp);

  retour->xmin = m->xmin;
  retour->xmax = m->xmax;
  retour->ymin = m->ymin;
  retour->ymax = m->ymax;
  retour->zmin = m->zmin;
  retour->zmax = m->zmax;


  for (int i = 0; i < m->nbvertex; i++)
    {
      point3d_init (&p1, m->ve[i].x, m->ve[i].y, m->ve[i].z);
      space_partition_nearest_point (&sp, &p1, &p2, &distmin,
				     REJECT_ZERO_LENGTH);
      a2ri_vef_add_edge (retour, i, p2.att_int, 1);
    }

  space_partition_free (&sp);
  return retour;
}


/**
   Calcul du graphe Gabriel (GG)
   @param m le modele
   @return un modele contenant les aretes
*/
vef_model *
a2ri_vef_gabriel_graph (
			const vef_model * const m)
{
  point3d p,
    centre;
  double distance;
  int alinterieur,
    k;
  vef_model *retour = (vef_model *) malloc (sizeof (vef_model));
  a2ri_erreur_critique_si (retour == NULL,
			"erreur allocation memoire pour retour\na2ri_vef_gabriel_graph");

  a2ri_vef_init (retour);

  for (int i = 0; i < m->nbvertex; i++)
    a2ri_vef_add_vertex (retour, m->ve[i].x, m->ve[i].y, m->ve[i].z);

  retour->xmin = m->xmin;
  retour->xmax = m->xmax;
  retour->ymin = m->ymin;
  retour->ymax = m->ymax;
  retour->zmin = m->zmin;
  retour->zmax = m->zmax;

  for (int i = 0; i < m->nbvertex; i++)
    {
      for (int j = 0; j < m->nbvertex; j++)
	if (i != j)
	  {
	    point3d_init (&centre,
			  ((m->ve[i].x + m->ve[j].x) * 1.0) / 2.0,
			  ((m->ve[i].y + m->ve[j].y) * 1.0) / 2.0,
			  ((m->ve[i].z + m->ve[j].z) * 1.0) / 2.0);
	    point3d_init (&p, m->ve[i].x, m->ve[i].y, m->ve[i].z);
	    distance = point3d_length (&centre, &p);
	    alinterieur = 0;
	    k = 0;
	    while (k < m->nbvertex && alinterieur == 0)
	      {
		if (k != i && k != j)
		  {
		    point3d_init (&p, m->ve[k].x, m->ve[k].y, m->ve[k].z);
		    if (point3d_length (&p, &centre) < distance)
		      alinterieur++;
		  }
		k++;
	      }

	    if (!alinterieur)
	      a2ri_vef_add_edge (retour, i, j, 1);
	  }
    }
  return retour;
}


/**
   Calcul du graphe etendu Gabriel (NNG)
   @param m le modele
   @return un modele contenant les aretes
*/
vef_model *
a2ri_vef_extended_gabriel_hypergraph (
				      const vef_model * const m)
{
  vef_model *retour = a2ri_vef_gabriel_graph (m);
  int *listve = NULL,
    size = 0,
    ve1,
    ve2,
    ve3;
  point3d p[3],
    *centre = NULL;
  double *rayon = NULL;
  int trouve,
    k;
  vector3d AB,
    AC;

  /*PARTIE1 */
  for (int i = 0; i < retour->nbedge; i++)
    {
      for (int j = 0; j < retour->nbedge; j++)
	{
	  listve = NULL;
	  size = 0;
	  list_int_add (&listve, &size, retour->ed[i].ve1,
			WITHOUT_REDUNDANCE);
	  list_int_add (&listve, &size, retour->ed[i].ve2,
			WITHOUT_REDUNDANCE);
	  list_int_add (&listve, &size, retour->ed[j].ve1,
			WITHOUT_REDUNDANCE);
	  list_int_add (&listve, &size, retour->ed[j].ve2,
			WITHOUT_REDUNDANCE);
	  if (size == 3)
	    {
	      vector3d_init (&AB,
			     retour->ve[listve[0]].x -
			     retour->ve[listve[1]].x,
			     retour->ve[listve[0]].y -
			     retour->ve[listve[1]].y,
			     retour->ve[listve[0]].z -
			     retour->ve[listve[1]].z);
	      vector3d_init (&AC,
			     retour->ve[listve[0]].x -
			     retour->ve[listve[2]].x,
			     retour->ve[listve[0]].y -
			     retour->ve[listve[2]].y,
			     retour->ve[listve[0]].z -
			     retour->ve[listve[2]].z);
	      double coeff = AB.dx / AC.dx;
	      AC.dx *= coeff;
	      AC.dy *= coeff;
	      AC.dz *= coeff;
	      if (!vector3d_equal (&AB, &AC))
		{
		  ve1 = retour->ed[i].ve1;
		  ve2 = retour->ed[i].ve2;
		  if (!(ve2 == retour->ed[j].ve1 || ve2 == retour->ed[j].ve2))
		    {
		      ve3 = ve1;
		      ve1 = ve2;
		      ve2 = ve3;
		    }
		  if (ve2 == retour->ed[j].ve1)
		    ve3 = retour->ed[j].ve2;
		  else
		    ve3 = retour->ed[j].ve1;

		  if (a2ri_vef_search_edge (retour, ve1, ve3) == -1)
		    {
		      point3d_init (&(p[0]), retour->ve[ve1].x,
				    retour->ve[ve1].y, retour->ve[ve1].z);
		      point3d_init (&(p[1]), retour->ve[ve2].x,
				    retour->ve[ve2].y, retour->ve[ve2].z);
		      point3d_init (&(p[2]), retour->ve[ve3].x,
				    retour->ve[ve3].y, retour->ve[ve3].z);

		      centre = NULL;
		      rayon = NULL;
		      point3d_bounding_ball_minimale (p, 1, &centre, &rayon);
		      trouve = 0;
		      k = 0;
		      while (k < retour->nbvertex && !trouve)
			{
			  point3d_init (&(p[0]), retour->ve[k].x,
					retour->ve[k].y, retour->ve[k].z);
			  if (point3d_length (&(p[0]), &(centre[0])) < rayon[0])
			    trouve = 1;
			  k++;
			}
		      if (!trouve)
			a2ri_vef_add_edge (retour, ve1, ve3, 0);

		      free (centre);
		      free (rayon);
		    }
		}
	    }
	  free (listve);
	}
    }
  /*PARTIE2 */
  for (int i = 0; i < retour->nbedge; i++)
    {
      int numvertex_a = retour->ed[i].ve1;
      for (int j = 0; j < retour->ve[numvertex_a].nbsharededges; j++)
	{
	  int numedge_a = retour->ve[numvertex_a].sharededges[j];
	  int numvertex_b = retour->ed[numedge_a].ve1;
	  for (k = 0; k < retour->ve[numvertex_b].nbsharededges; k++)
	    {
	      int numedge_b = retour->ve[numvertex_b].sharededges[k];
	      if (i != numedge_a && i != numedge_b && numedge_a != numedge_b)
		{
		  listve = NULL;
		  size = 0;
		  list_int_add (&listve, &size, retour->ed[i].ve1,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[i].ve2,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_a].ve1,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_a].ve2,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_b].ve1,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_b].ve2,
				WITHOUT_REDUNDANCE);

		  if (size == 3)
		    {
		      if (a2ri_vef_search_face
			  (retour, i, numedge_a, numedge_b) == -1
			  && a2ri_vef_search_face (retour, i, numedge_b,
						   numedge_a) == -1)
			a2ri_vef_add_face (retour, i, numedge_a, numedge_b);
		    }

		  free (listve);
		}
	    }

	  numvertex_b = retour->ed[numedge_a].ve2;
	  for (k = 0; k < retour->ve[numvertex_b].nbsharededges; k++)
	    {
	      int numedge_b = retour->ve[numvertex_b].sharededges[k];
	      if (i != numedge_a && i != numedge_b && numedge_a != numedge_b)
		{
		  listve = NULL;
		  size = 0;
		  list_int_add (&listve, &size, retour->ed[i].ve1,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[i].ve2,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_a].ve1,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_a].ve2,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_b].ve1,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_b].ve2,
				WITHOUT_REDUNDANCE);

		  if (size == 3)
		    {
		      if (a2ri_vef_search_face
			  (retour, i, numedge_a, numedge_b) == -1
			  && a2ri_vef_search_face (retour, i, numedge_b,
						   numedge_a) == -1)
			a2ri_vef_add_face (retour, i, numedge_a, numedge_b);
		    }

		  free (listve);
		}
	    }
	}

      //IDEM POUR LE SECOND POINT

      numvertex_a = retour->ed[i].ve2;
      for (int j = 0; j < retour->ve[numvertex_a].nbsharededges; j++)
	{
	  int numedge_a = retour->ve[numvertex_a].sharededges[j];
	  int numvertex_b = retour->ed[numedge_a].ve1;
	  for (k = 0; k < retour->ve[numvertex_b].nbsharededges; k++)
	    {
	      int numedge_b = retour->ve[numvertex_b].sharededges[k];
	      if (i != numedge_a && i != numedge_b && numedge_a != numedge_b)
		{
		  listve = NULL;
		  size = 0;
		  list_int_add (&listve, &size, retour->ed[i].ve1,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[i].ve2,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_a].ve1,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_a].ve2,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_b].ve1,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_b].ve2,
				WITHOUT_REDUNDANCE);

		  if (size == 3)
		    {
		      if (a2ri_vef_search_face
			  (retour, i, numedge_a, numedge_b) == -1
			  && a2ri_vef_search_face (retour, i, numedge_b,
						   numedge_a) == -1)
			a2ri_vef_add_face (retour, i, numedge_a, numedge_b);
		    }

		  free (listve);
		}
	    }

	  numvertex_b = retour->ed[numedge_a].ve2;
	  for (k = 0; k < retour->ve[numvertex_b].nbsharededges; k++)
	    {
	      int numedge_b = retour->ve[numvertex_b].sharededges[k];
	      if (i != numedge_a && i != numedge_b && numedge_a != numedge_b)
		{
		  listve = NULL;
		  size = 0;
		  list_int_add (&listve, &size, retour->ed[i].ve1,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[i].ve2,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_a].ve1,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_a].ve2,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_b].ve1,
				WITHOUT_REDUNDANCE);
		  list_int_add (&listve, &size, retour->ed[numedge_b].ve2,
				WITHOUT_REDUNDANCE);

		  if (size == 3)
		    {
		      if (a2ri_vef_search_face
			  (retour, i, numedge_a, numedge_b) == -1
			  && a2ri_vef_search_face (retour, i, numedge_b,
						   numedge_a) == -1)
			a2ri_vef_add_face (retour, i, numedge_a, numedge_b);
		    }

		  free (listve);
		}
	    }
	}
    }

  return retour;
}

/**
   Calcul du graphe recouvrant minimal (EMST - Euclidean Minimal Spanning Tree)
   @param m le modele
   @return un modele contenant les aretes
*/
vef_model *
a2ri_vef_euclidean_minimal_spanning_tree (
					  const vef_model * const m)
{
  int *listedge = NULL,
    sizelistedge = 0,
    sizelistlongueur = 0;
  double *listlongueur = NULL;
  point3d p1,
    p2;
  vef_model *retour = (vef_model *) malloc (sizeof (vef_model));
  a2ri_erreur_critique_si (retour == NULL,
			"erreur allocation memoire pour retour\na2ri_vef_euclidean_minimal_spanning_tree");
  int *vnew = NULL,
    sizevnew = 0;
  int k,
    trouve,
    contve1,
    contve2;


  a2ri_vef_init (retour);

  for (int i = 0; i < m->nbvertex; i++)
    a2ri_vef_add_vertex (retour, m->ve[i].x, m->ve[i].y, m->ve[i].z);

  retour->xmin = m->xmin;
  retour->xmax = m->xmax;
  retour->ymin = m->ymin;
  retour->ymax = m->ymax;
  retour->zmin = m->zmin;
  retour->zmax = m->zmax;

  for (int i = 0; i < m->nbedge; i++)
    {
      point3d_init (&p1, m->ve[m->ed[i].ve1].x, m->ve[m->ed[i].ve1].y,
		    m->ve[m->ed[i].ve1].z);
      point3d_init (&p2, m->ve[m->ed[i].ve2].x, m->ve[m->ed[i].ve2].y,
		    m->ve[m->ed[i].ve2].z);

      list_int_add (&listedge, &sizelistedge, i, WITH_REDUNDANCE);
      list_double_add (&listlongueur, &sizelistlongueur,
		       point3d_length (&p1, &p2), WITH_REDUNDANCE);
    }

  IFlist_edge_sort (listlongueur, listedge, sizelistedge);

  free (listlongueur);

  // Ajout du premier sommet dans la liste
  list_int_add (&vnew, &sizevnew, 0, WITH_REDUNDANCE);

  while (sizevnew != m->nbvertex)
    {
      k = 0;
      trouve = 0;
      while (!trouve)
	{
	  do
	    {
	      contve1 =
		list_int_contains (vnew, sizevnew, m->ed[listedge[k]].ve1);
	      contve2 =
		list_int_contains (vnew, sizevnew, m->ed[listedge[k]].ve2);
	      k++;
	    }
	  while (contve1 == -1 && contve2 == -1);
	  k--;

	  if (contve1 != -1 && contve2 != -1)
	    list_int_remove (&listedge, &sizelistedge, k);
	  else
	    trouve = 1;
	}

      a2ri_vef_add_edge (retour, m->ed[listedge[k]].ve1,
			 m->ed[listedge[k]].ve2, 0);
      if (contve1 == -1)
	list_int_add (&vnew, &sizevnew, m->ed[listedge[k]].ve1,
		      WITH_REDUNDANCE);
      else
	list_int_add (&vnew, &sizevnew, m->ed[listedge[k]].ve2,
		      WITH_REDUNDANCE);


      list_int_remove (&listedge, &sizelistedge, k);
    }

  free (listedge);
  free (vnew);

  return retour;

}
