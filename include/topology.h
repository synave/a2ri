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



#ifndef TOPOLOGY__H
#define TOPOLOGY__H

#define BYVERTEX 0
#define BYEDGE 1

#include "model.h"
#include "util.h"
#include "hashtable.h"

/**
   Recherche du voisinage pour une liste de faces et à une profondeur donnée
   @param type BYVERTEX ou BYEDGE <BR> BYVERTEX : recherche du voisinage par sommet <BR> BYEDGE : recherche du voisinage par arete.
   @param m le modèle
   @param faces tableau contenant les numéros des faces de départ
   @param nbfaces taille du tableau
   @param list pointeur sur le tableau des faces contenu dans le voisinage
   @param size pointeur sur le nombre de faces dans le voisinage
   @param depth profondeur du voisinage
   @return aucun
*/
void a2ri_vf_star (
		   int type,
		   const vf_model * const m,
		   const int * const faces,
		   int nbfaces,
		   int **list,
		   int *size,
		   int depth);

/**
   Calcul du nombre de trous/nombre de limites dans le modèle
   @param m le modèle
   @return nombre de trous/limites
*/
int a2ri_vf_nb_hole (
		     const vf_model * const m);

/**
   Calcul du nombre de trous/nombre de limites dans le modèle
   @param m le modèle
   @return nombre de trous/limites
*/
int a2ri_vef_nb_hole (
		      const vef_model * const m);

/**
   Calcul du nombre de parties connexes contenu dans le modèle
   @param m le modèle
   @param list tableau d'entier représentant les faces du modèle et contenant le numéro de la partie à laquelle la face appartient.
   @return nombre de parties connexes
*/
int a2ri_vf_nb_connected_part (
			       const vf_model * const m,
			       int **list);

/**
   Calcul du nombre de parties connexes contenu dans le modèle
   @param m le modèle
   @param list tableau d'entier représentant les faces du modèle et contenant le numéro de la partie à laquelle la face appartient.
   @return nombre de parties connexes
*/
int a2ri_vef_nb_connected_part (
				const vef_model * const m,
				int **list);

/**
   Cherche un trou/cycle dans le maillage contenant les deux sommets
   @param m le modele
   @param ve1 premier sommet qui doit etre contenu dans le cycle
   @param ve2 second sommet qui doit etre contenu dans le cycle
   @param list liste des sommets formant le cycle
   @param size taille de la liste
**/
void a2ri_vf_search_hole_contains (
				   const vf_model * const m,
				   int ve1,
				   int ve2,
				   int ** list,
				   int * size);

#endif
