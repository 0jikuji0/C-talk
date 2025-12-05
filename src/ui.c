#include <gtk/gtk.h>
#include <time.h>
#include <string.h>

static GtkWidget *main_window = NULL;
static GtkWidget *text_view = NULL;
static GtkTextBuffer *text_buffer = NULL;
static GtkWidget *entry = NULL;
static GtkWidget *sidebar = NULL;
static GtkWidget *sidebar_revealer = NULL;
static GtkWidget *conversations_list = NULL;
static GtkWidget *search_entry = NULL;
static char *current_contact = NULL;

// Structure pour stocker les contacts
typedef struct {
  char *name;
  char *last_message;
  gboolean online;
} Contact;

// Liste de contacts exemple
Contact contacts[] = {
  { "Alice Dupont", "Salut ! Comment vas-tu ?", TRUE },
  { "Bob Martin", "On se voit demain ?", TRUE },
  { "Charlie Petit", "Merci pour ton aide !", TRUE },
  { "Diana Rousseau", "J'ai envoyé le fichier", FALSE },
  { "Étienne Moreau", "À plus tard", TRUE }
};

static char *
get_current_time (void)
{
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  static char buffer[9];
  strftime(buffer, 9, "%H:%M:%S", tm_info);
  return buffer;
}

// Fonction pour ajouter un message dans la zone de discussion
static void
add_message (const char *username, const char *message, gboolean is_own_message)
{
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter (text_buffer, &iter);
  
  char *time_str = get_current_time();
  char *formatted = g_strdup_printf ("[%s] %s: %s\n", time_str, username, message);
  
  gtk_text_buffer_insert (text_buffer, &iter, formatted, -1);
  
  // Appliquer le tag de couleur
  GtkTextIter start;
  gtk_text_buffer_get_iter_at_line (text_buffer, &start, 
                                    gtk_text_buffer_get_line_count (text_buffer) - 2);
  gtk_text_buffer_get_end_iter (text_buffer, &iter);
  
  if (is_own_message)
    gtk_text_buffer_apply_tag_by_name (text_buffer, "own", &start, &iter);
  else
    gtk_text_buffer_apply_tag_by_name (text_buffer, "other", &start, &iter);
  
  g_free (formatted);
  
  // Scroll auto vers le bas
  GtkTextMark *mark = gtk_text_buffer_get_insert (text_buffer);
  gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW (text_view), mark, 0.0, TRUE, 0.0, 1.0);
}

// Fonction appelée quand on envoie un message
static void
on_send_clicked (GtkButton *button, gpointer user_data)
{
  GtkEntryBuffer *buffer = gtk_entry_get_buffer (GTK_ENTRY (entry));
  const char *text = gtk_entry_buffer_get_text (buffer);
  
  if (text != NULL && strlen(text) > 0) {
    add_message ("Vous", text, TRUE);
    gtk_entry_buffer_set_text (buffer, "", 0);
  }
}

// Fonction appelée quand on appuie sur Entrée
static void
on_entry_activate (GtkEntry *entry_widget, gpointer user_data)
{
  on_send_clicked (NULL, user_data);
}

// Fonction pour changer de conversation
static void
on_contact_selected (GtkListBox *box, GtkListBoxRow *row, gpointer user_data)
{
  if (row == NULL)
    return;
    
  int index = gtk_list_box_row_get_index (row);
  if (index >= 0 && index < G_N_ELEMENTS(contacts)) {
    if (current_contact)
      g_free (current_contact);
    current_contact = g_strdup (contacts[index].name);
    
    // Effacer et charger la nouvelle conversation
    gtk_text_buffer_set_text (text_buffer, "", 0);
    char *welcome = g_strdup_printf ("Conversation avec %s", current_contact);
    add_message ("Système", welcome, FALSE);
    g_free (welcome);
    
    // Mettre à jour le titre
    GtkWidget *header = gtk_window_get_titlebar (GTK_WINDOW (main_window));
    GtkWidget *title_widget = gtk_header_bar_get_title_widget (GTK_HEADER_BAR (header));
    char *title = g_strdup_printf ("💬 %s", current_contact);
    gtk_label_set_text (GTK_LABEL (title_widget), title);
    g_free (title);
  }
}

