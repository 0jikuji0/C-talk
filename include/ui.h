/*
 * =============================================================================
 * Fichier : ui.h
 * Projet  : C-talk - Application de messagerie en C
 * ----------------------------------------------------------------------------
 * Description :
 *   Déclarations des fonctions pour l'interface utilisateur.
 *   Affiche les messages et permet à l'utilisateur d'interagir avec l'application.
 * ----------------------------------------------------------------------------
 * Auteur(s) : FortyTwo Dev
 * Date de création : 05/10/2025
 * Dernière modification : 05/10/2025
 * =============================================================================
 */

#ifndef C_TALK_UI_H
#define C_TALK_UI_H

void display_welcome_message(void);
void display_message(const char *sender, const char *message);
char *get_user_input(void);

#endif // C_TALK_UI_H