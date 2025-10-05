#include <gtk/gtk.h>

// Structure pour stocker le compteur et le label
typedef struct {
    GtkWidget *label;
    int count;
} AppWidget;

// Fonction appelée quand le bouton est cliqué
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    AppWidget *app = (AppWidget *)data;
    app->count++;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Compteur : %d", app->count);
    gtk_label_set_text(GTK_LABEL(app->label), buffer);
}

// Fonction d'activation de l'application
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box;
    AppWidget *app_widget = g_malloc(sizeof(AppWidget));
    app_widget->count = 0;

    // Création de la fenêtre
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Compteur GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    // Création d'une boîte verticale pour organiser les widgets
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Création du label pour afficher le compteur
    app_widget->label = gtk_label_new("Compteur : 0");
    gtk_box_pack_start(GTK_BOX(box), app_widget->label, TRUE, TRUE, 0);

    // Création du bouton
    button = gtk_button_new_with_label("Cliquez-moi !");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), app_widget);
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    // Affichage de tous les widgets
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Création de l'application
    app = gtk_application_new("org.example.compteur", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
