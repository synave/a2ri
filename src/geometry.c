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

#include "geometry.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */


void 
IFplanecoeff (
	      const point3d * const A,
	      const point3d * const B,
	      const point3d * const C,
	      vector3d * N,
	      double *D)
{
  vector3d AB,
    AC;
  vector3d_init (&AB, B->x - A->x, B->y - A->y, B->z - A->z);
  vector3d_init (&AC, C->x - A->x, C->y - A->y, C->z - A->z);
  *N = vector3d_vectorialproduct (&AB, &AC);
  vector3d_init (&AB, A->x, A->y, A->z);
  *D = vector3d_scalarproduct (&AB, N);
}

/********** MAIN FUNCTIONS **********/

/**
   Calcul de la longueur entre deux point3d
   @param p1 premier point3d
   @param p2 second point3d
   @return longueur \f$p_1p_2\f$
*/
double
point3d_length (
		const point3d * const p1,
		const point3d * const p2)
{
  return sqrt (point3d_square_length (p1, p2));
}

/**
   Calcul de la longueur au carré entre deux point3d
   @param p1 premier point3d
   @param p2 second point3d
   @return longueur \f$p_1p_2\f$
*/
double
point3d_square_length (
		       const point3d * const p1,
		       const point3d * const p2)
{
  return (p2->x - p1->x) * (p2->x - p1->x) + (p2->y - p1->y) * (p2->y - p1->y) +
    (p2->z - p1->z) * (p2->z - p1->z);
}

/**
   Calcul de la longueur entre deux point2d
   @param p1 premier point2d
   @param p2 second point2d
   @return longueur \f$p_1p_2\f$
*/
double
point2d_length (
		const point2d * const p1,
		const point2d * const p2)
{
  return sqrt ((p2->x - p1->x) * (p2->x - p1->x) + (p2->y - p1->y) * (p2->y - p1->y));
}

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
void
equation_plan (
	       const point3d * const p,
	       int size,
	       double *aeq,
	       double *beq,
	       double *ceq,
	       double *deq)
{
  double a = 0;
  double b = 0;
  double c = 0;
  double d = 0;
  double xav = 0;
  double yav = 0;
  double zav = 0;

  for (int i = 0; i < size; i++)
    {
      a += (p[i].y - p[(i + 1) % size].y) * (p[i].z + p[(i + 1) % size].z);
      b += (p[i].z - p[(i + 1) % size].z) * (p[i].x + p[(i + 1) % size].x);
      c += (p[i].x - p[(i + 1) % size].x) * (p[i].y + p[(i + 1) % size].y);
      xav += p[i].x;
      yav += p[i].y;
      zav += p[i].z;
    }

  xav /= (size * 1.0);
  yav /= (size * 1.0);
  zav /= (size * 1.0);
  d = -(xav * a + yav * b + zav * c);

  *aeq = a;
  *beq = b;
  *ceq = c;
  *deq = d;
}


/**
   Calcul de l'aire du triangle défini par les trois points
   @param p1 premier point du triangle
   @param p2 second point
   @param p3 troisième point
   @return aire du triangle
*/
double
point3d_area (
	      const point3d * const p1,
	      const point3d * const p2,
	      const point3d * const p3)
{
  vector3d AB,
    AC,
    U;

  vector3d_init (&AB, p2->x - p1->x, p2->y - p1->y, p2->z - p1->z);
  vector3d_init (&AC, p3->x - p1->x, p3->y - p1->y, p3->z - p1->z);

  U = vector3d_vectorialproduct (&AB, &AC);

  //aire du triangle = 0.5 * norme du produit vectoriel AB AC
  return (0.5 * vector3d_size (&U));
}

