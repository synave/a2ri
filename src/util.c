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

#include "util.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

int
IFlist_int_etape_tri_rapide (
			   int *t,
			   int min,
			   int max,
			   int type)
{
  int temp = t[max];
  
  while (max > min)
    {
      if (type == ASC)
	{
	  while (max > min && t[min] <= temp)
	    min++;
	}
      else
	{
	  while (max > min && t[min] > temp)
	    min++;
	}
      if (max > min)
	{
	  t[max] = t[min];
	  max--;
	  if (type == ASC)
	    {
	      while (max > min && t[max] >= temp)
		max--;
	    }
	  else
	    {
	      while (max > min && t[max] < temp)
		max--;
	    }
	  if (max > min)
	    {
	      t[min] = t[max];
	      min++;
	    }
	}
    }
  t[max] = temp;
  return (max);
}

void
IFlist_int_tri_rapide (
		     int *t,
		     int deb,
		     int fin,
		     int type)
{
  int mil;
  
  if (deb < fin)
    {
      mil = IFlist_int_etape_tri_rapide (t, deb, fin, type);
      if (mil - deb > fin - mil)
	{
	  IFlist_int_tri_rapide (t, mil + 1, fin, type);
	  IFlist_int_tri_rapide (t, deb, mil - 1, type);
	}
      else
	{
	  IFlist_int_tri_rapide (t, deb, mil - 1, type);
	  IFlist_int_tri_rapide (t, mil + 1, fin, type);
	}
    }
}

int
IFlist_double_etape_tri_rapide (
			      double *t,
			      int min,
			      int max,
			      int type)
{
  double temp = t[max];
  
  while (max > min)
    {
      if (type == ASC)
	{
	  while (max > min && t[min] <= temp)
	    min++;
	}
      else
	{
	  while (max > min && t[min] > temp)
	    min++;
	}
      if (max > min)
	{
	  t[max] = t[min];
	  max--;
	  if (type == ASC)
	    {
	      while (max > min && t[max] >= temp)
		max--;
	    }
	  else
	    {
	      while (max > min && t[max] < temp)
		max--;
	    }
	  if (max > min)
	    {
	      t[min] = t[max];
	      min++;
	    }
	}
    }
  t[max] = temp;
  return (max);
}

void
IFlist_double_tri_rapide (
			double *t,
			int deb,
			int fin,
			int type)
{
  int mil;
  
  if (deb < fin)
    {
      mil = IFlist_double_etape_tri_rapide (t, deb, fin, type);
      if (mil - deb > fin - mil)
	{
	  IFlist_double_tri_rapide (t, mil + 1, fin, type);
	  IFlist_double_tri_rapide (t, deb, mil - 1, type);
	}
      else
	{
	  IFlist_double_tri_rapide (t, deb, mil - 1, type);
	  IFlist_double_tri_rapide (t, mil + 1, fin, type);
	}
    }
}

/********** MAIN FUNCTIONS **********/

/**
   Affiche un message d'erreur et stoppe le programme si la condition n'est pas satisfaite
   @param cond condition à satisfaire
   @param str message d'erreur à afficher dans le cas contraire
   @return aucun
**/
void
a2ri_erreur_critique_si (
		      int cond,
		      const char * const str)
{
  if (cond)
    {
      a2ri_printf_debug ("%s\n", str);
      exit (EXIT_FAILURE);
    }
}

/**
   Renvoie un entier compris entre min et max
   @param min valeur minimale
   @param max valeur maximale
   @return un entier
**/
int
rand_int (
	  int min,
	  int max)
{
  return ((((int) rand ()) % (max - min + 1)) + min);
}

/**
   Retourne la position de la valeur tosearch, -1 sinon
   @param list tableau d'entier
   @param size taille du tableau
   @param tosearch entier à chercher
   @return position de la première occurence, -1 s'il n'apparait pas dans le tableau
*/
int
list_int_contains (
		   const int * const list,
		   int size,
		   int tosearch)
{
  int i;
  for (i = 0; i < size; i++)
    if (list[i] == tosearch)
      return i;
  return -1;
}

