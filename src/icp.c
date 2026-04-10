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
/* Date de modification : 06/06/09   */
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

#include "icp.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

typedef struct
{
  space_partition *sp;
  point3d *liste;
  point3d *liste_retour;
  int nb;
} ITicp_thread_argument;

void *
IFthread_process_icp_MEC (
			  void *argu)
{
  ITicp_thread_argument *arg = (ITicp_thread_argument *) argu;
  point3d p;
  double longueur;

  for (int i = 0; i < (int) (arg->nb); i++)
    {
      space_partition_nearest_point (arg->sp, &((arg->liste)[i]), &p, &longueur,
				     ACCEPT_ZERO_LENGTH);
      point3d_init (&(arg->liste_retour[i]), p.x, p.y, p.z);
      arg->liste_retour[i].att_int=p.att_int;
    }

  pthread_exit (0);
}

void
IFmise_en_correspondance (
			  point3d * data_p,
			  point3d ** data_x,
			  const vf_model * const X,
			  int nbpoint_p,
			  int *nbpoint_x)
{
  //pour chaque point de data_p, nous allons lui associer le point de data_x le plus proche
  point3d *listx;
  point3d p1,
    p2;
  space_partition sp;

  pthread_t th[A2RI_NB_THREAD];
  ITicp_thread_argument argument[A2RI_NB_THREAD];
  void *ret[A2RI_NB_THREAD];

  point3d_init (&p1, X->xmin, X->ymin, X->zmin);
  point3d_init (&p2, X->xmax, X->ymax, X->zmax);
  space_partition_new (&sp, &p1, &p2, ((int) pow (X->nbvertex / 24.0, 0.33))+1,
		       ((int) pow (X->nbvertex / 24.0, 0.33))+1,
		       ((int) pow (X->nbvertex / 24.0, 0.33))+1);
  

  a2ri_vf_space_partition (X, &sp);

  //listx sera la liste final qui remplacera data_x
  listx = (point3d *) malloc (nbpoint_p * sizeof (point3d));
  a2ri_erreur_critique_si (listx == NULL,
			   "erreur allocation memoire pour listx\nIFmise_en_correspondance");

  for (int i = 0; i < A2RI_NB_THREAD - 1; i++)
    {
      argument[i].sp = &sp;
      argument[i].nb = nbpoint_p / A2RI_NB_THREAD;
      argument[i].liste = data_p + i * argument[i].nb;
      argument[i].liste_retour = listx + i * argument[i].nb;
    }
  argument[A2RI_NB_THREAD - 1].sp = &sp;
  argument[A2RI_NB_THREAD - 1].nb =
    nbpoint_p - (A2RI_NB_THREAD - 1) * (argument[0].nb);
  argument[A2RI_NB_THREAD - 1].liste =
    data_p + (A2RI_NB_THREAD - 1) * (argument[0].nb);
  argument[A2RI_NB_THREAD - 1].liste_retour =
    listx + (A2RI_NB_THREAD - 1) * (argument[0].nb);

  for (int i = 0; i < A2RI_NB_THREAD; i++)
    {
      if (pthread_create (th + i, NULL, IFthread_process_icp_MEC, argument + i)
	  < 0)
	{
	  fprintf (stderr, "pthread_create error for thread 2\n");
	  exit (1);
	}
    }

  for (int i = 0; i < A2RI_NB_THREAD; i++)
    (void) pthread_join (th[i], &ret[i]);

  *nbpoint_x = nbpoint_p;
  free (*data_x);
  *data_x = listx;
  space_partition_free (&sp);
}

int
IFsommet_sur_bord(
		  vf_model *m,
		  int nbsommet,
		  hashtable *table)
{
  vf_edge *temp;
  
  for(int i=0;i<m->ve[nbsommet].nbincidentvertices;i++)
    {
      temp=hashtable_look_for(table,nbsommet,m->ve[nbsommet].incidentvertices[i]);
      if(temp->nbsharedfaces==1)
	return 1;
    }
  return 0;
}

