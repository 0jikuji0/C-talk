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
typedef enum Mode {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM
} Mode;

/** @brief Type représentant un socket, son adresse et son mode de transmission. */
#ifndef DEFINE_SOCKET_STRUCT
#define DEFINE_SOCKET_STRUCT
typedef struct Socket{
    int socket;
    struct sockaddr_in address;
    Mode mode;
} Socket;
#endif
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
Socket newSocket(int socket, struct sockaddr_in address, Mode mode);

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
Socket create_socket(Mode mode, uint16_t port);

// Libération de socket
void free_socket(Socket socket);

// Fonctions de sockets serveurs initialisés
void s_listen(int socket);
Socket s_accept(int socket);

// Initialisation de socket
int init_socket(Mode mode);
void param_socket(int socket, Mode mode);
struct sockaddr_in attach_address(int socket, uint16_t port);

// Fermeture de socket
int close_socket(Socket socket);
void close_server_socket(ServerSocket* socket);

ServerSocket init_server(uint16_t port);
int send_message(int socket, const char *message);
char* receive_message(int socket);

// Fonctions utilitaires
void get_ip_str(struct sockaddr_in client_address, char* buff);

/**
 * @brief Envoie la clé publique calculée à partir des paramètres Diffie-Hellman.
 *
 * @param socket Un socket serveur ou client.
 * @param p Le nombre premier utilisé pour le calcul de la clé publique.
 * @param g Le générateur utilisé pour le calcul de la clé publique.
 * @param secret_key La clé secrète locale utilisée pour calculer la clé publique.
 *
 * @note En cas d'erreur lors de l'envoi, un message est affiché via `LOG_ERROR`.
 */
void send_public_key(Socket socket, uint64_t p, uint64_t g, uint64_t secret_key);

/**
 * @brief Génère la clé privée partagée à partir de la clé publique reçue et de la clé secrète locale,
 *        en utilisant l'algorithme Diffie-Hellman.
 *
 * @param socket Un socket serveur ou client.
 * @param p Le nombre premier utilisé pour le calcul de la clé privée partagée.
 * @param secret_key La clé secrète locale utilisée pour le calcul.
 *
 * @return uint64_t La clé privée partagée calculée.
 *
 * @note En cas d'erreur (réception ou conversion), un message est affiché via `LOG_ERROR`.
 *
 */
uint64_t generate_private_key(int socket, uint64_t p, uint64_t secret_key);

#endif // C_TALK_NETWORK_H
