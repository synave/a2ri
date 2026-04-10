/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 01/11/07       */
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

#include "space_partition.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

/********** MAIN FUNCTIONS **********/

/**
   Initialisation d'une partition de l'espace
   @param sp la partition de l'espace
   @return aucun
*/
void
space_partition_init (
		      space_partition * sp)
{
  point3d_init (&(sp->ptmin), 0, 0, 0);
  point3d_init (&(sp->ptmax), 0, 0, 0);
  sp->nb_part_x = 0;
  sp->nb_part_y = 0;
  sp->nb_part_z = 0;
  sp->x = NULL;
}

/**
   Création d'une nouvelle partition de l'espace
   @param sp la partition de l'espace
   @param min coin inférieur
   @param max coin supérieur
   @param nbpartx nombre de partition en X
   @param nbparty nombre de partition en Y
   @param nbpartz nombre de partition en Z
   @return aucun
*/
void
space_partition_new (
		     space_partition * sp,
		     const point3d * const min,
		     const point3d * const max,
		     int nbpartx,
		     int nbparty,
		     int nbpartz)
{
  point3d_init (&(sp->ptmin), min->x, min->y, min->z);
  point3d_init (&(sp->ptmax), max->x, max->y, max->z);
  sp->nb_part_x = nbpartx;
  sp->nb_part_y = nbparty;
  sp->nb_part_z = nbpartz;
  sp->x = (sp_width *) malloc (nbpartx * sizeof (sp_width));
  a2ri_erreur_critique_si (sp->x == NULL,
			"erreur allocation memoire pour sp->x\nspace_partition_new");
  for (int i = 0; i < nbpartx; i++)
    {
      sp->x[i].y = (sp_height *) malloc (nbparty * sizeof (sp_height));
      a2ri_erreur_critique_si (sp->x[i].y == NULL,
			    "erreur allocation memoire pour sp->x[i].y\nspace_partition_new");
      for (int j = 0; j < nbparty; j++)
	{
	  sp->x[i].y[j].z = (sp_depth *) malloc (nbpartz * sizeof (sp_depth));
	  a2ri_erreur_critique_si (sp->x[i].y[j].z == NULL,
				"erreur allocation memoire pour sp->x[i].y[j].z\nspace_partition_new");
	  for (int k = 0; k < nbpartz; k++)
	    {
	      sp->x[i].y[j].z[k].list_point = NULL;
	      sp->x[i].y[j].z[k].nb_point = 0;
	    }
	}
    }
}

/**
   Désallocation mémoire de la partition de l'espace
   @param sp la partition de l'espace
   @return aucun
*/
void
space_partition_free (
		      space_partition * sp)
{
  for (int i = 0; i < sp->nb_part_x; i++)
    for (int j = 0; j < sp->nb_part_y; j++)
      for (int k = 0; k < sp->nb_part_z; k++)
	free (sp->x[i].y[j].z[k].list_point);

  for (int i = 0; i < sp->nb_part_x; i++)
    for (int j = 0; j < sp->nb_part_y; j++)
      free (sp->x[i].y[j].z);

  for (int i = 0; i < sp->nb_part_x; i++)
    free (sp->x[i].y);

  free (sp->x);
  sp->x = NULL;
}

/**
   Affichage simple d'une partition de l'espace
   @param sp la partition de l'espace
   @return aucun
*/
void
space_partition_display (
			 const space_partition * const sp)
{
  printf ("xmin,ymin,zmin : (%lf,%lf,%lf) - xmin,ymin,zmin : (%lf,%lf,%lf)\n",
	  sp->ptmin.x, sp->ptmin.y, sp->ptmin.z, sp->ptmax.x, sp->ptmax.y,
	  sp->ptmax.z);
  printf
    ("nombre de partition (X Y Z): %d %d %d - dx,dy,dz : %lf , %lf , %lf\n",
     sp->nb_part_x, sp->nb_part_y, sp->nb_part_z,
     (sp->ptmax.x - sp->ptmin.x) / (sp->nb_part_x * 1.0),
     (sp->ptmax.y - sp->ptmin.y) / (sp->nb_part_y * 1.0),
     (sp->ptmax.z - sp->ptmin.z) / (sp->nb_part_z * 1.0));
}

