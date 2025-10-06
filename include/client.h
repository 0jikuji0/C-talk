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

/** @brief Type représentant une socket client. */
typedef struct
{
    int socket_fd;
} client_socket;

/**
 * @brief Initialise une socket client et établit une connexion avec un serveur distant.
 *
 * @param port Le numéro de port du serveur distant.
 * @param connection_host L'adresse IP du serveur distant.
 * @return client_socket Descripteur de socket en cas de succès.
 *
 * @note En cas d'échec, un message d'erreur est affiché via `LOG_ERROR`.
 *
 * @example
 *     client_socket sock = initialize_client(8080, "127.0.0.1");
 */
client_socket initialize_client(uint16_t port, char * connection_host);

/**
 * @brief Reçoit un message depuis une socket client et le stocke dans un buffer.
 *
 * @param socket_fd Descripteur de socket client.
 * @param buffer_size Taille maximale du buffer pour stocker le message.
 * @param buffer Buffer où stocker le message reçu.
 *
 * @note En cas d'erreur, un message est affiché via `LOG_ERROR`.
 *
 * @example
 *     char buffer[1024];
 *     receive_message_client(sock, 1024, buffer);
 */
void receive_message_client(int socket_fd, uint32_t buffer_size, char *buffer);

/**
 * @brief Envoie un message via une socket client.
 *
 * @param socket_fd Descripteur de socket client.
 * @param message Message à envoyer (chaîne de caractères).
 * @return int Nombre d'octets envoyés en cas de succès, -1 en cas d'échec.
 *
 * @note En cas d'échec, un message d'erreur est affiché via `LOG_ERROR`.
 *
 * @example
 *     int result = send_message_client(sock, "Bonjour, monde !");
 */
int send_message_client(int socket_fd, char message[]);

/**
 * @brief Ferme proprement une socket client et libère les ressources associées.
 *
 * @param client_socket Descripteur de socket client à fermer.
 * @return int 0 en cas de succès.
 *
 * @note En cas d'échec, un message d'erreur est affiché via `LOG_ERROR`.
 *
 * @example
 *     int result = close_client(sock);
 */
int close_client(client_socket client_socket);

#endif // C_TALK_CLIENT_H