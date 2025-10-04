#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Import expliqué plus bas
#include <arpa/inet.h>


// Le code de la dernière erreur déclenchée par le système
// contenue dans la variable errno
#include <errno.h>

// Sockets in C
// https://tala-informatique.fr/index.php?title=C_socket

// Importer sys/socket.h et netinet/in.h
#include <sys/socket.h>
#include <netinet/in.h>

/* Pour Coté Serveur
 * 1. int socket(int domain, int type, int protocol)
 * 2. int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
 * 3. int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
 * 4. int listen(int sockfd, int backlog)
 * 5. int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
*/

/* Pour Coté Client
 * 1. int socket(int domain, int type, int protocol)
 * 2. int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
 */

// Fonctions de lecture/écriture
// Importer unistd.h
#include <unistd.h>

/*
 * Lecture: ssize_t read(int fd, void *buf, size_t count);
 * Ecriture: ssize_t write(int fd, const void *buf, size_t count);
 * OU
 * Lecture: ssize_t recv(int fd, void *buf, size_t count, int flags);
 * Ecriture: ssize_t send(int fd, const void *buf, size_t count, int flags);
 *
 * Différence à voir sur le site..
*/


int main() {
    // -- D'abord créer "le descripteur de fichier"
    int fdsocket;
    if ((fdsocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    	printf("Echec de la création: %s\n", strerror(errno));
    	exit(EXIT_FAILURE);
    }

    // -- Configurer la réutilisation de l'adresse et du port
    int opt = 1;
    if (setsockopt(fdsocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,	sizeof(opt)) != 0) {
    	printf("Echec de paramètrage: %s\n", strerror(errno));
    	exit(EXIT_FAILURE);
    }


    // -- Attacher le socket à un port et une adresse
    struct sockaddr_in adresse;

    adresse.sin_family = AF_INET;
    // Ecoute sur toutes les adresses (INADDR_ANY <=> 0.0.0.0)
    adresse.sin_addr.s_addr = INADDR_ANY;
    // Conversion du port en valeur réseaux (Host TO Network Short)
    adresse.sin_port = htons(8080);

    if (bind(fdsocket, (struct sockaddr *) &adresse, sizeof(adresse)) != 0) {
    	printf("Echec d'attachement: %s\n", strerror(errno));
    	exit(EXIT_FAILURE);
    }

    // -- Passage à l'écoute des connexions des clients
    // Taille de la file d'attente
    const int BACKLOG = 3;

    if (listen(fdsocket, BACKLOG) != 0) {
	printf("Echec de la mise en écoute: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    // -- Acceptation des connexions entrantes

    int clientSocket;
    // Structure contenant l'adresse du client
    struct sockaddr_in clientAdresse;
    unsigned int addrLen = sizeof(clientAdresse);
    if ((clientSocket = accept(fdsocket, (struct sockaddr *) &clientAdresse, &addrLen)) != -1) {
    	// Convertion de l'IP en texte
        char ip[INET_ADDRSTRLEN];

        // Import de arpa/inet.h pour la fonction inet_ntop
    	inet_ntop(AF_INET, &(clientAdresse.sin_addr), ip, INET_ADDRSTRLEN);
    	printf("Connexion de %s:%i\n", ip, clientAdresse.sin_port);
    }

    // ========= MODE TCP ==========
    //
    // -- Envoyer et lire un message
    // Descripteur de la socket du client
    // int clientSocket;
    const int BUFFER_LEN = 200;
    char buffer[BUFFER_LEN];

    int len = recv(clientSocket, buffer, BUFFER_LEN, MSG_DONTWAIT);

    send(clientSocket, "Coucou\n", strlen("Coucou\n"), MSG_DONTWAIT);

    // -- Gérer le retour de recv
    // int len = recv(clientSocket, buffer, BUFFER_LEN, MSG_DONTWAIT);

    if (len == -1 && errno != EAGAIN) {
	// Une erreur est survenue
    } else if (len == 0) {
	// Le client s'est déconnecté (extrémité de la socket fermée)
    } else if (len > 0) {
	// Le client à envoyé des données
    }

    // ========= MODE UDP ==========
    //
    // Lire: ssize_t recvfrom(int fd, void *buf, size_t count, int flags, const struct sockaddr *addr, socklen_t addrlen);
    // Ecrire: ssize_t sendto(int fd, const void *buf, size_t count, const struct sockaddr *addr, socklen_t addrlen);


    return 0;
}
