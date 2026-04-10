/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 01/03/07       */
/* Date de modification : 15/03/15   */
/* Version : 0.4                     */
/*************************************/

/*************************************/
/* Auteur : Romain Leguay            */
/*          Nguyen Haiduong          */
/*          Marianne Fichoux         */
/* Date de modification : 26/05/09   */
/* Version : 0.2                     */
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



#ifndef MODEL__H
#define MODEL__H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <string.h>

#include "util.h"
#include "vertex.h"
#include "edge.h"
#include "face.h"
#include "point.h"
#include "geometry.h"
#include "hashtable.h"
#include "space_partition.h"


/**
 * \struct vf_model model.h vf_model
 * Structure de données pour stocker un maillage triangulaire grâce à une liste de sommets et une liste de faces. Une face étant définie par trois sommets identifiés par leur numéro d'incide dans la liste de sommets.
 */
typedef struct
{
  vf_vertex *ve;
  vf_face *fa;
  int nbvertex;
  int nbface;
  double xmin,
    ymin,
    zmin,
    xmax,
    ymax,
    zmax;
} vf_model;

typedef vf_model * pt_vf_model;


/**
 * \struct vef_model model.h vef_model
 * Structure de données pour stocker un maillage triangulaire grâce à une liste de sommets,une liste d'arêtes et une liste de faces. Une arête étant définie par deux sommets identifiés par leur numéro d'incide dans la liste de sommets. Une face étant définie par trois arêtes identifiées par leur numéro d'indice dans la liste d'arêtes.
 */
typedef struct
{
  vef_vertex *ve;
  vef_edge *ed;
  vef_face *fa;
  int nbvertex;
  int nbedge;
  int nbface;
  double xmin,
    ymin,
    zmin,
    xmax,
    ymax,
    zmax;
} vef_model;

typedef void (
	      *ptf_func_hashtable) (
				    vf_edge *,
				    const vf_model * const,
				    hashtable *);

/**
   Initialisation d'un modèle avec un maillage vide
   @param m pointeur sur le modèle
   @return aucun
*/
void a2ri_vf_init (
		   vf_model * m);

/**
   Libération de l'espace mémoire utilisé
   @param m pointeur sur le modèle
*/
void a2ri_vf_free (
		   vf_model * m);

/**
   Clonage du vf_model
   @param m pointeur sur le modèle
   @return un vf_model identique
*/
vf_model * a2ri_vf_clone (
			 const vf_model * const m);

/**
   Affichage des caractéristiques générales d'un modèle (nombre de sommets et de faces)
   @param m le modèle
   @return aucun
*/
void a2ri_vf_display (
		      const vf_model * const m);

/**
   Affichage des caractéristiques détaillées d'un modèle (nombre de sommets, de faces et leurs adjacences)
   @param m le modèle
   @return aucun
*/
void a2ri_vf_display_detail (
			     const vf_model * const m);

/**
   mis à jour du numéro d'arete en fonction de la taille de la table de hachage
   @param e arete à mettre à jour
   @param m le modele - parametre inutilisé
   @param table la table de hachage
   @return aucun
*/
void a2ri_vf_update_num_edge (
			      vf_edge * e,
			      const vf_model * const m,
			      const hashtable * const table);

/**
   mis à jour de la longueur de l'arete
   @param e arete à mettre à jour
   @param m le modele
   @param table la table de hachage - parametre inutilisé
   @return aucun
*/
void a2ri_vf_update_length_edge (
				 vf_edge * e,
				 const vf_model * const m,
				 hashtable * table);

/**
   Construction de la table de hachage contenant toutes les aretes d'un vf_model
   @param m le modele
   @param func tableau de fonctions devant etre appliqué
   @param nbfunc taille du tableau de fonctions
   @return la table de hachage
*/
hashtable *a2ri_vf_construction_edge_table (
					    const vf_model * const m,
					    ptf_func_hashtable * func,
					    int nbfunc);

/**
   Ajout d'un vertex (x,y,z) au modèle
   @param m pointeur sur le modèle
   @param x coordonnée x du vertex
   @param y coordonnée y
   @param z coordonnée z
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_add_vertex (
			vf_model * m,
			double x,
			double y,
			double z);

/**
   Recherche d'un vertex dans un modèle
   @param m le modèle
   @param x coordonnée x du vertex à rechercher
   @param y coordonnée y
   @param z coordonnée z
   @return numéro du vertex dans la liste de vertex du modèle, -1 si le vertex n'est pas trouvé.
*/
int a2ri_vf_search_vertex (
			   const vf_model * const m,
			   double x,
			   double y,
			   double z);

