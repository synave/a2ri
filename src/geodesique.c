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

#include "geodesique.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

void
IF_plan_lms_contraint_moyenne (
			    point3d A,
			    point3d B,
			    point3d * listept,
			    int size,
			    point3d * C)
{
  // calcul des trois points du plan moyen
  // le premier et le deuxieme sont connus : points de depart et d'arrivée
  point3d_init (C, 0, 0, 0);
  A.x = A.x;
  B.x = B.x;

  for (int i = 0; i < size; i++)
    {
      C->x += listept[i].x;
      C->y += listept[i].y;
      C->z += listept[i].z;
    }

  C->x /= (size * 1.0);
  C->y /= (size * 1.0);
  C->z /= (size * 1.0);
}

void
IF_plan_lms_contraint_v1 (
		       point3d A,
		       point3d B,
		       point3d * listept,
		       int size,
		       point3d * C)
{
  // calcul des trois points du plan moyen
  // le premier et le deuxieme sont connus : points de depart et d'arrivée
  point3d_init (C, 0, 0, 0);

  point3d p1,
    p2;
  point3d_init (&p1, listept[0].x, listept[0].y, listept[0].z);
  point3d_init (&p2, listept[1].x, listept[1].y, listept[1].z);
  double min = distance_point_plane (&p2, &A, &B, &p1),
    max = distance_point_plane (&p2, &A, &B, &p1);
  int indexmin = 0,
    indexmax = 0;

  for (int i = 0; i < size; i++)
    {
      //à chaque tour de boucle, on selectionne un point dans la liste des points comme troisieme point du plan
      //on regarde les distances mini et maxi
      point3d_init (&p1, listept[i].x, listept[i].y, listept[i].z);
      for (int j = 0; j < size; j++)
	{
	  if (i != j)
	    {
	      point3d_init (&p2, listept[j].x, listept[j].y, listept[j].z);
	      if (min > distance_point_plane (&p2, &A, &B, &p1))
		{
		  min = distance_point_plane (&p2, &A, &B, &p1);
		  indexmin = j;
		}
	      if (max < distance_point_plane (&p2, &A, &B, &p1))
		{
		  max = distance_point_plane (&p2, &A, &B, &p1);
		  indexmax = j;
		}

	    }
	}
    }

  point3d_init (C,
		(listept[indexmin].x + listept[indexmax].x) / 2.0,
		(listept[indexmin].y + listept[indexmax].y) / 2.0,
		(listept[indexmin].z + listept[indexmax].z) / 2.0);
}


void
IF_plan_lms_contraint_min_max (
			    point3d A,
			    point3d B,
			    point3d * listept,
			    int size,
			    point3d * C)
{
  double distancedroite = 0,
    distancegauche = 0;
  double a,
    b,
    c,
    d,
    calcul;
  point3d tableau[3];
  int indexdroite = -1,
    indexgauche = -1;

  point3d_init (&(tableau[0]), A.x, A.y, A.z);
  point3d_init (&(tableau[1]), B.x, B.y, B.z);

  for (int i = 0; i < size; i++)
    {
      point3d_init (&(tableau[2]), listept[i].x, listept[i].y, listept[i].z);
      equation_plan (tableau, 3, &a, &b, &c, &d);
      for (int j = 0; j < size; j++)
	{
	  if (i != j)
	    {
	      calcul =
		a * listept[j].x + b * listept[j].y + c * listept[j].z + d;
	      if (calcul < 0)
		{
		  if (calcul < distancegauche)
		    {
		      indexgauche = i;
		      distancegauche = calcul;
		    }
		}
	      else
		{
		  if (calcul > distancedroite)
		    {
		      indexdroite = i;
		      distancedroite = calcul;
		    }
		}
	    }
	}
    }
  point3d_init (C, (listept[indexdroite].x + listept[indexgauche].x) / 2.0,
		(listept[indexdroite].y + listept[indexgauche].y) / 2.0,
		(listept[indexdroite].z + listept[indexgauche].z) / 2.0);
}

void
IF_plan_lms_contraint_vecteur (
			    vf_model * m,
			    point3d A,
			    int *listept,
			    int size,
			    point3d * C)
{
  vector3d moyenne,
    normale_sommet;
  hashtable *table = a2ri_vf_construction_edge_table (m, NULL, 0);

  vector3d_init (&moyenne, 0.0, 0.0, 0.0);

  for (int i = 0; i < size; i++)
    {
      normale_sommet =
	a2ri_vf_normal_vertex_with_hashtable (m, listept[i], table);
      moyenne = vector3d_add (&moyenne, &normale_sommet);
    }
  vector3d_normalize (&moyenne);
  point3d_init (C, A.x + moyenne.dx, A.y + moyenne.dy, A.z + moyenne.dz);

  hashtable_free (table);
  free (table);
}


void
IF_plan_lms_contraint_intersection (
				 point3d A,
				 point3d B,
				 point3d * listept,
				 int size,
				 point3d * C)
{
  double distancedroite = 0,
    distancegauche = 0;
  double a,
    b,
    c,
    d;
  int nbintersection = 0,
    nbintersectionmax = -1;
  point3d tableau[3];

  point3d_init (&(tableau[0]), A.x, A.y, A.z);
  point3d_init (&(tableau[1]), B.x, B.y, B.z);

  for (int i = 0; i < size; i++)
    {
      point3d_init (&(tableau[2]), listept[i].x, listept[i].y, listept[i].z);
      equation_plan (tableau, 3, &a, &b, &c, &d);
      nbintersection = 0;
      for (int j = 0; j < size - 1; j++)
	{
	  distancegauche =
	    a * listept[j].x + b * listept[j].y + c * listept[j].z + d;
	  distancedroite =
	    a * listept[j + 1].x + b * listept[j + 1].y + c * listept[j +
								      1].z +
	    d;
	  if (distancedroite * distancegauche < 0)
	    nbintersection++;
	}
      if (nbintersection > nbintersectionmax)
	{
	  nbintersectionmax = nbintersection;
	  point3d_init (C, listept[i].x, listept[i].y, listept[i].z);
	}
    }
}


