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



#ifndef UTIL__H
#define UTIL__H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "config.h"
#include "debug.h"

#define WITH_REDUNDANCE 0
#define WITHOUT_REDUNDANCE 1
#define ASC 0
#define DESC 1

#define egalite(val1,val2) (val1>=val2-A2RI_PRECISION_FLOAT && val1<=val2+A2RI_PRECISION_FLOAT)

typedef struct timeval a2ri_time;

/**
   Affiche un message d'erreur et stoppe le programme si la condition n'est pas satisfaite
   @param cond condition à satisfaire
   @param str message d'erreur à afficher dans le cas contraire
   @return aucun
**/
void a2ri_erreur_critique_si (
			   int cond,
			   const char * const str);

/**
   Renvoie un entier compris entre min et max
   @param min valeur minimale
   @param max valeur maximale
   @return un entier
**/
int rand_int (
	      int min,
	      int max);

/**
   Retourne la position de la valeur tosearch, -1 sinon
   @param list tableau d'entier
   @param size taille du tableau
   @param tosearch entier à chercher
   @return position de la première occurence, -1 s'il n'apparait pas dans le tableau
*/
int list_int_contains (
		       const int * const list,
		       int size,
		       int tosearch);

/**
   Retourne la position de la valeur tosearch, -1 sinon
   @param list tableau de réels
   @param size taille du tableau
   @param tosearch réel à chercher
   @return position de la première occurence, -1 s'il n'apparait pas dans le tableau
*/
int list_double_contains (
			  const double * const list,
			  int size,
			  double tosearch);

/**
   Clone la liste
   @param list la liste à cloner
   @param size taille de la liste
   @param list_clone liste clonée
   @return 1 si la liste est bien clonée, 0 sinon
**/
int list_int_clone (
		    const int * const list,
		    int size,
		    int **list_clone);

/**
   Clone la liste
   @param list la liste à cloner
   @param size taille de la liste
   @param list_clone liste clonée
   @return 1 si la liste est bien clonée, 0 sinon
**/
int list_double_clone (
		       const double * const list,
		       int size,
		       double **list_clone);

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
void list_int_intersection (
			    const int * const list1,
			    int size1,
			    const int * const list2,
			    int size2,
			    int **list_inter,
			    int *size_list_inter);

/**
   Ajoute l'entier toadd en fin de liste
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param toadd entier à ajouter
   @param add_type WITH_REDUNDANCE ou WITHOUT_REDUNDANCE <BR> avec redondance : ajout simple <BR> sans redondance : ajout si la valeur n'apparait pas dans la liste
   @return 1 si succès, 0 sinon
*/
int list_int_add (
		  int **list,
		  int *size,
		  int toadd,
		  int add_type);

/**
   Ajoute le réel toadd en fin de liste
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param toadd réel à ajouter
   @param add_type WITH_REDUNDANCE ou WITHOUT_REDUNDANCE <BR> avec redondance : ajout simple <BR> sans redondance : ajout si la valeur n'apparait pas dans la liste
   @return 1 si succès, 0 sinon
*/
int list_double_add (
		     double **list,
		     int *size,
		     double toadd,
		     int add_type);

/**
   Enlève la valeur à la position index
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param index position de l'entier à supprimer
   @return 1 si succès, 0 sinon
*/
int list_int_remove (
		     int **list,
		     int *size,
		     int index);

/**
   Enlève la valeur à la position index
   @param list pointeur sur le premier élément du tableau
   @param size pointeur sur la taille du tableau
   @param index position du réel à supprimer
   @return 1 si succès, 0 sinon
*/
int list_double_remove (
			double **list,
			int *size,
			int index);


/**
   Mélange la liste d'entier
   @param list tableau d'entier
   @param size taille du tableau
   @return aucun
*/
void list_int_mix (
		   int *list,
		   int size);

/**
   Mélange la liste de réel
   @param list tableau de réel
   @param size taille du tableau
   @return aucun
*/
void list_double_mix (
		      double *list,
		      int size);

/**
   Affichage de la liste d'entier
   @param list tableau d'entier
   @param size taille du tableau
   @return aucun
*/
void list_int_display (
		       const int * const list,
		       int size);

/**
   Affichage de la liste de réel
   @param list tableau de réel
   @param size taille du tableau
   @return aucun
*/
void list_double_display (
			  const double * const list,
			  int size);

