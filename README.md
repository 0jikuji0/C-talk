# C-talk Documentation

Cette première partie du document décrit les conventions de style à suivre pour le projet **C-talk** en C, afin d'assurer la cohérence et la lisibilité du code.

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

- **Vérifiez toujours les valeurs de retour** des appels système (ex: `socket`, `bind`, `send`, `recv`).
- **Utilisez `<errno.h>` et `strerror`** pour afficher des messages d'erreur détaillés et personnalisés.
- **Exemple de base** :
  ```c
  #include <errno.h>
  #include <string.h>

  if (send(socket, message, strlen(message), 0) == -1) {
      fprintf(stderr, "Erreur lors de l'envoi du message : %s\n", strerror(errno));
      return -1;
  }
  ```

---

## 1. **Architecture Globale**

Cette deuxième partie du document décrit l'architecture globale de C-talk

### **Objectifs clés :**
- Chaque machine doit pouvoir **envoyer** et **recevoir** des messages en même temps.
- Gestion de plusieurs clients connectés (pour les discussions de groupe).
- Code modulaire pour faciliter la maintenance et l’évolution.

### **Composants principaux :**
| Composant               | Rôle                                                                 |
|-------------------------|----------------------------------------------------------------------|
| **Serveur**             | Écoute les connexions entrantes et gère les messages reçus.         |
| **Client**              | Se connecte à d’autres machines pour envoyer des messages.           |
| **Gestionnaire de réseau** | Gère les sockets, les connexions et les communications.            |
| **Interface utilisateur** | Affiche les messages et permet à l’utilisateur d’en envoyer.       |
| **Gestion des utilisateurs** | Stocke les informations des utilisateurs connectés (optionnel).   |

---

## 2. **Détail des Modules**

### **A. Module Serveur**
- **Rôle** : Écouter sur un port spécifique et accepter les connexions entrantes.
- **Fonctionnalités** :
  - Créer une socket serveur.
  - Accepter les connexions entrantes.
  - Lancer un thread (ou processus) pour chaque client connecté.
  - Recevoir les messages et les redistribuer aux autres clients connectés.

### **B. Module Client**
- **Rôle** : Se connecter à d’autres machines (serveurs) pour envoyer des messages.
- **Fonctionnalités** :
  - Créer une socket client.
  - Se connecter à une adresse IP et un port spécifiques.
  - Envoyer des messages à d’autres machines.

### **C. Module Gestionnaire de Réseau**
- **Rôle** : Centraliser la logique de communication (envoi/réception).
- **Fonctionnalités** :
  - Gérer les sockets (création, fermeture, erreurs).
  - Envoyer et recevoir des messages via les sockets.
  - Gérer les threads/processus pour les connexions simultanées.

### **D. Module Interface Utilisateur**
- **Rôle** : Permettre à l’utilisateur d’interagir avec l’application.
- **Fonctionnalités** :
  - Afficher les messages reçus.
  - Permettre à l’utilisateur de saisir des messages à envoyer.
  - (Optionnel) Interface en ligne de commande ou graphique (avec ncurses par exemple).

### **E. Module Gestion des Utilisateurs**
- **Rôle** : (Optionnel) Stocker les informations des utilisateurs connectés.
- **Fonctionnalités** :
  - Stocker les adresses IP et ports des utilisateurs connectés.
  - Gérer les pseudos ou identifiants.

---

## 3. **Flux de Communication**

1. **Lancement de l’application** :
   - Le programme démarre en mode **serveur** (écoute sur un port).
   - L’utilisateur peut également se connecter à d’autres machines en mode **client**.

2. **Envoi de message** :
   - L’utilisateur saisit un message.
   - Le module client envoie le message à la machine cible.

3. **Réception de message** :
   - Le module serveur reçoit le message et l’affiche à l’utilisateur.

4. **Gestion des connexions multiples** :
   - Utilisation de threads ou de `fork()` pour gérer plusieurs clients simultanément.

---

## 4. **Exemple de Structure de Fichiers**

```bash
c-talk/
├── include/
│   ├── server.h
│   ├── client.h
│   ├── network.h
│   └── ui.h
├── src/
│   ├── server.c
│   ├── client.c
│   ├── network.c
│   └── ui.c
├── Makefile
└── README.md
```