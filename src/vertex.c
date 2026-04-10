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

#include "vertex.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

/********** MAIN FUNCTIONS **********/

/**
   Affichage du vertex
   @param v le vertex à afficher
   @return aucun
*/
void
vf_vertex_display (
		   const vf_vertex * const v)
{
  printf ("Vertex --> [%f,%f,%f] , valence : %d\n", 
	  v->x, v->y, v->z, v->nbincidentvertices);
}

/**
   Affichage detaille du vertex
   @param v le vertex à afficher
   @return aucun
*/
void
vf_vertex_display_detail (
			  const vf_vertex * const v)
{
  printf ("[%5.2f , %5.2f , %5.2f]\n\t\t\tvalency : %d - list : ", 
	  v->x, v->y, v->z, v->nbincidentvertices);
  if (v->nbincidentvertices > 0)
    printf ("%d", v->incidentvertices[0]);
  for (int j = 1; j < v->nbincidentvertices; j++)
    printf (" , %d", v->incidentvertices[j]);
  printf ("\n");
}

/**
   Translation d'un vertex
   @param v pointeur sur le vertex à translater
   @param dx décalage suivant l'axe X
   @param dy décalage suivant l'axe Y
   @param dz décalage suivant l'axe Z
*/
void
vf_vertex_translate (
		     vf_vertex * v,
		     const vector3d * const delta)
{
  v->x += delta->dx;
  v->y += delta->dy;
  v->z += delta->dz;
}

/**
   Rotation d'un vertex en radian autour de l'axe X
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @return aucun
*/
void
vf_vertex_rotateX_radian (
			  vf_vertex * v,
			  double angle)
{
  double tempvy = v->y * cos (angle) + v->z * sin (angle);
  double tempvz = -v->y * sin (angle) + v->z * cos (angle);
  v->y = tempvy;
  v->z = tempvz;
}

/**
   Rotation d'un vertex en degre autour de l'axe X
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @return aucun
*/
void
vf_vertex_rotateX_degre (
			 vf_vertex * v,
			 double angle)
{
  double angleradian = angle * 2.0 * PI / 360.0;
  double tempvy = v->y * cos (angleradian) + v->z * sin (angleradian);
  double tempvz = -v->y * sin (angleradian) + v->z * cos (angleradian);
  v->y = tempvy;
  v->z = tempvz;
}

/**
   Rotation d'un vertex en radian suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void
vf_vertex_rotateX_center_radian (
				 vf_vertex * v,
				 double angle,
				 const point3d * const centre)
{
  vector3d delta;
  vector3d_init (&delta, -centre->x, -centre->y, -centre->z);

  vf_vertex_translate (v, &delta);
  vector3d_reverse (&delta);
  vf_vertex_rotateX_radian (v, angle);
  vf_vertex_translate (v, &delta);
}

/**
   Rotation d'un vertex en degre suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void
vf_vertex_rotateX_center_degre (
				vf_vertex * v,
				double angle,
				const point3d * const centre)
{
  vector3d delta;
  vector3d_init (&delta, -centre->x, -centre->y, -centre->z);

  vf_vertex_translate (v, &delta);
  vector3d_reverse (&delta);
  vf_vertex_rotateX_degre (v, angle);
  vf_vertex_translate (v, &delta);
}

/**
   Rotation d'un vertex en radian autour de l'axe Y
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @return aucun
*/
void
vf_vertex_rotateY_radian (
			  vf_vertex * v,
			  double angle)
{
  double tempvx = v->x * cos (angle) + v->z * sin (angle);
  double tempvz = -v->x * sin (angle) + v->z * cos (angle);
  v->x = tempvx;
  v->z = tempvz;
}

/**
   Rotation d'un vertex en degre autour de l'axe Y
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @return aucun
*/
void
vf_vertex_rotateY_degre (
			 vf_vertex * v,
			 double angle)
{
  double angleradian = angle * 2.0 * PI / 360.0;
  double tempvx = v->x * cos (angleradian) + v->z * sin (angleradian);
  double tempvz = -v->x * sin (angleradian) + v->z * cos (angleradian);
  v->x = tempvx;
  v->z = tempvz;
}

