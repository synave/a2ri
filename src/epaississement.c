/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 24/02/09       */
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

#include "epaississement.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

void
IFcomble_bord (
	       int key,
	       vf_edge * value,
	       void *user_data)
{
  if (value->nbsharedfaces != 1)
    return;

  vf_model *m = user_data;
  int ve1 = value->ve1;
  int ve2 = value->ve2;
  int numface = value->sharedfaces[0];
  int nbancienvertex = m->nbvertex / 2;

  if (((m->fa[numface].ve1 == ve1 && m->fa[numface].ve2 == ve2) ||
       (m->fa[numface].ve2 == ve1 && m->fa[numface].ve3 == ve2) ||
       (m->fa[numface].ve3 == ve1 && m->fa[numface].ve1 == ve2)))
    {
      ve1 = value->ve2;
      ve2 = value->ve1;
    }

  a2ri_vf_add_face (m, ve1, ve2, ve1 + nbancienvertex);
  a2ri_vf_add_face (m, ve2, ve2 + nbancienvertex, ve1 + nbancienvertex);
}

/********** MAIN FUNCTIONS **********/


/**
   Epaississement de la surface d'un maillage triangulaire
   @param m le modèle à épaissir
   @return aucun
**/
void
a2ri_vf_epaissi_surface (
			 vf_model * m,
			 double epaisseur)
{
  int nbvertex = m->nbvertex;
  hashtable *edge = a2ri_vf_construction_edge_table (m, NULL, 0);

  for (int i = 0; i < nbvertex; i++)
    {
      point3d p;
      vector3d n;
      int *listface = NULL,
	sizelist = 0;

      vector3d_init (&n, 0, 0, 0);
      point3d_init (&p, m->ve[i].x, m->ve[i].y, m->ve[i].z);

      for (int j = 0; j < m->ve[i].nbincidentvertices; j++)
	{
	  vf_edge *temp =
	    hashtable_look_for (edge, i, m->ve[i].incidentvertices[j]);
	  for (int k = 0; k < temp->nbsharedfaces; k++)
	    list_int_add (&listface, &sizelist, temp->sharedfaces[k],
			  WITHOUT_REDUNDANCE);
	}

      for (int j = 0; j < sizelist; j++)
	{
	  point3d A,
	    B,
	    C;

	  vector3d AB,
	    AC,
	    ntemp;

	  int ve1,
	    ve2,
	    ve3;

	  ve1 = m->fa[listface[j]].ve1;
	  ve2 = m->fa[listface[j]].ve2;
	  ve3 = m->fa[listface[j]].ve3;

	  point3d_init (&A, m->ve[ve1].x, m->ve[ve1].y, m->ve[ve1].z);
	  point3d_init (&B, m->ve[ve2].x, m->ve[ve2].y, m->ve[ve2].z);
	  point3d_init (&C, m->ve[ve3].x, m->ve[ve3].y, m->ve[ve3].z);

	  vector3d_init (&AB, B.x - A.x, B.y - A.y, B.z - A.z);
	  vector3d_init (&AC, C.x - A.x, C.y - A.y, C.z - A.z);
	  ntemp = vector3d_vectorialproduct (&AB, &AC);
	  vector3d_normalize (&ntemp);
	  vector3d_reverse (&ntemp);
	  n.dx += ntemp.dx;
	  n.dy += ntemp.dy;
	  n.dz += ntemp.dz;
	}

      n.dx /= sizelist;
      n.dy /= sizelist;
      n.dz /= sizelist;

      vector3d_normalize (&n);

      n.dx *= epaisseur;
      n.dy *= epaisseur;
      n.dz *= epaisseur;

      p.x += n.dx;
      p.y += n.dy;
      p.z += n.dz;

      a2ri_vf_add_vertex (m, p.x, p.y, p.z);

      free (listface);
    }

  int nbface = m->nbface;

  for (int i = 0; i < nbface; i++)
    a2ri_vf_add_face (m, m->fa[i].ve1 + nbvertex, m->fa[i].ve3 + nbvertex,
		      m->fa[i].ve2 + nbvertex);

  hashtable_foreach (edge, IFcomble_bord, m);

  hashtable_free (edge);
}