/**
   Retourne la position de la valeur tosearch, -1 sinon
   @param list tableau de réels
   @param size taille du tableau
   @param tosearch réel à chercher
   @return position de la première occurence, -1 s'il n'apparait pas dans le tableau
*/
int
list_double_contains (
		      const double * const list,
		      int size,
		      double tosearch)
{
  int i;
  for (i = 0; i < size; i++)
    if (list[i] == tosearch)
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
list_int_clone (
		const int * const list,
		int size,
		int **list_clone)
{
  (*list_clone) = (int *) malloc (size * sizeof (int));
  a2ri_erreur_critique_si (*list_clone == NULL,
			"erreur allocation memoire pour list_clone\nlist_int_clone");

  if ((*list_clone) == NULL)
    return 0;

  for (int i = 0; i < size; i++)
    (*list_clone)[i] = list[i];

  return 1;
}

/**
   Clone la liste
   @param list la liste à cloner
   @param size taille de la liste
   @param list_clone liste clonée
   @return 1 si la liste est bien clonée, 0 sinon
**/
int
list_double_clone (
		   const double * const list,
		   int size,
		   double **list_clone)
{
  (*list_clone) = (double *) malloc (size * sizeof (double));
  a2ri_erreur_critique_si (*list_clone == NULL,
			"erreur allocation memoire pour list_clone\nlist_double_clone");

  if ((*list_clone) == NULL)
    return 0;

  for (int i = 0; i < size; i++)
    (*list_clone)[i] = list[i];

  return 1;
}

/**
   calcul de l'intersection de deux liste d'entier
   @param list1 premiere liste d'entier
   @param size1 taille de la premiere liste
   @param list2 seconde liste d'entier
   @param size2 taille de la seconde liste
   @param list_inter liste d'entier représentatnt l'intersection de list1 et list2
   @param size_list_inter taille de la liste d'intersection
   @return aucun
*/
void
list_int_intersection (
		       const int * const list1,
		       int size1,
		       const int * const list2,
		       int size2,
		       int **list_inter,
		       int *size_list_inter)
{
  *list_inter = NULL;
  *size_list_inter = 0;
  for (int i = 0; i < size1; i++)
    {
      if (list_int_contains (list2, size2, list1[i]) != -1)
	list_int_add (list_inter, 
		      size_list_inter, 
		      list1[i],
		      WITHOUT_REDUNDANCE);
    }
}

/**
   Ajoute l'entier toadd en fin de liste
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param toadd entier à ajouter
   @param add_type WITH_REDUNDANCE ou WITHOUT_REDUNDANCE <BR> avec redondance : ajout simple <BR> sans redondance : ajout si la valeur n'apparait pas dans la liste
   @return 1 si succès, 0 sinon
*/
int
list_int_add (
	      int **list,
	      int *size,
	      int toadd,
	      int add_type)
{
  if (add_type == WITHOUT_REDUNDANCE)
    if (list_int_contains (*list, *size, toadd) != -1)
      return 0;
  

  int *newlist = (int *) malloc (((*size) + 1) * sizeof (int));
  a2ri_erreur_critique_si (newlist == NULL,
			   "erreur allocation memoire pour newlist\nlist_int_add");
  
  int i;
  for (i = 0; i < *size; i++)
    newlist[i] = (*list)[i];
  newlist[*size] = toadd;
  free (*list);
  (*list) = newlist;
  *size = (*size) + 1;
  
  return 1;
}

/**
   Ajoute le réel toadd en fin de liste
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param toadd réel à ajouter
   @param add_type WITH_REDUNDANCE ou WITHOUT_REDUNDANCE <BR> avec redondance : ajout simple <BR> sans redondance : ajout si la valeur n'apparait pas dans la liste
   @return 1 si succès, 0 sinon
*/
int
list_double_add (
		 double **list,
		 int *size,
		 double toadd,
		 int add_type)
{
  if (add_type == WITHOUT_REDUNDANCE)
    if (list_double_contains (*list, *size, toadd) != -1)
      return 0;
  
  double *newlist = (double *) malloc (((*size) + 1) * sizeof (double));
  a2ri_erreur_critique_si (newlist == NULL,
			   "erreur allocation memoire pour newlist\nlist_double_add");
  int i;
  for (i = 0; i < *size; i++)
    newlist[i] = (*list)[i];
  newlist[*size] = toadd;
  free (*list);
  (*list) = newlist;
  *size = (*size) + 1;
  
  return 1;
}

/**
   Enlève la valeur à la position index
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param index position de l'entier à supprimer
   @return 1 si succès, 0 sinon (cas oÃ¹ index>=size)
*/
int
list_int_remove (
		 int **list,
		 int *size,
		 int index)
{
  int *temp;

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
    (*list)[i] = (*list)[i + 1];
  temp = (int *) realloc (*list, ((*size) - 1) * sizeof (int));
  a2ri_erreur_critique_si (temp == NULL && *size != 0,
			    "erreur allocation memoire pour temp\nlist_int_remove");
  *list=temp;
  *size = (*size) - 1;
  a2ri_erreur_critique_si (*list == NULL && *size != 0,
			"erreur allocation memoire pour list\nlist_int_remove");

  return 1;
}

/**
   Enlève la valeur à la position index
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param index position du réel à supprimer
   @return 1 si succès, 0 sinon (cas oÃ¹ index>=size)
*/
int
list_double_remove (
		    double **list,
		    int *size,
		    int index)
{
  double *temp;

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
    (*list)[i] = (*list)[i + 1];
  temp = (double *) realloc (*list, ((*size) - 1) * sizeof (double));
  a2ri_erreur_critique_si (temp == NULL && *size != 0,
			    "erreur allocation memoire pour temp\nlist_double_remove");
  *list=temp;
  *size = (*size) - 1;
  a2ri_erreur_critique_si (*list == NULL && *size != 0,
			"erreur allocation memoire pour list\nlist_double_remove");

  return 1;
}

/**
   Mélange la liste d'entier
   @param list tableau d'entier
   @param size taille du tableau
   @return aucun
*/
void
list_int_mix (
	      int *list,
	      int size)
{
  int *list_clone = NULL,
    size_clone;
  list_int_clone (list, size, &list_clone);
  size_clone = size;

  for (int i = 0; i < size; i++)
    {
      int index = rand_int (0, size_clone - 1);
      list[i] = list_clone[index];
      list_int_remove (&list_clone, &size_clone, index);
    }

}

/**
   Mélange la liste de réel
   @param list tableau de réel
   @param size taille du tableau
   @return aucun
*/
void
list_double_mix (
		 double *list,
		 int size)
{
  double *list_clone = NULL;
  int size_clone;
  list_double_clone (list, size, &list_clone);
  size_clone = size;


  for (int i = 0; i < size; i++)
    {
      int index = rand_int (0, size_clone - 1);
      list[i] = list_clone[index];
      list_double_remove (&list_clone, &size_clone, index);
    }
}

/**
   Affichage de la liste d'entier
   @param list tableau d'entier
   @param size taille du tableau
   @return aucun
*/
void
list_int_display (
		  const int * const list,
		  int size)
{
  int i;
  if (size == 0)
    return;
  printf ("liste :\n%d", list[0]);
  for (i = 1; i < size; i++)
    printf (" - %d", list[i]);
  printf ("\n");
}

/**
   Affichage de la liste de réel
   @param list tableau de réel
   @param size taille du tableau
   @return aucun
*/
void
list_double_display (
		     const double * const list,
		     int size)
{
  int i;
  if (size == 0)
    return;
  printf ("liste :\n%lf", list[0]);
  for (i = 1; i < size; i++)
    printf (" - %lf", list[i]);
  printf ("\n");
}

/**
   Inverse la liste d'entier
   @param list tableau d'entier
   @param size taille du tableau
   @return aucun
*/
void
list_int_reverse (
		  int *list,
		  int size)
{
  int *listechang = (int *) malloc (size * sizeof (int));
  a2ri_erreur_critique_si (listechang == NULL,
			"erreur allocation memoire pour list\nlist_int_reverse");
  int i;

  for (i = 0; i < size; i++)
    listechang[i] = list[i];

  for (i = 0; i < size; i++)
    list[i] = listechang[size - 1 - i];

  free (listechang);
}

/**
   Inverse la liste de réel
   @param list tableau de réel
   @param size taille du tableau
   @return aucun
*/
void
list_double_reverse (
		     double *list,
		     int size)
{
  double *listechang = (double *) malloc (size * sizeof (double));
  a2ri_erreur_critique_si (listechang == NULL,
			"erreur allocation memoire pour list\nlist_double_reverse");
  int i;

  for (i = 0; i < size; i++)
    listechang[i] = list[i];

  for (i = 0; i < size; i++)
    list[i] = listechang[size - 1 - i];

  free (listechang);
}

/**
   Décale la liste d'entier à droite de "shift" position(s)
   @param list tableau d'entier
   @param size taille du tableau
   @param shift nombre de décalage à droite
   @return aucun
*/
void
list_int_shift_right (
		      int *list,
		      int size,
		      int shift)
{
  int *listechang = (int *) malloc (size * sizeof (int));
  a2ri_erreur_critique_si (listechang == NULL,
			"erreur allocation memoire pour list\nlist_int_shift_right");
  int i;

  for (i = 0; i < size; i++)
    listechang[i] = list[i];

  for (i = 0; i < size; i++)
    list[i] = listechang[(i - shift + size) % size];

  free (listechang);
}

/**
   Décale la liste d'entier à gauche de "shift" position(s)
   @param list tableau d'entier
   @param size taille du tableau
   @param shift nombre de décalage à gauche
   @return aucun
*/
void
list_int_shift_left (
		     int *list,
		     int size,
		     int shift)
{
  int *listechang = (int *) malloc (size * sizeof (int));
  a2ri_erreur_critique_si (listechang == NULL,
			"erreur allocation memoire pour list\nlist_int_shift_left");
  int i;

  for (i = 0; i < size; i++)
    listechang[i] = list[i];

  for (i = 0; i < size; i++)
    list[i] = listechang[(i + shift) % size];

  free (listechang);
}

/**
   Décale la liste de réel à droite de "shift" position(s)
   @param list tableau de réel
   @param size taille du tableau
   @param shift nombre de décalage à droite
   @return aucun
*/
void
list_double_shift_right (
			 double *list,
			 int size,
			 int shift)
{
  double *listechang = (double *) malloc (size * sizeof (double));
  a2ri_erreur_critique_si (listechang == NULL,
			"erreur allocation memoire pour list\nlist_double_shift_right");
  int i;

  for (i = 0; i < size; i++)
    listechang[i] = list[i];

  for (i = 0; i < size; i++)
    list[i] = listechang[(i - shift + size) % size];

  free (listechang);
}

/**
   Décale la liste de réel à gauche de "shift" position(s)
   @param list tableau de réel
   @param size taille du tableau
   @param shift nombre de décalage à gauche
   @return aucun
*/
void
list_double_shift_left (
			double *list,
			int size,
			int shift)
{
  double *listechang = (double *) malloc (size * sizeof (double));
  a2ri_erreur_critique_si (listechang == NULL,
			"erreur allocation memoire pour list\nlist_double_shift_left");
  int i;

  for (i = 0; i < size; i++)
    listechang[i] = list[i];

  for (i = 0; i < size; i++)
    list[i] = listechang[(i + shift) % size];

  free (listechang);
}

/**
   Trouve le plus petit entier de la liste
   @param list tableau d'entier
   @param size taille du tableau
   @return le plus petit entier
*/
int
list_int_min (
	      const int * const list,
	      int size)
{
  int min = list[0];
  for (int i = 1; i < size; i++)
    if (list[i] < min)
      min = list[i];

  return min;
}

/**
   Trouve le plus grand entier de la liste
   @param list tableau d'entier
   @param size taille du tableau
   @return le plus grand entier
*/
int
list_int_max (
	      const int * const list,
	      int size)
{
  int max = list[0];
  for (int i = 1; i < size; i++)
    if (list[i] > max)
      max = list[i];

  return max;
}

/**
   Trouve le plus petit réel de la liste
   @param list tableau de réel
   @param size taille du tableau
   @return le plus petit réel
*/
double
list_double_min (
		 const double * const list,
		 int size)
{
  double min = list[0];
  for (int i = 1; i < size; i++)
    if (list[i] < min)
      min = list[i];

  return min;
}

/**
   Trouve le plus grand réel de la liste
   @param list tableau de réel
   @param size taille du tableau
   @return le plus grand réel
*/
double
list_double_max (
		 const double * const list,
		 int size)
{
  double max = list[0];
  for (int i = 1; i < size; i++)
    if (list[i] > max)
      max = list[i];

  return max;
}

/**
   Calcul de la moyenne d'une liste d'entier
   @param list tableau d'entier
   @param size taille du tableau
   @return moyenne de la liste
*/
double
list_int_average (
		  const int * const list,
		  int size)
{
  int somme = 0;
  for (int i = 0; i < size; i++)
    somme += list[i];
  return (somme * 1.0) / (size * 1.0);
}

/**
   Calcul de la variance d'une liste d'entier
   @param list tableau d'entier
   @param size taille du tableau
   @return variance de la liste
*/
double
list_int_variance (
		   const int * const list,
		   int size)
{
  double moyenne = list_int_average (list, size),
    somme = 0;
  for (int i = 0; i < size; i++)
    somme += (list[i] - moyenne) * (list[i] - moyenne);
  return somme / (size * 1.0);
}

/**
   Calcul de la moyenne d'une liste de réel
   @param list tableau de réel
   @param size taille du tableau
   @return moyenne de la liste
*/
double
list_double_average (
		     const double * const list,
		     int size)
{
  double somme = 0;
  for (int i = 0; i < size; i++)
    somme += list[i];
  return somme / (size * 1.0);
}

/**
   Calcul de la variance d'une liste de réel
   @param list tableau de réel
   @param size taille du tableau
   @return variance de la liste
*/
double
list_double_variance (
		      const double * const list,
		      int size)
{
  double moyenne = list_double_average (list, size);
  double somme = 0;
  for (int i = 0; i < size; i++)
    somme += (list[i] - moyenne) * (list[i] - moyenne);
  return somme / (size * 1.0);
}

/**
   Tri de la liste d'entier
   @param list tableau d'entier
   @param size taille du tableau
   @param type ASC ou DESC : ordre croissant ou décroissant
   @return aucun
*/
void
list_int_sort (
	       int *list,
	       int size,
	       int type)
{
  IFlist_int_tri_rapide (list, 0, size - 1, type);
}

/**
   Tri de la liste de réel
   @param list tableau de réel
   @param size taille du tableau
   @param type ASC ou DESC : ordre croissant ou décroissant
   @return aucun
*/
void
list_double_sort (
		  double *list,
		  int size,
		  int type)
{
  IFlist_double_tri_rapide (list, 0, size - 1, type);
}

/**
   Initialisation à zéro d'un temps
   @param t pointeur vers un objet temps
   @return aucun
*/
void
a2ri_time_init (
		a2ri_time * t)
{
  t->tv_sec = 0;
  t->tv_usec = 0;
}

/**
   Acquisition de l'heure actuelle
   @param t pointeur vers un objet temps
   @return aucun
*/
a2ri_time
a2ri_get_time (
  )
{
  a2ri_time retour;
  gettimeofday (&retour, NULL);
  return retour;
}

/**
   Conversion d'un temps en double
   @param deb le temps inférieur
   @param fin le temps supérieur
   @return la conversion en double
*/
double
a2ri_time_interval_to_double (
			      a2ri_time deb,
			      a2ri_time fin)
{
  return ((fin.tv_sec - deb.tv_sec) * 1.0) +
    ((fin.tv_usec - deb.tv_usec) / 1000000.0);
}

/**
   Affichage du temps séparant les deux variables en secondes
   @param debut borne de début
   @param fin borne de fin
   @return aucun
*/
void
a2ri_display_interval_time (
			    const char * const str,
			    a2ri_time deb,
			    a2ri_time fin)
{
  printf ("%s%lf\n", 
	  str,
	  ((fin.tv_sec - deb.tv_sec) * 1.0) +
	  ((fin.tv_usec - deb.tv_usec) / 1000000.0));
}
