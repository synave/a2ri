/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 17/03/15       */
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

#include "skeleton.h"


/**
   Initialisation d'un squelette avec un maillage vide
   @param m pointeur sur le squelette
   @return aucun
*/
void a2ri_skeleton_init (
		   skeleton * s)
{
  s->ve=NULL;
  s->ed=NULL;
  s->nbvertex=0;
  s->nbedge=0;
}

/**
   Libération de l'espace mémoire utilisé
   @param m pointeur sur le squelette
*/
void a2ri_skeleton_free (
		   skeleton * s)
{
  for (int i = 0; i < s->nbvertex; i++)
    {
      free (s->ve[i].sharededges);
    }
  free (s->ve);
  free (s->ed);
}

/**
   Affichage des caractéristiques générales d'un squelette (sommets et arêtes)
   @param m le squelette
   @return aucun
*/

void a2ri_skeleton_display (
		      const skeleton * const s)
{
  printf ("********************\nSkeleton :");
  printf ("\n\t%d vertices\n", s->nbvertex);
  printf ("\t%d faces\n", s->nbedge);
  printf ("********************\n");
}

/**
   Affichage des caractéristiques détaillées d'un squelette (sommets et arêtes et leurs adjacences)
   @param m le squelette
   @return aucun
*/
void a2ri_skeleton_display_detail (
			     const skeleton * const s)
{
  printf ("********************\nSkeleton :");
  
  if (s->nbvertex)
    {
      printf ("\n\t%d vertices\n", s->nbvertex);
      for (int i = 0; i < s->nbvertex; i++)
	{
	  printf ("\t\t%5d - ", i);
	  vef_vertex_display_detail (&(s->ve[i]));
	  printf ("\n");
	}
    }

  if (s->nbedge)
    {
      printf ("\n\t%d edges\n", s->nbedge);
      for (int i = 0; i < s->nbedge; i++)
	{
	  printf ("\t\t%5d - (%d , %d) ",
		  i, s->ed[i].ve1, s->ed[i].ve2);
	  printf ("\n\n");
	}
    }
}

/**
   Ajout d'un vertex (x,y,z) au squelette
   @param m pointeur sur le squelette
   @param x coordonnée x du vertex
   @param y coordonnée y
   @param z coordonnée z
   @return 1 si succès, 0 sinon
*/
int a2ri_skeleton_add_vertex (
			skeleton * s,
			double x,
			double y,
			double z)
{
  if (s->ve == NULL)
    {
      s->ve = (skeleton_vertex *) malloc (sizeof (skeleton_vertex));
      a2ri_erreur_critique_si (s->ve == NULL,
			    "erreur allocation memoire pour s->ve\na2ri_skeleton_add_vertex");
    }
  else
    {
      skeleton_vertex *tempve;
      tempve =
	(skeleton_vertex *) realloc (s->ve, (s->nbvertex + 1) * sizeof (skeleton_vertex));
      a2ri_erreur_critique_si (tempve == NULL,
			    "erreur allocation memoire pour s->ve\na2ri_skeleton_add_vertex");
      s->ve=tempve;
    }

  s->ve[s->nbvertex].x = x;
  s->ve[s->nbvertex].y = y;
  s->ve[s->nbvertex].z = z;
  s->ve[s->nbvertex].sharededges = NULL;
  s->ve[s->nbvertex].nbsharededges = 0;
  (s->nbvertex)++;

  return 1;
}