void
IFmise_en_correspondance_pulli (
				vf_model * P,
				vf_model * X,
				point3d ** data_p,
				point3d ** data_x,
				int *nbpoint)
{
  //pour chaque point de P, nous allons lui associer le point de X le plus proche
  point3d p1,
    p2,
    *listx,*listP;
  space_partition sp;
  hashtable *tableP=a2ri_vf_construction_edge_table(P,NULL,0);
  hashtable *tableX=a2ri_vf_construction_edge_table(X,NULL,0);
  

  pthread_t th[A2RI_NB_THREAD];
  ITicp_thread_argument argument[A2RI_NB_THREAD];
  void *ret[A2RI_NB_THREAD];

  point3d_init (&p1, X->xmin, X->ymin, X->zmin);
  point3d_init (&p2, X->xmax, X->ymax, X->zmax);
  space_partition_new (&sp, &p1, &p2, ((int) pow (X->nbvertex / 24.0, 0.33))+1,
		       ((int) pow (X->nbvertex / 24.0, 0.33))+1,
		       ((int) pow (X->nbvertex / 24.0, 0.33))+1);


  a2ri_vf_space_partition (X, &sp);


  // listx est la correspondance dans X de chaque sommet de P
  listx = (point3d *) malloc (P->nbvertex * sizeof (point3d));
  a2ri_erreur_critique_si (listx == NULL,
			   "erreur allocation memoire pour listx\nIFmise_en_correspondance");

  listP=a2ri_vf_to_list_point3d(P);

  for (int i = 0; i < A2RI_NB_THREAD - 1; i++)
    {
      argument[i].sp = &sp;
      argument[i].nb = P->nbvertex / A2RI_NB_THREAD;
      argument[i].liste = listP + i * argument[i].nb;
      argument[i].liste_retour = listx + i * argument[i].nb;
    }
  argument[A2RI_NB_THREAD - 1].sp = &sp;
  argument[A2RI_NB_THREAD - 1].nb =
    P->nbvertex - (A2RI_NB_THREAD - 1) * (argument[0].nb);
  argument[A2RI_NB_THREAD - 1].liste =
    listP + (A2RI_NB_THREAD - 1) * (argument[0].nb);
  argument[A2RI_NB_THREAD - 1].liste_retour =
    listx + (A2RI_NB_THREAD - 1) * (argument[0].nb);

  for (int i = 0; i < A2RI_NB_THREAD; i++)
    {
      if (pthread_create (th + i, NULL, IFthread_process_icp_MEC, argument + i)
	  < 0)
	{
	  fprintf (stderr, "pthread_create error for thread 2\n");
	  exit (1);
	}
    }

  for (int i = 0; i < A2RI_NB_THREAD; i++)
    (void) pthread_join (th[i], &ret[i]);

  
  *nbpoint=0;
  
  for(int i=0;i<P->nbvertex;i++)
    {
      if(!IFsommet_sur_bord(P,i,tableP) && !IFsommet_sur_bord(X,listx[i].att_int,tableX))
  	{
  	  point3d pt;
  	  point3d_init(&pt,P->ve[i].x,P->ve[i].y,P->ve[i].z);
  	  //printf("%6d - (%9.4lf , %9.4lf , %9.4lf)\n",i,pt.x,pt.y,pt.z);
  	  list_point3d_add(data_p,nbpoint,&pt,WITH_REDUNDANCE);
  	  *nbpoint=*nbpoint-1;
  	  point3d_init(&pt,X->ve[listx[i].att_int].x,X->ve[listx[i].att_int].y,X->ve[listx[i].att_int].z);
  	  //printf("%6d - (%9.4lf , %9.4lf , %9.4lf)\n\n",listx[i].att_int,pt.x,pt.y,pt.z);
  	  list_point3d_add(data_x,nbpoint,&pt,WITH_REDUNDANCE);
  	}
    }

  space_partition_free (&sp);
  free(listP);
  free(listx);
  hashtable_free(tableX);
  hashtable_free(tableP);
  free(tableX);
  free(tableP);
}

double
IFcompute_error_nv(
		   point3d * data_pk,
		   point3d * data_x,
		   int nbpoint,
		   gsl_matrix *rotation,
		   gsl_matrix *translation)
{
  double retour=0.0;
  double data[3];
  gsl_matrix *pos1,
    *pos2;
  
  //rotation
  for (int i = 0; i < nbpoint; i++)
    {
      data[0] = data_pk[i].x;
      data[1] = data_pk[i].y;
      data[2] = data_pk[i].z;
      pos1 = matrix_init (data, 1, 3);
      pos2 = matrix_mul (pos1, rotation);

      data_pk[i].x = gsl_matrix_get (pos2, 0, 0);
      data_pk[i].y = gsl_matrix_get (pos2, 0, 1);
      data_pk[i].z = gsl_matrix_get (pos2, 0, 2);
      gsl_matrix_free (pos1);
      gsl_matrix_free (pos2);
    }

  //translation
  for (int i = 0; i < nbpoint; i++)
    {
      data[0] = data_pk[i].x;
      data[1] = data_pk[i].y;
      data[2] = data_pk[i].z;
      pos1 = matrix_init (data, 1, 3);
      pos2 = matrix_add (pos1, translation);

      data_pk[i].x = gsl_matrix_get (pos2, 0, 0);
      data_pk[i].y = gsl_matrix_get (pos2, 0, 1);
      data_pk[i].z = gsl_matrix_get (pos2, 0, 2);
      gsl_matrix_free (pos1);
      gsl_matrix_free (pos2);
    }

  for(int i=0;i<nbpoint;i++)
    retour+=point3d_square_length(&(data_pk[i]),&(data_x[i]));

  return retour;
}

int IFrejet_etape_tri_rapide (
			      double *longueur,
			      int *index,
			      int min,
			      int max,
			      int type)
{
  double temp = longueur[max];
  int temp_index = index[max];
  
  while (max > min)
    {
      if (type == ASC)
	{
	  while (max > min && longueur[min] <= temp)
	    min++;
	}
      else
	{
	  while (max > min && longueur[min] > temp)
	    min++;
	}
      if (max > min)
	{
	  longueur[max] = longueur[min];
	  index[max] = index[min];
	  max--;
	  if (type == ASC)
	    {
	      while (max > min && longueur[max] >= temp)
		max--;
	    }
	  else
	    {
	      while (max > min && longueur[max] < temp)
		max--;
	    }
	  if (max > min)
	    {
	      longueur[min] = longueur[max];
	      index[min] = index[max];
	      min++;
	    }
	}
    }
  longueur[max] = temp;
  index[max] = temp_index;
  return (max);
}

