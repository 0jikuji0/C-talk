/*
 * =============================================================================
 * Fichier : server.h
 * Projet  : C-talk - Application de messagerie en C
 * ----------------------------------------------------------------------------
 * Description :
 *   Déclarations des fonctions et structures pour le module serveur.
 *   Gère l'écoute des connexions entrantes et la réception des messages.
 * ----------------------------------------------------------------------------
 * Auteur(s) : FortyTwo Dev
 * Date de création : 05/10/2025
 * Dernière modification : 05/10/2025
 * =============================================================================
 */

#ifndef C_TALK_SERVER_H
#define C_TALK_SERVER_H

#include <stdint.h>

#define DEFAULT_PORT 8080
#define MAX_CLIENTS 10

int init_server(uint16_t port);
void handle_incoming_connections(int server_socket);
void broadcast_message(int *client_sockets, int sender_socket, const char *message);

#endif // C_TALK_SERVER_H