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



#ifndef HASHTABLE__H
#define HASHTABLE__H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#include "edge.h"
#include "util.h"


struct mycontainer
{
  struct mycontainer *next;
  vf_edge *data;
};

typedef struct mycontainer container;


typedef struct
{
  int size_array;
  int count;
  container **list;
} hashtable;

typedef void (
	      *ptf_foreach) (
			     int,
			     vf_edge *,
			     void *);


/**
   Création d'une table de hachage
   @param size taille de la table de hachage
   @return la nouvelles table de hachage
**/
hashtable *hashtable_new (
			  int size);


/**
   Destruction de la table de hachage avec libération de la mémoire
   @param table la table de hachage
   @return aucun;
**/
void hashtable_free (
		     hashtable * table);


/**
   TODO Ajouter le const sur l'arete qui est en parametre
   Ajout d'une arete dans la table de hachage
   @param table la table de hachage
   @param e l'arete a ajouter
   @return aucun
**/
void hashtable_add (
		    hashtable * table,
		    vf_edge * e);


/**
   Recherche d'une arete dans la table de hachage
   @param ve1 premier sommet de l'arete
   @param ve2 second sommet de l'arete
   @return l'arete si elle se trouve dans la table de hachage, NULL sinon.
**/
vf_edge *hashtable_look_for (
			     const hashtable * const table,
			     int ve1,
			     int ve2);


/**
   Affichage d'une table de hachage
   @param table la table de hachage
   @return aucun
**/
void hashtable_display (
			const hashtable * const table);


/**
   Retourne la taille de la table, le nombre d'éléments
   @param table la table de hachage
   @return aucun
**/
int hashtable_size (
		    const hashtable * const table);



/**
   Applique la fonction passé en paramètre à tous les éléments contenus dans la table
   @param table la table de hachage
   @param func la fonction à appliquer
   @param user_data un pointeur vers des données que l'on veut passé en paramètre à la fonction
   @return aucun
**/
void hashtable_foreach (
			hashtable * table,
			ptf_foreach func,
			void *user_data);

#endif