void IFrejet_tri_rapide (
			 double *longueur,
			 int *index,
			 int deb,
			 int fin,
			 int type)
{
  int mil;
  
  if (deb < fin)
    {
      mil = IFrejet_etape_tri_rapide (longueur, index, deb, fin, type);
      if (mil - deb > fin - mil)
	{
	  IFrejet_tri_rapide (longueur, index, mil + 1, fin, type);
	  IFrejet_tri_rapide (longueur, index, deb, mil - 1, type);
	}
      else
	{
	  IFrejet_tri_rapide (longueur, index, deb, mil - 1, type);
	  IFrejet_tri_rapide (longueur, index, mil + 1, fin, type);
	}
    }
}

void
IFrejet (
	 point3d ** data_pk,
	 point3d ** data_x,
	 point3d ** data_p,
	 int *nbpoint,
	 double recouvrement)
{
  int *index = (int *) malloc ((*nbpoint) * sizeof (int));
  a2ri_erreur_critique_si (index == NULL,
			   "erreur allocation memoire pour index\nIFrejet");
  double *listelongueur = (double *) malloc ((*nbpoint) * sizeof (double));
  a2ri_erreur_critique_si (listelongueur == NULL,
			   "erreur allocation memoire pour listelongueur\nIFrejet");
  point3d *new_data_pk,
    *new_data_x,
    *new_data_p;
  int agarder;

  //calcul du nombre de point à garder en fonction du taux de recouvrement
  agarder = (*nbpoint) * recouvrement;

  point3d p1,
    p2;

  for (int i = 0; i < (*nbpoint); i++)
    {
      point3d_init (&p1, (*data_pk)[i].x, (*data_pk)[i].y, (*data_pk)[i].z);
      point3d_init (&p2, (*data_x)[i].x, (*data_x)[i].y, (*data_x)[i].z);
      listelongueur[i] = point3d_length (&p1, &p2);
      index[i] = i;
    }
  IFrejet_tri_rapide (listelongueur, index, 0, (*nbpoint) - 1, ASC);


  new_data_pk = (point3d *) malloc (agarder * sizeof (point3d));
  a2ri_erreur_critique_si (new_data_pk == NULL,
			   "erreur allocation memoire pour new_data_pk\nIFrejet");
  new_data_x = (point3d *) malloc (agarder * sizeof (point3d));
  a2ri_erreur_critique_si (new_data_x == NULL,
			   "erreur allocation memoire pour new_data_x\nIFrejet");
  new_data_p = (point3d *) malloc (agarder * sizeof (point3d));
  a2ri_erreur_critique_si (new_data_p == NULL,
			   "erreur allocation memoire pour new_data_p\nIFrejet");

  //mise à jour des listes de points
  for (int i = 0; i < agarder; i++)
    {
      new_data_pk[i].x = (*data_pk)[index[i]].x;
      new_data_pk[i].y = (*data_pk)[index[i]].y;
      new_data_pk[i].z = (*data_pk)[index[i]].z;

      new_data_x[i].x = (*data_x)[index[i]].x;
      new_data_x[i].y = (*data_x)[index[i]].y;
      new_data_x[i].z = (*data_x)[index[i]].z;

      new_data_p[i].x = (*data_p)[index[i]].x;
      new_data_p[i].y = (*data_p)[index[i]].y;
      new_data_p[i].z = (*data_p)[index[i]].z;

    }

  free (index);
  free (listelongueur);
  free (*data_pk);
  free (*data_x);
  free (*data_p);
  *data_pk = new_data_pk;
  *data_x = new_data_x;
  *data_p = new_data_p;
  *nbpoint = agarder;
}

