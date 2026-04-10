/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 03/04/07       */
/* Date de modification : 18/09/16   */
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



#ifndef HASH_QUALITY__H
#define HASH_QUALITY__H

#include <math.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include "util.h"
#include "vector.h"
#include "point.h"
#include "vertex.h"
#include "face.h"
#include "model.h"
#include "skeleton.h"
#include "matrix.h"
#include "geometry.h"
#include "hashtable.h"
#include "topology.h"

#define NO_SAMPLING 0
#define SAMPLING 1

/**
   Calcul de l'erreur mean ratio metric sur un modèle pour une face
   @param m le modèle
   @param numface numéro de la face à évaluer
   @return évaluation de la face
*/
double a2ri_vf_mean_ratio_metric_for_a_face (
					     const vf_model * const m,
					     int numface);

/**
   Calcul de l'erreur mean ratio metric sur un modèle complet
   @param m le modèle
   @return évaluation du modèle
*/
double a2ri_vf_mean_ratio_metric (
				  const vf_model * const m);

/**
   Calcule la liste des angles des triangles composant le modèle
   @param m le modèle
   @param list pointeur sur le tableau contenant la liste des angles
   @param size pointeur sur la taille du tableau
   @return aucun
*/
void a2ri_vf_list_angle (
			 const vf_model * const m,
			 double ** list,
			 int * size);

/**
   Calcule la liste des aires des triangles composant le modèle
   @param m le modèle
   @param list pointeur sur le tableau contenant la liste des aires
   @param size pointeur sur la taille du tableau
   @return aucun
*/
void a2ri_vf_list_area (
			const vf_model * const m,
			double ** list,
			int * size);

/**
   Calcule la liste des valences des sommets composant le modèle
   @param m le modèle
   @param list pointeur sur le tableau contenant la liste des valences
   @param size pointeur sur la taille du tableau
   @return aucun
*/
void a2ri_vf_list_valence (
			   const vf_model * const m,
			   int ** list,
			   int * size);

/**
   Calcule la liste des longueurs des aretes composant le modèle
   @param m le modèle
   @param list pointeur sur le tableau contenant la liste des longueurs des aretes
   @param size pointeur sur la taille du tableau
   @return aucun
*/
//On ne peut pas garantir le const du vf_model ici 
void a2ri_vf_list_edge_length (
			       vf_model * m,
			       double ** list,
			       int * size);

/**
   Calcule la liste des longueurs des hauteurs des triangles composant le modèle
   @param m le modèle
   @param list pointeur sur le tableau contenant la liste des hauteurs
   @param size pointeur sur la taille du tableau
   @return aucun
*/
//On ne peut pas garantir le const du vf_model ici 
void a2ri_vf_list_height_length (
				 vf_model * m,
				 double ** list,
				 int * size);

/**
   Compte le nombre d'angles du modèle par intervalle de 10 degres
   @param m le modèle
   @return un tableau de 18 entiers contenant le nombre d'angle compris entre 0-10, 10-20, ... , 170-180
*/
int * a2ri_vf_angle_measure (
			    const vf_model * const m);

/**
   Calcul de l'erreur de Hausdorff
   @param m1 premier modèle
   @param m2 second modèle
   @param sampling SAMPLING ou NO_SAMPLING <BR> indique s'il faut ou non échantillonner les faces.
   @return erreur de Hausdorff
*/
double a2ri_vf_hausdorff (
			  const vf_model * const m1,
			  const vf_model * const m2,
			  int sampling);

/**
   Calcul de la distance entre un point et un ensemble de points en se basant sur la métrique Point-Point
   @param p le point
   @param points tableau de réel contenant les coordoonées des sommets du maillage
   @param size taille du tableau
   @return distance entre le point et le modèle
*/
double DPP (
	    const point3d *const  p,
	    const point3d * const points,
	    int size);

/**
   calcul de l'erreur moyenne "Emean" entre deux modèles
   @param m1 le premier modèle
   @param m2 le second modèle
   @param sampling SAMPLING ou NO_SAMPLING <BR> indique s'il faut ou non échantillonner les faces.
   @return l'erreur "Emean"
*/
double a2ri_vf_Emn_DPP (
			const vf_model * const m1,
			const vf_model * const m2,
			int sampling);

