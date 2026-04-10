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



#ifndef BOUNDING_BOX__H
#define BOUNDING_BOX__H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

#include "util.h"
#include "model.h"
#include "point.h"
#include "matrix.h"

/**
   Calcul des boites englobantes (parallélépipèdes) des faces de la liste
   @param m le modèle
   @param faces tableau contenant les numéros des faces pour lesquels on veut les boites englobantes
   @param nbfaces taille du tableau
   @param ptmin pointeur sur le tableau contenant les points (xmin,ymin,zmin) des boites englobantes
   @param ptmax pointeur sur le tableau contenant les points (xmax,ymax,zmax) des boites englobantes
**/
void a2ri_vf_axis_aligned_bounding_box (
					const vf_model * const m,
					const int * const faces,
					int nbfaces,
					point3d ** ptmin,
					point3d ** ptmax);

/**
   Calcul des sphères englobantes des faces de la liste en utilisant les fastball de Ritter
   @param m le modèle
   @param faces tableau contenant les numéros des faces pour lesquels on veut les sphères englobantes
   @param nbfaces taille du tableau
   @param listcentre pointeur sur le tableau de point3d contenant les centres de sphères englobantes
   @param listradius pointeur sur le tableau contenant les rayons des sphères englobantes
**/
void a2ri_vf_bounding_ball_ritter (
				   const vf_model * const m,
				   const int * const faces,
				   int nbfaces,
				   point3d ** listcentre,
				   double ** listradius);

/**
   Calcul des sphères englobantes des faces de la liste
   @param m le modèle
   @param faces tableau contenant les numéros des faces pour lesquels on veut les sphères englobantes
   @param nbfaces taille du tableau
   @param listcentre pointeur sur le tableau de point3d contenant les centres de sphères englobantes
   @param listradius pointeur sur le tableau contenant les rayons des sphères englobantes
**/
void a2ri_vf_bounding_ball_minimale (
				     const vf_model * const m,
				     const int * const faces,
				     int nbfaces,
				     point3d ** listcentre,
				     double ** listradius);

/**
   Calcul l'oriented bounding box du maillage
   @param m le modèle
   @param newbasex pointeur sur le vecteur "X" de la nouvelle base dans laquelle l'oriented bounding box est représenté
   @param newbasey pointeur sur le vecteur "Y" de la nouvelle base dans laquelle l'oriented bounding box est représenté
   @param newbasez pointeur sur le vecteur "Z" de la nouvelle base dans laquelle l'oriented bounding box est représenté
   @param ptmin pointeur sur le point3d contenant le coin inférieur de l'oriented bounding box
   @param ptmax pointeur sur le point3d contenant le coin supérieur de l'oriented bounding box
**/
void a2ri_vf_oriented_bounding_box (
				    const vf_model * const m,
				    vector3d * newbasex,
				    vector3d * newbasey,
				    vector3d * newbasez,
				    point3d * ptmin,
				    point3d * ptmax);

/**
   Calcul des sphères englobantes des faces de la liste
   @param m le modèle
   @param faces tableau contenant les numéros des faces pour lesquels on veut les sphères englobantes
   @param nbfaces taille du tableau
   @param listcentre pointeur sur le tableau de point3d contenant les centres de sphères englobantes
   @param listradius pointeur sur le tableau contenant les rayons des sphères englobantes
**/
void a2ri_vef_bounding_ball_minimale (
				      const vef_model * const m,
				      const int * const faces,
				      int nbfaces,
				      point3d ** listcentre,
				      double ** listradius);

/**
   Calcul des sphères englobantes des faces de la liste
   @param m le modèle
   @param faces tableau contenant les numéros des faces pour lesquels on veut les sphères englobantes
   @param nbfaces taille du tableau
   @param listcentre pointeur sur le tableau de point3d contenant les centres de sphères englobantes
   @param listradius pointeur sur le tableau contenant les rayons des sphères englobantes
**/
void point3d_bounding_ball_minimale (
				     const point3d * const faces,
				     int nbfaces,
				     point3d ** listcentre,
				     double ** listradius);

#endif
