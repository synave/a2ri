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

#include "subdivision.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

typedef struct
{
  vf_model *m;
  vf_vertex *nvpt;
  int att_int;
} ITargument_hashtable;

typedef struct
{
  vf_model *m;
  int *indexar;
  point3d * A,
    * B,
    * C;
} ITparam_decoup;

void
IFedge_cut (
	    int key,
	    vf_edge * value,
	    void *user_data)
{
  point3d p1,
    p2;
  double t;
  ITparam_decoup *arg = user_data;
  int *indexarete = arg->indexar;
  vf_model *m = arg->m;
  point3d * A = arg->A,
    * B = arg->B,
    * C = arg->C;
  
  p1.x = m->ve[value->ve1].x;
  p1.y = m->ve[value->ve1].y;
  p1.z = m->ve[value->ve1].z;
  p2.x = m->ve[value->ve2].x;
  p2.y = m->ve[value->ve2].y;
  p2.z = m->ve[value->ve2].z;
  
  if (intersection_droite_plan (&p1, &p2, A, B, C, &t) && !egalite (t, 0)
      && !egalite (t, 1) && t>0 && t<1)
    {
      value->att_int = indexarete[0];
      indexarete[0] = indexarete[0] + 1;
      value->att_double = t;
    }
  else
    {
      value->att_int = -1;
      value->att_double = -1;
    }
  
}

void
IFcenter_cut (
	      int key,
	      vf_edge * value,
	      void *user_data)
{
  ITargument_hashtable *c = user_data;
  vf_model *m = c->m;
  vf_vertex *nvpt = c->nvpt;

  //on affecte le numero du point milieu
  value->att_int = m->nbvertex;

  //on ajoute le nouveau point comme point incident aux points p1 et p2
  list_int_add (&(nvpt[value->ve1].incidentvertices),
		&(nvpt[value->ve1].nbincidentvertices), m->nbvertex,
		WITH_REDUNDANCE);
  list_int_add (&(nvpt[value->ve2].incidentvertices),
		&(nvpt[value->ve2].nbincidentvertices), m->nbvertex,
		WITH_REDUNDANCE);

  //on ajoute p1 et p2 à la liste des points incidents au nouveau point
  list_int_add (&(nvpt[m->nbvertex].incidentvertices),
		&(nvpt[m->nbvertex].nbincidentvertices), value->ve1,
		WITH_REDUNDANCE);
  list_int_add (&(nvpt[m->nbvertex].incidentvertices),
		&(nvpt[m->nbvertex].nbincidentvertices), value->ve2,
		WITH_REDUNDANCE);

  //on calcule les coordonnées temporaires du nouveau point
  double x = (m->ve[value->ve1].x + m->ve[value->ve2].x) / 2.0;
  double y = (m->ve[value->ve1].y + m->ve[value->ve2].y) / 2.0;
  double z = (m->ve[value->ve1].z + m->ve[value->ve2].z) / 2.0;

  //on ajoute le nouveau point. m->nbvertex sera incrémenté automatiquement
  a2ri_vf_add_vertex (m, x, y, z);
}

void
IFreplace_nvpt (
		int key,
		vf_edge * value,
		void *user_data)
{
  ITargument_hashtable *c = user_data;
  vf_model *m = c->m;
  vf_vertex *nvpt = c->nvpt;
  int ptmilieu = value->att_int;


  if (value->nbsharedfaces == 1)
    {
      //cas d'une arete sur un bord
      //calcul des nouvelles coordonnées
      double x = 0.5 * (m->ve[value->ve1].x + m->ve[value->ve2].x);
      double y = 0.5 * (m->ve[value->ve1].y + m->ve[value->ve2].y);
      double z = 0.5 * (m->ve[value->ve1].z + m->ve[value->ve2].z);
      //placement des coordonnées dans le tableau des nouveaux points
      nvpt[ptmilieu].x = x;
      nvpt[ptmilieu].y = y;
      nvpt[ptmilieu].z = z;
      //mis à jour de la bounding box
      if (m->xmin > x)
	m->xmin = x;
      if (m->xmax < x)
	m->xmax = x;
      if (m->ymin > y)
	m->ymin = y;
      if (m->ymax < y)
	m->ymax = y;
      if (m->zmin > z)
	m->zmin = z;
      if (m->zmax < z)
	m->zmax = z;
    }
  else
    {
      //cas d'un point intérieur
      int f1 = value->sharedfaces[0];
      int f2 = value->sharedfaces[1];
      int p3,
        p4;

      //p3 et p4 sont les deux sommets appartenant aux deux faces partagées par l'arete mais n'étant pas sur l'arete
      p3 = m->fa[f1].ve1;
      if (p3 == value->ve1 || p3 == value->ve2)
	p3 = m->fa[f1].ve2;
      if (p3 == value->ve1 || p3 == value->ve2)
	p3 = m->fa[f1].ve3;

      p4 = m->fa[f2].ve1;
      if (p4 == value->ve1 || p4 == value->ve2)
	p4 = m->fa[f2].ve2;
      if (p4 == value->ve1 || p4 == value->ve2)
	p4 = m->fa[f2].ve3;

      //calcul des nouvelles coordonnées
      double x =
	(3.0 / 8.0) * (m->ve[value->ve1].x + m->ve[value->ve2].x) +
	(1.0 / 8.0) * (m->ve[p3].x + m->ve[p4].x);
      double y =
	(3.0 / 8.0) * (m->ve[value->ve1].y + m->ve[value->ve2].y) +
	(1.0 / 8.0) * (m->ve[p3].y + m->ve[p4].y);
      double z =
	(3.0 / 8.0) * (m->ve[value->ve1].z + m->ve[value->ve2].z) +
	(1.0 / 8.0) * (m->ve[p3].z + m->ve[p4].z);
      //placement des coordonnées dans le tableau des nouveaux points
      nvpt[ptmilieu].x = x;
      nvpt[ptmilieu].y = y;
      nvpt[ptmilieu].z = z;
      //mis à jour de la bounding box
      if (m->xmin > x)
	m->xmin = x;
      if (m->xmax < x)
	m->xmax = x;
      if (m->ymin > y)
	m->ymin = y;
      if (m->ymax < y)
	m->ymax = y;
      if (m->zmin > z)
	m->zmin = z;
      if (m->zmax < z)
	m->zmax = z;
    }
}

void
IFcount_subdivided_edge (
			 int key,
			 vf_edge * value,
			 void *user_data)
{
  ITargument_hashtable *c = user_data;
  
  if (value->att_int != -1)
    c->att_int = (c->att_int) + 1;
}

void
IFvef_but_recherche_p1_p2 (
			   vef_model * m,
			   int numedge,
			   int *p1,
			   int *p2)
{
  *p1 = m->ed[numedge].ve1;
  *p2 = m->ed[numedge].ve2;
}

void
IFvef_but_recherche_p3_p4 (
			   vef_model * m,
			   int numedge,
			   int *p3,
			   int *p4,
			   int *artemp1,
			   int *artemp2,
			   int *artemp3,
			   int *artemp4)
{
  int f1 = -1;
  int f2 = -1;
  for (int i = 0; i < m->nbface; i++)
    {
      if (vef_face_contains (&(m->fa[i]), numedge))
	{
	  if (f1 == -1)
	    f1 = i;
	  else
	    f2 = i;
	}
    }

  if (f1 == -1)
    {
      *p3 = -1;
      *p4 = -1;
      *artemp1 = -1;
      *artemp2 = -1;
      *artemp3 = -1;
      *artemp4 = -1;
      return;
    }


  int ar1 = m->fa[f1].ed1;
  int ar2 = m->fa[f1].ed2;
  int ar3 = m->fa[f1].ed3;

  if (ar1 == numedge)
    ar1 = ar3;
  if (ar2 == numedge)
    ar2 = ar3;

  *artemp1 = ar1;
  *artemp2 = ar2;

  if (m->ed[ar1].ve1 == m->ed[ar2].ve1 || m->ed[ar1].ve1 == m->ed[ar2].ve2)
    *p3 = m->ed[ar1].ve1;
  else
    *p3 = m->ed[ar1].ve2;

  if (f2 == -1)
    {
      *p4 = -1;
      *artemp3 = -1;
      *artemp4 = -1;
      return;
    }

  ar1 = m->fa[f2].ed1;
  ar2 = m->fa[f2].ed2;
  ar3 = m->fa[f2].ed3;

  if (ar1 == numedge)
    ar1 = ar3;
  if (ar2 == numedge)
    ar2 = ar3;

  *artemp3 = ar1;
  *artemp4 = ar2;

  if (m->ed[ar1].ve1 == m->ed[ar2].ve1 || m->ed[ar1].ve1 == m->ed[ar2].ve2)
    *p4 = m->ed[ar1].ve1;
  else
    *p4 = m->ed[ar1].ve2;
}

void
IFvef_but_recherche_p5_p6_p7_p8 (
				 vef_model * m,
				 int artemp1,
				 int artemp2,
				 int artemp3,
				 int artemp4,
				 int *p5,
				 int *p6,
				 int *p7,
				 int *p8)
{
  int f = -1;
  int index = -1;
  int p1temp,
    p2temp,
    p3temp;


  while (f == -1 && index < (m->nbface - 1))
    {
      index++;
      if (vef_face_contains (&(m->fa[index]), artemp1)
	  && !vef_face_contains (&(m->fa[index]), artemp2))
	f = index;
    }
  if (f == -1)
    {
      *p5 = -1;
    }
  else
    {
      p1temp = m->fa[f].ed1;
      p2temp = m->fa[f].ed2;
      p3temp = m->fa[f].ed3;


      if (p1temp == artemp1)
	p1temp = p3temp;
      if (p2temp == artemp1)
	p2temp = p3temp;

      if (m->ed[p1temp].ve1 == m->ed[p2temp].ve1
	  || m->ed[p1temp].ve1 == m->ed[p2temp].ve2)
	*p5 = m->ed[p1temp].ve1;
      else
	*p5 = m->ed[p1temp].ve2;
    }



  f = -1;
  index = -1;
  while (f == -1 && index < (m->nbface - 1))
    {
      index++;
      if (vef_face_contains (&(m->fa[index]), artemp2)
	  && !vef_face_contains (&(m->fa[index]), artemp1))
	f = index;
    }
  if (f == -1)
    {
      *p6 = -1;
    }
  else
    {
      p1temp = m->fa[f].ed1;
      p2temp = m->fa[f].ed2;
      p3temp = m->fa[f].ed3;

      if (p1temp == artemp2)
	p1temp = p3temp;
      if (p2temp == artemp2)
	p2temp = p3temp;

      if (m->ed[p1temp].ve1 == m->ed[p2temp].ve1
	  || m->ed[p1temp].ve1 == m->ed[p2temp].ve2)
	*p6 = m->ed[p1temp].ve1;
      else
	*p6 = m->ed[p1temp].ve2;
    }




  f = -1;
  index = -1;
  while (f == -1 && index < (m->nbface - 1))
    {
      index++;
      if (vef_face_contains (&(m->fa[index]), artemp3)
	  && !vef_face_contains (&(m->fa[index]), artemp4))
	f = index;
    }
  if (f == -1)
    {
      *p7 = -1;
    }
  else
    {
      p1temp = m->fa[f].ed1;
      p2temp = m->fa[f].ed2;
      p3temp = m->fa[f].ed3;

      if (p1temp == artemp3)
	p1temp = p3temp;
      if (p2temp == artemp3)
	p2temp = p3temp;

      if (m->ed[p1temp].ve1 == m->ed[p2temp].ve1
	  || m->ed[p1temp].ve1 == m->ed[p2temp].ve2)
	*p7 = m->ed[p1temp].ve1;
      else
	*p7 = m->ed[p1temp].ve2;
    }




  f = -1;
  index = -1;
  while (f == -1 && index < (m->nbface - 1))
    {
      index++;
      if (vef_face_contains (&(m->fa[index]), artemp4)
	  && !vef_face_contains (&(m->fa[index]), artemp3))
	f = index;
    }
  if (f == -1)
    {
      *p8 = -1;
    }
  else
    {
      p1temp = m->fa[f].ed1;
      p2temp = m->fa[f].ed2;
      p3temp = m->fa[f].ed3;

      if (p1temp == artemp4)
	p1temp = p3temp;
      if (p2temp == artemp4)
	p2temp = p3temp;

      if (m->ed[p1temp].ve1 == m->ed[p2temp].ve1
	  || m->ed[p1temp].ve1 == m->ed[p2temp].ve2)
	*p8 = m->ed[p1temp].ve1;
      else
	*p8 = m->ed[p1temp].ve2;
    }
}

