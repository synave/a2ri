/*************************************/
/* Auteur : Rémi Synave              */
/* Date de création : 10/03/10       */
/* Date de modification : 18/09/16   */
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

#ifndef IO__H
#define IO__H

#include <locale.h>
#include "model.h"
#include "skeleton.h"

/**
   Ouverture d'un fichier représentant un squelette
   @param file chemin vers le fichier à ouvrir
   @param s pointeur vers le squelette
   @return 1 si succès, 0 sinon
 */
int a2ri_skeleton_open_file(
			   const char * const file,
			   skeleton * s);

/**
   Ouverture d'un fichier contenant un modèle
   @param file chemin du fichier à ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_open_file (
		       const char * const file,
		       vf_model * m);

/**
   Ouverture d'un fichier asc contenant un nuage de points
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_load_asc (
		      const char * const file,
		      vf_model * m);

/**
   Ouverture d'un fichier vef contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_load_vef (
		      const char * const file,
		      vf_model * m);

/**
   Ouverture d'un fichier gts contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_load_gts (
		      const char * const file,
		      vf_model * m);

/**
   Ouverture d'un fichier pgn contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_load_pgn (
		      const char * const file,
		      vf_model * m);

/**
   Ouverture d'un fichier off contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_load_off (
		      const char * const file,
		      vf_model * m);

/**
   Ouverture d'un fichier ply contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_load_ply (
		      const char * const file,
		      vf_model * m);

/**
   Ouverture d'un fichier wrl contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_load_wrl (
		      const char * const file,
		      vf_model * m);

/**
   Ouverture d'un fichier obj contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_load_obj (
		      const char * const file,
		      vf_model * m);

/**
   Ouverture d'un fichier stl contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_load_stl (
		      const char * const file,
		      vf_model * m);

/**
   Ouverture d'un fichier stl binaire contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_load_binary_stl (
			     const char * const file,
			     vf_model * m);

/**
   Ouverture d'un fichier stl ascii contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_load_ascii_stl (
			    const char * const file,
			    vf_model * m);

/**
   Enregistrement d'un modèle dans un format défini par l'extension du nom de fichier
   @param file nom du fichier
   @param m le modèle
   @return aucun
*/
int a2ri_vf_save_file (
		       const char * const file,
		       const vf_model * const m);

/**
   Enregistrement d'un modèle dans un fichier vef
   @param file chemin du fichier
   @param m modèle à  sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_save_vef (
		      const char * const file,
		      const vf_model * const m);

/**
   Enregistrement d'un modèle dans un fichier gts
   @param file chemin du fichier
   @param m modèle à  sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_save_gts (
		      const char * const file,
		      const vf_model * const m);

/**
   Enregistrement d'un modèle dans un fichier pgn
   @param file chemin du fichier
   @param m modèle à  sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_save_pgn (
		      const char * const file,
		      const vf_model * const m);

/**
   Enregistrement d'un modèle dans un fichier off
   @param file chemin du fichier
   @param m modèle à  sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_save_off (
		      const char * const file,
		      const vf_model * const m);

/**
   Enregistrement d'un modèle dans un fichier ply
   @param file chemin du fichier
   @param m modèle à  sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_save_ply (
		      const char * const file,
		      const vf_model * const m);

/**
   Enregistrement d'un modèle dans un fichier wrl
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_save_wrl (
		      const char * const file,
		      const vf_model * const m);

/**
   Enregistrement d'un modèle dans un fichier wrl version 1.0
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_save_wrl_v1 (
			 const char * const file,
			 const vf_model * const m);

/**
   Enregistrement d'un modèle dans un fichier obj
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_save_obj (
		      const char * const file,
		      const vf_model * const m);

/**
   Enregistrement d'un modèle dans un fichier stl binaire
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_save_binary_stl (
		      const char * const file,
		      const vf_model * const m);

/**
   Enregistrement d'un modèle dans un fichier stl ascii
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vf_save_ascii_stl (
		      const char * const file,
		      const vf_model * const m);

/**
   Ouverture d'un fichier contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_open_file (
			const char * const file,
			vef_model * m);

/**
   Ouverture d'un fichier vef contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_load_vef (
		       const char * const file,
		       vef_model * m);

/**
   Ouverture d'un fichier gts contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_load_gts (
		       const char * const file,
		       vef_model * m);

/**
   Ouverture d'un fichier pgn contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_load_pgn (
		       const char * const file,
		       vef_model * m);

/**
   Ouverture d'un fichier stl contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_load_stl (
		       const char * const file,
		       vef_model * m);

/**
   Ouverture d'un fichier off contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_load_off (
		       const char * const file,
		       vef_model * m);

/**
   Ouverture d'un fichier ply contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_load_ply (
		       const char * const file,
		       vef_model * m);

/**
   Ouverture d'un fichier wrl contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_load_wrl (
		       const char * const file,
		       vef_model * m);

/**
   Ouverture d'un fichier obj contenant un modèle
   @param file chemin du fichier à  ouvrir
   @param m pointeur sur le modèle
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_load_obj (
		       const char * const file,
		       vef_model * m);

/**
   Enregistrement d'un modèle dans un format défini par l'extension du nom de fichier
   @param file nom du fichier
   @param m le modèle
   @return aucun
*/
int a2ri_vef_save_file (
			const char * const file,
			const vef_model * const m);

/**
   Enregistrement d'un modèle dans un fichier vef
   @param file chemin du fichier
   @param m modèle à  sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_save_vef (
		       const char * const file,
		       const vef_model * const m);

/**
   Enregistrement d'un modèle dans un fichier gts
   @param file chemin du fichier
   @param m modèle à  sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_save_gts (
		       const char * const file,
		       const vef_model * const m);

/**
   Enregistrement d'un modèle dans un fichier pgn
   @param file chemin du fichier
   @param m modèle à  sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_save_pgn (
		       const char * const file,
		       const vef_model * const m);

/**
   Enregistrement d'un modèle dans un fichier off
   @param file chemin du fichier
   @param m modèle à  sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_save_off (
		       const char * const file,
		       const vef_model * const m);

/**
   Enregistrement d'un modèle dans un fichier ply
   @param file chemin du fichier
   @param m modèle à  sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_save_ply (
		       const char * const file,
		       const vef_model * const m);

/**
   Enregistrement d'un modèle dans un fichier wrl
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_save_wrl (
		       const char * const file,
		       const vef_model * const m);

/**
   Enregistrement d'un modèle dans un fichier obj
   @param file chemin du fichier
   @param m modèle à sauvegarder
   @return 1 si succès, 0 sinon
*/
int a2ri_vef_save_obj (
		       const char * const file,
		       const vef_model * const m);

#endif