/**
   Affichage simple d'une partition de l'espace
   @param sp la partition de l'espace
   @return aucun
*/
void
space_partition_display_detail (
				const space_partition * const sp)
{
  printf ("xmin,ymin,zmin : (%lf,%lf,%lf) - xmin,ymin,zmin : (%lf,%lf,%lf)\n",
	  sp->ptmin.x, sp->ptmin.y, sp->ptmin.z, sp->ptmax.x, sp->ptmax.y,
	  sp->ptmax.z);
  printf
    ("nombre de partition (X Y Z): %d %d %d - dx,dy,dz : %lf , %lf , %lf\n",
     sp->nb_part_x, sp->nb_part_y, sp->nb_part_z,
     (sp->ptmax.x - sp->ptmin.x) / (sp->nb_part_x * 1.0),
     (sp->ptmax.y - sp->ptmin.y) / (sp->nb_part_y * 1.0),
     (sp->ptmax.z - sp->ptmin.z) / (sp->nb_part_z * 1.0));
  for (int i = 0; i < sp->nb_part_x; i++)
    for (int j = 0; j < sp->nb_part_y; j++)
      for (int k = 0; k < sp->nb_part_z; k++)
	printf ("case (%d,%d,%d) : %d points\n\n", i, j, k,
		sp->x[i].y[j].z[k].nb_point);
}

/**
   Affichage detaillé d'une partition de l'espace
   @param sp la partition de l'espace
   @return aucun
*/
void
space_partition_display_high_detail (
				     const space_partition * const sp)
{
  for (int i = 0; i < sp->nb_part_x; i++)
    for (int j = 0; j < sp->nb_part_y; j++)
      for (int k = 0; k < sp->nb_part_z; k++)
	{
	  printf ("case (%d,%d,%d) : %d points\n", i, j, k,
		  sp->x[i].y[j].z[k].nb_point);
	  for (int l = 0; l < sp->x[i].y[j].z[k].nb_point; l++)
	    printf ("(%lf , %lf , %lf) - ",
		    sp->x[i].y[j].z[k].list_point[l].x,
		    sp->x[i].y[j].z[k].list_point[l].y,
		    sp->x[i].y[j].z[k].list_point[l].z);
	  printf ("\n\n");
	}
}

/**
   Calcule les numeros de cases du point
   @param sp la partition de l'espace
   @param p le point
   @param x le numéro de case en X
   @param y le numéro de case en Y
   @param z le numéro de case en Z
   @return aucun
**/
void
space_partition_get_XYZ (
			 const space_partition * const sp,
			 const point3d * const p,
			 int *x,
			 int *y,
			 int *z)
{
  point3d ptemp;
  double dx = (sp->ptmax.x - sp->ptmin.x) / (sp->nb_part_x * 1.0);
  double dy = (sp->ptmax.y - sp->ptmin.y) / (sp->nb_part_y * 1.0);
  double dz = (sp->ptmax.z - sp->ptmin.z) / (sp->nb_part_z * 1.0);
  int cellx,
    celly,
    cellz;

  point3d_init (&ptemp, p->x - sp->ptmin.x, p->y - sp->ptmin.y,
		p->z - sp->ptmin.z);

  if (dx != 0)
    cellx = ptemp.x / dx;
  else
    cellx = 0;
  if (cellx == sp->nb_part_x)
    cellx--;

  if (dy != 0)
    celly = ptemp.y / dy;
  else
    celly = 0;
  if (celly == sp->nb_part_y)
    celly--;

  if (dz != 0)
    cellz = ptemp.z / dz;
  else
    cellz = 0;
  if (cellz == sp->nb_part_z)
    cellz--;

  *x = cellx;
  *y = celly;
  *z = cellz;

}