/********** MAIN FUNCTIONS **********/

/**
   Subdivision d'un modèle avec la méthode de Loop
   @param m pointeur sur le modèle à subdiviser
   @param nbiter nombre d'itération de la subdivision
   @return aucun
*/
void
a2ri_vf_loop (
	      vf_model * m,
	      int nbiter)
{
  ITargument_hashtable *arg = (ITargument_hashtable *) malloc (sizeof (ITargument_hashtable));
  a2ri_erreur_critique_si (arg == NULL,
			"erreur allocation memoire pour arg\na2ri_vf_loop");

  for (int i = 0; i < nbiter; i++)
    {
      int nbpointancien = m->nbvertex;
      hashtable *table = a2ri_vf_construction_edge_table (m, NULL, 0);

      vf_vertex *nvpt =
	(vf_vertex *) malloc ((m->nbvertex + 3 * hashtable_size (table)) *
			      sizeof (vf_vertex));
      a2ri_erreur_critique_si (nvpt == NULL,
			    "erreur allocation memoire pour nvpt\na2ri_vf_loop");
      vf_face *nvfa = (vf_face *) malloc (m->nbface * 4 * sizeof (vf_face));
      a2ri_erreur_critique_si (nvfa == NULL,
			    "erreur allocation memoire pour nvfa\na2ri_vf_loop");

      //initialisation du tableau contenant les nouveaux sommets
      for (int j = 0; j < m->nbvertex + 3 * ((int) (hashtable_size (table)));
	   j++)
	{
	  nvpt[j].nbincidentvertices = 0;
	  nvpt[j].incidentvertices = NULL;
	}

      //on fait le découpage de toutes les aretes.
      arg->m = m;
      arg->nvpt = nvpt;
      hashtable_foreach (table, IFcenter_cut, arg);

      for (int j = 0; j < m->nbface; j++)
	{
	  int ptmilieu1,
	    ptmilieu2,
	    ptmilieu3;
	  vf_edge *e;

	  //on cherche les 3 aretes de la face en cours ainsi que les points milieux
	  e = hashtable_look_for (table, m->fa[j].ve1, m->fa[j].ve2);
	  ptmilieu1 = e->att_int;

	  e = hashtable_look_for (table, m->fa[j].ve1, m->fa[j].ve3);
	  ptmilieu2 = e->att_int;

	  e = hashtable_look_for (table, m->fa[j].ve2, m->fa[j].ve3);
	  ptmilieu3 = e->att_int;

	  //création de la première sous-face
	  nvfa[j * 4].ve1 = m->fa[j].ve1;
	  nvfa[j * 4].ve2 = ptmilieu1;
	  nvfa[j * 4].ve3 = ptmilieu2;

	  //création de la seconde sous-face
	  nvfa[(j * 4) + 1].ve1 = ptmilieu1;
	  nvfa[(j * 4) + 1].ve2 = m->fa[j].ve2;
	  nvfa[(j * 4) + 1].ve3 = ptmilieu3;

	  //création de la troisième sous-face
	  nvfa[(j * 4) + 2].ve1 = m->fa[j].ve3;
	  nvfa[(j * 4) + 2].ve2 = ptmilieu2;
	  nvfa[(j * 4) + 2].ve3 = ptmilieu3;

	  //création de la quatrième sous-face
	  nvfa[(j * 4) + 3].ve1 = ptmilieu1;
	  nvfa[(j * 4) + 3].ve2 = ptmilieu3;
	  nvfa[(j * 4) + 3].ve3 = ptmilieu2;

	  //on met à jour les liste des points incidents des points milieux
	  list_int_add (&(nvpt[ptmilieu1].incidentvertices),
			&(nvpt[ptmilieu1].nbincidentvertices), ptmilieu2,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ptmilieu1].incidentvertices),
			&(nvpt[ptmilieu1].nbincidentvertices), ptmilieu3,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ptmilieu2].incidentvertices),
			&(nvpt[ptmilieu2].nbincidentvertices), ptmilieu1,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ptmilieu2].incidentvertices),
			&(nvpt[ptmilieu2].nbincidentvertices), ptmilieu3,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ptmilieu3].incidentvertices),
			&(nvpt[ptmilieu3].nbincidentvertices), ptmilieu1,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ptmilieu3].incidentvertices),
			&(nvpt[ptmilieu3].nbincidentvertices), ptmilieu2,
			WITH_REDUNDANCE);
	}

      //les nouveaux points sont donc créés, placés et les faces dont découpées

      //les anciens points sont replacés
      for (int j = 0; j < nbpointancien; j++)
	{
	  int bord = 0;
	  int mpt1 = -1;
	  int mpt2 = -1;
	  for (int k = 0; k < m->ve[j].nbincidentvertices; k++)
	    {
	      vf_edge *e;

	      e = hashtable_look_for (table, j, m->ve[j].incidentvertices[k]);
	      if (e != NULL)
		//si le nombre de faces partagés est de une seule
		if (e->nbsharedfaces == 1)
		  {
		    //on est sur un bord
		    bord++;
		    //on recherche les deux points qui serviront à son repositionnement
		    if (mpt1 == -1)
		      if (e->ve1 == j)
			mpt1 = e->ve2;
		      else
			mpt1 = e->ve1;
		    else if (e->ve1 == j)
		      mpt2 = e->ve2;
		    else
		      mpt2 = e->ve1;
		  }
	    }


	  if (bord == 0)
	    {
	      //si le sommet est intérieur au maillage
	      double alphan =
		(1.0 / 64.0) * (40 -
				(3 +
				 2 * cos (2 * M_PI /
					  m->ve[j].nbincidentvertices)) * (3 +
									   2 *
									   cos
									   (2
									    *
									    M_PI
									    /
									    m->
									    ve
									    [j].
									    nbincidentvertices)));
	      double b = alphan / m->ve[j].nbincidentvertices;
	      double centre = 1.0 - alphan;
	      double x = centre * m->ve[j].x;
	      double y = centre * m->ve[j].y;
	      double z = centre * m->ve[j].z;
	      for (int k = 0; k < m->ve[j].nbincidentvertices; k++)
		{
		  x += b * m->ve[m->ve[j].incidentvertices[k]].x;
		  y += b * m->ve[m->ve[j].incidentvertices[k]].y;
		  z += b * m->ve[m->ve[j].incidentvertices[k]].z;
		}
	      //on place les nouvelles coordonnées dans le tableau des nouveaux points
	      nvpt[j].x = x;
	      nvpt[j].y = y;
	      nvpt[j].z = z;

	      //on met à jour la bonding box
	      if (j == 0)
		{
		  m->xmin = x;
		  m->xmax = x;
		  m->ymin = y;
		  m->ymax = y;
		  m->zmin = z;
		  m->zmax = z;
		}
	      else
		{
		  if (m->xmin > x)
		    m->xmin = x;
		  if (m->xmax < x)
		    m->xmax = x;
		  if (m->ymin > y)
		    m->ymin = y;
		  if (m->ymax < y)
		    m->ymax = y;
		  if (m->zmin > z)
		    m->zmin = z;
		  if (m->zmax < z)
		    m->zmax = z;
		}

	    }
	  else
	    {
	      //dans le cas d'un point situés sur un bord
	      double x =
		(1.0 / 8.0) * (m->ve[mpt1].x + m->ve[mpt2].x) +
		(6.0 / 8.0) * m->ve[j].x;
	      double y =
		(1.0 / 8.0) * (m->ve[mpt1].y + m->ve[mpt2].y) +
		(6.0 / 8.0) * m->ve[j].y;
	      double z =
		(1.0 / 8.0) * (m->ve[mpt1].z + m->ve[mpt2].z) +
		(6.0 / 8.0) * m->ve[j].z;
	      //les nouvelles coordonnées sont placés dans le tableau des nouveaux points
	      nvpt[j].x = x;
	      nvpt[j].y = y;
	      nvpt[j].z = z;

	      //on met à jour la bounding box
	      if (j == 0)
		{
		  m->xmin = x;
		  m->xmax = x;
		  m->ymin = y;
		  m->ymax = y;
		  m->zmin = z;
		  m->zmax = z;
		}
	      else
		{
		  if (m->xmin > x)
		    m->xmin = x;
		  if (m->xmax < x)
		    m->xmax = x;
		  if (m->ymin > y)
		    m->ymin = y;
		  if (m->ymax < y)
		    m->ymax = y;
		  if (m->zmin > z)
		    m->zmin = z;
		  if (m->zmax < z)
		    m->zmax = z;
		}

	    }
	}

      arg->m = m;
      arg->nvpt = nvpt;

      //pour toutes les aretes, les points mileux (points créés pendant la subdivision) sont repsoitionnés
      hashtable_foreach (table, IFreplace_nvpt, arg);

      //destruction de la table
      hashtable_free (table);
      free (table);

      for (int j = 0; j < m->nbvertex; j++)
	vf_vertex_free (&(m->ve[j]));

      free (m->ve);
      free (m->fa);


      //mis à jour du nombre de faces
      m->nbface = m->nbface * 4;
      //remplacement des tableaux de faces et de sommets
      m->fa = nvfa;
      m->ve = nvpt;

    }

  free (arg);
}