gsl_matrix *
IFcompute_rotation (
		    point3d * data_p,
		    point3d * data_x,
		    int nbpoint)
{
  //voir article de Besl p243
  gsl_matrix *Exp,
    *A,
    *Exptrans,
    *delta,
    *QExp,
    *identity3,
    *trExpidentity3,
    *basdroite,
    *rotation;
  double data[3],
    trExp,
    q0,
    q1,
    q2,
    q3;

  gsl_vector *eval = gsl_vector_alloc (4);
  a2ri_erreur_critique_si (eval == NULL,
			   "erreur allocation memoire pour eval\nIFcompute_rotation");
  gsl_matrix *evec = gsl_matrix_alloc (4, 4);
  a2ri_erreur_critique_si (evec == NULL,
			   "erreur allocation memoire pour evec\nIFcompute_rotation");
  gsl_eigen_symmv_workspace *w = gsl_eigen_symmv_alloc (4);
  a2ri_erreur_critique_si (w == NULL,
			   "erreur allocation memoire pour w\nIFcompute_rotation");

  identity3 = gsl_matrix_alloc (3, 3);
  a2ri_erreur_critique_si (identity3 == NULL,
			   "erreur allocation memoire pour identity3\nIFcompute_rotation");
  gsl_matrix_set_identity (identity3);

  Exp = cross_variance (data_p, data_x, nbpoint);
  Exptrans = matrix_transpose (Exp);
  A = matrix_sub (Exp, Exptrans);

  data[0] = gsl_matrix_get (A, 2, 1);
  data[1] = gsl_matrix_get (A, 0, 2);
  data[2] = gsl_matrix_get (A, 1, 0);

  delta = matrix_init (data, 3, 1);

  //trExp signifie trace de la matrice Exp
  trExp =
    gsl_matrix_get (Exp, 0, 0) + gsl_matrix_get (Exp, 1,
						 1) + gsl_matrix_get (Exp, 2,
								      2);

  trExpidentity3 = matrix_mul_scale (identity3, trExp);

  basdroite = matrix_sub (matrix_add (Exp, Exptrans), trExpidentity3);

  QExp = gsl_matrix_alloc (4, 4);
  a2ri_erreur_critique_si (QExp == NULL,
			   "erreur allocation memoire pour QExp\nIFcompute_rotation");
  gsl_matrix_set (QExp, 0, 0, trExp);
  gsl_matrix_set (QExp, 0, 1, gsl_matrix_get (delta, 0, 0));
  gsl_matrix_set (QExp, 0, 2, gsl_matrix_get (delta, 1, 0));
  gsl_matrix_set (QExp, 0, 3, gsl_matrix_get (delta, 2, 0));
  gsl_matrix_set (QExp, 1, 0, gsl_matrix_get (delta, 0, 0));
  gsl_matrix_set (QExp, 1, 1, gsl_matrix_get (basdroite, 0, 0));
  gsl_matrix_set (QExp, 1, 2, gsl_matrix_get (basdroite, 0, 1));
  gsl_matrix_set (QExp, 1, 3, gsl_matrix_get (basdroite, 0, 2));
  gsl_matrix_set (QExp, 2, 0, gsl_matrix_get (delta, 1, 0));
  gsl_matrix_set (QExp, 2, 1, gsl_matrix_get (basdroite, 1, 0));
  gsl_matrix_set (QExp, 2, 2, gsl_matrix_get (basdroite, 1, 1));
  gsl_matrix_set (QExp, 2, 3, gsl_matrix_get (basdroite, 1, 2));
  gsl_matrix_set (QExp, 3, 0, gsl_matrix_get (delta, 2, 0));
  gsl_matrix_set (QExp, 3, 1, gsl_matrix_get (basdroite, 2, 0));
  gsl_matrix_set (QExp, 3, 2, gsl_matrix_get (basdroite, 2, 1));
  gsl_matrix_set (QExp, 3, 3, gsl_matrix_get (basdroite, 2, 2));

  gsl_eigen_symmv (QExp, eval, evec, w);
  gsl_eigen_symmv_free (w);

  //extraction des valeurs maximales propres de la matrics QExp
  int index = 0;
  if (gsl_vector_get (eval, 1) > gsl_vector_get (eval, index))
    index = 1;
  if (gsl_vector_get (eval, 2) > gsl_vector_get (eval, index))
    index = 2;
  if (gsl_vector_get (eval, 3) > gsl_vector_get (eval, index))
    index = 3;

  //on ne garde que le vecteur prope associé à la valeur propre la plus élevée
  gsl_vector_view evec_i = gsl_matrix_column (evec, index);
  gsl_vector_free (eval);
  eval = &evec_i.vector;

  //rotation exprimé sous forme de quaternion
  q0 = gsl_vector_get (eval, 0);
  q1 = gsl_vector_get (eval, 1);
  q2 = gsl_vector_get (eval, 2);
  q3 = gsl_vector_get (eval, 3);

  rotation = gsl_matrix_alloc (3, 3);
  a2ri_erreur_critique_si (rotation == NULL,
			   "erreur allocation memoire pour rotation\nIFcompute_rotation");

  //transformation du quaternion en matrice 3x3
  gsl_matrix_set (rotation, 0, 0, q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3);
  gsl_matrix_set (rotation, 0, 1, 2 * (q1 * q2 - q0 * q3));
  gsl_matrix_set (rotation, 0, 2, 2 * (q1 * q3 + q0 * q2));
  gsl_matrix_set (rotation, 1, 0, 2 * (q1 * q2 + q0 * q3));
  gsl_matrix_set (rotation, 1, 1, q0 * q0 + q2 * q2 - q1 * q1 - q3 * q3);
  gsl_matrix_set (rotation, 1, 2, 2 * (q2 * q3 - q0 * q1));
  gsl_matrix_set (rotation, 2, 0, 2 * (q1 * q3 - q0 * q2));
  gsl_matrix_set (rotation, 2, 1, 2 * (q2 * q3 + q0 * q1));
  gsl_matrix_set (rotation, 2, 2, q0 * q0 + q3 * q3 - q1 * q1 - q2 * q2);

  gsl_matrix_free (Exp);
  gsl_matrix_free (A);
  gsl_matrix_free (Exptrans);
  gsl_matrix_free (delta);
  gsl_matrix_free (QExp);
  gsl_matrix_free (identity3);
  gsl_matrix_free (trExpidentity3);
  gsl_matrix_free (basdroite);
  gsl_matrix_free (evec);

  return rotation;
}

gsl_matrix *
IFcompute_translation (
		       point3d * data_p,
		       point3d * data_x,
		       int nbpoint,
		       gsl_matrix * rotation)
{
  //voir article de Besl p243
  gsl_matrix *up,
    *ux,
    *mul,
    *trans;
  point3d *centerP,
    *centerX;
  double d[3];

  centerP = center_of_mass (data_p, nbpoint);
  centerX = center_of_mass (data_x, nbpoint);

  d[0] = centerP->x;
  d[1] = centerP->y;
  d[2] = centerP->z;
  up = matrix_init (d, 1, 3);



  d[0] = centerX->x;
  d[1] = centerX->y;
  d[2] = centerX->z;
  ux = matrix_init (d, 1, 3);


  mul = matrix_mul (up, rotation);

  trans = matrix_sub (ux, mul);

  gsl_matrix_free (up);
  gsl_matrix_free (ux);
  gsl_matrix_free (mul);
  free (centerP);
  free (centerX);

  return trans;
}

