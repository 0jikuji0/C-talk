// gcc $(pkg-config --cflags --libs gtk4) src/ui.c -o ui

#include "../include/client.h"
#include "../include/server.h"
#include "../include/crypto.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <time.h>
#include <string.h>

static GtkWidget *main_window = NULL;
static GtkWidget *text_view = NULL;
static GtkTextBuffer *text_buffer = NULL;
static GtkWidget *entry = NULL;
static GtkWidget *ip_input = NULL;
static GtkWidget *port_input = NULL;
pthread_t server_tid;

extern uint64_t private_key;

int has_exechange = 0;
Socket socket_;

void *server_thread(void *arg)
{
  run_server_loop(DEFAULT_LISTENING_PORT);
  return NULL;
}

char *get_current_time(void)
{
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  static char buffer[9];
  strftime(buffer, 9, "%H:%M:%S", tm_info);
  return buffer;
}

void send_message_(char *plaintext)
{

  // if (!has_exechange)
  // {
  //   uint64_t p, g, secret_key, public_key;
  //   publicParams(&p, &g);
  //   privateParams(&secret_key);

  //   send_public_key(socket_, p, g, secret_key);
  //   has_exechange++;
  // }

  char *ciphertext = NULL;

  printf("[Shared Key] %zu\n", private_key);
  printf("Votre message: ");

  if (plaintext == NULL)
  {
    free(plaintext);
  }
  encrypt(plaintext, &ciphertext, private_key);
  send_message_client(socket_.socket, ciphertext);

  free(ciphertext);
  ciphertext = NULL;
}

// Fonction pour ajouter un message dans la zone de discussion
void add_message(const char *username, const char *message, gboolean is_own_message)
{
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(text_buffer, &iter);

  char *time_str = get_current_time();
  char *formatted = g_strdup_printf("[%s] %s: %s\n", time_str, username, message);

  gtk_text_buffer_insert(text_buffer, &iter, formatted, -1);

  // Appliquer le tag de couleur
  GtkTextIter start;
  gtk_text_buffer_get_iter_at_line(text_buffer, &start, gtk_text_buffer_get_line_count(text_buffer) - 2);
  gtk_text_buffer_get_end_iter(text_buffer, &iter);

  if (is_own_message)
    gtk_text_buffer_apply_tag_by_name(text_buffer, "own", &start, &iter);
  else
    gtk_text_buffer_apply_tag_by_name(text_buffer, "other", &start, &iter);

  g_free(formatted);

  // Scroll auto vers le bas
  GtkTextMark *mark = gtk_text_buffer_get_insert(text_buffer);
  gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(text_view), mark, 0.0, TRUE, 0.0, 1.0);
}

// Fonction appelée quand on envoie un message
void on_send_clicked(GtkButton *button, gpointer user_data)
{
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *text = gtk_entry_buffer_get_text(buffer);

  

  if (text != NULL && strlen(text) > 0)
  {
    
    add_message("Vous", text, TRUE);
    gtk_entry_buffer_set_text(buffer, "", 0);
    char * text_;
    sscanf(text, "%s", text_);
    send_message_(text_);
  }
}

// Fonction appelée quand on appuie sur Entrée
void on_entry_activate(GtkEntry *entry_widget, gpointer user_data)
{
  on_send_clicked(NULL, user_data);
}

// Fonction Quitter
void on_quit_activate(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
  GtkApplication *app = GTK_APPLICATION(user_data);
  g_application_quit(G_APPLICATION(app));
}

// Fonction À propos
void on_about_activate(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
  const char *authors[] = {
      "jikuji",
      "FortyTwo_dev",
      "Jensen",
      NULL};

  gtk_show_about_dialog(
      main_window ? GTK_WINDOW(main_window) : NULL,
      "program-name", "C-Talk",
      "version", "1.0.0",
      "comments", "Application de discussion",
      "authors", authors,
      "logo-icon-name", "system-users",
      NULL);
}