/**
   Ajout d'un point dans la partition de l'espace
   @param sp la partition de l'espace
   @param p le point à  ajouter
   @return aucun
*/
void
space_partition_add_point (
			   space_partition * sp,
			   const point3d * const p)
{
  point3d ptemp;
  int cellx,
    celly,
    cellz;

  if (p->x > sp->ptmax.x || p->y > sp->ptmax.y || p->z > sp->ptmax.z)
    return;


  point3d_init (&ptemp, p->x - sp->ptmin.x, p->y - sp->ptmin.y,
		p->z - sp->ptmin.z);

  if (ptemp.x < 0 || ptemp.y < 0 || ptemp.z < 0)
    return;

  space_partition_get_XYZ (sp, p, &cellx, &celly, &cellz);

  sp->x[cellx].y[celly].z[cellz].list_point =
    (point3d *) realloc (sp->x[cellx].y[celly].z[cellz].list_point,
			 ((sp->x[cellx].y[celly].z[cellz].nb_point) +
			  1) * sizeof (point3d));
  a2ri_erreur_critique_si (sp->x[cellx].y[celly].z[cellz].list_point == NULL,
			"erreur allocation memoire pour sp->x[cellx].y[celly].z[cellz].list_point\nspace_partition_add_point");
  point3d_init (&
		(sp->x[cellx].y[celly].z[cellz].
		 list_point[sp->x[cellx].y[celly].z[cellz].nb_point]), p->x,
		p->y, p->z);
  sp->x[cellx].y[celly].z[cellz].list_point[sp->x[cellx].y[celly].z[cellz].
					    nb_point].att_int = p->att_int;
  sp->x[cellx].y[celly].z[cellz].nb_point++;

}

/**
   Calcule le point moyen d'une cellule
   @param sp la partition de l'espace
   @param cell_x numéro de la cellule en X
   @param cell_y numéro de la cellule en Y
   @param cell_z numéro de la cellule en Z
   @param aucun
*/
point3d
space_partition_get_average (
			     const space_partition * const sp,
			     int cell_x,
			     int cell_y,
			     int cell_z)
{
  point3d p;
  point3d_init (&p, 0, 0, 0);
  for (int i = 0; i < sp->x[cell_x].y[cell_y].z[cell_z].nb_point; i++)
    {
      point3d_init (&p,
		    p.x + sp->x[cell_x].y[cell_y].z[cell_z].list_point[i].x,
		    p.y + sp->x[cell_x].y[cell_y].z[cell_z].list_point[i].y,
		    p.z + sp->x[cell_x].y[cell_y].z[cell_z].list_point[i].z);
    }

  point3d_init (&p, p.x / (sp->x[cell_x].y[cell_y].z[cell_z].nb_point * 1.0),
		p.y / (sp->x[cell_x].y[cell_y].z[cell_z].nb_point * 1.0),
		p.z / (sp->x[cell_x].y[cell_y].z[cell_z].nb_point * 1.0));

  return (p);
}

/**
   Calcule le point milieu d'une cellule
   @param sp la partition de l'espace
   @param cell_x numéro de la cellule en X
   @param cell_y numéro de la cellule en Y
   @param cell_z numéro de la cellule en Z
   @param aucun
*/
point3d
space_partition_get_middle (
			    const space_partition * const sp,
			    int cell_x,
			    int cell_y,
			    int cell_z)
{
  point3d p;
  double midx =
    sp->ptmin.x +
    ((sp->ptmax.x - sp->ptmin.x) / (sp->nb_part_x * 1.0)) * (cell_x + 0.5);
  double midy =
    sp->ptmin.y +
    ((sp->ptmax.y - sp->ptmin.y) / (sp->nb_part_y * 1.0)) * (cell_y + 0.5);
  double midz =
    sp->ptmin.z +
    ((sp->ptmax.z - sp->ptmin.z) / (sp->nb_part_z * 1.0)) * (cell_z + 0.5);
  point3d_init (&p, midx, midy, midz);
  return (p);
}

