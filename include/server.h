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

#define DEFAULT_LISTENING_PORT 31026
#define DEFAULT_PENDING_QUEUE_MAX_LENGHT 1
#define DEFAULT_BUFFER_SIZE 1024

typedef struct
{
    int socket_fd;
    int connected_socket_fd;
} server_socket;

server_socket initialize_server(uint16_t port, uint16_t pending_queue_max_length);
void receive_message_server(int connected_socket_fd, uint32_t buffer_size, char *buffer);
int send_message_server(int connected_socket_fd, char message[]);
int close_server(server_socket server_socket);

#endif // C_TALK_SERVER_H