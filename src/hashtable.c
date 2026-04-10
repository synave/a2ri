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

#include "hashtable.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

int
IFhash (
      char *key)
{
  char *p = key;
  int h = *p;

  if (h)
    {
      for (p += 1; *p != '\0'; p++)
	{
	  h = (h << 5) - h + *p;
	}
    }

  return h;
}

/********** MAIN FUNCTIONS **********/

/**
   Création d'une table de hachage
   @param size taille de la table de hachage
   @return la nouvelles table de hachage
**/
hashtable *
hashtable_new (
	       int size)
{
  hashtable *table = (hashtable *) malloc (sizeof (hashtable));

  a2ri_erreur_critique_si (table == NULL,
			"erreur allocation memoire pour table\nhashtable_new");
  table->size_array = size;
  table->count = 0;
  table->list = (container **) malloc (size * sizeof (container *));
  a2ri_erreur_critique_si (table->list == NULL,
			"erreur allocation memoire pour table->list\nhashtable_new");
  for (int i = 0; i < size; i++)
    table->list[i] = NULL;
  return table;
}


/**
   Destruction de la table de hachage avec libération de la mémoire
   @param table la table de hachage
   @return aucun;
**/
void
hashtable_free (
		hashtable * table)
{
  container *parc,
    *prev;

  for (int i = 0; i < table->size_array; i++)
    {
      parc = table->list[i];
      prev = parc;

      while (parc != NULL)
	{
	  vf_edge_free (parc->data);
	  free (parc->data);
	  prev = parc;
	  parc = parc->next;
	  free (prev);
	}
    }
  free (table->list);
}


/**
   Ajout d'une arete dans la table de hachage
   @param table la table de hachage
   @param e l'arete a ajouter
   @return aucun
**/
void
hashtable_add (
	       hashtable * table,
	       vf_edge * e)
{
  char temp[20];

  if (e->ve1 < e->ve2)
    sprintf (temp, "%d,%d", e->ve1, e->ve2);
  else
    sprintf (temp, "%d,%d", e->ve2, e->ve1);

  int index = abs (IFhash (temp) % table->size_array);

  container *parc = table->list[index];

  if (parc == NULL)
    {
      table->list[index] = (container *) malloc (sizeof (container));
      a2ri_erreur_critique_si (table->list[index] == NULL,
			    "erreur allocation memoire pour table->list[index]\nhashtable_add");
      parc = table->list[index];
      //TODO modifier ici pour garantir le const de l'arete
      //creer une nouvelle arete pour parc->data et copier les infos de l'arete e
      parc->data = e;      
      parc->next = NULL;
    }
  else
    {
      while (parc->next != NULL)
	parc = parc->next;
      parc->next = (container *) malloc (sizeof (container));
      a2ri_erreur_critique_si (parc->next == NULL,
			    "erreur allocation memoire pour parc->next\nhashtable_add");
      //TODO idem voir ci-dessus
      parc = parc->next;
      parc->data = e;
      parc->next = NULL;
    }

  table->count = table->count + 1;
}


/**
   Recherche d'une arete dans la table de hachage
   @param ve1 premier sommet de l'arete
   @param ve2 second sommet de l'arete
   @return l'arete si elle se trouve dans la table de hachage, NULL sinon.
**/
vf_edge *
hashtable_look_for (
		    const hashtable * const table,
		    int ve1,
		    int ve2)
{
  char variable[300];

  a2ri_erreur_critique_si (variable == NULL,
			"erreur allocation memoire pour temp\nhashtable_look_for\n");
  variable[0] = 0;

  int trouve = 0;

  if (ve1 < ve2)
    sprintf (variable, "%d,%d", ve1, ve2);
  else
    sprintf (variable, "%d,%d", ve2, ve1);

  int index = abs (IFhash (variable) % table->size_array);

  container *parc = table->list[index];

  while (!trouve && parc != NULL)
    {
      if ((parc->data->ve1 == ve1 && parc->data->ve2 == ve2)
	  || (parc->data->ve1 == ve2 && parc->data->ve2 == ve1))
	trouve = 1;
      else
	parc = parc->next;
    }


  if (trouve)
    return parc->data;
  else
    return NULL;

}

/**
   Affichage d'une table de hachage
   @param table la table de hachage
   @return aucun
**/
void
hashtable_display (
		   const hashtable * const table)
{
  printf ("***** TABLE DE HACHAGE *****\n");
  for (int i = 0; i < table->size_array; i++)
    {
      printf ("cle : %d\n", i);
      container *parc = table->list[i];
      while (parc != NULL)
	{
	  vf_edge_display (parc->data);
	  parc = parc->next;
	}
      printf ("\n");
    }
  printf ("****************************\n");
}


/**
   Retourne la taille de la table, le nombre d'éléments
   @param table la table de hachage
   @return aucun
**/
int
hashtable_size (
		const hashtable * const table)
{
  return table->count;
}

/**
   Applique la fonction passé en paramètre à tous les éléments contenus dans la table
   @param table la table de hachage
   @param func la fonction à appliquer
   @param user_data un pointeur vers des données que l'on veut passé en paramètre à la fonction
   @return aucun
**/
void
hashtable_foreach (
		   hashtable * table,
		   ptf_foreach func,
		   void *user_data)
{
  for (int i = 0; i < table->size_array; i++)
    {
      container *parc = table->list[i];
      while (parc != NULL)
	{
	  func (i, parc->data, user_data);
	  parc = parc->next;
	}
    }
}
