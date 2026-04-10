/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 15/05/08       */
/* Date de modification : 15/03/15   */
/* Version : 0.4                     */
/*************************************/

/*************************************/
/* Auteur : Romain Leguay            */
/*          Nguyen Haiduong          */
/*          Marianne Fichoux         */
/* Date de modification : 05/06/09   */
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

#ifndef TRIANGULATION__H
#define TRIANGULATION__H

#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "point.h"
#include "space_partition.h"
#include "edge.h"
#include "vector.h"
#include "util.h"
#include "overlap.h"
#include "icp.h"

/**
   Création d'un vf_model avec une triangulation de delaunay
   @param list liste de point
   @param nbpoint nombre de points
   @return aucun
**/
vf_model * a2ri_vf_delaunay_triangulation (
					   const point3d * const list,
					   int nbpoint);


/**
   Création d'un vef_model avec une triangulation de delaunay
   @param list liste de point
   @param nbpoint nombre de points
   @return le nouveau modele créé
**/
vef_model * a2ri_vef_delaunay_triangulation (
					     const point3d * const list,
					     int nbpoint);

/**
   Nettoyage par suppression des faces dans la zone de recouvrement
   @param base,m 2 vf_model
   @param sensibility sensibilité
   @return aucun
**/
void nettoyage_delete_face (
			    vf_model * base,
			    vf_model * m,
			    double sensibility);

/*ALGORITHME BPA 2010*/

/*SD*/

#define ACTIVE 0
#define BOUNDARY 1
#define FROZEN 2

struct bpa_fronts;

typedef struct bpa_edge
{
  int sigma_i,sigma_j,sigma_o;
  point3d cijo;
  char state;
  struct bpa_edge *next,*prev;
  struct bpa_fronts *front;
} bpa_edge;

typedef struct bpa_fronts
{
  bpa_edge *front;
  struct bpa_fronts *next;
} bpa_fronts;

/*Fonctions*/

void a2ri_vf_bpa(vf_model *m, vector3d *normal, double radius);

//void a2ri_vf_bpa_multipass(vf_model *m, vector3d *normal, double *listradius, int listsize);

void a2ri_vf_bpa_without_normal(vf_model *m, double radius);

void a2ri_bpa_free_fronts(bpa_fronts **fronts);

void a2ri_bpa_new_front(bpa_fronts **fronts, int sigma_i, int sigma_j, int sigma_k, point3d centre);

bpa_edge* a2ri_bpa_get_active_edge_in_fronts(bpa_fronts *fronts);

char a2ri_bpa_front_contains_point_in_fronts(bpa_fronts *fronts, int sigma_k);

int a2ri_bpa_front_distance(bpa_edge *front, int index);

char a2ri_bpa_find_seed_triangle(vf_model *m, space_partition *sp, vector3d *normal, double radius, int *listused, int sizeused, int *sigma_i, int *sigma_j, int *sigma_k, point3d *centre, int imin);

void a2ri_bpa_regularization(bpa_fronts **fronts, int sigma_i, int sigma_j, int sigma_k);

void a2ri_bpa_ball_pivot(vf_model *m, space_partition *sp, vector3d *normal, double radius, bpa_edge *e, int *listused, int sizeused, bpa_fronts *fronts, int *sigma_k, point3d *centre);

void a2ri_bpa_join(bpa_edge **e, int sigma_k, point3d centre);

void a2ri_bpa_display_fronts(bpa_fronts *fronts);

void a2ri_bpa_average_radius_suggestion(vf_model *m, double *min, double *max, double *average, double *sd);

void a2ri_bpa_initialisation(vf_model *m, bpa_fronts **fronts, int **listused, int *sizelistused);

#endif
