/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 01/10/08       */
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



#ifndef SHAPEMATCHING__H
#define SHAPEMATCHING__H

#include "model.h"
#include "util.h"
#include "point.h"
#include "geometry.h"

/**
   Calcul de l'empreinte d'un modèle
   @param m le modèle
   @param nb_tranche nombre de tranche de l'empreinte
   @param list liste de retour de l'empreinte
   @return aucun
**/
void a2ri_vf_empreinte (
			const vf_model * const m,
			int nb_tranche,
			double **list);

#endif