void
IFcalcul_plan_moyenne (
		     vf_model * m,
		     int numvedep,
		     int numvefin,
		     int *listve,
		     int sizelistve,
		     point3d * A,
		     point3d * B,
		     point3d * C)
{
  point3d *listept = NULL;
  int sizelistept;

  point3d_init (A, m->ve[numvedep].x, m->ve[numvedep].y, m->ve[numvedep].z);
  point3d_init (B, m->ve[numvefin].x, m->ve[numvefin].y, m->ve[numvefin].z);

  listept = (point3d *) malloc (sizelistve * sizeof (point3d));
  a2ri_erreur_critique_si (listept == NULL,
			"erreur allocation memoire pour listept\nIFcalcul_plan_moyenne");

  for (int i = 0; i < sizelistve; i++)
    point3d_init (&(listept[i]),
		  m->ve[listve[i]].x, m->ve[listve[i]].y, m->ve[listve[i]].z);
  sizelistept = sizelistve;

  IF_plan_lms_contraint_moyenne (*A, *B, listept, sizelistept, C);

  free (listept);

}


void
IFcalcul_plan_min_max (
		     vf_model * m,
		     int numvedep,
		     int numvefin,
		     int *listve,
		     int sizelistve,
		     point3d * A,
		     point3d * B,
		     point3d * C)
{
  point3d *listept = NULL;
  int sizelistept;

  point3d_init (A, m->ve[numvedep].x, m->ve[numvedep].y, m->ve[numvedep].z);
  point3d_init (B, m->ve[numvefin].x, m->ve[numvefin].y, m->ve[numvefin].z);

  listept = (point3d *) malloc (sizelistve * sizeof (point3d));
  a2ri_erreur_critique_si (listept == NULL,
			"erreur allocation memoire pour listept\nIFcalcul_plan_min_max");

  for (int i = 0; i < sizelistve; i++)
    point3d_init (&(listept[i]),
		  m->ve[listve[i]].x, m->ve[listve[i]].y, m->ve[listve[i]].z);
  sizelistept = sizelistve;

  IF_plan_lms_contraint_min_max (*A, *B, listept, sizelistept, C);

  free (listept);

}

void
IFcalcul_plan_vecteur (
		     vf_model * m,
		     int numvedep,
		     int numvefin,
		     int *listve,
		     int sizelistve,
		     point3d * A,
		     point3d * B,
		     point3d * C)
{
  point3d_init (A, m->ve[numvedep].x, m->ve[numvedep].y, m->ve[numvedep].z);
  point3d_init (B, m->ve[numvefin].x, m->ve[numvefin].y, m->ve[numvefin].z);


  IF_plan_lms_contraint_vecteur (m, *A, listve, sizelistve, C);

}

void
IFcalcul_plan_intersection (
			  vf_model * m,
			  int numvedep,
			  int numvefin,
			  int *listve,
			  int sizelistve,
			  point3d * A,
			  point3d * B,
			  point3d * C)
{
  point3d *listept = NULL;
  int sizelistept;

  point3d_init (A, m->ve[numvedep].x, m->ve[numvedep].y, m->ve[numvedep].z);
  point3d_init (B, m->ve[numvefin].x, m->ve[numvefin].y, m->ve[numvefin].z);

  listept = (point3d *) malloc (sizelistve * sizeof (point3d));
  a2ri_erreur_critique_si (listept == NULL,
			"erreur allocation memoire pour listept\nIFcalcul_plan_min_max");

  for (int i = 0; i < sizelistve; i++)
    point3d_init (&(listept[i]),
		  m->ve[listve[i]].x, m->ve[listve[i]].y, m->ve[listve[i]].z);
  sizelistept = sizelistve;

  IF_plan_lms_contraint_intersection (*A, *B, listept, sizelistept, C);

  free (listept);

}

double
construction_chemin_lineaire_intermediaire (
					    vf_model * m,
					    int ve_dep,
					    int ve_fin,
					    point3d A,
					    point3d B,
					    point3d C,
					    int v1,
					    int v2,
					    int **list,
					    int *size)
{
  double aplan,
    bplan,
    cplan,
    dplan,
    longueurtemp;
  point3d arete1,
    arete2;
  int vsuivant = -1;
  point3d tab[3];
  point3d_init (&(tab[0]), A.x, A.y, A.z);
  point3d_init (&(tab[1]), B.x, B.y, B.z);
  point3d_init (&(tab[2]), C.x, C.y, C.z);
  equation_plan (tab, 3, &aplan, &bplan, &cplan, &dplan);

  if (v2 == ve_fin)
    return 0;

  for (int i = 0; i < m->ve[v2].nbincidentvertices; i++)
    {
      if (egalite
	  (aplan * m->ve[m->ve[v2].incidentvertices[i]].x +
	   bplan * m->ve[m->ve[v2].incidentvertices[i]].y +
	   cplan * m->ve[m->ve[v2].incidentvertices[i]].z + dplan, 0))
	if (m->ve[v2].incidentvertices[i] != v1)
	  vsuivant = m->ve[v2].incidentvertices[i];
    }

  if (vsuivant == -1 || vsuivant == ve_dep)
    return -1;

  point3d_init (&arete1, m->ve[v2].x, m->ve[v2].y, m->ve[v2].z);
  point3d_init (&arete2, m->ve[vsuivant].x, m->ve[vsuivant].y,
		m->ve[vsuivant].z);
  list_int_add (list, size, vsuivant, WITH_REDUNDANCE);
  longueurtemp =
    construction_chemin_lineaire_intermediaire (m, ve_dep, ve_fin, A, B, C,
						v2, vsuivant, list, size);
  if (longueurtemp != -1)
    return point3d_length (&arete1, &arete2) + longueurtemp;
  else
    return -1;

}