/**
   Ajout d'une arête défini par trois points (numéros d'index dans la liste de points) au squelette
   @param m pointeur sur le squelette
   @param ve1 index du premier point
   @param ve2 index du second point
   @return 1 si succès, 0 sinon
*/
int a2ri_skeleton_add_edge (
		      skeleton * s,
		      int ve1,
		      int ve2)
{
  //allocation ou reallocatin de l'espace memoire
  if (s->ed == NULL)
    {
      s->ed = (skeleton_edge *) malloc (sizeof (skeleton_edge));
      a2ri_erreur_critique_si (s->ed == NULL,
			       "erreur allocation memoire pour m->ed\na2ri_skeleton_add_edge");
    }
  else
    {
      skeleton_edge *temped;
      temped =
	(skeleton_edge *) realloc (s->ed, (s->nbedge + 1) * sizeof (skeleton_edge));
      a2ri_erreur_critique_si (temped == NULL,
			       "erreur allocation memoire pour m->ed\na2ri_skeleton_add_vertex");
      s->ed=temped;
    }
  
  s->ed[s->nbedge].ve1 = ve1;
  s->ed[s->nbedge].ve2 = ve2;
  (s->nbedge)++;
  
  list_int_add (&(s->ve[ve1].sharededges),
		&(s->ve[ve1].nbsharededges),
		(s->nbedge) - 1, WITH_REDUNDANCE);
  list_int_add (&(s->ve[ve2].sharededges),
		&(s->ve[ve2].nbsharededges),
		(s->nbedge) - 1, WITH_REDUNDANCE);
  
  return 1;
}

/**
   Translation du squelette
   @param m pointeur sur le squelette à translater
   @param delta pointeur sur le vecteur de translation
   @return aucun
*/
void a2ri_skeleton_translate (
			skeleton * s,
			const vector3d * const delta)
{
  //translation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_translate (&(s->ve[i]), delta);
}

/**
   Rotation d'un squelette en radian autour de l'axe X
   @param v pointeur sur le squelette
   @param angle angle de rotation du squelette en radian
   @return aucun
*/
void a2ri_skeleton_rotateX_radian (
			     skeleton * s,
			     double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotateX_radian (&(s->ve[i]), angle);
}

/**
   Rotation d'un squelette en degré autour de l'axe X
   @param v pointeur sur le squelette
   @param angle angle de rotation du squelette en degré
   @return aucun
*/
void a2ri_skeleton_rotateX_degre (
			    skeleton * s,
			    double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotateX_degre (&(s->ve[i]), angle);
}

/**
   Rotation d'un squelette en radian suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le squelette
   @param angle angle de rotation du squelette en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_skeleton_rotateX_center_radian (
				    skeleton * s,
				    double angle,
				    const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotateX_center_radian (&(s->ve[i]), angle, centre);
}

/**
   Rotation d'un squelette en degre suivant l'axe X de centre (cx,cy,cz)
   @param v pointeur sur le squelette
   @param angle angle de rotation du squelette en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_skeleton_rotateX_center_degre (
				   skeleton * s,
				   double angle,
				   const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotateX_center_degre (&(s->ve[i]), angle, centre);
}

/**
   Rotation d'un squelette en radian autour de l'axe Y
   @param v pointeur sur le squelette
   @param angle angle de rotation du squelette en radian
   @return aucun
*/
void a2ri_skeleton_rotateY_radian (
			     skeleton * s,
			     double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotateY_radian (&(s->ve[i]), angle);
}

/**
   Rotation d'un squelette en degré autour de l'axe Y
   @param v pointeur sur le squelette
   @param angle angle de rotation du squelette en degré
   @return aucun
*/
void a2ri_skeleton_rotateY_degre (
			    skeleton * s,
			    double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotateY_degre (&(s->ve[i]), angle);
}

/**
   Rotation d'un squelette en radian suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le squelette
   @param angle angle de rotation du squelette en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_skeleton_rotateY_center_radian (
				    skeleton * s,
				    double angle,
				    const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotateY_center_radian (&(s->ve[i]), angle, centre);
}

/**
   Rotation d'un squelette en degre suivant l'axe Y de centre (cx,cy,cz)
   @param v pointeur sur le squelette
   @param angle angle de rotation du squelette en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_skeleton_rotateY_center_degre (
				   skeleton * s,
				   double angle,
				   const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotateY_center_degre (&(s->ve[i]), angle, centre);
}

/**
   Rotation d'un squelette en radian autour de l'axe Z
   @param v pointeur sur le squelette
   @param angle angle de rotation du squelette en radian
   @return aucun
*/
void a2ri_skeleton_rotateZ_radian (
			     skeleton * s,
			     double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotateZ_radian (&(s->ve[i]), angle);
}

