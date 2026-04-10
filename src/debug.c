/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 12/03/10       */
/* Date de modification : 12/03/10   */
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

#include "debug.h"

/********** INTERMEDIATE TYPES AND FUNCTIONS **********/
/* Les fonctions intermédiaires sont préfixées de IF  */
/* et les types intermédiaires de IT                  */

/********** MAIN FUNCTIONS **********/


/**
   Affichage d'un message en mode debug
   @param str message
   @return aucun
**/
void a2ri_printf_debug (
			char str[], ...)
{  
  va_list args;
  
  va_start(args,str);
  
#ifdef _DEBUG
  vfprintf(stderr,str,args);
#endif
  va_end(args);
  str=NULL;
}
