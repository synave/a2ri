/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 24/02/09       */
/* Date de modification : 08/01/10   */
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



#ifndef EPAISSISSEMENT__H
#define EPAISSISSEMENT__H

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
#include "subdivision.h"


/**
   Epaississement de la surface d'un maillage triangulaire
   @param m le modèle à épaissir
   @param epaisseur épaisseur de la surface souhaitée
   @return aucun
**/
void a2ri_vf_epaissi_surface (
			      vf_model * m,
			      double epaisseur);


#endif
