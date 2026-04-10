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

#include "conversion.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

/********** MAIN FUNCTIONS **********/

/**
   Conversion d'un format de fichier en un autre. Le format des fichiers est définis par leurs extensions.
   @param input chemin du fichier entrant
   @param output chemin du fichier sortant
   @return aucun
*/
void
a2ri_vf_conversion (
		    const char * const input,
		    const char * const output)
{
  vf_model m;

  //initialisation d'un nouveau modèle
  a2ri_vf_init (&m);

  //ouverture du modèle input
  a2ri_vf_open_file (input, &m);

  //enregistrement du modèle output
  a2ri_vf_save_file (output, &m);

  a2ri_vf_free (&m);

  return;

}