/**
   Retourne le nombre total de points de la partition de l'espace
   @param sp la partition de l'espace
   @return le nombre de points
*/
int
space_partition_nb_points (
			   const space_partition * const sp)
{
  int total = 0;
  for (int i = 0; i < sp->nb_part_x; i++)
    for (int j = 0; j < sp->nb_part_y; j++)
      for (int k = 0; k < sp->nb_part_z; k++)
	total += sp->x[i].y[j].z[k].nb_point;

  return total;
}

void
add_sp_depth_to_list (
		      pt_sp_depth ** list,
		      int *size,
		      pt_sp_depth d)
{
  *list = (pt_sp_depth *) realloc (*list, (*size + 1) * sizeof (pt_sp_depth));
  a2ri_erreur_critique_si (*list == NULL,
			"erreur allocation memoire pour list\nadd_sp_depth_to_list");
  (*list)[*size] = d;
  *size = (*size) + 1;
}

/**
   Retourne les cases adjacentes à celle contenant le point passé en paramètre est à une distance maximum de distance
   @param sp la partition de l'espace
   @param pt le point servant à sélectionner la case centrale
   @param distance profondeur de la sélection
   @param list liste des cases sélectionnées
   @param size taille de la liste retournée
   @return aucun
**/
void
space_partition_get_neighbour (
			       const space_partition * const sp,
			       const point3d * const pt,
			       int distance,
			       pt_sp_depth ** list,
			       int *size)
{
  int cellx,
    celly,
    cellz;

  space_partition_get_XYZ (sp, pt, &cellx, &celly, &cellz);

  if (cellx < 0)
    cellx = 0;
  if (cellx >= sp->nb_part_x)
    cellx = sp->nb_part_x - 1;
  if (celly < 0)
    celly = 0;
  if (celly >= sp->nb_part_y)
    celly = sp->nb_part_y - 1;
  if (cellz < 0)
    cellz = 0;
  if (cellz >= sp->nb_part_z)
    cellz = sp->nb_part_z - 1;

  *list = NULL;
  *size = 0;

  while(distance>0)
    {      
      for (int i = -distance; i <= distance; i++)
	for (int j = -distance; j <= distance; j++)
	  {
	    if (cellx + i >= 0 && cellx + i < sp->nb_part_x && celly + j >= 0
		&& celly + j < sp->nb_part_y && cellz - distance >= 0)
	      add_sp_depth_to_list (list, size,
				    &(sp->x[cellx + i].y[celly + j].
				      z[cellz - distance]));
	    
	    if (cellx + i >= 0 && cellx + i < sp->nb_part_x && celly + j >= 0
		&& celly + j < sp->nb_part_y && cellz + distance < sp->nb_part_z)
	      add_sp_depth_to_list (list, size,
				    &(sp->x[cellx + i].y[celly + j].
				      z[cellz + distance]));
	  }
      
      for (int i = -distance + 1; i < distance; i++)
	for (int j = -distance; j <= distance; j++)
	  {
	    if (cellx - distance >= 0 && celly + j >= 0
		&& celly + j < sp->nb_part_y && cellz + i >= 0
		&& cellz + i < sp->nb_part_z)
	      add_sp_depth_to_list (list, size,
				    &(sp->x[cellx - distance].y[celly + j].
				      z[cellz + i]));
	    
	    if (cellx + distance < sp->nb_part_x && celly + j >= 0
		&& celly + j < sp->nb_part_y && cellz + i >= 0
		&& cellz + i < sp->nb_part_z)
	      add_sp_depth_to_list (list, size,
				    &(sp->x[cellx + distance].y[celly + j].
				      z[cellz + i]));
	  }
      
      for (int i = -distance + 1; i < distance; i++)
	for (int j = -distance + 1; j < distance; j++)
	  {
	    if (cellx + i >= 0 && cellx + i < sp->nb_part_x
		&& celly - distance >= 0 && cellz + j >= 0
		&& cellz + j < sp->nb_part_z)
	      add_sp_depth_to_list (list, size,
				    &(sp->x[cellx + i].y[celly - distance].
				      z[cellz + j]));
	    
	    if (cellx + i >= 0 && cellx + i < sp->nb_part_x
		&& celly + distance < sp->nb_part_y && cellz + j >= 0
		&& cellz + j < sp->nb_part_z)
	      add_sp_depth_to_list (list, size,
				    &(sp->x[cellx + i].y[celly + distance].
				      z[cellz + j]));
	  }
      distance--;
    }

  add_sp_depth_to_list (list, size, &(sp->x[cellx].y[celly].z[cellz]));
  
}