/**
   Subdivision d'un modele ou coupant les triangles en 6 triangles avec un point central
   @param m pointeur sur le modele a subdiviser
   @param nbiter nom c'iteration de la subdivision
   @param aucun
*/
void
a2ri_vf_6_subdivision (
		       vf_model * m,
		       int nbiter)
{
  ITargument_hashtable *arg = (ITargument_hashtable *) malloc (sizeof (ITargument_hashtable));
  a2ri_erreur_critique_si (arg == NULL,
			"erreur allocation memoire pour arg\na2ri_vf_6_subdivision");

  for (int i = 0; i < nbiter; i++)
    {
      hashtable *table = a2ri_vf_construction_edge_table (m, NULL, 0);

      vf_vertex *nvpt =
	(vf_vertex *)
	malloc ((m->nbvertex + m->nbface +
		 3 * hashtable_size (table)) * sizeof (vf_vertex));
      a2ri_erreur_critique_si (nvpt == NULL,
			    "erreur allocation memoire pour nvpt\na2ri_vf_6_subdivision");
      vf_face *nvfa = (vf_face *) malloc (m->nbface * 6 * sizeof (vf_face));
      a2ri_erreur_critique_si (nvfa == NULL,
			    "erreur allocation memoire pour nvfa\na2ri_vf_6_subdivision");

      //initialisation du tableau contenant les nouveaux sommets
      for (int j = 0;
	   j <
	     (m->nbvertex + m->nbface + 3 * ((int) (hashtable_size (table))));
	   j++)
	{
	  nvpt[j].nbincidentvertices = 0;
	  nvpt[j].incidentvertices = NULL;
	}

      //on fait le découpage de toutes les aretes.
      arg->m = m;
      arg->nvpt = nvpt;
      arg->att_int = 0;
      hashtable_foreach (table, IFcenter_cut, arg);

      for (int j = 0; j < m->nbface; j++)
	{
	  int ptmilieu1,
	    ptmilieu2,
	    ptmilieu3;
	  vf_edge *e;

	  double x =
	    (m->ve[m->fa[j].ve1].x + m->ve[m->fa[j].ve2].x +
	     m->ve[m->fa[j].ve3].x) / 3.0;
	  double y =
	    (m->ve[m->fa[j].ve1].y + m->ve[m->fa[j].ve2].y +
	     m->ve[m->fa[j].ve3].y) / 3.0;
	  double z =
	    (m->ve[m->fa[j].ve1].z + m->ve[m->fa[j].ve2].z +
	     m->ve[m->fa[j].ve3].z) / 3.0;

	  //on cherche les 3 aretes de la face en cours ainsi que les points milieux
	  e = hashtable_look_for (table, m->fa[j].ve1, m->fa[j].ve2);
	  ptmilieu1 = e->att_int;

	  e = hashtable_look_for (table, m->fa[j].ve1, m->fa[j].ve3);
	  ptmilieu2 = e->att_int;

	  e = hashtable_look_for (table, m->fa[j].ve2, m->fa[j].ve3);
	  ptmilieu3 = e->att_int;

	  //création de la première sous-face
	  nvfa[j * 6].ve1 = m->fa[j].ve1;
	  nvfa[j * 6].ve2 = ptmilieu1;
	  nvfa[j * 6].ve3 = m->nbvertex;

	  //création de la seconde sous-face
	  nvfa[(j * 6) + 1].ve1 = ptmilieu1;
	  nvfa[(j * 6) + 1].ve2 = m->fa[j].ve2;
	  nvfa[(j * 6) + 1].ve3 = m->nbvertex;

	  //création de la troisième sous-face
	  nvfa[(j * 6) + 2].ve1 = m->fa[j].ve2;
	  nvfa[(j * 6) + 2].ve2 = ptmilieu3;
	  nvfa[(j * 6) + 2].ve3 = m->nbvertex;

	  //création de la quatrième sous-face
	  nvfa[(j * 6) + 3].ve1 = ptmilieu3;
	  nvfa[(j * 6) + 3].ve2 = m->fa[j].ve3;
	  nvfa[(j * 6) + 3].ve3 = m->nbvertex;

	  //création de la cinquième sous-face
	  nvfa[(j * 6) + 4].ve1 = m->fa[j].ve3;
	  nvfa[(j * 6) + 4].ve2 = ptmilieu2;
	  nvfa[(j * 6) + 4].ve3 = m->nbvertex;

	  //création de la sixième sous-face
	  nvfa[(j * 6) + 5].ve1 = ptmilieu2;
	  nvfa[(j * 6) + 5].ve2 = m->fa[j].ve1;
	  nvfa[(j * 6) + 5].ve3 = m->nbvertex;

	  //on met à jour les liste des points incidents des points milieux
	  list_int_add (&(nvpt[m->fa[j].ve1].incidentvertices),
			&(nvpt[m->fa[j].ve1].nbincidentvertices), m->nbvertex,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[m->nbvertex].incidentvertices),
			&(nvpt[m->nbvertex].nbincidentvertices), m->fa[j].ve1,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[m->fa[j].ve2].incidentvertices),
			&(nvpt[m->fa[j].ve2].nbincidentvertices), m->nbvertex,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[m->nbvertex].incidentvertices),
			&(nvpt[m->nbvertex].nbincidentvertices), m->fa[j].ve2,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[m->fa[j].ve3].incidentvertices),
			&(nvpt[m->fa[j].ve3].nbincidentvertices), m->nbvertex,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[m->nbvertex].incidentvertices),
			&(nvpt[m->nbvertex].nbincidentvertices), m->fa[j].ve3,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ptmilieu1].incidentvertices),
			&(nvpt[ptmilieu1].nbincidentvertices), m->nbvertex,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[m->nbvertex].incidentvertices),
			&(nvpt[m->nbvertex].nbincidentvertices), ptmilieu1,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ptmilieu2].incidentvertices),
			&(nvpt[ptmilieu2].nbincidentvertices), m->nbvertex,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[m->nbvertex].incidentvertices),
			&(nvpt[m->nbvertex].nbincidentvertices), ptmilieu2,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ptmilieu3].incidentvertices),
			&(nvpt[ptmilieu3].nbincidentvertices), m->nbvertex,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[m->nbvertex].incidentvertices),
			&(nvpt[m->nbvertex].nbincidentvertices), ptmilieu3,
			WITH_REDUNDANCE);


	  a2ri_vf_add_vertex (m, x, y, z);
	}

      for (int i = 0; i < m->nbvertex; i++)
	{
	  nvpt[i].x = m->ve[i].x;
	  nvpt[i].y = m->ve[i].y;
	  nvpt[i].z = m->ve[i].z;
	}

      //destruction de la table
      hashtable_free (table);
      free (table);

      for (int j = 0; j < m->nbvertex; j++)
	vf_vertex_free (&(m->ve[j]));

      //mis à jour du nombre de faces
      m->nbface = m->nbface * 6;

      free (m->ve);
      free (m->fa);

      //remplacement des tableaux de faces et de sommets
      m->fa = nvfa;
      m->ve = nvpt;

    }
  free (arg);
}


/**
   Subdivision d'un modele ou coupant les triangles en 4 triangles
   @param m pointeur sur le modele a subdiviser
   @param nbiter nom c'iteration de la subdivision
   @param aucun
*/
void
a2ri_vf_4_subdivision (
		       vf_model * m,
		       int nbiter)
{
  ITargument_hashtable *arg = (ITargument_hashtable *) malloc (sizeof (ITargument_hashtable));
  a2ri_erreur_critique_si (arg == NULL,
			"erreur allocation memoire pour arg\na2ri_vf_4_subdivision");

  for (int i = 0; i < nbiter; i++)
    {
      hashtable *table = a2ri_vf_construction_edge_table (m, NULL, 0);

      vf_vertex *nvpt =
	(vf_vertex *) malloc ((m->nbvertex + 3 * hashtable_size (table)) *
			      sizeof (vf_vertex));
      a2ri_erreur_critique_si (nvpt == NULL,
			    "erreur allocation memoire pour nvpt\na2ri_vf_4_subdivision");
      vf_face *nvfa = (vf_face *) malloc (m->nbface * 4 * sizeof (vf_face));
      a2ri_erreur_critique_si (nvfa == NULL,
			    "erreur allocation memoire pour nvfa\na2ri_vf_4_subdivision");

      //initialisation du tableau contenant les nouveaux sommets
      for (int j = 0; j < m->nbvertex + 3 * ((int) (hashtable_size (table)));
	   j++)
	{
	  nvpt[j].nbincidentvertices = 0;
	  nvpt[j].incidentvertices = NULL;
	}

      for (int j = 0; j < m->nbvertex; j++)
	{
	  nvpt[j].x = m->ve[j].x;
	  nvpt[j].y = m->ve[j].y;
	  nvpt[j].z = m->ve[j].z;
	}

      //on fait le découpage de toutes les aretes.
      arg->m = m;
      arg->nvpt = nvpt;
      hashtable_foreach (table, IFcenter_cut, arg);

      for (int j = 0; j < m->nbface; j++)
	{
	  int ptmilieu1,
	    ptmilieu2,
	    ptmilieu3;
	  vf_edge *e;

	  //on cherche les 3 aretes de la face en cours ainsi que les points milieux
	  e = hashtable_look_for (table, m->fa[j].ve1, m->fa[j].ve2);
	  ptmilieu1 = e->att_int;
	  nvpt[ptmilieu1].x = (m->ve[e->ve1].x + m->ve[e->ve2].x) / 2.0;
	  nvpt[ptmilieu1].y = (m->ve[e->ve1].y + m->ve[e->ve2].y) / 2.0;
	  nvpt[ptmilieu1].z = (m->ve[e->ve1].z + m->ve[e->ve2].z) / 2.0;

	  e = hashtable_look_for (table, m->fa[j].ve1, m->fa[j].ve3);
	  ptmilieu2 = e->att_int;
	  nvpt[ptmilieu2].x = (m->ve[e->ve1].x + m->ve[e->ve2].x) / 2.0;
	  nvpt[ptmilieu2].y = (m->ve[e->ve1].y + m->ve[e->ve2].y) / 2.0;
	  nvpt[ptmilieu2].z = (m->ve[e->ve1].z + m->ve[e->ve2].z) / 2.0;

	  e = hashtable_look_for (table, m->fa[j].ve2, m->fa[j].ve3);
	  ptmilieu3 = e->att_int;
	  nvpt[ptmilieu3].x = (m->ve[e->ve1].x + m->ve[e->ve2].x) / 2.0;
	  nvpt[ptmilieu3].y = (m->ve[e->ve1].y + m->ve[e->ve2].y) / 2.0;
	  nvpt[ptmilieu3].z = (m->ve[e->ve1].z + m->ve[e->ve2].z) / 2.0;

	  //création de la première sous-face
	  nvfa[j * 4].ve1 = m->fa[j].ve1;
	  nvfa[j * 4].ve2 = ptmilieu1;
	  nvfa[j * 4].ve3 = ptmilieu2;

	  //création de la seconde sous-face
	  nvfa[(j * 4) + 1].ve1 = ptmilieu1;
	  nvfa[(j * 4) + 1].ve2 = m->fa[j].ve2;
	  nvfa[(j * 4) + 1].ve3 = ptmilieu3;

	  //création de la troisième sous-face
	  nvfa[(j * 4) + 2].ve1 = m->fa[j].ve3;
	  nvfa[(j * 4) + 2].ve2 = ptmilieu2;
	  nvfa[(j * 4) + 2].ve3 = ptmilieu3;

	  //création de la quatrième sous-face
	  nvfa[(j * 4) + 3].ve1 = ptmilieu1;
	  nvfa[(j * 4) + 3].ve2 = ptmilieu3;
	  nvfa[(j * 4) + 3].ve3 = ptmilieu2;

	  //on met à jour les liste des points incidents des points milieux
	  list_int_add (&(nvpt[ptmilieu1].incidentvertices),
			&(nvpt[ptmilieu1].nbincidentvertices), ptmilieu2,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ptmilieu1].incidentvertices),
			&(nvpt[ptmilieu1].nbincidentvertices), ptmilieu3,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ptmilieu2].incidentvertices),
			&(nvpt[ptmilieu2].nbincidentvertices), ptmilieu1,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ptmilieu2].incidentvertices),
			&(nvpt[ptmilieu2].nbincidentvertices), ptmilieu3,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ptmilieu3].incidentvertices),
			&(nvpt[ptmilieu3].nbincidentvertices), ptmilieu1,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ptmilieu3].incidentvertices),
			&(nvpt[ptmilieu3].nbincidentvertices), ptmilieu2,
			WITH_REDUNDANCE);
	}

      //les nouveaux points sont donc créés, placés et les faces dont découpées


      //destruction de la table
      hashtable_free (table);
      free (table);

      for (int j = 0; j < m->nbvertex; j++)
	vf_vertex_free (&(m->ve[j]));

      free (m->ve);
      free (m->fa);


      //mis à jour du nombre de faces
      m->nbface = m->nbface * 4;
      //remplacement des tableaux de faces et de sommets
      m->fa = nvfa;
      m->ve = nvpt;

    }

  free (arg);
}

