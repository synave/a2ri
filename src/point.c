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

#include "point.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

/********** MAIN FUNCTIONS **********/

/**
   Initialisation d'un point2d (x,y)
   @param p pointeur sur un point2d
   @param x
   @param y
   @return aucun
*/
void
point2d_init (
	      point2d * p,
	      double x,
	      double y)
{
  p->x = x;
  p->y = y;
}

/**
   Initialisation d'un point3d (x,y,z)
   @param p pointeur sur un point3d
   @param x
   @param y
   @param z
   @return aucun
*/
void
point3d_init (
	      point3d * p,
	      double x,
	      double y,
	      double z)
{
  p->x = x;
  p->y = y;
  p->z = z;
}

/**
   Affichage d'un point2d
   @param p le point
   @return aucun
**/
void
point2d_display (
		 const point2d * const p)
{
  printf ("(%lf , %lf)\n", p->x, p->y);
}

/**
   Affichage d'un point3d
   @param p le point
   @return aucun
**/
void
point3d_display (
		 const point3d * const p)
{
  printf ("(%lf , %lf , %lf)\n", p->x, p->y, p->z);
}

/**
   Teste l'égalité de 2 points. Retourne vrai si les composantes sont égales deux à deux.
   @param p1 premier point
   @param p2 second point
   @return 1 si p1==p2, 0 sinon
*/
int
point3d_equal (
	       const point3d * const p1,
	       const point3d * const p2)
{
  return (egalite (p1->x, p2->x) && egalite (p1->y, p2->y)
	  && egalite (p1->z, p2->z));
}

/**
   Retourne la position de la valeur tosearch, -1 sinon
   @param list tableau de point3d
   @param size taille du tableau
   @param tosearch point3d à chercher
   @return position de la première occurence, -1 s'il n'apparait pas dans le tableau
*/
int
list_point3d_contains (
		       const point3d * const list,
		       int size,
		       const point3d * const tosearch)
{
  int i;
  for (i = 0; i < size; i++)
    if (point3d_equal (&(list[i]), tosearch))
      return i;
  return -1;
}

/**
   Clone la liste
   @param list la liste à cloner
   @param size taille de la liste
   @param list_clone liste clonée
   @return 1 si la liste est bien clonée, 0 sinon
**/
int
list_point3d_clone (
		    const point3d * const list,
		    int size,
		    point3d ** list_clone)
{
  (*list_clone) = (point3d *) malloc (size * sizeof (point3d));
  a2ri_erreur_critique_si (*list_clone == NULL,
			"erreur allocation memoire pour list_clone\nlist_point3d_clone");

  if ((*list_clone) == NULL)
    return 0;

  for (int i = 0; i < size; i++)
    point3d_init (&((*list_clone)[i]), list[i].x, list[i].y, list[i].z);

  return 1;
}

/**
   Ajoute le point3d toadd en fin de liste
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param toadd point3d à ajouter
   @param add_type WITH_REDUNDANCE ou WITHOUT_REDUNDANCE <BR> avec redondance : ajout simple <BR> sans redondance : ajout si la valeur n'apparait pas dans la liste
   @return 1 si succès, 0 sinon
*/
int
list_point3d_add (
		  point3d ** list,
		  int *size,
		  const point3d * const toadd,
		  int add_type)
{
  if (add_type == WITHOUT_REDUNDANCE)
    if (list_point3d_contains (*list, *size, toadd) != -1)
      return 1;

  point3d *newlist =
    (point3d *) malloc (((*size) + 1) * sizeof (point3d));
  a2ri_erreur_critique_si (newlist == NULL,
			   "erreur allocation memoire pour newlist\nlist_point3d_add");
  
  int i;
  for (i = 0; i < *size; i++)
    point3d_init (&(newlist[i]), ((*list)[i]).x, ((*list)[i]).y,
		  ((*list)[i]).z);
  point3d_init (&(newlist[*size]), toadd->x, toadd->y, toadd->z);
  free (*list);
  (*list) = newlist;
  *size = (*size) + 1;
   
  return 1;
}

/**
   Enlève le point3d à la position index
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param index position du réel à supprimer
   @return 1 si succès, 0 sinon
*/
int
list_point3d_remove (
		     point3d ** list,
		     int *size,
		     int index)
{
  point3d *temp;
  
  if (index >= *size)
    return 0;

  if(*size==1)
    {
      free(*list);
      *size=0;
      *list=NULL;
      return 1;
    }

  for (int i = index; i < (*size) - 1; i++)
    point3d_init (&((*list)[i]), ((*list)[i + 1]).x, ((*list)[i + 1]).y,
		  ((*list)[i + 1]).z);
  temp = (point3d *) realloc (*list, ((*size) - 1) * sizeof (point3d));
  a2ri_erreur_critique_si(temp==NULL && *size != 0, "erreur allocation memoire pour temp dans list_point3d_remove\n");
  *list=temp;
  *size = (*size) - 1;
  a2ri_erreur_critique_si (*list == NULL
			&& *size != 0,
			"erreur allocation memoire pour list\nlist_point3d_remove");

  return 1;
}

/**
   Mélange la liste de point3d
   @param list tableau de point3d
   @param size taille du tableau
   @return aucun
*/
void
list_point3d_mix (
		  point3d * list,
		  int size)
{
  point3d *list_clone = NULL;
  int size_clone;
  list_point3d_clone (list, size, &list_clone);
  size_clone = size;


  for (int i = 0; i < size; i++)
    {
      int index = rand_int (0, size_clone - 1);
      list[i].x = list_clone[index].x;
      list[i].y = list_clone[index].y;
      list[i].z = list_clone[index].z;
      list_point3d_remove (&list_clone, &size_clone, index);
    }
}

/**
   Affichage de la liste de point3d
   @param list tableau de point3d
   @param size taille du tableau
   @return aucun
*/
void
list_point3d_display (
		      const point3d * const list,
		      int size)
{
  int i;
  if (size == 0)
    return;
  printf ("liste :\n");
  for (i = 0; i < size; i++)
    point3d_display (&(list[i]));
}

/**
   Calcul le centre de gravite d'un ensemble de point
   @param data liste de point3d
   @param nbpoint nombre de point
   @return le centre de gravité
*/
/*Implémentation selon Besl p243*/
point3d *
center_of_mass (
		const point3d * const data,
		int nbpoint)
{
  point3d *center;

  center = (point3d *) malloc (sizeof (point3d));
  a2ri_erreur_critique_si (center == NULL,
			"erreur allocation memoire pour center\ncenter_of_mass");

  center->x = 0;
  center->y = 0;
  center->z = 0;

  for (int i = 0; i < nbpoint; i++)
    {
      center->x += data[i].x;
      center->y += data[i].y;
      center->z += data[i].z;
    }

  center->x /= (nbpoint * 1.0);
  center->y /= (nbpoint * 1.0);
  center->z /= (nbpoint * 1.0);

  return center;

}
