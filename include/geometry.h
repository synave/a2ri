/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 01/03/07       */
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



#ifndef GEOMETRY__H
#define GEOMETRY__H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "util.h"
#include "point.h"
#include "vector.h"
#include "matrix.h"

/**
   Calcul de la longueur entre deux point3d
   @param p1 premier point3d
   @param p2 second point3d
   @return longueur \f$p_1p_2\f$
*/
double point3d_length (
		       const point3d * const p1,
		       const point3d * const p2);

/**
   Calcul de la longueur au carré entre deux point3d
   @param p1 premier point3d
   @param p2 second point3d
   @return longueur \f$p_1p_2\f$
*/
double point3d_square_length (
			      const point3d * const p1,
			      const point3d * const p2);

/**
   Calcul de la longueur entre deux point2d
   @param p1 premier point2d
   @param p2 second point2d
   @return longueur \f$p_1p_2\f$
*/
double point2d_length (
		       const point2d * const p1,
		       const point2d * const p2);

/**
   Calcule l'équation d'un plan à partir d'une liste de point sous la forme ax+by+cz+d=0
   @param p liste de points
   @param size nombre de points
   @param aeq valeur de "a"
   @param beq valeur de "b"
   @param ceq valeur de "c"
   @param deq valeur de "d"
   @return aucun
   Exemple de code
   @code
   double a,b,c,d;
   point3d p[3];
   point3d_init(&(p[0]),0.0,0.0,0.0);
   point3d_init(&(p[1]),1.0,1.0,-2.0);
   point3d_init(&(p[2]),-1.0,1.0,0.0);
   equation_plan(p,3,&a,&b,&c,&d);
   printf("%dx+%dy+%dz+%d=0\n",a,b,c,d);
   @endcode
   donne le résultat
   @code
   1.0x+1.0y+1.0z+0.0=0
   @endcode
*/
void equation_plan (
		    const point3d * const p,
		    int size,
		    double *aeq,
		    double *beq,
		    double *ceq,
		    double *deq);


/**
   Calcul de l'aire du triangle défini par les trois points
   @param p1 premier point du triangle
   @param p2 second point
   @param p3 troisième point
   @return aire du triangle
*/
double point3d_area (
		     const point3d * const p1,
		     const point3d * const p2,
		     const point3d * const p3);

/**
   Transformation des coordonnées d'un point dans un espace à trois dimension en coordonnées dans un espace à deux dimensions dont la base est le couple (base1,base2) et d'origine le point3d "origin"
   @param p point dont les coordonnées vont etre recalculées
   @param origin point origine du nouveau repére
   @param base1 premier vecteur formant la nouvelle base
   @param base2 second vecteur formant la nouvelle base
   @param newp point contenant les coordonnées dans la nouvelle base
   @return aucun
*/
void base_modification_3d_to_2d (
				 const point3d * const p,
				 const point3d * const origin,
				 const vector3d * const base1,
				 const vector3d * const base2,
				 point2d * newp);

/**
   Calcul du vecteur normal à AB et contenu dans le meme plan que celui formé par les vecteurs (AB,AC). Le résultat final est une base orthogonal du repére.
   @param AB premier vecteur de la base
   @param AC second vecteur du repére
   @param U pointeur sur le nouveau vecteur formant la base orthogonal
*/
void find_second_base_vector (
			      const vector3d * const AB,
			      const vector3d * const AC,
			      vector3d * U);

/**
   Vérification qu'un point M se trouve bien dans le triangle ABC
   @param M le point à tester
   @param A premier sommet du triangle
   @param B second sommet du triangle
   @param C troisième sommet du triangle
   @return 1 si le point M appartient au triangle ABC, 0 sinon
*/
int point_in_triangle (
		       const point3d * const M,
		       const point3d * const A,
		       const point3d * const B,
		       const point3d * const C);

/**
   Calcule l'angle en radian formé par les deux vector3d
   @param v1 le premier vector3d
   @param v2 le second vector3d
   @return angle en radian
*/
double vector3d_angle_radian (
			      const vector3d * const v1,
			      const vector3d * const v2);

/**
   Calcule l'angle en degré formé par les deux vector3d
   @param v1 le premier vector3d
   @param v2 le second vector3d
   @return angle en degré
*/
double vector3d_angle_degre (
			     const vector3d * const v1,
			     const vector3d * const v2);

/**
   Calcule le centre du cercle inscrit à un triangle
   @param A premier point du triangle
   @param B second point
   @param C troisème point
   @return le point centre du cercle inscrit
*/
point3d incircle_center (
			 const point3d * const A,
			 const point3d * const B,
			 const point3d * const C);