/**
   Transformation des coordonnées d'un point dans un espace à trois dimension en coordonnées dans un espace à deux dimensions dont la base est le couple (base1,base2) et d'origine le point3d "origin"
   @param p point dont les coordonnées vont etre recalculées
   @param origin point origine du nouveau repére
   @param base1 premier vecteur formant la nouvelle base
   @param base2 second vecteur formant la nouvelle base
   @param newp point contenant les coordonnées dans la nouvelle base
   @return aucun
*/
void
base_modification_3d_to_2d (
			    const point3d * const p,
			    const point3d * const origin,
			    const vector3d * const base1,
			    const vector3d * const base2,
			    point2d * newp)
{
  //résolution du système de trois équations à deux inconnues grace à la méthode des moindres carrés
  point3d ptemp;

  point3d_init(&ptemp,p->x-origin->x,p->y-origin->y,p->z-origin->z);


  double left_data[6];
  double right_data[3];

  left_data[0] = base1->dx;
  left_data[1] = base2->dx;
  left_data[2] = base1->dy;
  left_data[3] = base2->dy;
  left_data[4] = base1->dz;
  left_data[5] = base2->dz;
  right_data[0] = ptemp.x;
  right_data[1] = ptemp.y;
  right_data[2] = ptemp.z;

  gsl_matrix *A = matrix_init (left_data, 3, 2);
  gsl_matrix *Y = matrix_init (right_data, 3, 1);
  gsl_matrix *transA = matrix_transpose (A);
  gsl_matrix *multransAA = matrix_mul (transA, A);
  gsl_matrix *invmultransAA = matrix_inverse (multransAA);

  gsl_matrix *pseudoinverse = matrix_mul (invmultransAA, transA);

  gsl_matrix *inc = matrix_mul (pseudoinverse, Y);

  newp->x = gsl_matrix_get (inc, 0, 0);
  newp->y = gsl_matrix_get (inc, 1, 0);

  gsl_matrix_free (A);
  gsl_matrix_free (Y);
  gsl_matrix_free (pseudoinverse);
  gsl_matrix_free (inc);
  gsl_matrix_free (transA);
  gsl_matrix_free (multransAA);
  gsl_matrix_free (invmultransAA);

}

/**
   Calcul du vecteur normal à AB et contenu dans le meme plan que celui formé par les vecteurs (AB,AC). Le résultat final est une base orthogonal du repére.
   @param AB premier vecteur de la base
   @param AC second vecteur du repére
   @param U pointeur sur le nouveau vecteur formant la base orthogonal
*/
void
find_second_base_vector (
			 const vector3d * const AB,
			 const vector3d * const AC,
			 vector3d * U)
{
  vector3d W,
    Utemp;

  W = vector3d_vectorialproduct (AB, AC);
  Utemp = vector3d_vectorialproduct (&W, AB);

  U->dx = Utemp.dx;
  U->dy = Utemp.dy;
  U->dz = Utemp.dz;

}

/**
   Vérification qu'un point M se trouve bien dans le triangle ABC
   @param M le point à tester
   @param A premier sommet du triangle
   @param B second sommet du triangle
   @param C troisième sommet du triangle
   @return 1 si le point M appartient au triangle ABC, 0 sinon
*/
int
point_in_triangle (
		   const point3d * const M,
		   const point3d * const A,
		   const point3d * const B,
		   const point3d * const C)
{
  double sommeangle = 0;
  vector3d MA,
    MB,
    MC;

  vector3d_init (&MA, A->x - M->x, A->y - M->y, A->z - M->z);
  vector3d_init (&MB, B->x - M->x, B->y - M->y, B->z - M->z);
  vector3d_init (&MC, C->x - M->x, C->y - M->y, C->z - M->z);

  sommeangle += vector3d_angle_degre (&MA, &MB);
  sommeangle += vector3d_angle_degre (&MB, &MC);
  sommeangle += vector3d_angle_degre (&MC, &MA);

  //le point est contenu dans le triangle si la somme des angles est égale à 360
  return (egalite (sommeangle, 360));

}

/**
   Calcule l'angle en radian formé par les deux vector3d
   @param v1 le premier vector3d
   @param v2 le second vector3d
   @return angle en radian
*/
double
vector3d_angle_radian (
		       const vector3d * const v1,
		       const vector3d * const v2)
{
  vector3d v1temp,v2temp;
  vector3d_init(&v1temp,v1->dx,v1->dy,v1->dz);
  vector3d_init(&v2temp,v2->dx,v2->dy,v2->dz);
  vector3d_normalize (&v1temp);
  vector3d_normalize (&v2temp);
  double prodscal = vector3d_scalarproduct (&v1temp, &v2temp);
  if (prodscal < -1)
    prodscal = -1;
  if (prodscal > 1)
    prodscal = 1;
  return acos (prodscal);
}

/**
   Calcule l'angle en degré formé par les deux vector3d
   @param v1 le premier vector3d
   @param v2 le second vector3d
   @return angle en degré
*/
double
vector3d_angle_degre (
		      const vector3d * const v1,
		      const vector3d * const v2)
{
  vector3d v1temp,v2temp;
  vector3d_init(&v1temp,v1->dx,v1->dy,v1->dz);
  vector3d_init(&v2temp,v2->dx,v2->dy,v2->dz);
  vector3d_normalize (&v1temp);
  vector3d_normalize (&v2temp);
  double prodscal = vector3d_scalarproduct (&v1temp, &v2temp);
  if (prodscal < -1)
    prodscal = -1;
  if (prodscal > 1)
    prodscal = 1;

  return (acos (prodscal)) * (360 / (2 * PI));
}

