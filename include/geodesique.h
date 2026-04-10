/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 25/01/08       */
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



#ifndef GEODESIQUE__H
#define GEODESIQUE__H

#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "vertex.h"
#include "edge.h"
#include "face.h"
#include "point.h"
#include "model.h"
#include "geometry.h"
#include "hashtable.h"
#include "graph.h"
#include "subdivision.h"
#include "topology.h"

#define A2RI_GEODESIQUE_CHEMIN_INTROUVABLE 0
#define A2RI_GEODESIQUE_POINTS_ALIGNES 1
#define A2RI_GEODESIQUE_OK 2


/**
   Calcul du chemin le plus court entre deux sommets du modele
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @return la longueur du chemin
**/
double a2ri_vf_dijkstra (
			 const vf_model * const m,
			 int ve_dep,
			 int ve_fin,
			 int **list,
			 int *size);

/**
   Calcul d'un chemin entre deux sommets du modele
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @return la longueur du chemin
**/
double a2ri_vf_A_star (
			 const vf_model * const m,
			 int ve_dep,
			 int ve_fin,
			 int **list,
			 int *size);


/**
   Calcul du chemin le plus court entre deux sommets du modele
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @return la longueur du chemin
**/
double a2ri_vf_approche (
			 const vf_model * const m,
			 int ve_dep,
			 int ve_fin,
			 int **list,
			 int *size);


/**
   Calcul du chemin le plus court entre deux sommets du modele
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @return la longueur du chemin
**/
double a2ri_vef_dijkstra (
			  const vef_model * const m,
			  int ve_dep,
			  int ve_fin,
			  int **list,
			  int *size);

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return 1 si un chemin a été créé et trouvé, 0 sinon
**/
int a2ri_vf_geodesic_path_approche_plan_moyen (
					       vf_model * m,
					       int ve_dep,
					       int ve_fin,
					       int **list,
					       int *size,
					       double *length,
					       int nbsubdiv);


/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return 1 si un chemin a été créé et trouvé, 0 sinon
**/
int a2ri_vf_geodesic_path_approche_plan_minmax (
						vf_model * m,
						int ve_dep,
						int ve_fin,
						int **list,
						int *size,
						double *length,
						int nbsubdiv);

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return 1 si un chemin a été créé et trouvé, 0 sinon
**/
int a2ri_vf_geodesic_path_approche_plan_vecteur (
						 vf_model * m,
						 int ve_dep,
						 int ve_fin,
						 int **list,
						 int *size,
						 double *length,
						 int nbsubdiv);

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return 1 si un chemin a été créé et trouvé, 0 sinon
**/
int a2ri_vf_geodesic_path_approche_plan_intersection (
						      vf_model * m,
						      int ve_dep,
						      int ve_fin,
						      int **list,
						      int *size,
						      double *length,
						      int nbsubdiv);

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return 1 si un chemin a été créé et trouvé, 0 sinon
**/
int a2ri_vf_geodesic_path_A_star_plan_moyen (
					       vf_model * m,
					       int ve_dep,
					       int ve_fin,
					       int **list,
					       int *size,
					       double *length,
					       int nbsubdiv);


/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return 1 si un chemin a été créé et trouvé, 0 sinon
**/
int a2ri_vf_geodesic_path_A_star_plan_minmax (
						vf_model * m,
						int ve_dep,
						int ve_fin,
						int **list,
						int *size,
						double *length,
						int nbsubdiv);

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return 1 si un chemin a été créé et trouvé, 0 sinon
**/
int a2ri_vf_geodesic_path_A_star_plan_vecteur (
						 vf_model * m,
						 int ve_dep,
						 int ve_fin,
						 int **list,
						 int *size,
						 double *length,
						 int nbsubdiv);

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return 1 si un chemin a été créé et trouvé, 0 sinon
**/
int a2ri_vf_geodesic_path_A_star_plan_intersection (
						      vf_model * m,
						      int ve_dep,
						      int ve_fin,
						      int **list,
						      int *size,
						      double *length,
						      int nbsubdiv);

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return 1 si un chemin a été créé et trouvé, 0 sinon
**/
int a2ri_vf_geodesic_path_dijkstra_plan_minmax (
						vf_model * m,
						int ve_dep,
						int ve_fin,
						int **list,
						int *size,
						double *length,
						int nbsubdiv);

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return 1 si un chemin a été créé et trouvé, 0 sinon
**/
int a2ri_vf_geodesic_path_dijkstra_plan_vecteur (
						 vf_model * m,
						 int ve_dep,
						 int ve_fin,
						 int **list,
						 int *size,
						 double *length,
						 int nbsubdiv);

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return 1 si un chemin a été créé et trouvé, 0 sinon
**/
int a2ri_vf_geodesic_path_dijkstra_plan_intersection (
						      vf_model * m,
						      int ve_dep,
						      int ve_fin,
						      int **list,
						      int *size,
						      double *length,
						      int nbsubdiv);


/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return 1 si un chemin a été créé et trouvé, 0 sinon
**/
int a2ri_vf_geodesic_path_dijkstra_plan_moyen (
					       vf_model * m,
					       int ve_dep,
					       int ve_fin,
					       int **list,
					       int *size,
					       double *length,
					       int nbsubdiv);
#endif
