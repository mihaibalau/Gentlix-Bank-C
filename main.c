#include <gtk/gtk.h>
#include "services/services.h"
#include "gui/gui.h"
#include "repository/repository.h"

// Create the GTK Application, activate the main window for it and return the run status.
int main(int argc, char *argv[]) {

    GtkApplication * mainApplication;
    int applicationStatus;

    RepositoryFormat* database = createRepository();

    mainApplication = gtk_application_new("com.bank.GentlixBank", G_APPLICATION_FLAGS_NONE);
    g_object_set_data(G_OBJECT(mainApplication), "database", database);
    g_signal_connect(mainApplication, "activate", G_CALLBACK(activate_main_menu), NULL);
    applicationStatus = g_application_run(G_APPLICATION(mainApplication), argc, argv);
    g_object_unref(mainApplication);

    return applicationStatus;
}