/**
   Retrait d'un sommet du modèle. L'opération ne peut être effectué que si le sommet n'est référencée dans aucune face.
   @param m le modèle
   @param numvertex numèro du vertex à retirer
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_remove_vertex (
			   vf_model * m,
			   int numvertex);

/**
   Retrait d'une liste de sommets du modèle. L'opération ne peut être effectué que si le sommet n'est référencée dans aucune face.
   @param m le modèle
   @param numvertex numèro du vertex à retirer
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_remove_list_of_vertex (
				   vf_model * m,
				   int *listvertex,
				   int sizelist);

/**
   Ajout d'une face défini par trois points (numéros d'index dans la liste de points) au modèle
   @param m pointeur sur le modèle
   @param ve1 index du premier point
   @param ve2 index du second point
   @param ve3 index du troisième point
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_add_face (
		      vf_model * m,
		      int ve1,
		      int ve2,
		      int ve3);

/**
   Recherche d'une face dans un modèle
   @param m le modèle
   @param ve1 index d'un point
   @param ve2 index d'un point
   @param ve3 index d'un point
   @return numéro de la face dans la liste de face du modèle, -1 si la face n'est pas trouvée.
   @warning Cette fonction est sensible à l'orientation de la face.
   Exemple pour un modèle m:
   @code
   liste de faces (index -> ve1 ve2 ve3):
   0 -> 0 1 2
   1 -> 0 2 3 
   2 -> 0 3 1
   3 -> 1 3 2
   @endcode
   le code
   @code
   a2ri_vf_search_face(m,0,3,1);
   ou
   a2ri_vf_search_face(m,3,1,0);
   ou
   a2ri_vf_search_face(m,1,0,3);
   @endcode
   retournera 2 alors que
   @code
   a2ri_vf_search_face(m,3,0,1);
   @endcode
   retournera -1
*/
int a2ri_vf_search_face (
			 const vf_model * const m,
			 int ve1,
			 int ve2,
			 int ve3);

/**
   Retrait d'une face du modèle. La fonction enlève également les sommets qui ne sont plus utilisés.
   @param m le modèle
   @param numface numèro de la face à retirer
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_remove_face (
			 vf_model * m,
			 int numface);

/**
   Retrait d'une liste de faces du modèle. La fonction enlève également les sommets qui ne sont plus utilisés.
   @param m le modèle
   @param numface numèro de la face à retirer
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_remove_list_of_face (
				 vf_model * m,
				 int *listface,
				 int sizelist);

/**
   Translation du modèle
   @param m pointeur sur le modèle à translater
   @param delta pointeur sur le vecteur de translation
   @return aucun
*/
void a2ri_vf_translate (
			vf_model * m,
			const vector3d * const delta);

/**
   Translation d'un modele afin qu'il se trouve centré sur l'origine (0,0,0)
   @param m le modele a centrer
   @return aucun
**/
void a2ri_vf_center (
		     vf_model * m);

/**
   Rotation d'un modèle en radian autour de l'axe X
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @return aucun
*/
void a2ri_vf_rotateX_radian (
			     vf_model * m,
			     double angle);

/**
   Rotation d'un modèle en degré autour de l'axe X
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degré
   @return aucun
*/
void a2ri_vf_rotateX_degre (
			    vf_model * m,
			    double angle);

/**
   Rotation d'un modèle en radian suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_vf_rotateX_radian_center (
				    vf_model * m,
				    double angle,
				    const point3d * const centre);

/**
   Rotation d'un modèle en degre suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_vf_rotateX_degre_center (
				   vf_model * m,
				   double angle,
				   const point3d * const centre);

/**
   Rotation d'un modèle en radian autour de l'axe Y
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @return aucun
*/
void a2ri_vf_rotateY_radian (
			     vf_model * m,
			     double angle);

/**
   Rotation d'un modèle en degré autour de l'axe Y
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degré
   @return aucun
*/
void a2ri_vf_rotateY_degre (
			    vf_model * m,
			    double angle);