/**
   Calcule le centre du cercle inscrit à un triangle
   @param A premier point du triangle
   @param B second point
   @param C troisème point
   @return le point centre du cercle inscrit
*/
point3d
incircle_center (
		 const point3d * const A,
		 const point3d * const B,
		 const point3d * const C)
{
  point3d center;
  double a = point3d_length (B, C);
  double b = point3d_length (A, C);
  double c = point3d_length (A, B);

  center.x = (a * A->x + b * B->x + c * C->x) / (a + b + c);
  center.y = (a * A->y + b * B->y + c * C->y) / (a + b + c);
  center.z = (a * A->z + b * B->z + c * C->z) / (a + b + c);
  center.att_int = 0;
  center.att_double = 0;

  return center;
}

/**
   Calcule le centre du cercle circonscrit à un triangle
   @param A premier point du triangle
   @param B second point
   @param C troisème point
   @return le point centre du cercle inscrit
*/
point3d
circumcircle_center (
		     point3d * A,
		     point3d * B,
		     point3d * C)
{
  /** méthode du bouquin : 3D Math Pimer for Graphics and Game Development **/

  vector3d e1,
    e2,
    e3,
    me1,
    me2,
    me3;
  double d1,
    d2,
    d3,
    c1,
    c2,
    c3,
    c,
    alpha,
    beta,
    gamma;
  point3d centre;

  vector3d_init (&e1, C->x - B->x, C->y - B->y, C->z - B->z);
  vector3d_init (&e2, A->x - C->x, A->y - C->y, A->z - C->z);
  vector3d_init (&e3, B->x - A->x, B->y - A->y, B->z - A->z);
  vector3d_init (&me1, B->x - C->x, B->y - C->y, B->z - C->z);
  vector3d_init (&me2, C->x - A->x, C->y - A->y, C->z - A->z);
  vector3d_init (&me3, A->x - B->x, A->y - B->y, A->z - B->z);

  d1 = vector3d_scalarproduct (&me2, &e3);
  d2 = vector3d_scalarproduct (&me3, &e1);
  d3 = vector3d_scalarproduct (&me1, &e2);

  c1 = d2 * d3;
  c2 = d3 * d1;
  c3 = d1 * d2;

  c = c1 + c2 + c3;

  alpha = (c2 + c3) / (2.0 * c);
  beta = (c3 + c1) / (2.0 * c);
  gamma = (c1 + c2) / (2.0 * c);

  centre.x = (A->x * alpha + B->x * beta + C->x * gamma);
  centre.y = (A->y * alpha + B->y * beta + C->y * gamma);
  centre.z = (A->z * alpha + B->z * beta + C->z * gamma);

  centre.att_double = 0;
  centre.att_int = 0;

  return centre;
}

/**
   Calcule le centre de sphère circonscrite au tétraédre
   @param A premier point du tétraédre
   @param B second point
   @param C troisème point
   @param D quatrième point
   @return le point centre de la sphère circonscrite
*/
point3d
circumsphere_center (
		     point3d * A,
		     point3d * B,
		     point3d * C,
		     point3d * D)
{
  double dataL[9] = { 2 * (B->x - A->x), 2 * (B->y - A->y), 2 * (B->z - A->z),
		      2 * (C->x - A->x), 2 * (C->y - A->y), 2 * (C->z - A->z),
		      2 * (D->x - A->x), 2 * (D->y - A->y), 2 * (D->z - A->z)
  };
  double dataR[3] = { point3d_length (A, B) * point3d_length (A, B),
		      point3d_length (A, C) * point3d_length (A, C),
		      point3d_length (A, D) * point3d_length (A, D)
  };

  gsl_matrix *matA = matrix_init (dataL, 3, 3);
  gsl_matrix *Y = matrix_init (dataR, 3, 1);

  gsl_matrix *pseudoinverse =
    matrix_mul (matrix_inverse (matrix_mul (matrix_transpose (matA), matA)),
		matrix_transpose (matA));

  gsl_matrix *inc = matrix_mul (pseudoinverse, Y);

  point3d centre;
  point3d_init (&centre, A->x + gsl_matrix_get (inc, 0, 0),
		A->y + gsl_matrix_get (inc, 1, 0), A->z + gsl_matrix_get (inc,
									2,
									0));

  return centre;
}

/**
   Calcule la distance entre un point M et une droite définie par deux points A et B
   @param M point
   @param A premier point sur la droite
   @param B second point sur la droite
   @return distance entre le point M et la droite AB
*/
double
distance_point_straight_line (
			      point3d * M,
			      point3d * A,
			      point3d * B)
{
  double aire = point3d_area (M, A, B);
  return (2.0 * aire) / point3d_length (A, B);
}

