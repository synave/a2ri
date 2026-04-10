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

#include "boundingbox.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

/********** MAIN FUNCTIONS **********/

/**
   Calcul des boites englobantes (parallélépipèdes) des faces de la liste
   @param m le modèle
   @param faces tableau contenant les numéros des faces pour lesquels on veut les boites englobantes
   @param nbfaces taille du tableau
   @param ptmin pointeur sur le tableau contenant les points (xmin,ymin,zmin) des boites englobantes
   @param ptmax pointeur sur le tableau contenant les points (xmax,ymax,zmax) des boites englobantes
**/
void
a2ri_vf_axis_aligned_bounding_box (
				   const vf_model * const m,
				   const int * const faces,
				   int nbfaces,
				   point3d ** ptmin,
				   point3d ** ptmax)
{
  //allocation de l'espace pour les tableaux ptmin et ptmax
  (*ptmin) = (point3d *) malloc (nbfaces * sizeof (point3d));
  a2ri_erreur_critique_si (*ptmin == NULL,
			"erreur allocation memoire pour ptmin\na2ri_vf_axis_aligned_bounding_box");
  (*ptmax) = (point3d *) malloc (nbfaces * sizeof (point3d));
  a2ri_erreur_critique_si (*ptmax == NULL,
			"erreur allocation memoire pour ptmax\na2ri_vf_axis_aligned_bounding_box");

  for (int i = 0; i < nbfaces; i++)
    {
      //pour la face i : renseignement des coins ptmin et ptmax avec le sommet ve1
      (*ptmin)[faces[i]].x = m->ve[m->fa[faces[i]].ve1].x;
      (*ptmin)[faces[i]].y = m->ve[m->fa[faces[i]].ve1].y;
      (*ptmin)[faces[i]].z = m->ve[m->fa[faces[i]].ve1].z;

      (*ptmax)[faces[i]].x = m->ve[m->fa[faces[i]].ve1].x;
      (*ptmax)[faces[i]].y = m->ve[m->fa[faces[i]].ve1].y;
      (*ptmax)[faces[i]].z = m->ve[m->fa[faces[i]].ve1].z;

      //mise à jour si nécessaire avec ve2 et ve3
      if (m->ve[m->fa[faces[i]].ve2].x < (*ptmin)[faces[i]].x)
	(*ptmin)[faces[i]].x = m->ve[m->fa[faces[i]].ve2].x;
      if (m->ve[m->fa[faces[i]].ve2].y < (*ptmin)[faces[i]].y)
	(*ptmin)[faces[i]].y = m->ve[m->fa[faces[i]].ve2].y;
      if (m->ve[m->fa[faces[i]].ve2].z < (*ptmin)[faces[i]].z)
	(*ptmin)[faces[i]].z = m->ve[m->fa[faces[i]].ve2].z;

      if (m->ve[m->fa[faces[i]].ve2].x > (*ptmax)[faces[i]].x)
	(*ptmax)[faces[i]].x = m->ve[m->fa[faces[i]].ve2].x;
      if (m->ve[m->fa[faces[i]].ve2].y > (*ptmax)[faces[i]].y)
	(*ptmax)[faces[i]].y = m->ve[m->fa[faces[i]].ve2].y;
      if (m->ve[m->fa[faces[i]].ve2].z > (*ptmax)[faces[i]].z)
	(*ptmax)[faces[i]].z = m->ve[m->fa[faces[i]].ve2].z;

      if (m->ve[m->fa[faces[i]].ve3].x < (*ptmin)[faces[i]].x)
	(*ptmin)[faces[i]].x = m->ve[m->fa[faces[i]].ve3].x;
      if (m->ve[m->fa[faces[i]].ve3].y < (*ptmin)[faces[i]].y)
	(*ptmin)[faces[i]].y = m->ve[m->fa[faces[i]].ve3].y;
      if (m->ve[m->fa[faces[i]].ve3].z < (*ptmin)[faces[i]].z)
	(*ptmin)[faces[i]].z = m->ve[m->fa[faces[i]].ve3].z;

      if (m->ve[m->fa[faces[i]].ve3].x > (*ptmax)[faces[i]].x)
	(*ptmax)[faces[i]].x = m->ve[m->fa[faces[i]].ve3].x;
      if (m->ve[m->fa[faces[i]].ve3].y > (*ptmax)[faces[i]].y)
	(*ptmax)[faces[i]].y = m->ve[m->fa[faces[i]].ve3].y;
      if (m->ve[m->fa[faces[i]].ve3].z > (*ptmax)[faces[i]].z)
	(*ptmax)[faces[i]].z = m->ve[m->fa[faces[i]].ve3].z;
    }
}