double
construction_chemin_lineaire (
			      vf_model * m,
			      int ve_dep,
			      int ve_fin,
			      point3d A,
			      point3d B,
			      point3d C,
			      int **list,
			      int *size)
{
  double longueur1 = -1,
    longueur2 = -1,
    aplan,
    bplan,
    cplan,
    dplan;
  int v1 = -1,
    v2 = -1;
  point3d tab[3],
    pv1,
    pv2;
  point3d_init (&(tab[0]), A.x, A.y, A.z);
  point3d_init (&(tab[1]), B.x, B.y, B.z);
  point3d_init (&(tab[2]), C.x, C.y, C.z);

  int *list1 = NULL,
    *list2 = NULL;
  int size1 = 0,
    size2 = 0;

  equation_plan (tab, 3, &aplan, &bplan, &cplan, &dplan);

  for (int i = 0; i < m->ve[ve_dep].nbincidentvertices; i++)
    {
      if (egalite
	  (aplan * m->ve[m->ve[ve_dep].incidentvertices[i]].x +
	   bplan * m->ve[m->ve[ve_dep].incidentvertices[i]].y +
	   cplan * m->ve[m->ve[ve_dep].incidentvertices[i]].z + dplan, 0))
	{
	  if (v1 == -1)
	    v1 = m->ve[ve_dep].incidentvertices[i];
	  else
	    v2 = m->ve[ve_dep].incidentvertices[i];
	}
    }

  if (v1 != -1)
    {
      list_int_add (&list1, &size1, ve_dep, WITH_REDUNDANCE);
      list_int_add (&list1, &size1, v1, WITH_REDUNDANCE);
    }

  if (v2 != -1)
    {
      list_int_add (&list2, &size2, ve_dep, WITH_REDUNDANCE);
      list_int_add (&list2, &size2, v2, WITH_REDUNDANCE);
    }

  if (v1 != -1)
    longueur1 =
      construction_chemin_lineaire_intermediaire (m, ve_dep, ve_fin, A, B, C,
						  ve_dep, v1, &list1, &size1);


  if (v2 != -1)
    longueur2 =
      construction_chemin_lineaire_intermediaire (m, ve_dep, ve_fin, A, B, C,
						  ve_dep, v2, &list2, &size2);



  if (longueur1 == -1 && longueur2 == -1)
    return -1;

  if (longueur1 == -1)
    longueur1 = longueur2 + 1;

  if (longueur2 == -1)
    longueur2 = longueur1 + 1;

  if (longueur1 < longueur2)
    {
      *list = list1;
      *size = size1;
      point3d_init (&pv1, m->ve[ve_dep].x, m->ve[ve_dep].y, m->ve[ve_dep].z);
      point3d_init (&pv2, m->ve[v1].x, m->ve[v1].y, m->ve[v1].z);
      return longueur1 + point3d_length (&pv1, &pv2);
    }
  else
    {
      *list = list2;
      *size = size2;
      point3d_init (&pv1, m->ve[ve_dep].x, m->ve[ve_dep].y, m->ve[ve_dep].z);
      point3d_init (&pv2, m->ve[v2].x, m->ve[v2].y, m->ve[v2].z);
      return longueur2 + point3d_length (&pv1, &pv2);
    }

}

/********** MAIN FUNCTIONS **********/

/**
   Calcul du chemin le plus court entre deux sommets du modele
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @return la longueur du chemin
*/
double
a2ri_vf_dijkstra (
		  const vf_model * const m,
		  int ve_dep,
		  int ve_fin,
		  int **list,
		  int *size)
{
  double *parcouru = NULL;
  int *precedent = NULL;
  int *pasencorevu = NULL,
    size_pasencorevu = 0;
  double distance;
  int index = 0,
    n1 = 0,
    n2;
  vf_edge *e;
  ptf_func_hashtable func[1];

  func[0] = a2ri_vf_update_length_edge;

  hashtable *table = a2ri_vf_construction_edge_table (m, func, 1);

  //initialisation des tableaux parcouru, precedent et pasencorevu
  precedent=(int*)malloc(m->nbvertex*sizeof(int));
  parcouru=(double*)malloc(m->nbvertex*sizeof(double));
  pasencorevu=(int*)malloc(m->nbvertex*sizeof(int));
  for(int i=0;i<m->nbvertex;i++)
    {
      precedent[i]=-1;
      parcouru[i]=-1;
      pasencorevu[i]=i;
    }

  size_pasencorevu=m->nbvertex;

  parcouru[ve_dep] = 0;


  while (size_pasencorevu != 0)
    {
      double dist_min = -1;
      for (int i = 0; i < size_pasencorevu; i++)
	{
	  if (parcouru[pasencorevu[i]] >= 0
	      && (parcouru[pasencorevu[i]] < dist_min || dist_min < 0))
	    {
	      index = i;
	      n1 = pasencorevu[i];
	      dist_min = parcouru[n1];
	    }
	}

      list_int_remove (&pasencorevu, &size_pasencorevu, index);

      for (int i = 0; i < m->ve[n1].nbincidentvertices; i++)
	{
	  n2 = m->ve[n1].incidentvertices[i];

	  e = hashtable_look_for (table, n1, n2);

	  distance = e->att_double;

	  if (parcouru[n2] > parcouru[n1] + distance || parcouru[n2] == -1)
	    {
	      parcouru[n2] = parcouru[n1] + distance;
	      precedent[n2] = n1;
	    }

	}

    }

  index = ve_fin;

  while (index != ve_dep)
    {
      point3d p1,
        p2;
      point3d_init (&p1, m->ve[ve_dep].x, m->ve[ve_dep].y, m->ve[ve_dep].z);
      point3d_init (&p2, m->ve[precedent[index]].x, m->ve[precedent[index]].y,
		    m->ve[precedent[index]].z);
      double distance = point3d_length (&p1, &p2);
      for (int i = 0; i < m->ve[index].nbincidentvertices; i++)
	{
	  if (parcouru[precedent[index]] ==
	      parcouru[m->ve[index].incidentvertices[i]])
	    {
	      point3d_init (&p2, m->ve[m->ve[index].incidentvertices[i]].x,
			    m->ve[m->ve[index].incidentvertices[i]].y,
			    m->ve[m->ve[index].incidentvertices[i]].z);
	      if (distance > point3d_length (&p1, &p2))
		{
		  distance = point3d_length (&p1, &p2);
		  precedent[index] = m->ve[index].incidentvertices[i];
		}
	    }
	}
      list_int_add (list, size, index, WITH_REDUNDANCE);
      index = precedent[index];
    }
  list_int_add (list, size, ve_dep, WITH_REDUNDANCE);

  list_int_reverse (*list, *size);

  hashtable_free (table);

  return parcouru[ve_fin];

}