/**
   Retourne le point le plus proche d'un point donné
   @param sp la partition de l'espace
   @param pt le point dont on cherche le point le plus proche dans la partition de l'espace
   @param near_pt point le plus proche de la partition de l'espace
   @param length longueur entre les deux points
   @return un pointeur vers le point de la partition le plus paroche de celui recherche
*/
point3d *
space_partition_nearest_point (
			       const space_partition * const sp,
			       const point3d * const pt,
			       point3d * near_pt,
			       double *length,
			       int acceptzerolength)
{
  int trouve = 0,
    oncontinue = 1,
    xinf,
    xsup,
    yinf,
    ysup,
    zinf,
    zsup;
  int dist = -1;
  pt_sp_depth *list = NULL;
  int size = 0;
  point3d temp,
    *retour = NULL;

  if (space_partition_nb_points (sp) == 0)
    return NULL;


  while (oncontinue)
    {
      dist++;
      free (list);
      size = 0;
      oncontinue = 1;
      space_partition_get_neighbour (sp, pt, dist, &list, &size);
      int x,
        y,
        z;
      space_partition_get_XYZ (sp, pt, &x, &y, &z);
      for (int i = 0; i < size; i++)
	if ((*(list[i])).nb_point != 0)
	  {
	    for (int j = 0; j < (*(list[i])).nb_point; j++)
	      {
		point3d_init (&temp, (*(list[i])).list_point[j].x,
			      (*(list[i])).list_point[j].y,
			      (*(list[i])).list_point[j].z);
		temp.att_int = (*(list[i])).list_point[j].att_int;
		if (!trouve
		    && (point3d_length (pt, &temp) != 0
			|| acceptzerolength == ACCEPT_ZERO_LENGTH))
		  {
		    trouve = 1;
		    *length = point3d_length (pt, &temp);
		    point3d_init (near_pt, temp.x, temp.y, temp.z);
		    near_pt->att_int = temp.att_int;
		    retour = &(*(list[i])).list_point[j];
		  }
		else
		  {
		    if (*length > point3d_length (pt, &temp)
			&& (point3d_length (pt, &temp) != 0
			    || acceptzerolength == ACCEPT_ZERO_LENGTH))
		      {
			*length = point3d_length (pt, &temp);
			point3d_init (near_pt, temp.x, temp.y, temp.z);
			near_pt->att_int = temp.att_int;
			retour = &(*(list[i])).list_point[j];
		      }
		  }
	      }
	  }

      if (trouve)
	{
	  double dx = (sp->ptmax.x - sp->ptmin.x) / (sp->nb_part_x * 1.0);
	  double dy = (sp->ptmax.y - sp->ptmin.y) / (sp->nb_part_y * 1.0);
	  double dz = (sp->ptmax.z - sp->ptmin.z) / (sp->nb_part_z * 1.0);
	  point3d coin;

	  oncontinue = 0;
	  space_partition_get_XYZ (sp, pt, &x, &y, &z);

	  xinf = x - dist;
	  if (xinf < 0)
	    xinf = 0;

	  xsup = x + dist;
	  if (xsup >= sp->nb_part_x)
	    xsup = sp->nb_part_x - 1;

	  yinf = y - dist;
	  if (yinf < 0)
	    yinf = 0;

	  ysup = y + dist;
	  if (ysup >= sp->nb_part_y)
	    ysup = sp->nb_part_y - 1;

	  zinf = z - dist;
	  if (zinf < 0)
	    zinf = 0;

	  zsup = z + dist;
	  if (zsup >= sp->nb_part_z)
	    zsup = sp->nb_part_z - 1;


	  point3d_init (&coin, sp->ptmin.x + dx * xinf, pt->y, pt->z);
	  oncontinue = oncontinue || (point3d_length (pt, &coin) < *length
				      && xinf != 0);

	  point3d_init (&coin, sp->ptmin.x + dx * (xsup + 1), pt->y, pt->z);
	  oncontinue = oncontinue || (point3d_length (pt, &coin) < *length
				      && xsup + 1 != sp->nb_part_x);

	  point3d_init (&coin, pt->x, sp->ptmin.y + dy * yinf, pt->z);
	  oncontinue = oncontinue || (point3d_length (pt, &coin) < *length
				      && yinf != 0);

	  point3d_init (&coin, pt->x, sp->ptmin.y + dy * (ysup + 1), pt->z);
	  oncontinue = oncontinue || (point3d_length (pt, &coin) < *length
				      && ysup + 1 != sp->nb_part_y);

	  point3d_init (&coin, pt->x, pt->y, sp->ptmin.z + dz * zinf);
	  oncontinue = oncontinue || (point3d_length (pt, &coin) < *length
				      && zinf != 0);

	  point3d_init (&coin, pt->x, pt->y, sp->ptmin.z + dz * (zsup + 1));
	  oncontinue = oncontinue || (point3d_length (pt, &coin) < *length
				      && zsup + 1 != sp->nb_part_z);

	}
    }

  free (list);

  return retour;
}