/**
   Calcul des sphères englobantes des faces de la liste en utilisant les fastball de Ritter
   @param m le modèle
   @param faces tableau contenant les numéros des faces pour lesquels on veut les sphères englobantes
   @param nbfaces taille du tableau
   @param listcentre pointeur sur le tableau de point3d contenant les centres de sphères englobantes
   @param listradius pointeur sur le tableau contenant les rayons des sphères englobantes
**/
void
a2ri_vf_bounding_ball_ritter (
			      const vf_model * const m,
			      const int * const faces,
			      int nbfaces,
			      point3d ** listcentre,
			      double **listradius)
{
  int nbelt = 0;

  //allocation de l'espae pour les centres des sphères
  *listcentre = (point3d *) malloc (nbfaces * sizeof (point3d));
  a2ri_erreur_critique_si (*listcentre == NULL,
			"erreur allocation memoire pour listcentre\na2ri_vf_bounding_ball_ritter");

  for (int i = 0; i < nbfaces; i++)
    {
      //pour la face faces[i]
      int ve1 = m->fa[faces[i]].ve1;
      int ve2 = m->fa[faces[i]].ve2;
      int ve3 = m->fa[faces[i]].ve3;

      //calcul de la boite englobgante
      double xmin = m->ve[ve1].x;
      double xmax = xmin;
      double ymin = m->ve[ve1].y;
      double ymax = ymin;
      double zmin = m->ve[ve1].z;
      double zmax = zmin;
      if (m->ve[ve2].x < xmin)
	xmin = m->ve[ve2].x;
      if (m->ve[ve2].x > xmax)
	xmax = m->ve[ve2].x;
      if (m->ve[ve3].x < xmin)
	xmin = m->ve[ve3].x;
      if (m->ve[ve3].x > xmax)
	xmax = m->ve[ve3].x;
      if (m->ve[ve2].y < ymin)
	ymin = m->ve[ve2].y;
      if (m->ve[ve2].y > ymax)
	ymax = m->ve[ve2].y;
      if (m->ve[ve3].y < ymin)
	ymin = m->ve[ve3].y;
      if (m->ve[ve3].y > ymax)
	ymax = m->ve[ve3].y;
      if (m->ve[ve2].z < zmin)
	zmin = m->ve[ve2].z;
      if (m->ve[ve2].z > zmax)
	zmax = m->ve[ve2].z;
      if (m->ve[ve3].z < zmin)
	zmin = m->ve[ve3].z;
      if (m->ve[ve3].z > zmax)
	zmax = m->ve[ve3].z;

      //calcul du centre de la boite englobante
      double centrex = (xmin + xmax) / 2.0;
      double centrey = (ymin + ymax) / 2.0;
      double centrez = (zmin + zmax) / 2.0;
      (*listcentre)[i].x = centrex;
      (*listcentre)[i].y = centrey;
      (*listcentre)[i].z = centrez;

      //calcul des trois rayons -> centre-ve1, centre-ve2, centre-ve3
      double radius1 =
	sqrt ((centrex - m->ve[ve1].x) * (centrex - m->ve[ve1].x) +
	      (centrey - m->ve[ve1].y) * (centrey - m->ve[ve1].y) + (centrez -
								   m->ve[ve1].
								   z) *
	      (centrez - m->ve[ve1].z));

      double radius2 =
	sqrt ((centrex - m->ve[ve2].x) * (centrex - m->ve[ve2].x) +
	      (centrey - m->ve[ve2].y) * (centrey - m->ve[ve2].y) + (centrez -
								   m->ve[ve2].
								   z) *
	      (centrez - m->ve[ve2].z));

      double radius3 =
	sqrt ((centrex - m->ve[ve3].x) * (centrex - m->ve[ve3].x) +
	      (centrey - m->ve[ve3].y) * (centrey - m->ve[ve3].y) + (centrez -
								   m->ve[ve3].
								   z) *
	      (centrez - m->ve[ve3].z));

      if (radius2 > radius1)
	radius1 = radius2;
      if (radius3 > radius1)
	radius1 = radius3;
      //ajout du rayon maximal à la liste des rayons
      list_double_add (listradius, &nbelt, radius1, WITH_REDUNDANCE);
    }
}