// Fonction pour récupérer les valeurs des inputs
void on_get_input_clicked(GtkButton *button, gpointer user_data)
{
  const char *ip = gtk_editable_get_text(GTK_EDITABLE(ip_input));
  const char *port = gtk_editable_get_text(GTK_EDITABLE(port_input));

  if (ip != NULL && strlen(ip) > 0 && port != NULL && strlen(port) > 0)
  {
    char *message = g_strdup_printf("Connexion à %s:%s", ip, port);
    add_message("Système", message, FALSE);
    g_free(message);

    int port_;
    char ip_[16]; 

    sscanf(port, "%d", &port_);
    strncpy(ip_, ip, sizeof(ip_)-1);
    ip_[sizeof(ip_)-1] = '\0';


    printf("[Client] 1\n");
    socket_ = initialize_client(port_, ip_);
    uint64_t p, g, secret_key, public_key;
    publicParams(&p, &g);
    privateParams(&secret_key);

    send_public_key(socket_, p, g, secret_key);

    // close_socket(socket);
    // free_socket(socket);
    // pthread_join(server_tid, NULL);
  }
  else
  {
    add_message("Erreur", "Veuillez remplir tous les champs", FALSE);
  }
}

// Fonction Effacer la conversation
void on_clear_activate(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
  gtk_text_buffer_set_text(text_buffer, "", 0);
  add_message("Systeme", "Conversation effacée", FALSE);
}

// Fonction pour démarrer le serveur
void on_start_server_clicked(GtkButton *button, gpointer user_data)
{
  // Afficher un message de statut
  add_message("Serveur", "Démarrage du serveur...", FALSE);

  // Changer le label du bouton
  gtk_button_set_label(button, "Serveur actif");
  gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);

  printf("[Server] 1\n");
  if (pthread_create(&server_tid, NULL, server_thread, NULL) != 0)
  {
    perror("Erreur création thread serveur");
    exit(EXIT_FAILURE);
  }
  // TODO: Appeler la fonction de démarrage du serveur
  // start_server();
  add_message("Serveur", "Bien demarrer", FALSE);
}

// Actions de l'application
GActionEntry app_actions[] = {
    {"about", on_about_activate},
    {"clear", on_clear_activate},
    {"quit", on_quit_activate}};

// Fonction de démarrage
void on_startup(GtkApplication *app, gpointer user_data)
{
  const char *quit_accels[2] = {"<Ctrl>Q", NULL};
  gtk_application_set_accels_for_action(app, "app.quit", quit_accels);
}

// Créer les tags pour le formatage du texte
void create_text_tags(void)
{
  gtk_text_buffer_create_tag(text_buffer, "own",
                             "foreground", "#2196F3",
                             "weight", PANGO_WEIGHT_BOLD,
                             NULL);

  gtk_text_buffer_create_tag(text_buffer, "other",
                             "foreground", "#4CAF50",
                             NULL);
}