void
IFapply_rotation_nv (
		     vf_model *P,
		     gsl_matrix * rotation)
{
  gsl_matrix *pos1,
    *pos2;
  double data[3];

  for (int i = 0; i < P->nbvertex; i++)
    {
      data[0] = P->ve[i].x;
      data[1] = P->ve[i].y;
      data[2] = P->ve[i].z;
      pos1 = matrix_init (data, 1, 3);
      pos2 = matrix_mul (pos1, rotation);

      P->ve[i].x = gsl_matrix_get (pos2, 0, 0);
      P->ve[i].y = gsl_matrix_get (pos2, 0, 1);
      P->ve[i].z = gsl_matrix_get (pos2, 0, 2);
      gsl_matrix_free (pos1);
      gsl_matrix_free (pos2);
    }
}

void
IFapply_translation_nv (
			vf_model *P,
			gsl_matrix * translation)
{
  gsl_matrix *pos1,
    *pos2;
  double data[3];
  for (int i = 0; i < P->nbvertex; i++)
    {
      data[0] = P->ve[i].x;
      data[1] = P->ve[i].y;
      data[2] = P->ve[i].z;
      pos1 = matrix_init (data, 1, 3);
      pos2 = matrix_add (pos1, translation);

      P->ve[i].x = gsl_matrix_get (pos2, 0, 0);
      P->ve[i].y = gsl_matrix_get (pos2, 0, 1);
      P->ve[i].z = gsl_matrix_get (pos2, 0, 2);
      gsl_matrix_free (pos1);
      gsl_matrix_free (pos2);
    }
}

void
IFapply_transformation_nv (
			   vf_model * P,
			   gsl_matrix * rotation,
			   gsl_matrix * translation)
{
  //application de la rotation puis translation et calcule de la nouvelle erreur
  IFapply_rotation_nv (P, rotation);
  IFapply_translation_nv (P, translation);

  P->xmin=P->ve[0].x;
  P->xmax=P->ve[0].x;
  P->ymin=P->ve[0].y;
  P->ymax=P->ve[0].y;
  P->zmin=P->ve[0].z;
  P->zmax=P->ve[0].z;
  for(int i=1;i<P->nbvertex;i++)
    {
      if (P->ve[i].x < P->xmin)
	P->xmin = P->ve[i].x;
      if (P->ve[i].x > P->xmax)
	P->xmax = P->ve[i].x;
      if (P->ve[i].y < P->ymin)
	P->ymin = P->ve[i].y;
      if (P->ve[i].y > P->ymax)
	P->ymax = P->ve[i].y;
      if (P->ve[i].z < P->zmin)
	P->zmin = P->ve[i].z;
      if (P->ve[i].z > P->zmax)
	P->zmax = P->ve[i].z;
    }
}

void
IFapply_transformation_list_nv (
				vf_model ** P,
				int sizeP,
				gsl_matrix * rotation,
				gsl_matrix * translation)
{
  for(int i=0;i<sizeP;i++)
    IFapply_transformation_nv(P[i],rotation,translation);
}

/********** MAIN FUNCTIONS **********/

/**
   Recalage de deux modeles
   @param P modele à recaler
   @param X premier modele servant de base
   @param dkarret critère d'arret.
   @return aucun
**/
void
a2ri_vf_icp (
	     vf_model * P,
	     const vf_model * const X,
	     double dkarret)
{
  //voir Besl p243 pour l'alogrithme général et
  //voir chetverikov pour le rejet
  gsl_matrix *rotation,
    *translation;
  point3d *data_p = NULL,
    *data_x = NULL,
    *data_pk = NULL;
  double erreur,
    erreurprec,
    delta=0;
  int nbpoint_p,
    nbpoint_pk,
    nbpoint_x,
    k = 0;
  double dk = 2.0,
    dk_1 = 1.0;

  delta = delta+2;
  point3d p1,
    p2;
  point3d_init (&p1, P->xmin, P->ymin, P->zmin);
  point3d_init (&p2, P->xmax, P->ymax, P->zmax);

  //critère d'arret : dk supérieur à notre critère d'arret,
  //nombre d'itération atteignant un certain seuil,
  //soustraction de deux dk successifs trop faible


  while ((dk_1 > dk && dk > dkarret && k < 200
	  && ((fabs (dk - dk_1)) > (dkarret / 1000.0))) || k < 2)
    {
      erreurprec = erreur;
      dk_1 = dk;

      data_pk=a2ri_vf_to_list_point3d(P);
      nbpoint_pk = P->nbvertex;

      data_x=a2ri_vf_to_list_point3d(X);
      nbpoint_x = X->nbvertex;
      data_p=a2ri_vf_to_list_point3d(P);
      nbpoint_p = P->nbvertex;

      //Besl : étape a) -> mise en correspondance
      IFmise_en_correspondance (data_pk, &data_x, X, nbpoint_pk, &nbpoint_x);
      //chetverikov : on ne garde qu'un certain pourcentage de couple
      nbpoint_pk = nbpoint_p;
      nbpoint_x = nbpoint_p;
      //Besl : étape b) -> calcul de la transformation rigide

      rotation = IFcompute_rotation (data_pk, data_x, nbpoint_p);
      translation =
	IFcompute_translation (data_pk, data_x, nbpoint_p, rotation);

      erreur=IFcompute_error_nv(data_pk, data_x, nbpoint_p, rotation, translation);
      
      free (data_x);
      data_x = NULL;
      free (data_pk);
      data_pk = NULL;
      free (data_p);
      data_p = NULL;
      data_p=a2ri_vf_to_list_point3d(P);
      nbpoint_p = P->nbvertex;
      nbpoint_pk = nbpoint_p;
      //Besl : étape c) -> application de la transformation rigide

      IFapply_transformation_nv (P, rotation, translation);

      gsl_matrix_free (rotation);
      gsl_matrix_free (translation);

      k++;
      if (k > 1)
	{
	  //Besl : étape d) -> fin de l'itération, calcul de l'erreur et réitération si nécessaire (voir le WHILE)
	  delta = erreurprec / erreur;
	  dk = (erreur * 100) / (point3d_length (&p1, &p2));
	}
      else
	{
	  dk = (erreur * 100) / (point3d_length (&p1, &p2));
	  dk_1 = dk + 1;
	}
    }

  if (data_p != NULL)
    free (data_p);
  if (data_x != NULL)
    free (data_x);
  if (data_pk != NULL)
    free (data_pk);
}

