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

enum Mode {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM
};

typedef struct ClientSocket {
    int socket;
    struct sockaddr_in address;
} ClientSocket;

typedef struct Socket {
    int socket;
    void (*s_listen)(int);
    ClientSocket (*s_accept)(int);
} Socket;

typedef struct ServerSocket {
    Socket listening_socket;
    ClientSocket client_socket;
} ServerSocket;

// Création d'un struct
Socket NewSocket(int socket);
ClientSocket NewClientSocket(int socket, struct sockaddr_in address);

// Utilisation de enum(Mode)
Socket create_socket(enum Mode mode, uint16_t port);

// Fonctions de sockets serveurs initialisés
void s_listen(int socket);
ClientSocket s_accept(int socket);

// Initialisation de socket
int init_socket(enum Mode mode);
void param_socket(int socket);
void attach_address(int socket, uint16_t port);

void close_server_socket(ServerSocket socket);

ServerSocket init_server(uint16_t port);
int send_message(int socket, const char *message);
char* receive_message(int socket);

// Fonctions utilitaires
void get_ip_str(struct sockaddr_in client_address, char* buff);

#endif // C_TALK_NETWORK_H