/**
   Subdivision d'un modele par un plan
   @param m pointeur sur le modele a subdiviser
   @param A premier point définissant le plan
   @param B second point définissant le plan
   @param C troisième point définissant le plan
   @param aucun
**/
void
a2ri_vf_subdivision_by_plane (
			      vf_model * m,
			      point3d * A,
			      point3d * B,
			      point3d * C)
{ 
  int indexarete[1];
  ITparam_decoup *arg = (ITparam_decoup *) malloc (sizeof (ITparam_decoup));
  a2ri_erreur_critique_si (arg == NULL,
			"erreur allocation memoire pour arg\na2ri_vf_subdivision_by_plane");

  hashtable *table = a2ri_vf_construction_edge_table (m, NULL, 0);
  indexarete[0] = 0;

  arg->indexar = indexarete;
  arg->m = m;
  arg->A = A;
  arg->B = B;
  arg->C = C;

  hashtable_foreach (table, IFedge_cut, arg);
  a2ri_vf_general_subdivision (m, table);
  hashtable_free (table);
  free (table);
  free (arg);
}

/**
   Subdivision d'un modele ou coupant les triangles en 6 triangles avec un point central
   @param m pointeur sur le modele a subdiviser
   @param nbiter nom c'iteration de la subdivision
   @param aucun
**/
void
a2ri_vf_general_subdivision (
			     vf_model * m,
			     hashtable * table)
{ 
  vf_edge *e1,
    *e2,
    *e3;
  ITargument_hashtable *arg = (ITargument_hashtable *) malloc (sizeof (ITargument_hashtable));
  a2ri_erreur_critique_si (arg == NULL,
			"erreur allocation memoire pour arg\na2ri_vf_general_subdivision");
  int nbfacesupp = 0;
  int ve1,
    ve2,
    ve3,
    ve4,
    ve5,
    ve6;
  int nbfacecourant = 0;
  double x,
    y,
    z;

  arg->att_int = 0;

  //calcul du nombre de sommets en plus
  hashtable_foreach (table, IFcount_subdivided_edge, arg);

  //calcul du nombre de faces supplémentaire
  for (int i = 0; i < m->nbface; i++)
    {
      e1 = hashtable_look_for (table, m->fa[i].ve1, m->fa[i].ve2);
      if (e1->att_int != -1)
	nbfacesupp++;

      e1 = hashtable_look_for (table, m->fa[i].ve2, m->fa[i].ve3);
      if (e1->att_int != -1)
	nbfacesupp++;

      e1 = hashtable_look_for (table, m->fa[i].ve3, m->fa[i].ve1);
      if (e1->att_int != -1)
	nbfacesupp++;
    }

  vf_vertex *nvpt =
    (vf_vertex *) malloc ((m->nbvertex + arg->att_int) * sizeof (vf_vertex));
  a2ri_erreur_critique_si (nvpt == NULL,
			"erreur allocation memoire pour nvpt\na2ri_vf_general_subdivision");
  vf_face *nvfa =
    (vf_face *) malloc ((m->nbface + nbfacesupp) * sizeof (vf_face));
  a2ri_erreur_critique_si (nvfa == NULL,
			"erreur allocation memoire pour nvfa\na2ri_vf_general_subdivision");

  //on met à jour la liste des nouveaux sommets
  for (int i = 0; i < m->nbvertex; i++)
    {
      nvpt[i].x = m->ve[i].x;
      nvpt[i].y = m->ve[i].y;
      nvpt[i].z = m->ve[i].z;
      nvpt[i].nbincidentvertices = 0;
      nvpt[i].incidentvertices = NULL;
    }

  for (int i = 0; i < arg->att_int; i++)
    {
      nvpt[i + m->nbvertex].nbincidentvertices = 0;
      nvpt[i + m->nbvertex].incidentvertices = NULL;
    }

  //on met à jour la liste des aretes
  for (int i = 0; i < m->nbface; i++)
    {
      nbfacesupp = 0;
      //on cherche le nombre de face à ajouter
      e1 = hashtable_look_for (table, m->fa[i].ve1, m->fa[i].ve2);
      if (e1->att_int != -1)
	nbfacesupp++;

      e2 = hashtable_look_for (table, m->fa[i].ve2, m->fa[i].ve3);
      if (e2->att_int != -1)
	nbfacesupp++;

      e3 = hashtable_look_for (table, m->fa[i].ve3, m->fa[i].ve1);
      if (e3->att_int != -1)
	nbfacesupp++;

      ve1 = m->fa[i].ve1;
      ve2 = m->fa[i].ve2;
      ve3 = m->fa[i].ve3;

      //en fonction du nombre de face supplémentaire, on traite d'une facon differente
      if (nbfacesupp == 0)
	{
	  nvfa[nbfacecourant].ve1 = ve1;
	  nvfa[nbfacecourant].ve2 = ve2;
	  nvfa[nbfacecourant].ve3 = ve3;
	  nbfacecourant++;

	  list_int_add (&(nvpt[ve1].incidentvertices),
			&(nvpt[ve1].nbincidentvertices), ve2,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve2].incidentvertices),
			&(nvpt[ve2].nbincidentvertices), ve1,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve1].incidentvertices),
			&(nvpt[ve1].nbincidentvertices), ve3,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve3].incidentvertices),
			&(nvpt[ve3].nbincidentvertices), ve1,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve2].incidentvertices),
			&(nvpt[ve2].nbincidentvertices), ve3,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve3].incidentvertices),
			&(nvpt[ve3].nbincidentvertices), ve2,
			WITH_REDUNDANCE);
	}


      if (nbfacesupp == 1)
	{
	  ve4 = e1->att_int + m->nbvertex;
	  if (e1->att_int == -1)
	    {
	      if (e2->att_int != -1)
		{
		  ve1 = m->fa[i].ve2;
		  ve2 = m->fa[i].ve3;
		  ve3 = m->fa[i].ve1;
		  e1 = e2;
		  ve4 = e1->att_int + m->nbvertex;
		}
	      else
		{
		  ve1 = m->fa[i].ve3;
		  ve2 = m->fa[i].ve1;
		  ve3 = m->fa[i].ve2;
		  e1 = e3;
		  ve4 = e1->att_int + m->nbvertex;
		}
	    }
	  nvfa[nbfacecourant].ve1 = ve1;
	  nvfa[nbfacecourant].ve2 = ve4;
	  nvfa[nbfacecourant].ve3 = ve3;
	  nbfacecourant++;
	  nvfa[nbfacecourant].ve1 = ve4;
	  nvfa[nbfacecourant].ve2 = ve2;
	  nvfa[nbfacecourant].ve3 = ve3;
	  nbfacecourant++;

	  list_int_add (&(nvpt[ve1].incidentvertices),
			&(nvpt[ve1].nbincidentvertices), ve4,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve4].incidentvertices),
			&(nvpt[ve4].nbincidentvertices), ve1,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve1].incidentvertices),
			&(nvpt[ve1].nbincidentvertices), ve3,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve3].incidentvertices),
			&(nvpt[ve3].nbincidentvertices), ve1,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve4].incidentvertices),
			&(nvpt[ve4].nbincidentvertices), ve3,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve3].incidentvertices),
			&(nvpt[ve3].nbincidentvertices), ve4,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve2].incidentvertices),
			&(nvpt[ve2].nbincidentvertices), ve3,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve3].incidentvertices),
			&(nvpt[ve3].nbincidentvertices), ve2,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve2].incidentvertices),
			&(nvpt[ve2].nbincidentvertices), ve4,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve4].incidentvertices),
			&(nvpt[ve4].nbincidentvertices), ve2,
			WITH_REDUNDANCE);

	  //on parcourt une fois les aretes dans l'ordre où elles sont été définie puis dans la direction opposée
	  //c'est pourquoi la différence se fait dans le mauvais sens

	  if(ve1==e1->ve1)
	    {
	      x = m->ve[ve1].x + e1->att_double * (m->ve[ve2].x - m->ve[ve1].x);
	      y = m->ve[ve1].y + e1->att_double * (m->ve[ve2].y - m->ve[ve1].y);
	      z = m->ve[ve1].z + e1->att_double * (m->ve[ve2].z - m->ve[ve1].z);
	      
	      nvpt[ve4].x = x;
	      nvpt[ve4].y = y;
	      nvpt[ve4].z = z;
	    }
	  //sinon on parcourt l'arete dans le mauvais sens et on ne
	  //fait rien car on l'a deja parcouru dans le bon sens avant
	}
      if (nbfacesupp == 2)
	{
	  ve4 = e1->att_int + m->nbvertex;
	  ve5 = e2->att_int + m->nbvertex;
	  if (e3->att_int != -1)
	    {
	      if (e2->att_int == -1)
		{
		  ve1 = m->fa[i].ve3;
		  ve2 = m->fa[i].ve1;
		  ve3 = m->fa[i].ve2;
		  e2 = e1;
		  e1 = e3;
		  ve4 = e1->att_int + m->nbvertex;
		  ve5 = e2->att_int + m->nbvertex;
		}
	      else
		{
		  ve1 = m->fa[i].ve2;
		  ve2 = m->fa[i].ve3;
		  ve3 = m->fa[i].ve1;
		  e1 = e2;
		  e2 = e3;
		  ve4 = e1->att_int + m->nbvertex;
		  ve5 = e2->att_int + m->nbvertex;
		}
	    }
	  nvfa[nbfacecourant].ve1 = ve1;
	  nvfa[nbfacecourant].ve2 = ve4;
	  nvfa[nbfacecourant].ve3 = ve5;
	  nbfacecourant++;
	  nvfa[nbfacecourant].ve1 = ve4;
	  nvfa[nbfacecourant].ve2 = ve2;
	  nvfa[nbfacecourant].ve3 = ve5;
	  nbfacecourant++;
	  nvfa[nbfacecourant].ve1 = ve5;
	  nvfa[nbfacecourant].ve2 = ve3;
	  nvfa[nbfacecourant].ve3 = ve1;
	  nbfacecourant++;

	  list_int_add (&(nvpt[ve1].incidentvertices),
			&(nvpt[ve1].nbincidentvertices), ve4,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve4].incidentvertices),
			&(nvpt[ve4].nbincidentvertices), ve1,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve1].incidentvertices),
			&(nvpt[ve1].nbincidentvertices), ve3,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve3].incidentvertices),
			&(nvpt[ve3].nbincidentvertices), ve1,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve1].incidentvertices),
			&(nvpt[ve1].nbincidentvertices), ve5,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve5].incidentvertices),
			&(nvpt[ve5].nbincidentvertices), ve1,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve4].incidentvertices),
			&(nvpt[ve4].nbincidentvertices), ve5,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve5].incidentvertices),
			&(nvpt[ve5].nbincidentvertices), ve4,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve2].incidentvertices),
			&(nvpt[ve2].nbincidentvertices), ve4,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve4].incidentvertices),
			&(nvpt[ve4].nbincidentvertices), ve2,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve2].incidentvertices),
			&(nvpt[ve2].nbincidentvertices), ve5,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve5].incidentvertices),
			&(nvpt[ve5].nbincidentvertices), ve2,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve3].incidentvertices),
			&(nvpt[ve3].nbincidentvertices), ve5,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve5].incidentvertices),
			&(nvpt[ve5].nbincidentvertices), ve3,
			WITH_REDUNDANCE);

	  if(ve1==e1->ve1)
	    {
	      x = m->ve[ve1].x + e1->att_double * (m->ve[ve2].x - m->ve[ve1].x);
	      y = m->ve[ve1].y + e1->att_double * (m->ve[ve2].y - m->ve[ve1].y);
	      z = m->ve[ve1].z + e1->att_double * (m->ve[ve2].z - m->ve[ve1].z);
	      
	      nvpt[ve4].x = x;
	      nvpt[ve4].y = y;
	      nvpt[ve4].z = z;
	    }
	  //sinon on parcourt l'arete dans le mauvais sens et on ne
	  //fait rien car on l'a deja parcouru dans le bon sens avant

	  if(ve2==e2->ve1)
	    {
	      x = m->ve[ve2].x + e2->att_double * (m->ve[ve3].x - m->ve[ve2].x);
	      y = m->ve[ve2].y + e2->att_double * (m->ve[ve3].y - m->ve[ve2].y);
	      z = m->ve[ve2].z + e2->att_double * (m->ve[ve3].z - m->ve[ve2].z);
	      
	      nvpt[ve5].x = x;
	      nvpt[ve5].y = y;
	      nvpt[ve5].z = z;
	    }
	  //sinon on parcourt l'arete dans le mauvais sens et on ne
	  //fait rien car on l'a deja parcouru dans le bon sens avant
	}
      if (nbfacesupp == 3)
	{
	  ve4 = e1->att_int + m->nbvertex;
	  ve5 = e2->att_int + m->nbvertex;
	  ve6 = e3->att_int + m->nbvertex;

	  nvfa[nbfacecourant].ve1 = ve1;
	  nvfa[nbfacecourant].ve2 = ve4;
	  nvfa[nbfacecourant].ve3 = ve6;
	  nbfacecourant++;
	  nvfa[nbfacecourant].ve1 = ve4;
	  nvfa[nbfacecourant].ve2 = ve2;
	  nvfa[nbfacecourant].ve3 = ve5;
	  nbfacecourant++;
	  nvfa[nbfacecourant].ve1 = ve5;
	  nvfa[nbfacecourant].ve2 = ve3;
	  nvfa[nbfacecourant].ve3 = ve6;
	  nbfacecourant++;
	  nvfa[nbfacecourant].ve1 = ve6;
	  nvfa[nbfacecourant].ve2 = ve4;
	  nvfa[nbfacecourant].ve3 = ve5;
	  nbfacecourant++;

	  list_int_add (&(nvpt[ve1].incidentvertices),
			&(nvpt[ve1].nbincidentvertices), ve4,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve4].incidentvertices),
			&(nvpt[ve4].nbincidentvertices), ve1,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve1].incidentvertices),
			&(nvpt[ve1].nbincidentvertices), ve6,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve6].incidentvertices),
			&(nvpt[ve6].nbincidentvertices), ve1,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve4].incidentvertices),
			&(nvpt[ve4].nbincidentvertices), ve5,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve5].incidentvertices),
			&(nvpt[ve5].nbincidentvertices), ve4,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve4].incidentvertices),
			&(nvpt[ve4].nbincidentvertices), ve6,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve6].incidentvertices),
			&(nvpt[ve6].nbincidentvertices), ve4,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve2].incidentvertices),
			&(nvpt[ve2].nbincidentvertices), ve4,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve4].incidentvertices),
			&(nvpt[ve4].nbincidentvertices), ve2,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve2].incidentvertices),
			&(nvpt[ve2].nbincidentvertices), ve5,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve5].incidentvertices),
			&(nvpt[ve5].nbincidentvertices), ve2,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve3].incidentvertices),
			&(nvpt[ve3].nbincidentvertices), ve5,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve5].incidentvertices),
			&(nvpt[ve5].nbincidentvertices), ve3,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve6].incidentvertices),
			&(nvpt[ve6].nbincidentvertices), ve5,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve5].incidentvertices),
			&(nvpt[ve5].nbincidentvertices), ve6,
			WITH_REDUNDANCE);

	  list_int_add (&(nvpt[ve3].incidentvertices),
			&(nvpt[ve3].nbincidentvertices), ve6,
			WITH_REDUNDANCE);
	  list_int_add (&(nvpt[ve6].incidentvertices),
			&(nvpt[ve6].nbincidentvertices), ve3,
			WITH_REDUNDANCE);

	  if(ve1==e1->ve1)
	    {
	      x = m->ve[ve1].x + e1->att_double * (m->ve[ve2].x - m->ve[ve1].x);
	      y = m->ve[ve1].y + e1->att_double * (m->ve[ve2].y - m->ve[ve1].y);
	      z = m->ve[ve1].z + e1->att_double * (m->ve[ve2].z - m->ve[ve1].z);
	      
	      nvpt[ve4].x = x;
	      nvpt[ve4].y = y;
	      nvpt[ve4].z = z;
	    }
	  //sinon on parcourt l'arete dans le mauvais sens et on ne
	  //fait rien car on l'a deja parcouru dans le bon sens avant
	  
	  if(ve2==e2->ve1)
	    {
	      x = m->ve[ve2].x + e2->att_double * (m->ve[ve3].x - m->ve[ve2].x);
	      y = m->ve[ve2].y + e2->att_double * (m->ve[ve3].y - m->ve[ve2].y);
	      z = m->ve[ve2].z + e2->att_double * (m->ve[ve3].z - m->ve[ve2].z);
	      
	      nvpt[ve5].x = x;
	      nvpt[ve5].y = y;
	      nvpt[ve5].z = z;
	    }
	  //sinon on parcourt l'arete dans le mauvais sens et on ne
	  //fait rien car on l'a deja parcouru dans le bon sens avant
	  	  
	  if(ve3==e3->ve1)
	    {
	      x = m->ve[ve3].x + e3->att_double * (m->ve[ve1].x - m->ve[ve3].x);
	      y = m->ve[ve3].y + e3->att_double * (m->ve[ve1].y - m->ve[ve3].y);
	      z = m->ve[ve3].z + e3->att_double * (m->ve[ve1].z - m->ve[ve3].z);
	      
	      nvpt[ve6].x = x;
	      nvpt[ve6].y = y;
	      nvpt[ve6].z = z;
	    }
	  //sinon on parcourt l'arete dans le mauvais sens et on ne
	  //fait rien car on l'a deja parcouru dans le bon sens avant
	}
    }

  for (int i = 0; i < m->nbvertex; i++)
    vf_vertex_free (&(m->ve[i]));

  free (m->ve);
  free (m->fa);


  m->nbface = nbfacecourant;
  m->nbvertex = m->nbvertex + arg->att_int;
  m->fa = nvfa;
  m->ve = nvpt;

  free (arg);

}


