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

#define DEFAULT_CONNECTION_HOST "127.0.0.1"
#define DEFAULT_LISTENING_PORT 31026
#define DEFAULT_BUFFER_SIZE 1024

typedef struct
{
    int socket_fd;
} client_socket;

client_socket initialize_client(uint16_t port, char * connection_host);
void receive_message_client(int socket_fd, uint32_t buffer_size, char *buffer);
int send_message_client(int socket_fd, char message[]);
int close_client(client_socket client_socket);

#endif // C_TALK_CLIENT_H