/**
   Recalage de deux modeles
   @param P modele à recaler
   @param X premier modele servant de base
   @param recouvrement estimation du taux de recouvrement
   @param dkarret critère d'arret.
   @return aucun
**/
void
a2ri_vf_trimmed_icp (
		     vf_model * P,
		     const vf_model * const X,
		     double recouvrement,
		     double dkarret)
{
  //voir Besl p243 pour l'alogrithme général et
  //voir chetverikov pour le rejet
  gsl_matrix *rotation,
    *translation;
  point3d *data_p = NULL,
    *data_x = NULL,
    *data_pk = NULL;
  double erreur,
    erreurprec,
    delta=0;
  int nbpoint_p,
    nbpoint_pk,
    nbpoint_x,
    k = 0;
  double dk = 2.0,
    dk_1 = 1.0;

  delta = delta+2;
  point3d p1,
    p2;
  point3d_init (&p1, P->xmin, P->ymin, P->zmin);
  point3d_init (&p2, P->xmax, P->ymax, P->zmax);


  //critère d'arret : dk supérieur à notre critère d'arret,
  //nombre d'itération atteignant un certain seuil,
  //soustraction de deux dk successifs trop faible


  while ((dk_1 > dk && dk > dkarret && k < 200
	  && ((fabs (dk - dk_1)) > (dkarret / 1000.0))) || k < 2)
    {
      erreurprec = erreur;
      dk_1 = dk;

      data_pk=a2ri_vf_to_list_point3d(P);
      nbpoint_pk = P->nbvertex;

      data_x=a2ri_vf_to_list_point3d(X);
      nbpoint_x = X->nbvertex;
      data_p=a2ri_vf_to_list_point3d(P);
      nbpoint_p = P->nbvertex;

      //Besl : étape a) -> mise en correspondance
      IFmise_en_correspondance (data_pk, &data_x, X, nbpoint_pk, &nbpoint_x);
      //chetverikov : on ne garde qu'un certain pourcentage de couple
      IFrejet (&data_pk, &data_x, &data_p, &nbpoint_p, recouvrement);
      nbpoint_pk = nbpoint_p;
      nbpoint_x = nbpoint_p;
      //Besl : étape b) -> calcul de la transformation rigide

      rotation = IFcompute_rotation (data_pk, data_x, nbpoint_p);
      translation =
	IFcompute_translation (data_pk, data_x, nbpoint_p, rotation);

      erreur=IFcompute_error_nv(data_pk, data_x, nbpoint_p, rotation, translation);

      free (data_x);
      data_x = NULL;
      free (data_pk);
      data_pk = NULL;
      free (data_p);
      data_p = NULL;
      data_p=a2ri_vf_to_list_point3d(P);
      nbpoint_p = P->nbvertex;
      nbpoint_pk = nbpoint_p;
      //Besl : étape c) -> application de la transformation rigide

      IFapply_transformation_nv (P, rotation, translation);

      gsl_matrix_free (rotation);
      gsl_matrix_free (translation);

      k++;
      if (k > 1)
	{
	  //Besl : étape d) -> fin de l'itération, calcul de l'erreur et réitération si nécessaire (voir le WHILE)
	  delta = erreurprec / erreur;
	  dk =
	    ((erreur * 100) / (P->nbvertex * recouvrement)) /
	    (point3d_length (&p1, &p2));
	}
      else
	{
	  dk =
	    ((erreur * 100) / (P->nbvertex * recouvrement)) /
	    (point3d_length (&p1, &p2));
	  dk_1 = dk + 1;
	}
    }

  if (data_p != NULL)
    free (data_p);
  if (data_x != NULL)
    free (data_x);
  if (data_pk != NULL)
    free (data_pk);
}


