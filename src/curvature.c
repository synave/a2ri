/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 29/05/26       */
/* Date de modification : 29/05/26   */
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

#include "curvature.h"

/**
   Calcul de la courbure gaussienne discrète pour un sommet d'un modèle.
   @param m le modèle
   @param num_sommet le numéro du sommet
   @return la courbure
 */
double a2ri_vf_courbure_gaussienne_discrete(
					    vf_model *m,
					    int num_sommet)
{
  int *liste_face=NULL;
  int size_liste_face=0;

  for(unsigned int i = 0; i < m->nbface; i++)
    {
      if(m->fa[i].ve1 == num_sommet
	 || m->fa[i].ve2 == num_sommet
	 || m->fa[i].ve3 == num_sommet )
	list_int_add(&liste_face, &size_liste_face, i, WITH_REDUNDANCE);
    }

  printf("Le sommet %d est partagé par %d faces :\n", num_sommet, size_liste_face);
  list_int_display(liste_face, size_liste_face);

  double somme_angle_sommet = 0;
  
  for(unsigned int i=0; i < size_liste_face; i++)
    {
      int ve1, ve2, ve3;
      if(m->fa[liste_face[i]].ve1 == num_sommet)
	{
	  ve1 = m->fa[liste_face[i]].ve1;
	  ve2 = m->fa[liste_face[i]].ve2;
	  ve3 = m->fa[liste_face[i]].ve3;
	}
      else
	{
	  if(m->fa[liste_face[i]].ve2 == num_sommet)
	    {
	      ve1 = m->fa[liste_face[i]].ve2;
	      ve2 = m->fa[liste_face[i]].ve1;
	      ve3 = m->fa[liste_face[i]].ve3;
	    }
	  else
	    {
	      ve1 = m->fa[liste_face[i]].ve3;
	      ve2 = m->fa[liste_face[i]].ve1;
	      ve3 = m->fa[liste_face[i]].ve2;
	    }
	}
      printf("face %d : %d %d %d\n", liste_face[i], ve1, ve2, ve3);
      vector3d v1, v2;
      vector3d_init(&v1, m->ve[ve1].x-m->ve[ve2].x, m->ve[ve1].y-m->ve[ve2].y, m->ve[ve1].z-m->ve[ve2].z);
      vector3d_init(&v2, m->ve[ve1].x-m->ve[ve3].x, m->ve[ve1].y-m->ve[ve3].y, m->ve[ve1].z-m->ve[ve3].z);
      
      somme_angle_sommet += vector3d_angle_radian(&v1, &v2);
    }

  return 2*M_PI-somme_angle_sommet;
}