/**
   Calcule le centre du cercle circonscrit à un triangle
   @param A premier point du triangle
   @param B second point
   @param C troisème point
   @return le point centre du cercle circonscrit
*/
point3d circumcircle_center (
			     point3d * A,
			     point3d * B,
			     point3d * C);

/**
   Calcule le centre de sphère circonscrite au tétraédre
   @param A premier point du tétraédre
   @param B second point
   @param C troisème point
   @param D quatrième point
   @return le point centre de la sphère circonscrite
*/
point3d circumsphere_center (
			     point3d * A,
			     point3d * B,
			     point3d * C,
			     point3d * D);

/**
   Calcule la distance entre un point M et une droite définie par deux points A et B
   @param M point
   @param A premier point sur la droite
   @param B second point sur la droite
   @return distance entre le point M et la droite AB
*/
double distance_point_straight_line (
				     point3d * M,
				     point3d * A,
				     point3d * B);

/**
   Calcule la distance entre un point M et un plan définie par trois points A,b et C
   @param M point
   @param A premier point du plan
   @param B second point du plan
   @param C troisieme point du plan
   @return distance entre le point M et le plan ABC
*/
double distance_point_plane (
			     point3d * M,
			     point3d * A,
			     point3d * B,
			     point3d * C);

/**
   Calcule la distance entre un point M et un triangle défini par trois points A,b et C
   @param M point
   @param A premier point du triangle
   @param B second point du triangle
   @param C troisieme point du triangle
   @return distance entre le point M et le triangle ABC
*/
double distance_point_triangle (
				point3d * M,
				point3d * A,
				point3d * B,
				point3d * C);

/**
   Echantillonne le triangle ABC
   @param A point A du triangle
   @param B point B
   @param C point C
   @param nb_sample nombre d'échantillon à extraire
   @param list tableau de réel représentant les coordonées des échantillons
   @param size taille du tableau
   @return aucun
*/
void sample_triangle (
		      const point3d * const A,
		      const point3d * const B,
		      const point3d * const C,
		      int nb_sample,
		      point3d ** list);

/**
   Test de l'instrsection entre une droite et un plan
   @param d1 premier point définissant la droite
   @param d2 second point définissant la droite
   @param p1 premier point du plan
   @param p2 second point du plan
   @param p3 troisieme point du plan
   @param t paramètre d'intersection
   @return 1 si intersection, 0 sinon
*/
int intersection_droite_plan (
			      point3d * d1,
			      point3d * d2,
			      point3d * p1,
			      point3d * p2,
			      point3d * p3,
			      double *t);

/**
   Test de l'intersection entre une droite et un triangle
   @param d1 premier point définissant la droite
   @param d2 second point définissant la droite
   @param p1 premier point du triangle
   @param p2 second point du triangle
   @param p3 troisieme point du triangle
   @return 1 si intersection, 0 sinon
*/
int intersection_droite_triangle (
				  point3d * d1,
				  point3d * d2,
				  point3d * p1,
				  point3d * p2,
				  point3d * p3,
				  double *t);

/**
   Test de l'intersection entre un segment et un triangle
   @param s1 premier point définissant le segment
   @param s2 second point définissant le segment
   @param p1 premier point du triangle
   @param p2 second point du triangle
   @param p3 troisieme point du triangle
   @return 1 si intersection, 0 sinon
*/
int intersection_segment_triangle (
				  point3d * s1,
				  point3d * s2,
				  point3d * p1,
				  point3d * p2,
				  point3d * p3,
				  double *t);

/**
   Test de l'intersection entre deux triangles
   @param t1 premier point définissant le premier triangle
   @param t2 second point définissant le premier triangle
   @param t3 troisieme point définissant le premier triangle
   @param p1 premier point du second triangle
   @param p2 second point du second triangle
   @param p3 troisieme point du second triangle
   @return 1 si intersection, 0 sinon
*/
int intersection_triangle_triangle (
				    point3d * t1,
				    point3d * t2,
				    point3d * t3,
				    point3d * p1,
				    point3d * p2,
				    point3d * p3);

/**
   Teste si les trois points A, B et C sont alignés
   @param A premier point
   @param B second point
   @param C troisieme point
   @return 1 si les trois points sont alignés, 0 sinon
*/
int trois_points_alignes (
			  point3d * A,
			  point3d * B,
			  point3d * C);

#endif