/**
   Rotation d'un vertex en radian suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void
vf_vertex_rotateY_center_radian (
				 vf_vertex * v,
				 double angle,
				 const point3d * const centre)
{
  vector3d delta;
  vector3d_init (&delta, -centre->x, -centre->y, -centre->z);

  vf_vertex_translate (v, &delta);
  vector3d_reverse (&delta);
  vf_vertex_rotateY_radian (v, angle);
  vf_vertex_translate (v, &delta);
}

/**
   Rotation d'un vertex en degre suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void
vf_vertex_rotateY_center_degre (
				vf_vertex * v,
				double angle,
				const point3d * const centre)
{
  vector3d delta;
  vector3d_init (&delta, -centre->x, -centre->y, -centre->z);

  vf_vertex_translate (v, &delta);
  vector3d_reverse (&delta);
  vf_vertex_rotateY_degre (v, angle);
  vf_vertex_translate (v, &delta);
}

/**
   Rotation d'un vertex en radian autour de l'axe Z
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @return aucun
*/
void
vf_vertex_rotateZ_radian (
			  vf_vertex * v,
			  double angle)
{
  double tempvx = v->x * cos (angle) + v->y * sin (angle);
  double tempvy = -v->x * sin (angle) + v->y * cos (angle);
  v->x = tempvx;
  v->y = tempvy;
}

/**
   Rotation d'un vertex en degre autour de l'axe Z
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @return aucun
*/
void
vf_vertex_rotateZ_degre (
			 vf_vertex * v,
			 double angle)
{
  double angleradian = angle * 2.0 * PI / 360.0;
  double tempvx = v->x * cos (angleradian) + v->y * sin (angleradian);
  double tempvy = -v->x * sin (angleradian) + v->y * cos (angleradian);
  v->x = tempvx;
  v->y = tempvy;
}

/**
   Rotation d'un vertex en radian suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void
vf_vertex_rotateZ_center_radian (
				 vf_vertex * v,
				 double angle,
				 const point3d * const centre)
{
  vector3d delta;
  vector3d_init (&delta, -centre->x, -centre->y, -centre->z);

  vf_vertex_translate (v, &delta);
  vector3d_reverse (&delta);
  vf_vertex_rotateZ_radian (v, angle);
  vf_vertex_translate (v, &delta);
}

/**
   Rotation d'un vertex en degre suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void
vf_vertex_rotateZ_center_degre (
				vf_vertex * v,
				double angle,
				const point3d * const centre)
{
  vector3d delta;
  vector3d_init (&delta, -centre->x, -centre->y, -centre->z);

  vf_vertex_translate (v, &delta);
  vector3d_reverse (&delta);
  vf_vertex_rotateZ_degre (v, angle);
  vf_vertex_translate (v, &delta);
}

/**
   Rotation d'un vertex en degre suivant l'axe donnée
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param axe l'axe de rotation avec (nx2 + ny2 + nz2 = 1)
   @return aucun
**/
void
vf_vertex_rotate_axe_radian (
			     vf_vertex * v,
			     gsl_matrix * M)
{

  double tempvx =
    gsl_matrix_get (M, 0, 0) * v->x + 
    gsl_matrix_get (M, 0, 1) * v->y +
    gsl_matrix_get (M, 0, 2) * v->z;
  double tempvy =
    gsl_matrix_get (M, 1, 0) * v->x +
    gsl_matrix_get (M, 1, 1) * v->y +
    gsl_matrix_get (M, 1, 2) * v->z;
  double tempvz =
    gsl_matrix_get (M, 2, 0) * v->x + 
    gsl_matrix_get (M, 2, 1) * v->y +
    gsl_matrix_get (M, 2, 2) * v->z;;

  v->x = tempvx;
  v->y = tempvy;
  v->z = tempvz;

}

/**
   Libération de la mémoire
   @param v le vf_vertex
   @return aucun
**/
void
vf_vertex_free (
		vf_vertex * v)
{
  free (v->incidentvertices);
}










