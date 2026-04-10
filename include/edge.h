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



#ifndef EDGE__H
#define EDGE__H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  int ve1;
  int ve2;
  int *sharedfaces;
  int nbsharedfaces;
  int att_int;
  double att_double;
} vf_edge;

typedef struct
{
  int ve1;
  int ve2;
  int *sharedfaces;
  int nbsharedfaces;
} vef_edge;

typedef struct
{
  int ve1;
  int ve2;
} skeleton_edge;

/**
   Affichage d'une arete
   @param e arête à afficher
   @return aucun
*/
void vf_edge_display (
		      const vf_edge * const e);

/**
   Libération de la mémoire
   @param e l'arete
   @return aucun
**/
void vf_edge_free (
		   vf_edge * e);

/**
   Affichage d'une arete
   @param e arête à afficher
   @return aucun
*/
void vef_edge_display (
		       const vef_edge * const e);

/**
   Libération de la mémoire
   @param e l'arete
   @return aucun
**/
void vef_edge_free (
		    vef_edge * e);

#endif
