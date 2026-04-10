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



#ifndef POINT__H
#define POINT__H

#include <stdio.h>
#include <math.h>
#include "util.h"

/**
   Structure point2d
*/
typedef struct
{
  double x,
    y;
  int att_int;
  double att_double;
} point2d;

/**
   Structure point3d
*/
typedef struct
{
  double x,
    y,
    z;
  int att_int;
  double att_double;
} point3d,
  *pt_point3d;

/**
   Initialisation d'un point2d (x,y)
   @param p pointeur sur un point2d
   @param x
   @param y
   @return aucun
*/
void point2d_init (
		   point2d * p,
		   double x,
		   double y);

/**
   Initialisation d'un point3d (x,y,z)
   @param p pointeur sur un point3d
   @param x
   @param y
   @param z
   @return aucun
*/
void point3d_init (
		   point3d * p,
		   double x,
		   double y,
		   double z);

/**
   Affichage d'un point2d
   @param p le point
   @return aucun
**/
void point2d_display (
		      const point2d * const p);

/**
   Affichage d'un point3d
   @param p le point
   @return aucun
**/
void point3d_display (
		      const point3d * const p);

/**
   Teste l'égalité de 2 points. Retourne vrai si les composantes sont égales deux à deux.
   @param p1 premier point
   @param p2 second point
   @return 1 si p1==p2, 0 sinon
*/
int point3d_equal (
		   const point3d * const p1,
		   const point3d * const p2);

/**
   Retourne la position de la valeur tosearch, -1 sinon
   @param list tableau de point3d
   @param size taille du tableau
   @param tosearch point3d à chercher
   @return position de la première occurence, -1 s'il n'apparait pas dans le tableau
*/
int list_point3d_contains (
			   const point3d * const list,
			   int size,
			   const point3d * const tosearch);

/**
   Clone la liste
   @param list la liste à cloner
   @param size taille de la liste
   @param list_clone liste clonée
   @return 1 si la liste est bien clonée, 0 sinon
**/
int list_point3d_clone (
			const point3d * const list,
			int size,
			point3d ** list_clone);

/**
   Ajoute le point3d toadd en fin de liste
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param toadd point3d à ajouter
   @param add_type WITH_REDUNDANCE ou WITHOUT_REDUNDANCE <BR> avec redondance : ajout simple <BR> sans redondance : ajout si la valeur n'apparait pas dans la liste
   @return 1 si succès, 0 sinon
*/
int list_point3d_add (
		      point3d ** list,
		      int *size,
		      const point3d * const toadd,
		      int add_type);

/**
   Enlève le point3d à la position index
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param index position du réel à supprimer
   @return 1 si succès, 0 sinon
*/
int list_point3d_remove (
			 point3d ** list,
			 int *size,
			 int index);

/**
   Mélange la liste de point3d
   @param list tableau de point3d
   @param size taille du tableau
   @return aucun
*/
void list_point3d_mix (
		       point3d * list,
		       int size);

/**
   Affichage de la liste de point3d
   @param list tableau de point3d
   @param size taille du tableau
   @return aucun
*/
void list_point3d_display (
			   const point3d * const list,
			   int size);

/**
   Calcul le centre de gravite d'un ensemble de point
   @param data liste de point3d
   @param nbpoint nombre de point
   @return le centre de gravité
*/
point3d *center_of_mass (
			 const point3d * const data,
			 int nbpoint);

#endif