/**
   Affichage du vertex
   @param v le vertex à afficher
   @return aucun
*/
void
vef_vertex_display (
		    vef_vertex * v)
{
  printf ("Vertex --> [%f,%f,%f] , valence : %d\n", 
	  v->x, v->y, v->z, v->nbsharededges);
}

/**
   Affichage detaille du vertex
   @param v le vertex à afficher
   @return aucun
*/
void vef_vertex_display_detail (
			       const vef_vertex * const v)
{
  printf ("[%5.2f , %5.2f , %5.2f]\n\t\t\tvalency : %d - list : ", 
	  v->x, v->y, v->z, v->nbsharededges);
  if (v->nbsharededges > 0)
    printf ("%d", v->sharededges[0]);
  for (int j = 1; j < v->nbsharededges; j++)
    printf (" , %d", v->sharededges[j]);
  printf ("\n");
}

/**
   Translation d'un vertex
   @param v pointeur sur le vertex à translater
   @param dx décalage suivant l'axe X
   @param dy décalage suivant l'axe Y
   @param dz décalage suivant l'axe Z
*/
void
vef_vertex_translate (
		      vef_vertex * v,
		      const vector3d * const delta)
{
  v->x += delta->dx;
  v->y += delta->dy;
  v->z += delta->dz;
}

/**
   Rotation d'un vertex en radian autour de l'axe X
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @return aucun
*/
void
vef_vertex_rotateX_radian (
			   vef_vertex * v,
			   double angle)
{
  double tempvy = v->y * cos (angle) + v->z * sin (angle);
  double tempvz = -v->y * sin (angle) + v->z * cos (angle);
  v->y = tempvy;
  v->z = tempvz;
}

/**
   Rotation d'un vertex en degre autour de l'axe X
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @return aucun
*/
void
vef_vertex_rotateX_degre (
			  vef_vertex * v,
			  double angle)
{
  double angleradian = angle * 2.0 * PI / 360.0;
  double tempvy = v->y * cos (angleradian) + v->z * sin (angleradian);
  double tempvz = -v->y * sin (angleradian) + v->z * cos (angleradian);
  v->y = tempvy;
  v->z = tempvz;
}

/**
   Rotation d'un vertex en radian suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void
vef_vertex_rotateX_center_radian (
				  vef_vertex * v,
				  double angle,
				  const point3d * const centre)
{
  vector3d delta;
  vector3d_init (&delta, -centre->x, -centre->y, -centre->z);

  vef_vertex_translate (v, &delta);
  vector3d_reverse (&delta);
  vef_vertex_rotateX_radian (v, angle);
  vef_vertex_translate (v, &delta);
}

/**
   Rotation d'un vertex en degre suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void
vef_vertex_rotateX_center_degre (
				 vef_vertex * v,
				 double angle,
				 const point3d * const centre)
{
  vector3d delta;
  vector3d_init (&delta, -centre->x, -centre->y, -centre->z);

  vef_vertex_translate (v, &delta);
  vector3d_reverse (&delta);
  vef_vertex_rotateX_degre (v, angle);
  vef_vertex_translate (v, &delta);
}

/**
   Rotation d'un vertex en radian autour de l'axe Y
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @return aucun
*/
void
vef_vertex_rotateY_radian (
			   vef_vertex * v,
			   double angle)
{
  double tempvx = v->x * cos (angle) + v->z * sin (angle);
  double tempvz = -v->x * sin (angle) + v->z * cos (angle);
  v->x = tempvx;
  v->z = tempvz;
}

/**
   Rotation d'un vertex en degre autour de l'axe Y
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @return aucun
*/
void
vef_vertex_rotateY_degre (
			  vef_vertex * v,
			  double angle)
{
  double angleradian = angle * 2.0 * PI / 360.0;
  double tempvx = v->x * cos (angleradian) + v->z * sin (angleradian);
  double tempvz = -v->x * sin (angleradian) + v->z * cos (angleradian);
  v->x = tempvx;
  v->z = tempvz;
}

