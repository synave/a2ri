/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 01/10/08       */
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

#include "shapematching.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

/********** MAIN FUNCTIONS **********/

/**
   Calcul de l'empreinte d'un modèle
   @param m le modèle
   @param nb_tranche nombre de tranche de l'empreinte
   @param list liste de retour de l'empreinte
   @return aucun
**/
void
a2ri_vf_empreinte (
		   const vf_model * const m,
		   int nb_tranche,
		   double **list)
{
  int size = 0;
  int index;
  point3d ptmin,
    ptmax,
    centre,
    p;

  point3d_init (&ptmin, m->xmin, m->ymin, m->zmin);
  point3d_init (&ptmax, m->xmax, m->ymax, m->zmax);

  double longdiag = point3d_length (&ptmin, &ptmax);
  double longueur;
  longdiag /= 2.0;

  point3d_init (&centre, (ptmin.x + ptmax.x) / 2.0, (ptmin.y + ptmax.y) / 2.0,
		(ptmin.z + ptmax.z) / 2.0);

  *list = NULL;
  for (int i = 0; i < nb_tranche; i++)
    list_double_add (list, &size, 0.0, WITH_REDUNDANCE);

  for (int i = 0; i < m->nbvertex; i++)
    {
      point3d_init (&p, m->ve[i].x, m->ve[i].y, m->ve[i].z);
      longueur = point3d_length (&p, &centre);
      index = (int) (longueur / longdiag * nb_tranche);
      (*list)[index] = (*list)[index] + 1;
    }

  for (int i = 0; i < nb_tranche; i++)
    (*list)[i] = ((*list)[i] / m->nbvertex);

}
