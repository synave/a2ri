/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 10/03/10       */
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

#include "io.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

void
IFaffichage_chargement (
		      FILE * is,
		      long posmax)
{
  long poscur;

  poscur = ftell (is);
  printf ("%c%c%c%c%c%c%c%c%c%c%c%c%c%lf %c", 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	  8, 8, ((poscur) * 100.0) / (posmax * 1.0), 37);
}

//fonction qui ecrit les aretes dans un fichier vef
void IFsave_vef_write_edge (
			  const vf_model * const m,
			  hashtable * table,
			  FILE * os)
{
  int p1,
    p2;
  int indexarete = 0;
  
  //pour toutes les faces, on regarde si l'arete existe deja dans la table de hachage
  //si oui, on ne fait rien 
  //sinon on entre l'arete dans la table et on l'ecrit dans le fichier. 
  //on enregistre également son index pour l'écriture des faces définies par trois aretes
  for (int i = 0; i < m->nbface; i++)
    {
      p1 = m->fa[i].ve1;
      p2 = m->fa[i].ve2;
      vf_edge *edgetemp = hashtable_look_for (table, p1, p2);
      if (edgetemp == NULL)
	{
	  edgetemp = (vf_edge *) malloc (sizeof (vf_edge));
	  a2ri_erreur_critique_si (edgetemp == NULL,
				"erreur allocation memoire pour edgetemp\nIFsave_vef_write_edge");
	  edgetemp->ve1 = p1;
	  edgetemp->ve2 = p2;
	  edgetemp->nbsharedfaces=0;
	  edgetemp->sharedfaces=NULL;
	  edgetemp->att_int = indexarete;
	  indexarete++;
	  fprintf (os, "%d %d\n", p1, p2);
	  hashtable_add (table, edgetemp);
	}
      
      p1 = m->fa[i].ve2;
      p2 = m->fa[i].ve3;
      edgetemp = hashtable_look_for (table, p1, p2);
      if (edgetemp == NULL)
	{
	  edgetemp = (vf_edge *) malloc (sizeof (vf_edge));
	  a2ri_erreur_critique_si (edgetemp == NULL,
				"erreur allocation memoire pour edgetemp\nIFsave_vef_write_edge");
	  edgetemp->ve1 = p1;
	  edgetemp->ve2 = p2;
	  edgetemp->nbsharedfaces=0;
	  edgetemp->sharedfaces=NULL;
	  edgetemp->att_int = indexarete;
	  indexarete++;
	  fprintf (os, "%d %d\n", p1, p2);
	  hashtable_add (table, edgetemp);
	}
      
      p1 = m->fa[i].ve3;
      p2 = m->fa[i].ve1;
      edgetemp = hashtable_look_for (table, p1, p2);
      if (edgetemp == NULL)
	{
	  edgetemp = (vf_edge *) malloc (sizeof (vf_edge));
	  a2ri_erreur_critique_si (edgetemp == NULL,
				"erreur allocation memoire pour edgetemp\nIFsave_vef_write_edge");
	  edgetemp->ve1 = p1;
	  edgetemp->ve2 = p2;
	  edgetemp->nbsharedfaces=0;
	  edgetemp->sharedfaces=NULL;
	  edgetemp->att_int = indexarete;
	  indexarete++;
	  fprintf (os, "%d %d\n", p1, p2);
	  hashtable_add (table, edgetemp);
	}
    }
}

//fonction qui ecrit les aretes dans un fichier gts
void IFsave_gts_write_edge (
			  const vf_model * const m,
			  hashtable * table,
			  FILE * os)
{
  int p1,
    p2;
  int indexarete = 0;
  
  //pour toute les faces, on regarde si l'arete existe deja dans la table de hachage
  //si oui, on ne fait rien 
  //sinon on entre l'arete dans la table et on l'ecrit dans le fichier. 
  //on enregistre également son index pour l'écriture des faces définies par trois aretes
  for (int i = 0; i < m->nbface; i++)
    {
      p1 = m->fa[i].ve1;
      p2 = m->fa[i].ve2;
      vf_edge *edgetemp = hashtable_look_for (table, p1, p2);
      if (edgetemp == NULL)
	{
	  edgetemp = (vf_edge *) malloc (sizeof (vf_edge));
	  a2ri_erreur_critique_si (edgetemp == NULL,
				"erreur allocation memoire pour edgetemp\nIFsave_gts_write_edge");
	  edgetemp->ve1 = p1;
	  edgetemp->ve2 = p2;
	  edgetemp->att_int = indexarete;
	  indexarete++;
	  fprintf (os, "%d %d\n", p1 + 1, p2 + 1);
	  hashtable_add (table, edgetemp);
	}
      
      p1 = m->fa[i].ve2;
      p2 = m->fa[i].ve3;
      edgetemp = hashtable_look_for (table, p1, p2);
      if (edgetemp == NULL)
	{
	  edgetemp = (vf_edge *) malloc (sizeof (vf_edge));
	  a2ri_erreur_critique_si (edgetemp == NULL,
				"erreur allocation memoire pour edgetemp\nIFsave_gts_write_edge");
	  edgetemp->ve1 = p1;
	  edgetemp->ve2 = p2;
	  edgetemp->att_int = indexarete;
	  indexarete++;
	  fprintf (os, "%d %d\n", p1 + 1, p2 + 1);
	  hashtable_add (table, edgetemp);
	}
      
      p1 = m->fa[i].ve3;
      p2 = m->fa[i].ve1;
      edgetemp = hashtable_look_for (table, p1, p2);
      if (edgetemp == NULL)
	{
	  edgetemp = (vf_edge *) malloc (sizeof (vf_edge));
	  a2ri_erreur_critique_si (edgetemp == NULL,
				"erreur allocation memoire pour edgetemp\nIFsave_gts_write_edge");
	  edgetemp->ve1 = p1;
	  edgetemp->ve2 = p2;
	  edgetemp->att_int = indexarete;
	  indexarete++;
	  fprintf (os, "%d %d\n", p1 + 1, p2 + 1);
	  hashtable_add (table, edgetemp);
	}
    }
}

/********** MAIN FUNCTIONS **********/