/**
   Recalage automatique de deux modeles
   @param P modele à recaler
   @param X premier modele servant de base
   @param dkarret critère d'arret.
   @param sensibility sensibilité du taux de recouvrement
   @return aucun
**/
//REMARQUE : le maillage devrait etre qualifié const mais impossible a cause de l'utilisation de la focntion a2ri_vf_bounding_ball_minimale_compute_overlap qui utilise des threads et ne permet donc pas la qualifidation de const du maillage.
void
a2ri_vf_automated_trimmed_icp (
			       vf_model * P,
			       vf_model * X,
			       double dkarret,
			       double sensibility)
{
  //voir Besl p243 pour l'alogrithme général et
  //voir chetverikov pour le rejet
  gsl_matrix *rotation,
    *translation;
  point3d *data_p = NULL,
    *data_x = NULL,
    *data_pk = NULL;
  double erreur,
    erreurprec,
    delta=0;
  int nbpoint_p,
    nbpoint_pk,
    nbpoint_x,
    k = 0;
  double dk = 2.0,
    dk_1 = 1.0,
    recouvrement = 0;

  delta = delta+2;
  point3d p1,
    p2;
  point3d_init (&p1, P->xmin, P->ymin, P->zmin);
  point3d_init (&p2, P->xmax, P->ymax, P->zmax);

  //critère d'arret : dk supérieur à notre critère d'arret,
  //nombre d'itération atteignant un certain seuil,
  //soustraction de deux dk successifs trop faible

  while (((dk_1 > dk || (k - 1) % 10 == 0) && dk > dkarret
	  && ((fabs (dk - dk_1)) > (dkarret / 1000.0))) || k < 2)
    {
      if (k % 10 == 0)
	{
	  recouvrement =
	    a2ri_vf_bounding_ball_minimale_compute_overlap (X, P,
							    sensibility);
	}

      erreurprec = erreur;
      dk_1 = dk;

      data_pk=a2ri_vf_to_list_point3d(P);
      nbpoint_pk = P->nbvertex;

      data_x=a2ri_vf_to_list_point3d(X);
      nbpoint_x = X->nbvertex;
      data_p=a2ri_vf_to_list_point3d(P);
      nbpoint_p = P->nbvertex;

      //Besl : étape a) -> mise en correspondance
      IFmise_en_correspondance (data_pk, &data_x, X, nbpoint_pk, &nbpoint_x);
      //chetverikov : on ne garde qu'un certain pourcentage de couple
      IFrejet (&data_pk, &data_x, &data_p, &nbpoint_p, recouvrement);
      nbpoint_pk = nbpoint_p;
      nbpoint_x = nbpoint_p;
      //Besl : étape b) -> calcul de la transformation rigide
      rotation = IFcompute_rotation (data_pk, data_x, nbpoint_p);
      translation =
	IFcompute_translation (data_pk, data_x, nbpoint_p, rotation);
      
      erreur=IFcompute_error_nv(data_pk, data_x, nbpoint_p, rotation, translation);

      free (data_pk);
      data_pk = NULL;
      free (data_x);
      data_x = NULL;
      free (data_p);
      data_p = NULL;
      data_p=a2ri_vf_to_list_point3d(P);
      nbpoint_p = P->nbvertex;
      nbpoint_pk = nbpoint_p;
      //Besl : étape c) -> application de la transformation rigide
      
      IFapply_transformation_nv (P, rotation, translation);

      gsl_matrix_free (rotation);
      gsl_matrix_free (translation);

      k++;
      if (k > 1)
	{
	  //Besl : étape d) -> fin de l'itération, calcul de l'erreur et réitération si nécessaire (voir le WHILE)
	  delta = erreurprec / erreur;
	  dk =
	    ((erreur * 100) / (P->nbvertex * recouvrement)) /
	    (point3d_length (&p1, &p2));
	}
      else
	{
	  dk =
	    ((erreur * 100) / (P->nbvertex * recouvrement)) /
	    (point3d_length (&p1, &p2));
	  dk_1 = dk + 1;
	}
    }

  if (data_p != NULL)
    free (data_p);
  if (data_x != NULL)
    free (data_x);
  if (data_pk != NULL)
    free (data_pk);
}

/**
   Recalage automatique de deux modeles
   @param P modele à recaler
   @param X premier modele servant de base
   @param dkarret critère d'arret.
   @return aucun
**/
//REMARQUE : le maillage devrait etre qualifié const mais impossible a cause de l'utilisation de thread.
void
a2ri_vf_automated_trimmed_icp_pulli (
				     vf_model * P,
				     vf_model * X,
				     double dkarret)
{
  //voir Besl p243 pour l'alogrithme général et
  //voir chetverikov pour le rejet
  gsl_matrix *rotation,
    *translation;
  point3d *data_x = NULL,
    *data_p = NULL;
  double erreur,
    erreurprec,
    delta=0;
  int nbpoint,
    k = 0;
  double dk = 2.0,
    dk_1 = 1.0;

  delta = delta+2;
  point3d p1,
    p2;
  point3d_init (&p1, P->xmin, P->ymin, P->zmin);
  point3d_init (&p2, P->xmax, P->ymax, P->zmax);

  //critère d'arret : dk supérieur à notre critère d'arret,
  //nombre d'itération atteignant un certain seuil,
  //soustraction de deux dk successifs trop faible

  while (((dk_1 > dk || (k - 1) % 10 == 0) && dk > dkarret
	  && ((fabs (dk - dk_1)) > (dkarret / 1000.0))) || k < 2)
    {
      erreurprec = erreur;
      dk_1 = dk;

      free(data_p);
      data_p=NULL;
      free(data_x);
      data_x=NULL;

      //Besl : étape a) -> mise en correspondance
      IFmise_en_correspondance_pulli (P,X,&data_p,&data_x,&nbpoint);
      //Besl : étape b) -> calcul de la transformation rigide
      rotation = IFcompute_rotation (data_p, data_x, nbpoint);
      translation =
	IFcompute_translation (data_p, data_x, nbpoint, rotation);
      
      erreur=IFcompute_error_nv(data_p, data_x, nbpoint, rotation, translation);

      //Besl : étape c) -> application de la transformation rigide
      
      IFapply_transformation_nv (P, rotation, translation);

      gsl_matrix_free (rotation);
      gsl_matrix_free (translation);

      k++;
      if (k > 1)
	{
	  //Besl : étape d) -> fin de l'itération, calcul de l'erreur et réitération si nécessaire (voir le WHILE)
	  delta = erreurprec / erreur;
	  dk =
	    ((erreur * 100) / (nbpoint)) /
	    (point3d_length (&p1, &p2));
	}
      else
	{
	  dk =
	    ((erreur * 100) / (nbpoint)) /
	    (point3d_length (&p1, &p2));
	  dk_1 = dk + 1;
	}
    }

  if (data_x != NULL)
    free (data_x);
  if (data_p != NULL)
    free (data_p);  
}