/**
   Calcule la distance entre un point M et un plan définie par trois points A,b et C
   @param M point
   @param A premier point du plan
   @param B second point du plan
   @param C troisieme point du plan
   @return distance entre le point M et le plan ABC
*/
double
distance_point_plane (
		      point3d * M,
		      point3d * A,
		      point3d * B,
		      point3d * C)
{
  double a,
    b,
    c,
    d;
  point3d p[3];

  point3d_init (&p[0], A->x, A->y, A->z);
  point3d_init (&p[1], B->x, B->y, B->z);
  point3d_init (&p[2], C->x, C->y, C->z);


  equation_plan (p, 3, &a, &b, &c, &d);

  return (fabs (a * M->x + b * M->y + c * M->z + d) /
	  (sqrt (a * a + b * b + c * c)));
}

/**
   Calcule la distance entre un point M et un triangle défini par trois points A,b et C
   @param M point
   @param A premier point du triangle
   @param B second point du triangle
   @param C troisieme point du triangle
   @return distance entre le point M et le triangle ABC
*/
double
distance_point_triangle (
			 point3d * M,
			 point3d * A,
			 point3d * B,
			 point3d * C)
{
  vector3d AB,
    AC,
    U,
    V;
  point2d Mb,
    Ab,
    Bb,
    Cb;
  point3d Mc,
    Ac,
    Bc,
    Cc;
  double d[3];

  vector3d_init (&AB, B->x - A->x, B->y - A->y, B->z - A->z);
  vector3d_init (&AC, C->x - A->x, C->y - A->y, C->z - A->z);

  vector3d_normalize (&AB);
  U = AB;
  find_second_base_vector (&AB, &AC, &V);
  base_modification_3d_to_2d (M, A, &U, &V, &Mb);
  base_modification_3d_to_2d (A, A, &U, &V, &Ab);
  base_modification_3d_to_2d (B, A, &U, &V, &Bb);
  base_modification_3d_to_2d (C, A, &U, &V, &Cb);

  point3d_init (&Mc, Mb.x, Mb.y, 0);
  point3d_init (&Ac, Ab.x, Ab.y, 0);
  point3d_init (&Bc, Bb.x, Bb.y, 0);
  point3d_init (&Cc, Cb.x, Cb.y, 0);

  if (point_in_triangle (&Mc, &Ac, &Bc, &Cc))
    return distance_point_plane (M, A, B, C);
  else
    {
      d[0] = point3d_length (M, A);
      d[1] = point3d_length (M, B);
      d[2] = point3d_length (M, C);
      list_double_sort (d, 3, ASC);
      return d[0];
    }

}

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
void
sample_triangle (
		 const point3d * const A,
		 const point3d * const B,
		 const point3d * const C,
		 int nb_sample,
		 point3d ** list)
{
  vector3d AB,
    AC;
  double alea1,
    alea2,
    alea3,
    somme,
    echx,
    echy,
    echz;

  *list = (point3d *) malloc (nb_sample * sizeof (point3d));
  a2ri_erreur_critique_si (*list == NULL,
			"erreur allocation memoire pour list\nsample_triangle");

  srand (time (NULL));

  vector3d_init (&AB, B->x - A->x, B->y - A->y, B->z - A->z);
  vector3d_init (&AC, C->x - A->x, C->y - A->y, C->z - A->z);

  for (int i = 0; i < nb_sample; i++)
    {
      alea1 = (rand () % 1000) / 1000.0;
      alea2 = (rand () % 1000) / 1000.0;
      alea3 = (rand () % 1000) / 1000.0;
      somme = alea1 + alea2 + alea3;
      alea1 /= somme;
      alea2 /= somme;
      alea3 /= somme;
      echx = A->x + alea1 * AB.dx + alea1 * AC.dx;
      echy = A->y + alea2 * AB.dy + alea2 * AC.dy;
      echz = A->z + alea3 * AB.dz + alea3 * AC.dz;
      (*list)[i].x = echx;
      (*list)[i].y = echy;
      (*list)[i].z = echz;
    }
}