/**
   Calcul des sphères englobantes des faces de la liste
   @param m le modèle
   @param faces tableau contenant les numéros des faces pour lesquels on veut les sphères englobantes
   @param nbfaces taille du tableau
   @param listcentre pointeur sur le tableau de point3d contenant les centres de sphères englobantes
   @param listradius pointeur sur le tableau contenant les rayons des sphères englobantes
**/
void
a2ri_vf_bounding_ball_minimale (
				const vf_model * const m,
				const int * const faces,
				int nbfaces,
				point3d ** listcentre,
				double **listradius)
{
  double radius,
    x,
    y,
    z;
  int ve1,
    ve2,
    ve3,
    nbelt = 0;
  point3d p3d_1,
    p3d_2,
    p3d_3,
    p3d_centre;
  double longar[3],
    longp1p2,
    longp1p3,
    longp2p3;

  free (*listcentre);
  *listcentre = (point3d *) malloc (nbfaces * sizeof (point3d));
  a2ri_erreur_critique_si (*listcentre == NULL,
			"erreur allocation memoire pour listcentre\na2ri_vf_bounding_ball_minimale");

  for (int alpha = 0; alpha < nbfaces; alpha++)
    {
      ve1 = m->fa[faces[alpha]].ve1;
      ve2 = m->fa[faces[alpha]].ve2;
      ve3 = m->fa[faces[alpha]].ve3;

      point3d_init (&p3d_1, m->ve[ve1].x, m->ve[ve1].y, m->ve[ve1].z);
      point3d_init (&p3d_2, m->ve[ve2].x, m->ve[ve2].y, m->ve[ve2].z);
      point3d_init (&p3d_3, m->ve[ve3].x, m->ve[ve3].y, m->ve[ve3].z);

      longar[0] = point3d_square_length (&p3d_1, &p3d_2);
      longp1p2 = longar[0];
      longar[1] = point3d_square_length (&p3d_1, &p3d_3);
      longp1p3 = longar[1];
      longar[2] = point3d_square_length (&p3d_2, &p3d_3);
      longp2p3 = longar[2];


      list_double_sort (longar, 3, ASC);

      if (longar[2] > longar[0] + longar[1])
	{
	  if (longar[2] == longp1p2)
	    {
	      (*listcentre)[alpha].x = ((p3d_1.x + p3d_2.x) * 1.0) / 2.0;
	      (*listcentre)[alpha].y = ((p3d_1.y + p3d_2.y) * 1.0) / 2.0;
	      (*listcentre)[alpha].z = ((p3d_1.z + p3d_2.z) * 1.0) / 2.0;
	    }
	  if (longar[2] == longp1p3)
	    {
	      (*listcentre)[alpha].x = ((p3d_1.x + p3d_3.x) * 1.0) / 2.0;
	      (*listcentre)[alpha].y = ((p3d_1.y + p3d_3.y) * 1.0) / 2.0;
	      (*listcentre)[alpha].z = ((p3d_1.z + p3d_3.z) * 1.0) / 2.0;
	    }
	  if (longar[2] == longp2p3)
	    {
	      (*listcentre)[alpha].x = ((p3d_2.x + p3d_3.x) * 1.0) / 2.0;
	      (*listcentre)[alpha].y = ((p3d_2.y + p3d_3.y) * 1.0) / 2.0;
	      (*listcentre)[alpha].z = ((p3d_2.z + p3d_3.z) * 1.0) / 2.0;
	    }
	  list_double_add (listradius, &nbelt, sqrt (longar[2]) / 2.0,
			   WITH_REDUNDANCE);
	}
      else
	{
	  p3d_centre = circumcircle_center (&p3d_1, &p3d_2, &p3d_3);

	  x = p3d_centre.x;
	  y = p3d_centre.y;
	  z = p3d_centre.z;


	  radius = sqrt ((m->ve[ve1].x - x) * (m->ve[ve1].x - x) +
			 (m->ve[ve1].y - y) * (m->ve[ve1].y - y) +
			 (m->ve[ve1].z - z) * (m->ve[ve1].z - z));
	  (*listcentre)[alpha].x = x;
	  (*listcentre)[alpha].y = y;
	  (*listcentre)[alpha].z = z;
	  list_double_add (listradius, &nbelt, radius, WITH_REDUNDANCE);
	}
    }
}