/**
   Recalage automatique de deux modeles
   @param P tableau de modeles à recaler
   @param X tableau de modeles servant de référence
   @param sizeP nombre de modele dans P
   @param sizeX nombre de modele dans X
   @param dkarret critère d'arret.
   @param sensibility sensibilité du taux de recouvrement
   @return aucun
**/
//REMARQUE : le maillage devrait etre qualifié const mais impossible a cause de l'utilisation de la focntion a2ri_vf_bounding_ball_minimale_compute_overlap qui utilise des threads et ne permet donc pas la qualifidation de const du maillage.
void
a2ri_vf_automated_trimmed_icp_multiple (
					vf_model ** P,
					vf_model ** X,
					int sizeP,
					int sizeX,
					double dkarret,
					double sensibility)
{
  //voir Besl p243 pour l'alogrithme général et
  //voir chetverikov pour le rejet
  gsl_matrix *rotation,
    *translation;
  point3d *data_p = NULL,
    *data_x = NULL,
    *data_pk = NULL;
  double erreur,
    erreurprec,
    delta=0;
  int nbpoint_p,
    nbpoint_pk,
    nbpoint_x,
    k = 0;
  double dk = 2.0,
    dk_1 = 1.0,
    recouvrement = 0;
  vf_model *tempX = NULL,
    *tempP = NULL;

  tempX = a2ri_vf_concat (X, sizeX);

  delta = delta+2;
  point3d p1,
    p2;
  point3d_init (&p1, tempP->xmin, tempP->ymin, tempP->zmin);
  point3d_init (&p2, tempP->xmax, tempP->ymax, tempP->zmax);

  //critère d'arret : dk supérieur à notre critère d'arret,
  //nombre d'itération atteignant un certain seuil,
  //soustraction de deux dk successifs trop faible

  while (((dk_1 > dk || (k - 1) % 10 == 0) && ((fabs (dk - dk_1)) > dkarret))
	 || k < 2)
    {
      if (k % 10 == 0)
	recouvrement =
	  a2ri_vf_bounding_ball_minimale_compute_overlap (tempX, tempP,
							  sensibility);

      erreurprec = erreur;
      dk_1 = dk;

      a2ri_vf_free(tempP);
      tempP=NULL;
      tempP = a2ri_vf_concat (P, sizeP);

      data_pk=a2ri_vf_to_list_point3d(tempP);
      nbpoint_pk = tempP->nbvertex;

      data_x=a2ri_vf_to_list_point3d(tempX);
      nbpoint_x = tempX->nbvertex;
      data_p=a2ri_vf_to_list_point3d(tempP);
      nbpoint_p = tempP->nbvertex;

      //Besl : étape a) -> mise en correspondance
      IFmise_en_correspondance (data_pk, &data_x, tempX, nbpoint_pk,
			      &nbpoint_x);
      //chetverikov : on ne garde qu'un certain pourcentage de couple
      IFrejet (&data_pk, &data_x, &data_p, &nbpoint_p, recouvrement);
      nbpoint_pk = nbpoint_p;
      nbpoint_x = nbpoint_p;
      //Besl : étape b) -> calcul de la transformation rigide

      rotation = IFcompute_rotation (data_pk, data_x, nbpoint_p);
      translation =
	IFcompute_translation (data_pk, data_x, nbpoint_p, rotation);

      erreur=IFcompute_error_nv(data_pk, data_x, nbpoint_p, rotation, translation);

      free (data_pk);
      data_pk = NULL;
      free (data_x);
      data_x = NULL;
      free (data_p);
      data_p = NULL;
      data_p=a2ri_vf_to_list_point3d(tempP);
      nbpoint_p = tempP->nbvertex;
      nbpoint_pk = nbpoint_p;
      //Besl : étape c) -> application de la transformation rigide

      IFapply_transformation_list_nv (P, sizeP, rotation, translation);

      gsl_matrix_free (rotation);
      gsl_matrix_free (translation);

      k++;
      if (k > 1)
	{
	  //Besl : étape d) -> fin de l'itération, calcul de l'erreur et réitération si nécessaire (voir le WHILE)
	  delta = erreurprec / erreur;
	  dk =
	    ((erreur * 100) / (tempP->nbvertex * recouvrement)) /
	    (point3d_length (&p1, &p2));
	}
      else
	{
	  dk =
	    ((erreur * 100) / (tempP->nbvertex * recouvrement)) /
	    (point3d_length (&p1, &p2));
	  dk_1 = dk + 1;
	}
    }

  if (data_p != NULL)
    free (data_p);
  if (data_x != NULL)
    free (data_x);
  if (data_pk != NULL)
    free (data_pk);

  a2ri_vf_free (tempP);
  a2ri_vf_free (tempX);
}

