/*
 * =============================================================================
 * Fichier : network.h
 * Projet  : C-talk - Application de messagerie en C
 * ----------------------------------------------------------------------------
 * Description :
 *   Déclarations des fonctions et structures pour la gestion des sockets et
 *   des connexions réseau.
 *
 * ----------------------------------------------------------------------------
 * Auteur(s) : FortyTwo Dev
 * Date de création : 05/10/2025
 * Dernière modification : 05/10/2025
 * =============================================================================
 */

#ifndef C_TALK_NETWORK_H
#define C_TALK_NETWORK_H

#include <stdint.h>
#include <sys/socket.h>

#define DEFAULT_PORT 8080
#define MAX_BUFFER_SIZE 1024

typedef struct {
    int socket;
    struct sockaddr_in address;
} client_connection_t;

int init_server(uint16_t port);
int accept_client_connection(int server_socket);
void send_message(int socket, const char *message);

#endif // C_TALK_NETWORK_H