/**
   Inverse la liste d'entier
   @param list tableau d'entier
   @param size taille du tableau
   @return aucun
*/
void list_int_reverse (
		       int *list,
		       int size);

/**
   Inverse la liste de réel
   @param list tableau de réel
   @param size taille du tableau
   @return aucun
*/
void list_double_reverse (
			  double *list,
			  int size);

/**
   Décale la liste d'entier à droite de "shift" position(s)
   @param list tableau d'entier
   @param size taille du tableau
   @param shift nombre de décalage à droite
   @return aucun
*/
void list_int_shift_right (
			   int *list,
			   int size,
			   int shift);

/**
   Décale la liste d'entier à gauche de "shift" position(s)
   @param list tableau d'entier
   @param size taille du tableau
   @param shift nombre de décalage à gauche
   @return aucun
*/
void list_int_shift_left (
			  int *list,
			  int size,
			  int shift);

/**
   Décale la liste de réel à droite de "shift" position(s)
   @param list tableau de réel
   @param size taille du tableau
   @param shift nombre de décalage à droite
   @return aucun
*/
void list_double_shift_right (
			      double *list,
			      int size,
			      int shift);

/**
   Décale la liste de réel à gauche de "shift" position(s)
   @param list tableau de réel
   @param size taille du tableau
   @param shift nombre de décalage à gauche
   @return aucun
*/
void list_double_shift_left (
			     double *list,
			     int size,
			     int shift);

/**
   Trouve le plus petit entier de la liste
   @param list tableau d'entier
   @param size taille du tableau
   @return le plus petit entier
*/
int list_int_min (
		  const int * const list,
		  int size);

/**
   Trouve le plus grand entier de la liste
   @param list tableau d'entier
   @param size taille du tableau
   @return le plus grand entier
*/
int list_int_max (
		  const int * const list,
		  int size);

/**
   Trouve le plus petit réel de la liste
   @param list tableau de réel
   @param size taille du tableau
   @return le plus petit réel
*/
double list_double_min (
			const double * const list,
			int size);

/**
   Trouve le plus grand réel de la liste
   @param list tableau de réel
   @param size taille du tableau
   @return le plus grand réel
*/
double list_double_max (
			const double * const list,
			int size);

/**
   Calcul de la moyenne d'une liste d'entier
   @param list tableau d'entier
   @param size taille du tableau
   @return moyenne de la liste
*/
double list_int_average (
			 const int * const list,
			 int size);

/**
   Calcul de la variance d'une liste d'entier
   @param list tableau d'entier
   @param size taille du tableau
   @return variance de la liste
*/
double list_int_variance (
			  const int * const list,
			  int size);

/**
   Calcul de la moyenne d'une liste de réel
   @param list tableau de réel
   @param size taille du tableau
   @return moyenne de la liste
*/
double list_double_average (
			    const double * const list,
			    int size);

/**
   Calcul de la variance d'une liste de réel
   @param list tableau de réel
   @param size taille du tableau
   @return variance de la liste
*/
double list_double_variance (
			     const double * const list,
			     int size);

/**
   Tri de la liste d'entier
   @param list tableau d'entier
   @param size taille du tableau
   @param type ASC ou DESC : ordre croissant ou décroissant
   @return aucun
*/
void list_int_sort (
		    int *list,
		    int size,
		    int type);

/**
   Tri de la liste de réel
   @param list tableau de réel
   @param size taille du tableau
   @param type ASC ou DESC : ordre croissant ou décroissant
   @return aucun
*/
void list_double_sort (
		       double *list,
		       int size,
		       int type);

/**
   Initialisation à zéro d'un temps
   @param t pointeur vers un objet temps
   @return aucun
*/
void a2ri_time_init (
		     a2ri_time * t);

/**
   Acquisition de l'heure actuelle
   @param t pointeur vers un objet temps
   @return aucun
*/
a2ri_time a2ri_get_time (
  );

/**
   Conversion d'un temps en double
   @param dep le temps inférieur
   @param fin le temps supérieur
   @return la conversion en double
*/
double a2ri_time_interval_to_double (
				     a2ri_time deb,
				     a2ri_time fin);

/**
   Affichage du temps séparant les deux variables en secondes
   @param debut borne de début
   @param fin borne de fin
   @return aucun
*/
void a2ri_display_interval_time (
				 const char * const str,
				 a2ri_time deb,
				 a2ri_time fin);

#endif
