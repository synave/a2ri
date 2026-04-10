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



#ifndef OVERLAP__H
#define OVERLAP__H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "model.h"
#include "boundingbox.h"
#include "point.h"
#include "util.h"


/**
   Test de l'intersection d'une AABB avec une liste d'AABB
   @param min coin inférieur de la AABB
   @param max coin supérieur de la AABB
   @param listmin liste des coins inférieurs des AABB
   @param listmax liste des coins supérieurs des AABB
   @param size taille de la liste des AABB
   @return 1 si intersection, 0 sinon
**/
int intersection_axis_aligned_bounding_box_with_axis_aligned_bounding_box_list (
										const point3d * const min,
										const point3d * const max,
										const point3d * const listmin,
										const point3d * const listmax,
										int size);

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
int intersection_bounding_ball_with_bounding_ball_list (
							const point3d * const c,
							double rayon,
							const point3d * const listcentre,
							const double * const listrayon,
							int nbelt,
							double alpha);

/**
   Calcule le taux de recouvrement en utilisant les bounding ball de ritter
   @param base modèle de base
   @param m modèle dont on veux trouver le taux de recouvrement avec le modèle de base
   @param sensibility sensibilité du taux de recouvrement
   @return le taux de recouvrement trouvé
**/
double a2ri_vf_bounding_ball_ritter_compute_overlap (
						     const vf_model * const base,
						     const vf_model * const m,
						     double sensibility);

/**
   Calcule le taux de recouvrement en utilisant les bounding ball minimales
   @param base modèle de base
   @param m modèle dont on veux trouver le taux de recouvrement avec le modèle de base
   @param sensibility sensibilité du taux de recouvrement
   @return le taux de recouvrement trouvé
**/
//Remarque : les maillages devraient etre qualifiés de const mais impossible a cause de l'utilisation de pthread
double a2ri_vf_bounding_ball_minimale_compute_overlap (
						       vf_model * base,
						       vf_model * m,
						       double sensibility);

/**
   Calcule le taux de recouvrement en utilisant les axis aligned bounding box
   @param base modèle de base
   @param m modèle dont on veux trouver le taux de recouvrement avec le modèle de base
   @return le taux de recouvrement trouvé
**/
double a2ri_vf_axis_aligned_bounding_box_compute_overlap (
							  const vf_model * const base,
							  const vf_model * const m);

/**
   Calcule la liste des faces du modèle m s'intersectant avec le modèle base en utilisant les bounding ball de ritter.
   @param base modèle servant de base
   @param m modèle dont on cherche les faces s'intersectant avec le modèle de base
   @param list liste des faces s'intersectant
   @param size taille de la liste
   @param alpha variable alpha servant lors du test d'intesection
   @return aucun
**/
void a2ri_vf_bounding_ball_ritter_find_face_overlapping (
							 const vf_model * const base,
							 const vf_model * const m,
							 int **list,
							 int *size,
							 double alpha);

/**
   Calcule la liste des faces du modèle m s'intersectant avec le modèle base en utilisant les bounding ball de ritter.
   @param base modèle servant de base
   @param m modèle dont on cherche les faces s'intersectant avec le modèle de base
   @param list liste des faces s'intersectant
   @param size taille de la liste
   @param alpha variable alpha servant lors du test d'intesection
   @return aucun
**/
void a2ri_vf_bounding_ball_minimale_find_face_overlapping (
							   const vf_model * const base,
							   const vf_model * const m,
							   int **list,
							   int *size,
							   double alpha);

/**
   Calcule la liste des faces du modèle m s'intersectant avec le modèle base en utilisant les AABB
   @param base modèle servant de base
   @param m modèle dont on cherche les faces s'intersectant avec le modèle de base
   @param list liste des faces s'intersectant
   @param size taille de la liste
   @return aucun
**/
void a2ri_vf_axis_aligned_bounding_box_find_face_overlapping (
							      const vf_model * const base,
							      const vf_model * const m,
							      int **listface,
							      int *size);

#endif