// Créer une ligne de contact
static GtkWidget *
create_contact_row (Contact *contact)
{
  GtkWidget *row_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
  gtk_widget_set_margin_top (row_box, 8);
  gtk_widget_set_margin_bottom (row_box, 8);
  gtk_widget_set_margin_start (row_box, 12);
  gtk_widget_set_margin_end (row_box, 12);
  
  // Indicateur en ligne (point vert/gris)
  GtkWidget *status = gtk_label_new (contact->online ? "🟢" : "⚫");
  gtk_box_append (GTK_BOX (row_box), status);
  
  // Infos du contact
  GtkWidget *info_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
  gtk_widget_set_hexpand (info_box, TRUE);
  
  GtkWidget *name_label = gtk_label_new (contact->name);
  gtk_widget_add_css_class (name_label, "heading");
  gtk_label_set_xalign (GTK_LABEL (name_label), 0);
  gtk_box_append (GTK_BOX (info_box), name_label);
  
  GtkWidget *msg_label = gtk_label_new (contact->last_message);
  gtk_label_set_xalign (GTK_LABEL (msg_label), 0);
  gtk_label_set_ellipsize (GTK_LABEL (msg_label), PANGO_ELLIPSIZE_END);
  gtk_widget_add_css_class (msg_label, "dim-label");
  gtk_widget_add_css_class (msg_label, "caption");
  gtk_box_append (GTK_BOX (info_box), msg_label);
  
  gtk_box_append (GTK_BOX (row_box), info_box);
  
  return row_box;
}

// Filtrer la liste des contacts
static gboolean
filter_contacts (GtkListBoxRow *row, gpointer user_data)
{
  const char *search_text = gtk_editable_get_text (GTK_EDITABLE (search_entry));
  
  if (search_text == NULL || strlen(search_text) == 0)
    return TRUE;
  
  int index = gtk_list_box_row_get_index (row);
  if (index >= 0 && index < G_N_ELEMENTS(contacts)) {
    char *name_lower = g_utf8_strdown (contacts[index].name, -1);
    char *search_lower = g_utf8_strdown (search_text, -1);
    gboolean match = strstr (name_lower, search_lower) != NULL;
    g_free (name_lower);
    g_free (search_lower);
    return match;
  }
  
  return FALSE;
}

// Callback pour la recherche
static void
on_search_changed (GtkSearchEntry *entry, gpointer user_data)
{
  gtk_list_box_invalidate_filter (GTK_LIST_BOX (conversations_list));
}

// Toggle sidebar
static void
on_sidebar_toggle (GtkButton *button, gpointer user_data)
{
  gboolean revealed = gtk_revealer_get_reveal_child (GTK_REVEALER (sidebar_revealer));
  gtk_revealer_set_reveal_child (GTK_REVEALER (sidebar_revealer), !revealed);
}

// Fonction Quitter
static void
on_quit_activate (GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
  GtkApplication *app = GTK_APPLICATION (user_data);
  g_application_quit (G_APPLICATION (app));
}

// Fonction À propos
static void
on_about_activate (GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
  const char *authors[] = { 
    "jikuji",
    "FortyTwo_dev",
    "Jensen",
    NULL };
  
  gtk_show_about_dialog (
    main_window ? GTK_WINDOW (main_window) : NULL,
    "program-name", "C-Talk",
    "version", "1.0.0",
    "comments", "Application de discussion",
    "authors", authors,
    "logo-icon-name", "system-users",
    NULL
  );
}

// Fonction Effacer la conversation
static void
on_clear_activate (GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
  gtk_text_buffer_set_text (text_buffer, "", 0);
  add_message ("Système", "Conversation effacée", FALSE);
}

// Actions de l'application
static GActionEntry app_actions[] = {
  { "about", on_about_activate },
  { "clear", on_clear_activate },
  { "quit", on_quit_activate }
};

// Fonction de démarrage
static void
on_startup (GtkApplication *app, gpointer user_data)
{
  const char *quit_accels[2] = { "<Ctrl>Q", NULL };
  gtk_application_set_accels_for_action (app, "app.quit", quit_accels);
}