/**
   Calcul l'oriented bounding box du maillage
   @param m le modèle
   @param newbasex pointeur sur le vecteur "X" de la nouvelle base dans laquelle l'oriented bounding box est représenté
   @param newbasey pointeur sur le vecteur "Y" de la nouvelle base dans laquelle l'oriented bounding box est représenté
   @param newbasez pointeur sur le vecteur "Z" de la nouvelle base dans laquelle l'oriented bounding box est représenté
   @param ptmin pointeur sur le point3d contenant le coin inférieur de l'oriented bounding box
   @param ptmax pointeur sur le point3d contenant le coin supérieur de l'oriented bounding box
**/
void
a2ri_vf_oriented_bounding_box (
			       const vf_model * const m,
			       vector3d * newbasex,
			       vector3d * newbasey,
			       vector3d * newbasez,
			       point3d * ptmin,
			       point3d * ptmax)
{
  point3d *listsommet = NULL;
  gsl_matrix *covariance,
    *matpass = NULL,
    *point,
    *res = NULL;
  double data[9];

  gsl_vector *eval = gsl_vector_alloc (3);
  a2ri_erreur_critique_si (eval == NULL,
			"erreur allocation memoire pour eval\na2ri_vf_oriented_bounding_box");
  gsl_matrix *evec = gsl_matrix_alloc (3, 3);
  a2ri_erreur_critique_si (evec == NULL,
			"erreur allocation memoire pour evec\na2ri_vf_oriented_bounding_box");
  gsl_eigen_symmv_workspace *w = gsl_eigen_symmv_alloc (3);
  a2ri_erreur_critique_si (w == NULL,
			"erreur allocation memoire pour w\na2ri_vf_oriented_bounding_box");
  gsl_vector_view evec_i;

  point = gsl_matrix_alloc (1, 3);
  a2ri_erreur_critique_si (point == NULL,
			"erreur allocation memoire pour point\na2ri_vf_oriented_bounding_box");

  //calcul de la matrice de covariance
  listsommet = (point3d *) malloc (m->nbvertex * sizeof (point3d));
  a2ri_erreur_critique_si (listsommet == NULL,
			"erreur allocation memoire pour listsommet\na2ri_vf_oriented_bounding_box");

  for (int i = 0; i < m->nbvertex; i++)
    point3d_init (&(listsommet[i]), m->ve[i].x, m->ve[i].y, m->ve[i].z);

  covariance = cross_variance (listsommet, listsommet, m->nbvertex);

  //calcul des vecteurs propres qui seront les directions principales de la faces
  gsl_eigen_symmv (covariance, eval, evec, w);

  //récupération du premier vecteur propre
  evec_i = gsl_matrix_column (evec, 0);
  //affectation du nouveau vecteur "X" dans newbasex
  newbasex->dx = gsl_vector_get (&evec_i.vector, 0);
  newbasex->dy = gsl_vector_get (&evec_i.vector, 1);
  newbasex->dz = gsl_vector_get (&evec_i.vector, 2);
  data[0] = gsl_vector_get (&evec_i.vector, 0);
  data[3] = gsl_vector_get (&evec_i.vector, 1);
  data[6] = gsl_vector_get (&evec_i.vector, 2);

  //récupération du second vecteur propre
  evec_i = gsl_matrix_column (evec, 1);
  //affectation du nouveau vecteur "Y" dans newbasey
  newbasey->dx = gsl_vector_get (&evec_i.vector, 0);
  newbasey->dy = gsl_vector_get (&evec_i.vector, 1);
  newbasey->dz = gsl_vector_get (&evec_i.vector, 2);
  data[1] = gsl_vector_get (&evec_i.vector, 0);
  data[4] = gsl_vector_get (&evec_i.vector, 1);
  data[7] = gsl_vector_get (&evec_i.vector, 2);

  //récupération du troisième vecteur propre
  evec_i = gsl_matrix_column (evec, 2);
  //affectation du nouveau vecteur "Z" dans newbaseZ
  newbasez->dx = gsl_vector_get (&evec_i.vector, 0);
  newbasez->dy = gsl_vector_get (&evec_i.vector, 1);
  newbasez->dz = gsl_vector_get (&evec_i.vector, 2);
  data[2] = gsl_vector_get (&evec_i.vector, 0);
  data[5] = gsl_vector_get (&evec_i.vector, 1);
  data[8] = gsl_vector_get (&evec_i.vector, 2);


  //construction de la matrice de passage de l'ancienne à la nouvelle base
  matpass = matrix_init (data, 3, 3);


  //calcul des nouvelles coordonnées du premier sommet et mise à jour des coins inférieurs et supérieurs dans ptmin et ptmax
  gsl_matrix_set (point, 0, 0, m->ve[0].x);
  gsl_matrix_set (point, 0, 1, m->ve[0].y);
  gsl_matrix_set (point, 0, 2, m->ve[0].z);
  res = matrix_mul (point, matpass);
  ptmin->x = gsl_matrix_get (res, 0, 0);
  ptmax->x = gsl_matrix_get (res, 0, 0);
  ptmin->y = gsl_matrix_get (res, 0, 1);
  ptmax->y = gsl_matrix_get (res, 0, 1);
  ptmin->z = gsl_matrix_get (res, 0, 2);
  ptmax->z = gsl_matrix_get (res, 0, 2);

  //calcul des nouvelles coordonnées des autres sommets et mise à jour des coins inférieurs et supérieurs dans ptmin et ptmax
  for (int i = 1; i < m->nbvertex; i++)
    {
      gsl_matrix_set (point, 0, 0, m->ve[i].x);
      gsl_matrix_set (point, 0, 1, m->ve[i].y);
      gsl_matrix_set (point, 0, 2, m->ve[i].z);
      res = matrix_mul (point, matpass);
      if (ptmin->x > gsl_matrix_get (res, 0, 0))
	ptmin->x = gsl_matrix_get (res, 0, 0);
      if (ptmax->x < gsl_matrix_get (res, 0, 0))
	ptmax->x = gsl_matrix_get (res, 0, 0);
      if (ptmin->y > gsl_matrix_get (res, 0, 1))
	ptmin->y = gsl_matrix_get (res, 0, 1);
      if (ptmax->y < gsl_matrix_get (res, 0, 1))
	ptmax->y = gsl_matrix_get (res, 0, 1);
      if (ptmin->z > gsl_matrix_get (res, 0, 2))
	ptmin->z = gsl_matrix_get (res, 0, 2);
      if (ptmax->z < gsl_matrix_get (res, 0, 2))
	ptmax->z = gsl_matrix_get (res, 0, 2);
    }

  gsl_matrix_free (covariance);
  gsl_matrix_free (matpass);
  gsl_matrix_free (point);
  gsl_matrix_free (res);
  gsl_eigen_symmv_free (w);

}


