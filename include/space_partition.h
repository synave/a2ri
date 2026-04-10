/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 01/11/07       */
/* Date de modification : 15/03/15   */
/* Version : 0.4                     */
/*************************************/

/*************************************/
/* Auteur : Romain Leguay            */
/*          Nguyen Haiduong          */
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

#ifndef SPACE_PARTITION__H
#define SPACE_PARTITION__H

#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "geometry.h"
#include "util.h"

#define REJECT_ZERO_LENGTH 0
#define ACCEPT_ZERO_LENGTH 1

typedef struct
{
  point3d *list_point;
  int nb_point;
} sp_depth,
  *pt_sp_depth;

typedef struct
{
  sp_depth *z;
} sp_height;

typedef struct
{
  sp_height *y;
} sp_width;

/**
   structure de partition de l'espace avec un tableau à 3 dimensions contenant des listes de points
*/
typedef struct
{
  point3d ptmin,
    ptmax;
  int nb_part_x,
    nb_part_y,
    nb_part_z;
  sp_width *x;
} space_partition;

/**
   Initialisation d'une partition de l'espace
   @param sp la partition de l'espace
   @return aucun
*/
void space_partition_init (
			   space_partition * sp);

/**
   Création d'une nouvelle partition de l'espace
   @param sp la partition de l'espace
   @param min coin inférieur
   @param max coin supérieur
   @param nbpartx nombre de partition en X
   @param nbparty nombre de partition en Y
   @param nbpartz nombre de partition en Z
   @return aucun
*/
void space_partition_new (
			  space_partition * sp,
			  const point3d * const min,
			  const point3d * const max,
			  int nbpartx,
			  int nbparty,
			  int nbpartz);

/**
   Désallocation mémoire de la partition de l'espace
   @param sp la partition de l'espace
   @return aucun
*/
void space_partition_free (
			   space_partition * sp);

/**
   Affichage simple d'une partition de l'espace
   @param sp la partition de l'espace
   @return aucun
*/
void space_partition_display (
			      const space_partition * const sp);

/**
   Affichage detaillé d'une partition de l'espace
   @param sp la partition de l'espace
   @return aucun
*/
void space_partition_display_detail (
				     const space_partition * const sp);

/**
   Affichage detaillé d'une partition de l'espace
   @param sp la partition de l'espace
   @return aucun
*/
void space_partition_display_high_detail (
					  const space_partition * const sp);

/**
   Calcule les numeros de cases du point
   @param sp la partition de l'espace
   @param p le point
   @param x le numéro de case en X
   @param y le numéro de case en Y
   @param z le numéro de case en Z
   @return aucun
**/
void space_partition_get_XYZ (
			      const space_partition * const sp,
			      const point3d * const p,
			      int *x,
			      int *y,
			      int *z);

/**
   Ajout d'un point dans la partition de l'espace
   @param sp la partition de l'espace
   @param p le point à ajouter
   @return aucun
*/
void space_partition_add_point (
				space_partition * sp,
				const point3d * const p);

/**
   Calcule le point moyen d'une cellule
   @param sp la partition de l'espace
   @param cell_x numéro de la cellule en X
   @param cell_y numéro de la cellule en Y
   @param cell_z numéro de la cellule en Z
   @param aucun
*/
point3d space_partition_get_average (
				     const space_partition * const sp,
				     int cell_x,
				     int cell_y,
				     int cell_z);

/**
   Calcule le point milieu d'une cellule
   @param sp la partition de l'espace
   @param cell_x numéro de la cellule en X
   @param cell_y numéro de la cellule en Y
   @param cell_z numéro de la cellule en Z
   @param aucun
*/
point3d space_partition_get_middle (
				    const space_partition * const sp,
				    int cell_x,
				    int cell_y,
				    int cell_z);

/**
   Retourne le nombre total de points de la partition de l'espace
   @param sp la partition de l'espace
   @return le nombre de points
*/
int space_partition_nb_points (
			       const space_partition * const sp);

/**
   Retourne les cases adjacentes à celle contenant le point passé en paramètre est à une distance maximum de distance
   @param sp la partition de l'espace
   @param pt le point servant à sélectionner la case centrale
   @param distance profondeur de la sélection
   @param list liste des cases sélectionnées
   @param size taille de la liste retournée
   @return aucun
**/
void space_partition_get_neighbour (
				    const space_partition * const sp,
				    const point3d * const pt,
				    int distance,
				    pt_sp_depth ** list,
				    int *size);

/**
   Retourne le point le plus proche d'un point donné
   @param sp la partition de l'espace
   @param pt le point dont on cherche le point le plus proche dans la partition de l'espace
   @param near_pt point le plus proche de la partition de l'espace
   @param length longueur entre les deux points
   @return un pointeur vers le point de la partition le plus paroche de celui recherche
*/
point3d * space_partition_nearest_point (
					const space_partition * const sp,
					const point3d * const pt,
					point3d * near_pt,
					double *length,
					int acceptzerolength);

/**
   Retourne un pointeur vers le point de la partition s'il existe
   @param sp la partition de l'espace
   @param pt le point que l'on cherche dans la partition
   @return un pointeur vers le point cherché, NULL sinon
*/
point3d * space_partition_contains_point (
					const space_partition * const sp,
					const point3d * const pt);

/**
   Enlève les outliers
   @param sp la partition de l'espace
   @return aucun
*/
void space_partition_remove_outliers (
				      space_partition * sp);

/**
   retourne une liste de pointeur sur des point3d contenus dans les cellules passées en paramètre
   @param sp la partition de l'espace
   @param listcellx liste des index des cellules en X
   @param listcelly liste des index des cellules en Y
   @param listcellz liste des index des cellules en Z
   @param sizelist taille de la liste des cellules
   @param listpoint liste des pointeurs vers les point3d
   @param sizelistpoint taille de la liste de pointeur
   @return aucun
**/
void space_partition_get_points_cells (
				       const space_partition * const sp,
				       const int * const listcellx,
				       const int * const listcelly,
				       const int * const listcellz,
				       int sizelist,
				       pt_point3d ** listpoint,
				       int *sizelistpoint);


/**
   Calcule la Bounding Box de la liste de points
   @param sp la partition de l'epace
   @param list la list de point
   @param size nombre de points
   @param xmin cellule minimale en X
   @param ymin cellule minimale en Y
   @param zmin cellule minimale en Z
   @param xmax cellule maximale en X
   @param ymax cellule maximale en Y
   @param zmax cellule maximale en Z
**/
void space_partition_bounding_box (
				   const space_partition * const sp,
				   const point3d * const list,
				   int size,
				   int *xmin,
				   int *ymin,
				   int *zmin,
				   int *xmax,
				   int *ymax,
				   int *zmax);

/**
   Retourne les cases adjacentes et elle-même à celle contenant le point passé en paramètre est à une distance maximum de distance
   @param sp la partition de l'espace
   @param pt le point servant à sélectionner la case centrale
   @param list liste des cases sélectionnées
   @param size taille de la liste retournée
   @return aucun
**/
void space_partition_get_list_near_point (
					  const space_partition * const sp,
					  const point3d * const pt,
					  pt_sp_depth ** list,
					  int *size);

#endif
