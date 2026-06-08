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

#ifndef PLY__H
#define PLY__H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <locale.h>
#include "model.h"

int a2ri_vf_load_ply(
		     const char * const file,
		     vf_model * m);

int a2ri_vef_load_ply(
		      const char * const file,
		      vef_model * m);

#endif
