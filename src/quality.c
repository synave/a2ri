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

#include "quality.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

#define NB_SAMPLE_HAUSDORFF 50
#define NB_SAMPLE_EMN 50

typedef struct
{
  vf_model *m;
  double *list;
  int size;
} ITcouple;

//ajout de la longueur de l'arete value dans la liste contenu dans la variable user_data
void
IFlongueur_arete (
		int key,
		vf_edge * value,
		void * user_data)
{
  ITcouple *c = user_data;
  
  list_double_add (&((*c).list), &((*c).size), value->att_double,
		   WITH_REDUNDANCE);
}

/********** MAIN FUNCTIONS **********/

/**
   Calcul de l'erreur mean ratio metric sur un modèle pour une face
   @param m le modèle
   @param numface numéro de la face à évaluer
   @return évaluation de la face
*/
double
a2ri_vf_mean_ratio_metric_for_a_face (
				      const vf_model * const m,
				      int numface)
{
  int ve1 = m->fa[numface].ve1;
  int ve2 = m->fa[numface].ve2;
  int ve3 = m->fa[numface].ve3;

  point3d p[3];
  point2d pnvbase[3];
  vector3d AB,
    AC,
    U;
  double a,
    b,
    c,
    d;

  //initialisation des triangles
  point3d_init (p, m->ve[ve1].x, m->ve[ve1].y, m->ve[ve1].z);
  point3d_init (p + 1, m->ve[ve2].x, m->ve[ve2].y, m->ve[ve2].z);
  point3d_init (p + 2, m->ve[ve3].x, m->ve[ve3].y, m->ve[ve3].z);

  //recherche equation du plan
  equation_plan (p, 3, &a, &b, &c, &d);

  //changement de base
  //initialisation du premier vecteur de la nouvelle base
  vector3d_init (&AB, p[1].x - p[0].x, p[1].y - p[0].y, p[1].z - p[0].z);
  vector3d_init (&AC, p[2].x - p[0].x, p[2].y - p[0].y, p[2].z - p[0].z);

  //recherche du second vecteur de la nouvelle base
  find_second_base_vector (&AB, &AC, &U);

  //normalisation des vecteurs
  vector3d_normalize (&AB);
  vector3d_normalize (&U);


  //recuperation des nouvelles coordonnées du point dans la nouvelle base

  base_modification_3d_to_2d (&(p[0]), &(p[0]), &AB, &U, &pnvbase[0]);
  base_modification_3d_to_2d (&(p[1]), &(p[0]), &AB, &U, &pnvbase[1]);
  base_modification_3d_to_2d (&(p[2]), &(p[0]), &AB, &U, &pnvbase[2]);


  //application du calcul mean ratio metric 
  //-> article : A comparison of two optimization methods for mesh quality improvement
  gsl_matrix *A = gsl_matrix_alloc (2, 2);
  a2ri_erreur_critique_si (A == NULL,
			"erreur allocation memoire pour A\na2ri_vf_mean_ratio_metric_for_a_face");
  gsl_matrix_set (A, 0, 0, pnvbase[1].x - pnvbase[0].x);
  gsl_matrix_set (A, 0, 1, pnvbase[2].x - pnvbase[0].x);
  gsl_matrix_set (A, 1, 0, pnvbase[1].y - pnvbase[0].y);
  gsl_matrix_set (A, 1, 1, pnvbase[2].y - pnvbase[0].y);


  gsl_matrix *Winv = gsl_matrix_alloc (2, 2);
  a2ri_erreur_critique_si (Winv == NULL,
			"erreur allocation memoire pour Winv\na2ri_vf_mean_ratio_metric_for_a_face");

  gsl_matrix_set (Winv, 0, 0, 1);
  gsl_matrix_set (Winv, 0, 1, -1.0 / sqrt (3.0));
  gsl_matrix_set (Winv, 1, 0, 0);
  gsl_matrix_set (Winv, 1, 1, 2.0 / sqrt (3.0));

  gsl_matrix *S = matrix_mul (A, Winv);

  double erreurface =
    ((2 * matrix_determinant (S)) /
     (matrix_frobenius_norm (S) * matrix_frobenius_norm (S)));

  if (erreurface < 0)
    erreurface *= -1;

  gsl_matrix_free (A);
  gsl_matrix_free (Winv);
  gsl_matrix_free (S);


  return erreurface;
}

/**
   Calcul de l'erreur mean ratio metric sur un modèle complet
   @param m le modèle
   @return évaluation du modèle
*/
double
a2ri_vf_mean_ratio_metric (
			   const vf_model * const m)
{
  double somme = 0.0;

  for (int i = 0; i < m->nbface; i++)
    somme += a2ri_vf_mean_ratio_metric_for_a_face (m, i);

  somme /= (m->nbface) * 1.0;

  return somme;

}

/**
   Calcule la liste des angles des triangles composant le modèle
   @param m le modèle
   @param list pointeur sur le tableau contenant la liste des angles
   @param size pointeur sur la taille du tableau
   @return aucun
*/
void
a2ri_vf_list_angle (
		    const vf_model * const m,
		    double ** list,
		    int * size)
{
  *size = 0;
  *list = NULL;
  int ve1,
    ve2,
    ve3;
  double dx,
    dy,
    dz,
    angledegre;
  vector3d v1,
    v2;

  for (int i = 0; i < m->nbface; i++)
    {
      ve1 = m->fa[i].ve1;
      ve2 = m->fa[i].ve2;
      ve3 = m->fa[i].ve3;

      //angle ve1-ve2 ve1-ve3
      dx = m->ve[ve2].x - m->ve[ve1].x;
      dy = m->ve[ve2].y - m->ve[ve1].y;
      dz = m->ve[ve2].z - m->ve[ve1].z;
      vector3d_init (&v1, dx, dy, dz);

      dx = m->ve[ve3].x - m->ve[ve1].x;
      dy = m->ve[ve3].y - m->ve[ve1].y;
      dz = m->ve[ve3].z - m->ve[ve1].z;
      vector3d_init (&v2, dx, dy, dz);

      angledegre = vector3d_angle_degre (&v1, &v2);

      list_double_add (list, size, angledegre, WITH_REDUNDANCE);

      //angle ve2-ve1 ve2-ve3
      dx = m->ve[ve1].x - m->ve[ve2].x;
      dy = m->ve[ve1].y - m->ve[ve2].y;
      dz = m->ve[ve1].z - m->ve[ve2].z;
      vector3d_init (&v1, dx, dy, dz);

      dx = m->ve[ve3].x - m->ve[ve2].x;
      dy = m->ve[ve3].y - m->ve[ve2].y;
      dz = m->ve[ve3].z - m->ve[ve2].z;
      vector3d_init (&v2, dx, dy, dz);

      angledegre = vector3d_angle_degre (&v1, &v2);

      list_double_add (list, size, angledegre, WITH_REDUNDANCE);

      //angle ve3-ve2 ve3-ve1
      dx = m->ve[ve2].x - m->ve[ve3].x;
      dy = m->ve[ve2].y - m->ve[ve3].y;
      dz = m->ve[ve2].z - m->ve[ve3].z;
      vector3d_init (&v1, dx, dy, dz);

      dx = m->ve[ve1].x - m->ve[ve3].x;
      dy = m->ve[ve1].y - m->ve[ve3].y;
      dz = m->ve[ve1].z - m->ve[ve3].z;
      vector3d_init (&v2, dx, dy, dz);

      angledegre = vector3d_angle_degre (&v1, &v2);

      list_double_add (list, size, angledegre, WITH_REDUNDANCE);

    }
}

