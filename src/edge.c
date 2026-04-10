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

#include "edge.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

/********** MAIN FUNCTIONS **********/

/**
   Affichage d'une arete
   @param e arête à afficher
   @return aucun
*/
void
vf_edge_display (
		 const vf_edge * const e)
{
  printf ("Edge --> [vertex %d , vertex %d] , valence : %d\n", e->ve1, e->ve2,
	  e->nbsharedfaces);
}

/**
   Libération de la mémoire
   @param e l'arete
   @return aucun
**/
void
vf_edge_free (
	      vf_edge * e)
{
  free (e->sharedfaces);
}


/**
   Affichage d'une arete
   @param e arete à afficher
   @return aucun
*/
void
vef_edge_display (
		  const vef_edge * const e)
{
  printf ("Edge --> [vertex %d , vertex %d] , valence : %d\n", e->ve1, e->ve2,
	  e->nbsharedfaces);
}


/**
   Libération de la mémoire
   @param e l'arete
   @return aucun
**/
void
vef_edge_free (
	       vef_edge * e)
{
  free (e->sharedfaces);
}