/**
   Retourne un pointeur vers le point de la partition s'il existe
   @param sp la partition de l'espace
   @param pt le point que l'on cherche dans la partition
   @return un pointeur vers le point cherché, NULL sinon
*/
point3d * space_partition_contains_point (
					const space_partition * const sp,
					const point3d * const pt)
{
  if (space_partition_nb_points (sp) == 0)
    return NULL;
  
  int x,
    y,
    z;
  
  space_partition_get_XYZ (sp, pt, &x, &y, &z);
  
  for (int i = 0; i < sp->x[x].y[y].z[z].nb_point; i++)
    if(point3d_equal(&(sp->x[x].y[y].z[z].list_point[i]),pt))
      return &(sp->x[x].y[y].z[z].list_point[i]);       

  return NULL;
}

/**
   Enlève les outliers
   @param sp la partition de l'espace
   @return aucun
*/
void
space_partition_remove_outliers (
				 space_partition * sp)
{
  /*nimporte quoi */
  /*TODO à coder*/
  point3d p;
  p = space_partition_get_average (sp, 0, 0, 0);
  point3d_init(&p,0,0,0);
}


void
add_point3d_to_list (
		     pt_point3d ** list,
		     int *size,
		     pt_point3d p)
{
  *list = (pt_point3d *) realloc (*list, ((*size) + 1) * sizeof (pt_point3d));
  a2ri_erreur_critique_si (*list == NULL,
			"erreur allocation memoire pour list\nadd_point3d_to_list");
  (*list)[*size] = p;
  *size = (*size) + 1;
}


/**
   retourne une liste de pointeur sur des point3d contenus dans les cellules passées en paramètre
   @param sp la partition de l'espace
   @param listcellx liste des index des cellules en X
   @param listcelly liste des index des cellules en Y
   @param listcellz liste des index des cellules en Z
   @param sizelist taille de la liste des cellules
   @param listpoint liste des pointeurs vers les point3d
   @param sizelistpoint taille de la liste de pointeur
   @return aucun
**/
void
space_partition_get_points_cells (
				  const space_partition * const sp,
				  const int * const listcellx,
				  const int * const listcelly,
				  const int * const listcellz,
				  int sizelist,
				  pt_point3d ** listpoint,
				  int *sizelistpoint)
{
  *sizelistpoint = 0;
  *listpoint = NULL;
  for (int i = 0; i < sizelist; i++)
    {
      for (int j = 0;
	   j < sp->x[listcellx[i]].y[listcelly[i]].z[listcellz[i]].nb_point;
	   j++)
	add_point3d_to_list (listpoint, sizelistpoint,
			     &(sp->x[listcellx[i]].y[listcelly[i]].
			       z[listcellz[i]].list_point[j]));
    }
}