/**
   Calcul du chemin le plus court entre deux sommets du modele
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @return la longueur du chemin
*/
double
a2ri_vf_A_star (
		  const vf_model * const m,
		  int ve_dep,
		  int ve_fin,
		  int **list,
		  int *size)
{
  int *liste_ouverte=NULL,size_liste_ouverte=0,*liste_fermee=NULL,size_liste_fermee=0;
  double *parcouru=NULL,*distance_fin=NULL;
  int *precedent=NULL;
  int courant;
  point3d pcourant,pvoisin,pfin;
  int index;


  point3d_init(&pfin,m->ve[ve_fin].x,m->ve[ve_fin].y,m->ve[ve_fin].z);

  precedent=(int*)malloc(m->nbvertex*sizeof(int));
  parcouru=(double*)malloc(m->nbvertex*sizeof(double));
  distance_fin=(double*)malloc(m->nbvertex*sizeof(double));
  for(int i=0;i<m->nbvertex;i++)
    {
      precedent[i]=-1;
      parcouru[i]=-1;
      distance_fin[i]=-1;
    }

  list_int_add(&liste_ouverte,&size_liste_ouverte,ve_dep,WITH_REDUNDANCE);
  parcouru[ve_dep]=0; 
  courant=ve_dep;

  //boucle principale de l'algorithme
  while(courant!=ve_fin && size_liste_ouverte!=0)
    {

      index=0;
      if(distance_fin[liste_ouverte[0]]<0)
	{
	  point3d_init(&pcourant,m->ve[liste_ouverte[0]].x,m->ve[liste_ouverte[0]].y,m->ve[liste_ouverte[0]].z);
	  distance_fin[liste_ouverte[0]]=point3d_length(&pcourant,&pfin);
	}
      //on cherche le meilleur dans liste_ouverte
      for(int i=1;i<size_liste_ouverte;i++)
	{
	  if(distance_fin[liste_ouverte[i]]<0)
	    {
	      point3d_init(&pcourant,m->ve[liste_ouverte[i]].x,m->ve[liste_ouverte[i]].y,m->ve[liste_ouverte[i]].z);
	      distance_fin[liste_ouverte[i]]=point3d_length(&pcourant,&pfin);
	    }
	  if(distance_fin[liste_ouverte[i]]<distance_fin[liste_ouverte[index]])
	    index=i;
	}
      courant=liste_ouverte[index];

      
      if(courant!=ve_fin)
	{
	  list_int_remove(&liste_ouverte,&size_liste_ouverte,index);

	  list_int_add(&liste_fermee,&size_liste_fermee,courant,WITH_REDUNDANCE);

	  
	  //ajout des voisins du sommet courant dans la liste ouverte 
	  point3d_init(&pcourant,m->ve[courant].x,m->ve[courant].y,m->ve[courant].z);
	  for(int i=0;i<m->ve[courant].nbincidentvertices;i++)
	    {

	      if(list_int_contains(liste_fermee,size_liste_fermee,m->ve[courant].incidentvertices[i])==-1)
		{
		  point3d_init(&pvoisin,
			       m->ve[m->ve[courant].incidentvertices[i]].x,
			       m->ve[m->ve[courant].incidentvertices[i]].y,
			       m->ve[m->ve[courant].incidentvertices[i]].z);
		  if(list_int_contains(liste_ouverte,
				       size_liste_ouverte,
				       m->ve[courant].incidentvertices[i])!=-1)
		    {
		      if(parcouru[m->ve[courant].incidentvertices[i]]>parcouru[courant]+point3d_length(&pcourant,&pvoisin))
			{
			  precedent[m->ve[courant].incidentvertices[i]]=courant;
			  parcouru[m->ve[courant].incidentvertices[i]]=parcouru[courant]+point3d_length(&pcourant,&pvoisin);
			}
		    }
		  else
		    {
		      precedent[m->ve[courant].incidentvertices[i]]=courant;
		      parcouru[m->ve[courant].incidentvertices[i]]=parcouru[courant]+point3d_length(&pcourant,&pvoisin);
		      list_int_add(&liste_ouverte,&size_liste_ouverte,m->ve[courant].incidentvertices[i],WITH_REDUNDANCE);
		    }
		}
	      
	    }
	}
    }
  

  if(size_liste_ouverte==0)
    {
      *list=NULL;
      *size=0;
      return -1;
    }  

  courant=ve_fin;

  while(courant!=ve_dep)
    {
      list_int_add(list,size,courant,WITH_REDUNDANCE);
      courant=precedent[courant];
    }
  list_int_add(list,size,ve_dep,WITH_REDUNDANCE);
  
  list_int_reverse (*list, *size);

  return parcouru[ve_fin];
}


/**
   Calcul du chemin le plus court entre deux sommets du modele
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @return la longueur du chemin
*/
double
a2ri_vf_approche (
		  const vf_model * const m,
		  int ve_dep,
		  int ve_fin,
		  int **list,
		  int *size)
{
  int fin = 0,
    index = ve_dep,
    a_ajouter = ve_dep;
  double distance = 0;
  point3d p1,
    p2,
    p3;
  double length = 0;

  list_int_add (list, size, a_ajouter, WITH_REDUNDANCE);
  
  point3d_init (&p2, m->ve[ve_fin].x, m->ve[ve_fin].y, m->ve[ve_fin].z);

  while (!fin && index != ve_fin)
    {

      for (int i = 0; i < m->ve[index].nbincidentvertices; i++)
	{
	  point3d_init (&p1, m->ve[m->ve[index].incidentvertices[i]].x,
			m->ve[m->ve[index].incidentvertices[i]].y,
			m->ve[m->ve[index].incidentvertices[i]].z);
	  if (i == 0)
	    {
	      distance = point3d_length (&p1, &p2);
	      a_ajouter = m->ve[index].incidentvertices[0];
	    }
	  else
	    {
	      if (distance > point3d_length (&p1, &p2))
		{
		  distance = point3d_length (&p1, &p2);
		  a_ajouter = m->ve[index].incidentvertices[i];
		}
	    }
	}

      point3d_init (&p1, m->ve[(*list)[(*size) - 1]].x,
		    m->ve[(*list)[(*size) - 1]].y,
		    m->ve[(*list)[(*size) - 1]].z);
      point3d_init (&p3, m->ve[a_ajouter].x, m->ve[a_ajouter].y,
		    m->ve[a_ajouter].z);

      if (list_int_contains (*list, *size, a_ajouter) != -1)
	fin = 1;

      index = a_ajouter;
      list_int_add (list, size, a_ajouter, WITH_REDUNDANCE);
      length = length + point3d_length (&p1, &p3);
      if (fin)
	{
	  length = -1;
	  free (*list);
	  *list = NULL;
	  *size = 0;
	}
    }

  return length;
}


