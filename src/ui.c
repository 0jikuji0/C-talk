// ui.c - Fichier principal avec main et gestion de l'interface
#include <gtk/gtk.h>

// Variable globale pour stocker la fenêtre
static GtkWidget *main_window = NULL;

// Fonction appelée quand on clique sur Preferences
static void
on_preferences_activate (GSimpleAction *action,
                         GVariant      *parameter,
                         gpointer       user_data)
{
  g_print ("Preferences activé\n");
  
  // Créer une fenêtre pour les préférences (GTK4 moderne)
  GtkWidget *dialog = gtk_window_new();
  gtk_window_set_title (GTK_WINDOW (dialog), "Préférences");
  gtk_window_set_default_size (GTK_WINDOW (dialog), 400, 300);
  gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
  
  if (main_window != NULL) {
    gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (main_window));
  }
  
  // Créer le contenu
  GtkWidget *box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_widget_set_margin_top (box, 24);
  gtk_widget_set_margin_bottom (box, 24);
  gtk_widget_set_margin_start (box, 24);
  gtk_widget_set_margin_end (box, 24);
  
  GtkWidget *label = gtk_label_new ("Trancho > Sananes");
  gtk_box_append (GTK_BOX (box), label);
  
  GtkWidget *button = gtk_button_new_with_label ("Fermer");
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_destroy), dialog);
  gtk_box_append (GTK_BOX (box), button);
  
  gtk_window_set_child (GTK_WINDOW (dialog), box);
  gtk_window_present (GTK_WINDOW (dialog));
}

// Fonction appelée quand on clique sur Quit
static void
on_quit_activate (GSimpleAction *action,
                  GVariant      *parameter,
                  gpointer       user_data)
{
  GtkApplication *app = GTK_APPLICATION (user_data);
  g_application_quit (G_APPLICATION (app));
}

// Fonction appelée quand on clique sur About
static void
on_about_activate (GSimpleAction *action,
                   GVariant      *parameter,
                   gpointer       user_data)
{
  const char *authors[] = {
    "Votre Nom",
    NULL
  };
  
  GtkWindow *parent = main_window ? GTK_WINDOW (main_window) : NULL;
  
  gtk_show_about_dialog (
    parent,
    "program-name", "Example Application",
    "version", "1.0.0",
    "comments", "Une application exemple avec menu GTK4",
    "authors", authors,
    "website", "https://www.gtk.org",
    "website-label", "Site web GTK",
    "logo-icon-name", "application-x-executable",
    NULL
  );
}

// Tableau des actions de l'application
static GActionEntry app_actions[] = {
  { "preferences", on_preferences_activate },
  { "about", on_about_activate },
  { "quit", on_quit_activate }
};

// Fonction appelée au démarrage de l'application
static void
on_startup (GtkApplication *app,
            gpointer        user_data)
{
  GtkBuilder *builder;
  GMenuModel *menubar;
  const char *quit_accels[2] = { "<Ctrl>Q", NULL };
  const char *about_accels[2] = { "F1", NULL };
  
  g_print ("Application démarrée\n");
  
  // Charger le menu depuis le fichier XML
  builder = gtk_builder_new_from_file ("menu.xml");
  menubar = G_MENU_MODEL (gtk_builder_get_object (builder, "menubar"));
  
  if (menubar == NULL) {
    g_warning ("Impossible de charger le menu depuis menu.xml");
  } else {
    gtk_application_set_menubar (app, menubar);
  }
  
  g_object_unref (builder);
  
  // Définir les raccourcis clavier
  gtk_application_set_accels_for_action (app, "app.quit", quit_accels);
  gtk_application_set_accels_for_action (app, "app.about", about_accels);
}

// Fonction appelée à l'activation de l'application
static void
on_activate (GtkApplication *app,
             gpointer        user_data)
{
  GtkBuilder *builder;
  GtkWidget *window;
  GtkWidget *menu_button;
  GMenuModel *menu_model;
  
  g_print ("Création de la fenêtre\n");
  
  // Ajouter les actions à l'application
  g_action_map_add_action_entries (G_ACTION_MAP (app),
                                   app_actions,
                                   G_N_ELEMENTS (app_actions),
                                   app);
  
  // Charger l'interface depuis le fichier XML
  builder = gtk_builder_new_from_file ("window.xml");
  
  if (builder == NULL) {
    g_warning ("Impossible de charger window.xml");
    return;
  }
  
  // Récupérer la fenêtre
  window = GTK_WIDGET (gtk_builder_get_object (builder, "main_window"));
  
  if (window == NULL) {
    g_warning ("Impossible de trouver 'main_window' dans window.xml");
    g_object_unref (builder);
    return;
  }
  
  // Stocker la fenêtre globalement
  main_window = window;
  
  // Récupérer le bouton menu
  menu_button = GTK_WIDGET (gtk_builder_get_object (builder, "menu_button"));
  
  if (menu_button != NULL) {
    // Charger le modèle de menu
    GtkBuilder *menu_builder = gtk_builder_new_from_file ("menu.xml");
    menu_model = G_MENU_MODEL (gtk_builder_get_object (menu_builder, "primary_menu"));
    
    if (menu_model != NULL) {
      gtk_menu_button_set_menu_model (GTK_MENU_BUTTON (menu_button), menu_model);
    }
    
    g_object_unref (menu_builder);
  }
  
  gtk_application_add_window (app, GTK_WINDOW (window));
  gtk_window_present (GTK_WINDOW (window));
  
  g_object_unref (builder);
}

// Fonction main
int
main (int argc, char *argv[])
{
  GtkApplication *app;
  int status;
  
  // Créer l'application
  app = gtk_application_new ("org.example.app", G_APPLICATION_DEFAULT_FLAGS);
  
  // Connecter les signaux
  g_signal_connect (app, "startup", G_CALLBACK (on_startup), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
  
  // Lancer l'application
  status = g_application_run (G_APPLICATION (app), argc, argv);
  
  // Nettoyer
  g_object_unref (app);
  
  return status;
}