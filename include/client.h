/*
 * =============================================================================
 * Fichier : client.h
 * Projet  : C-talk - Application de messagerie en C
 * ----------------------------------------------------------------------------
 * Description :
 *   Déclarations des fonctions et structures pour le module client.
 *   Gère la connexion aux serveurs distants et l'envoi de messages.
 * ----------------------------------------------------------------------------
 * Auteur(s) : FortyTwo Dev
 * Date de création : 05/10/2025
 * Dernière modification : 05/10/2025
 * =============================================================================
 */

#ifndef C_TALK_CLIENT_H
#define C_TALK_CLIENT_H

#include <stdint.h>
#include <sys/socket.h>

int connect_to_server(const char *ip, uint16_t port);
void send_message_to_server(int socket, const char *message);
void receive_messages_from_server(int socket);

#endif // C_TALK_CLIENT_H