/**
   Calcul des sphères englobantes des faces de la liste
   @param m le modèle
   @param faces tableau contenant les numéros des faces pour lesquels on veut les sphères englobantes
   @param nbfaces taille du tableau
   @param listcentre pointeur sur le tableau de point3d contenant les centres de sphères englobantes
   @param listradius pointeur sur le tableau contenant les rayons des sphères englobantes
**/
void
a2ri_vef_bounding_ball_minimale (
				 const vef_model * const m,
				 const int * const faces,
				 int nbfaces,
				 point3d ** listcentre,
				 double **listradius)
{
  double radius,
    x,
    y,
    z;
  int ve1,
    ve2,
    ve3,
    nbelt = 0;
  point3d p3d_1,
    p3d_2,
    p3d_3,
    p3d_centre;
  double longar[3],
    longp1p2,
    longp1p3,
    longp2p3;

  free (*listcentre);
  *listcentre = (point3d *) malloc (nbfaces * sizeof (point3d));
  a2ri_erreur_critique_si (*listcentre == NULL,
			"erreur allocation memoire pour listcentre\na2ri_vef_bounding_ball_minimale");

  for (int alpha = 0; alpha < nbfaces; alpha++)
    {
      vef_face_get_vertices (&(m->fa[faces[alpha]]), m->ed, &ve1, &ve2, &ve3);

      point3d_init (&p3d_1, m->ve[ve1].x, m->ve[ve1].y, m->ve[ve1].z);
      point3d_init (&p3d_2, m->ve[ve2].x, m->ve[ve2].y, m->ve[ve2].z);
      point3d_init (&p3d_3, m->ve[ve3].x, m->ve[ve3].y, m->ve[ve3].z);

      longar[0] = point3d_square_length (&p3d_1, &p3d_2);
      longp1p2 = longar[0];
      longar[1] = point3d_square_length (&p3d_1, &p3d_3);
      longp1p3 = longar[1];
      longar[2] = point3d_square_length (&p3d_2, &p3d_3);
      longp2p3 = longar[2];


      list_double_sort (longar, 3, ASC);

      if (longar[2] > longar[0] + longar[1])
	{
	  if (longar[2] == longp1p2)
	    {
	      (*listcentre)[alpha].x = ((p3d_1.x + p3d_2.x) * 1.0) / 2.0;
	      (*listcentre)[alpha].y = ((p3d_1.y + p3d_2.y) * 1.0) / 2.0;
	      (*listcentre)[alpha].z = ((p3d_1.z + p3d_2.z) * 1.0) / 2.0;
	    }
	  if (longar[2] == longp1p3)
	    {
	      (*listcentre)[alpha].x = ((p3d_1.x + p3d_3.x) * 1.0) / 2.0;
	      (*listcentre)[alpha].y = ((p3d_1.y + p3d_3.y) * 1.0) / 2.0;
	      (*listcentre)[alpha].z = ((p3d_1.z + p3d_3.z) * 1.0) / 2.0;
	    }
	  if (longar[2] == longp2p3)
	    {
	      (*listcentre)[alpha].x = ((p3d_2.x + p3d_3.x) * 1.0) / 2.0;
	      (*listcentre)[alpha].y = ((p3d_2.y + p3d_3.y) * 1.0) / 2.0;
	      (*listcentre)[alpha].z = ((p3d_2.z + p3d_3.z) * 1.0) / 2.0;
	    }
	  list_double_add (listradius, &nbelt, sqrt (longar[2]) / 2.0,
			   WITH_REDUNDANCE);
	}
      else
	{
	  p3d_centre = circumcircle_center (&p3d_1, &p3d_2, &p3d_3);

	  x = p3d_centre.x;
	  y = p3d_centre.y;
	  z = p3d_centre.z;


	  radius = sqrt ((m->ve[ve1].x - x) * (m->ve[ve1].x - x) +
			 (m->ve[ve1].y - y) * (m->ve[ve1].y - y) +
			 (m->ve[ve1].z - z) * (m->ve[ve1].z - z));
	  (*listcentre)[alpha].x = x;
	  (*listcentre)[alpha].y = y;
	  (*listcentre)[alpha].z = z;
	  list_double_add (listradius, &nbelt, radius, WITH_REDUNDANCE);
	}
    }
}