/**
   Evaluation de l'aspect ratio d'un triangle
   @param m le modèle
   @param numface numéro de la face à évaluer
   @return évaluation de l'aspect ratio
*/
double a2ri_vf_triangle_aspect_ratio (
				      const vf_model * const m,
				      int numface);

/**
   Evaluation de la qualité d'une face d'un triangle avec la méthode de Rypl
   @param m le modèle
   @param numface numéro de la face à évaluer
   @return évaluation de l'aspect ratio
*/
double a2ri_vf_triangle_rypl (
			      const vf_model * const m,
			      int numface);

/**
   Calcul de la liste d'aspect ratio d'un modèle
   @param m le modèle
   @param list pointeur sur un tableau contenant tous les aspect ratio des triangles composant le modèle
   @param siuze pointeur sur la taille du tableau
   @return aucun
*/
void a2ri_vf_list_triangle_aspect_ratio (
					 const vf_model * const m,
					 double ** list,
					 int  *size);


/**
   Calcul de la variation de taille (d'aire) entre deux faces du modèle
   @param m le modèle
   @param face1 numéro de la première face servant de base
   @param face2 numéro de la seconde face
   @return variation de la taille : Aire_face1/Aire_face2
*/
double a2ri_vf_face_size_variation (
				    const vf_model * const m,
				    int face1,
				    int face2);

/**
   Calcul de la liste des rayons des cercles inscrit aux faces du modèles
   @param m le modèle
   @param list pointeur sur le tableau contenant la liste des rayons des cercles inscrits
   @param size pointeur sur la taille du tableau
   @return aucun
*/
void a2ri_vf_list_radius_incircle (
				   const vf_model * const m,
				   double ** list,
				   int * size);

/**
   Calcul de la courbure de gauss pour un sommet
   @param m le modele
   @param ve numéro du sommet
   @return le courbure de gauss en ce sommet
*/
double a2ri_vf_gauss_curvature (
				const vf_model * const m,
				int ve);

/**
   Calcul de la courbure de gauss pour une liste de sommet
   @param m le modele
   @param ve liste de sommets
   @param size taille de la liste
   @param list liste des courbures associés à chaque sommet
   @return aucun
*/
void a2ri_vf_gauss_curvature_list_of_vertex (
					     const vf_model * const m,
					     const int * const ve,
					     int size,
					     double ** list);

/**
   Calcul de la mean curvature pour une arete
   @param m le modele
   @param ve1 premier sommet de l'arete
   @param ve2 second sommet de l'arete
   @return la mean curvature pour cette arete
*/
vector3d a2ri_vf_mean_curvature (
				 const vf_model * const m,
				 int ve1,
				 int ve2);

/**
   Calcul de la mean curvature pour un chemin
   @param m le modele
   @param list chemin décrit par ses sommets
   @param size taille du tableau list
   @param list_vector liste des vector3d retourner pour chaque arete
   @param size_list_vector taille de la liste list_vector
   @return aucun
*/
void a2ri_vf_mean_curvature_path (
				  const vf_model * const m,
				  const int * const list,
				  int size,
				  vector3d ** list_vector,
				  int * size_list_vector);

/**
   Calcul de la courbure de Levi-Civita
   @param m le modele
   @param numve numéro du sommet
   @return la courbure de Levi-Civita
*/
double a2ri_vf_levi_civita (
			    const vf_model * const m,
			    int numve);

/**
   Calcul de la courbure de Levi-Civita
   @param m le modele
   @param numve numéro du sommet
   @return la courbure de Levi-Civita
*/
void a2ri_vf_levi_civita_list_of_vertex (
					 const vf_model * const m,
					 const int * const ve,
					 int size,
					 double ** list);

/**
   Calcul des courbures avec la méthode garimella
   @param m le modele
   @param numve numéro du sommet
   @param K gaussian curvature
   @param H mean curvature
   @return aucun
**/
void a2ri_vf_garimella (
			const vf_model * const m,
			int numve,
			double * K,
			double * H);

/**
   Calcul de la précision, justesse d'un squelette par rapport à un modèle numérique
   @param m le modèle
   @param s le squelette
   @return un nombre définissant la précision du squelette par rapport au modèle. Plus ce nombre sera élevé, moins le squelette sera ajusté au modèle.
 **/
int a2ri_vf_model_skeleton_accuracy(
				    const vf_model * const m,
				    const skeleton * const s);

#endif