// Fonction d'activation (création de l'interface)
void on_activate(GtkApplication *app, gpointer user_data)
{
  g_action_map_add_action_entries(G_ACTION_MAP(app), app_actions,
                                  G_N_ELEMENTS(app_actions), app);

  main_window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(main_window), "Chat Application");
  gtk_window_set_default_size(GTK_WINDOW(main_window), 900, 600);

  // Créer la HeaderBar
  GtkWidget *header = gtk_header_bar_new();
  gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header), TRUE);

  // Titre de la HeaderBar
  GtkWidget *title_label = gtk_label_new("Chat");
  gtk_widget_add_css_class(title_label, "title");
  gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header), title_label);

  // Bouton menu
  GtkWidget *menu_button = gtk_menu_button_new();
  gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(menu_button), "view-more-symbolic");

  // Créer le menu
  GMenu *menu = g_menu_new();
  GMenu *section1 = g_menu_new();
  g_menu_append(section1, "Effacer la conversation", "app.clear");
  g_menu_append_section(menu, NULL, G_MENU_MODEL(section1));

  GMenu *section2 = g_menu_new();
  g_menu_append(section2, "À propos", "app.about");
  g_menu_append(section2, "Quitter", "app.quit");
  g_menu_append_section(menu, NULL, G_MENU_MODEL(section2));

  gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(menu_button), G_MENU_MODEL(menu));
  gtk_header_bar_pack_end(GTK_HEADER_BAR(header), menu_button);

  // Bouton pour démarrer le serveur
  GtkWidget *server_button = gtk_button_new_with_label("Démarrer serveur");
  gtk_widget_add_css_class(server_button, "suggested-action");
  g_signal_connect(server_button, "clicked", G_CALLBACK(on_start_server_clicked), NULL);
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), server_button);

  gtk_window_set_titlebar(GTK_WINDOW(main_window), header);

  // === ZONE DE CHAT ===
  GtkWidget *chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  // === ZONE DE CONFIGURATION (IP/Port) ===
  GtkWidget *config_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
  gtk_widget_set_margin_top(config_box, 12);
  gtk_widget_set_margin_bottom(config_box, 12);
  gtk_widget_set_margin_start(config_box, 12);
  gtk_widget_set_margin_end(config_box, 12);

  // Label IP
  GtkWidget *ip_label = gtk_label_new("IP :");
  gtk_box_append(GTK_BOX(config_box), ip_label);

  // Input IP
  ip_input = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(ip_input), "127.0.0.1");
  gtk_widget_set_hexpand(ip_input, TRUE);
  gtk_box_append(GTK_BOX(config_box), ip_input);

  // Label Port
  GtkWidget *port_label = gtk_label_new("Port :");
  gtk_box_append(GTK_BOX(config_box), port_label);

  // Input Port
  port_input = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(port_input), "8080");
  gtk_widget_set_size_request(port_input, 100, -1);
  gtk_box_append(GTK_BOX(config_box), port_input);

  // Bouton Connexion
  GtkWidget *connect_button = gtk_button_new_with_label("Connecter");
  g_signal_connect(connect_button, "clicked", G_CALLBACK(on_get_input_clicked), NULL);
  gtk_box_append(GTK_BOX(config_box), connect_button);

  gtk_box_append(GTK_BOX(chat_box), config_box);

  // Séparateur
  GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_box_append(GTK_BOX(chat_box), sep);

  // Zone de texte avec scroll
  GtkWidget *scrolled = gtk_scrolled_window_new();
  gtk_widget_set_vexpand(scrolled, TRUE);
  gtk_widget_set_hexpand(scrolled, TRUE);

  text_view = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), 10);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view), 10);
  gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_view), 10);
  gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_view), 10);

  text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  create_text_tags();

  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), text_view);
  gtk_box_append(GTK_BOX(chat_box), scrolled);

  // Séparateur
  GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_box_append(GTK_BOX(chat_box), separator);

  // Zone de saisie en bas
  GtkWidget *input_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_widget_set_margin_top(input_box, 12);
  gtk_widget_set_margin_bottom(input_box, 12);
  gtk_widget_set_margin_start(input_box, 12);
  gtk_widget_set_margin_end(input_box, 12);

  entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Tapez votre message...");
  gtk_widget_set_hexpand(entry, TRUE);
  g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), NULL);
  gtk_box_append(GTK_BOX(input_box), entry);

  GtkWidget *send_button = gtk_button_new_with_label("Envoyer");
  gtk_widget_add_css_class(send_button, "suggested-action");
  g_signal_connect(send_button, "clicked", G_CALLBACK(on_send_clicked), NULL);
  gtk_box_append(GTK_BOX(input_box), send_button);

  gtk_box_append(GTK_BOX(chat_box), input_box);

  gtk_window_set_child(GTK_WINDOW(main_window), chat_box);

  // Message de bienvenue
  add_message("Système", "Bienvenue dans le chat !", FALSE);

  gtk_window_present(GTK_WINDOW(main_window));
}

int main(int argc, char *argv[])
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.example.chat", G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect(app, "startup", G_CALLBACK(on_startup), NULL);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

  status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);

  return status;
}