/**
   Rotation d'un modèle en radian suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_vf_rotateY_radian_center (
				    vf_model * m,
				    double angle,
				    const point3d * const centre);

/**
   Rotation d'un modèle en degre suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_vf_rotateY_degre_center (
				   vf_model * m,
				   double angle,
				   const point3d * const centre);

/**
   Rotation d'un modèle en radian autour de l'axe Z
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @return aucun
*/
void a2ri_vf_rotateZ_radian (
			     vf_model * m,
			     double angle);

/**
   Rotation d'un modèle en degré autour de l'axe Z
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degré
   @return aucun
*/
void a2ri_vf_rotateZ_degre (
			    vf_model * m,
			    double angle);

/**
   Rotation d'un modèle en radian suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_vf_rotateZ_radian_center (
				    vf_model * m,
				    double angle,
				    const point3d * const centre);

/**
   Rotation d'un modèle en degre suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_vf_rotateZ_degre_center (
				   vf_model * m,
				   double angle,
				   const point3d * const centre);

/**
   Rotation d'un modèle en degre suivant l'axe donnée
   @param m pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param axe l'axe de rotation
   @return aucun
*/
void a2ri_vf_rotate_axe_radian (
				vf_model * m,
				double angle,
				const vector3d * const axe);

/**
   Calcul de l'aire totale d'un modèle
   @param m le modèle
   @return aire totale du modèle
*/
double a2ri_vf_area (
		     const vf_model * const m);

/**
   Trouve les faces entourant un sommet
   @param m le modèle
   @param numve numéro du sommet
   @param list liste des numéros de faces
   @param size nombre de faces
   @return aucun
**/
void a2ri_vf_faces_next_vertex (
				const vf_model * const m,
				int numve,
				int **list,
				int *size);

/**
   Trouve les faces entourant un sommet avec une hashtable contenant les aretes fournie
   @param m le modèle
   @param numve numéro du sommet
   @param list liste des numéros de faces
   @param size nombre de faces
   @param table la hashtable
   @return aucun
**/
void a2ri_vf_faces_next_vertex_with_hashtable (
					       const vf_model * const m,
					       int numve,
					       int **list,
					       int *size,
					       const hashtable * const table);

/**
   calcul d'une normale à la face
   @param m le modele
   @param numfa numéro de la face
   @return vecteur normal à la face
*/
//TODO retourner un vector3d *
vector3d a2ri_vf_normal_face (
			      const vf_model * const m,
			      int numfa);

/**
   calcul d'une normale au sommet comme étant la moyenne des normales des faces adjacentes au sommet
   @param m le modele
   @param numve numéro du sommet
   @return vecteur normal au sommet
*/
//TODO retourner un vector3d *
vector3d a2ri_vf_normal_vertex (
				const vf_model * const m,
				int numve);

/**
   calcul d'une normale au sommet comme étant la moyenne des normales des faces adjacentes au sommet
   @param m le modele
   @param numve numéro du sommet
   @param table table de hachage contenant les aretes
   @return vecteur normal au sommet
*/
//TODO retourner un vector3d *
vector3d a2ri_vf_normal_vertex_with_hashtable (
					       const vf_model * const m,
					       int numve,
					       const hashtable * const table);

/**
   Concaténation de plusieurs maillages dans un seul modèle
   @param m tableau de modèle
   @param size nombre de modele numérique
   @return un maillage content tous les maillages.
*/
vf_model * a2ri_vf_concat (
			   const pt_vf_model * const m,
			   int size);

/**
   Ajout des points d'un vf_model dans une partition de l'espace
   @param m le modèle
   @param sp la partition de l'espace
   @return aucun
*/
void a2ri_vf_space_partition (
			      const vf_model * const m,
			      space_partition * sp);

/**
   Conversion d'un vf_model en liste de point3d
   @param m le modèle
   @return la liste de point3d
**/
point3d * a2ri_vf_to_list_point3d(
				  const vf_model * const m);

/**
   Conversion d'un vf_model en vef_model
   @param le modèle à convertir
   @return le vef_model
 **/
vef_model * a2ri_vf_to_vef (
			   const vf_model * const m);

/**
   Initialisation d'un modèle avec un maillage vide
   @param m pointeur sur le modèle
   @return aucun
*/
void a2ri_vef_init (
		    vef_model * m);