/**
   Test de l'intersection entre une droite et un plan
   @param d1 premier point définissant la droite
   @param d2 second point définissant la droite
   @param p1 premier point du plan
   @param p2 second point du plan
   @param p3 troisieme point du plan
   @param t paramètre d'intersection
   @return 1 si intersection, 0 sinon
*/
int
intersection_droite_plan (
			  point3d * d1,
			  point3d * d2,
			  point3d * p1,
			  point3d * p2,
			  point3d * p3,
			  double *t)
{
  vector3d N,
    rq;
  double D,
    num,
    denom;
  IFplanecoeff (p1, p2, p3, &N, &D);
  vector3d_init (&rq, d1->x, d1->y, d1->z);
  num = D - vector3d_scalarproduct (&rq, &N);
  vector3d_init (&rq, d2->x - d1->x, d2->y - d1->y, d2->z - d1->z);
  denom = vector3d_scalarproduct (&rq, &N);

  if (denom == 0.0)
    {
      if (num == 0.0)
	{
	  *t = 0.0;
	  return 1;
	}
      else
	return 0;
    }

  *t = num / denom;

  return 1;
}

/**
   Test de l'intersection entre une droite et un triangle
   @param d1 premier point définissant la droite
   @param d2 second point définissant la droite
   @param p1 premier point du triangle
   @param p2 second point du triangle
   @param p3 troisieme point du triangle
   @return 1 si intersection, 0 sinon
*/
int
intersection_droite_triangle (
			      point3d * d1,
			      point3d * d2,
			      point3d * p1,
			      point3d * p2,
			      point3d * p3,
			      double *t)
{
  point3d inter;

  intersection_droite_plan (d1, d2, p1, p2, p3, t);

  point3d_init (&inter, d1->x + (*t) * (d2->x - d1->x),
		d1->y + (*t) * (d2->y - d1->y), d1->z + (*t) * (d2->z - d1->z));

  return point_in_triangle (&inter, p1, p2, p3);
}

/**
   Test de l'intersection entre un segment et un triangle
   @param s1 premier point définissant le segment
   @param s2 second point définissant le segment
   @param p1 premier point du triangle
   @param p2 second point du triangle
   @param p3 troisieme point du triangle
   @return 1 si intersection, 0 sinon
*/
int
intersection_segment_triangle (
			      point3d * s1,
			      point3d * s2,
			      point3d * p1,
			      point3d * p2,
			      point3d * p3,
			      double *t)
{
  point3d inter;

  intersection_droite_plan (s1, s2, p1, p2, p3, t);

  if(*t>=0 && *t<=1.0){

    point3d_init (&inter, s1->x + (*t) * (s2->x - s1->x),
		  s1->y + (*t) * (s2->y - s1->y), s1->z + (*t) * (s2->z - s1->z));
    
    return point_in_triangle (&inter, p1, p2, p3);
  }
  return 0;
}


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
int
intersection_triangle_triangle (
				point3d * t1,
				point3d * t2,
				point3d * t3,
				point3d * p1,
				point3d * p2,
				point3d * p3)
{
  double t;

  if (intersection_droite_triangle (t1, t2, p1, p2, p3, &t))
    {
      if (t >= 0 && t <= 1)
	return 1;
    }

  if (intersection_droite_triangle (t1, t3, p1, p2, p3, &t))
    {
      if (t >= 0 && t <= 1)
	return 1;
    }

  if (intersection_droite_triangle (t2, t3, p1, p2, p3, &t))
    {
      if (t >= 0 && t <= 1)
	return 1;
    }

  if (intersection_droite_triangle (p1, p2, t1, t2, t3, &t))
    {
      if (t >= 0 && t <= 1)
	return 1;
    }

  if (intersection_droite_triangle (p1, p3, t1, t2, t3, &t))
    {
      if (t >= 0 && t <= 1)
	return 1;
    }

  if (intersection_droite_triangle (p2, p3, t1, t2, t3, &t))
    {
      if (t >= 0 && t <= 1)
	return 1;
    }
  
  return 0;
}

/**
   Teste si les trois points A, B et C sont alignés
   @param A premier point
   @param B second point
   @param C troisieme point
   @return 1 si les trois points sont alignés, 0 sinon
*/
int
trois_points_alignes (
		      point3d * A,
		      point3d * B,
		      point3d * C)
{
  double t;

  if (point3d_equal (A, B))
    return 1;

  if (point3d_equal (A, C))
    return 1;

  if (point3d_equal (B, C))
    return 1;

  if (B->x - A->x != 0)
    t = (C->x - A->x) / (B->x - A->x);
  else
    {
      if (B->y - A->y != 0)
	t = (C->y - A->y) / (B->y - A->y);
      else
	t = (C->z - A->z) / (B->z - A->z);
    }

  if (!egalite (A->x + t * (B->x - A->x), C->x))
    return 0;

  if (!egalite (A->y + t * (B->y - A->y), C->y))
    return 0;

  if (!egalite (A->z + t * (B->z - A->z), C->z))
    return 0;

  return 1;
}
