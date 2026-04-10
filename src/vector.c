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

#include "vector.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

/********** MAIN FUNCTIONS **********/

/**
   Initialisation d'un vector3d (dx,dy,dz)
   @param v pointeur sur le vector3d
   @param dx dx
   @param dy dy
   @param dz dz
   @return aucun
*/
void
vector3d_init (
	       vector3d * v,
	       double dx,
	       double dy,
	       double dz)
{
  v->dx = dx;
  v->dy = dy;
  v->dz = dz;
}

/**
   Affichage d'un vector3d
   @param v le vector3d à afficher
   @return aucun
*/
void
vector3d_display (
		  const vector3d * const v)
{
  printf ("VECTOR --> [%f %f %f]\n", v->dx, v->dy, v->dz);
}

/**
   Calcule la longueur d'un vector3d
   @param v le vector3d à mesurer
   @return taille du vector3d
*/
double
vector3d_size (
	       const vector3d * const v)
{
  return sqrt (v->dx * v->dx + v->dy * v->dy + v->dz * v->dz);
}

/**
   Teste si deux vector3d sont égaux
   @param v1 le premier vector3d
   @param v2 le second vector3d à tester
   @return 1 si les deux vector3d sont égaux, 0 sinon
*/
int
vector3d_equal (
		const vector3d * const v1,
		const vector3d * const v2)
{
  return (v1->dx == v2->dx && v1->dy == v2->dy && v1->dz == v2->dz);
}

/**
   Normalise le vector3d : transforme le vector3d de sorte que sa longueur soit égale à 1.0
   @param v poiteur sur le vector3d
   @return aucun
*/
void
vector3d_normalize (
		    vector3d * v)
{
  if (v->dx == 0 && v->dy == 0 && v->dz == 0)
    return;

  double size = vector3d_size (v);
  v->dx /= size;
  v->dy /= size;
  v->dz /= size;
}

/**
   Produit scalaire de deux vector3d
   @param v1 le premier vector3d
   @param v2 le second vector3d
   @return produit scalaire v1.v2
*/
double
vector3d_scalarproduct (
			const vector3d * const v1,
			const vector3d * const v2)
{
  return v1->dx * v2->dx + v1->dy * v2->dy + v1->dz * v2->dz;
}

/**
   Produit vectoriel de deux vector3d
   @param v1 le premier vector3d
   @param v2 le second vector3d
   @return un vector3d résultat de v1^v2
*/
vector3d
vector3d_vectorialproduct (
			   const vector3d * const v1,
			   const vector3d * const v2)
{
  vector3d W;

  W.dx = v1->dy * v2->dz - v1->dz * v2->dy;
  W.dy = -(v1->dx * v2->dz - v1->dz * v2->dx);
  W.dz = v1->dx * v2->dy - v1->dy * v2->dx;

  return W;
}

/**
   Inverse le vecteur
   @param v pointeur sur le vecteur à inverser
   @return aucun
**/
void
vector3d_reverse (
		  vector3d * v)
{
  v->dx *= -1;
  v->dy *= -1;
  v->dz *= -1;
}

/**
   Addition de deux vecteurs3d v1+v2
   @param v1 premier vecteur
   @param v2 second vecteur
   @param un vector3d résultat de v1+v2
**/
vector3d
vector3d_add (
	      const vector3d * const v1,
	      const vector3d * const v2)
{
  vector3d v;
  v.dx = v1->dx + v2->dx;
  v.dy = v1->dy + v2->dy;
  v.dz = v1->dz + v2->dz;
  return v;
}

/**
   Soustraction de deux vecteurs3d v1-v2
   @param v1 premier vecteur
   @param v2 second vecteur
   @param un vector3d résultat de v1-v2
**/
vector3d
vector3d_sub (
	      const vector3d * const v1,
	      const vector3d * const v2)
{
  vector3d v;
  v.dx = v1->dx - v2->dx;
  v.dy = v1->dy - v2->dy;
  v.dz = v1->dz - v2->dz;
  return v;
}

/**
   Retourne la position de la valeur tosearch, -1 sinon
   @param list tableau de vector3d
   @param size taille du tableau
   @param tosearch vector3d à chercher
   @return position de la première occurence, -1 s'il n'apparait pas dans le tableau
*/
int
list_vector3d_contains (
			const vector3d * const list,
			int size,
			const vector3d * const tosearch)
{
  int i;
  for (i = 0; i < size; i++)
    if (vector3d_equal (&(list[i]), tosearch))
      return i;
  return -1;
}

/**
   Ajoute le vector3d toadd en fin de liste
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param toadd vector3d à ajouter
   @param add_type WITH_REDUNDANCE ou WITHOUT_REDUNDANCE <BR> avec redondance : ajout simple <BR> sans redondance : ajout si la valeur n'apparait pas dans la liste
   @return 1 si succès, 0 sinon
*/
int
list_vector3d_add (
		   vector3d ** list,
		   int *size,
		   const vector3d * const toadd,
		   int add_type)
{
  if (add_type == WITHOUT_REDUNDANCE)
    if (list_vector3d_contains (*list, *size, toadd) != -1)
      return 1;
    else
      {

	vector3d *newlist =
	  (vector3d *) malloc (((*size) + 1) * sizeof (vector3d));
	a2ri_erreur_critique_si (newlist == NULL,
			      "erreur allocation memoire pour newlist\nlist_vector3d_add");

	int i;
	for (i = 0; i < *size; i++)
	  vector3d_init (&(newlist[i]), ((*list)[i]).dx, ((*list)[i]).dy,
			 ((*list)[i]).dz);
	vector3d_init (&(newlist[*size]), toadd->dx, toadd->dy, toadd->dz);
	free (*list);
	(*list) = newlist;
	*size = (*size) + 1;
      }
  else
    {
      vector3d *temp;
      temp = (vector3d *) realloc (*list, (*size + 1) * sizeof (vector3d));
      a2ri_erreur_critique_si (temp == NULL,
			       "erreur allocation memoire pour list\nlist_vector3d_add");
      *list=temp;
      vector3d_init (&((*list)[*size]), toadd->dx, toadd->dy, toadd->dz);
      *size = (*size) + 1;
    }
  return 1;
}

/**
   Affichage de la liste de vector3d
   @param list tableau de vector3d
   @param size taille du tableau
   @return aucun
*/
void
list_vector3d_display (
		       const vector3d * const list,
		       int size)
{
  int i;
  if (size == 0)
    return;
  printf ("liste :\n");
  for (i = 0; i < size; i++)
    vector3d_display (&(list[i]));
}