/**
   Libération de l'espace mémoire utilisé
   @param m pointeur sur le modèle
*/
void a2ri_vef_free (
		    vef_model * m);

/**
   Affichage des caractéristiques générales d'un modèle (nombre de sommets et de faces)
   @param m le modèle
   @return aucun
*/
void a2ri_vef_display (
		       const vef_model * const m);

/**
   Affichage des caractéristiques détaillées d'un modèle (nombre de sommets, de faces et leurs adjacences)
   @param m le modèle
   @return aucun
*/
void a2ri_vef_display_detail (
			      const vef_model * const m);

/**
   Ajout d'un vertex (x,y,z) au modèle
   @param m pointeur sur le modèle
   @param x coordonnée x du vertex
   @param y coordonnée y
   @param z coordonnée z
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_add_vertex (
			 vef_model * m,
			 double x,
			 double y,
			 double z);

/**
   Recherche d'un vertex dans un modèle
   @param m le modèle
   @param x coordonnée x du vertex à rechercher
   @param y coordonnée y
   @param z coordonnée z
   @return numéro du vertex dans la liste de vertex du modèle, -1 si le vertex n'est pas trouvé.
*/
int a2ri_vef_search_vertex (
			    const vef_model * const m,
			    double x,
			    double y,
			    double z);

/**
   Retrait d'un sommet du modèle. L'opération ne peut être effectué que si le sommet n'est référencée dans aucune arete.
   @param m le modèle
   @param numvertex numèro du vertex à retirer
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_remove_vertex (
			    vef_model * m,
			    int numvertex);

/**
   Retrait d'un sommet du modèle. L'opération ne peut être effectué que si le sommet n'est référencée dans aucune arete.
   @param m le modèle
   @param numvertex numèro du vertex à retirer
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_remove_list_of_vertex (
				    vef_model * m,
				    int *listvertex,
				    int size);

/**
   Ajout d'une arete (ve1,ve2) au modèle
   @param m pointeur sur le modèle
   @param ve1 index du premier point
   @param ve2 index du second point
   @param verif 1 s'il faut vérifier si l'arete existe deja, 0 sinon
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_add_edge (
		       vef_model * m,
		       int ve1,
		       int ve2,
		       int verif);

/**
   Recherche d'une arete dans un modèle
   @param m le modèle
   @param ve1 indice d'un point
   @param ve2 indice d'un point
   @return index de l'arete, -1 si l'arete n'est pas trouvée.
   @warning a2ri_vef_search_edge(m,0,1) retournera le meme resultat que a2ri_vef_search_edge(m,1,0).
*/
int a2ri_vef_search_edge (
			  const vef_model * const m,
			  int ve1,
			  int ve2);

/**
   Retrait d'une arete du modèle. L'opération ne peut être effectué que si l'arete n'est référencée dans aucune face.
   @param m le modèle
   @param numvertex numèro du vertex à retirer
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_remove_edge (
			  vef_model * m,
			  int numedge);

/**
   Retrait d'une liste d'arete du modèle. L'opération ne peut être effectué que si l'arete n'est référencée dans aucune face.
   @param m le modèle
   @param numvertex numèro du vertex à retirer
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_remove_list_of_edge (
				  vef_model * m,
				  int *listedge,
				  int size);

/**
   Ajout d'une face défini par trois points (numéros d'index dans la liste de points) au modèle
   @param m pointeur sur le modèle
   @param ed1 index de la premiere arete
   @param ed2 index de la seconde arete
   @param ed3 index de la troiseieme arete
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_add_face (
		       vef_model * m,
		       int ed1,
		       int ed2,
		       int ed3);

/**
   Recherche d'une face dans un modèle
   @param m le modèle
   @param ed1 index de la premiere arete
   @param ed2 index de la seconde arete
   @param ed3 index de la troiseieme arete
   @return numéro de la face dans la liste de face du modèle, -1 si la face n'est pas trouvée.
   @warning Cette fonction est sensible à l'orientation de la face.
   Exemple pour un modèle m:
   @code
   liste de faces (index -> ve1 ve2 ve3):
   0 -> 0 1 2
   1 -> 0 2 3 
   2 -> 0 3 1
   3 -> 1 3 2
   @endcode
   le code
   @code
   a2ri_vef_search_face(m,0,3,1);
   ou
   a2ri_vef_search_face(m,3,1,0);
   ou
   a2ri_vef_search_face(m,1,0,3);
   @endcode
   retournera 2 alors que
   @code
   a2ri_vef_search_face(m,3,0,1);
   @endcode
   retournera -1
*/
int a2ri_vef_search_face (
			  const vef_model * const m,
			  int ed1,
			  int ed2,
			  int ed3);

