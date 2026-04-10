/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 01/03/07       */
/* Date de modification : 15/03/15   */
/* Version : 0.4                     */
/*************************************/

/*************************************/
/* Auteur : Romain Leguay            */
/*          Nguyen Haiduong          */
/*          Marianne Fichoux         */
/* Date de modification : 26/05/09   */
/* Version : 0.2                    */
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



#ifndef VERTEX__H
#define VERTEX__H

#include <stdio.h>
#include <math.h>
#include "util.h"
#include "point.h"
#include "vector.h"
#include "matrix.h"

typedef struct
{
  double x;
  double y;
  double z;
  int *incidentvertices;
  int nbincidentvertices;
} vf_vertex;
//structure de données pour les sommets vf_vertex

typedef struct
{
  double x;
  double y;
  double z;
  int *sharededges;
  int nbsharededges;
} vef_vertex;

typedef vef_vertex skeleton_vertex;

/**
   Affichage du vertex
   @param v le vertex à afficher
   @return aucun
*/
void vf_vertex_display (
			const vf_vertex * const v);

/**
   Affichage detaille du vertex
   @param v le vertex à afficher
   @return aucun
*/
void vf_vertex_display_detail (
			       const vf_vertex * const v);

/**
   Translation d'un vertex
   @param v pointeur sur le vertex à translater
   @param dx décalage suivant l'axe X
   @param dy décalage suivant l'axe Y
   @param dz décalage suivant l'axe Z
*/
void vf_vertex_translate (
			  vf_vertex * v,
			  const vector3d * const delta);

/**
   Rotation d'un vertex en radian autour de l'axe X
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @return aucun
*/
void vf_vertex_rotateX_radian (
			       vf_vertex * v,
			       double angle);

/**
   Rotation d'un vertex en degre autour de l'axe X
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @return aucun
*/
void vf_vertex_rotateX_degre (
			      vf_vertex * v,
			      double angle);

/**
   Rotation d'un vertex en radian suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void vf_vertex_rotateX_center_radian (
				      vf_vertex * v,
				      double angle,
				      const point3d * const centre);

/**
   Rotation d'un vertex en degre suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void vf_vertex_rotateX_center_degre (
				     vf_vertex * v,
				     double angle,
				     const point3d * const centre);

/**
   Rotation d'un vertex en radian autour de l'axe Y
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @return aucun
*/
void vf_vertex_rotateY_radian (
			       vf_vertex * v,
			       double angle);

/**
   Rotation d'un vertex en degre autour de l'axe Y
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @return aucun
*/
void vf_vertex_rotateY_degre (
			      vf_vertex * v,
			      double angle);

/**
   Rotation d'un vertex en radian suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void vf_vertex_rotateY_center_radian (
				      vf_vertex * v,
				      double angle,
				      const point3d * const centre);

/**
   Rotation d'un vertex en degre suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void vf_vertex_rotateY_center_degre (
				     vf_vertex * v,
				     double angle,
				     const point3d * const centre);

/**
   Rotation d'un vertex en radian autour de l'axe Z
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @return aucun
*/
void vf_vertex_rotateZ_radian (
			       vf_vertex * v,
			       double angle);

/**
   Rotation d'un vertex en degre autour de l'axe Z
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @return aucun
*/
void vf_vertex_rotateZ_degre (
			      vf_vertex * v,
			      double angle);

/**
   Rotation d'un vertex en radian suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void vf_vertex_rotateZ_center_radian (
				      vf_vertex * v,
				      double angle,
				      const point3d * const centre);

/**
   Rotation d'un vertex en degre suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void vf_vertex_rotateZ_center_degre (
				     vf_vertex * v,
				     double angle,
				     const point3d * const centre);

/**
   Rotation d'un vertex en degre suivant l'axe donnée
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param axe l'axe de rotation avec (nx2 + ny2 + nz2 = 1)
   @return aucun
**/
void vf_vertex_rotate_axe_radian (
				  vf_vertex * v,
				  gsl_matrix * M);

/**
   Libération de la mémoire
   @param v le vf_vertex
   @return aucun
**/
void vf_vertex_free (
		     vf_vertex * v);

/**
   Affichage du vertex
   @param v le vertex à afficher
   @return aucun
*/
void vef_vertex_display (
			 vef_vertex * v);

/**
   Affichage detaille du vertex
   @param v le vertex à afficher
   @return aucun
*/
void vef_vertex_display_detail (
			       const vef_vertex * const v);

/**
   Translation d'un vertex
   @param v pointeur sur le vertex à translater
   @param dx décalage suivant l'axe X
   @param dy décalage suivant l'axe Y
   @param dz décalage suivant l'axe Z
*/
void vef_vertex_translate (
			   vef_vertex * v,
			   const vector3d * const delta);

/**
   Rotation d'un vertex en radian autour de l'axe X
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @return aucun
*/
void vef_vertex_rotateX_radian (
				vef_vertex * v,
				double angle);

/**
   Rotation d'un vertex en degre autour de l'axe X
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @return aucun
*/
void vef_vertex_rotateX_degre (
			       vef_vertex * v,
			       double angle);

/**
   Rotation d'un vertex en radian suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void vef_vertex_rotateX_center_radian (
				       vef_vertex * v,
				       double angle,
				       const point3d * const centre);

/**
   Rotation d'un vertex en degre suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void vef_vertex_rotateX_center_degre (
				      vef_vertex * v,
				      double angle,
				      const point3d * const centre);

/**
   Rotation d'un vertex en radian autour de l'axe Y
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @return aucun
*/
void vef_vertex_rotateY_radian (
				vef_vertex * v,
				double angle);

/**
   Rotation d'un vertex en degre autour de l'axe Y
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @return aucun
*/
void vef_vertex_rotateY_degre (
			       vef_vertex * v,
			       double angle);

/**
   Rotation d'un vertex en radian suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void vef_vertex_rotateY_center_radian (
				       vef_vertex * v,
				       double angle,
				       const point3d * const centre);

/**
   Rotation d'un vertex en degre suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void vef_vertex_rotateY_center_degre (
				      vef_vertex * v,
				      double angle,
				      const point3d * const centre);

/**
   Rotation d'un vertex en radian autour de l'axe Z
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @return aucun
*/
void vef_vertex_rotateZ_radian (
				vef_vertex * v,
				double angle);

/**
   Rotation d'un vertex en degre autour de l'axe Z
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @return aucun
*/
void vef_vertex_rotateZ_degre (
			       vef_vertex * v,
			       double angle);

/**
   Rotation d'un vertex en radian suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void vef_vertex_rotateZ_center_radian (
				       vef_vertex * v,
				       double angle,
				       const point3d * const centre);

/**
   Rotation d'un vertex en degre suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void vef_vertex_rotateZ_center_degre (
				      vef_vertex * v,
				      double angle,
				      const point3d * const centre);

/**
   Rotation d'un vertex en degre suivant l'axe donnée
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param axe l'axe de rotation avec (nx2 + ny2 + nz2 = 1)
   @return aucun
**/
void vef_vertex_rotate_axe_radian (
				   vef_vertex * v,
				   gsl_matrix * M);

/**
   Libération de la mémoire
   @param v le vef_vertex
   @return aucun
**/
void vef_vertex_free (
		      vef_vertex * v);

#endif
