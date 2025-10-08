/*
 * =============================================================================
 * Fichier : server.h
 * Projet  : C-talk - Application de messagerie en C
 * ----------------------------------------------------------------------------
 * Description :
 *   Prototypes des fonctions du module serveur pour la gestion des connexions
 *   entrantes et la communication avec les clients.
 * ----------------------------------------------------------------------------
 * Auteur(s) : FortyTwo Dev
 * Date de création : 05/10/2025
 * Dernière modification : 06/10/2025
 * =============================================================================
 */

#ifndef C_TALK_SERVER_H
#define C_TALK_SERVER_H

#include "network.h"
#include <stdint.h>

#define DEFAULT_LISTENING_PORT 31026
#define DEFAULT_PENDING_QUEUE_MAX_LENGHT 1
#define DEFAULT_BUFFER_SIZE 1024

/** @brief Structure représentant un socket serveur et son socket connectée. */
typedef struct
{
    int socket_fd;
    int connected_socket_fd;
} server_socket;

/**
 * @brief Initialise un socket serveur et la configure pour écouter les connexions entrantes.
 *
 * @param port Le numéro de port sur lequel le serveur écoutera.
 * @param pending_queue_max_length Longueur maximale de la file d'attente pour les connexions en attente.
 * @return server_socket Structure contenant les descripteurs de socket en cas de succès.
 *
 * @note En cas d'échec, un message d'erreur est affiché via `LOG_ERROR`.
 *
 * @example
 *     server_socket server = initialize_server(DEFAULT_LISTENING_PORT, DEFAULT_PENDING_QUEUE_MAX_LENGHT);
 */
server_socket initialize_server(uint16_t port, uint16_t pending_queue_max_length);


/**
 * @brief Reçoit un message depuis un socket connectée (client) et le stocke dans un buffer.
 *
 * @param connected_socket_fd Descripteur de socket connectée (client).
 *
 * @note En cas d'erreur, un message est affiché via `LOG_ERROR`.
 *
 * @example
 *     char* msg_ptr = receive_message_server(server.connected_socket_fd);
 */
char* receive_message_server(int connected_socket_fd);

/**
 * @brief Envoie un message via un socket connectée (client).
 *
 * @param connected_socket_fd Descripteur de socket connectée (client).
 * @param message Message à envoyer (chaîne de caractères).
 * @return int -1 en cas d'échec.
 *
 * @note En cas d'échec, un message d'erreur est affiché via `LOG_ERROR`.
 *
 * @example
 *     int result = send_message_server(server.connected_socket_fd, "Bonjour, client !");
 */
int send_message_server(int connected_socket_fd, char message[]);

/**
 * @brief Ferme proprement un socket serveur et ses ressources associées.
 *
 * @param server_socket Structure contenant les descripteurs de socket à fermer.
 * @return int 0 en cas de succès.
 *
 * @note En cas d'échec, un message d'erreur est affiché via `LOG_ERROR`.
 *
 * @example
 *     int result = close_server(server_socket);
 */
int close_server(ServerSocket socket);

#endif // C_TALK_SERVER_H
