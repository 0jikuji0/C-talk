/*
 * =============================================================================
 * Fichier : server.c
 * Projet  : C-talk - Application de messagerie en C
 * ----------------------------------------------------------------------------
 * Description :
 *   Implémentation des fonctions pour le module serveur.
 *   Écoute les connexions entrantes et redistribue les messages aux clients.
 * ----------------------------------------------------------------------------
 * Auteur(s) : FortyTwo Dev
 * Date de création : 05/10/2025
 * Dernière modification : 05/10/2025
 * =============================================================================
 */

#include "../include/server.h"
#include "../include/network.h"
#include "../include/threads.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define LOG_ERROR(msg) fprintf(stderr, "[ERROR] (SERVER) %s : %s (code: %d)\n", (msg), strerror(errno), errno); exit(EXIT_FAILURE);

ServerSocket init_server(uint16_t port) {
    // Utilisation, pour l'instant, du port par défaut
    // pour écouter en TCP
    Socket sock = create_socket(TCP, port);

    // TODO créer un thread afin que ça ne bloque pas le programme.
    ThreadPool thr_pool = create_thread_pool();

    // Modifier le comportement pour supporter le multi-threading ?
    s_listen(sock.socket);
    Socket client_sock = s_accept(sock.socket);

    // TODO socket UDP pour signaler la room au réseau
    // listening_loop() en UDP dans un thread dédié

    ServerSocket server_socket = newServerSocket(&sock, &client_sock);

    return server_socket;
}

// --- Routine UDP (Signalement) ---
void* udp_beacon_thread(void* arg) {
    // On récupère le port depuis l'argument (ou une constante)
    uint16_t port = *((uint16_t*)arg);

    // Création du socket UDP via votre fonction existante
    Socket beacon_sock = create_socket(UDP, port); // Assurez-vous que create_socket gère le SO_BROADCAST

    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    dest.sin_addr.s_addr = INADDR_BROADCAST; // 255.255.255.255

    char message[] = "C-TALK_SERVER_PRESENCE";

    while (1) {
        if (sendto(beacon_sock.socket, message, strlen(message), 0,
                   (struct sockaddr *)&dest, sizeof(dest)) < 0) {
            perror("UDP Broadcast failed");
        }
        sleep(2); // Signale toutes les 2 secondes
    }
    return NULL;
}

// --- Routine TCP (Gestion Client) ---
void* client_handler_thread(void* arg) {
    Thread* self = (Thread*)arg;
    int sock = self->context.socket_fd;

    printf("Nouveau client traité sur le thread %lu\n", self->thread_id);

    // Boucle de réception des messages
    while (1) {
        char* msg = receive_message_server(sock);
        if (msg == NULL) {
            // Déconnexion détectée
            break;
        }
        printf("[Client %d] : %s\n", sock, msg);

        // Exemple : Echo du message (ou broadcast aux autres clients plus tard)
        send_message_server(sock, "Reçu 5/5");

        free(msg);
    }

    close(sock);
    // TODO : Ici, il faudrait appeler remove_from_pool() pour se retirer proprement
    return NULL;
}

void run_server_loop(uint16_t port) {
    // 1. Initialisation du Socket d'écoute TCP
    Socket listener_sock = create_socket(TCP, port);
    s_listen(listener_sock.socket);

    // 2. Création du Thread Pool
    ThreadPool pool = create_thread_pool();

    // 3. Lancement du Thread UDP (Beacon)
    pthread_t udp_thread;
    uint16_t port_arg = port;
    if (pthread_create(&udp_thread, NULL, udp_beacon_thread, &port_arg) != 0) {
        LOG_ERROR("Impossible de créer le thread UDP");
    }

    printf("Serveur démarré. En attente de connexions...\n");

    // 4. Boucle Principale : Accepter les clients TCP
    while (1) {
        // Cette fonction est bloquante jusqu'à ce qu'un client arrive
        Socket new_client = s_accept(listener_sock.socket);

        // Préparation du nouveau thread client
        Thread* thread_struct = malloc(sizeof(Thread));
        thread_struct->context.socket_fd = new_client.socket;
        thread_struct->context.address = new_client.address;
        thread_struct->is_active = 1;

        // Lancement du thread pour ce client
        if (pthread_create(&thread_struct->thread_id, NULL, client_handler_thread, thread_struct) != 0) {
            fprintf(stderr, "Erreur création thread client\n");
            close(new_client.socket);
            free(thread_struct);
            continue;
        }

        // Ajout au pool
        add_to_pool(&pool, thread_struct);

        // On détache le thread pour que ses ressources soient libérées automatiquement à la fin
        pthread_detach(thread_struct->thread_id);
    }
}

char* receive_message_server(int connected_socket_fd) {
    char* message_ptr = receive_message(connected_socket_fd);

    if (message_ptr == NULL) {
        LOG_ERROR("Echec de la reception du message du client");
    }

    return message_ptr;
}

int send_message_server(int connected_socket_fd, char message[]) {
    int send_bytes = send(connected_socket_fd, message, strlen(message), 0);

    if (send_bytes == -1) {
        LOG_ERROR("Echec de l'envoie du message au client");
    }

    return 0;
}

int close_server(ServerSocket socket) {
    close_server_socket(&socket);
    return 0;
}

void free_server(ServerSocket server_socket) {
    free_socket(server_socket.connection);
    free_socket(server_socket.listener);
}