/**
   Calcule la liste des aires des triangles composant le modèle
   @param m le modèle
   @param list pointeur sur le tableau contenant la liste des aires
   @param size pointeur sur la taille du tableau
   @return aucun
*/
void
a2ri_vf_list_area (
		   const vf_model * const m,
		   double ** list,
		   int * size)
{
  int ve1,
    ve2,
    ve3;
  point3d p1,
    p2,
    p3;
  *list = NULL;
  *size = 0;
  double aire;

  for (int i = 0; i < m->nbface; i++)
    {
      ve1 = m->fa[i].ve1;
      ve2 = m->fa[i].ve2;
      ve3 = m->fa[i].ve3;
      point3d_init (&p1, m->ve[ve1].x, m->ve[ve1].y, m->ve[ve1].z);
      point3d_init (&p2, m->ve[ve2].x, m->ve[ve2].y, m->ve[ve2].z);
      point3d_init (&p3, m->ve[ve3].x, m->ve[ve3].y, m->ve[ve3].z);
      aire = point3d_area (&p1, &p2, &p3);
      list_double_add (list, size, aire, WITH_REDUNDANCE);
    }
}

/**
   Calcule la liste des valences des sommets composant le modèle
   @param m le modèle
   @param list pointeur sur le tableau contenant la liste des valences
   @param size pointeur sur la taille du tableau
   @return aucun
*/
void
a2ri_vf_list_valence (
		      const vf_model * const m,
		      int ** list,
		      int * size)
{
  *list = NULL;
  *size = 0;
  for (int i = 0; i < m->nbvertex; i++)
    list_int_add (list, size, m->ve[i].nbincidentvertices, WITH_REDUNDANCE);
}




/**
   Calcule la liste des longueurs des aretes composant le modèle
   @param m le modèle
   @param list pointeur sur le tableau contenant la liste des longueurs des aretes
   @param size pointeur sur la taille du tableau
   @return aucun
*/
//On ne peut pas garantir le const du vf_model ici 
void
a2ri_vf_list_edge_length (
			  vf_model * m,
			  double ** list,
			  int * size)
{
  ITcouple c;
  c.m = m;
  c.list = NULL;
  c.size = 0;
  ptf_func_hashtable func[1];

  func[0] = a2ri_vf_update_length_edge;
  //construction de la table de hachage des aretes
  hashtable *table = a2ri_vf_construction_edge_table (m, func, 1);

  //pour toute les aretes, on ajoute leur longueur dans la liste
  hashtable_foreach (table, IFlongueur_arete, &c);

  *list = c.list;
  *size = c.size;

  hashtable_free (table);
  free (table);

}

