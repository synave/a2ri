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

#include "matrix.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

/********** MAIN FUNCTIONS **********/

/**
   Calcul du déterminant d'une matrice
   @param m pointeur sur une matrice
   @return le déterminant
*/
double
matrix_determinant (
		    const gsl_matrix * const m)
{
  double det;
  int s;
  gsl_permutation *perm = gsl_permutation_alloc (m->size1);
  a2ri_erreur_critique_si (perm == NULL,
			"erreur allocation memoire pour perm\nmatrix_determinant");
  gsl_matrix *ludecomp = gsl_matrix_alloc (m->size1, m->size1);
  a2ri_erreur_critique_si (ludecomp == NULL,
			"erreur allocation memoire pour ludecomp\nmatrix_determinant");


  gsl_matrix_memcpy (ludecomp, m);

  //get the LU decomposition
  gsl_linalg_LU_decomp (ludecomp, perm, &s);

  //get the determinant
  det = gsl_linalg_LU_det (ludecomp, s);

  gsl_matrix_free (ludecomp);
  gsl_permutation_free (perm);
  return det;
}

/**
   Calcul de la norme de Frobenius
   @param m pointeur sur une matrice
   @return la norme de Frobenius
*/
double
matrix_frobenius_norm (
		       const gsl_matrix * const m)
{
  double somme = 0.0;
  for (size_t i = 0; i < m->size1; i++)
    for (size_t j = 0; j < m->size2; j++)
      somme += gsl_matrix_get (m, i, j) * gsl_matrix_get (m, i, j);
  somme = sqrt (somme);
  return somme;
}

/**
   Addition de deux matrices
   @param A pointeur sur la première matrice
   @param B pointeur sur la seconde matrice
   @return pointeur sur une matrice contenant l'addition A+B
*/
gsl_matrix *
matrix_add (
	    const gsl_matrix * const A,
	    const gsl_matrix * const B)
{
  gsl_matrix *res;

  a2ri_erreur_critique_si (A->size1 != B->size1
			|| A->size2 != B->size2,
			"matrice incompatible pour l'addition");

  res = gsl_matrix_alloc (A->size1, B->size2);
  a2ri_erreur_critique_si (res == NULL,
			"erreur allocation memoire pour res\nmatrix_add");

  for (size_t i = 0; i < A->size1; i++)
    for (size_t j = 0; j < A->size2; j++)
      gsl_matrix_set (res, i, j,
		      gsl_matrix_get (A, i, j) + gsl_matrix_get (B, i, j));

  return res;
}

/**
   Soustraction de deux matrices
   @param A pointeur sur la première matrice
   @param B pointeur sur la seconde matrice
   @return pointeur sur une matrice contenant la soustraction A-B
*/
gsl_matrix *
matrix_sub (
	    const gsl_matrix * const A,
	    const gsl_matrix * const B)
{
  gsl_matrix *res;

  a2ri_erreur_critique_si (A->size1 != B->size1
			|| A->size2 != B->size2,
			"matrice incompatible pour la soustraction");

  res = gsl_matrix_alloc (A->size1, B->size2);
  a2ri_erreur_critique_si (res == NULL,
			"erreur allocation memoire pour res\nmatrix_sub");

  for (size_t i = 0; i < A->size1; i++)
    for (size_t j = 0; j < A->size2; j++)
      gsl_matrix_set (res, i, j,
		      gsl_matrix_get (A, i, j) - gsl_matrix_get (B, i, j));

  return res;
}

/**
   Multiplication d'une matrice par un scalaire
   @param A pointeur sur une matrice
   @param n scalaire par lequel on multiplie la matrice
   @return pointeur sur la matrice contenant n*A
*/
gsl_matrix *
matrix_mul_scale (
		  const gsl_matrix * const A,
		  double n)
{
  gsl_matrix *res = gsl_matrix_alloc (A->size1, A->size2);
  a2ri_erreur_critique_si (res == NULL,
			"erreur allocation memoire pour res\nmatrix_scale");

  for (size_t i = 0; i < A->size1; i++)
    for (size_t j = 0; j < A->size2; j++)
      gsl_matrix_set (res, i, j, gsl_matrix_get (A, i, j) * n);

  return res;
}

/**
   Multiplication de deux matrices
   @param A pointeur sur la première matrice
   @param B pointeur sur la seconde matrice
   @return pointeur sur une matrice contenant la multiplication A*B
   @warning Erreur du programme si le nombre de colonne de la première matrice est différent du nombre de ligne de la seconde.
*/
gsl_matrix *
matrix_mul (
	    const gsl_matrix * const A,
	    const gsl_matrix * const B)
{
  double alpha = 1.,
    beta = 0.;
  gsl_matrix *C = gsl_matrix_alloc (A->size1, B->size2);
  a2ri_erreur_critique_si (C == NULL,
			"erreur allocation memoire pour C\nmatrix_mul");

  gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, alpha, A, B, beta, C);
  return C;
}

