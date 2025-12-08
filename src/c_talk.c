#include "../include/client.h"
#include "../include/server.h"
#include "../include/crypto.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

extern uint64_t private_key;

void *server_thread(void *arg)
{
    run_server_loop(DEFAULT_LISTENING_PORT);
    return NULL;
}

int main()
{
    pthread_t server_tid;
    char value;
    printf("Server s ou Client c en premier ?\n");
    scanf(" %c", &value);

    if (value == 's') {
        printf("[Server] 1\n");
        if (pthread_create(&server_tid, NULL, server_thread, NULL) != 0)
        {
            perror("Erreur création thread serveur");
            return 1;
        }
    }

    scanf(" %c", &value);

    if (value == 'c')
    {
        printf("[Client] 1\n");
        Socket socket = initialize_client(DEFAULT_LISTENING_PORT, DEFAULT_CONNECTION_HOST);
    
        uint64_t p, g, secret_key, public_key;
        publicParams(&p, &g);
        privateParams(&secret_key);
    
        send_public_key(socket, p, g, secret_key);

        scanf(" %c", &value);

        if (value == 's')
        {
            printf("[Server] 2\n");
            if (pthread_create(&server_tid, NULL, server_thread, NULL) != 0)
            {
                perror("Erreur création thread serveur");
                return 1;
            }
        }
        
        char *ciphertext = NULL;
    
        for (;;)
        {
            printf("[Shared Key] %zu\n", private_key);
            printf("Votre message: ");
    
            char *plaintext = get_message();
    
            if (plaintext == NULL)
            {
                free(plaintext);
                continue;
            }
            encrypt(plaintext, &ciphertext, private_key);
            send_message_client(socket.socket, ciphertext);
    
            free(ciphertext);
            ciphertext = NULL;
        }
    
        close_socket(socket);
        free_socket(socket);
        pthread_join(server_tid, NULL);
        return 0;
    }
}
