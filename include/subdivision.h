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



#ifndef SUBDIVISION__H
#define SUBDIVISION__H

#include "util.h"
#include "model.h"
#include "edge.h"
#include "hashtable.h"

/**
   Subdivision d'un modèle avec la méthode de Loop
   @param m pointeur sur le modèle à subdiviser
   @param nbiter nombre d'itération de la subdivision
   @return aucun
**/
void a2ri_vf_loop (
		   vf_model * m,
		   int nbiter);

/**
   Subdivision d'un modele ou coupant les triangles en 6 triangles avec un point central
   @param m pointeur sur le modele a subdiviser
   @param nbiter nom c'iteration de la subdivision
   @param aucun
**/
void a2ri_vf_6_subdivision (
			    vf_model * m,
			    int nbiter);

/**
   Subdivision d'un modele ou coupant les triangles en 4 triangles
   @param m pointeur sur le modele a subdiviser
   @param nbiter nom c'iteration de la subdivision
   @param aucun
**/
void a2ri_vf_4_subdivision (
			    vf_model * m,
			    int nbiter);

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
			      point3d * C);

/**
   Subdivision d'un modele ou coupant les triangles en 6 triangles avec un point central
   @param m pointeur sur le modele a subdiviser
   @param nbiter nom c'iteration de la subdivision
   @param aucun
**/
void a2ri_vf_general_subdivision (
				  vf_model * m,
				  hashtable * table);

/**
   Subdivision d'un modèle avec la méthode de Loop
   @param m pointeur sur le modèle à subdiviser
   @param nbiter nombre d'itération de la subdivision
   @return aucun
**/
void a2ri_vef_loop (
		    vef_model * m,
		    int nbiter);

/**
   Subdivision d'un modèle avec la méthode de Butterfly
   @param m pointeur sur le modèle à subdiviser
   @param nbiter nombre d'itération de la subdivision
   @param tension paramètre de tension
   @return aucun
**/
void a2ri_vef_butterfly (
			 vef_model * m,
			 int nbiter,
			 double tension);


#endif
