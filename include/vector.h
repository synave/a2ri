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



#ifndef VECTOR__H
#define VECTOR__H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"


typedef struct
{
  double dx,
    dy,
    dz;
} vector3d;

/**
   Initialisation d'un vector3d (dx,dy,dz)
   @param v pointeur sur le vector3d
   @param dx dx
   @param dy dy
   @param dz dz
   @return aucun
*/
void vector3d_init (
		    vector3d * v,
		    double dx,
		    double dy,
		    double dz);

/**
   Affichage d'un vector3d
   @param v le vector3d à afficher
   @return aucun
*/
void vector3d_display (
		       const vector3d * const v);

/**
   Calcule la longueur d'un vector3d
   @param v le vector3d à mesurer
   @return taille du vector3d
*/
double vector3d_size (
		      const vector3d * const v);

/**
   Teste si deux vector3d sont égaux
   @param v1 le premier vector3d
   @param v2 le second vector3d à tester
   @return 1 si les deux vector3d sont égaux, 0 sinon
*/
int vector3d_equal (
		    const vector3d * const v1,
		    const vector3d * const v2);

/**
   Normalise le vector3d : transforme le vector3d de sorte que sa longueur soit égale à 1.0
   @param v poiteur sur le vector3d
   @return aucun
*/
void vector3d_normalize (
			 vector3d * v);

/**
   Produit scalaire de deux vector3d
   @param v1 le premier vector3d
   @param v2 le second vector3d
   @return produit scalaire v1.v2
*/
double vector3d_scalarproduct (
			       const vector3d * const v1,
			       const vector3d * const v2);

/**
   Produit vectoriel de deux vector3d
   @param v1 le premier vector3d
   @param v2 le second vector3d
   @return un vector3d résultat de v1^v2
*/
vector3d vector3d_vectorialproduct (
				    const vector3d * const v1,
				    const vector3d * const v2);

/**
   Inverse le vecteur
   @param v pointeur sur le vecteur à inverser
   @return aucun
**/
void vector3d_reverse (
		       vector3d * v);

/**
   Addition de deux vecteurs3d v1+v2
   @param v1 premier vecteur
   @param v2 second vecteur
   @param un vector3d résultat de v1+v2
**/
vector3d vector3d_add (
		       const vector3d * const v1,
		       const vector3d * const v2);

/**
   Soustraction de deux vecteurs3d v1-v2
   @param v1 premier vecteur
   @param v2 second vecteur
   @param un vector3d résultat de v1-v2
**/
vector3d vector3d_sub (
		       const vector3d * const v1,
		       const vector3d * const v2);

/**
   Retourne la position de la valeur tosearch, -1 sinon
   @param list tableau de vector3d
   @param size taille du tableau
   @param tosearch vector3d à chercher
   @return position de la première occurence, -1 s'il n'apparait pas dans le tableau
*/
int list_vector3d_contains (
			    const vector3d * const list,
			    int size,
			    const vector3d * const tosearch);

/**
   Ajoute le vector3d toadd en fin de liste
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param toadd vector3d à ajouter
   @param add_type WITH_REDUNDANCE ou WITHOUT_REDUNDANCE <BR> avec redondance : ajout simple <BR> sans redondance : ajout si la valeur n'apparait pas dans la liste
   @return 1 si succès, 0 sinon
*/
int list_vector3d_add (
		       vector3d ** list,
		       int *size,
		       const vector3d * const toadd,
		       int add_type);

/**
   Affichage de la liste de vector3d
   @param list tableau de vector3d
   @param size taille du tableau
   @return aucun
*/
void list_vector3d_display (
			    const vector3d * const list,
			    int size);

#endif