/**
   Calcule la Bounding Box de la liste de points
   @param sp la partition de l'epace
   @param list la list de point
   @param size nombre de points
   @param xmin cellule minimale en X
   @param ymin cellule minimale en Y
   @param zmin cellule minimale en Z
   @param xmax cellule maximale en X
   @param ymax cellule maximale en Y
   @param zmax cellule maximale en Z
**/
void
space_partition_bounding_box (
			      const space_partition * const sp,
			      const point3d * const list,
			      int size,
			      int *xmin,
			      int *ymin,
			      int *zmin,
			      int *xmax,
			      int *ymax,
			      int *zmax)
{
  int *listx,
    *listy,
    *listz,
    x,
    y,
    z;

  listx = (int *) malloc (size * sizeof (int));
  a2ri_erreur_critique_si (listx == NULL,
			"erreur allocation memoire pour listx\nspace_partition_bounding_box");
  listy = (int *) malloc (size * sizeof (int));
  a2ri_erreur_critique_si (listy == NULL,
			"erreur allocation memoire pour listy\nspace_partition_bounding_box");
  listz = (int *) malloc (size * sizeof (int));
  a2ri_erreur_critique_si (listz == NULL,
			"erreur allocation memoire pour listz\nspace_partition_bounding_box");

  for (int i = 0; i < size; i++)
    {
      space_partition_get_XYZ (sp, &(list[i]), &x, &y, &z);
      if (x < 0)
	x = 0;
      if (y < 0)
	y = 0;
      if (z < 0)
	z = 0;
      if (x >= sp->nb_part_x)
	x = sp->nb_part_x - 1;
      if (y >= sp->nb_part_y)
	y = sp->nb_part_y - 1;
      if (z >= sp->nb_part_z)
	z = sp->nb_part_z - 1;
      listx[i] = x;
      listy[i] = y;
      listz[i] = z;
    }

  list_int_sort (listx, size, ASC);
  list_int_sort (listy, size, ASC);
  list_int_sort (listz, size, ASC);

  *xmin = listx[0];
  *xmax = listx[size - 1];
  *ymin = listy[0];
  *ymax = listy[size - 1];
  *zmin = listz[0];
  *zmax = listz[size - 1];

}

/**
   Retourne les cases adjacentes et elle-mÃªme à celle contenant le point passé en paramètre est à une distance maximum de distance
   @param sp la partition de l'espace
   @param pt le point servant à sélectionner la case centrale
   @param list liste des cases sélectionnées
   @param size taille de la liste retournée
   @return aucun
**/
void
space_partition_get_list_near_point (
				     const space_partition * const sp,
				     const point3d * const pt,
				     pt_sp_depth ** list,
				     int *size)
{

  int cellx,
    celly,
    cellz;

  space_partition_get_XYZ (sp, pt, &cellx, &celly, &cellz);

  if (cellx < 0)
    cellx = 0;
  if (cellx >= sp->nb_part_x)
    cellx = sp->nb_part_x - 1;
  if (celly < 0)
    celly = 0;
  if (celly >= sp->nb_part_y)
    celly = sp->nb_part_y - 1;
  if (cellz < 0)
    cellz = 0;
  if (cellz >= sp->nb_part_z)
    cellz = sp->nb_part_z - 1;

  space_partition_get_neighbour (sp, pt, 1, list, size);

  add_sp_depth_to_list (list, size, &(sp->x[cellx].y[celly].z[cellz]));
}
