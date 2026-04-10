/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 05/07/08       */
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



#ifndef GRAPH__H
#define GRAPH__H

#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "vertex.h"
#include "edge.h"
#include "face.h"
#include "point.h"
#include "model.h"
#include "geometry.h"
#include "hashtable.h"
#include "boundingbox.h"
#include "space_partition.h"


/**
   Calcul du graphe Nearest Neighbour Graph (NNG)
   @param m le modele
   @return un modele contenant les aretes
*/
vef_model * a2ri_vef_nearest_neighbour_graph (
					     const vef_model * const m);

/**
   Calcul du graphe Gabriel (GG)
   @param m le modele
   @return un modele contenant les aretes
*/
vef_model * a2ri_vef_gabriel_graph (
				   const vef_model * const m);

/**
   Calcul du graphe etendu Gabriel (NNG)
   @param m le modele
   @return un modele contenant les aretes
*/
vef_model * a2ri_vef_extended_gabriel_hypergraph (
						 const vef_model * const m);


/**
   Calcul du graphe recouvrant minimal (EMST - Euclidean Minimal Spanning Tree)
   @param m le modele
   @return un modele contenant les aretes
*/
vef_model * a2ri_vef_euclidean_minimal_spanning_tree (
						     const vef_model * const m);


#endif