/**
   Calcul du chemin le plus court entre deux sommets du modele
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @return la longueur du chemin
*/
double
a2ri_vef_dijkstra (
		   const vef_model * const m,
		   int ve_dep,
		   int ve_fin,
		   int **list,
		   int *size)
{
  double *parcouru = NULL;
  int size_parcouru = 0;
  int *precedent = NULL,
    size_precedent = 0;
  int *pasencorevu = NULL,
    size_pasencorevu = 0;
  double *distance = NULL;
  int size_distance = 0;
  point3d p1,
    p2;
  int index = 0,
    n1 = 0,
    n2;


  //initialisation des longueurs des aretes
  for (int i = 0; i < m->nbedge; i++)
    {
      point3d_init (&p1, m->ve[m->ed[i].ve1].x, m->ve[m->ed[i].ve1].y,
		    m->ve[m->ed[i].ve1].z);
      point3d_init (&p2, m->ve[m->ed[i].ve2].x, m->ve[m->ed[i].ve2].y,
		    m->ve[m->ed[i].ve2].z);
      list_double_add (&distance, &size_distance, point3d_length (&p1, &p2),
		       WITH_REDUNDANCE);
    }

  //initialisation des tableaux parcouru, precedent et pasencorevu
  for (int i = 0; i < m->nbvertex; i++)
    {
      list_double_add (&parcouru, &size_parcouru, -1, WITH_REDUNDANCE);
      list_int_add (&precedent, &size_precedent, -1, WITH_REDUNDANCE);
      list_int_add (&pasencorevu, &size_pasencorevu, i, WITH_REDUNDANCE);
    }

  parcouru[ve_dep] = 0;


  while (size_pasencorevu != 0)
    {
      double dist_min = -1;
      for (int i = 0; i < size_pasencorevu; i++)
	{
	  if (parcouru[pasencorevu[i]] >= 0
	      && (parcouru[pasencorevu[i]] < dist_min || dist_min < 0))
	    {
	      index = i;
	      n1 = pasencorevu[i];
	      dist_min = parcouru[n1];
	    }
	}

      list_int_remove (&pasencorevu, &size_pasencorevu, index);

      for (int i = 0; i < m->ve[n1].nbsharededges; i++)
	{
	  if (m->ed[m->ve[n1].sharededges[i]].ve1 == n1)
	    n2 = m->ed[m->ve[n1].sharededges[i]].ve2;
	  else
	    n2 = m->ed[m->ve[n1].sharededges[i]].ve1;

	  if (parcouru[n2] >
	      parcouru[n1] + distance[a2ri_vef_search_edge (m, n1, n2)]
	      || parcouru[n2] == -1)
	    {
	      parcouru[n2] =
		parcouru[n1] + distance[a2ri_vef_search_edge (m, n1, n2)];
	      precedent[n2] = n1;
	    }
	}

    }

  index = ve_fin;
  while (index != ve_dep)
    {
      list_int_add (list, size, index, WITH_REDUNDANCE);
      index = precedent[index];
    }
  list_int_add (list, size, ve_dep, WITH_REDUNDANCE);

  return parcouru[ve_fin];

}

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return aucun
**/
int
a2ri_vf_geodesic_path_approche_plan_moyen (
					   vf_model * m,
					   int ve_dep,
					   int ve_fin,
					   int **list,
					   int *size,
					   double *length,
					   int nbsubdiv)
{
  int *listtemp1 = NULL,
    size_listtemp1 = 0;
  int *listtemp2 = NULL,
    size_listtemp2 = 0;
  point3d A,
    B,
    C;

  vf_model *temp = a2ri_vf_clone (m);

  a2ri_vf_6_subdivision (temp, nbsubdiv);

  a2ri_vf_approche (temp, ve_dep, ve_fin, &listtemp1, &size_listtemp1);

  if (size_listtemp1 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  a2ri_vf_approche (temp, ve_fin, ve_dep, &listtemp2, &size_listtemp2);
  if (size_listtemp2 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  list_int_remove (&listtemp1, &size_listtemp1, size_listtemp1 - 1);
  list_int_remove (&listtemp2, &size_listtemp2, size_listtemp2 - 1);
  list_int_reverse (listtemp2, size_listtemp2);
  for (int i = 0; i < size_listtemp2; i++)
    list_int_add (&listtemp1, &size_listtemp1, listtemp2[i], WITH_REDUNDANCE);
  free (listtemp2);
  IFcalcul_plan_moyenne (temp, listtemp1[0], listtemp1[size_listtemp1 - 1],
		       listtemp1 + 1, size_listtemp1 - 2, &A, &B, &C);
  a2ri_vf_free (temp);
  free (temp);
  if (trois_points_alignes (&A, &B, &C))
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_POINTS_ALIGNES;
    }
  a2ri_vf_subdivision_by_plane (m, &A, &B, &C);
  free (listtemp1);

  *list = NULL;
  *size = 0;
  *length =
    construction_chemin_lineaire (m, ve_dep, ve_fin, A, B, C, list, size);

  return 1;
}

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return aucun
**/
int
a2ri_vf_geodesic_path_A_star_plan_moyen (
					   vf_model * m,
					   int ve_dep,
					   int ve_fin,
					   int **list,
					   int *size,
					   double *length,
					   int nbsubdiv)
{
  int *listtemp1 = NULL,
    size_listtemp1 = 0;
  int *listtemp2 = NULL,
    size_listtemp2 = 0;
  point3d A,
    B,
    C;

  vf_model *temp = a2ri_vf_clone (m);

  a2ri_vf_6_subdivision (temp, nbsubdiv);
  a2ri_vf_A_star (temp, ve_dep, ve_fin, &listtemp1, &size_listtemp1);
  if (size_listtemp1 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  a2ri_vf_A_star (temp, ve_fin, ve_dep, &listtemp2, &size_listtemp2);
  if (size_listtemp2 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  list_int_remove (&listtemp1, &size_listtemp1, size_listtemp1 - 1);
  list_int_remove (&listtemp2, &size_listtemp2, size_listtemp2 - 1);
  list_int_reverse (listtemp2, size_listtemp2);
  for (int i = 0; i < size_listtemp2; i++)
    list_int_add (&listtemp1, &size_listtemp1, listtemp2[i], WITH_REDUNDANCE);
  free (listtemp2);
  IFcalcul_plan_moyenne (temp, listtemp1[0], listtemp1[size_listtemp1 - 1],
			 listtemp1 + 1, size_listtemp1 - 2, &A, &B, &C);
  a2ri_vf_free (temp);
  free (temp);
  if (trois_points_alignes (&A, &B, &C))
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_POINTS_ALIGNES;
    }
  a2ri_vf_subdivision_by_plane (m, &A, &B, &C);
  free (listtemp1);

  *list = NULL;
  *size = 0;
  *length =
    construction_chemin_lineaire (m, ve_dep, ve_fin, A, B, C, list, size);

  return 1;
}


/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return aucun
**/
int
a2ri_vf_geodesic_path_approche_plan_minmax (
					    vf_model * m,
					    int ve_dep,
					    int ve_fin,
					    int **list,
					    int *size,
					    double *length,
					    int nbsubdiv)
{
  int *listtemp1 = NULL,
    size_listtemp1 = 0;
  int *listtemp2 = NULL,
    size_listtemp2 = 0;
  point3d A,
    B,
    C;

  vf_model *temp = a2ri_vf_clone (m);

  a2ri_vf_6_subdivision (temp, nbsubdiv);

  a2ri_vf_approche (temp, ve_dep, ve_fin, &listtemp1, &size_listtemp1);
  if (size_listtemp1 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  a2ri_vf_approche (temp, ve_fin, ve_dep, &listtemp2, &size_listtemp2);
  if (size_listtemp2 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  list_int_remove (&listtemp1, &size_listtemp1, size_listtemp1 - 1);
  list_int_remove (&listtemp2, &size_listtemp2, size_listtemp2 - 1);
  list_int_reverse (listtemp2, size_listtemp2);
  for (int i = 0; i < size_listtemp2; i++)
    list_int_add (&listtemp1, &size_listtemp1, listtemp2[i], WITH_REDUNDANCE);
  free (listtemp2);
  IFcalcul_plan_min_max (temp, listtemp1[0], listtemp1[size_listtemp1 - 1],
		       listtemp1 + 1, size_listtemp1 - 2, &A, &B, &C);
  a2ri_vf_free (temp);
  free (temp);
  if (trois_points_alignes (&A, &B, &C))
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_POINTS_ALIGNES;
    }
  a2ri_vf_subdivision_by_plane (m, &A, &B, &C);
  free (listtemp1);


  *list = NULL;
  *size = 0;
  *length =
    construction_chemin_lineaire (m, ve_dep, ve_fin, A, B, C, list, size);

  return 1;
}

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return aucun
**/
int
a2ri_vf_geodesic_path_A_star_plan_minmax (
					    vf_model * m,
					    int ve_dep,
					    int ve_fin,
					    int **list,
					    int *size,
					    double *length,
					    int nbsubdiv)
{
  int *listtemp1 = NULL,
    size_listtemp1 = 0;
  int *listtemp2 = NULL,
    size_listtemp2 = 0;
  point3d A,
    B,
    C;


  vf_model *temp = a2ri_vf_clone (m);

  a2ri_vf_6_subdivision (temp, nbsubdiv);

  a2ri_vf_A_star (temp, ve_dep, ve_fin, &listtemp1, &size_listtemp1);
  if (size_listtemp1 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  a2ri_vf_A_star (temp, ve_fin, ve_dep, &listtemp2, &size_listtemp2);
  if (size_listtemp2 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  list_int_remove (&listtemp1, &size_listtemp1, size_listtemp1 - 1);
  list_int_remove (&listtemp2, &size_listtemp2, size_listtemp2 - 1);
  list_int_reverse (listtemp2, size_listtemp2);
  for (int i = 0; i < size_listtemp2; i++)
    list_int_add (&listtemp1, &size_listtemp1, listtemp2[i], WITH_REDUNDANCE);
  free (listtemp2);
  IFcalcul_plan_min_max (temp, listtemp1[0], listtemp1[size_listtemp1 - 1],
		       listtemp1 + 1, size_listtemp1 - 2, &A, &B, &C);
  a2ri_vf_free (temp);
  free (temp);
  if (trois_points_alignes (&A, &B, &C))
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_POINTS_ALIGNES;
    }
  a2ri_vf_subdivision_by_plane (m, &A, &B, &C);
  free (listtemp1);


  *list = NULL;
  *size = 0;
  *length =
    construction_chemin_lineaire (m, ve_dep, ve_fin, A, B, C, list, size);

  return 1;
}


/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return aucun
**/
int
a2ri_vf_geodesic_path_approche_plan_vecteur (
					     vf_model * m,
					     int ve_dep,
					     int ve_fin,
					     int **list,
					     int *size,
					     double *length,
					     int nbsubdiv)
{
  int *listtemp1 = NULL,
    size_listtemp1 = 0;
  int *listtemp2 = NULL,
    size_listtemp2 = 0;
  point3d A,
    B,
    C;

  vf_model *temp = a2ri_vf_clone (m);

  a2ri_vf_6_subdivision (temp, nbsubdiv);

  a2ri_vf_approche (temp, ve_dep, ve_fin, &listtemp1, &size_listtemp1);

  if (size_listtemp1 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  a2ri_vf_approche (temp, ve_fin, ve_dep, &listtemp2, &size_listtemp2);

  if (size_listtemp2 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  list_int_remove (&listtemp1, &size_listtemp1, size_listtemp1 - 1);
  list_int_remove (&listtemp2, &size_listtemp2, size_listtemp2 - 1);
  list_int_reverse (listtemp2, size_listtemp2);
  for (int i = 0; i < size_listtemp2; i++)
    list_int_add (&listtemp1, &size_listtemp1, listtemp2[i], WITH_REDUNDANCE);
  free (listtemp2);
  IFcalcul_plan_vecteur (temp, listtemp1[0], listtemp1[size_listtemp1 - 1],
		       listtemp1 + 1, size_listtemp1 - 2, &A, &B, &C);
  a2ri_vf_free (temp);
  free (temp);
  if (trois_points_alignes (&A, &B, &C))
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_POINTS_ALIGNES;
    }
  a2ri_vf_subdivision_by_plane (m, &A, &B, &C);
  free (listtemp1);

  free (*list);
  *list = NULL;
  *size = 0;
  *length =
    construction_chemin_lineaire (m, ve_dep, ve_fin, A, B, C, list, size);

  return 1;
}

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return aucun
**/
int
a2ri_vf_geodesic_path_A_star_plan_vecteur (
					     vf_model * m,
					     int ve_dep,
					     int ve_fin,
					     int **list,
					     int *size,
					     double *length,
					     int nbsubdiv)
{
  int *listtemp1 = NULL,
    size_listtemp1 = 0;
  int *listtemp2 = NULL,
    size_listtemp2 = 0;
  point3d A,
    B,
    C;

  vf_model *temp = a2ri_vf_clone (m);

  a2ri_vf_6_subdivision (temp, nbsubdiv);

  a2ri_vf_A_star (temp, ve_dep, ve_fin, &listtemp1, &size_listtemp1);

  if (size_listtemp1 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  a2ri_vf_A_star (temp, ve_fin, ve_dep, &listtemp2, &size_listtemp2);

  if (size_listtemp2 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  list_int_remove (&listtemp1, &size_listtemp1, size_listtemp1 - 1);
  list_int_remove (&listtemp2, &size_listtemp2, size_listtemp2 - 1);
  list_int_reverse (listtemp2, size_listtemp2);
  for (int i = 0; i < size_listtemp2; i++)
    list_int_add (&listtemp1, &size_listtemp1, listtemp2[i], WITH_REDUNDANCE);
  free (listtemp2);
  IFcalcul_plan_vecteur (temp, listtemp1[0], listtemp1[size_listtemp1 - 1],
		       listtemp1 + 1, size_listtemp1 - 2, &A, &B, &C);
  a2ri_vf_free (temp);
  free (temp);
  if (trois_points_alignes (&A, &B, &C))
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_POINTS_ALIGNES;
    }
  a2ri_vf_subdivision_by_plane (m, &A, &B, &C);
  free (listtemp1);

  free (*list);
  *list = NULL;
  *size = 0;
  *length =
    construction_chemin_lineaire (m, ve_dep, ve_fin, A, B, C, list, size);

  return 1;
}

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return aucun
**/
int
a2ri_vf_geodesic_path_approche_plan_intersection (
						  vf_model * m,
						  int ve_dep,
						  int ve_fin,
						  int **list,
						  int *size,
						  double *length,
						  int nbsubdiv)
{
  int *listtemp1 = NULL,
    size_listtemp1 = 0;
  int *listtemp2 = NULL,
    size_listtemp2 = 0;
  point3d A,
    B,
    C;


  vf_model *temp = a2ri_vf_clone (m);

  a2ri_vf_6_subdivision (temp, nbsubdiv);

  a2ri_vf_approche (temp, ve_dep, ve_fin, &listtemp1, &size_listtemp1);
  if (size_listtemp1 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  a2ri_vf_approche (temp, ve_fin, ve_dep, &listtemp2, &size_listtemp2);
  if (size_listtemp2 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  list_int_remove (&listtemp1, &size_listtemp1, size_listtemp1 - 1);
  list_int_remove (&listtemp2, &size_listtemp2, size_listtemp2 - 1);
  list_int_reverse (listtemp2, size_listtemp2);
  for (int i = 0; i < size_listtemp2; i++)
    list_int_add (&listtemp1, &size_listtemp1, listtemp2[i], WITH_REDUNDANCE);
  free (listtemp2);
  IFcalcul_plan_intersection (temp, listtemp1[0], listtemp1[size_listtemp1 - 1],
			    listtemp1 + 1, size_listtemp1 - 2, &A, &B, &C);
  a2ri_vf_free (temp);
  free (temp);
  if (trois_points_alignes (&A, &B, &C))
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_POINTS_ALIGNES;
    }
  a2ri_vf_subdivision_by_plane (m, &A, &B, &C);
  free (listtemp1);

  *list = NULL;
  *size = 0;
  *length =
    construction_chemin_lineaire (m, ve_dep, ve_fin, A, B, C, list, size);

  return 1;
}

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return aucun
**/
int
a2ri_vf_geodesic_path_A_star_plan_intersection (
						  vf_model * m,
						  int ve_dep,
						  int ve_fin,
						  int **list,
						  int *size,
						  double *length,
						  int nbsubdiv)
{
  int *listtemp1 = NULL,
    size_listtemp1 = 0;
  int *listtemp2 = NULL,
    size_listtemp2 = 0;
  point3d A,
    B,
    C;

  vf_model *temp = a2ri_vf_clone (m);

  a2ri_vf_6_subdivision (temp, nbsubdiv);

  a2ri_vf_A_star (temp, ve_dep, ve_fin, &listtemp1, &size_listtemp1);
  if (size_listtemp1 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  a2ri_vf_A_star (temp, ve_fin, ve_dep, &listtemp2, &size_listtemp2);
  if (size_listtemp2 == 0)
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_CHEMIN_INTROUVABLE;
    }
  list_int_remove (&listtemp1, &size_listtemp1, size_listtemp1 - 1);
  list_int_remove (&listtemp2, &size_listtemp2, size_listtemp2 - 1);
  list_int_reverse (listtemp2, size_listtemp2);
  for (int i = 0; i < size_listtemp2; i++)
    list_int_add (&listtemp1, &size_listtemp1, listtemp2[i], WITH_REDUNDANCE);
  free (listtemp2);
  IFcalcul_plan_intersection (temp, listtemp1[0], listtemp1[size_listtemp1 - 1],
			    listtemp1 + 1, size_listtemp1 - 2, &A, &B, &C);
  a2ri_vf_free (temp);
  free (temp);
  if (trois_points_alignes (&A, &B, &C))
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_POINTS_ALIGNES;
    }
  a2ri_vf_subdivision_by_plane (m, &A, &B, &C);
  free (listtemp1);

  *list = NULL;
  *size = 0;
  *length =
    construction_chemin_lineaire (m, ve_dep, ve_fin, A, B, C, list, size);

  return 1;
}


/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return aucun
**/
int
a2ri_vf_geodesic_path_dijkstra_plan_minmax (
					    vf_model * m,
					    int ve_dep,
					    int ve_fin,
					    int **list,
					    int *size,
					    double *length,
					    int nbsubdiv)
{
  int *listtemp1 = NULL,
    size_listtemp1 = 0;
  point3d A,
    B,
    C;

  vf_model *temp = a2ri_vf_clone (m);

  a2ri_vf_6_subdivision (temp, nbsubdiv);

  a2ri_vf_dijkstra (temp, ve_dep, ve_fin, &listtemp1, &size_listtemp1);
  IFcalcul_plan_min_max (temp, listtemp1[0], listtemp1[size_listtemp1 - 1],
		       listtemp1 + 1, size_listtemp1 - 2, &A, &B, &C);
  a2ri_vf_free (temp);
  free (temp);
  if (trois_points_alignes (&A, &B, &C))
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_POINTS_ALIGNES;
    }
  a2ri_vf_subdivision_by_plane (m, &A, &B, &C);
  free (listtemp1);

  *list = NULL;
  *size = 0;
  *length =
    construction_chemin_lineaire (m, ve_dep, ve_fin, A, B, C, list, size);

  return 1;
}

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return aucun
**/
int
a2ri_vf_geodesic_path_dijkstra_plan_vecteur (
					     vf_model * m,
					     int ve_dep,
					     int ve_fin,
					     int **list,
					     int *size,
					     double *length,
					     int nbsubdiv)
{
  int *listtemp1 = NULL,
    size_listtemp1 = 0;
  point3d A,
    B,
    C;

  vf_model *temp = a2ri_vf_clone (m);

  a2ri_vf_6_subdivision (temp, nbsubdiv);

  a2ri_vf_dijkstra (temp, ve_dep, ve_fin, &listtemp1, &size_listtemp1);

  IFcalcul_plan_vecteur (temp, listtemp1[0], listtemp1[size_listtemp1 - 1],
		       listtemp1 + 1, size_listtemp1 - 2, &A, &B, &C);
  a2ri_vf_free (temp);
  free (temp);
  if (trois_points_alignes (&A, &B, &C))
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_POINTS_ALIGNES;
    }
  a2ri_vf_subdivision_by_plane (m, &A, &B, &C);
  free (listtemp1);

  *list = NULL;
  *size = 0;
  *length =
    construction_chemin_lineaire (m, ve_dep, ve_fin, A, B, C, list, size);

  return 1;
}

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return aucun
**/
int
a2ri_vf_geodesic_path_dijkstra_plan_intersection (
						  vf_model * m,
						  int ve_dep,
						  int ve_fin,
						  int **list,
						  int *size,
						  double *length,
						  int nbsubdiv)
{
  int *listtemp1 = NULL,
    size_listtemp1 = 0;
  point3d A,
    B,
    C;

  vf_model *temp = a2ri_vf_clone (m);

  a2ri_vf_6_subdivision (temp, nbsubdiv);

  a2ri_vf_dijkstra (temp, ve_dep, ve_fin, &listtemp1, &size_listtemp1);
  IFcalcul_plan_intersection (temp, listtemp1[0], listtemp1[size_listtemp1 - 1],
			    listtemp1 + 1, size_listtemp1 - 2, &A, &B, &C);
  a2ri_vf_free (temp);
  free (temp);
  if (trois_points_alignes (&A, &B, &C))
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_POINTS_ALIGNES;
    }
  a2ri_vf_subdivision_by_plane (m, &A, &B, &C);
  free (listtemp1);

  *list = NULL;
  *size = 0;
  *length =
    construction_chemin_lineaire (m, ve_dep, ve_fin, A, B, C, list, size);

  return 1;
}

/**
   Calcul le chemin géodésique entre deux sommets. Le maillage sera subdivisé et le chemin entre des deux sommets retournés
   @param m le modèle
   @param ve_dep sommet de départ
   @param ve_fin sommet d'arrivée
   @param list liste des sommets parcourus
   @param size taille du tableau contenant les sommets parcourus
   @param length longueur totale parcouru
   @return aucun
**/
int
a2ri_vf_geodesic_path_dijkstra_plan_moyen (
					   vf_model * m,
					   int ve_dep,
					   int ve_fin,
					   int **list,
					   int *size,
					   double *length,
					   int nbsubdiv)
{
  int *listtemp1 = NULL,
    size_listtemp1 = 0;
  point3d A,
    B,
    C;

  vf_model *temp = a2ri_vf_clone (m);

  a2ri_vf_6_subdivision (temp, nbsubdiv);

  a2ri_vf_dijkstra (temp, ve_dep, ve_fin, &listtemp1, &size_listtemp1);
  IFcalcul_plan_moyenne (temp, listtemp1[0], listtemp1[size_listtemp1 - 1],
		       listtemp1 + 1, size_listtemp1 - 2, &A, &B, &C);
  a2ri_vf_free (temp);
  free (temp);
  if (trois_points_alignes (&A, &B, &C))
    {
      *length = -1;
      *size = 0;
      *list = NULL;
      return A2RI_GEODESIQUE_POINTS_ALIGNES;
    }
  a2ri_vf_subdivision_by_plane (m, &A, &B, &C);
  free (listtemp1);

  *list = NULL;
  *size = 0;
  *length =
    construction_chemin_lineaire (m, ve_dep, ve_fin, A, B, C, list, size);

  return 1;
}