/**
   Rotation d'un vertex en radian suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void
vef_vertex_rotateY_center_radian (
				  vef_vertex * v,
				  double angle,
				  const point3d * const centre)
{
  vector3d delta;
  vector3d_init (&delta, -centre->x, -centre->y, -centre->z);

  vef_vertex_translate (v, &delta);
  vector3d_reverse (&delta);
  vef_vertex_rotateY_radian (v, angle);
  vef_vertex_translate (v, &delta);
}

/**
   Rotation d'un vertex en degre suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void
vef_vertex_rotateY_center_degre (
				 vef_vertex * v,
				 double angle,
				 const point3d * const centre)
{
  vector3d delta;
  vector3d_init (&delta, -centre->x, -centre->y, -centre->z);

  vef_vertex_translate (v, &delta);
  vector3d_reverse (&delta);
  vef_vertex_rotateY_degre (v, angle);
  vef_vertex_translate (v, &delta);
}

/**
   Rotation d'un vertex en radian autour de l'axe Z
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @return aucun
*/
void
vef_vertex_rotateZ_radian (
			   vef_vertex * v,
			   double angle)
{
  double tempvx = v->x * cos (angle) + v->y * sin (angle);
  double tempvy = -v->x * sin (angle) + v->y * cos (angle);
  v->x = tempvx;
  v->y = tempvy;
}

/**
   Rotation d'un vertex en degre autour de l'axe Z
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @return aucun
*/
void
vef_vertex_rotateZ_degre (
			  vef_vertex * v,
			  double angle)
{
  double angleradian = angle * 2.0 * PI / 360.0;
  double tempvx = v->x * cos (angleradian) + v->y * sin (angleradian);
  double tempvy = -v->x * sin (angleradian) + v->y * cos (angleradian);
  v->x = tempvx;
  v->y = tempvy;
}

/**
   Rotation d'un vertex en radian suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en radian
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void
vef_vertex_rotateZ_center_radian (
				  vef_vertex * v,
				  double angle,
				  const point3d * const centre)
{
  vector3d delta;
  vector3d_init (&delta, -centre->x, -centre->y, -centre->z);

  vef_vertex_translate (v, &delta);
  vector3d_reverse (&delta);
  vef_vertex_rotateZ_radian (v, angle);
  vef_vertex_translate (v, &delta);
}

/**
   Rotation d'un vertex en degre suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param cx coordonnée x du centre de rotation
   @param cy coordonnée y du centre de rotation
   @param cz coordonnée z du centre de rotation
   @return aucun
*/
void
vef_vertex_rotateZ_center_degre (
				 vef_vertex * v,
				 double angle,
				 const point3d * const centre)
{
  vector3d delta;
  vector3d_init (&delta, -centre->x, -centre->y, -centre->z);

  vef_vertex_translate (v, &delta);
  vector3d_reverse (&delta);
  vef_vertex_rotateZ_degre (v, angle);
  vef_vertex_translate (v, &delta);
}

/**
   Rotation d'un vertex en degre suivant l'axe donnée
   @param v pointeur sur le vertex
   @param angle angle de rotation du vertex en degre
   @param axe l'axe de rotation avec (nx2 + ny2 + nz2 = 1)
   @return aucun
**/
void
vef_vertex_rotate_axe_radian (
			      vef_vertex * v,
			      gsl_matrix * M)
{

  double tempvx =
    gsl_matrix_get (M, 0, 0) * v->x + 
    gsl_matrix_get (M, 0, 1) * v->y +
    gsl_matrix_get (M, 0, 2) * v->z;
  double tempvy =
    gsl_matrix_get (M, 1, 0) * v->x +
    gsl_matrix_get (M, 1, 1) * v->y +
    gsl_matrix_get (M, 1, 2) * v->z;
  double tempvz =
    gsl_matrix_get (M, 2, 0) * v->x + 
    gsl_matrix_get (M, 2, 1) * v->y +
    gsl_matrix_get (M, 2, 2) * v->z;;

  v->x = tempvx;
  v->y = tempvy;
  v->z = tempvz;

}


/**
   Libération de la mémoire
   @param v le vef_vertex
   @return aucun
**/
void
vef_vertex_free (
		 vef_vertex * v)
{
  free (v->sharededges);
}
