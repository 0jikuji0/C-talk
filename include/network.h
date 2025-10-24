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

#include <netinet/in.h>
#include <stdint.h>
#include <sys/socket.h>

#define DEFAULT_PORT 8080
#define MAX_BUFFER_SIZE 1024

// On va ne fonctionner qu'en IPV4, donc on crée un socket avec AF_INET par défaut

/*
typedef struct {
    int socket;
    struct sockaddr_in address;
} client_connection_t;
 */

/** @brief Enum ayant pour but de simplifier les type de sockets utilisés. */
enum Mode {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM
};

/** @brief Type représentant un socket et son adresse. */
typedef struct {
    int socket;
    struct sockaddr_in* address;
} Socket;

/** @brief Type représentant un serveur de socket
 *
(1 qui écoute les nouvelles connexions, et 1 qui communique avec le client).
*/
typedef struct {
    Socket listener;
    Socket connection;
} ServerSocket;

// Création d'un struct

/**
 * @brief Crée un socket et le retourne.
 *
 * @param socket Descripteur de socket client.
 * @param address* Pointeur vers l'adresse du socket.
 *
 * @note En cas d'erreur, un message est affiché via `LOG_ERROR`.
 *
 * @example
 *  Socket my_socket = newSocket(socketfd, *address);
 */
Socket newSocket(int socket, struct sockaddr_in* address);

/**
 * @brief Crée un struct ServerSocket et le retourne.
 *
 * @param Socket listener qui écoute les nouvelles connexions.
 * @param Socket connection qui est le socket de la connexion établie entre le client et le serveur.
 *
 * @note En cas d'erreur, un message est affiché via `LOG_ERROR`.
 *
 * @example
 * ServerSocket my_server_socket = newServerSocket(listener, connection);
 */
ServerSocket newServerSocket(Socket* listening_socket, Socket* client_socket);

/**
 * @brief Initie un descripteur de fichier socket et retourne un struct Socket.
 *
 * @param Mode mode le mode de transmission de données (TCP, UDP).
 * @param uint16_t port le port que le socket écoute.
 *
 * @note En cas d'erreur, un message est affiché via `LOG_ERROR`.
 *
 * @example
 * Socket my_socket = create_socket(UDP, 42069);
 */
Socket create_socket(enum Mode mode, uint16_t port);

// Fonctions de sockets serveurs initialisés
void s_listen(int socket);
Socket s_accept(int socket);

// Initialisation de socket
int init_socket(enum Mode mode);
void param_socket(int socket);
struct sockaddr_in* attach_address(int socket, uint16_t port);

// Fermeture de socket
int close_socket(Socket* socket);
void close_server_socket(ServerSocket* socket);

ServerSocket init_server(uint16_t port);
int send_message(int socket, const char *message);
char* receive_message(int socket);

// Fonctions utilitaires
void get_ip_str(struct sockaddr_in client_address, char* buff);

#endif // C_TALK_NETWORK_H
