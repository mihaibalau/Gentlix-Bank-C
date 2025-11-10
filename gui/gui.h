#ifndef GENTLIX_BANK_GUI_H
#define GENTLIX_BANK_GUI_H

#include <gtk/gtk.h>

void activate_main_menu(GtkApplication *app, gpointer data);
void close_window(GtkWidget *widget, gpointer data);
void show_window(GtkWidget *widget, gpointer data);
void show_error(gchar *message);
void handleErrorCode(int errorCode);
void add_to_balance(GtkWidget *widget, gpointer data);
void withdraw_from_balance(GtkWidget *widget, gpointer data);
void make_a_payment(GtkWidget *widget, gpointer data);
void make_a_transaction(GtkWidget *widget, gpointer data);
void logout_from_an_account();
void delete_an_account();
void edit_an_account(GtkWidget *widget, gpointer data);
void login_to_an_account(GtkWidget *widget, gpointer data);
void create_an_account(GtkWidget *widget, gpointer data);
void show_new_transaction_interface(GtkWidget *widget, gpointer data);
void show_all_transactions_interface(GtkWidget *widget, gpointer data);
void show_edit_account_interface(GtkWidget *widget, gpointer data);
void show_account_interface();
void show_login_interface(GtkWidget *widget, gpointer data);
void show_register_interface(GtkWidget *widget, gpointer data);
void show_import_interface(GtkWidget *widget, gpointer data);
void show_export_interface(GtkWidget *widget, gpointer data);

#endif //GENTLIX_BANK_GUI_H
