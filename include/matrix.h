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



#ifndef MATRIX__H
#define MATRIX__H

#include "util.h"
#include "point.h"
#include <math.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>

/**
   Calcul du déterminant d'une matrice
   @param m pointeur sur une matrice
   @return le déterminant
*/
double matrix_determinant (
			   const gsl_matrix * const m);

/**
   Calcul de la norme de Frobenius
   @param m pointeur sur une matrice
   @return la norme de Frobenius
*/
double matrix_frobenius_norm (
			      const gsl_matrix * const m);

/**
   Addition de deux matrices
   @param A pointeur sur la première matrice
   @param B pointeur sur la seconde matrice
   @return pointeur sur une matrice contenant l'addition A+B
*/
gsl_matrix *matrix_add (
			const gsl_matrix * const A,
			const gsl_matrix * const B);

/**
   Soustraction de deux matrices
   @param A pointeur sur la première matrice
   @param B pointeur sur la seconde matrice
   @return pointeur sur une matrice contenant la soustraction A-B
*/
gsl_matrix *matrix_sub (
			const gsl_matrix * const A,
			const gsl_matrix * const B);

/**
   Multiplication d'une matrice par un scalaire
   @param A pointeur sur une matrice
   @param n scalaire par lequel on multiplie la matrice
   @return pointeur sur la matrice contenant n*A
*/
gsl_matrix *matrix_mul_scale (
			      const gsl_matrix * const A,
			      double n);

/**
   Multiplication de deux matrices
   @param A pointeur sur la première matrice
   @param B pointeur sur la seconde matrice
   @return pointeur sur une matrice contenant la multiplication A*B
   @warning Erreur du programme si le nombre de colonne de la première matrice est différent du nombre de ligne de la seconde.
*/
gsl_matrix *matrix_mul (
			const gsl_matrix * const A,
			const gsl_matrix * const B);

/**
   Calcul de l'inverse d'une matrice
   @param m pointeur sur une matrice
   @return pointeur sur une matrice contenant l'inverse \f$A^{-1}\f$
*/
gsl_matrix *matrix_inverse (
			    const gsl_matrix * const m);

/**
   Affichage d'une matrice
   @param m pointeur sur une matrice
   @return aucun
*/
void matrix_display (
		     const gsl_matrix * const m);

/**
   Initialisation d'une matrice à partir d'une liste de valeur
   @param data liste de valeur
   @param nbline nombre de ligne
   @param nbcol nombre de colonne
   @return pointeur sur la matrice initialisée avec la liste de valeur
   Exemple de code
   @code
   double data[]={1.0,2.0,3.0,4.0,5.0,6.0};
   gsl_matrix *mat=matrix_init(data,2,3);
   matrix_display(mat);
   @endcode
   donne le résultat suivant
   @code
   1.0 2.0 3.0
   4.0 5.0 6.0
   @endcode
*/
gsl_matrix *matrix_init (
			 double *data,
			 int nbline,
			 int nbcol);

/**
   Calcul de la transposé d'une matrice
   @param m pointeur sur une matrice
   @return pointeur sur une matrice contenant la transposé \f$A^T\f$
*/
gsl_matrix *matrix_transpose (
			      const gsl_matrix * const m);

/**
   Calcul de la matrice de covariance
   @param data_p premier ensemble de point
   @param data_x second ensemble de point
   @param nbpoint nombre de point
   @return la matrice de covariance
*/
gsl_matrix *cross_variance (
			    const point3d * const data_p,
			    const point3d * const data_x,
			    int nbpoint);


#endif
