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



#ifndef ICP__H
#define ICP__H

#include <string.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <math.h>
#include <pthread.h>
#include "model.h"
#include "matrix.h"
#include "point.h"
#include "util.h"
#include "overlap.h"
#include "geometry.h"
#include "space_partition.h"

/**
   Recalage de deux modeles
   @param P modele à recaler
   @param X premier modele servant de base
   @param dkarret critère d'arret.
   @return aucun
**/
void a2ri_vf_icp (
		  vf_model * P,
		  const vf_model * const X,
		  double dkarret);

/**
   Recalage de deux modeles
   @param P modele à recaler
   @param X premier modele servant de base
   @param recouvrement estimation du taux de recouvrement
   @param dkarret critère d'arret.
   @return aucun
**/
void a2ri_vf_trimmed_icp (
			  vf_model * P,
			  const vf_model * const X,
			  double recouvrement,
			  double dkarret);

/**
   Recalage automatique de deux modeles
   @param P modele à recaler
   @param X premier modele servant de base
   @param dkarret critère d'arret.
   @param sensibility sensiblité du taux de recouvrement
   @return aucun
**/
//REMARQUE : le maillage devrait etre qualifié const mais impossible a cause de l'utilisation de la focntion a2ri_vf_bounding_ball_minimale_compute_overlap qui utilise des threads et ne permet donc pas la qualifidation de const du maillage.
void a2ri_vf_automated_trimmed_icp (
				    vf_model * P,
				    vf_model * X,
				    double dkarret,
				    double sensibility);

/**
   Recalage automatique de deux modeles
   @param P modele à recaler
   @param X premier modele servant de base
   @param dkarret critère d'arret.
   @return aucun
**/
//REMARQUE : le maillage devrait etre qualifié const mais impossible a cause de l'utilisation de thread.
void a2ri_vf_automated_trimmed_icp_pulli (
					  vf_model * P,
					  vf_model * X,
					  double dkarret);

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
void a2ri_vf_automated_trimmed_icp_multiple (
					     vf_model ** P,
					     vf_model ** X,
					     int sizeP,
					     int sizeX,
					     double dkarret,
					     double sensibility);

#endif
