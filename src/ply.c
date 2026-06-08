/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 08/06/26       */
/* Date de modification : 08/06/26   */
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

#include "ply.h"

/********** INTERMEDIATE FUNCTIONS **********/

uint32_t read_uint32(FILE *f)
{
  uint8_t b[4];
  fread(b, 1, 4, f);
  return (uint32_t)b[0]
    | ((uint32_t)b[1] << 8)
    | ((uint32_t)b[2] << 16)
    | ((uint32_t)b[3] << 24);
}

float read_float(FILE *f)
{
  float v;
  fread(&v, sizeof(float), 1, f);
  return v;
}

/********** MAIN FUNCTIONS **********/

/**
   Ouverture d'un fichier ply contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_load_ply_ascii (
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


int a2ri_vf_load_ply_binary(
			    const char *filename,
			    vf_model * m)
{
  FILE *f = fopen(filename, "rb");
  if (!f) {
    perror("fopen");
    return 0;
  }
  
  char line[256];
  int vertex_count = 0;
  int face_count = 0;
  double coordx,
    coordy,
    coordz;
  int ve1,
    ve2,
    ve3;


  // Lecture du header
  while (fgets(line, sizeof(line), f)) {

    if (sscanf(line, "element vertex %d", &vertex_count) == 1) {
      continue;
    }

    if (sscanf(line, "element face %d", &face_count) == 1) {
      continue;
    }

    if (strncmp(line, "end_header", 10) == 0) {
      break;
    }
  }

  // Lecture des sommets
  for (int i = 0; i < vertex_count; i++) {
    coordx = read_float(f);
    coordy = read_float(f);
    coordz = read_float(f);
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
    
  // Lecture des faces
  for (int i = 0; i < face_count; i++) {
      
    uint8_t count;
    fread(&count, 1, 1, f);
    a2ri_erreur_critique_si(count != 3, "PLY binaire - Seuls les maillages TRIANGULAIRES sont pris en charge.");

    ve1 = read_uint32(f);
    ve2 = read_uint32(f);
    ve3 = read_uint32(f);
    
    a2ri_vf_add_face (m, ve1, ve2, ve3);
  }
    
  fclose(f);
  return 1;
}


/**
   Ouverture d'un fichier ply contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vf_load_ply(
		 const char * const file,
		 vf_model * m)
{
  FILE *is = NULL;
  char str[128];
  
  setlocale(LC_NUMERIC,"C");
  
  is = fopen (file, "r");
  if (is == NULL)
    return 0;
  
  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"PLY - Erreur en ouverture de fichier");
  
  if (strcmp (str, "ply"))
    {
      printf("PLY - Magic number non trouvé pour le fichier %s\n",file);
      return 0;
    }

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"PLY - erreur en lecture du format de fichier");

  if (strcmp (str, "format"))
    {
      printf("PLY - format non trouvé pour le fichier %s\n",file);
      return 0;
    }

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"PLY - erreur en lecture du format de fichier");

  if (!strcmp (str, "ascii"))
    return a2ri_vf_load_ply_ascii(file, m);

  if (!strncmp(str, "format binary_little_endian", 27) == 0)
    return a2ri_vf_load_ply_binary(file, m);

  return 0;
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
   Ouverture d'un fichier ply contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_load_ply_ascii (
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

int a2ri_vef_load_ply_binary(
			    const char *filename,
			    vef_model * m)
{
  FILE *f = fopen(filename, "rb");
  if (!f) {
    perror("fopen");
    return 0;
  }
  
  char line[256];
  int vertex_count = 0;
  int face_count = 0;
  double coordx,
    coordy,
    coordz;
  int ve1,
    ve2,
    ve3,
    ed1,
    ed2,
    ed3;

  // Lecture du header
  while (fgets(line, sizeof(line), f)) {

    if (sscanf(line, "element vertex %d", &vertex_count) == 1) {
      continue;
    }

    if (sscanf(line, "element face %d", &face_count) == 1) {
      continue;
    }

    if (strncmp(line, "end_header", 10) == 0) {
      break;
    }
  }

  // Lecture des sommets
  for (int i = 0; i < vertex_count; i++) {
    coordx = read_float(f);
    coordy = read_float(f);
    coordz = read_float(f);
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
    
  // Lecture des faces
  for (int i = 0; i < face_count; i++) {
      
    uint8_t count;
    fread(&count, 1, 1, f);
    a2ri_erreur_critique_si(count != 3, "PLY binaire - Seuls les maillages TRIANGULAIRES sont pris en charge.");
      
    ve1 = read_uint32(f);
    ve2 = read_uint32(f);
    ve3 = read_uint32(f);
  

    a2ri_vef_add_edge (m, ve1, ve2, 1);
    a2ri_vef_add_edge (m, ve2, ve3, 1);
    a2ri_vef_add_edge (m, ve3, ve1, 1);
    //recherche des aretes
    ed1 = a2ri_vef_search_edge (m, ve1, ve2);
    ed2 = a2ri_vef_search_edge (m, ve2, ve3);
    ed3 = a2ri_vef_search_edge (m, ve3, ve1);
    //ajout de la faces avec les trois aretes
    a2ri_vef_add_face (m, ed1, ed2, ed3);
  }
  
  fclose(f);
  return 1;
}

/**
   Ouverture d'un fichier ply contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int
a2ri_vef_load_ply(
		  const char * const file,
		  vef_model * m)
{
  FILE *is = NULL;
  char str[128];
  
  setlocale(LC_NUMERIC,"C");
  
  is = fopen (file, "r");
  if (is == NULL)
    return 0;
  
  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"erreur en ouverture de fichier");
  
  if (strcmp (str, "ply"))
    {
      printf("PLY - Magic number non trouvé pour le fichier %s\n",file);
      return 0;
    }

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"PLY - erreur en lecture du format de fichier");

  if (strcmp (str, "format"))
    {
      printf("PLY - format non trouvé pour le fichier %s\n",file);
      return 0;
    }

  a2ri_erreur_critique_si(!fscanf (is, "%s", str),"PLY - erreur en lecture du format de fichier");

  if (!strcmp (str, "ascii"))
    return a2ri_vef_load_ply_ascii(file, m);

  if (!strncmp(str, "format binary_little_endian", 27) == 0)
    return a2ri_vef_load_ply_binary(file, m);

  return 0;
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