/**
   Subdivision d'un modèle avec la méthode de Loop
   @param m pointeur sur le modèle à subdiviser
   @param nbiter nombre d'itération de la subdivision
   @return aucun
*/
void
a2ri_vef_loop (
	       vef_model * m,
	       int nbiter)
{
  int p1,
    p2,
    p3,
    p4,
    artemp1,
    artemp2;
  int indexarete;
  for (int i = 0; i < nbiter; i++)
    {
      int nbpointancien = m->nbvertex;

      //on crée les nouveaux points milieux des aretes avec des coordonnées temporaires
      for (int j = 0; j < m->nbedge; j++)
	{
	  p1 = m->ed[j].ve1;
	  p2 = m->ed[j].ve2;

	  double x = 0.5 * (m->ve[p1].x + m->ve[p2].x);
	  double y = 0.5 * (m->ve[p1].y + m->ve[p2].y);
	  double z = 0.5 * (m->ve[p1].z + m->ve[p2].z);
	  a2ri_vef_add_vertex (m, x, y, z);
	}

      //creation des nouveaux tableaux de sommets, aretes et faces
      vef_vertex *nvpt =
	(vef_vertex *) malloc (m->nbvertex * sizeof (vef_vertex));
      a2ri_erreur_critique_si (nvpt == NULL,
			    "erreur allocation memoire pour nvpt\na2ri_vef_loop");
      vef_edge *nvar =
	(vef_edge *) malloc ((m->nbedge * 2 + m->nbface * 3) *
			     sizeof (vef_edge));
      a2ri_erreur_critique_si (nvar == NULL,
			    "erreur allocation memoire pour nvar\na2ri_vef_loop");
      vef_face *nvfa =
	(vef_face *) malloc (m->nbface * 4 * sizeof (vef_face));
      a2ri_erreur_critique_si (nvfa == NULL,
			    "erreur allocation memoire pour nvfa\na2ri_vef_loop");

      //initialisation des tableaux de sommets et arete
      for (int j = 0; j < m->nbvertex; j++)
	{
	  nvpt[j].nbsharededges = 0;
	  nvpt[j].sharededges = NULL;
	}
      for (int j = 0; j < m->nbedge * 2 + m->nbface * 3; j++)
	{
	  nvar[j].nbsharedfaces = 0;
	  nvar[j].sharedfaces = NULL;
	}



      //pour toutes les aretes
      for (int j = 0; j < m->nbedge; j++)
	{
	  //découpage des aretes en deux avec le nouveau point milieu d'arete (d'index : nombre d'ancien point + index de l'arete)
	  nvar[j * 2].ve1 = m->ed[j].ve1;
	  nvar[j * 2].ve2 = nbpointancien + j;
	  list_int_add (&(nvpt[m->ed[j].ve1].sharededges),
			&(nvpt[m->ed[j].ve1].nbsharededges),
			j * 2, WITH_REDUNDANCE);
	  list_int_add (&(nvpt[nbpointancien + j].sharededges),
			&(nvpt[nbpointancien + j].nbsharededges),
			j * 2, WITH_REDUNDANCE);

	  nvar[j * 2 + 1].ve1 = m->ed[j].ve2;
	  nvar[j * 2 + 1].ve2 = nbpointancien + j;
	  list_int_add (&(nvpt[m->ed[j].ve2].sharededges),
			&(nvpt[m->ed[j].ve2].nbsharededges),
			j * 2 + 1, WITH_REDUNDANCE);
	  list_int_add (&(nvpt[nbpointancien + j].sharededges),
			&(nvpt[nbpointancien + j].nbsharededges),
			j * 2 + 1, WITH_REDUNDANCE);
	}

      indexarete = m->nbedge * 2;

      //pour toutes les faces du modele
      for (int j = 0; j < m->nbface; j++)
	{

	  int ar1 = m->fa[j].ed1;
	  int ar2 = m->fa[j].ed2;
	  int ar3 = m->fa[j].ed3;

	  int sommet;

	  //recherche du sommet commun entre ar1 et ar2
	  if (m->ed[ar1].ve1 == m->ed[ar2].ve1
	      || m->ed[ar1].ve1 == m->ed[ar2].ve2)
	    sommet = m->ed[ar1].ve1;
	  else
	    sommet = m->ed[ar1].ve2;

	  //on ajoute la nouvelle arete entre les deux points milieux
	  nvar[indexarete].ve1 = nbpointancien + ar1;
	  nvar[indexarete].ve2 = nbpointancien + ar2;
	  list_int_add (&(nvpt[nbpointancien + ar1].sharededges),
			&(nvpt[nbpointancien + ar1].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  list_int_add (&(nvpt[nbpointancien + ar2].sharededges),
			&(nvpt[nbpointancien + ar2].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  indexarete++;



	  // on recupere la premiere "demi arete" de l'ancienne arete1 contenant le sommet commun
	  if (nvar[ar1 * 2].ve1 == sommet || nvar[ar1 * 2].ve2 == sommet)
	    artemp1 = ar1 * 2;
	  else
	    artemp1 = (ar1 * 2) + 1;

	  // on recupere la premiere "demi arete" de l'ancienne arete2 contenant le sommet commun
	  if (nvar[ar2 * 2].ve1 == sommet || nvar[ar2 * 2].ve2 == sommet)
	    artemp2 = ar2 * 2;
	  else
	    artemp2 = (ar2 * 2) + 1;

	  //on ajoute la face contenant les deux demi aretes et la nouvelle arete
	  nvfa[j * 4].ed1 = indexarete - 1;
	  nvfa[j * 4].ed2 = artemp1;
	  nvfa[j * 4].ed3 = artemp2;
	  list_int_add (&(nvar[indexarete - 1].sharedfaces),
			&(nvar[indexarete - 1].nbsharedfaces),
			j * 4, WITH_REDUNDANCE);
	  list_int_add (&(nvar[artemp1].sharedfaces),
			&(nvar[artemp1].nbsharedfaces),
			j * 4, WITH_REDUNDANCE);
	  list_int_add (&(nvar[artemp2].sharedfaces),
			&(nvar[artemp2].nbsharedfaces),
			j * 4, WITH_REDUNDANCE);





	  //on repete l'operation deux fois encore


	  if (m->ed[ar2].ve1 == m->ed[ar3].ve1
	      || m->ed[ar2].ve1 == m->ed[ar3].ve2)
	    sommet = m->ed[ar2].ve1;
	  else
	    sommet = m->ed[ar2].ve2;

	  nvar[indexarete].ve1 = nbpointancien + ar2;
	  nvar[indexarete].ve2 = nbpointancien + ar3;
	  list_int_add (&(nvpt[nbpointancien + ar2].sharededges),
			&(nvpt[nbpointancien + ar2].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  list_int_add (&(nvpt[nbpointancien + ar3].sharededges),
			&(nvpt[nbpointancien + ar3].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  indexarete++;



	  if (nvar[ar2 * 2].ve1 == sommet || nvar[ar2 * 2].ve2 == sommet)
	    artemp1 = ar2 * 2;
	  else
	    artemp1 = (ar2 * 2) + 1;

	  if (nvar[ar3 * 2].ve1 == sommet || nvar[ar3 * 2].ve2 == sommet)
	    artemp2 = ar3 * 2;
	  else
	    artemp2 = (ar3 * 2) + 1;

	  nvfa[(j * 4) + 1].ed1 = indexarete - 1;
	  nvfa[(j * 4) + 1].ed2 = artemp1;
	  nvfa[(j * 4) + 1].ed3 = artemp2;
	  list_int_add (&(nvar[indexarete - 1].sharedfaces),
			&(nvar[indexarete - 1].nbsharedfaces),
			j * 4 + 1, WITH_REDUNDANCE);
	  list_int_add (&(nvar[artemp1].sharedfaces),
			&(nvar[artemp1].nbsharedfaces),
			j * 4 + 1, WITH_REDUNDANCE);
	  list_int_add (&(nvar[artemp2].sharedfaces),
			&(nvar[artemp2].nbsharedfaces),
			j * 4 + 1, WITH_REDUNDANCE);






	  if (m->ed[ar1].ve1 == m->ed[ar3].ve1
	      || m->ed[ar1].ve1 == m->ed[ar3].ve2)
	    sommet = m->ed[ar1].ve1;
	  else
	    sommet = m->ed[ar1].ve2;

	  nvar[indexarete].ve1 = nbpointancien + ar1;
	  nvar[indexarete].ve2 = nbpointancien + ar3;
	  list_int_add (&(nvpt[nbpointancien + ar1].sharededges),
			&(nvpt[nbpointancien + ar1].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  list_int_add (&(nvpt[nbpointancien + ar3].sharededges),
			&(nvpt[nbpointancien + ar3].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  indexarete++;




	  if (nvar[ar1 * 2].ve1 == sommet || nvar[ar1 * 2].ve2 == sommet)
	    artemp1 = ar1 * 2;
	  else
	    artemp1 = (ar1 * 2) + 1;
	  if (nvar[ar3 * 2].ve1 == sommet || nvar[ar3 * 2].ve2 == sommet)
	    artemp2 = ar3 * 2;
	  else
	    artemp2 = (ar3 * 2) + 1;

	  nvfa[(j * 4) + 2].ed1 = indexarete - 1;
	  nvfa[(j * 4) + 2].ed2 = artemp2;
	  nvfa[(j * 4) + 2].ed3 = artemp1;
	  list_int_add (&(nvar[indexarete - 1].sharedfaces),
			&(nvar[indexarete - 1].nbsharedfaces),
			j * 4 + 2, WITH_REDUNDANCE);
	  list_int_add (&(nvar[artemp1].sharedfaces),
			&(nvar[artemp1].nbsharedfaces),
			j * 4 + 2, WITH_REDUNDANCE);
	  list_int_add (&(nvar[artemp2].sharedfaces),
			&(nvar[artemp2].nbsharedfaces),
			j * 4 + 2, WITH_REDUNDANCE);





	  //on ajoute une nouvelles face composé des trois nouvelles aretes
	  nvfa[(j * 4) + 3].ed1 = indexarete - 1;
	  nvfa[(j * 4) + 3].ed2 = indexarete - 3;
	  nvfa[(j * 4) + 3].ed3 = indexarete - 2;
	  list_int_add (&(nvar[indexarete - 1].sharedfaces),
			&(nvar[indexarete - 1].nbsharedfaces),
			j * 4 + 3, WITH_REDUNDANCE);
	  list_int_add (&(nvar[indexarete - 2].sharedfaces),
			&(nvar[indexarete - 2].nbsharedfaces),
			j * 4 + 3, WITH_REDUNDANCE);
	  list_int_add (&(nvar[indexarete - 3].sharedfaces),
			&(nvar[indexarete - 3].nbsharedfaces),
			j * 4 + 3, WITH_REDUNDANCE);



	}

      //repositionnement des anciens sommets
      for (int j = 0; j < nbpointancien; j++)
	{
	  int bord = 0;
	  int mpt1 = -1;
	  int mpt2 = -1;
	  //on regarde sur le sommet appartient à une arete ne partageant qu'une seule face
	  for (int k = 0; k < m->ve[j].nbsharededges; k++)
	    if (m->ed[m->ve[j].sharededges[k]].nbsharedfaces == 1)
	      {
		//dans ce cas le point est sur le bord du maillage
		bord++;
		//on recherche les deux sommets qui serviront à son repositionnement
		if (mpt1 == -1)
		  if (m->ed[m->ve[j].sharededges[k]].ve1 == j)
		    mpt1 = m->ed[m->ve[j].sharededges[k]].ve2;
		  else
		    mpt1 = m->ed[m->ve[j].sharededges[k]].ve1;
		else if (m->ed[m->ve[j].sharededges[k]].ve1 == j)
		  mpt2 = m->ed[m->ve[j].sharededges[k]].ve2;
		else
		  mpt2 = m->ed[m->ve[j].sharededges[k]].ve1;
	      }

	  if (bord == 0)
	    {
	      //dans le cas d'un sommet intérieur au maillage
	      double alphan =
		(1.0 / 64.0) * (40 -
				(3 +
				 2 * cos (2 * M_PI /
					  m->ve[j].nbsharededges)) * (3 +
								      2 *
								      cos (2 *
									   M_PI
									   /
									   m->
									   ve
									   [j].
									   nbsharededges)));
	      double centre = 1.0 - alphan;
	      double b = alphan / m->ve[j].nbsharededges;
	      int *looploi1 = NULL;
	      int nbeltlooploi1 = 0;
	      for (int k = 0; k < m->ve[j].nbsharededges; k++)
		if (m->ed[m->ve[j].sharededges[k]].ve1 == j)
		  list_int_add (&looploi1, &nbeltlooploi1,
				(m->ed[m->ve[j].sharededges[k]].ve2),
				WITH_REDUNDANCE);
		else
		  list_int_add (&looploi1, &nbeltlooploi1,
				(m->ed[m->ve[j].sharededges[k]].ve1),
				WITH_REDUNDANCE);

	      //on calcule les nouvelles coordonnées du sommet
	      double x = centre * m->ve[j].x;
	      double y = centre * m->ve[j].y;
	      double z = centre * m->ve[j].z;
	      for (int k = 0; k < m->ve[j].nbsharededges; k++)
		{
		  x += b * m->ve[looploi1[k]].x;
		  y += b * m->ve[looploi1[k]].y;
		  z += b * m->ve[looploi1[k]].z;
		}
	      //qu'on place dans le nouveau tableau
	      nvpt[j].x = x;
	      nvpt[j].y = y;
	      nvpt[j].z = z;
	      //mis à jour de la bounding box
	      if (j == 0)
		{
		  m->xmin = x;
		  m->xmax = x;
		  m->ymin = y;
		  m->ymax = y;
		  m->zmin = z;
		  m->zmax = z;
		}
	      else
		{
		  if (m->xmin > x)
		    m->xmin = x;
		  if (m->xmax < x)
		    m->xmax = x;
		  if (m->ymin > y)
		    m->ymin = y;
		  if (m->ymax < y)
		    m->ymax = y;
		  if (m->zmin > z)
		    m->zmin = z;
		  if (m->zmax < z)
		    m->zmax = z;
		}
	      free (looploi1);
	    }
	  else
	    {
	      //dans le cas d'un point sur le bord du maillage
	      //on calcule les nouvelles coordonnées du sommet
	      double x =
		(1.0 / 8.0) * (m->ve[mpt1].x + m->ve[mpt2].x) +
		(6.0 / 8.0) * m->ve[j].x;
	      double y =
		(1.0 / 8.0) * (m->ve[mpt1].y + m->ve[mpt2].y) +
		(6.0 / 8.0) * m->ve[j].y;
	      double z =
		(1.0 / 8.0) * (m->ve[mpt1].z + m->ve[mpt2].z) +
		(6.0 / 8.0) * m->ve[j].z;
	      //qu'on place dans le nouveau tableau
	      nvpt[j].x = x;
	      nvpt[j].y = y;
	      nvpt[j].z = z;
	      //on met à jour la bounding box
	      if (j == 0)
		{
		  m->xmin = x;
		  m->xmax = x;
		  m->ymin = y;
		  m->ymax = y;
		  m->zmin = z;
		  m->zmax = z;
		}
	      else
		{
		  if (m->xmin > x)
		    m->xmin = x;
		  if (m->xmax < x)
		    m->xmax = x;
		  if (m->ymin > y)
		    m->ymin = y;
		  if (m->ymax < y)
		    m->ymax = y;
		  if (m->zmin > z)
		    m->zmin = z;
		  if (m->zmax < z)
		    m->zmax = z;
		}
	    }
	}


      //pour les nouveaux sommets
      for (int j = 0; j < m->nbvertex - nbpointancien; j++)
	{
	  //on récupère les sommets servant au repositionnement
	  p1 = m->ed[j].ve1;
	  p2 = m->ed[j].ve2;
	  p3 = -1;
	  p4 = -1;
	  if (m->ed[j].nbsharedfaces == 2)
	    {
	      //sommet intérieur au maillage
	      //p3 et p4 sont les deux sommets appartenant aux deux faces partagées par l'arete mais n'étant pas sur l'arete
	      int f1 = m->ed[j].sharedfaces[0];
	      int f2 = m->ed[j].sharedfaces[1];
	      int ar1 = m->fa[f1].ed1;
	      if (ar1 == j)
		ar1 = m->fa[f1].ed2;
	      int ar2 = m->fa[f2].ed1;
	      if (ar2 == j)
		ar2 = m->fa[f2].ed2;
	      p3 = m->ed[ar1].ve1;
	      if (p3 == p1 || p3 == p2)
		p3 = m->ed[ar1].ve2;
	      p4 = m->ed[ar2].ve1;
	      if (p4 == p1 || p4 == p2)
		p4 = m->ed[ar2].ve2;
	      //on calcule les nuvelles coordonnées du sommet
	      double x =
		(3.0 / 8.0) * (m->ve[p1].x + m->ve[p2].x) +
		(1.0 / 8.0) * (m->ve[p3].x + m->ve[p4].x);
	      double y =
		(3.0 / 8.0) * (m->ve[p1].y + m->ve[p2].y) +
		(1.0 / 8.0) * (m->ve[p3].y + m->ve[p4].y);
	      double z =
		(3.0 / 8.0) * (m->ve[p1].z + m->ve[p2].z) +
		(1.0 / 8.0) * (m->ve[p3].z + m->ve[p4].z);
	      //qu'on place dans le nuveau tableau
	      nvpt[nbpointancien + j].x = x;
	      nvpt[nbpointancien + j].y = y;
	      nvpt[nbpointancien + j].z = z;
	      //mis à jour de la bounding box
	      if (m->xmin > x)
		m->xmin = x;
	      if (m->xmax < x)
		m->xmax = x;
	      if (m->ymin > y)
		m->ymin = y;
	      if (m->ymax < y)
		m->ymax = y;
	      if (m->zmin > z)
		m->zmin = z;
	      if (m->zmax < z)
		m->zmax = z;
	    }
	  else
	    {
	      //calcul de la nouvelles position du nouveau point (sommet sur le bord du maillage)
	      double x = 0.5 * (m->ve[p1].x + m->ve[p2].x);
	      double y = 0.5 * (m->ve[p1].y + m->ve[p2].y);
	      double z = 0.5 * (m->ve[p1].z + m->ve[p2].z);
	      //on met ces coordonnées dans le nouveau tableau
	      nvpt[nbpointancien + j].x = x;
	      nvpt[nbpointancien + j].y = y;
	      nvpt[nbpointancien + j].z = z;
	      //mis à jour de la bounding box
	      if (m->xmin > x)
		m->xmin = x;
	      if (m->xmax < x)
		m->xmax = x;
	      if (m->ymin > y)
		m->ymin = y;
	      if (m->ymax < y)
		m->ymax = y;
	      if (m->zmin > z)
		m->zmin = z;
	      if (m->zmax < z)
		m->zmax = z;
	    }
	}


      for (int j = 0; j < m->nbvertex; j++)
	vef_vertex_free (&(m->ve[j]));

      for (int j = 0; j < m->nbedge; j++)
	vef_edge_free (&(m->ed[j]));

      free (m->ve);
      free (m->ed);
      free (m->fa);

      //on remplace les tableaux de sommets, aretes et faces
      m->fa = nvfa;
      m->ed = nvar;
      m->ve = nvpt;

      //on met à jour les nombre d'aretes et de faces
      m->nbedge = 2 * m->nbedge + 3 * m->nbface;
      m->nbface = m->nbface * 4;
    }

}

/**
   Subdivision d'un modèle avec la méthode de Butterfly
   @param m pointeur sur le modèle à subdiviser
   @param nbiter nombre d'itération de la subdivision
   @param tension paramètre de tension
   @return aucun
*/
void
a2ri_vef_butterfly (
		    vef_model * m,
		    int nbiter,
		    double tension)
{
  int p1,
    p2,
    p3,
    p4,
    p5,
    p6,
    p7,
    p8,
    artemp1,
    artemp2,
    artemp3,
    artemp4;

  for (int i = 0; i < nbiter; i++)
    {
      int nbpointancien = m->nbvertex;


      for (int j = 0; j < m->nbedge; j++)
	{

	  p1 = -1;
	  p2 = -1;
	  p3 = -1;
	  p4 = -1;
	  p5 = -1;
	  p6 = -1;
	  p7 = -1;
	  p8 = -1;
	  IFvef_but_recherche_p1_p2 (m, j, &p1, &p2);
	  IFvef_but_recherche_p3_p4 (m, j, &p3, &p4, &artemp1, &artemp2,
				     &artemp3, &artemp4);
	  IFvef_but_recherche_p5_p6_p7_p8 (m, artemp1, artemp2, artemp3,
					   artemp4, &p5, &p6, &p7, &p8);
	  double x,
	    y,
	    z;
	  x = 0;
	  y = 0;
	  z = 0;
	  if (p1 != -1)
	    {
	      x += 0.5 * m->ve[p1].x;
	      y += 0.5 * m->ve[p1].y;
	      z += 0.5 * m->ve[p1].z;
	    }
	  if (p2 != -1)
	    {
	      x += 0.5 * m->ve[p2].x;
	      y += 0.5 * m->ve[p2].y;
	      z += 0.5 * m->ve[p2].z;
	    }
	  if (p3 != -1)
	    {
	      x += 2 * tension * m->ve[p3].x;
	      y += 2 * tension * m->ve[p3].y;
	      z += 2 * tension * m->ve[p3].z;
	    }
	  if (p4 != -1)
	    {
	      x += 2 * tension * m->ve[p4].x;
	      y += 2 * tension * m->ve[p4].y;
	      z += 2 * tension * m->ve[p4].z;
	    }
	  if (p5 != -1)
	    {
	      x -= tension * m->ve[p5].x;
	      y -= tension * m->ve[p5].y;
	      z -= tension * m->ve[p5].z;
	    }
	  if (p6 != -1)
	    {
	      x -= tension * m->ve[p6].x;
	      y -= tension * m->ve[p6].y;
	      z -= tension * m->ve[p6].z;
	    }
	  if (p7 != -1)
	    {
	      x -= tension * m->ve[p7].x;
	      y -= tension * m->ve[p7].y;
	      z -= tension * m->ve[p7].z;
	    }
	  if (p8 != -1)
	    {
	      x -= tension * m->ve[p8].x;
	      y -= tension * m->ve[p8].y;
	      z -= tension * m->ve[p8].z;
	    }
	  a2ri_vef_add_vertex (m, x, y, z);
	  if (x < m->xmin)
	    m->xmin = x;
	  if (x > m->xmax)
	    m->xmax = x;
	  if (y < m->ymin)
	    m->ymin = y;
	  if (y > m->ymax)
	    m->ymax = y;
	  if (z < m->zmin)
	    m->zmin = z;
	  if (z > m->zmax)
	    m->zmax = z;
	}


      vef_edge *nvar =
	(vef_edge *) malloc (((m->nbedge * 2) + (m->nbface * 3)) *
			     sizeof (vef_edge));
      a2ri_erreur_critique_si (nvar == NULL,
			    "erreur allocation memoire pour nvar\na2ri_vef_butterfly");
      vef_face *nvfa =
	(vef_face *) malloc ((m->nbface * 4) * sizeof (vef_face));
      a2ri_erreur_critique_si (nvfa == NULL,
			    "erreur allocation memoire pour nvfa\na2ri_vef_butterfly");

      for (int i = 0; i < m->nbvertex; i++)
	{
	  free (m->ve[i].sharededges);
	  m->ve[i].sharededges = NULL;
	  m->ve[i].nbsharededges = 0;
	}

      //subdivision

      int indexarete = 0;

      //coupe toutes les anciennes aretes en deux
      for (int j = 0; j < m->nbedge; j++)
	{
	  nvar[indexarete].ve1 = m->ed[j].ve1;
	  nvar[indexarete].ve2 = nbpointancien + j;
	  list_int_add (&(m->ve[m->ed[j].ve1].sharededges),
			&(m->ve[m->ed[j].ve1].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  list_int_add (&(m->ve[nbpointancien + j].sharededges),
			&(m->ve[nbpointancien + j].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  indexarete++;
	  nvar[indexarete].ve1 = m->ed[j].ve2;
	  nvar[indexarete].ve2 = nbpointancien + j;
	  list_int_add (&(m->ve[m->ed[j].ve2].sharededges),
			&(m->ve[m->ed[j].ve2].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  list_int_add (&(m->ve[nbpointancien + j].sharededges),
			&(m->ve[nbpointancien + j].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  indexarete++;
	}


      //pour chaque face
      for (int j = 0; j < m->nbface; j++)
	{
	  int ar1 = m->fa[j].ed1;
	  int ar2 = m->fa[j].ed2;
	  int ar3 = m->fa[j].ed3;

	  int artemp1,
	    artemp2;

	  int sommet;

	  // on recupere le sommet commun a l'arete 1 et 2
	  if (m->ed[ar1].ve1 == m->ed[ar2].ve1
	      || m->ed[ar1].ve1 == m->ed[ar2].ve2)
	    sommet = m->ed[ar1].ve1;
	  else
	    sommet = m->ed[ar1].ve2;

	  // on ajoute une arete entre les points milieu de l'arete 1 et 2 (point milieu qui se trouve a ar1+nbpointancien)
	  nvar[indexarete].ve1 = nbpointancien + ar1;
	  nvar[indexarete].ve2 = nbpointancien + ar2;
	  list_int_add (&(m->ve[nbpointancien + ar1].sharededges),
			&(m->ve[nbpointancien + ar1].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  list_int_add (&(m->ve[nbpointancien + ar2].sharededges),
			&(m->ve[nbpointancien + ar2].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  indexarete++;

	  // on recupere la premiere "demi arete" de l'ancienne arete1 contenant le sommet commun
	  if (nvar[ar1 * 2].ve1 == sommet || nvar[ar1 * 2].ve2 == sommet)
	    artemp1 = ar1 * 2;
	  else
	    artemp1 = (ar1 * 2) + 1;
	  // on recupere la premiere "demi arete" de l'ancienne arete2 contenant le sommet commun
	  if (nvar[ar2 * 2].ve1 == sommet || nvar[ar2 * 2].ve2 == sommet)
	    artemp2 = ar2 * 2;
	  else
	    artemp2 = (ar2 * 2) + 1;
	  //on ajoute la face contenant les deux demi aretes et la nouvelle arete
	  nvfa[j * 4].ed1 = indexarete - 1;
	  nvfa[j * 4].ed2 = artemp1;
	  nvfa[j * 4].ed3 = artemp2;
	  list_int_add (&(nvar[indexarete - 1].sharedfaces),
			&(nvar[indexarete - 1].nbsharedfaces),
			j * 4, WITH_REDUNDANCE);
	  list_int_add (&(nvar[artemp1].sharedfaces),
			&(nvar[artemp1].nbsharedfaces),
			j * 4, WITH_REDUNDANCE);
	  list_int_add (&(nvar[artemp2].sharedfaces),
			&(nvar[artemp2].nbsharedfaces),
			j * 4, WITH_REDUNDANCE);


	  //on repete l'operation deux fois encore


	  if (m->ed[ar2].ve1 == m->ed[ar3].ve1
	      || m->ed[ar2].ve1 == m->ed[ar3].ve2)
	    sommet = m->ed[ar2].ve1;
	  else
	    sommet = m->ed[ar2].ve2;

	  nvar[indexarete].ve1 = nbpointancien + ar2;
	  nvar[indexarete].ve2 = nbpointancien + ar3;
	  list_int_add (&(m->ve[nbpointancien + ar2].sharededges),
			&(m->ve[nbpointancien + ar2].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  list_int_add (&(m->ve[nbpointancien + ar3].sharededges),
			&(m->ve[nbpointancien + ar3].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  indexarete++;

	  if (nvar[ar2 * 2].ve1 == sommet || nvar[ar2 * 2].ve2 == sommet)
	    artemp1 = ar2 * 2;
	  else
	    artemp1 = (ar2 * 2) + 1;
	  if (nvar[ar3 * 2].ve1 == sommet || nvar[ar3 * 2].ve2 == sommet)
	    artemp2 = ar3 * 2;
	  else
	    artemp2 = (ar3 * 2) + 1;

	  nvfa[j * 4 + 1].ed1 = indexarete - 1;
	  nvfa[j * 4 + 1].ed2 = artemp1;
	  nvfa[j * 4 + 1].ed3 = artemp2;
	  list_int_add (&(nvar[indexarete - 1].sharedfaces),
			&(nvar[indexarete - 1].nbsharedfaces),
			j * 4 + 1, WITH_REDUNDANCE);
	  list_int_add (&(nvar[artemp1].sharedfaces),
			&(nvar[artemp1].nbsharedfaces),
			j * 4 + 1, WITH_REDUNDANCE);
	  list_int_add (&(nvar[artemp2].sharedfaces),
			&(nvar[artemp2].nbsharedfaces),
			j * 4 + 1, WITH_REDUNDANCE);




	  if (m->ed[ar1].ve1 == m->ed[ar3].ve1
	      || m->ed[ar1].ve1 == m->ed[ar3].ve2)
	    sommet = m->ed[ar1].ve1;
	  else
	    sommet = m->ed[ar1].ve2;

	  nvar[indexarete].ve1 = nbpointancien + ar1;
	  nvar[indexarete].ve2 = nbpointancien + ar3;
	  list_int_add (&(m->ve[nbpointancien + ar1].sharededges),
			&(m->ve[nbpointancien + ar1].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  list_int_add (&(m->ve[nbpointancien + ar3].sharededges),
			&(m->ve[nbpointancien + ar3].nbsharededges),
			indexarete, WITH_REDUNDANCE);
	  indexarete++;

	  if (nvar[ar1 * 2].ve1 == sommet || nvar[ar1 * 2].ve2 == sommet)
	    artemp1 = ar1 * 2;
	  else
	    artemp1 = (ar1 * 2) + 1;
	  if (nvar[ar3 * 2].ve1 == sommet || nvar[ar3 * 2].ve2 == sommet)
	    artemp2 = ar3 * 2;
	  else
	    artemp2 = (ar3 * 2) + 1;

	  nvfa[j * 4 + 2].ed1 = indexarete - 1;
	  nvfa[j * 4 + 2].ed2 = artemp2;
	  nvfa[j * 4 + 2].ed3 = artemp1;
	  list_int_add (&(nvar[indexarete - 1].sharedfaces),
			&(nvar[indexarete - 1].nbsharedfaces),
			j * 4 + 2, WITH_REDUNDANCE);
	  list_int_add (&(nvar[artemp1].sharedfaces),
			&(nvar[artemp1].nbsharedfaces),
			j * 4 + 2, WITH_REDUNDANCE);
	  list_int_add (&(nvar[artemp2].sharedfaces),
			&(nvar[artemp2].nbsharedfaces),
			j * 4 + 2, WITH_REDUNDANCE);


	  //on ajoute une nouvelles face composé des trois nouvelles aretes
	  nvfa[j * 4 + 3].ed1 = indexarete - 1;
	  nvfa[j * 4 + 3].ed2 = indexarete - 3;
	  nvfa[j * 4 + 3].ed3 = indexarete - 2;
	  list_int_add (&(nvar[indexarete - 1].sharedfaces),
			&(nvar[indexarete - 1].nbsharedfaces),
			j * 4 + 3, WITH_REDUNDANCE);
	  list_int_add (&(nvar[indexarete - 2].sharedfaces),
			&(nvar[indexarete - 2].nbsharedfaces),
			j * 4 + 3, WITH_REDUNDANCE);
	  list_int_add (&(nvar[indexarete - 3].sharedfaces),
			&(nvar[indexarete - 3].nbsharedfaces),
			j * 4 + 3, WITH_REDUNDANCE);

	}

      m->nbedge = 2 * m->nbedge + 3 * m->nbface;
      m->nbface *= 4;

      m->fa = nvfa;
      m->ed = nvar;

      for (int j = 0; j < m->nbvertex; j++)
	{
	  if (m->ve[j].x < m->xmin)
	    m->xmin = m->ve[j].x;
	  if (m->ve[j].x > m->xmax)
	    m->xmax = m->ve[j].x;
	  if (m->ve[j].y < m->ymin)
	    m->ymin = m->ve[j].y;
	  if (m->ve[j].y > m->ymax)
	    m->ymax = m->ve[j].y;
	  if (m->ve[j].z < m->zmin)
	    m->zmin = m->ve[j].z;
	  if (m->ve[j].z > m->zmax)
	    m->zmax = m->ve[j].z;

	}

    }
}
