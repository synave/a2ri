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



#ifndef FACE__H
#define FACE__H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "edge.h"

typedef struct
{
  int ve1;
  int ve2;
  int ve3;
} vf_face;

typedef struct
{
  int ed1;
  int ed2;
  int ed3;
} vef_face;

/**
   Affichage d'une face
   @param f face à afficher
   @return aucun
*/
void vf_face_display (
		      const vf_face * const f);

/**
   Inversion de l'orientation de la face
   @param f pointeur sur la face à inverser
   @return aucun
*/
void vf_face_reverse (
		      vf_face * f);

/**
   Test de l'appartenance d'un sommet à une face
   @param f pointeur sur la face
   @param numvertex numero du sommet
   @return 1 si le sommet appartient à la face, 0 sinon
*/
int vf_face_contains (
		      const vf_face * const f,
		      int numvertex);

/**
   Affichage d'une face
   @param f face à afficher
   @return aucun
*/
void vef_face_display (
		       const vef_face * const f);

/**
   Inversion de l'orientation de la face
   @param f pointeur sur la face à inverser
   @return aucun
*/
void vef_face_reverse (
		       vef_face * f);

/**
   Donne les trois points formant la face
   @param f la face dont on veut les trois sommets
   @param list list des aretes
   @param ve1 index du premier sommet
   @param ve2 index du second sommet
   @param ve3 index du troisième sommet
   @return aucun
*/
void vef_face_get_vertices (
			    const vef_face * const f,
			    const vef_edge * const list,
			    int * ve1,
			    int * ve2,
			    int * ve3);

/**
   Test de l'appartenance d'une arete à une face
   @param f pointeur sur la face
   @param numvertex numero de l'arete
   @return 1 si l'arete appartient à la face, 0 sinon
*/
int vef_face_contains (
		       const vef_face * const f,
		       int numedge);

#endif
