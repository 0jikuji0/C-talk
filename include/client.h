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

#include "network.h"
#include <stdint.h>

#define DEFAULT_CONNECTION_HOST "192.168.1.36"
#define DEFAULT_LISTENING_PORT 31026
#define MAX_BUFFER_SIZE 1024

/**
 * @brief Initialise un socket client et établit une connexion avec un serveur distant.
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
Socket initialize_client(uint16_t port, char * connection_host);

/**
 * @brief Reçoit un message depuis un socket client et le stocke dans un buffer.
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
 * @brief Envoie un message via un socket client.
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
int send_message_client(int socket_fd, char* message);

/**
 * @brief Récupère les caractères écrits dans STDIN et renvoie un pointeur vers la chaîne de caractère
 *
 * @return char* Chaîne de caractère. Si rien n'est reçu, retourne NULL.
 *
 *
 * @example
 *     char* message = get_message();
 *     // Utilisation de message
 *
 *     // Libération de la mémoire de message
 *     free(message);
 *     message = NULL;
 */
char* get_message();

/**
 * @brief Ferme proprement un socket client et libère les ressources associées.
 *
 * @param client_socket Descripteur de socket client à fermer.
 * @return int 0 en cas de succès.
 *
 * @note En cas d'échec, un message d'erreur est affiché via `LOG_ERROR`.
 *
 * @example
 *     int result = close_client(sock);
 */
int close_client(Socket client_socket);

#endif // C_TALK_CLIENT_H