/**
   Ouverture d'un fichier représentant un squelette
   @param file chemin vers le fichier à ouvrir
   @param s pointeur vers le squelette
   @return 1 si succès, 0 sinon
*/
int a2ri_skeleton_open_file(
			   const char * const file,
			   skeleton * s)
{
  //TODO
  char ext[5];
  FILE *is = NULL;
  int nbvertex,
    nbedge,
    ve1,
    ve2;
  double coordx,
    coordy,
    coordz;
  
  memcpy (ext, file + (strlen (file) - 4), 4);
  ext[4] = 0;
  
  //appel de la bonne fonction en fonction de l'extension du fichier
  if (strcmp (ext, ".skl"))
    return 0;

  setlocale(LC_NUMERIC,"C");
  
  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  //lecture du nombre de sommets et de faces
  a2ri_erreur_critique_si(!fscanf (is, "%d %d", &nbvertex, &nbedge),"erreur en ouverture de fichier");

  //lecture des sommets
  for (int i = 0; i < nbvertex; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf %lf %lf", &coordx, &coordy, &coordz),"erreur en ouverture de fichier");
      a2ri_skeleton_add_vertex (s, coordx, coordy, coordz);
    }

  //lecture des arêtes
  for (int i = 0; i < nbedge; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%d %d", &ve1, &ve2),"erreur en ouverture de fichier");
      a2ri_skeleton_add_edge (s, ve1, ve2);
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;
  
}

/**
   Ouverture d'un fichier contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_open_file (
		   const char * const file,
		   vf_model * m)
{
  char ext[5];
  
  memcpy (ext, file + (strlen (file) - 4), 4);
  ext[4] = 0;

  //appel de la bonne fonction en fonction de l'extension du fichier
  if (!strcmp (ext, ".wrl"))
    return (a2ri_vf_load_wrl (file, m));

  if (!strcmp (ext, ".asc"))
    return (a2ri_vf_load_asc (file, m));

  if (!strcmp (ext, ".vef"))
    return (a2ri_vf_load_vef (file, m));

  if (!strcmp (ext, ".pgn"))
    return (a2ri_vf_load_pgn (file, m));

  if (!strcmp (ext, ".ply"))
    return (a2ri_vf_load_ply (file, m));

  if (!strcmp (ext, ".off"))
    return (a2ri_vf_load_off (file, m));

  if (!strcmp (ext, ".obj"))
    return (a2ri_vf_load_obj (file, m));

  if (!strcmp (ext, ".gts"))
    return (a2ri_vf_load_gts (file, m));

  if (!strcmp (ext, ".stl"))
    return (a2ri_vf_load_stl (file, m));

  return 0;

}

/**
   Ouverture d'un fichier asc contenant un nuage de point
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_load_asc (
		  const char * const file,
		  vf_model * m)
{

  FILE *is = NULL;
  double coordx,
    coordy,
    coordz;
  int i;

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  i=0;
  a2ri_erreur_critique_si(!fscanf (is, "%lf %lf %lf", &coordx, &coordy, &coordz),"erreur en ouverture de fichier");
  while(!feof(is))
    {
      //fscanf (is, "%lf %lf %lf", &coordx, &coordy, &coordz),"erreur en ouverture de fichier");
      a2ri_vf_add_vertex (m, coordx, coordy, coordz);
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  //mis à jour de la bounding box du modèle
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
      a2ri_erreur_critique_si(!fscanf (is, "%lf %lf %lf", &coordx, &coordy, &coordz),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf %lf %lf", &coordx, &coordy, &coordz),"erreur en ouverture de fichier");
      i=1;
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;

}

/**
   Ouverture d'un fichier pgn contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_load_pgn (
		  const char * const file,
		  vf_model * m)
{

  FILE *is = NULL;
  int nbvertex,
    nbface,
    ve1,
    ve2,
    ve3;
  double coordx,
    coordy,
    coordz;

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  //lecture du nombre de sommets et de faces
  a2ri_erreur_critique_si(!fscanf (is, "%d %d", &nbvertex, &nbface),"erreur en ouverture de fichier");

  //lecture des sommets
  for (int i = 0; i < nbvertex; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf %lf %lf", &coordx, &coordy, &coordz),"erreur en ouverture de fichier");
      a2ri_vf_add_vertex (m, coordx, coordy, coordz);
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  //mis à jour de la bounding box du modèle
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
    }

  //lecture des faces
  for (int i = 0; i < nbface; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%d %d %d", &ve1, &ve2, &ve3),"erreur en ouverture de fichier");
      a2ri_vf_add_face (m, ve1, ve2, ve3);

      a2ri_erreur_critique_si(!fscanf (is, "%d", &ve1),"erreur en ouverture de fichier");
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;

}

/**
   Ouverture d'un fichier obj contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_load_obj (
		  const char * const file,
		  vf_model * m)
{

  FILE *is = NULL;
  int ve1,
    ve2,
    ve3;
  double coordx,
    coordy,
    coordz;
  char str[250],
    str_ve1[20],
    str_ve2[20],
    str_ve3[20];

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");


  while (strcmp (str, "v") != 0)
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  //lecture des sommets
  while (strcmp (str, "v") == 0  && !feof (is))
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf %lf %lf", &coordx, &coordy, &coordz),"erreur en ouverture de fichier");
      //ajout d'un sommet
      a2ri_vf_add_vertex (m, coordx, coordy, coordz);
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      //mis à jour de la bounding box
      if (m->nbvertex == 1)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
    }

  while (strcmp (str, "f") != 0  && !feof (is))
    {
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
    }

  //lecture des faces
  while (strcmp (str, "f") == 0 && !feof (is))
    {
      a2ri_erreur_critique_si(!fscanf (is, "%s %s %s", str_ve1, str_ve2, str_ve3),"erreur en ouverture de fichier");
      ve1 = atoi (str_ve1);
      ve2 = atoi (str_ve2);
      ve3 = atoi (str_ve3);
      ve1--;
      ve2--;
      ve3--;
      //ajout d'une face
      a2ri_vf_add_face (m, ve1, ve2, ve3);

      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      if (str[0] >= '0' && str[0] <= '9')
	{
	  ve2 = atoi (str);
	  ve2--;
	  a2ri_vf_add_face (m, ve3, ve2, ve1);
	  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
	}
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;

}

/**
   Ouverture d'un fichier wrl contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_load_wrl1 (
		   const char * const file,
		   vf_model * m)
{
  FILE *is = NULL;
  char str[20];
  char coordx[15],
    coordy[15],
    coordz[15];

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  while (strcmp (str, "point") && strcmp (str, "point["))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  if (!strcmp (str, "point"))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  //lecture des sommets
  a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
  int i = 0;
  while (strcmp (coordx, "}") && strcmp (coordx, "]"))
    {
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordy),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordz),"erreur en ouverture de fichier");
      if (coordz[strlen (coordz) - 1] == ',')
	{
	  memcpy (coordz, coordz, strlen (coordz) - 1);
	  coordz[strlen (coordz)] = '\0';
	}
      //ajout d'un sommet
      a2ri_vf_add_vertex (m, atof (coordx), atof (coordy), atof (coordz));
      //mis à jour de la bounding box
      if (i == 0)
	{
	  i++;
	  m->xmin = atof (coordx);
	  m->xmax = atof (coordx);
	  m->ymin = atof (coordy);
	  m->ymax = atof (coordy);
	  m->zmin = atof (coordz);
	  m->zmax = atof (coordz);
	}
      else
	{
	  if (atof (coordx) < m->xmin)
	    m->xmin = atof (coordx);
	  if (atof (coordx) > m->xmax)
	    m->xmax = atof (coordx);
	  if (atof (coordy) < m->ymin)
	    m->ymin = atof (coordy);
	  if (atof (coordy) > m->ymax)
	    m->ymax = atof (coordy);
	  if (atof (coordz) < m->zmin)
	    m->zmin = atof (coordz);
	  if (atof (coordz) > m->zmax)
	    m->zmax = atof (coordz);
	}
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
    }

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  while (strcmp (str, "coordIndex") && strcmp (str, "coordIndex["))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  if (!strcmp (str, "coordIndex"))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  //lecture des faces
  a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
  while (strcmp (coordx, "}") && strcmp (coordx, "]"))
    {
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordy),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordz),"erreur en ouverture de fichier");

      if (coordx[strlen (coordx) - 1] == ',')
	{
	  memcpy (coordx, coordx, strlen (coordx) - 1);
	  coordz[strlen (coordx)] = '\0';
	}
      if (coordy[strlen (coordy) - 1] == ',')
	{
	  memcpy (coordy, coordy, strlen (coordy) - 1);
	  coordy[strlen (coordy)] = '\0';
	}
      if (coordz[strlen (coordz) - 1] == ',')
	{
	  memcpy (coordz, coordz, strlen (coordz) - 1);
	  coordz[strlen (coordz)] = '\0';
	}
      //ajout d'une face
      a2ri_vf_add_face (m, atoi (coordx), atoi (coordy), atoi (coordz));
      // reading the -1
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordy),"erreur en ouverture de fichier");

      if (coordy[strlen (coordy) - 1] == ',')
	{
	  memcpy (coordy, coordy, strlen (coordy) - 1);
	  coordy[strlen (coordy)] = '\0';
	}
      if (atoi (coordy) != -1)
	{
	  a2ri_vf_add_face (m, atoi (coordz), atoi (coordy), atoi (coordx));
	  a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
	}


      a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Ouverture d'un fichier wrl contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_load_wrl2 (
		   const char * const file,
		   vf_model * m)
{
  FILE *is = NULL;
  char str[20];
  char coordx[15],
    coordy[15],
    coordz[15];

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  while (strcmp (str, "point") && strcmp (str, "point["))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  if (!strcmp (str, "point"))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  //lecture des sommets
  a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
  int i = 0;
  while (strcmp (coordx, "}") && strcmp (coordx, "]"))
    {
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordy),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordz),"erreur en ouverture de fichier");

      if (coordz[strlen (coordz) - 1] == ',')
	{
	  memcpy (coordz, coordz, strlen (coordz) - 1);
	  coordz[strlen (coordz)] = '\0';
	}
      //ajout d'un sommet
      a2ri_vf_add_vertex (m, atof (coordx), atof (coordy), atof (coordz));
      //mis à jour de la bounding box
      if (i == 0)
	{
	  i++;
	  m->xmin = atof (coordx);
	  m->xmax = atof (coordx);
	  m->ymin = atof (coordy);
	  m->ymax = atof (coordy);
	  m->zmin = atof (coordz);
	  m->zmax = atof (coordz);
	}
      else
	{
	  if (atof (coordx) < m->xmin)
	    m->xmin = atof (coordx);
	  if (atof (coordx) > m->xmax)
	    m->xmax = atof (coordx);
	  if (atof (coordy) < m->ymin)
	    m->ymin = atof (coordy);
	  if (atof (coordy) > m->ymax)
	    m->ymax = atof (coordy);
	  if (atof (coordz) < m->zmin)
	    m->zmin = atof (coordz);
	  if (atof (coordz) > m->zmax)
	    m->zmax = atof (coordz);
	}
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
    }

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  while (strcmp (str, "coordIndex") && strcmp (str, "coordIndex["))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  if (!strcmp (str, "coordIndex"))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  //lecture des faces
  a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
  while (strcmp (coordx, "}") && strcmp (coordx, "]"))
    {
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordy),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordz),"erreur en ouverture de fichier");
      //ajout d'une face
      a2ri_vf_add_face (m, atoi (coordx), atoi (coordy), atoi (coordz));
      // reading the -1
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
    }

  fclose (is);
  
  setlocale(LC_NUMERIC,"");

  return 1;
}


/**
   Ouverture d'un fichier wrl contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_load_wrl (
		  const char * const file,
		  vf_model * m)
{
  FILE *is = NULL;
  char str[20];

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  if (!strcmp (str, "v1.0") || !strcmp (str, "V1.0"))
    return a2ri_vf_load_wrl1 (file, m);

  if (!strcmp (str, "v2.0") || !strcmp (str, "V2.0"))
    return a2ri_vf_load_wrl2 (file, m);

  return 0;
}

/**
   Ouverture d'un fichier vef contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_load_vef (
		  const char * const file,
		  vf_model * m)
{
  FILE *is = NULL;
  int nbvertex,
    nbedge,
    nbface,
    ve1,
    ve2,
    ve3,
    ed1,
    ed2,
    ed3;
  double coordx,
    coordy,
    coordz;

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_vf_init (m);

  //lecture du nombre de sommets
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbvertex),"erreur en ouverture de fichier");

  //lecture du nombre d'aretes
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbedge),"erreur en ouverture de fichier");

  //lecture du nombre de faces
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbface),"erreur en ouverture de fichier");


  //lecture des sommets
  for (int i = 0; i < nbvertex; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordx),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordy),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordz),"erreur en ouverture de fichier");
      //ajout d'un sommet
      a2ri_vf_add_vertex (m, coordx, coordy, coordz);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
    }

  //lecture des aretes et enregistrement dans un tableau temporaire
  vf_edge *temp = (vf_edge *) malloc (nbedge * sizeof (vf_edge));
  a2ri_erreur_critique_si (temp == NULL,
			"erreur allocation memoire pour temp\na2ri_vf_load_vef");
  for (int i = 0; i < nbedge; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ve1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ve2),"erreur en ouverture de fichier");
      temp[i].ve1 = ve1;
      temp[i].ve2 = ve2;
    }

  //lecture des faces
  for (int i = 0; i < nbface; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ed1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ed2),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ed3),"erreur en ouverture de fichier");

      //recherche des trois sommets contenu dans les trois aretes
      if (temp[ed1].ve1 == temp[ed2].ve1 || temp[ed1].ve1 == temp[ed2].ve2)
	{
	  ve1 = temp[ed1].ve1;
	  ve3 = temp[ed1].ve2;
	}
      else
	{
	  ve1 = temp[ed1].ve2;
	  ve3 = temp[ed1].ve1;
	}

      if (temp[ed2].ve1 == temp[ed3].ve1 || temp[ed2].ve1 == temp[ed3].ve2)
	ve2 = temp[ed2].ve1;
      else
	ve2 = temp[ed2].ve2;

      //ajout de la face
      a2ri_vf_add_face (m, ve1, ve2, ve3);
    }

  fclose (is);
  
  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Ouverture d'un fichier gts contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_load_gts (
		  const char * const file,
		  vf_model * m)
{
  FILE *is = NULL;
  int nbvertex,
    nbedge,
    nbface,
    ve1,
    ve2,
    ve3,
    ed1,
    ed2,
    ed3;
  double coordx,
    coordy,
    coordz;

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_vf_init (m);

  //lecture du nombre de sommets
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbvertex),"erreur en ouverture de fichier");

  //lecture du nombre d'aretes
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbedge),"erreur en ouverture de fichier");

  //lecture du nombre de faces
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbface),"erreur en ouverture de fichier");


  //lecture des sommets
  for (int i = 0; i < nbvertex; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordx),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordy),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordz),"erreur en ouverture de fichier");
      //ajout d'un sommet
      a2ri_vf_add_vertex (m, coordx, coordy, coordz);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
    }

  //lecture des aretes et enregistrement dans un tableau temporaire
  vf_edge *temp = (vf_edge *) malloc (nbedge * sizeof (vf_edge));
  a2ri_erreur_critique_si (temp == NULL,
			"erreur allocation memoire pour temp\na2ri_vf_load_gts");
  for (int i = 0; i < nbedge; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ve1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ve2),"erreur en ouverture de fichier");
      ve1--;
      ve2--;
      temp[i].ve1 = ve1;
      temp[i].ve2 = ve2;
    }

  //lecture des faces
  for (int i = 0; i < nbface; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ed1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ed2),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ed3),"erreur en ouverture de fichier");
      ed1--;
      ed2--;
      ed3--;

      //recherche des trois sommets contenu dans les trois aretes
      if (temp[ed1].ve1 == temp[ed2].ve1 || temp[ed1].ve1 == temp[ed2].ve2)
	{
	  ve1 = temp[ed1].ve1;
	  ve3 = temp[ed1].ve2;
	}
      else
	{
	  ve1 = temp[ed1].ve2;
	  ve3 = temp[ed1].ve1;
	}

      if (temp[ed2].ve1 == temp[ed3].ve1 || temp[ed2].ve1 == temp[ed3].ve2)
	ve2 = temp[ed2].ve1;
      else
	ve2 = temp[ed2].ve2;

      //ajout de la face
      a2ri_vf_add_face (m, ve1, ve2, ve3);
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Ouverture d'un fichier ply contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_load_ply (
		  const char * const file,
		  vf_model * m)
{
  int prop = 0,
    nbpropvertex = 0,
    nbpropface = 0,
    nbvertex,
    nbface,
    ve1,
    ve2,
    ve3;
  char str[20];
  double coordx,
    coordy,
    coordz;

  FILE *is = NULL;

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  if (strcmp (str, "ply"))
    return 0;

  while (strcmp (str, "end_header"))
    {
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      if (prop == 1 && !strcmp (str, "property"))
	nbpropvertex++;
      if (prop == 2 && !strcmp (str, "property"))
	nbpropface++;

      if (!strcmp (str, "element"))
	{
	  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
	  //lecture du nombre de sommets
	  if (!strcmp (str, "vertex"))
	    {
	      prop = 1;
	      a2ri_erreur_critique_si(!fscanf (is, "%d", &nbvertex),"erreur en ouverture de fichier");
	    }
	  //lecture du nombre de face
	  else
	    {
	      if (!strcmp (str, "face"))
		{
		  prop = 1;
		  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbface),"erreur en ouverture de fichier");
		}
	    }
	}
    }

  //lecture des sommets
  for (int i = 0; i < nbvertex; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf %lf %lf", &coordx, &coordy, &coordz),"erreur en ouverture de fichier");
      //ajout d'un sommet
      a2ri_vf_add_vertex (m, coordx, coordy, coordz);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
      for (int j = 0; j < nbpropvertex - 4; j++)
	a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

    }

  //lecture des faces
  for (int i = 0; i < nbface; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ve1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d %d %d", &ve1, &ve2, &ve3),"erreur en ouverture de fichier");
      //ajout d'une face
      a2ri_vf_add_face (m, ve1, ve2, ve3);
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Ouverture d'un fichier off contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_load_off (
		  const char * const file,
		  vf_model * m)
{

  FILE *is = NULL;
  int nbvertex,
    nbface,
    ve1,
    ve2,
    ve3;
  double coordx,
    coordy,
    coordz;
  char str[250];

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  if (strcmp (str, "OFF"))
    return 0;

  //lecture du nombre de sommets et de faces
  a2ri_erreur_critique_si(!fscanf (is, "%d %d %d", &nbvertex, &nbface, &ve1),"erreur en ouverture de fichier");

  m->ve=(vf_vertex*)malloc(nbvertex*sizeof(vf_vertex));
  m->nbvertex=nbvertex;

  //lecture des sommets
  for (int i = 0; i < nbvertex; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf %lf %lf", &coordx, &coordy, &coordz),"erreur en ouverture de fichier");
      //ajout du sommet
      m->ve[i].x=coordx;m->ve[i].y=coordy;m->ve[i].z=coordz;
      m->ve[i].incidentvertices=NULL;
      m->ve[i].nbincidentvertices=0;
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
    }
  
  m->fa=(vf_face*)malloc(nbface*sizeof(vf_face));
  m->nbface=nbface;  
  
  //lecture des faces
  for (int i = 0; i < nbface; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ve1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d %d %d", &ve1, &ve2, &ve3),"erreur en ouverture de fichier");
      //ajout d'une face
      m->fa[i].ve1 = ve1;
      m->fa[i].ve2 = ve2;
      m->fa[i].ve3 = ve3;
      list_int_add (&(m->ve[ve1].incidentvertices),
		    &(m->ve[ve1].nbincidentvertices), ve2, WITHOUT_REDUNDANCE);
      list_int_add (&(m->ve[ve2].incidentvertices),
		    &(m->ve[ve2].nbincidentvertices), ve1, WITHOUT_REDUNDANCE);
      list_int_add (&(m->ve[ve1].incidentvertices),
		    &(m->ve[ve1].nbincidentvertices), ve3, WITHOUT_REDUNDANCE);
      list_int_add (&(m->ve[ve3].incidentvertices),
		    &(m->ve[ve3].nbincidentvertices), ve1, WITHOUT_REDUNDANCE);
      list_int_add (&(m->ve[ve3].incidentvertices),
		    &(m->ve[ve3].nbincidentvertices), ve2, WITHOUT_REDUNDANCE);
      list_int_add (&(m->ve[ve2].incidentvertices),
		    &(m->ve[ve2].nbincidentvertices), ve3, WITHOUT_REDUNDANCE);
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;

}

/**
   Ouverture d'un fichier stl contenant un modèle
   @param file chemin du fichier ÃÂ  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succÃÂ¨s, 0 sinon
*/
int
a2ri_vf_load_stl (
		  const char * const file,
		  vf_model * m)
{
  FILE *is = NULL;
  char str[250];

  is = fopen (file, "r");
  if (is == NULL)
    return 0;


  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  fclose (is);

  if (!strcmp (str, "solid"))
    return a2ri_vf_load_ascii_stl (file, m);
  else
    return a2ri_vf_load_binary_stl (file, m);
}

/**
   Ouverture d'un fichier stl binaire contenant un modèle
   @param file chemin du fichier ÃÂ  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succÃÂ¨s, 0 sinon
*/
int
a2ri_vf_load_binary_stl (
			 const char * const file,
			 vf_model * m)
{
  FILE *is = NULL;
  char str[80];
  unsigned int nbface;
  unsigned char finligne;
  float coordx,
    coordy,
    coordz;
  int ve1 = 0,
    ve2 = 0,
    ve3 = 0;
  point3d ptmin,
    ptmax;
  space_partition sp;

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "rb");
  if (is == NULL)
    return 0;

  a2ri_erreur_critique_si(!fread (&str, sizeof (char), 80, is),"erreur en ouverture de fichier");

  a2ri_erreur_critique_si(!fread (&nbface, sizeof (unsigned int), 1, is),"erreur en ouverture de fichier");

  //recherche de la bounding box du modele numerique
  for (unsigned int i = 0; i < nbface; i++)
    {
      a2ri_erreur_critique_si(!fread (&coordx, sizeof (float), 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&coordy, sizeof (float), 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&coordz, sizeof (float), 1, is),"erreur en ouverture de fichier");
      for (unsigned int j = 0; j < 3; j++)
	{
	  //read the vertex
	  a2ri_erreur_critique_si(!fread (&coordx, sizeof (float), 1, is),"erreur en ouverture de fichier");
	  a2ri_erreur_critique_si(!fread (&coordy, sizeof (float), 1, is),"erreur en ouverture de fichier");
	  a2ri_erreur_critique_si(!fread (&coordz, sizeof (float), 1, is),"erreur en ouverture de fichier");

	  if (i == 0 && j == 0)
	    {
	      m->xmin = coordx;
	      m->xmax = coordx;
	      m->ymin = coordy;
	      m->ymax = coordy;
	      m->zmin = coordz;
	      m->zmax = coordz;
	    }
	  else
	    {
	      if (coordx < m->xmin)
		m->xmin = coordx;
	      if (coordx > m->xmax)
		m->xmax = coordx;
	      if (coordy < m->ymin)
		m->ymin = coordy;
	      if (coordy > m->ymax)
		m->ymax = coordy;
	      if (coordz < m->zmin)
		m->zmin = coordz;
	      if (coordz > m->zmax)
		m->zmax = coordz;
	    }
	}

      a2ri_erreur_critique_si(!fread (&finligne, 1, 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&finligne, 1, 1, is),"erreur en ouverture de fichier");
    }

  point3d_init (&ptmin, m->xmin, m->ymin, m->zmin);
  point3d_init (&ptmax, m->xmax, m->ymax, m->zmax);

  int nb_part = (int) (pow (nbface/22.0, (1.0 / 3.0))) + 1;

  space_partition_new (&sp, &ptmin, &ptmax, nb_part, nb_part, nb_part);

  //retour au debut du fichier
  fseek (is, 0, SEEK_SET);
  a2ri_erreur_critique_si(!fread (&str, sizeof (char), 80, is),"erreur en ouverture de fichier");
  a2ri_erreur_critique_si(!fread (&nbface, sizeof (unsigned int), 1, is),"erreur en ouverture de fichier");

  for (unsigned int i = 0; i < nbface; i++)
    {
      a2ri_erreur_critique_si(!fread (&coordx, sizeof (float), 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&coordy, sizeof (float), 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&coordz, sizeof (float), 1, is),"erreur en ouverture de fichier");
      for (unsigned int j = 0; j < 3; j++)
	{
	  a2ri_erreur_critique_si(!fread (&coordx, sizeof (float), 1, is),"erreur en ouverture de fichier");
	  a2ri_erreur_critique_si(!fread (&coordy, sizeof (float), 1, is),"erreur en ouverture de fichier");
	  a2ri_erreur_critique_si(!fread (&coordz, sizeof (float), 1, is),"erreur en ouverture de fichier");	//read the vertex
	  point3d_init (&ptmin, coordx, coordy, coordz);
	  if (i == 0 && j == 0)
	    {
	      ve1 = 0;
	      ptmin.att_int = space_partition_nb_points (&sp);
	      space_partition_add_point (&sp, &ptmin);
	      a2ri_vf_add_vertex (m, ptmin.x, ptmin.y, ptmin.z);
	    }
	  else
	    {
	      point3d *ret=space_partition_contains_point (&sp, &ptmin);
	      if (ret!=NULL)
		{
		  switch (j)
		    {
		    case 0:
		      ve1 = ret->att_int;
		      break;
		    case 1:
		      ve2 = ret->att_int;
		      break;
		    default:
		      ve3 = ret->att_int;
		    }
		}
	      else
		{
		  ptmin.att_int = space_partition_nb_points (&sp);
		  switch (j)
		    {
		    case 0:
		      ve1 = ptmin.att_int;
		      break;
		    case 1:
		      ve2 = ptmin.att_int;
		      break;
		    default:
		      ve3 = ptmin.att_int;
		    }
		  space_partition_add_point (&sp, &ptmin);
		  a2ri_vf_add_vertex (m, ptmin.x, ptmin.y, ptmin.z);
		}
	    }
	}
      a2ri_vf_add_face (m, ve1, ve2, ve3);
      a2ri_erreur_critique_si(!fread (&finligne, 1, 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&finligne, 1, 1, is),"erreur en ouverture de fichier");
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Ouverture d'un fichier stl ascii contenant un modèle
   @param file chemin du fichier ÃÂ  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succÃÂ¨s, 0 sinon
*/
int
a2ri_vf_load_ascii_stl (
			const char * const file,
			vf_model * m)
{
  FILE *is = NULL;
  int ve1,
    ve2,
    ve3,
    i = 0,
    fin = 0;
  double coordx,
    coordy,
    coordz;
  char str[250];

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
  while (strcmp (str, "facet"))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  while (!fin)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read normal
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read the 3 values of the normal
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//outer loop
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read vertex
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordx = atof (str);	//coordx
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordy = atof (str);	//coordy
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordz = atof (str);	//coordz
      ve1 = a2ri_vf_search_vertex (m, coordx, coordy, coordz);
      if (ve1 == -1)
	{
	  ve1 = m->nbvertex;
	  a2ri_vf_add_vertex (m, coordx, coordy, coordz);
	}
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	  i++;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read vertex
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordx = atof (str);	//coordx
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordy = atof (str);	//coordy
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordz = atof (str);	//coordz
      ve2 = a2ri_vf_search_vertex (m, coordx, coordy, coordz);
      if (ve2 == -1)
	{
	  ve2 = m->nbvertex;
	  a2ri_vf_add_vertex (m, coordx, coordy, coordz);
	}
      if (coordx < m->xmin)
	m->xmin = coordx;
      if (coordx > m->xmax)
	m->xmax = coordx;
      if (coordy < m->ymin)
	m->ymin = coordy;
      if (coordy > m->ymax)
	m->ymax = coordy;
      if (coordz < m->zmin)
	m->zmin = coordz;
      if (coordz > m->zmax)
	m->zmax = coordz;
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read vertex
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordx = atof (str);	//coordx
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordy = atof (str);	//coordy
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordz = atof (str);	//coordz;
      ve3 = a2ri_vf_search_vertex (m, coordx, coordy, coordz);
      if (ve3 == -1)
	{
	  ve3 = m->nbvertex;
	  a2ri_vf_add_vertex (m, coordx, coordy, coordz);
	}
      if (coordx < m->xmin)
	m->xmin = coordx;
      if (coordx > m->xmax)
	m->xmax = coordx;
      if (coordy < m->ymin)
	m->ymin = coordy;
      if (coordy > m->ymax)
	m->ymax = coordy;
      if (coordz < m->zmin)
	m->zmin = coordz;
      if (coordz > m->zmax)
	m->zmax = coordz;

      a2ri_vf_add_face (m, ve1, ve2, ve3);

      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read endloop
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read endfacet
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read facet ou endsolid
      fin = !strcmp (str, "endsolid");
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Enregistrement d'un modèle dans un format défini par l'extension du nom de fichier
   @param file nom du fichier
   @param m le modèle
   @return aucun
*/
int
a2ri_vf_save_file (
		   const char * const file,
		   const vf_model * const m)
{
  char ext[4];

  memcpy (ext, file + (strlen (file) - 3), 3);
  ext[3] = 0;

  //enregistrement du modèle en fonction de l'extension
  if (!strcmp (ext, "wrl"))
    {
      return a2ri_vf_save_wrl (file, m);
    }

  if (!strcmp (ext, "vef"))
    {
      return a2ri_vf_save_vef (file, m);
    }

  if (!strcmp (ext, "pgn"))
    {
      return a2ri_vf_save_pgn (file, m);
    }

  if (!strcmp (ext, "ply"))
    {
      return a2ri_vf_save_ply (file, m);
    }

  if (!strcmp (ext, "off"))
    {
      return a2ri_vf_save_off (file, m);
    }

  if (!strcmp (ext, "obj"))
    {
      return a2ri_vf_save_obj (file, m);
    }

  if (!strcmp (ext, "gts"))
    {
      return a2ri_vf_save_gts (file, m);
    }
  if (!strcmp (ext, "stl"))
    {
      return a2ri_vf_save_binary_stl (file, m);
    }

  return 0;

}

/**
   Enregistrement d'un modèle dans un fichier wr@param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_save_wrl (
		  const char * const file,
		  const vf_model * const m)
{
  setlocale(LC_NUMERIC,"C");
  
  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture de l'en-tete
  fprintf (os, "#VRML V2.0 utf8\n");
  fprintf (os, "Shape{\n");
  fprintf (os, "appearance Appearance {\n");
  fprintf (os, "material Material {\n");
  fprintf (os, "}\n");
  fprintf (os, "}\n");
  fprintf (os, "geometry IndexedFaceSet{\n");
  fprintf (os, "coord Coordinate{\n");
  fprintf (os, "point[\n");

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%f %f %f,\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);
  fprintf (os, "]\n");
  fprintf (os, "}\n");

  //ecriture des faces
  fprintf (os, "coordIndex[\n");
  for (int i = 0; i < m->nbface; i++)
    {
      int nb1 = m->fa[i].ve1;
      int nb2 = m->fa[i].ve2;
      int nb3 = m->fa[i].ve3;
      fprintf (os, "%d %d %d -1\n", nb1, nb2, nb3);
    }
  fprintf (os, "]\n");
  fprintf (os, "}\n");
  fprintf (os, "}\n");
  fclose (os);
  
  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Enregistrement d'un modèle dans un fichier wrl version 1.0
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_save_wrl_v1 (
		     const char * const file,
		     const vf_model * const m)
{
  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture de l'en-tete
  fprintf (os, "#VRML V1.0 ascii\n");
  fprintf (os, "Separator {\n");
  fprintf (os, "Switch {\n");
  fprintf (os, "DEF Pol{\n");
  fprintf (os, "Separator {\n");
  fprintf (os, "Coordinate3 {\n");
  fprintf (os, "point[\n");

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%f %f %f,\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);
  fprintf (os, "]\n");
  fprintf (os, "}\n");

  //ecriture des faces
  fprintf (os, "IndexedFaceSet {\n");
  fprintf (os, "coordIndex [\n");
  for (int i = 0; i < m->nbface; i++)
    {
      int nb1 = m->fa[i].ve1;
      int nb2 = m->fa[i].ve2;
      int nb3 = m->fa[i].ve3;
      fprintf (os, "%d, %d, %d, -1,\n", nb1, nb2, nb3);
    }
  fprintf (os, "]\n");
  fprintf (os, "}\n");
  fprintf (os, "}\n");
  fprintf (os, "}\n");
  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Enregistrement d'un modèle dans un fichier pgn
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_save_pgn (
		  const char * const file,
		  const vf_model * const m)
{
  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture du nombre de sommets et de faces
  fprintf (os, "%d %d\n", m->nbvertex, m->nbface);

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%lf %lf %lf\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);

  fprintf (os, "\n");

  //ecriture des faces
  for (int i = 0; i < m->nbface; i++)
    fprintf (os, "%d %d %d -1\n", m->fa[i].ve1, m->fa[i].ve2, m->fa[i].ve3);

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;

}

/**
   Enregistrement d'un modèle dans un fichier vef
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
**/
int
a2ri_vf_save_vef (
		  const char * const file,
		  const vf_model * const m)
{
  hashtable *table = hashtable_new (m->nbvertex + m->nbface);
  hashtable *tabletemp = NULL;

  int p1,
    p2;

  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  tabletemp = a2ri_vf_construction_edge_table (m, NULL, 0);

  //ecriture du nombre de sommets, d'aretes et de faces
  fprintf (os, "%d %d %d\n", m->nbvertex, hashtable_size (tabletemp),
	   m->nbface);

  
  hashtable_free (tabletemp);
  free(tabletemp);

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%lf %lf %lf\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);

  fprintf (os, "\n");

  //ecriture des aretes
  IFsave_vef_write_edge (m, table, os);

  fprintf (os, "\n");

  //ecriture des faces
  for (int i = 0; i < m->nbface; i++)
    {
      p1 = m->fa[i].ve1;
      p2 = m->fa[i].ve2;
      //recherche de l'arete dans la table de hachage
      vf_edge *edgetemp = hashtable_look_for (table, p1, p2);
      fprintf (os, "%d ", edgetemp->att_int);

      p1 = m->fa[i].ve2;
      p2 = m->fa[i].ve3;
      edgetemp = hashtable_look_for (table, p1, p2);
      fprintf (os, "%d ", edgetemp->att_int);

      p1 = m->fa[i].ve3;
      p2 = m->fa[i].ve1;
      edgetemp = hashtable_look_for (table, p1, p2);
      fprintf (os, "%d\n", edgetemp->att_int);
    }

  hashtable_free (table);
  free(table);

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 0;
}

/**
   Enregistrement d'un modèle dans un fichier gts
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
**/
int
a2ri_vf_save_gts (
		  const char * const file,
		  const vf_model * const m)
{
  hashtable *table = hashtable_new (m->nbvertex + m->nbface);
  hashtable *tabletemp = NULL;

  int p1,
    p2;

  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  tabletemp = a2ri_vf_construction_edge_table (m, NULL, 0);

  //ecriture du nombre de sommets, d'aretes et de faces
  fprintf (os, "%d %d %d\n", m->nbvertex, hashtable_size (tabletemp),
	   m->nbface);

  hashtable_free (tabletemp);

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%lf %lf %lf\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);

  fprintf (os, "\n");


  //ecriture des aretes
  IFsave_gts_write_edge (m, table, os);

  fprintf (os, "\n");

  //ecriture des faces
  for (int i = 0; i < m->nbface; i++)
    {
      p1 = m->fa[i].ve1;
      p2 = m->fa[i].ve2;
      //recherche de l'arete dans la table de hachage
      vf_edge *edgetemp = hashtable_look_for (table, p1, p2);
      fprintf (os, "%d ", edgetemp->att_int + 1);

      p1 = m->fa[i].ve2;
      p2 = m->fa[i].ve3;
      edgetemp = hashtable_look_for (table, p1, 2);
      fprintf (os, "%d ", edgetemp->att_int + 1);

      p1 = m->fa[i].ve3;
      p2 = m->fa[i].ve1;
      edgetemp = hashtable_look_for (table, p1, p2);
      fprintf (os, "%d\n", edgetemp->att_int + 1);
    }

  hashtable_free (table);

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 0;
}

/**
   Enregistrement d'un modèle dans un fichier off
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_save_off (
		  const char * const file,
		  const vf_model * const m)
{
  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture du nombre de sommets et de faces
  fprintf (os, "OFF\n%d %d 0\n", m->nbvertex, m->nbface);

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%lf %lf %lf\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);

  fprintf (os, "\n");

  //ecriture des faces
  for (int i = 0; i < m->nbface; i++)
    fprintf (os, "3 %d %d %d\n", m->fa[i].ve1, m->fa[i].ve2, m->fa[i].ve3);

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Enregistrement d'un modèle dans un fichier ply
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_save_ply (
		  const char * const file,
		  const vf_model * const m)
{
  int ve1,
    ve2,
    ve3;

  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture des en-tetes
  fprintf (os, "ply\nformat ascii 1.0\ncomment zipper output\n");
  fprintf (os,
	   "element vertex %d\nproperty float x\nproperty float y\nproperty float z\nelement face %d\n",
	   m->nbvertex, m->nbface);
  fprintf (os, "property list uchar int vertex_indices\nend_header\n");

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%f %f %f\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);

  //ecriture des faces
  for (int i = 0; i < m->nbface; i++)
    {
      ve1 = m->fa[i].ve1;
      ve2 = m->fa[i].ve2;
      ve3 = m->fa[i].ve3;
      fprintf (os, "3 %d %d %d \n", ve1, ve2, ve3);
    }

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;

}

/**
   Enregistrement d'un modèle dans un fichier obj
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_save_obj (
		  const char * const file,
		  const vf_model * const m)
{
  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "v %lf %lf %lf\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);

  fprintf (os, "\n");

  //ecriture des faces
  for (int i = 0; i < m->nbface; i++)
    fprintf (os, "f %d %d %d\n", m->fa[i].ve1 + 1, m->fa[i].ve2 + 1,
	     m->fa[i].ve3 + 1);

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;

}

/**
   Enregistrement d'un modèle dans un fichier stl binaire
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succés, 0 sinon
*/
int
a2ri_vf_save_binary_stl (
		  const char * const file,
		  const vf_model * const m)
{

  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "wb");
  if (os == NULL)
    return 0;

    fwrite
    ("generated by A2RI                                                               ",
     1, 80, os);
  fwrite (&(m->nbface), sizeof (unsigned int), 1, os);
  for (int i = 0; i < m->nbface; i++)
    {
      vector3d AB,
        AC,
        normale;
      vector3d_init (&AB,
		     m->ve[m->fa[i].ve2].x - m->ve[m->fa[i].ve1].x,
		     m->ve[m->fa[i].ve2].y - m->ve[m->fa[i].ve1].y,
		     m->ve[m->fa[i].ve2].z - m->ve[m->fa[i].ve1].z);
      vector3d_init (&AC,
		     m->ve[m->fa[i].ve3].x - m->ve[m->fa[i].ve1].x,
		     m->ve[m->fa[i].ve3].y - m->ve[m->fa[i].ve1].y,
		     m->ve[m->fa[i].ve3].z - m->ve[m->fa[i].ve1].z);
      normale = vector3d_vectorialproduct (&AB, &AC);
      float x,
        y,
        z;
      x = normale.dx;
      y = normale.dy;
      z = normale.dz;
      fwrite (&x, sizeof (float), 1, os);
      fwrite (&y, sizeof (float), 1, os);
      fwrite (&z, sizeof (float), 1, os);
      x = m->ve[m->fa[i].ve1].x;
      y = m->ve[m->fa[i].ve1].y;
      z = m->ve[m->fa[i].ve1].z;
      fwrite (&x, sizeof (float), 1, os);
      fwrite (&y, sizeof (float), 1, os);
      fwrite (&z, sizeof (float), 1, os);
      x = m->ve[m->fa[i].ve2].x;
      y = m->ve[m->fa[i].ve2].y;
      z = m->ve[m->fa[i].ve2].z;
      fwrite (&x, sizeof (float), 1, os);
      fwrite (&y, sizeof (float), 1, os);
      fwrite (&z, sizeof (float), 1, os);
      x = m->ve[m->fa[i].ve3].x;
      y = m->ve[m->fa[i].ve3].y;
      z = m->ve[m->fa[i].ve3].z;
      fwrite (&x, sizeof (float), 1, os);
      fwrite (&y, sizeof (float), 1, os);
      fwrite (&z, sizeof (float), 1, os);
      fwrite ("  ", 1, 2, os);
    }

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Enregistrement d'un modèle dans un fichier stl ascii
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succés, 0 sinon
*/
int
a2ri_vf_save_ascii_stl (
		  const char * const file,
		  const vf_model * const m)
{

  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  fprintf(os,"solid generatedByA2RI\n");
  for (int i = 0; i < m->nbface; i++)
    {
      vector3d AB,
        AC,
        normale;
      vector3d_init (&AB,
		     m->ve[m->fa[i].ve2].x - m->ve[m->fa[i].ve1].x,
		     m->ve[m->fa[i].ve2].y - m->ve[m->fa[i].ve1].y,
		     m->ve[m->fa[i].ve2].z - m->ve[m->fa[i].ve1].z);
      vector3d_init (&AC,
		     m->ve[m->fa[i].ve3].x - m->ve[m->fa[i].ve1].x,
		     m->ve[m->fa[i].ve3].y - m->ve[m->fa[i].ve1].y,
		     m->ve[m->fa[i].ve3].z - m->ve[m->fa[i].ve1].z);
      normale = vector3d_vectorialproduct (&AB, &AC);
      float x,
        y,
        z;
      x = normale.dx;
      y = normale.dy;
      z = normale.dz;
      fprintf (os,"facet normal %f %f %f\n\touter loop\n\t\tvertex ",x,y,z);
      x = m->ve[m->fa[i].ve1].x;
      y = m->ve[m->fa[i].ve1].y;
      z = m->ve[m->fa[i].ve1].z;
      fprintf (os,"%f %f %f\n\t\tvertex ",x,y,z);
      x = m->ve[m->fa[i].ve2].x;
      y = m->ve[m->fa[i].ve2].y;
      z = m->ve[m->fa[i].ve2].z;
      fprintf (os,"%f %f %f\n\t\tvertex ",x,y,z);
      x = m->ve[m->fa[i].ve3].x;
      y = m->ve[m->fa[i].ve3].y;
      z = m->ve[m->fa[i].ve3].z;
      fprintf (os,"%f %f %f\n\tendloop\nendfacet\n",x,y,z);
    }

  fprintf(os,"endsolid generatedByA2RI\n");

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Ouverture d'un fichier contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_open_file (
		    const char * const file,
		    vef_model * m)
{
  char ext[5];

  memcpy (ext, file + (strlen (file) - 4), 4);
  ext[4] = 0;

  //appel de la fonction correspondant à l'extension du fichier
  if (!strcmp (ext, ".wrl"))
    return (a2ri_vef_load_wrl (file, m));

  if (!strcmp (ext, ".vef"))
    return (a2ri_vef_load_vef (file, m));

  if (!strcmp (ext, ".pgn"))
    return (a2ri_vef_load_pgn (file, m));

  if (!strcmp (ext, ".ply"))
    return (a2ri_vef_load_ply (file, m));

  if (!strcmp (ext, ".off"))
    return (a2ri_vef_load_off (file, m));

  if (!strcmp (ext, ".obj"))
    return (a2ri_vef_load_obj (file, m));

  if (!strcmp (ext, ".gts"))
    return (a2ri_vef_load_gts (file, m));

  if (!strcmp (ext, ".stl"))
    return (a2ri_vef_load_stl (file, m));

  return 0;

}

/**
   Ouverture d'un fichier pgn contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_load_pgn (
		   const char * const file,
		   vef_model * m)
{

  FILE *is = NULL;
  double coordx,
    coordy,
    coordz;
  int ar1,
    ar2,
    ar3,
    nbvertex,
    nbface;

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;
  a2ri_vef_init (m);

  //lecture du nombre de sommets
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbvertex),"erreur en ouverture de fichier");

  //lecture du nombre de faces
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbface),"erreur en ouverture de fichier");

  //lecture des sommets
  for (int i = 0; i < nbvertex; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordx),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordy),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordz),"erreur en ouverture de fichier");
      a2ri_vef_add_vertex (m, coordx, coordy, coordz);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
    }

  //lecture des faces
  for (int i = 0; i < nbface; i++)
    {
      //lecture des trois sommets composant la face
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ar1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ar2),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ar3),"erreur en ouverture de fichier");
      //ajout des aretes avec vérification au préalable de leur existence
      a2ri_vef_add_edge (m, ar1, ar2, 1);
      a2ri_vef_add_edge (m, ar2, ar3, 1);
      a2ri_vef_add_edge (m, ar3, ar1, 1);
      //recherche des aretes
      int ed1 = a2ri_vef_search_edge (m, ar1, ar2);
      int ed2 = a2ri_vef_search_edge (m, ar2, ar3);
      int ed3 = a2ri_vef_search_edge (m, ar3, ar1);
      //ajout de la faces avec les trois aretes
      a2ri_vef_add_face (m, ed1, ed2, ed3);
      // reading the -1
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ar1),"erreur en ouverture de fichier");
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;

}

/**
   Ouverture d'un fichier stl binaire contenant un modèle
   @param file chemin du fichier ÃÂ  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succÃÂ¨s, 0 sinon
*/
int
a2ri_vef_load_binary_stl (
			  const char * const file,
			  vef_model * m)
{
  FILE *is = NULL;
  char str[80];
  unsigned int nbface;
  unsigned char finligne;
  float coordx,
    coordy,
    coordz;
  int ve1,
    ve2,
    ve3,
    
    ed1,
    ed2,
    ed3;

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "rb");
  if (is == NULL)
    return 0;

  a2ri_erreur_critique_si(!fread (&str, sizeof (char), 80, is),"erreur en ouverture de fichier");

  a2ri_erreur_critique_si(!fread (&nbface, sizeof (unsigned int), 1, is),"erreur en ouverture de fichier");


  for (unsigned int i = 0; i < nbface; i++)
    {
      a2ri_erreur_critique_si(!fread (&coordx, sizeof (float), 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&coordy, sizeof (float), 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&coordz, sizeof (float), 1, is),"erreur en ouverture de fichier");	//read the normal
      a2ri_erreur_critique_si(!fread (&coordx, sizeof (float), 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&coordy, sizeof (float), 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&coordz, sizeof (float), 1, is),"erreur en ouverture de fichier");	//read the first vertex

      ve1 = a2ri_vef_search_vertex (m, coordx, coordy, coordz);
      if (ve1 == -1)
	{
	  ve1 = m->nbvertex;
	  a2ri_vef_add_vertex (m, coordx, coordy, coordz);
	}
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
      a2ri_erreur_critique_si(!fread (&coordx, sizeof (float), 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&coordy, sizeof (float), 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&coordz, sizeof (float), 1, is),"erreur en ouverture de fichier");	//read the second vertex
      ve2 = a2ri_vef_search_vertex (m, coordx, coordy, coordz);
      if (ve2 == -1)
	{
	  ve2 = m->nbvertex;
	  a2ri_vef_add_vertex (m, coordx, coordy, coordz);
	}
      if (coordx < m->xmin)
	m->xmin = coordx;
      if (coordx > m->xmax)
	m->xmax = coordx;
      if (coordy < m->ymin)
	m->ymin = coordy;
      if (coordy > m->ymax)
	m->ymax = coordy;
      if (coordz < m->zmin)
	m->zmin = coordz;
      if (coordz > m->zmax)
	m->zmax = coordz;
      a2ri_erreur_critique_si(!fread (&coordx, sizeof (float), 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&coordy, sizeof (float), 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&coordz, sizeof (float), 1, is),"erreur en ouverture de fichier");	//read the third vertex
      ve3 = a2ri_vef_search_vertex (m, coordx, coordy, coordz);
      if (ve3 == -1)
	{
	  ve3 = m->nbvertex;
	  a2ri_vef_add_vertex (m, coordx, coordy, coordz);
	}
      if (coordx < m->xmin)
	m->xmin = coordx;
      if (coordx > m->xmax)
	m->xmax = coordx;
      if (coordy < m->ymin)
	m->ymin = coordy;
      if (coordy > m->ymax)
	m->ymax = coordy;
      if (coordz < m->zmin)
	m->zmin = coordz;
      if (coordz > m->zmax)
	m->zmax = coordz;

      ed1 = a2ri_vef_search_edge (m, ve1, ve2);
      ed2 = a2ri_vef_search_edge (m, ve1, ve3);
      ed3 = a2ri_vef_search_edge (m, ve2, ve3);

      if (ed1 == -1)
	{
	  ed1 = m->nbedge;
	  a2ri_vef_add_edge (m, ve1, ve2, 0);
	}
      if (ed2 == -1)
	{
	  ed2 = m->nbedge;
	  a2ri_vef_add_edge (m, ve1, ve3, 0);
	}
      if (ed3 == -1)
	{
	  ed3 = m->nbedge;
	  a2ri_vef_add_edge (m, ve2, ve3, 0);
	}

      a2ri_vef_add_face (m, ed2, ed1, ed3);

      a2ri_erreur_critique_si(!fread (&finligne, 1, 1, is),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fread (&finligne, 1, 1, is),"erreur en ouverture de fichier");
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Ouverture d'un fichier stl ascii contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_load_ascii_stl (
			 const char * const file,
			 vef_model * m)
{
  FILE *is = NULL;
  int ve1,
    ve2,
    ve3,
    i = 0,
    fin = 0,
    ed1,
    ed2,
    ed3;
  double coordx,
    coordy,
    coordz;
  char str[250];

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
  while (strcmp (str, "facet"))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  while (!fin)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read normal
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read the 3 values of the normal
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//outer loop
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read vertex
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordx = atof (str);	//coordx
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordy = atof (str);	//coordy
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordz = atof (str);	//coordz
      ve1 = a2ri_vef_search_vertex (m, coordx, coordy, coordz);
      if (ve1 == -1)
	{
	  ve1 = m->nbvertex;
	  a2ri_vef_add_vertex (m, coordx, coordy, coordz);
	}
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	  i++;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read vertex
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordx = atof (str);	//coordx
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordy = atof (str);	//coordy
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordz = atof (str);	//coordz
      ve2 = a2ri_vef_search_vertex (m, coordx, coordy, coordz);
      if (ve2 == -1)
	{
	  ve2 = m->nbvertex;
	  a2ri_vef_add_vertex (m, coordx, coordy, coordz);
	}
      if (coordx < m->xmin)
	m->xmin = coordx;
      if (coordx > m->xmax)
	m->xmax = coordx;
      if (coordy < m->ymin)
	m->ymin = coordy;
      if (coordy > m->ymax)
	m->ymax = coordy;
      if (coordz < m->zmin)
	m->zmin = coordz;
      if (coordz > m->zmax)
	m->zmax = coordz;
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read vertex
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordx = atof (str);	//coordx
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordy = atof (str);	//coordy
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      coordz = atof (str);	//coordz;
      ve3 = a2ri_vef_search_vertex (m, coordx, coordy, coordz);
      if (ve3 == -1)
	{
	  ve3 = m->nbvertex;
	  a2ri_vef_add_vertex (m, coordx, coordy, coordz);
	}
      if (coordx < m->xmin)
	m->xmin = coordx;
      if (coordx > m->xmax)
	m->xmax = coordx;
      if (coordy < m->ymin)
	m->ymin = coordy;
      if (coordy > m->ymax)
	m->ymax = coordy;
      if (coordz < m->zmin)
	m->zmin = coordz;
      if (coordz > m->zmax)
	m->zmax = coordz;

      ed1 = a2ri_vef_search_edge (m, ve1, ve2);
      ed2 = a2ri_vef_search_edge (m, ve1, ve3);
      ed3 = a2ri_vef_search_edge (m, ve2, ve3);

      if (ed1 == -1)
	{
	  ed1 = m->nbedge;
	  a2ri_vef_add_edge (m, ve1, ve2, 0);
	}
      if (ed2 == -1)
	{
	  ed2 = m->nbedge;
	  a2ri_vef_add_edge (m, ve1, ve3, 0);
	}
      if (ed3 == -1)
	{
	  ed3 = m->nbedge;
	  a2ri_vef_add_edge (m, ve2, ve3, 0);
	}

      a2ri_vef_add_face (m, ed2, ed1, ed3);

      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read endloop
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read endfacet
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");	//read facet ou endsolid
      fin = !strcmp (str, "endsolid");
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Ouverture d'un fichier stl contenant un modèle
   @param file chemin du fichier ÃÂ  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succÃÂ¨s, 0 sinon
*/
int
a2ri_vef_load_stl (
		   const char * const file,
		   vef_model * m)
{
  FILE *is = NULL;
  char str[250];

  is = fopen (file, "r");
  if (is == NULL)
    return 0;


  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  fclose (is);

  if (!strcmp (str, "solid"))
    return a2ri_vef_load_ascii_stl (file, m);
  else
    return a2ri_vef_load_binary_stl (file, m);
}

/**
   Ouverture d'un fichier obj contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_load_obj (
		   const char * const file,
		   vef_model * m)
{

  FILE *is = NULL;
  int ve1,
    ve2,
    ve3;
  double coordx,
    coordy,
    coordz;
  char str[250];

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  while (strcmp (str, "v") != 0)
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  //lecture des sommets
  while (strcmp (str, "g") != 0)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf %lf %lf", &coordx, &coordy, &coordz),"erreur en ouverture de fichier");
      //ajout d'un sommet
      a2ri_vef_add_vertex (m, coordx, coordy, coordz);
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      //mis à jour de la bounding box
      if (m->nbvertex == 1)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
    }

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  //lecture des faces
  while (!feof (is))
    {
      //lecture des trois sommets
      a2ri_erreur_critique_si(!fscanf (is, "%d %d %d", &ve1, &ve2, &ve3),"erreur en ouverture de fichier");
      ve1--;
      ve2--;
      ve3--;
      //ajout des aretes avec vérification au préalable de leur existence
      a2ri_vef_add_edge (m, ve1, ve2, 1);
      a2ri_vef_add_edge (m, ve2, ve3, 1);
      a2ri_vef_add_edge (m, ve3, ve1, 1);
      //recherche des aretes
      int ed1 = a2ri_vef_search_edge (m, ve1, ve2);
      int ed2 = a2ri_vef_search_edge (m, ve2, ve3);
      int ed3 = a2ri_vef_search_edge (m, ve3, ve1);
      //ajout de la faces avec les trois aretes
      a2ri_vef_add_face (m, ed1, ed2, ed3);

      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      //si la face compte plus de trois sommets, on relit et on ajoute des aretes et une seconde face
      if (strcmp (str, "f") != 0)
	{
	  ve2 = atoi (str);
	  ve2--;

	  a2ri_vef_add_edge (m, ve1, ve2, 1);
	  a2ri_vef_add_edge (m, ve2, ve3, 1);
	  a2ri_vef_add_edge (m, ve3, ve1, 1);
	  int ed1 = a2ri_vef_search_edge (m, ve1, ve2);
	  int ed2 = a2ri_vef_search_edge (m, ve2, ve3);
	  int ed3 = a2ri_vef_search_edge (m, ve3, ve1);
	  a2ri_vef_add_face (m, ed1, ed3, ed2);


	  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
	}
    }


  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;

}

/**
   Ouverture d'un fichier wrl contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_load_wrl (
		   const char * const file,
		   vef_model * m)
{
  FILE *is = NULL;
  char str[20];
  char coordx[15],
    coordy[15],
    coordz[15];

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  while (strcmp (str, "point") && strcmp (str, "point["))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  if (!strcmp (str, "point"))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  // reading the file until coordx = } or ]
  a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
  int i = 0;
  //lecture des sommets
  while (strcmp (coordx, "}") && strcmp (coordx, "]"))
    {
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordy),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordz),"erreur en ouverture de fichier");
      if (coordz[strlen (coordz) - 1] == ',')
	{
	  memcpy (coordz, coordz, strlen (coordz) - 1);
	  coordz[strlen (coordz)] = '\0';
	}
      a2ri_vef_add_vertex (m, atof (coordx), atof (coordy), atof (coordz));
      //mis à jour de la bounding box
      if (i == 0)
	{
	  i++;
	  m->xmin = atof (coordx);
	  m->xmax = atof (coordx);
	  m->ymin = atof (coordy);
	  m->ymax = atof (coordy);
	  m->zmin = atof (coordz);
	  m->zmax = atof (coordz);
	}
      else
	{
	  if (atof (coordx) < m->xmin)
	    m->xmin = atof (coordx);
	  if (atof (coordx) > m->xmax)
	    m->xmax = atof (coordx);
	  if (atof (coordy) < m->ymin)
	    m->ymin = atof (coordy);
	  if (atof (coordy) > m->ymax)
	    m->ymax = atof (coordy);
	  if (atof (coordz) < m->zmin)
	    m->zmin = atof (coordz);
	  if (atof (coordz) > m->zmax)
	    m->zmax = atof (coordz);
	}
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
    }

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  while (strcmp (str, "coordIndex") && strcmp (str, "coordIndex["))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  if (!strcmp (str, "coordIndex"))
    a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  //lecture des faces
  a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
  while (strcmp (coordx, "}") && strcmp (coordx, "]"))
    {
      //lecture des trois sommets composant la face
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordy),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordz),"erreur en ouverture de fichier");
      //ajout des aretes avec vérification au préalable de leur existence
      a2ri_vef_add_edge (m, atoi (coordx), atoi (coordy), 1);
      a2ri_vef_add_edge (m, atoi (coordy), atoi (coordz), 1);
      a2ri_vef_add_edge (m, atoi (coordz), atoi (coordx), 1);
      //recherche des aretes
      int ed1 = a2ri_vef_search_edge (m, atoi (coordx), atoi (coordy));
      int ed2 = a2ri_vef_search_edge (m, atoi (coordy), atoi (coordz));
      int ed3 = a2ri_vef_search_edge (m, atoi (coordz), atoi (coordx));
      //ajout de la faces avec les trois aretes
      a2ri_vef_add_face (m, ed1, ed2, ed3);

      // reading the -1
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%s", coordx),"erreur en ouverture de fichier");
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Ouverture d'un fichier vef contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_load_vef (
		   const char * const file,
		   vef_model * m)
{
  FILE *is = NULL;
  int nbvertex,
    nbedge,
    nbface,
    ve1,
    ve2,
    ed1,
    ed2,
    ed3;
  double coordx,
    coordy,
    coordz;

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_vef_init (m);

  //lecture du nombre de sommets
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbvertex),"erreur en ouverture de fichier");

  //lecture du nombre d'aretes
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbedge),"erreur en ouverture de fichier");

  //lecture du nombre de face
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbface),"erreur en ouverture de fichier");

  //lecture des sommets
  for (int i = 0; i < nbvertex; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordx),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordy),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordz),"erreur en ouverture de fichier");
      a2ri_vef_add_vertex (m, coordx, coordy, coordz);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
    }


  //lecture des aretes
  for (int i = 0; i < nbedge; i++)
    {

      a2ri_erreur_critique_si(!fscanf (is, "%d", &ve1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ve2),"erreur en ouverture de fichier");
      a2ri_vef_add_edge (m, ve1, ve2, 0);
    }

  //lecture des faces
  for (int i = 0; i < nbface; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ed1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ed2),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ed3),"erreur en ouverture de fichier");
      a2ri_vef_add_face (m, ed1, ed2, ed3);
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Ouverture d'un fichier gts contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_load_gts (
		   const char * const file,
		   vef_model * m)
{
  FILE *is = NULL;
  int nbvertex,
    nbedge,
    nbface,
    ve1,
    ve2,
    ed1,
    ed2,
    ed3;
  double coordx,
    coordy,
    coordz;

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;

  a2ri_vef_init (m);

  //lecture du nombre de sommets
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbvertex),"erreur en ouverture de fichier");

  //lecture du nombre d'aretes
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbedge),"erreur en ouverture de fichier");

  //lecture du nombre de face
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbface),"erreur en ouverture de fichier");

  //lecture des sommets
  for (int i = 0; i < nbvertex; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordx),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordy),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordz),"erreur en ouverture de fichier");
      a2ri_vef_add_vertex (m, coordx, coordy, coordz);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
    }


  //lecture des aretes
  for (int i = 0; i < nbedge; i++)
    {

      a2ri_erreur_critique_si(!fscanf (is, "%d", &ve1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ve2),"erreur en ouverture de fichier");
      ve1--;
      ve2--;
      a2ri_vef_add_edge (m, ve1, ve2, 0);
    }

  //lecture des faces
  for (int i = 0; i < nbface; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ed1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ed2),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ed3),"erreur en ouverture de fichier");
      ed1--;
      ed2--;
      ed3--;
      a2ri_vef_add_face (m, ed1, ed2, ed3);
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Ouverture d'un fichier ply contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_load_ply (
		   const char * const file,
		   vef_model * m)
{
  int prop = 0,
    nbpropvertex = 0,
    nbpropface = 0,
    nbvertex,
    nbface,
    ve1,
    ve2,
    ve3;
  char str[20];
  double coordx,
    coordy,
    coordz;

  FILE *is = NULL;

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;


  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  if (strcmp (str, "ply"))
    return 0;

  while (strcmp (str, "end_header"))
    {
      a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
      if (prop == 1 && !strcmp (str, "property"))
	nbpropvertex++;
      if (prop == 2 && !strcmp (str, "property"))
	nbpropface++;

      if (!strcmp (str, "element"))
	{
	  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
	  //lecture du nombre de sommets
	  if (!strcmp (str, "vertex"))
	    {
	      prop = 1;
	      a2ri_erreur_critique_si(!fscanf (is, "%d", &nbvertex),"erreur en ouverture de fichier");
	    }
	  //lecture du nombre de faces
	  else
	    {
	      prop = 1;
	      a2ri_erreur_critique_si(!fscanf (is, "%d", &nbface),"erreur en ouverture de fichier");
	    }
	}
    }

  //lecture des sommets
  for (int i = 0; i < nbvertex; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf %lf %lf", &coordx, &coordy, &coordz),"erreur en ouverture de fichier");
      a2ri_vef_add_vertex (m, coordx, coordy, coordz);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
      for (int j = 0; j < nbpropvertex - 4; j++)
	a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
    }

  //lecture des faces
  for (int i = 0; i < nbface; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ve1),"erreur en ouverture de fichier");
      //lecture des trois sommets composant la face
      a2ri_erreur_critique_si(!fscanf (is, "%d %d %d", &ve1, &ve2, &ve3),"erreur en ouverture de fichier");
      //ajout des aretes avec vérification au préalable de leur existence
      a2ri_vef_add_edge (m, ve1, ve2, 1);
      a2ri_vef_add_edge (m, ve2, ve3, 1);
      a2ri_vef_add_edge (m, ve3, ve1, 1);
      //recherche des aretes
      int ed1 = a2ri_vef_search_edge (m, ve1, ve2);
      int ed2 = a2ri_vef_search_edge (m, ve2, ve3);
      int ed3 = a2ri_vef_search_edge (m, ve3, ve1);
      //ajout de la faces avec les trois aretes
      a2ri_vef_add_face (m, ed1, ed2, ed3);
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Ouverture d'un fichier off contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_load_off (
		   const char * const file,
		   vef_model * m)
{

  FILE *is = NULL;
  char str[20];
  double coordx,
    coordy,
    coordz;
  int ar1,
    ar2,
    ar3,
    nbvertex,
    nbface;

  setlocale(LC_NUMERIC,"C");

  is = fopen (file, "r");
  if (is == NULL)
    return 0;
  a2ri_vef_init (m);

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");

  if (strcmp (str, "OFF"))
    return 0;

  //lecture du nombre de sommets
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbvertex),"erreur en ouverture de fichier");

  //lecture du nombre de faces
  a2ri_erreur_critique_si(!fscanf (is, "%d", &nbface),"erreur en ouverture de fichier");

  a2ri_erreur_critique_si(!fscanf (is, "%d", &ar1),"erreur en ouverture de fichier");

  //lecture des sommets
  for (int i = 0; i < nbvertex; i++)
    {
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordx),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordy),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%lf", &coordz),"erreur en ouverture de fichier");
      a2ri_vef_add_vertex (m, coordx, coordy, coordz);
      //mis à jour de la bounding box
      if (i == 0)
	{
	  m->xmin = coordx;
	  m->xmax = coordx;
	  m->ymin = coordy;
	  m->ymax = coordy;
	  m->zmin = coordz;
	  m->zmax = coordz;
	}
      else
	{
	  if (coordx < m->xmin)
	    m->xmin = coordx;
	  if (coordx > m->xmax)
	    m->xmax = coordx;
	  if (coordy < m->ymin)
	    m->ymin = coordy;
	  if (coordy > m->ymax)
	    m->ymax = coordy;
	  if (coordz < m->zmin)
	    m->zmin = coordz;
	  if (coordz > m->zmax)
	    m->zmax = coordz;
	}
    }

  //lecture des faces
  for (int i = 0; i < nbface; i++)
    {
      //lecture des trois sommets composant la face
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ar1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ar1),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ar2),"erreur en ouverture de fichier");
      a2ri_erreur_critique_si(!fscanf (is, "%d", &ar3),"erreur en ouverture de fichier");
      //ajout des aretes avec vérification au préalable de leur existence
      a2ri_vef_add_edge (m, ar1, ar2, 1);
      a2ri_vef_add_edge (m, ar2, ar3, 1);
      a2ri_vef_add_edge (m, ar3, ar1, 1);
      //recherche des aretes
      int ed1 = a2ri_vef_search_edge (m, ar1, ar2);
      int ed2 = a2ri_vef_search_edge (m, ar2, ar3);
      int ed3 = a2ri_vef_search_edge (m, ar3, ar1);
      //ajout de la faces avec les trois aretes
      a2ri_vef_add_face (m, ed1, ed2, ed3);
    }

  fclose (is);

  setlocale(LC_NUMERIC,"");

  return 1;

}

/**
   Enregistrement d'un modèle dans un format défini par l'extension du nom de fichier
   @param file nom du fichier
   @param m le modèle
   @return aucun
*/
int
a2ri_vef_save_file (
		    const char * const file,
		    const vef_model * const m)
{
  char ext[4];
  
  memcpy (ext, file + (strlen (file) - 3), 3);
  ext[3] = 0;

  //appel de la fonction d'enregistrement en fonction du type de l'extension du fichier
  if (!strcmp (ext, "wrl"))
    return a2ri_vef_save_wrl (file, m);

  if (!strcmp (ext, "vef"))
    return a2ri_vef_save_vef (file, m);

  if (!strcmp (ext, "pgn"))
    return a2ri_vef_save_pgn (file, m);

  if (!strcmp (ext, "ply"))
    return a2ri_vef_save_ply (file, m);

  if (!strcmp (ext, "off"))
    return a2ri_vef_save_off (file, m);

  if (!strcmp (ext, "obj"))
    return a2ri_vef_save_obj (file, m);

  if (!strcmp (ext, "gts"))
    return a2ri_vef_save_gts (file, m);

  return 0;

}

/**
   Enregistrement d'un modèle dans un fichier wrl
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_save_wrl (
		   const char * const file,
		   const vef_model * const m)
{
  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture de l'en-tete
  fprintf (os, "#VRML V2.0 utf8\n");
  fprintf (os, "Shape{\n");
  fprintf (os, "appearance Appearance {\n");
  fprintf (os, "material Material {\n");
  fprintf (os, "}\n");
  fprintf (os, "}\n");
  fprintf (os, "geometry IndexedFaceSet{\n");
  fprintf (os, "coord Coordinate{\n");
  fprintf (os, "point[\n");

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%f %f %f,\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);
  fprintf (os, "]\n");
  fprintf (os, "}\n");

  //ecriture des faces
  fprintf (os, "coordIndex[\n");
  for (int i = 0; i < m->nbface; i++)
    {
      int nb1,
        nb2,
        nb3;
      vef_face_get_vertices (&(m->fa[i]), m->ed, &nb1, &nb2, &nb3);
      fprintf (os, "%d %d %d -1\n", nb1, nb2, nb3);
    }
  fprintf (os, "]\n");
  fprintf (os, "}\n");
  fprintf (os, "}\n");
  fclose (os);
  
  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Enregistrement d'un modèle dans un fichier pgn
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_save_pgn (
		   const char * const file,
		   const vef_model * const m)
{
  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture du nombre de sommets et faces
  fprintf (os, "%d %d \n\n", m->nbvertex, m->nbface);

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%f %f %f\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);

  //ecriture des faces
  for (int i = 0; i < m->nbface; i++)
    {
      int ve1,
        ve2,
        ve3;
      vef_face_get_vertices (&(m->fa[i]), m->ed, &ve1, &ve2, &ve3);
      fprintf (os, "%d %d %d -1\n", ve1, ve2, ve3);
    }

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Enregistrement d'un modèle dans un fichier vef
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_save_vef (
		   const char * const file,
		   const vef_model * const m)
{
  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture des nombre de sommets, aretes et faces
  fprintf (os, "%d %d %d\n\n", m->nbvertex, m->nbedge, m->nbface);

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%f %f %f\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);

  //ecriture des aretes
  for (int i = 0; i < m->nbedge; i++)
    fprintf (os, "%d %d\n", m->ed[i].ve1, m->ed[i].ve2);

  //ecriture des faces
  for (int i = 0; i < m->nbface; i++)
    fprintf (os, "%d %d %d\n", m->fa[i].ed1, m->fa[i].ed2, m->fa[i].ed3);

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Enregistrement d'un modèle dans un fichier gts
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_save_gts (
		   const char * const file,
		   const vef_model * const m)
{
  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture des nombre de sommets, aretes et faces
  fprintf (os, "%d %d %d\n\n", m->nbvertex, m->nbedge, m->nbface);

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%f %f %f\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);

  //ecriture des aretes
  for (int i = 0; i < m->nbedge; i++)
    fprintf (os, "%d %d\n", m->ed[i].ve1 + 1, m->ed[i].ve2 + 1);

  //ecriture des faces
  for (int i = 0; i < m->nbface; i++)
    fprintf (os, "%d %d %d\n", m->fa[i].ed1 + 1, m->fa[i].ed2 + 1,
	     m->fa[i].ed3 + 1);

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Enregistrement d'un modèle dans un fichier off
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_save_off (
		   const char * const file,
		   const vef_model * const m)
{
  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture de l'en-tete et des nombres de sommets et faces
  fprintf (os, "OFF\n%d %d 0\n\n", m->nbvertex, m->nbface);

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%f %f %f\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);

  //ecriture des faces
  for (int i = 0; i < m->nbface; i++)
    {
      int ve1,
        ve2,
        ve3;
      vef_face_get_vertices (&(m->fa[i]), m->ed, &ve1, &ve2, &ve3);
      fprintf (os, "3 %d %d %d \n", ve1, ve2, ve3);
    }

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Enregistrement d'un modèle dans un fichier ply
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_save_ply (
		   const char * const file,
		   const vef_model * const m)
{
  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture de l'en-tete avec le nombre de sommets et de faces
  fprintf (os, "ply\nformat ascii 1.0\ncomment zipper output\n");
  fprintf (os,
	   "element vertex %d\nproperty float x\nproperty float y\nproperty float z\nelement face %d\n",
	   m->nbvertex, m->nbface);
  fprintf (os, "property list uchar int vertex_indices\nend_header\n");

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "%f %f %f\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);

  //ecriture des faces
  for (int i = 0; i < m->nbface; i++)
    {
      int ve1,
        ve2,
        ve3;
      vef_face_get_vertices (&(m->fa[i]), m->ed, &ve1, &ve2, &ve3);
      fprintf (os, "3 %d %d %d \n", ve1, ve2, ve3);
    }

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;
}

/**
   Enregistrement d'un modèle dans un fichier obj
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_save_obj (
		   const char * const file,
		   const vef_model * const m)
{
  setlocale(LC_NUMERIC,"C");

  FILE *os = NULL;
  os = fopen (file, "w");
  if (os == NULL)
    return 0;

  //ecriture des sommets
  for (int i = 0; i < m->nbvertex; i++)
    fprintf (os, "v %f %f %f\n", m->ve[i].x, m->ve[i].y, m->ve[i].z);

  fprintf (os, "\n");

  //ecriture des faces
  for (int i = 0; i < m->nbface; i++)
    {
      int ve1,
        ve2,
        ve3;
      vef_face_get_vertices (&(m->fa[i]), m->ed, &ve1, &ve2, &ve3);
      fprintf (os, "f %d %d %d\n", ve1 + 1, ve2 + 1, ve3 + 1);
    }

  fclose (os);

  setlocale(LC_NUMERIC,"");

  return 1;
}