/**
   Rotation d'un squelette en degré autour de l'axe Z
   @param v pointeur sur le squelette
   @param angle angle de rotation du squelette en degré
   @return aucun
*/
void a2ri_skeleton_rotateZ_degre (
			    skeleton * s,
			    double angle)
{
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotateZ_degre (&(s->ve[i]), angle);
}

/**
   Rotation d'un squelette en radian suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le squelette
   @param angle angle de rotation du squelette en radian
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_skeleton_rotateZ_center_radian (
				    skeleton * s,
				    double angle,
				    const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotateZ_center_radian (&(s->ve[i]), angle, centre);
}

/**
   Rotation d'un squelette en degre suivant l'axe Z de centre (cx,cy,cz)
   @param v pointeur sur le squelette
   @param angle angle de rotation du squelette en degre
   @param centre pointeur sur le point3d servant de centre à la rotation
   @return aucun
*/
void a2ri_skeleton_rotateZ_center_degre (
				   skeleton * s,
				   double angle,
				   const point3d * const centre)
{
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotateZ_center_degre (&(s->ve[i]), angle, centre);
}

/**
   Rotation d'un squelette en degre suivant l'axe donnée
   @param m pointeur sur le squelette
   @param angle angle de rotation du squelette en degre
   @param axe l'axe de rotation
   @return aucun
*/
void a2ri_skeleton_rotate_axe_radian (
				skeleton * s,
				double angle,
				const vector3d * const axe)
{
  gsl_matrix *identity3,
    *A,
    *B,
    *M;
  double x = axe->dx;
  double y = axe->dy;
  double z = axe->dz;
  
  identity3 = gsl_matrix_alloc (3, 3);
  gsl_matrix_set_identity (identity3);
  
  A = gsl_matrix_alloc (3, 3);
  gsl_matrix_set (A, 0, 0, x * x);
  gsl_matrix_set (A, 0, 1, x * y);
  gsl_matrix_set (A, 0, 2, x * z);
  gsl_matrix_set (A, 1, 0, x * y);
  gsl_matrix_set (A, 1, 1, y * y);
  gsl_matrix_set (A, 1, 2, y * z);
  gsl_matrix_set (A, 2, 0, x * z);
  gsl_matrix_set (A, 2, 1, y * z);
  gsl_matrix_set (A, 2, 2, z * z);
  
  
  B = gsl_matrix_alloc (3, 3);
  gsl_matrix_set (B, 0, 0, 0);
  gsl_matrix_set (B, 0, 1, -z);
  gsl_matrix_set (B, 0, 2, y);
  gsl_matrix_set (B, 1, 0, z);
  gsl_matrix_set (B, 1, 1, 0);
  gsl_matrix_set (B, 1, 2, -x);
  gsl_matrix_set (B, 2, 0, -y);
  gsl_matrix_set (B, 2, 1, x);
  gsl_matrix_set (B, 2, 2, 0);
  
  
  M =
    matrix_add (matrix_add
		(matrix_mul_scale (identity3, cos (angle)),
		 matrix_mul_scale (A, 1 - cos (angle))), matrix_mul_scale (B,
									   sin
									   (angle)));
  
  
  //rotation de tous les sommets
  for (int i = 0; i < s->nbvertex; i++)
    vef_vertex_rotate_axe_radian (&(s->ve[i]), M);
}

/**
   Ajout des points d'un skeleton dans une partition de l'espace
   @param m le squelette
   @param sp la partition de l'espace
   @return aucun
*/
void a2ri_skeleton_space_partition (
			      const skeleton * const s,
			      space_partition * sp)
{
  point3d p;
  for (int i = 0; i < s->nbvertex; i++)
    {
      point3d_init (&p, s->ve[i].x, s->ve[i].y, s->ve[i].z);
      p.att_int = i;
      space_partition_add_point (sp, &p);
    }
}
