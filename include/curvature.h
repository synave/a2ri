/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 29/05/26       */
/* Date de modification : 29/05/26   */
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



#ifndef CURVATURE__H
#define CURVATURE__H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "model.h"
#include "util.h"
#include "vector.h"
#include "geometry.h"

/**
   Calcul de la courbure gaussienne discrète pour un sommet d'un modèle.
   @param m le modèle
   @param num_sommet le numéro du sommet
   @return la courbure
 */
double a2ri_vf_courbure_gaussienne_discrete(vf_model *m, int num_sommet);

#endif
