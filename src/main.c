#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

char name[20];
int PORT;

void sending();
void receiving(int server_fd);
void *receive_thread(void *server_fd);

int main(int argc, char const *argv[])
{
    // ANSI color codes
    const char *RED = "\033[1;31m";
    const char *GREEN = "\033[1;32m";
    const char *YELLOW = "\033[1;33m";
    const char *CYAN = "\033[1;36m";
    const char *RESET = "\033[0m";

    printf("%s============================\n", CYAN);
    printf("   Bienvenue sur C-Talk!\n");
    printf("============================%s\n", RESET);
    printf("Entrez votre nom: ");
    scanf("%s", name);

    printf("Entrez votre numéro de port: ");
    scanf("%d", &PORT);

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int k = 0;

    // Création du socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("%sErreur: création du socket échouée%s\n", RED, RESET);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    printf("%sAdresse IP: %s\n", YELLOW, inet_ntoa(address.sin_addr));
    printf("Port: %d%s\n", (int)ntohs(address.sin_port), RESET);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("%sErreur: bind échoué%s\n", RED, RESET);
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0)
    {
        printf("%sErreur: listen échoué%s\n", RED, RESET);
        exit(EXIT_FAILURE);
    }
    int ch;
    pthread_t tid;
    pthread_create(&tid, NULL, &receive_thread, &server_fd);

    do {
        printf("%s\n========= MENU =========\n", CYAN);
        printf("1. Envoyer un message\n");
        printf("0. Quitter\n");
        printf("========================%s\n", RESET);
        printf("Votre choix: ");
        scanf("%d", &ch);
        switch (ch)
        {
            case 1:
                sending();
                break;
            case 0:
                printf("%s\nDéconnexion... Au revoir!%s\n", GREEN, RESET);
                break;
            default:
                printf("%s\nChoix invalide, réessayez.%s\n", RED, RESET);
        }
    } while (ch);

    close(server_fd);
    return 0;
}

//Sending messages to port
void sending()
{

    char buffer[2000] = {0};
    //Fetching port number
    int PORT_server;

    //IN PEER WE TRUST
    printf("Enter the port to send message:"); //Considering each peer will enter different port
    scanf("%d", &PORT_server);

    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char hello[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY always gives an IP of 0.0.0.0
    serv_addr.sin_port = htons(PORT_server);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return;
    }

    char dummy;
    printf("Enter your message:");
    scanf("%c", &dummy); //The buffer is our enemy
    scanf("%[^\n]s", hello);
    sprintf(buffer, "%s[PORT:%d] says: %s", name, PORT, hello);
    send(sock, buffer, sizeof(buffer), 0);
    printf("\nMessage sent\n");
    close(sock);
}

//Calling receiving every 2 seconds
void *receive_thread(void *server_fd)
{
    int s_fd = *((int *)server_fd);
    while (1)
    {
        sleep(2);
        receiving(s_fd);
    }
}

//Receiving messages on our port
void receiving(int server_fd)
{
    struct sockaddr_in address;
    int valread;
    char buffer[2000] = {0};
    int addrlen = sizeof(address);
    fd_set current_sockets, ready_sockets;

    //Initialize my current set
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    int k = 0;
    while (1)
    {
        k++;
        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {

                if (i == server_fd)
                {
                    int client_socket;

                    if ((client_socket = accept(server_fd, (struct sockaddr *)&address,
                                                (socklen_t *)&addrlen)) < 0)
                    {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    FD_SET(client_socket, &current_sockets);
                }
                else
                {
                    valread = recv(i, buffer, sizeof(buffer), 0);
                    printf("\n%s\n", buffer);
                    FD_CLR(i, &current_sockets);
                }
            }
        }

        if (k == (FD_SETSIZE * 2))
            break;
    }
}