/**
   Calcul des sphères englobantes des faces de la liste
   @param m le modèle
   @param faces tableau contenant les numéros des faces pour lesquels on veut les sphères englobantes
   @param nbfaces taille du tableau
   @param listcentre pointeur sur le tableau de point3d contenant les centres de sphères englobantes
   @param listradius pointeur sur le tableau contenant les rayons des sphères englobantes
**/
void
point3d_bounding_ball_minimale (
				const point3d * const faces,
				int nbfaces,
				point3d ** listcentre,
				double **listradius)
{
  double radius,
    x,
    y,
    z;
  int nbelt = 0;
  point3d p3d_1,
    p3d_2,
    p3d_3,
    p3d_centre;
  double longar[3],
    longp1p2,
    longp1p3,
    longp2p3;

  free (*listcentre);
  *listcentre = (point3d *) malloc (nbfaces * sizeof (point3d));
  a2ri_erreur_critique_si (*listcentre == NULL,
			"erreur allocation memoire pour lsitcentre\nvef_bounding_ball_minimale");

  for (int alpha = 0; alpha < nbfaces; alpha++)
    {
      point3d_init (&p3d_1, faces[alpha * 3].x, faces[alpha * 3].y,
		    faces[alpha * 3].z);
      point3d_init (&p3d_2, faces[alpha * 3 + 1].x, faces[alpha * 3 + 1].y,
		    faces[alpha * 3 + 1].z);
      point3d_init (&p3d_3, faces[alpha * 3 + 2].x, faces[alpha * 3 + 2].y,
		    faces[alpha * 3 + 2].z);

      longar[0] = point3d_square_length (&p3d_1, &p3d_2);
      longp1p2 = longar[0];
      longar[1] = point3d_square_length (&p3d_1, &p3d_3);
      longp1p3 = longar[1];
      longar[2] = point3d_square_length (&p3d_2, &p3d_3);
      longp2p3 = longar[2];


      list_double_sort (longar, 3, ASC);

      if (longar[2] > longar[0] + longar[1])
	{
	  if (longar[2] == longp1p2)
	    {
	      (*listcentre)[alpha].x = ((p3d_1.x + p3d_2.x) * 1.0) / 2.0;
	      (*listcentre)[alpha].y = ((p3d_1.y + p3d_2.y) * 1.0) / 2.0;
	      (*listcentre)[alpha].z = ((p3d_1.z + p3d_2.z) * 1.0) / 2.0;
	    }
	  if (longar[2] == longp1p3)
	    {
	      (*listcentre)[alpha].x = ((p3d_1.x + p3d_3.x) * 1.0) / 2.0;
	      (*listcentre)[alpha].y = ((p3d_1.y + p3d_3.y) * 1.0) / 2.0;
	      (*listcentre)[alpha].z = ((p3d_1.z + p3d_3.z) * 1.0) / 2.0;
	    }
	  if (longar[2] == longp2p3)
	    {
	      (*listcentre)[alpha].x = ((p3d_2.x + p3d_3.x) * 1.0) / 2.0;
	      (*listcentre)[alpha].y = ((p3d_2.y + p3d_3.y) * 1.0) / 2.0;
	      (*listcentre)[alpha].z = ((p3d_2.z + p3d_3.z) * 1.0) / 2.0;
	    }
	  list_double_add (listradius, &nbelt, sqrt (longar[2]) / 2.0,
			   WITH_REDUNDANCE);
	}
      else
	{
	  p3d_centre = circumcircle_center (&p3d_1, &p3d_2, &p3d_3);

	  x = p3d_centre.x;
	  y = p3d_centre.y;
	  z = p3d_centre.z;


	  radius = sqrt ((p3d_1.x - x) * (p3d_1.x - x) +
			 (p3d_1.y - y) * (p3d_1.y - y) +
			 (p3d_1.z - z) * (p3d_1.z - z));
	  (*listcentre)[alpha].x = x;
	  (*listcentre)[alpha].y = y;
	  (*listcentre)[alpha].z = z;
	  list_double_add (listradius, &nbelt, radius, WITH_REDUNDANCE);
	}
    }
}