/**
   Calcul de l'inverse d'une matrice
   @param m pointeur sur une matrice
   @return pointeur sur une matrice contenant l'inverse \f$A^{-1}\f$
*/
gsl_matrix *
matrix_inverse (
		const gsl_matrix * const m)
{
  int s;
  gsl_permutation *p = gsl_permutation_alloc (m->size1);
  a2ri_erreur_critique_si (p == NULL,
			"erreur allocation memoire pour p\nmatrix_inverse");
  gsl_matrix *ludecomp = gsl_matrix_alloc (m->size1, m->size1);
  a2ri_erreur_critique_si (ludecomp == NULL,
			"erreur allocation memoire pour ludecomp\nmatrix_inverse");
  gsl_matrix *inv = gsl_matrix_alloc (m->size1, m->size1);
  a2ri_erreur_critique_si (inv == NULL,
			"erreur allocation memoire pour inv\nmatrix_inverse");


  gsl_matrix_memcpy (ludecomp, m);
  gsl_linalg_LU_decomp (ludecomp, p, &s);
  gsl_linalg_LU_invert (ludecomp, p, inv);
  gsl_matrix_free (ludecomp);
  gsl_permutation_free (p);
  return inv;
}

/**
   Affichage d'une matrice
   @param m pointeur sur une matrice
   @return aucun
*/
void
matrix_display (
		const gsl_matrix * const m)
{
  printf ("[[\n");
  for (size_t i = 0; i < m->size1; i++)
    {
      for (size_t j = 0; j < m->size2; j++)
	printf ("%4.2lf\t", gsl_matrix_get (m, i, j));
      printf ("\n");
    }
  printf ("]]\n");
}

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
gsl_matrix *
matrix_init (
	     double *data,
	     int nbline,
	     int nbcol)
{
  gsl_matrix *m = gsl_matrix_alloc (nbline, nbcol);
  a2ri_erreur_critique_si (m == NULL,
			"erreur allocation memoire pour m\nmatrix_init");
  for (int i = 0; i < nbline; i++)
    for (int j = 0; j < nbcol; j++)
      gsl_matrix_set (m, i, j, data[i * nbcol + j]);

  return m;
}

/**
   Calcul de la transposé d'une matrice
   @param m pointeur sur une matrice
   @return pointeur sur une matrice contenant la transposé \f$A^T\f$
*/
gsl_matrix *
matrix_transpose (
		  const gsl_matrix * const m)
{
  gsl_matrix *ret = gsl_matrix_alloc (m->size2, m->size1);
  a2ri_erreur_critique_si (ret == NULL,
			"erreur allocation memoire pour ret\nmatrix_transpose");

  for (size_t i = 0; i < m->size1; i++)
    for (size_t j = 0; j < m->size2; j++)
      gsl_matrix_set (ret, j, i, gsl_matrix_get (m, i, j));

  return ret;
}

/**
   Calcul de la matrice de covariance
   @param data_p premier ensemble de point
   @param data_x second ensemble de point
   @param nbpoint nombre de point
   @return la matrice de covariance
*/
//calcul de la matrice covariance
//voir Besl p243
gsl_matrix *
cross_variance (
		const point3d * const data_p,
		const point3d * const data_x,
		int nbpoint)
{
  point3d *centerP,
    *centerX,
    tempp,
    tempx;
  gsl_matrix *somme,
    *up,
    *ux,
    *mul = NULL,
    *pi,
    *xi;
  double d[3];

  somme = gsl_matrix_calloc (3, 3);
  a2ri_erreur_critique_si (somme == NULL,
			"erreur allocation memoire pour somme\ncross_variance");

  centerP = center_of_mass (data_p, nbpoint);
  centerX = center_of_mass (data_x, nbpoint);

  d[0] = centerP->x;
  d[1] = centerP->y;
  d[2] = centerP->z;
  up = matrix_init (d, 3, 1);

  d[0] = centerX->x;
  d[1] = centerX->y;
  d[2] = centerX->z;
  ux = matrix_init (d, 1, 3);

  for (int i = 0; i < nbpoint; i++)
    {
      //p_i-up
      tempp.x = data_p[i].x - gsl_matrix_get (up, 0, 0);
      tempp.y = data_p[i].y - gsl_matrix_get (up, 1, 0);
      tempp.z = data_p[i].z - gsl_matrix_get (up, 2, 0);

      //p_x-ux
      tempx.x = data_x[i].x - gsl_matrix_get (ux, 0, 0);
      tempx.y = data_x[i].y - gsl_matrix_get (ux, 0, 1);
      tempx.z = data_x[i].z - gsl_matrix_get (ux, 0, 2);

      d[0] = tempp.x;
      d[1] = tempp.y;
      d[2] = tempp.z;
      pi = matrix_init (d, 3, 1);
      d[0] = tempx.x;
      d[1] = tempx.y;
      d[2] = tempx.z;
      xi = matrix_init (d, 1, 3);

      //(p_i-up)(p_x-ux)
      mul = matrix_mul (pi, xi);

      //somme
      somme = matrix_add (somme, mul);

      gsl_matrix_free (pi);
      gsl_matrix_free (xi);

    }

  for (size_t i = 0; i < somme->size1; i++)
    for (size_t j = 0; j < somme->size2; j++)
      gsl_matrix_set (somme, i, j,
		      gsl_matrix_get (somme, i, j) / (nbpoint * 1.0));

  free (centerP);
  free (centerX);
  gsl_matrix_free (up);
  gsl_matrix_free (ux);
  gsl_matrix_free (mul);

  return somme;
}
