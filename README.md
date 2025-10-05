# Guide de Style pour C-talk

Ce document décrit les conventions de style à suivre pour le projet **C-talk** en C, afin d'assurer la cohérence et la lisibilité du code.

---

## 1. Nommage

### Variables et Fonctions
- **Style** : `snake_case`
- **Exemples** :
  ```c
  int client_socket;
  char * message_buffer[256];
  void send_message(int socket, const char *message);
  ```

### Constantes et Macros
- **Style** : `UPPER_SNAKE_CASE`
- **Exemples** :
  ```c
  #define MAX_BUFFER_SIZE 1024
  #define DEFAULT_PORT 8080
  ```

### Structures et Types Personnalisés
- **Style** : `snake_case` pour les noms de structures, avec `typedef` pour simplifier l'utilisation.
- **Exemples** :
  ```c
  typedef struct {
      int id;
      char username[32];
  } user_t;
  ```

---

## 2. Organisation du Code

### Fichiers d'En-tête (`.h`)
- Utilisez des gardes d'inclusion :
  ```c
  #ifndef C_TALK_NETWORK_H
  #define C_TALK_NETWORK_H

  void send_message(int socket, const char *message);

  #endif // C_TALK_NETWORK_H
  ```

### Fichiers Source (`.c`)
- Documentez chaque fonction avec un commentaire au-dessus de sa définition :
  ```c
  /**
   * Envoie un message via une socket.
   * @param socket : Descripteur de la socket.
   * @param message : Message à envoyer.
   * @return : 0 en cas de succès, -1 en cas d'erreur.
   */
  int send_message(int socket, const char *message) {
      // Implémentation...
  }
  ```

---

## 3. Indentation et Formatage
- **Indentation** : tabulations.
- **Accolades** : Ouvrante sur la même ligne, fermante sur une nouvelle ligne.
  ```c
  void handle_client(int socket) {
      char buffer[1024];
      if (recv(socket, buffer, sizeof(buffer), 0) <= 0) {
          close(socket);
          return;
      }
      printf("Message reçu : %s\n", buffer);
  }
  ```

- **Espaces** :
  - Après les mots-clés (`if`, `for`, `while`).
  - Pas d'espace avant une parenthèse ouvrante.
  ```c
  if (condition) {
        do_something();
    }
  ```


---

## 4. Commentaires
- Utilisez `/* */` pour les commentaires multi-lignes et `//` pour les commentaires courts.
- **Exemple** :
  ```c
  /* Initialise le serveur et écoute sur le port spécifié. */
  int init_server(unsigned int port) {
      // Création de la socket...
  }
  ```

---

## 5. Gestion des Erreurs
- Vérifiez toujours les valeurs de retour des appels système.
- Utilisez `perror` pour afficher des messages d'erreur :
  ```c
  if (send(socket, message, strlen(message), 0) == -1) {
      perror("Erreur lors de l'envoi du message");
      return -1;
  }
  ```