/**
   Calcule la liste des longueurs des hauteurs des triangles composant le modèle
   @param m le modèle
   @param list pointeur sur le tableau contenant la liste des hauteurs
   @param size pointeur sur la taille du tableau
   @return aucun
*/
//On ne peut pas garantir le const du vf_model ici 
void
a2ri_vf_list_height_length (
			    vf_model * m,
			    double ** list,
			    int * size)
{
  *list = NULL;
  *size = 0;
  point3d p1,
    p2,
    p3;
  vector3d v1,
    v2,
    vp;
  for (int i = 0; i < m->nbface; i++)
    {
      //initialisation des trois points correpondant aux trois sommets de la face
      point3d_init (&p1, m->ve[m->fa[i].ve1].x, m->ve[m->fa[i].ve1].y,
		    m->ve[m->fa[i].ve1].z);
      point3d_init (&p2, m->ve[m->fa[i].ve2].x, m->ve[m->fa[i].ve2].y,
		    m->ve[m->fa[i].ve2].z);
      point3d_init (&p3, m->ve[m->fa[i].ve3].x, m->ve[m->fa[i].ve3].y,
		    m->ve[m->fa[i].ve3].z);

      //ajout de la première hauteur du triangle
      vector3d_init (&v1, p3.x - p2.x, p3.y - p2.y, p3.z - p2.z);
      vector3d_init (&v2, p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
      vp = vector3d_vectorialproduct (&v1, &v2);
      list_double_add (list, size, vector3d_size (&vp) / vector3d_size (&v1),
		       WITH_REDUNDANCE);

      //ajout de la seconde hauteur du triangle
      vector3d_init (&v1, p1.x - p3.x, p1.y - p3.y, p1.z - p3.z);
      vector3d_init (&v2, p3.x - p2.x, p3.y - p2.y, p3.z - p2.z);
      vp = vector3d_vectorialproduct (&v1, &v2);
      list_double_add (list, size, vector3d_size (&vp) / vector3d_size (&v1),
		       WITH_REDUNDANCE);

      //ajout de la troisieme hauteur du triangle
      vector3d_init (&v1, p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
      vector3d_init (&v2, p1.x - p3.x, p1.y - p3.y, p1.z - p3.z);
      vp = vector3d_vectorialproduct (&v1, &v2);
      list_double_add (list, size, vector3d_size (&vp) / vector3d_size (&v1),
		       WITH_REDUNDANCE);
    }
}

/**
   Compte le nombre d'angles du modèle par intervalle de 10 degres
   @param m le modèle
   @return un tableau de 18 entiers contenant le nombre d'angle compris entre 0-10, 10-20, ... , 170-180
*/
int *
a2ri_vf_angle_measure (
		       const vf_model * const m)
{
  //|0-10|10-20|20-30|...|160-170|170-180|
  int *angle = (int *) malloc (18 * sizeof (int));
  a2ri_erreur_critique_si (angle == NULL,
			"erreur allocation memoire pour angle\na2ri_vf_angle_measure");
  int ve1,
    ve2,
    ve3,
    tabindice;
  double dx,
    dy,
    dz,
    angledegre;
  vector3d v1,
    v2;


  for (int i = 0; i < 18; i++)
    angle[i] = 0;

  for (int i = 0; i < m->nbface; i++)
    {
      ve1 = m->fa[i].ve1;
      ve2 = m->fa[i].ve2;
      ve3 = m->fa[i].ve3;

      //angle ve1-ve2 ve1-ve3
      dx = m->ve[ve2].x - m->ve[ve1].x;
      dy = m->ve[ve2].y - m->ve[ve1].y;
      dz = m->ve[ve2].z - m->ve[ve1].z;
      vector3d_init (&v1, dx, dy, dz);

      dx = m->ve[ve3].x - m->ve[ve1].x;
      dy = m->ve[ve3].y - m->ve[ve1].y;
      dz = m->ve[ve3].z - m->ve[ve1].z;
      vector3d_init (&v2, dx, dy, dz);

      angledegre = vector3d_angle_degre (&v1, &v2);

      angledegre *= 10;

      tabindice = (int) angledegre;

      if (tabindice % 10 >= 5)
	tabindice += 5;

      tabindice /= 100;

      angle[tabindice] = angle[tabindice] + 1;

      //angle ve2-ve1 ve2-ve3
      dx = m->ve[ve1].x - m->ve[ve2].x;
      dy = m->ve[ve1].y - m->ve[ve2].y;
      dz = m->ve[ve1].z - m->ve[ve2].z;
      vector3d_init (&v1, dx, dy, dz);

      dx = m->ve[ve3].x - m->ve[ve2].x;
      dy = m->ve[ve3].y - m->ve[ve2].y;
      dz = m->ve[ve3].z - m->ve[ve2].z;
      vector3d_init (&v2, dx, dy, dz);

      angledegre = vector3d_angle_degre (&v1, &v2);

      angledegre *= 10;

      tabindice = (int) angledegre;

      if (tabindice % 10 >= 5)
	tabindice += 5;

      tabindice /= 100;

      angle[tabindice] = angle[tabindice] + 1;

      //angle ve3-ve2 ve3-ve1
      dx = m->ve[ve2].x - m->ve[ve3].x;
      dy = m->ve[ve2].y - m->ve[ve3].y;
      dz = m->ve[ve2].z - m->ve[ve3].z;
      vector3d_init (&v1, dx, dy, dz);

      dx = m->ve[ve1].x - m->ve[ve3].x;
      dy = m->ve[ve1].y - m->ve[ve3].y;
      dz = m->ve[ve1].z - m->ve[ve3].z;
      vector3d_init (&v2, dx, dy, dz);

      angledegre = vector3d_angle_degre (&v1, &v2);

      angledegre *= 10;

      tabindice = (int) angledegre;

      if (tabindice % 10 >= 5)
	tabindice += 5;

      tabindice /= 100;

      angle[tabindice] = angle[tabindice] + 1;

    }

  return angle;
}


/**
   Calcul de l'erreur de Hausdorff
   @param m1 premier modèle
   @param m2 second modèle
   @return erreur de Hausdorff
*/
double
a2ri_vf_hausdorff (
		   const vf_model * const m1,
		   const vf_model * const m2,
		   int sampling)
{
  point3d *liste = NULL;
  point3d *listesamplem1 = NULL;
  point3d *listesamplem2 = NULL;
  double max = 0.0;
  point3d p,
    A,
    B,
    C;

  //premier cas : pas d'echantillonnage
  if (sampling == NO_SAMPLING)
    {
      //transformation des sommets de m2 en lsite de points
      liste = (point3d *) malloc (m2->nbvertex * sizeof (point3d));
      a2ri_erreur_critique_si (liste == NULL,
			    "erreur allocation memoire pour liste\na2ri_vf_hausdorff");
      for (int i = 0; i < m2->nbvertex; i++)
	{
	  liste[i].x = m2->ve[i].x;
	  liste[i].y = m2->ve[i].y;
	  liste[i].z = m2->ve[i].z;
	}
      //on cherche la longueur maximale entre les sommets de m1 et les somemts de m2
      for (int i = 0; i < m1->nbvertex; i++)
	{
	  point3d_init (&p, m1->ve[i].x, m1->ve[i].y, m1->ve[i].z);
	  double temp = DPP (&p, liste, m2->nbvertex);
	  if (max < temp)
	    max = temp;
	}

      free (liste);
      liste = NULL;
      //transformation des sommets de m1 en lsite de points
      liste = (point3d *) malloc (m1->nbvertex * sizeof (point3d));
      a2ri_erreur_critique_si (liste == NULL,
			    "erreur allocation memoire pour liste\na2ri_vf_hausdorff");
      for (int i = 0; i < m1->nbvertex; i++)
	{
	  liste[i].x = m1->ve[i].x;
	  liste[i].y = m1->ve[i].y;
	  liste[i].z = m1->ve[i].z;
	}
      //on cherche la longueur maximale entre les sommets de m2 et les somemts de m1
      for (int i = 0; i < m2->nbvertex; i++)
	{
	  point3d_init (&p, m2->ve[i].x, m2->ve[i].y, m2->ve[i].z);
	  double temp = DPP (&p, liste, m1->nbvertex);
	  if (max < temp)
	    max = temp;
	}
      
      free (liste);
    }
  else
    {
      listesamplem1 =
	(point3d *) malloc ((m1->nbvertex + m1->nbface * NB_SAMPLE_HAUSDORFF)
			    * sizeof (point3d));
      a2ri_erreur_critique_si (listesamplem1 == NULL,
			    "erreur allocation memoire pour listesamplem1\na2ri_vf_hausdorff");
      listesamplem2 =
	(point3d *) malloc ((m2->nbvertex + m2->nbface * NB_SAMPLE_HAUSDORFF)
			    * sizeof (point3d));
      a2ri_erreur_critique_si (listesamplem2 == NULL,
			    "erreur allocation memoire pour listesamplem2\na2ri_vf_hausdorff");

      //echantillonage de m1 et m2
      for (int i = 0; i < m1->nbvertex; i++)
	{
	  listesamplem1[i].x = m1->ve[i].x;
	  listesamplem1[i].y = m1->ve[i].y;
	  listesamplem1[i].z = m1->ve[i].z;
	}

      for (int i = 0; i < m2->nbvertex; i++)
	{
	  listesamplem2[i].x = m2->ve[i].x;
	  listesamplem2[i].y = m2->ve[i].y;
	  listesamplem2[i].z = m2->ve[i].z;
	}

      for (int i = 0; i < m1->nbface; i++)
	{
	  point3d_init (&A, m1->ve[m1->fa[i].ve1].x, m1->ve[m1->fa[i].ve1].y,
			m1->ve[m1->fa[i].ve1].z);
	  point3d_init (&B, m1->ve[m1->fa[i].ve2].x, m1->ve[m1->fa[i].ve2].y,
			m1->ve[m1->fa[i].ve2].z);
	  point3d_init (&C, m1->ve[m1->fa[i].ve3].x, m1->ve[m1->fa[i].ve3].y,
			m1->ve[m1->fa[i].ve3].z);
	  sample_triangle (&A, &B, &C, NB_SAMPLE_HAUSDORFF, &liste);
	  for (int j = 0; j < NB_SAMPLE_HAUSDORFF; j++)
	    {
	      listesamplem1[(i * NB_SAMPLE_HAUSDORFF) + m1->nbvertex + j].x =
		liste[i].x;
	      listesamplem1[(i * NB_SAMPLE_HAUSDORFF) + m1->nbvertex + j].y =
		liste[i].y;
	      listesamplem1[(i * NB_SAMPLE_HAUSDORFF) + m1->nbvertex + j].z =
		liste[i].z;
	    }
	}

      for (int i = 0; i < m2->nbface; i++)
	{
	  point3d_init (&A, m2->ve[m2->fa[i].ve1].x, m2->ve[m2->fa[i].ve1].y,
			m2->ve[m2->fa[i].ve1].z);
	  point3d_init (&B, m2->ve[m2->fa[i].ve2].x, m2->ve[m2->fa[i].ve2].y,
			m2->ve[m2->fa[i].ve2].z);
	  point3d_init (&C, m2->ve[m2->fa[i].ve3].x, m2->ve[m2->fa[i].ve3].y,
			m2->ve[m2->fa[i].ve3].z);
	  sample_triangle (&A, &B, &C, NB_SAMPLE_HAUSDORFF, &liste);
	  for (int j = 0; j < NB_SAMPLE_HAUSDORFF; j++)
	    {
	      listesamplem2[(i * NB_SAMPLE_HAUSDORFF) + m2->nbvertex + j].x =
		liste[i].x;
	      listesamplem2[(i * NB_SAMPLE_HAUSDORFF) + m2->nbvertex + j].y =
		liste[i].y;
	      listesamplem2[(i * NB_SAMPLE_HAUSDORFF) + m2->nbvertex + j].z =
		liste[i].z;
	    }
	}

      //recherche de la longueur max entre m1 et m2
      for (int i = 0; i < m1->nbvertex + m1->nbface * NB_SAMPLE_HAUSDORFF;
	   i++)
	{
	  double temp =
	    DPP (&(listesamplem1[i]), listesamplem2,
		 m2->nbvertex + m2->nbface * NB_SAMPLE_HAUSDORFF);
	  if (max < temp)
	    max = temp;
	}

      //recherche de la distance max entre m2 et m1
      for (int i = 0; i < m2->nbvertex + m2->nbface * NB_SAMPLE_HAUSDORFF;
	   i++)
	{
	  double temp =
	    DPP (&(listesamplem2[i]), listesamplem1,
		 m1->nbvertex + m1->nbface * NB_SAMPLE_HAUSDORFF);
	  if (max < temp)
	    max = temp;
	}

      free (listesamplem1);
      free (listesamplem2);

    }

  return max;
}

/**
   Calcul de la distance entre un point et un ensemble de points en se basant sur la métrique Point-Point
   @param p le point
   @param points tableau de réel contenant les coordoonées des sommets du maillage
   @param size taille du tableau
   @return distance entre le point et le modèle
*/
double
DPP (
     const point3d * const p,
     const point3d *const  points,
     int size)
{
  double min = 0;
  //on cherche la distance minimale entre un point p et un ensemble de points
  for (int i = 0; i < size; i++)
    {
      double temp = point3d_length (p, &(points[i]));
      if (i == 0)
	min = temp;
      else if (temp < min)
	min = temp;
    }
  return min;
}

/**
   Calcul de l'erreur moyenne "Emean" entre deux modèles
   @param m1 le premier modèle
   @param m2 le second modèle
   @param sampling SAMPLING ou NO_SAMPLING <BR> indique s'il faut ou non échantillonner les faces.
   @return l'erreur "Emean"
*/
double
a2ri_vf_Emn_DPP (
		 const vf_model * const m1,
		 const vf_model * const m2,
		 int sampling)
{
  point3d *liste = NULL;
  double erreur = 0.0;
  point3d p;

  //premier cas : pas d'echantillonnage
  if (sampling == NO_SAMPLING)
    {
      //transformation de la liste de sommets de m2 en liste de points
      liste = (point3d *) malloc (m2->nbvertex * sizeof (point3d));
      a2ri_erreur_critique_si (liste == NULL,
			    "erreur allocation memoire pour liste\na2ri_vf_Emn_DPP");
      for (int i = 0; i < m2->nbvertex; i++)
	{
	  liste[i].x = m2->ve[i].x;
	  liste[i].y = m2->ve[i].y;
	  liste[i].z = m2->ve[i].z;
	}

      //somme DPP(m1,m2)*DPP(m1,m2)
      for (int i = 0; i < m1->nbvertex; i++)
	{
	  point3d_init (&p, m1->ve[i].x, m1->ve[i].y, m1->ve[i].z);
	  double temp = DPP (&p, liste, m2->nbvertex);
	  erreur += temp * temp;
	}

      free (liste);
      liste = NULL;
      //transformation de la liste de sommets de m1 en liste de points
      liste = (point3d *) malloc (m1->nbvertex * sizeof (point3d));
      a2ri_erreur_critique_si (liste == NULL,
			    "erreur allocation memoire pour liste\na2ri_vf_Emn_DPP");

      for (int i = 0; i < m1->nbvertex; i++)
	{
	  liste[i].x = m1->ve[i].x;
	  liste[i].y = m1->ve[i].y;
	  liste[i].z = m1->ve[i].z;
	}
      //somme DPP(m2,m1)*DPP(m2,m1)
      for (int i = 0; i < m2->nbvertex; i++)
	{
	  point3d_init (&p, m2->ve[i].x, m2->ve[i].y, m2->ve[i].z);
	  double temp = DPP (&p, liste, m1->nbvertex);
	  erreur += temp * temp;
	}

      free (liste);

    }
  else
    {
      /*TODO meme algo avec echantillonnage*/ 
      erreur = -1;
    }

  return erreur / ((m1->nbvertex + m2->nbvertex) * 1.0);
}

/**
   Evaluation de l'aspect ratio d'un triangle
   @param m le modèle
   @param numface numéro de la face à évaluer
   @return évaluation de l'aspect ratio
*/
double
a2ri_vf_triangle_aspect_ratio (
			       const vf_model * const m,
			       int numface)
{
  double distmax = 0,
    hauteur;
  int index = 0;
  point3d p1,
    p2,
    p3;
  vector3d v1,
    v2,
    vp;
  point3d_init (&p1, m->ve[m->fa[numface].ve1].x, m->ve[m->fa[numface].ve1].y,
		m->ve[m->fa[numface].ve1].z);
  point3d_init (&p2, m->ve[m->fa[numface].ve2].x, m->ve[m->fa[numface].ve2].y,
		m->ve[m->fa[numface].ve2].z);
  point3d_init (&p3, m->ve[m->fa[numface].ve3].x, m->ve[m->fa[numface].ve3].y,
		m->ve[m->fa[numface].ve3].z);

  //on cherche le plus grand cote
  distmax = point3d_length (&p1, &p2);
  if (distmax < point3d_length (&p1, &p3))
    {
      index = 1;
      distmax = point3d_length (&p1, &p3);
    }
  if (distmax < point3d_length (&p2, &p3))
    {
      index = 2;
      distmax = point3d_length (&p2, &p3);
    }
  //on cherche la hauteur du coté le plus grand
  switch (index)
    {
    case 0:
      vector3d_init (&v1, p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
      vector3d_init (&v2, p3.x - p2.x, p3.y - p2.y, p3.z - p2.z);
      break;
    case 1:
      vector3d_init (&v1, p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);
      vector3d_init (&v2, p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
      break;
    case 2:
      vector3d_init (&v1, p2.x - p3.x, p2.y - p3.y, p2.z - p3.z);
      vector3d_init (&v2, p1.x - p3.x, p1.y - p3.y, p1.z - p3.z);
      break;
    }
  vp = vector3d_vectorialproduct (&v1, &v2);
  hauteur = vector3d_size (&vp) / vector3d_size (&v1);

  return (vector3d_size (&v1) / hauteur);
}

/**
   Evaluation de la qualité d'un triangle avec la méthode de Rypl
   @param m le modèle
   @param numface numéro de la face à évaluer
   @return évaluation de l'aspect ratio
*/
double
a2ri_vf_triangle_rypl (
		       const vf_model * const m,
		       int numface)
{
  double a,
    b,
    c,
    aire;

  point3d p1,
    p2,
    p3;

  point3d_init (&p1, m->ve[m->fa[numface].ve1].x, m->ve[m->fa[numface].ve1].y,
		m->ve[m->fa[numface].ve1].z);
  point3d_init (&p2, m->ve[m->fa[numface].ve2].x, m->ve[m->fa[numface].ve2].y,
		m->ve[m->fa[numface].ve2].z);
  point3d_init (&p3, m->ve[m->fa[numface].ve3].x, m->ve[m->fa[numface].ve3].y,
		m->ve[m->fa[numface].ve3].z);

  aire = point3d_area (&p1, &p2, &p3);

  a = point3d_length (&p2, &p3);
  a = a * a;
  b = point3d_length (&p1, &p3);
  b = b * b;
  c = point3d_length (&p1, &p2);
  c = c * c;

  return (4 * sqrt (3) * aire / (a + b + c));
}

/**
   Calcul de la liste d'aspect ratio d'un modèle
   @param m le modèle
   @param list pointeur sur un tableau contenant tous les aspect ratio des triangles composant le modèle
   @param siuze pointeur sur la taille du tableau
   @return aucun
*/
void
a2ri_vf_list_triangle_aspect_ratio (
				    const vf_model * const m,
				    double ** list,
				    int * size)
{
  *list = NULL;
  *size = 0;
  for (int i = 0; i < m->nbface; i++)
    list_double_add (list, size, a2ri_vf_triangle_aspect_ratio (m, i),
		     WITH_REDUNDANCE);
}

/**
   Calcul de la variation de taille (d'aire) entre deux faces du modèle
   @param m le modèle
   @param face1 numéro de la première face servant de base
   @param face2 numéro de la seconde face
   @return variation de la taille : Aire_face1/Aire_face2
*/
double
a2ri_vf_face_size_variation (
			     const vf_model * const m,
			     int face1,
			     int face2)
{
  point3d A,
    B,
    C,
    D,
    E,
    F;

  point3d_init (&A, m->ve[m->fa[face1].ve1].x, m->ve[m->fa[face1].ve1].y,
		m->ve[m->fa[face1].ve1].z);
  point3d_init (&B, m->ve[m->fa[face1].ve2].x, m->ve[m->fa[face1].ve2].y,
		m->ve[m->fa[face1].ve2].z);
  point3d_init (&C, m->ve[m->fa[face1].ve3].x, m->ve[m->fa[face1].ve3].y,
		m->ve[m->fa[face1].ve3].z);

  point3d_init (&D, m->ve[m->fa[face2].ve1].x, m->ve[m->fa[face2].ve1].y,
		m->ve[m->fa[face2].ve1].z);
  point3d_init (&E, m->ve[m->fa[face2].ve2].x, m->ve[m->fa[face2].ve2].y,
		m->ve[m->fa[face2].ve2].z);
  point3d_init (&F, m->ve[m->fa[face2].ve3].x, m->ve[m->fa[face2].ve3].y,
		m->ve[m->fa[face2].ve3].z);

  return (point3d_area (&A, &B, &C) / point3d_area (&D, &E, &F));

}

/**
   Calcul de la liste des rayons des cercles inscrit aux faces du modèles
   @param m le modèle
   @param list pointeur sur le tableau contenant la liste des rayons des cercles inscrits
   @param size pointeur sur la taille du tableau
   @return aucun
*/
void
a2ri_vf_list_radius_incircle (
			      const vf_model * const m,
			      double ** list,
			      int * size)
{
  point3d A,
    B,
    C,
    center;
  for (int i = 0; i < m->nbface; i++)
    {
      point3d_init (&A, m->ve[m->fa[i].ve1].x, m->ve[m->fa[i].ve1].y,
		    m->ve[m->fa[i].ve1].z);
      point3d_init (&B, m->ve[m->fa[i].ve2].x, m->ve[m->fa[i].ve2].y,
		    m->ve[m->fa[i].ve2].z);
      point3d_init (&C, m->ve[m->fa[i].ve3].x, m->ve[m->fa[i].ve3].y,
		    m->ve[m->fa[i].ve3].z);
      center = incircle_center (&A, &B, &C);
      list_double_add (list, size,
		       distance_point_straight_line (&center, &A, &B),
		       WITH_REDUNDANCE);
    }
}

/**
   Calcul de la courbure de gauss pour un sommet
   @param m le modele
   @param ve numéro du sommet
   @return le courbure de gauss en ce sommet
*/
double
a2ri_vf_gauss_curvature (
			 const vf_model * const m,
			 int ve)
{
  //TODO vérifier le bon fonctionnement de la fonction
  vector3d v1,
    v2;
  int *star = NULL,
    size = 0,
    ve1,
    ve2,
    ve3;
  double sommeangle = 0;

  a2ri_vf_faces_next_vertex (m, ve, &star, &size);


  for (int i = 0; i < size; i++)
    {
      ve1 = m->fa[star[i]].ve1;
      ve2 = m->fa[star[i]].ve2;
      ve3 = m->fa[star[i]].ve3;

      if (ve == ve2)
	{
	  ve2 = ve1;
	  ve1 = ve;
	}

      if (ve == ve3)
	{
	  ve3 = ve1;
	  ve1 = ve;
	}

      vector3d_init (&v1, m->ve[ve2].x - m->ve[ve1].x,
		     m->ve[ve2].y - m->ve[ve1].y,
		     m->ve[ve2].z - m->ve[ve1].z);
      vector3d_init (&v2, m->ve[ve3].x - m->ve[ve1].x,
		     m->ve[ve3].y - m->ve[ve1].y,
		     m->ve[ve3].z - m->ve[ve1].z);


      sommeangle += vector3d_angle_radian (&v1, &v2);
    }
  free (star);

  return (2.0 * PI - sommeangle);

}

/**
   Calcul de la courbure de gauss pour une liste de sommet
   @param m le modele
   @param ve liste de sommets
   @param size taille de la liste
   @param list liste des courbures associés à chaque sommet
   @return aucun
*/
void
a2ri_vf_gauss_curvature_list_of_vertex (
					const vf_model * const m,
					const int * const ve,
					int size,
					double ** list)
{
  //TODO vérifier le bon fonctionnement de la fonction
  int newsize = 0;
  *list = NULL;

  for (int i = 0; i < size; i++)
    list_double_add (list, &newsize, a2ri_vf_gauss_curvature (m, ve[i]),
		     WITH_REDUNDANCE);
}

/**
   calcule de la mean curvature pour une arete
   @param m le modele
   @param ve1 premier sommet de l'arete
   @param ve2 second sommet de l'arete
   @return la mean curvature pour cette arete
*/
vector3d
a2ri_vf_mean_curvature (
			const vf_model * const m,
			int ve1,
			int ve2)
{
  int *list1 = NULL,
    *list2 = NULL,
    *list3 = NULL;
  int size1 = 0,
    size2 = 0,
    size3 = 0;
  vector3d retour,
    e,
    mu1,
    mu2;

  a2ri_vf_faces_next_vertex (m, ve1, &list1, &size1);
  a2ri_vf_faces_next_vertex (m, ve2, &list2, &size2);

  list_int_intersection (list1, size1, list2, size2, &list3, &size3);

  if (size3 != 2)
    {
      vector3d_init (&retour, 0, 0, 0);
      return retour;
    }

  vector3d_init (&e, m->ve[ve1].x - m->ve[ve2].x, m->ve[ve1].y - m->ve[ve2].y,
		 m->ve[ve1].z - m->ve[ve2].z);
  mu1 = a2ri_vf_normal_face (m, list3[0]);
  mu2 = a2ri_vf_normal_face (m, list3[1]);

  vector3d_normalize (&mu1);
  vector3d_normalize (&mu2);

  vector3d vp1=vector3d_vectorialproduct (&e, &mu1);
  vector3d vp2=vector3d_vectorialproduct (&e, &mu2);

  return (vector3d_sub(&vp1,&vp2));
}

/**
   Calcul de la mean curvature pour un chemin
   @param m le modele
   @param list chemin décrit par ses sommets
   @param size taille du tableau list
   @param list_vector liste des vector3d retourner pour chaque arete
   @param size_list_vector taille de la liste list_vector
   @return aucun
*/
void
a2ri_vf_mean_curvature_path (
			     const vf_model * const m,
			     const int * const list,
			     int size,
			     vector3d ** list_vector,
			     int * size_list_vector)
{
  *size_list_vector = 0;
  *list_vector = NULL;

  for (int i = 0; i < size - 1; i++)
    {
      vector3d vtemp=a2ri_vf_mean_curvature (m, list[i], list[i + 1]);
      list_vector3d_add (list_vector, size_list_vector,
			 &vtemp,
			 WITH_REDUNDANCE);
    }
}

/**
   Calcul de la courbure de Levi-Civita
   @param m le modele
   @param numve numéro du sommet
   @return la courbure de Levi-Civita
*/
double
a2ri_vf_levi_civita (
		     const vf_model * const m,
		     int numve)
{
  double sommeangle = 0;
  vector3d v1,
    v2;
  int index,
    fa1,
    fa2,
    *dejavu = NULL,
    size = 0;

  if (m->ve[numve].nbincidentvertices == 0)
    return 0;

  int prem = m->ve[numve].incidentvertices[0];
  index = prem++;

  list_int_add (&dejavu, &size, prem, WITH_REDUNDANCE);
  list_int_add (&dejavu, &size, numve, WITH_REDUNDANCE);

  hashtable *table = a2ri_vf_construction_edge_table (m, NULL, 0);

  vf_edge *temp = hashtable_look_for (table, numve, index);

  while (index != -1)
    {
      if (temp->nbsharedfaces != 2)
	return 0;

      fa1 = temp->sharedfaces[0];
      fa2 = temp->sharedfaces[1];

      v1 = a2ri_vf_normal_face (m, fa1);
      v2 = a2ri_vf_normal_face (m, fa2);

      sommeangle += vector3d_angle_radian (&v1, &v2);

      index = -1;

      if (list_int_contains (dejavu, size, m->fa[fa1].ve1) == -1)
	index = m->fa[fa1].ve1;
      if (list_int_contains (dejavu, size, m->fa[fa1].ve2) == -1)
	index = m->fa[fa1].ve2;
      if (list_int_contains (dejavu, size, m->fa[fa1].ve3) == -1)
	index = m->fa[fa1].ve3;
      if (list_int_contains (dejavu, size, m->fa[fa2].ve1) == -1)
	index = m->fa[fa2].ve1;
      if (list_int_contains (dejavu, size, m->fa[fa2].ve2) == -1)
	index = m->fa[fa2].ve2;
      if (list_int_contains (dejavu, size, m->fa[fa2].ve3) == -1)
	index = m->fa[fa2].ve3;
      list_int_add (&dejavu, &size, index, WITH_REDUNDANCE);
      if (index != -1)
	temp = hashtable_look_for (table, numve, index);
    }

  free (dejavu);
  hashtable_free (table);
  free (table);

  return (2.0 * PI - sommeangle);
}

/**
   Calcul de la courbure de Levi-Civita
   @param m le modele
   @param ve liste des numéros de sommets
   @param size taille de la liste
   @param list liste des courbures de levi-civita
   @return aucun
*/
void
a2ri_vf_levi_civita_list_of_vertex (
				    const vf_model * const m,
				    const int * const ve,
				    int size,
				    double ** list)
{
  int newsize = 0;
  *list = NULL;

  for (int i = 0; i < size; i++)
    list_double_add (list, &newsize, a2ri_vf_levi_civita (m, ve[i]),
		     WITH_REDUNDANCE);
}


/**
   Calcul des courbures avec la méthode garimella
   @param m le modele
   @param numve numéro du sommet
   @param K gaussian curvature
   @param H mean curvature
   @return aucun
**/
void
a2ri_vf_garimella (
		   const vf_model * const m,
		   int numve,
		   double * K,
		   double * H)
{
  //TODO vérifier le bon fonctionnement de la fonction
  vector3d X,
    Y,
    Z,
    temp;
  int *listface = NULL,
    size_listface = 0;
  int *listfacestar = NULL,
    size_listfacestar = 0;
  int *listvertex = NULL,
    size_listvertex = 0;
  point3d *sommet = NULL,
    aajouter;
  int size_sommet = 0;
  double data2[3],
    *data3,
    *data4,
    data3_temp[3],
    data9_temp[9];
  double a,
    b,
    c,
    d,
    e;
  gsl_matrix *mat_identity,
    *mat_normale,
    *mat_normale_trans,
    *mat_normale_carre,
    *mat_i,
    *mat_sous,
    *mat_mul,
    *mat_R;

  mat_identity = gsl_matrix_calloc (3, 3);
  a2ri_erreur_critique_si (mat_identity == NULL,
			"erreur allocation memoire pour mat_identity\na2ri_vf_garimella");
  gsl_matrix_set_identity (mat_identity);

  //evaluer la normale au sommet numve qui est l'axe Z du nouveau repere
  a2ri_vf_faces_next_vertex (m, numve, &listface, &size_listface);
  vector3d_init (&Z, 0, 0, 0);
  for (int i = 0; i < size_listface; i++)
    {
      temp = a2ri_vf_normal_face (m, listface[i]);
      Z.dx += temp.dx;
      Z.dy += temp.dy;
      Z.dz += temp.dz;
    }

  vector3d_normalize (&Z);

  //vector3d_reverse(&Z);

  data3_temp[0] = Z.dx;
  data3_temp[1] = Z.dy;
  data3_temp[2] = Z.dz;


  mat_normale = matrix_init (data3_temp, 3, 1);
  mat_normale_trans = matrix_transpose (mat_normale);
  mat_normale_carre = matrix_mul (mat_normale, mat_normale_trans);


  if ((Z.dx == 1 || Z.dx == -1) && Z.dy == 0 && Z.dz == 0)
    {
      //cas particulier
      data3_temp[0] = 0;
      data3_temp[1] = 1;
      data3_temp[2] = 0;
    }
  else
    {
      data3_temp[0] = 1;
      data3_temp[1] = 0;
      data3_temp[2] = 0;
    }

  mat_i = matrix_init (data3_temp, 3, 1);

  mat_sous = matrix_sub (mat_identity, mat_normale_carre);

  mat_mul = matrix_mul (mat_sous, mat_i);

  X.dx = gsl_matrix_get (mat_mul, 0, 0);
  X.dy = gsl_matrix_get (mat_mul, 1, 0);
  X.dz = gsl_matrix_get (mat_mul, 2, 0);

  Y = vector3d_vectorialproduct (&X, &Z);

  vector3d_normalize (&X);
  vector3d_normalize (&Y);

  gsl_matrix_free (mat_mul);
  gsl_matrix_free (mat_sous);
  gsl_matrix_free (mat_normale_carre);
  gsl_matrix_free (mat_normale);
  gsl_matrix_free (mat_normale_trans);
  gsl_matrix_free (mat_identity);
  gsl_matrix_free (mat_i);

  data9_temp[0] = X.dx;
  data9_temp[1] = X.dy;
  data9_temp[2] = X.dz;
  data9_temp[3] = Y.dx;
  data9_temp[4] = Y.dy;
  data9_temp[5] = Y.dz;
  data9_temp[6] = Z.dx;
  data9_temp[7] = Z.dy;
  data9_temp[8] = Z.dz;

  mat_R = matrix_init (data9_temp, 3, 3);



  //selectionner au moins 5 voisins

  for (int i = 0; i < size_listface; i++)
    {
      if (m->fa[listface[i]].ve1 != numve)
	list_int_add (&listvertex, &size_listvertex, m->fa[listface[i]].ve1,
		      WITHOUT_REDUNDANCE);
      if (m->fa[listface[i]].ve2 != numve)
	list_int_add (&listvertex, &size_listvertex, m->fa[listface[i]].ve2,
		      WITHOUT_REDUNDANCE);
      if (m->fa[listface[i]].ve3 != numve)
	list_int_add (&listvertex, &size_listvertex, m->fa[listface[i]].ve3,
		      WITHOUT_REDUNDANCE);
    }

  if (size_listvertex < 5)
    {
      free (listvertex);
      listvertex = NULL;
      size_listvertex = 0;
      a2ri_vf_star (BYVERTEX, m, listface, size_listface, &listfacestar,
		    &size_listfacestar, 2);

      for (int i = 0; i < size_listfacestar; i++)
	{
	  if (m->fa[listfacestar[i]].ve1 != numve)
	    list_int_add (&listvertex, &size_listvertex,
			  m->fa[listfacestar[i]].ve1, WITHOUT_REDUNDANCE);
	  if (m->fa[listfacestar[i]].ve2 != numve)
	    list_int_add (&listvertex, &size_listvertex,
			  m->fa[listfacestar[i]].ve2, WITHOUT_REDUNDANCE);
	  if (m->fa[listfacestar[i]].ve3 != numve)
	    list_int_add (&listvertex, &size_listvertex,
			  m->fa[listfacestar[i]].ve3, WITHOUT_REDUNDANCE);
	}
    }

  free (listface);


  //changement de repère pour les voisins

  //translation des sommets
  for (int i = 0; i < size_listvertex; i++)
    {
      point3d_init (&aajouter,
		    m->ve[listvertex[i]].x - m->ve[numve].x,
		    m->ve[listvertex[i]].y - m->ve[numve].y,
		    m->ve[listvertex[i]].z - m->ve[numve].z);
      list_point3d_add (&sommet, &size_sommet, &aajouter, WITH_REDUNDANCE);
    }

  free (listvertex);


  //changement de repere avec la matrice R
  for (int i = 0; i < size_sommet; i++)
    {
      data2[0] = sommet[i].x;
      data2[1] = sommet[i].y;
      data2[2] = sommet[i].z;

      gsl_matrix *m = matrix_init (data2, 3, 1);
      gsl_matrix *mul = matrix_mul (mat_R, m);
      sommet[i].x = gsl_matrix_get (mul, 0, 0);
      sommet[i].y = gsl_matrix_get (mul, 1, 0);
      sommet[i].z = gsl_matrix_get (mul, 2, 0);
      gsl_matrix_free (m);
      gsl_matrix_free (mul);
    }


  //resolution avec la methodes des moindres carres
  data3 = (double *) malloc (5 * size_sommet * sizeof (double));
  a2ri_erreur_critique_si (data3 == NULL,
			"erreur allocation memoire pour data3\na2ri_vf_garimella");
  data4 = (double *) malloc (size_sommet * sizeof (double));
  a2ri_erreur_critique_si (data4 == NULL,
			"erreur allocation memoire pour data4\na2ri_vf_garimella");

  for (int i = 0; i < size_sommet; i++)
    {
      data3[i * 5] = sommet[i].x * sommet[i].x;
      data3[i * 5 + 1] = sommet[i].x * sommet[i].y;
      data3[i * 5 + 2] = sommet[i].y * sommet[i].y;
      data3[i * 5 + 3] = sommet[i].x;
      data3[i * 5 + 4] = sommet[i].y;
      data4[i] = sommet[i].z;
    }

  gsl_matrix *A = matrix_init (data3, size_sommet, 5);
  gsl_matrix *B = matrix_init (data4, size_sommet, 1);

  gsl_matrix *transA = matrix_transpose (A);
  gsl_matrix *multransAA = matrix_mul (transA, A);
  gsl_matrix *invmultransAA = matrix_inverse (multransAA);

  gsl_matrix *pseudoinverse = matrix_mul (invmultransAA, transA);

  gsl_matrix *x = matrix_mul (pseudoinverse, B);

  a = gsl_matrix_get (x, 0, 0);
  b = gsl_matrix_get (x, 1, 0);
  c = gsl_matrix_get (x, 2, 0);
  d = gsl_matrix_get (x, 3, 0);
  e = gsl_matrix_get (x, 4, 0);

  gsl_matrix_free (A);
  gsl_matrix_free (B);
  gsl_matrix_free (pseudoinverse);
  gsl_matrix_free (x);
  gsl_matrix_free (transA);
  gsl_matrix_free (multransAA);
  gsl_matrix_free (invmultransAA);

  free (sommet);


  //calcul des courbures
  *K = (4 * a * c * -b * b) / ((1 + d * d + e * e) * (1 + d * d + e * e));
  *H =
    (a + c + a * e * e + c * d * d -
     b * d * e) / (pow (1 + d * d + e * e, 1.5));
}

/**
   Calcul de la précision, justesse d'un squelette par rapport à un modèle numérique
   @param m le modèle
   @param s le squelette
   @return un nombre définissant la précision du squelette par rapport au modèle. Plus ce nombre sera élevé, moins le squelette sera ajusté au modèle.
 **/
int a2ri_vf_model_skeleton_accuracy(
				    const vf_model * const m,
				    const skeleton * const s)
{
  /*int non_visible=0;
  for(int i=0;i<m->nbvertex;i++)
    for(int j=0;j<s->nbvertex;j++)
      {
	int cpt=0;
	point3d s1,s2,t1,t2,t3;
	double t;
	point3d_init(&s1,m->ve[i].x,m->ve[i].y,m->ve[i].z);
	point3d_init(&s2,s->ve[j].x,s->ve[j].y,s->ve[j].z);
	point3d_init(&t1,m->ve[m->fa[cpt].ve1].x,m->ve[m->fa[cpt].ve1].y,m->ve[m->fa[cpt].ve1].z);
	point3d_init(&t2,m->ve[m->fa[cpt].ve2].x,m->ve[m->fa[cpt].ve2].y,m->ve[m->fa[cpt].ve2].z);
	point3d_init(&t3,m->ve[m->fa[cpt].ve3].x,m->ve[m->fa[cpt].ve3].y,m->ve[m->fa[cpt].ve3].z);
	
	//while(cpt<m->nbface && !intersection_segment_triangle(&s1,&s2,&t1,&t2,&t3,&t))
	// {
	//   cpt++;
	//   if(cpt!=m->nbface)
	//     {
	//point3d_init(&t1,m->ve[m->fa[cpt].ve1].x,m->ve[m->fa[cpt].ve1].y,m->ve[m->fa[cpt].ve1].z);
	//		point3d_init(&t2,m->ve[m->fa[cpt].ve2].x,m->ve[m->fa[cpt].ve2].y,m->ve[m->fa[cpt].ve2].z);
	//		point3d_init(&t3,m->ve[m->fa[cpt].ve3].x,m->ve[m->fa[cpt].ve3].y,m->ve[m->fa[cpt].ve3].z);
	//     }
	//     }
	int fin=0;
	while(cpt<m->nbface && !fin)
	  {
	    printf("-----------------\n");
	    printf("point du modele numerique : (%lf , %lf , %lf)\n",s1.x,s1.y,s1.z);
	    printf("point du squelette : (%lf , %lf , %lf)\n",s2.x,s2.y,s2.z);
	    printf("test face : %d - (%lf , %lf , %lf) (%lf , %lf , %lf) (%lf , %lf , %lf)\n",cpt,t1.x,t1.y,t1.z,t2.x,t2.y,t2.z,t3.x,t3.y,t3.z);
	    if(intersection_segment_triangle(&s1,&s2,&t1,&t2,&t3,&t))
	      printf("********************************************intersection en t = %lf - (%lf , %lf , %lf)\n",t,s1.x+t*(s2.x-s1.x),s1.y+t*(s2.y-s1.y),s1.z+t*(s2.z-s1.z));
	    else
	      printf("Pas d'intersection\n");
	    printf("-----------------\n");
	    if(!intersection_segment_triangle(&s1,&s2,&t1,&t2,&t3,&t))
	      fin=1;
	    
	    cpt++;
	    if(cpt!=m->nbface)
	      {
		point3d_init(&t1,m->ve[m->fa[cpt].ve1].x,m->ve[m->fa[cpt].ve1].y,m->ve[m->fa[cpt].ve1].z);
		point3d_init(&t2,m->ve[m->fa[cpt].ve2].x,m->ve[m->fa[cpt].ve2].y,m->ve[m->fa[cpt].ve2].z);
		point3d_init(&t3,m->ve[m->fa[cpt].ve3].x,m->ve[m->fa[cpt].ve3].y,m->ve[m->fa[cpt].ve3].z);
	      }
	  }
	
	if(cpt==m->nbface)
	  non_visible++;
      }
      return non_visible;*/
  int non_visible=0;
  point3d s1,s2,t1,t2,t3;
  double t;
  
  for(int i=0;i<m->nbvertex;i++)
    {
      point3d_init(&s1,m->ve[i].x,m->ve[i].y,m->ve[i].z);
      for(int j=0;j<s->nbvertex;j++)
	{
	  point3d_init(&s2,s->ve[j].x,s->ve[j].y,s->ve[j].z);
	  int nb_intersection=0;
	  int k=0;
	  while(k<m->nbface && nb_intersection==0)
	    {
	      point3d_init(&t1,m->ve[m->fa[k].ve1].x,m->ve[m->fa[k].ve1].y,m->ve[m->fa[k].ve1].z);
	      point3d_init(&t2,m->ve[m->fa[k].ve2].x,m->ve[m->fa[k].ve2].y,m->ve[m->fa[k].ve2].z);
	      point3d_init(&t3,m->ve[m->fa[k].ve3].x,m->ve[m->fa[k].ve3].y,m->ve[m->fa[k].ve3].z);
	      if(intersection_segment_triangle(&s1,&s2,&t1,&t2,&t3,&t))
		nb_intersection++;
	      k++;
	    }
	  if(nb_intersection>0)
	    non_visible++;
	}
    }
  return non_visible;
}
