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

typedef struct NewSocket {
    int e;
} NewSocket;



// Utilisation de enum(Mode)
int create_socket(enum Mode mode);

int init_socket(enum Mode mode);
void param_socket(int socket);
void attach_address(int socket);

int init_server(uint16_t port);
int accept_client_connection(int server_socket);
void send_message(int socket, const char *message);

#endif // C_TALK_NETWORK_H