/**
   Retrait d'une face du modèle. La fonction enlève également les sommets qui ne sont plus utilisés.
   @param m le modèle
   @param numface numèro de la face à retirer
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_remove_face (
			  vef_model * m,
			  int numface);

/**
   Retrait d'une liste de face du modèle. La fonction enlève également les sommets qui ne sont plus utilisés.
   @param m le modèle
   @param numface numèro de la face à retirer
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_remove_list_of_face (
				  vef_model * m,
				  int *listface,
				  int size);

/**
   Translation du modèle
   @param m pointeur sur le modèle à translater
   @param delta pointeur sur le vecteur de translation
   @return aucun
*/
void a2ri_vef_translate (
			 vef_model * m,
			 const vector3d * const delta);

/**
   Rotation d'un modèle en radian autour de l'axe X
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @return aucun
*/
void a2ri_vef_rotateX_radian (
			      vef_model * m,
			      double angle);

/**
   Rotation d'un modèle en degré autour de l'axe X
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degré
   @return aucun
*/
void a2ri_vef_rotateX_degre (
			     vef_model * m,
			     double angle);

/**
   Rotation d'un modèle en radian suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_vef_rotateX_radian_center (
				     vef_model * m,
				     double angle,
				     const point3d * const centre);

/**
   Rotation d'un modèle en degre suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_vef_rotateX_degre_center (
				    vef_model * m,
				    double angle,
				    const point3d * const centre);

/**
   Rotation d'un modèle en radian autour de l'axe Y
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @return aucun
*/
void a2ri_vef_rotateY_radian (
			      vef_model * m,
			      double angle);

/**
   Rotation d'un modèle en degré autour de l'axe Y
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degré
   @return aucun
*/
void a2ri_vef_rotateY_degre (
			     vef_model * m,
			     double angle);

/**
   Rotation d'un modèle en radian suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_vef_rotateY_radian_center (
				     vef_model * m,
				     double angle,
				     const point3d * const centre);

/**
   Rotation d'un modèle en degre suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_vef_rotateY_degre_center (
				    vef_model * m,
				    double angle,
				    const point3d * const centre);

/**
   Rotation d'un modèle en radian autour de l'axe Z
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @return aucun
*/
void a2ri_vef_rotateZ_radian (
			      vef_model * m,
			      double angle);

/**
   Rotation d'un modèle en degré autour de l'axe Z
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degré
   @return aucun
*/
void a2ri_vef_rotateZ_degre (
			     vef_model * m,
			     double angle);

/**
   Rotation d'un modèle en radian suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_vef_rotateZ_radian_center (
				     vef_model * m,
				     double angle,
				     const point3d * const centre);

/**
   Rotation d'un modèle en degre suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_vef_rotateZ_degre_center (
				    vef_model * m,
				    double angle,
				    const point3d * const centre);

/**
   Rotation d'un modèle en degre suivant l'axe donnée
   @param m pointeur sur le modèle
   @param angle angle de rotation du modèle en degre
   @param axe l'axe de rotation
   @return aucun
*/
void a2ri_vef_rotate_axe_radian (
				 vef_model * m,
				 double angle,
				 const vector3d * const axe);

/**
   Calcul de l'aire totale d'un modèle
   @param m le modèle
   @return aire totale du modèle
*/
double a2ri_vef_area_model (
			    const vef_model * const m);

/**
   Ajout des points d'un vef_model dans une partition de l'espace
   @param m le modèle
   @param sp la partition de l'espace
   @return aucun
*/
void a2ri_vef_space_partition (
			       const vef_model * const m,
			       space_partition * sp);

/**
   Conversion d'un vef_model en liste de point3d
   @param m le modèle
   @return la liste de point3d
**/
point3d * a2ri_vef_to_list_point3d(
				   const vef_model * const m);

/**
   Conversion d'un vef_model en vf_model
   @param m le modèle à convertir
   @return un pointeur sur le vf_model
 **/
vf_model * a2ri_vef_to_vf(
			 const vef_model * const m);

#endif