// Créer les tags pour le formatage du texte
static void
create_text_tags (void)
{
  gtk_text_buffer_create_tag (text_buffer, "own",
                              "foreground", "#2196F3",
                              "weight", PANGO_WEIGHT_BOLD,
                              NULL);
  
  gtk_text_buffer_create_tag (text_buffer, "other",
                              "foreground", "#4CAF50",
                              NULL);
}

// Fonction d'activation (création de l'interface)
static void
on_activate (GtkApplication *app, gpointer user_data)
{
  g_action_map_add_action_entries (G_ACTION_MAP (app), app_actions,
                                   G_N_ELEMENTS (app_actions), app);
  
  main_window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (main_window), "Chat Application");
  gtk_window_set_default_size (GTK_WINDOW (main_window), 900, 600);
  
  // Créer la HeaderBar
  GtkWidget *header = gtk_header_bar_new ();
  gtk_header_bar_set_show_title_buttons (GTK_HEADER_BAR (header), TRUE);
  
  // Bouton pour afficher/masquer la sidebar (3 barres)
  GtkWidget *sidebar_button = gtk_button_new_from_icon_name ("open-menu-symbolic");
  g_signal_connect (sidebar_button, "clicked", G_CALLBACK (on_sidebar_toggle), NULL);
  gtk_header_bar_pack_start (GTK_HEADER_BAR (header), sidebar_button);
  
  // Titre de la HeaderBar
  GtkWidget *title_label = gtk_label_new ("Chat");
  gtk_widget_add_css_class (title_label, "title");
  gtk_header_bar_set_title_widget (GTK_HEADER_BAR (header), title_label);
  
  // Bouton menu
  GtkWidget *menu_button = gtk_menu_button_new ();
  gtk_menu_button_set_icon_name (GTK_MENU_BUTTON (menu_button), "view-more-symbolic");
  
  // Créer le menu
  GMenu *menu = g_menu_new ();
  GMenu *section1 = g_menu_new ();
  g_menu_append (section1, "Effacer la conversation", "app.clear");
  g_menu_append_section (menu, NULL, G_MENU_MODEL (section1));
  
  GMenu *section2 = g_menu_new ();
  g_menu_append (section2, "À propos", "app.about");
  g_menu_append (section2, "Quitter", "app.quit");
  g_menu_append_section (menu, NULL, G_MENU_MODEL (section2));
  
  gtk_menu_button_set_menu_model (GTK_MENU_BUTTON (menu_button), G_MENU_MODEL (menu));
  gtk_header_bar_pack_end (GTK_HEADER_BAR (header), menu_button);
  
  gtk_window_set_titlebar (GTK_WINDOW (main_window), header);
  
  // Conteneur principal horizontal (sidebar + chat)
  GtkWidget *paned = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
  
  // === SIDEBAR ===
  sidebar_revealer = gtk_revealer_new ();
  gtk_revealer_set_transition_type (GTK_REVEALER (sidebar_revealer), 
                                    GTK_REVEALER_TRANSITION_TYPE_SLIDE_RIGHT);
  gtk_revealer_set_reveal_child (GTK_REVEALER (sidebar_revealer), TRUE);
  
  sidebar = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request (sidebar, 10, 0);
  
  // Barre de recherche
  GtkWidget *search_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_margin_top (search_box, 12);
  gtk_widget_set_margin_bottom (search_box, 6);
  gtk_widget_set_margin_start (search_box, 12);
  gtk_widget_set_margin_end (search_box, 12);
  
  search_entry = gtk_search_entry_new ();
  gtk_search_entry_set_placeholder_text (GTK_SEARCH_ENTRY (search_entry), "Rechercher une personne...");
  g_signal_connect (search_entry, "search-changed", G_CALLBACK (on_search_changed), NULL);
  gtk_box_append (GTK_BOX (search_box), search_entry);
  gtk_box_append (GTK_BOX (sidebar), search_box);
  
  // Séparateur
  GtkWidget *sep1 = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_box_append (GTK_BOX (sidebar), sep1);
  
  // Liste des conversations
  GtkWidget *scrolled_sidebar = gtk_scrolled_window_new ();
  gtk_widget_set_vexpand (scrolled_sidebar, TRUE);
  
  conversations_list = gtk_list_box_new ();
  gtk_list_box_set_selection_mode (GTK_LIST_BOX (conversations_list), GTK_SELECTION_SINGLE);
  gtk_list_box_set_filter_func (GTK_LIST_BOX (conversations_list), 
                                filter_contacts, NULL, NULL);
  g_signal_connect (conversations_list, "row-activated", 
                   G_CALLBACK (on_contact_selected), NULL);
  
  // Ajouter les contacts
  for (int i = 0; i < G_N_ELEMENTS(contacts); i++) {
    GtkWidget *row_content = create_contact_row (&contacts[i]);
    gtk_list_box_append (GTK_LIST_BOX (conversations_list), row_content);
  }
  
  gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scrolled_sidebar), conversations_list);
  gtk_box_append (GTK_BOX (sidebar), scrolled_sidebar);
  
  gtk_revealer_set_child (GTK_REVEALER (sidebar_revealer), sidebar);
  gtk_paned_set_start_child (GTK_PANED (paned), sidebar_revealer);
  gtk_paned_set_shrink_start_child (GTK_PANED (paned), TRUE);
  gtk_paned_set_shrink_end_child (GTK_PANED (paned), FALSE);
  
  // === ZONE DE CHAT ===
  GtkWidget *chat_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  
  // Zone de texte avec scroll
  GtkWidget *scrolled = gtk_scrolled_window_new ();
  gtk_widget_set_vexpand (scrolled, TRUE);
  gtk_widget_set_hexpand (scrolled, TRUE);
  
  text_view = gtk_text_view_new ();
  gtk_text_view_set_editable (GTK_TEXT_VIEW (text_view), FALSE);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view), GTK_WRAP_WORD_CHAR);
  gtk_text_view_set_left_margin (GTK_TEXT_VIEW (text_view), 10);
  gtk_text_view_set_right_margin (GTK_TEXT_VIEW (text_view), 10);
  gtk_text_view_set_top_margin (GTK_TEXT_VIEW (text_view), 10);
  gtk_text_view_set_bottom_margin (GTK_TEXT_VIEW (text_view), 10);
  
  text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
  create_text_tags ();
  
  gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scrolled), text_view);
  gtk_box_append (GTK_BOX (chat_box), scrolled);
  
  // Séparateur
  GtkWidget *separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_box_append (GTK_BOX (chat_box), separator);
  
  // Zone de saisie en bas
  GtkWidget *input_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_widget_set_margin_top (input_box, 12);
  gtk_widget_set_margin_bottom (input_box, 12);
  gtk_widget_set_margin_start (input_box, 12);
  gtk_widget_set_margin_end (input_box, 12);
  
  entry = gtk_entry_new ();
  gtk_entry_set_placeholder_text (GTK_ENTRY (entry), "Tapez votre message...");
  gtk_widget_set_hexpand (entry, TRUE);
  g_signal_connect (entry, "activate", G_CALLBACK (on_entry_activate), NULL);
  gtk_box_append (GTK_BOX (input_box), entry);
  
  GtkWidget *send_button = gtk_button_new_with_label ("Envoyer");
  gtk_widget_add_css_class (send_button, "suggested-action");
  g_signal_connect (send_button, "clicked", G_CALLBACK (on_send_clicked), NULL);
  gtk_box_append (GTK_BOX (input_box), send_button);
  
  gtk_box_append (GTK_BOX (chat_box), input_box);
  
  gtk_paned_set_end_child (GTK_PANED (paned), chat_box);
  gtk_paned_set_resize_end_child (GTK_PANED (paned), TRUE);
  
  gtk_window_set_child (GTK_WINDOW (main_window), paned);
  
  // Message de bienvenue
  add_message ("Système", "Bienvenue dans le chat ! Sélectionnez un contact ou recherchez une personne.", FALSE);
  
  gtk_window_present (GTK_WINDOW (main_window));
}

int main (int argc, char *argv[])
{
  GtkApplication *app;
  int status;
  
  app = gtk_application_new ("org.example.chat", G_APPLICATION_DEFAULT_FLAGS);
  
  g_signal_connect (app, "startup", G_CALLBACK (on_startup), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
  
  status = g_application_run (G_APPLICATION (app), argc, argv);
  
  g_object_unref (app);
  
  return status;
}