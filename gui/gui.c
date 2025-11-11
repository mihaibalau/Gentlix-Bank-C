#include "gui.h"
#include <gtk/gtk.h>
#include <string.h>
#include "../domain/domain.h"
#include "../repository/repository.h"
#include "../services/services.h"

Account* currentAccount = NULL;
GtkApplication* app = NULL;
GtkWidget* main_menu = NULL;

// Close the window get through gpointer data parameter
void close_window(GtkWidget *widget, gpointer data) {
    if (data != NULL)
        gtk_widget_destroy(GTK_WIDGET(data));
}

// Show the window get through gpointer data parameter
void show_window(GtkWidget *widget, gpointer data){
    if (data != NULL)
        gtk_widget_show(GTK_WIDGET(data));
}

// Open an error window and print the string from gchar *message parameter
void show_error(gchar *message){
    GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(NULL),GTK_DIALOG_MODAL , GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s", message);
    gtk_dialog_run(GTK_DIALOG(error_dialog));
    gtk_widget_destroy(error_dialog);
}

// Helper function to style buttons with modern design (GTK 3.0 compatible)
void style_button(GtkWidget *button, const char *bg_color, const char *hover_color, const char *text_color) {
    gchar *css = g_strdup_printf(
        "button { "
        "background-color: %s; "
        "color: %s; "
        "border: none; "
        "border-radius: 8px; "
        "padding: 15px 40px; "
        "font-size: 18px; "
        "font-weight: 700; "
        "min-width: 400px; "
        "} "
        "button:hover { "
        "background-color: %s; "
        "} "
        "button:active { "
        "background-color: %s; "
        "}",
        bg_color, text_color, hover_color, hover_color
    );
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(button);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    g_free(css);
}

// Helper function to style primary buttons (blue)
void style_primary_button(GtkWidget *button) {
    style_button(button, "#4A90E2", "#357ABD", "#1A1A1A");
}

// Helper function to style success buttons (green) - for login forms
void style_success_button(GtkWidget *button) {
    // Use green background with black text for better readability
    style_button(button, "#2ECC71", "#27AE60", "#000000");  // Green with black text
}

// Helper function to style danger buttons (red) - for login forms
void style_danger_button(GtkWidget *button) {
    // Use red background with black text for better readability
    style_button(button, "#E74C3C", "#C0392B", "#000000");  // Red with black text
}

// Helper function to style warning buttons (orange)
void style_warning_button(GtkWidget *button) {
    style_button(button, "#F39C12", "#D68910", "#1A1A1A");
}

// Helper function to style secondary buttons (gray)
void style_secondary_button(GtkWidget *button) {
    style_button(button, "#95A5A6", "#7F8C8D", "#1A1A1A");
}

// Helper function to style success buttons for main menu (green background, dark text)
void style_success_button_main_menu(GtkWidget *button) {
    style_button(button, "#2ECC71", "#27AE60", "#1A1A1A");  // Green with dark text for main menu
}

// Helper function to style danger buttons for main menu (red background, dark text)
void style_danger_button_main_menu(GtkWidget *button) {
    style_button(button, "#E74C3C", "#C0392B", "#1A1A1A");  // Red with dark text for main menu
}

// Helper function to style title labels
void style_title_label(GtkWidget *label, const char *color) {
    gchar *css = g_strdup_printf(
        "label { "
        "font-size: 56px; "
        "color: %s; "
        "font-weight: bold; "
        "}",
        color
    );
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(label);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    g_free(css);
}

// Helper function to style subtitle labels
void style_subtitle_label(GtkWidget *label, const char *color) {
    gchar *css = g_strdup_printf(
        "label { "
        "font-size: 22px; "
        "color: %s; "
        "font-weight: 500; "
        "}",
        color
    );
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(label);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    g_free(css);
}

// Helper function to style entry fields
void style_entry(GtkWidget *entry) {
    const char *css = 
        "entry { "
        "background-color: #FFFFFF; "
        "border: 2px solid #BDC3C7; "
        "border-radius: 6px; "
        "padding: 10px 14px; "
        "font-size: 16px; "
        "color: #2C3E50; "
        "} "
        "entry:focus { "
        "border-color: #4A90E2; "
        "}";
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(entry);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

void handleErrorCode(int errorCode) {

    switch (errorCode) {
        case -101:
            show_error("The day needs to be a number!");
            break;
        case -102:
            show_error("The month needs to be a number!");
            break;
        case -103:
            show_error("The year needs to be a number!");
            break;
        case -104:
            show_error("The entered year is far too far away!");
            break;
        case -105:
            show_error("You do not have a minimum age to open an account!");
            break;
        case -106:
            show_error("The month does not exist!");
            break;
        case -107:
            show_error("The day does not exist!");
            break;
        case -108:
            show_error("This month has only 30 days!");
            break;
        case -109:
            show_error("The month can have a maximum of 29 days!");
            break;
        case -110:
            show_error("The year February has a maximum of 28 days!");
            break;

        case -121:
            show_error("The day needs to be a number!");
            break;
        case -122:
            show_error("The month needs to be a number!");
            break;
        case -123:
            show_error("The year needs to be a number!");
            break;
        case -124:
            show_error("The entered year is far too far away!");
            break;
        case -125:
            show_error("You do not have a minimum age to open an account!");
            break;
        case -126:
            show_error("The month does not exist!");
            break;
        case -127:
            show_error("The day does not exist!");
            break;
        case -128:
            show_error("The month has only 30 days!");
            break;
        case -129:
            show_error("The month can have a maximum of 29 days!");
            break;
        case -130:
            show_error("The year February has a maximum of 28 days!");
            break;

        case -141:
            show_error("The day needs to be a number!");
            break;
        case -142:
            show_error("The month needs to be a number!");
            break;
        case -143:
            show_error("The year needs to be a number!");
            break;
        case -144:
            show_error("There were no banks this year. People kept currency hidden in their homes!");
            break;
        case -145:
            show_error("The year format is invalid. It must have a maximum of 4 digits!");
            break;
        case -146:
            show_error("This month does not exist!");
            break;
        case -147:
            show_error("This day does not exist!");
            break;
        case -148:
            show_error("This month has only 30 days!");
            break;
        case -149:
            show_error("This month can have a maximum of 29 days!");
            break;
        case -150:
            show_error("This year February has a maximum of 28 days!");
            break;
        case -151:
            show_error("The last transaction was recorded in the future. You can't introduce now something that has already happened.");
            break;
            
        case -201:
            show_error("Invalid account");
            break;
        case -202:
            show_error("Invalid transaction");
            break;
        case -203:
            show_error("Memory management error");
            break;
        case -211:
            show_error("Invalid account");
            break;
        case -212:
            show_error("Invalid transaction");
            break;
        case -213:
            show_error("Affiliate already exists");
            break;
        case -214:
            show_error("Memory reallocation failed");
            break;
        case -221:
            show_error("Invalid account");
            break;
        case -222:
            show_error("Invalid transaction");
            break;
        case -223:
            show_error("Affiliate not found");
            break;
        case -231:
            show_error("Invalid account");
            break;
        case -232:
            show_error("Invalid user account");
            break;
        case -233:
            show_error("User account already exists");
            break;
        case -234:
            show_error("Memory reallocation failed");
            break;
        case -241:
            show_error("Invalid account");
            break;
        case -242:
            show_error("Invalid user account type");
            break;
        case -243:
            show_error("User account not found");
            break;
            
        case -301:
            show_error("Internal problem with program database.");
            break;
        case -302:
            show_error("Missing username input.");
            break;
        case -303:
            show_error("Missing the password for your account.");
            break;
        case -304:
            show_error("Imputed account tag is too long! Maximum 20 characters.");
            break;
        case -305:
            show_error("Imputed password is too long! Maximum 32 characters.");
            break;
        case -306:
            show_error("Account tag can have only letters!");
            break;
        case -311:
            show_error("Repository is NULL.");
            break;
        case -312:
            show_error("Missing account tag input.");
            break;
        case -313:
            show_error("Missing password input.");
            break;
        case -314:
            show_error("Missing password confirmation input.");
            break;
        case -315:
            show_error("Missing account type input.");
            break;
        case -316:
            show_error("Missing phone number input.");
            break;
        case -317:
            show_error("Missing first name input.");
            break;
        case -318:
            show_error("Missing second name input.");
            break;
        case -319:
            show_error("Missing day input.");
            break;
        case -320:
            show_error("Missing month input.");
            break;
        case -321:
            show_error("Missing year input.");
            break;
        case -322:
            show_error("Account tag already used.");
            break;
        case -323:
            show_error("Invalid birthday.");
            break;
        case -324:
            show_error("Account tag can have only letters!");
            break;
        case -325:
            show_error("The passwords do not match!");
            break;
        case -326:
            show_error("Invalid account type! Available types: savings, checking, credit.");
            break;
        case -327:
            show_error("First name can have only letters!");
            break;
        case -328:
            show_error("Second name can have only letters!");
            break;
        case -329:
            show_error("Phone number can have only digits!");
            break;
        case -330:
            show_error("Failed to create an account.");
            break;
        case -331:
            show_error("Failed to add account to repository.");
            break;
        case -332:
            show_error("Failed to create user account.");
            break;
        case -341:
            show_error("The imputed password is wrong!");
            break;
        case -342:
            show_error("The passwords do not match!");
            break;
        case -343:
            show_error("Invalid account type! Available types: savings, checking, credit.");
            break;
        case -344:
            show_error("First name can have only letters!");
            break;
        case -345:
            show_error("Second name can have only letters!");
            break;
        case -346:
            show_error("Phone number can have only digits!");
            break;
        case -401:
        case -411:
        case -421:
        case -431:
            show_error("Invalid account");
            break;
        case -402:
        case -412:
        case -422:
        case -432:
            show_error("You need to add an amount!");
            break;
        case -403:
        case -413:
        case -423:
        case -433:
            show_error("Missing description");
            break;
        case -404:
        case -414:
        case -434:
            show_error("The description is too long! You can use maximum 100 characters.");
            break;
        case -405:
        case -415:
        case -425:
        case -435:
            show_error("The amount isn't a number!");
            break;
        case -406:
        case -416:
        case -426:
        case -436:
            show_error("Invalid amount");
            break;
        case -407:
        case -418:
        case -429:
        case -438:
            show_error("Failed to create transaction");
            break;
        case -417:
        case -428:
        case -437:
            show_error("You have a lower balance than inputed number!");
            break;
        case -424:
            show_error("Missing receiver IBAN");
            break;
        case -307:
            show_error("Account tag can't be found or wrong password!");
            break;
        case -351:
            show_error("Invalid parameters for account deletion.");
            break;
        case -352:
            show_error("Invalid account tag.");
            break;
        case -340:
            show_error("Invalid account.");
            break;
        default:
            show_error("An unexpected error occurred.");
            break;
    }
}

// Manage the inputs from new transaction interface and work with them inside the memory to add money in user account
// GtkApplication *app - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the location inside the memory for inputs
//void add_to_balance(GtkWidget *widget, gpointer data){
//
//    GtkWidget **entries = (GtkWidget **)data;
//    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries[0]));
//    const gchar *description = gtk_entry_get_text(GTK_ENTRY(entries[1]));
//    const gchar *day = gtk_entry_get_text(GTK_ENTRY(entries[2]));
//    const gchar *month = gtk_entry_get_text(GTK_ENTRY(entries[3]));
//    const gchar *year = gtk_entry_get_text(GTK_ENTRY(entries[4]));
//
//    short error_encountered = 0;
//    gdouble money_amount;
//
//    if(strlen(amount) == 0){
//        error_encountered = 1;
//        show_error("You need to add an amount!");}
//
//    if(string_have_only_digits_extended(amount))
//        money_amount = g_ascii_strtod(amount, NULL); // this function transform a string of digits into a double number
//    else{
//        error_encountered = 1;
//        show_error("The amount isn't a number!");}
//
//    if(strlen(description) > 99){
//        error_encountered = 1;
//        show_error("The description is too long! You can use maximum 100 characters.");}
//
//    if(!check_date_for_transaction(day, month, year) && error_encountered == 0){
//
//        guint64 int_day = g_ascii_strtoull(day, NULL, 10);
//        guint64 int_month = g_ascii_strtoull(month, NULL, 10);
//        guint64 int_year = g_ascii_strtoull(year, NULL, 10);
//
//        account_database[current_account].transaction_index ++;
//        int last_transaction_index = account_database[current_account].transaction_index;
//
//        account_database[current_account].account_balace += money_amount;
//        account_database[current_account].transaction[last_transaction_index].date.day = int_day;
//        account_database[current_account].transaction[last_transaction_index].date.month = int_month;
//        account_database[current_account].transaction[last_transaction_index].date.year = int_year;
//        account_database[current_account].transaction[last_transaction_index].amount = money_amount;
//        strcpy(account_database[current_account].transaction[last_transaction_index].description, description);
//        strcpy(account_database[current_account].transaction[last_transaction_index].category, "deposit");
//
//        gtk_widget_destroy(last_window);
//        show_account_interface();
//    }
//}

void logout_from_an_account(){
    currentAccount = NULL;
}

void delete_an_account(){
    if (currentAccount == NULL || app == NULL) return;
    
    RepositoryFormat* database = g_object_get_data(G_OBJECT(app), "database");
    if (database != NULL) {
        deleteAccountService(database, &currentAccount);
    }
    currentAccount = NULL;
}

// Manage the inputs from edit interface
// GtkWidget *widget - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the location inside the memory for inputs
void edit_an_account(GtkWidget *widget, gpointer data){

    if (currentAccount == NULL || app == NULL) {
        show_error("No account logged in!");
        return;
    }

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *current_password = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *account_password = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *account_password2 = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    
    // Get account type from combobox
    gchar *account_type = NULL;
    if (GTK_IS_COMBO_BOX(entries[3])) {
        account_type = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[3]));
        if (account_type == NULL) account_type = g_strdup("");
    } else {
        account_type = g_strdup("");
    }
    
    const gchar *user_phone_number = gtk_entry_get_text(GTK_ENTRY(entries[4]));
    
    // Parse full name into first and second name
    const gchar *full_name = gtk_entry_get_text(GTK_ENTRY(entries[5]));
    gchar **name_parts = g_strsplit(full_name, " ", 2);
    const gchar *user_first_name = (name_parts != NULL && name_parts[0] != NULL) ? name_parts[0] : "";
    const gchar *user_second_name = (name_parts != NULL && name_parts[1] != NULL) ? name_parts[1] : "";
    
    // Parse birthday from DD/MM/YYYY format
    const gchar *birthday_str = gtk_entry_get_text(GTK_ENTRY(entries[6]));
    gchar **date_parts = g_strsplit(birthday_str, "/", 3);
    const gchar *user_birthday_day = (date_parts != NULL && date_parts[0] != NULL) ? date_parts[0] : "";
    const gchar *user_birthday_month = (date_parts != NULL && date_parts[1] != NULL) ? date_parts[1] : "";
    const gchar *user_birthday_year = (date_parts != NULL && date_parts[2] != NULL) ? date_parts[2] : "";

    int resultCode = editAccountService(&currentAccount, current_password, account_password, account_password2, account_type, user_phone_number, user_first_name, user_second_name,
                                          user_birthday_day, user_birthday_month, user_birthday_year);

    // Free allocated strings
    g_free(account_type);
    if (name_parts != NULL) g_strfreev(name_parts);
    if (date_parts != NULL) g_strfreev(date_parts);
    g_free(entries);

    if(resultCode == 1) {

        GtkWidget *last_window = g_object_get_data(G_OBJECT(app), "last_window");
        if (last_window != NULL)
            gtk_widget_destroy(last_window);

        show_account_interface();

    } else {
        handleErrorCode(resultCode);
    }
}


// Manage the inputs from login interface
// GtkWidget *widget - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the location inside the memory for inputs
void login_to_an_account(GtkWidget *widget, gpointer data){

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *account_tag = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *account_password = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    g_free(entries);

    if (app == NULL) {
        show_error("Application not initialized!");
        return;
    }

    RepositoryFormat* database = g_object_get_data(G_OBJECT(app), "database");
    Account* loggedAccount = NULL;

    int resultCode = loginService(database, account_tag, account_password, &loggedAccount);

    if(resultCode == 1 && loggedAccount != NULL) {

        GtkWidget *last_window = g_object_get_data(G_OBJECT(app), "last_window");
        if (last_window != NULL)
            gtk_widget_destroy(last_window);

        currentAccount = loggedAccount;
        show_account_interface();

    } else {
        handleErrorCode(resultCode);
    }
}

// Manage the inputs from register interface
// GtkWidget *widget - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the location inside the memory for inputs
void create_an_account(GtkWidget *widget, gpointer data){

    if (data == NULL) {
        show_error("Invalid form data!");
        return;
    }

    GtkWidget **entries = (GtkWidget **)data;
    
    // Validate entries array and all entries exist
    if (entries == NULL) {
        show_error("Form entries not initialized!");
        return;
    }
    
    // Check all entries exist and are valid widgets
    for (int i = 0; i < 6; i++) {
        if (entries[i] == NULL) {
            show_error("One or more form fields are missing!");
            return;
        }
    }
    
    // Validate app exists
    if (app == NULL) {
        show_error("Application not initialized!");
        return;
    }

    // Get all field values with NULL checks
    const gchar *account_tag = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    if (account_tag == NULL) account_tag = "";
    
    const gchar *full_name = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    if (full_name == NULL) full_name = "";
    
    const gchar *account_password = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    if (account_password == NULL) account_password = "";
    
    // Use password for password2 (confirm password) since we removed the confirm field
    const gchar *account_password2 = account_password;
    
    // Get account type from combobox (must be freed)
    gchar *account_type = NULL;
    if (GTK_IS_COMBO_BOX(entries[3])) {
        account_type = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[3]));
        if (account_type == NULL) account_type = g_strdup("");
    } else {
        show_error("Account type selector is invalid!");
        return;
    }
    
    const gchar *user_phone_number = gtk_entry_get_text(GTK_ENTRY(entries[4]));
    if (user_phone_number == NULL) user_phone_number = "";
    
    const gchar *birthday_str = gtk_entry_get_text(GTK_ENTRY(entries[5]));
    if (birthday_str == NULL) birthday_str = "";
    
    // Validate account tag is not empty
    if (strlen(account_tag) == 0) {
        show_error("Account tag is required!");
        g_free(account_type);
        return;
    }
    
    // Validate password is not empty
    if (strlen(account_password) == 0) {
        show_error("Password is required!");
        g_free(account_type);
        return;
    }
    
    // Validate and parse full name - must have at least 2 words, each with minimum 3 characters
    if (strlen(full_name) == 0) {
        show_error("Name is required!");
        g_free(account_type);
        return;
    }
    
    gchar **name_parts = g_strsplit(full_name, " ", 2);
    if (name_parts == NULL || name_parts[0] == NULL) {
        show_error("Invalid name format!");
        g_free(account_type);
        if (name_parts != NULL) g_strfreev(name_parts);
        return;
    }
    
    // Check first name has at least 3 characters
    if (strlen(name_parts[0]) < 3) {
        show_error("First name must be at least 3 characters long!");
        g_free(account_type);
        g_strfreev(name_parts);
        return;
    }
    
    // Check second name exists and has at least 3 characters
    if (name_parts[1] == NULL || strlen(name_parts[1]) < 3) {
        show_error("Name must contain at least 2 words separated by space, each with minimum 3 characters!");
        g_free(account_type);
        g_strfreev(name_parts);
        return;
    }
    
    const gchar *user_first_name = name_parts[0];
    const gchar *user_second_name = name_parts[1];
    
    // Validate phone number is not empty
    if (strlen(user_phone_number) == 0) {
        show_error("Phone number is required!");
        g_free(account_type);
        g_strfreev(name_parts);
        return;
    }
    
    // Validate and parse date from DD/MM/YYYY format
    if (strlen(birthday_str) == 0) {
        show_error("Birthday is required! Please enter date in DD/MM/YYYY format (e.g., 10/02/2025)");
        g_free(account_type);
        g_strfreev(name_parts);
        return;
    }
    
    gchar **date_parts = g_strsplit(birthday_str, "/", 3);
    if (date_parts == NULL || date_parts[0] == NULL || date_parts[1] == NULL || date_parts[2] == NULL) {
        show_error("Invalid date format! Please use DD/MM/YYYY format (e.g., 10/02/2025)");
        g_free(account_type);
        g_strfreev(name_parts);
        if (date_parts != NULL) g_strfreev(date_parts);
        return;
    }
    
    // Check all date parts are not empty
    if (strlen(date_parts[0]) == 0 || strlen(date_parts[1]) == 0 || strlen(date_parts[2]) == 0) {
        show_error("Invalid date format! Please use DD/MM/YYYY format (e.g., 10/02/2025)");
        g_free(account_type);
        g_strfreev(name_parts);
        g_strfreev(date_parts);
        return;
    }
    
    const gchar *user_birthday_day = date_parts[0];
    const gchar *user_birthday_month = date_parts[1];
    const gchar *user_birthday_year = date_parts[2];
    
    // Validate account type is not empty
    if (strlen(account_type) == 0) {
        show_error("Account type is required!");
        g_free(account_type);
        g_strfreev(name_parts);
        g_strfreev(date_parts);
        return;
    }

    RepositoryFormat* database = g_object_get_data(G_OBJECT(app), "database");
    if (database == NULL) {
        show_error("Database not initialized!");
        g_free(account_type);
        g_strfreev(name_parts);
        g_strfreev(date_parts);
        return;
    }
    
    Account* loggedAccount = NULL;

    int resultCode = createAccountService(database, account_tag, account_password, account_password2, account_type, user_phone_number, user_first_name, user_second_name,
                                  user_birthday_day, user_birthday_month, user_birthday_year, &loggedAccount);

    // Free allocated strings
    g_free(account_type);
    g_strfreev(name_parts);
    g_strfreev(date_parts);

    if(resultCode == 1 && loggedAccount != NULL) {

        GtkWidget *last_window = g_object_get_data(G_OBJECT(app), "last_window");
        if (last_window != NULL)
            gtk_widget_destroy(last_window);

        currentAccount = loggedAccount;
        show_account_interface();

    } else {
        handleErrorCode(resultCode);
    }
}


// Transaction callback functions
void add_to_balance(GtkWidget *widget, gpointer data) {
    if (currentAccount == NULL || app == NULL) {
        show_error("No account logged in!");
        return;
    }

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *description = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *day = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *month = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    const gchar *year = gtk_entry_get_text(GTK_ENTRY(entries[4]));

    int resultCode = depositService(currentAccount, amount, description, day, month, year);
    
    if (resultCode == 1) {
        GtkWidget *last_window = g_object_get_data(G_OBJECT(app), "last_window");
        if (last_window != NULL)
            gtk_widget_destroy(last_window);
        show_account_interface();
    } else {
        handleErrorCode(resultCode);
    }
    
    g_free(entries);
}

void withdraw_from_balance(GtkWidget *widget, gpointer data) {
    if (currentAccount == NULL || app == NULL) {
        show_error("No account logged in!");
        return;
    }

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *description = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *day = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *month = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    const gchar *year = gtk_entry_get_text(GTK_ENTRY(entries[4]));

    int resultCode = withdrawService(currentAccount, amount, description, day, month, year);
    
    if (resultCode == 1) {
        GtkWidget *last_window = g_object_get_data(G_OBJECT(app), "last_window");
        if (last_window != NULL)
            gtk_widget_destroy(last_window);
        show_account_interface();
    } else {
        handleErrorCode(resultCode);
    }
    
    g_free(entries);
}

void make_a_payment(GtkWidget *widget, gpointer data) {
    if (currentAccount == NULL || app == NULL) {
        show_error("No account logged in!");
        return;
    }

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *description = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *day = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *month = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    const gchar *year = gtk_entry_get_text(GTK_ENTRY(entries[4]));

    int resultCode = paymentService(currentAccount, amount, description, day, month, year);
    
    if (resultCode == 1) {
        GtkWidget *last_window = g_object_get_data(G_OBJECT(app), "last_window");
        if (last_window != NULL)
            gtk_widget_destroy(last_window);
        show_account_interface();
    } else {
        handleErrorCode(resultCode);
    }
    
    g_free(entries);
}

void make_a_transaction(GtkWidget *widget, gpointer data) {
    if (currentAccount == NULL || app == NULL) {
        show_error("No account logged in!");
        return;
    }

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *description = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *receiverIBAN = gtk_entry_get_text(GTK_ENTRY(entries[5])); // Assuming entries[5] is for IBAN
    const gchar *day = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *month = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    const gchar *year = gtk_entry_get_text(GTK_ENTRY(entries[4]));

    int resultCode = transferService(currentAccount, amount, description, receiverIBAN, day, month, year);
    
    if (resultCode == 1) {
        GtkWidget *last_window = g_object_get_data(G_OBJECT(app), "last_window");
        if (last_window != NULL)
            gtk_widget_destroy(last_window);
        show_account_interface();
    } else {
        handleErrorCode(resultCode);
    }
    
    g_free(entries);
}

// Create the new transaction interface with fields for informations and a menu which transaction types.
// GtkWidget *widget - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the window from which this menu was opened to hide it
void show_new_transaction_interface(GtkWidget *widget, gpointer data) {

    if (currentAccount == NULL) {
        show_error("No account logged in!");
        return;
    }

    GtkWidget *new_transaction_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_object_set_data(G_OBJECT(app), "last_window", new_transaction_window);

    gtk_window_set_default_icon_from_file("images/bank_icon.png", NULL);
    gtk_window_set_title(GTK_WINDOW(new_transaction_window), "GentlixBank - Application");
    gtk_window_set_default_size(GTK_WINDOW(new_transaction_window), 800, 1000);
    gtk_window_maximize(GTK_WINDOW(new_transaction_window));

    GtkWidget *overlay_main_box = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(new_transaction_window), overlay_main_box);
    GtkWidget *image_app_background = gtk_image_new_from_file("images/app_background.png");
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), image_app_background);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_set_spacing(GTK_BOX(main_box), 50);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 50);

    GtkWidget *upper_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(GTK_WIDGET(upper_box), -1, 400);
    gtk_box_pack_start(GTK_BOX(main_box), upper_box, TRUE, TRUE, 0);

    GtkWidget *overlay_upper_box = gtk_overlay_new();
    gtk_box_pack_start(GTK_BOX(upper_box), overlay_upper_box, TRUE, TRUE, 0);
    GtkWidget *image_background = gtk_image_new_from_file("images/transactions_background.png");
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_upper_box), image_background);

    GtkWidget *content_box_for_upper_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *image_logo = gtk_image_new_from_file("images/bank_logo.png");
    gtk_box_pack_start(GTK_BOX(content_box_for_upper_box), image_logo, FALSE, FALSE, 65);

    GtkWidget *text_title = gtk_label_new("New Transaction");
    const gchar *css_title_format = "label { font-size: 72px; color: #000000; font-weight: bold; }";
    GtkCssProvider *title_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(title_provider, css_title_format, -1, NULL);
    GtkStyleContext *title_context = gtk_widget_get_style_context(text_title);
    gtk_style_context_add_provider(title_context, GTK_STYLE_PROVIDER(title_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(title_provider);

    gtk_box_pack_start(GTK_BOX(content_box_for_upper_box), text_title, FALSE, FALSE, 10);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_upper_box), content_box_for_upper_box);

    GtkWidget *lower_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_set_size_request(GTK_WIDGET(lower_box), -1, 500);
    gtk_box_pack_start(GTK_BOX(main_box), lower_box, TRUE, TRUE, 0);

    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_size_request(grid, 100, -1);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_box_pack_start(GTK_BOX(lower_box), grid, FALSE, FALSE, 30);

    GtkWidget **entries = (GtkWidget **)g_malloc(6 * sizeof(GtkWidget *));

    PangoAttrList *attr_list_text = pango_attr_list_new();
    PangoAttribute *attr_size_text = pango_attr_size_new(35 * PANGO_SCALE);
    pango_attr_list_insert(attr_list_text, attr_size_text);
    PangoAttribute *attr_bold_text = pango_attr_weight_new(PANGO_WEIGHT_SEMIBOLD);
    pango_attr_list_insert(attr_list_text, attr_bold_text);

    PangoAttrList *attr_list_entry = pango_attr_list_new();
    PangoAttribute *attr_size_entry = pango_attr_size_new(25 * PANGO_SCALE);
    pango_attr_list_insert(attr_list_entry, attr_size_entry);

    GtkWidget *username_label = gtk_label_new("                            Amount:");
    gtk_label_set_attributes(GTK_LABEL(username_label), attr_list_text);
    gtk_grid_attach(GTK_GRID(grid), username_label, 0, 0, 1, 1);

    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Enter money amount");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 50);
    gtk_grid_attach(GTK_GRID(grid), entries[0], 1, 0, 1, 1);

    GtkWidget *password_label = gtk_label_new("                          Description:");
    gtk_label_set_attributes(GTK_LABEL(password_label), attr_list_text);
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 1, 1, 1);

    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter transfer's description");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 50);
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);

    GtkWidget *day_birthday_label = gtk_label_new("                   Transaction Day:");
    gtk_label_set_attributes(GTK_LABEL(day_birthday_label), attr_list_text);
    gtk_grid_attach(GTK_GRID(grid), day_birthday_label, 2, 0, 1, 1);

    entries[2] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[2]), "Enter the day");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[2]), 50);
    gtk_grid_attach(GTK_GRID(grid), entries[2], 3, 0, 1, 1);

    GtkWidget *month_birthday_label = gtk_label_new("                  Transaction Month:");
    gtk_label_set_attributes(GTK_LABEL(month_birthday_label), attr_list_text);
    gtk_grid_attach(GTK_GRID(grid), month_birthday_label, 2, 1, 1, 1);

    entries[3] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[3]), "Enter the month");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[3]), 50);
    gtk_grid_attach(GTK_GRID(grid), entries[3], 3, 1, 1, 1);

    GtkWidget *year_birthday_label = gtk_label_new("                  Transaction Year:");
    gtk_label_set_attributes(GTK_LABEL(year_birthday_label), attr_list_text);
    gtk_grid_attach(GTK_GRID(grid), year_birthday_label, 2, 2, 1, 1);

    entries[4] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[4]), "Enter the year");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[4]), 50);
    gtk_grid_attach(GTK_GRID(grid), entries[4], 3, 2, 1, 1);

    GtkWidget *iban_label = gtk_label_new("                    Receiver IBAN (for transfer):");
    gtk_label_set_attributes(GTK_LABEL(iban_label), attr_list_text);
    gtk_grid_attach(GTK_GRID(grid), iban_label, 0, 2, 1, 1);

    entries[5] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[5]), "Enter receiver IBAN (only for transfer)");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[5]), 50);
    gtk_grid_attach(GTK_GRID(grid), entries[5], 1, 2, 1, 1);

    GtkWidget *deposit_button = gtk_button_new_with_label("Deposit");
    GtkWidget *withdraw_button = gtk_button_new_with_label("Withdraw");
    GtkWidget *pay_button = gtk_button_new_with_label("Pay");
    GtkWidget *transfer_button = gtk_button_new_with_label("Transfer");
    GtkWidget *cancel_button = gtk_button_new_with_label("Cancel");

    const char *css = "label { font-size: 45px; font-weight: 600; }";
    GtkCssProvider *buttons_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(buttons_provider, css, -1, NULL);
    GtkWidget *buttons[] = {deposit_button, withdraw_button, pay_button, transfer_button, cancel_button};
    for (int i = 0; i < G_N_ELEMENTS(buttons); i++) {
        GtkWidget *label = gtk_bin_get_child(GTK_BIN(buttons[i]));
        gtk_style_context_add_provider(gtk_widget_get_style_context(label), GTK_STYLE_PROVIDER(buttons_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        if (i == 0) {
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(add_to_balance), entries);
        } else if (i == 1) {
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(withdraw_from_balance), entries);
        } else if (i == 2) {
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(make_a_payment), entries);
        } else if (i == 3) {
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(make_a_transaction), entries);
        } else if (i == 4) {
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(show_account_interface), NULL);
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(close_window), new_transaction_window);
        }
        gtk_box_pack_start(GTK_BOX(lower_box), buttons[i], TRUE, TRUE, 0);
    }
    g_object_unref(buttons_provider);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), main_box);
    gtk_widget_show_all(GTK_WIDGET(new_transaction_window));
    if (data != NULL)
        gtk_widget_hide(GTK_WIDGET(data));
}

// Create the all transactions interface and a grid table with all transactions for logged user
// GtkWidget *widget - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the window from which this menu was opened to hide it
void show_all_transactions_interface(GtkWidget *widget, gpointer data) {

    if (currentAccount == NULL) {
        show_error("No account logged in!");
        return;
    }

    GtkWidget *all_transactions_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_object_set_data(G_OBJECT(app), "last_window", all_transactions_window);

    gtk_window_set_default_icon_from_file("images/bank_icon.png", NULL);
    gtk_window_set_title(GTK_WINDOW(all_transactions_window), "GentlixBank - Application");
    gtk_window_set_default_size(GTK_WINDOW(all_transactions_window), 800, 1000);
    gtk_window_maximize(GTK_WINDOW(all_transactions_window));

    GtkWidget *overlay_main_box = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(all_transactions_window), overlay_main_box);
    GtkWidget *image_app_background = gtk_image_new_from_file("images/app_background.png");
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), image_app_background);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_set_spacing(GTK_BOX(main_box), 50);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 50);

    GtkWidget *upper_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(GTK_WIDGET(upper_box), -1, 400);
    gtk_box_pack_start(GTK_BOX(main_box), upper_box, TRUE, TRUE, 0);

    GtkWidget *overlay_upper_box = gtk_overlay_new();
    gtk_box_pack_start(GTK_BOX(upper_box), overlay_upper_box, TRUE, TRUE, 0);
    GtkWidget *image_background = gtk_image_new_from_file("images/history_background.png");
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_upper_box), image_background);

    GtkWidget *content_box_for_upper_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *image_logo = gtk_image_new_from_file("images/bank_logo.png");
    gtk_box_pack_start(GTK_BOX(content_box_for_upper_box), image_logo, FALSE, FALSE, 55);

    GtkWidget *text_title = gtk_label_new("Your Transactions");
    const gchar *css_title_format = "label { font-size: 72px; color: #000000; font-weight: bold; }";
    GtkCssProvider *title_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(title_provider, css_title_format, -1, NULL);
    GtkStyleContext *title_context = gtk_widget_get_style_context(text_title);
    gtk_style_context_add_provider(title_context, GTK_STYLE_PROVIDER(title_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(title_provider);

    gtk_box_pack_start(GTK_BOX(content_box_for_upper_box), text_title, FALSE, FALSE, 10);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_upper_box), content_box_for_upper_box);

    GtkWidget *lower_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_set_size_request(GTK_WIDGET(lower_box), -1, 400);
    gtk_box_pack_start(GTK_BOX(main_box), lower_box, TRUE, TRUE, 0);

    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_size_request(grid, 100, -1);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 50);
    gtk_box_pack_start(GTK_BOX(lower_box), grid, FALSE, FALSE, 30);

    PangoAttrList *attr_list_title = pango_attr_list_new();
    PangoAttribute *attr_size_title = pango_attr_size_new(35 * PANGO_SCALE);
    pango_attr_list_insert(attr_list_title, attr_size_title);
    PangoAttribute *attr_bold_title = pango_attr_weight_new(PANGO_WEIGHT_SEMIBOLD);
    pango_attr_list_insert(attr_list_title, attr_bold_title);

    PangoAttrList *attr_list_content = pango_attr_list_new();
    PangoAttribute *attr_size_content = pango_attr_size_new(25 * PANGO_SCALE);
    pango_attr_list_insert(attr_list_content, attr_size_content);

    GtkWidget *number_label = gtk_label_new("                                             Number");
    gtk_label_set_attributes(GTK_LABEL(number_label), attr_list_title);
    gtk_grid_attach(GTK_GRID(grid), number_label, 0, 0, 1, 1);

    GtkWidget *type_label = gtk_label_new("Type");
    gtk_label_set_attributes(GTK_LABEL(type_label), attr_list_title);
    gtk_grid_attach(GTK_GRID(grid), type_label, 1, 0, 1, 1);

    GtkWidget *amount_label = gtk_label_new("Amount");
    gtk_label_set_attributes(GTK_LABEL(amount_label), attr_list_title);
    gtk_grid_attach(GTK_GRID(grid), amount_label, 2, 0, 1, 1);

    GtkWidget *day_label = gtk_label_new("Day");
    gtk_label_set_attributes(GTK_LABEL(day_label), attr_list_title);
    gtk_grid_attach(GTK_GRID(grid), day_label, 3, 0, 1, 1);

    GtkWidget *month_label = gtk_label_new("Month");
    gtk_label_set_attributes(GTK_LABEL(month_label), attr_list_title);
    gtk_grid_attach(GTK_GRID(grid), month_label, 4, 0, 1, 1);

    GtkWidget *year_label = gtk_label_new("Year");
    gtk_label_set_attributes(GTK_LABEL(year_label), attr_list_title);
    gtk_grid_attach(GTK_GRID(grid), year_label, 5, 0, 1, 1);

    GtkWidget *description_label = gtk_label_new("Description");
    gtk_label_set_attributes(GTK_LABEL(description_label), attr_list_title);
    gtk_grid_attach(GTK_GRID(grid), description_label, 6, 0, 1, 1);

    int transactionsNumber = getAccountTransactionsNumber(currentAccount);
    for(int index = 0; index < transactionsNumber; index++)
    {
        Transaction* transaction = currentAccount->transactions[index];
        if (transaction == NULL) continue;

        gchar *print_number_format = g_strdup_printf("                                                             %d.", index + 1);
        GtkWidget *number_text = gtk_label_new(NULL);
        gtk_label_set_text(GTK_LABEL(number_text), print_number_format);
        gtk_label_set_attributes(GTK_LABEL(number_text), attr_list_content);
        gtk_grid_attach(GTK_GRID(grid), number_text, 0, index + 1, 1, 1);
        g_free(print_number_format);

        const char* transactionType = getTransactionType(transaction);
        gchar *print_type_format = g_strdup_printf("%s", transactionType ? transactionType : "");
        GtkWidget *type_text = gtk_label_new(NULL);
        gtk_label_set_text(GTK_LABEL(type_text), print_type_format);
        gtk_label_set_attributes(GTK_LABEL(type_text), attr_list_content);
        gtk_grid_attach(GTK_GRID(grid), type_text, 1, index + 1, 1, 1);
        g_free(print_type_format);

        float transactionAmount = getTransactionAmount(transaction);
        gchar *print_amount_format = g_strdup_printf("%.2f$", transactionAmount);
        GtkWidget *amount_text = gtk_label_new(NULL);
        gtk_label_set_text(GTK_LABEL(amount_text), print_amount_format);
        gtk_label_set_attributes(GTK_LABEL(amount_text), attr_list_content);
        gtk_grid_attach(GTK_GRID(grid), amount_text, 2, index + 1, 1, 1);
        g_free(print_amount_format);

        Date transactionDate = getTransactionDate(transaction);
        gchar *print_day_format = g_strdup_printf("%d", transactionDate.day);
        GtkWidget *day_text = gtk_label_new(NULL);
        gtk_label_set_text(GTK_LABEL(day_text), print_day_format);
        gtk_label_set_attributes(GTK_LABEL(day_text), attr_list_content);
        gtk_grid_attach(GTK_GRID(grid), day_text, 3, index + 1, 1, 1);
        g_free(print_day_format);

        gchar *print_month_format = g_strdup_printf("%d", transactionDate.month);
        GtkWidget *month_text = gtk_label_new(NULL);
        gtk_label_set_text(GTK_LABEL(month_text), print_month_format);
        gtk_label_set_attributes(GTK_LABEL(month_text), attr_list_content);
        gtk_grid_attach(GTK_GRID(grid), month_text, 4, index + 1, 1, 1);
        g_free(print_month_format);

        gchar *print_year_format = g_strdup_printf("%d", transactionDate.year);
        GtkWidget *year_text = gtk_label_new(NULL);
        gtk_label_set_text(GTK_LABEL(year_text), print_year_format);
        gtk_label_set_attributes(GTK_LABEL(year_text), attr_list_content);
        gtk_grid_attach(GTK_GRID(grid), year_text, 5, index + 1, 1, 1);
        g_free(print_year_format);

        const char* transactionDescription = getTransactionDescription(transaction);
        gchar *print_description_format = g_strdup_printf("%s", transactionDescription ? transactionDescription : "");
        GtkWidget *description_text = gtk_label_new(NULL);
        gtk_label_set_text(GTK_LABEL(description_text), print_description_format);
        gtk_label_set_attributes(GTK_LABEL(description_text), attr_list_content);
        gtk_grid_attach(GTK_GRID(grid), description_text, 6, index + 1, 1, 1);
        g_free(print_description_format);
    }

    GtkWidget *close_button = gtk_button_new_with_label("Close");

    const char *css = "label { font-size: 45px; font-weight: 600; }";
    GtkCssProvider *buttons_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(buttons_provider, css, -1, NULL);
    GtkWidget *buttons[] = {close_button};
    for (int i = 0; i < G_N_ELEMENTS(buttons); i++) {
        GtkWidget *label = gtk_bin_get_child(GTK_BIN(buttons[i]));
        gtk_style_context_add_provider(gtk_widget_get_style_context(label), GTK_STYLE_PROVIDER(buttons_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        if (i == 0) {
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(show_account_interface), NULL);
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(close_window), all_transactions_window);
        }
        gtk_box_pack_start(GTK_BOX(lower_box), buttons[i], FALSE, TRUE, 0);
    }
    g_object_unref(buttons_provider);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), main_box);
    gtk_widget_show_all(GTK_WIDGET(all_transactions_window));
    if (data != NULL)
        gtk_widget_hide(GTK_WIDGET(data));
}

// Create the edit account interface with same fields as register one but blank ones are ignored
// GtkWidget *widget - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the window from which this menu was opened to hide it
void show_edit_account_interface(GtkWidget *widget, gpointer data){

    if (currentAccount == NULL || app == NULL) {
        show_error("No account logged in!");
        return;
    }

    GtkWidget *edit_account_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_object_set_data(G_OBJECT(app), "last_window", edit_account_window);

    gtk_window_set_default_icon_from_file("images/bank_icon.png", NULL);
    gtk_window_set_title(GTK_WINDOW(edit_account_window), "GentlixBank - Application");
    gtk_window_set_default_size(GTK_WINDOW(edit_account_window), 800, 1000);
    gtk_window_maximize(GTK_WINDOW(edit_account_window));

    GtkWidget *overlay_main_box = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(edit_account_window), overlay_main_box);
    GtkWidget *image_app_background = gtk_image_new_from_file("images/app_background.png");
    gtk_widget_set_sensitive(image_app_background, FALSE);  // Allow clicks to pass through
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), image_app_background);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_set_spacing(GTK_BOX(main_box), 0);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 50);

    // Spacer to push header to center
    GtkWidget *spacer_top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_sensitive(spacer_top, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), spacer_top, TRUE, TRUE, 0);
    
    // Header box with logo and title side by side (centered like login/register)
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_halign(header_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(header_box, 30);
    gtk_widget_set_margin_bottom(header_box, 20);
    
    // Logo on the left
    GtkWidget *image_logo = gtk_image_new_from_file("images/bank_logo.png");
    gtk_box_pack_start(GTK_BOX(header_box), image_logo, FALSE, FALSE, 0);

    // Title on the right (centered vertically with logo)
    GtkWidget *text_title = gtk_label_new("Edit account");
    style_title_label(text_title, "#FFDFAF");
    gtk_widget_set_valign(text_title, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(header_box), text_title, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(main_box), header_box, FALSE, FALSE, 0);
    
    // Small spacer between header and form card
    GtkWidget *card_spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(card_spacer, -1, 20);
    gtk_widget_set_sensitive(card_spacer, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), card_spacer, FALSE, FALSE, 0);

    // Form card container (same size as register)
    GtkWidget *form_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(form_card, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(form_card, 500, -1);
    gtk_container_set_border_width(GTK_CONTAINER(form_card), 30);
    
    // Style the form card with background and yellow border
    const gchar *card_css = 
        "box { "
        "background-color: #FFFFFF; "
        "border-radius: 12px; "
        "border: 3px solid #FFD700; "
        "padding: 30px; "
        "}";
    GtkCssProvider *card_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(card_provider, card_css, -1, NULL);
    GtkStyleContext *card_context = gtk_widget_get_style_context(form_card);
    gtk_style_context_add_provider(card_context, GTK_STYLE_PROVIDER(card_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(card_provider);
    
    gtk_box_pack_start(GTK_BOX(main_box), form_card, FALSE, FALSE, 0);
    
    // Spacer to push buttons to bottom
    GtkWidget *spacer_bottom = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_sensitive(spacer_bottom, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), spacer_bottom, TRUE, TRUE, 0);

    // Allocate entries array: 0=current_password, 1=new_password, 2=confirm_password, 3=account_type, 4=phone, 5=name, 6=birthday
    GtkWidget **entries = (GtkWidget **)g_malloc(7 * sizeof(GtkWidget *));
    
    // Label styling (same as register)
    const gchar *label_css = "label { font-size: 14px; color: #2C3E50; font-weight: 600; }";
    GtkCssProvider *label_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider, label_css, -1, NULL);
    
    // Get current account data for preload
    Date birthday = getAccountBirthday(currentAccount);
    gchar *birthday_str = g_strdup_printf("%02d/%02d/%04d", birthday.day, birthday.month, birthday.year);
    gchar *full_name = g_strdup_printf("%s %s", getAccountFirstName(currentAccount), getAccountSecondName(currentAccount));
    const char *phone = getAccountPhoneNumber(currentAccount);
    const char *account_type = NULL;
    if (getAccountUserAccountsNumber(currentAccount) > 0 && currentAccount->userAccounts[0] != NULL) {
        account_type = getUserAccountType(currentAccount->userAccounts[0]);
    }
    
    // 1. Current Password field
    GtkWidget *current_password_label = gtk_label_new("Current Password:");
    GtkStyleContext *label_context1 = gtk_widget_get_style_context(current_password_label);
    gtk_style_context_add_provider(label_context1, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_widget_set_halign(current_password_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(current_password_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), current_password_label, FALSE, FALSE, 0);
    
    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Enter current password");
    gtk_entry_set_visibility(GTK_ENTRY(entries[0]), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(entries[0]), '*');
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 30);
    style_entry(entries[0]);
    gtk_widget_set_margin_bottom(entries[0], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[0], FALSE, FALSE, 0);
    
    // 2. New Password field
    GtkWidget *new_password_label = gtk_label_new("New Password:");
    GtkCssProvider *label_provider2 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider2, label_css, -1, NULL);
    GtkStyleContext *label_context2 = gtk_widget_get_style_context(new_password_label);
    gtk_style_context_add_provider(label_context2, GTK_STYLE_PROVIDER(label_provider2), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider2);
    gtk_widget_set_halign(new_password_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(new_password_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), new_password_label, FALSE, FALSE, 0);
    
    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter new password (optional)");
    gtk_entry_set_visibility(GTK_ENTRY(entries[1]), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(entries[1]), '*');
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 30);
    style_entry(entries[1]);
    gtk_widget_set_margin_bottom(entries[1], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[1], FALSE, FALSE, 0);
    
    // 3. Confirm Password field
    GtkWidget *confirm_password_label = gtk_label_new("Confirm Password:");
    GtkCssProvider *label_provider3 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider3, label_css, -1, NULL);
    GtkStyleContext *label_context3 = gtk_widget_get_style_context(confirm_password_label);
    gtk_style_context_add_provider(label_context3, GTK_STYLE_PROVIDER(label_provider3), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider3);
    gtk_widget_set_halign(confirm_password_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(confirm_password_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), confirm_password_label, FALSE, FALSE, 0);
    
    entries[2] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[2]), "Repeat new password (optional)");
    gtk_entry_set_visibility(GTK_ENTRY(entries[2]), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(entries[2]), '*');
    gtk_entry_set_width_chars(GTK_ENTRY(entries[2]), 30);
    style_entry(entries[2]);
    gtk_widget_set_margin_bottom(entries[2], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[2], FALSE, FALSE, 0);
    
    // 4. Account Type - Combobox (preload current type)
    GtkWidget *account_type_label = gtk_label_new("Account Type:");
    GtkCssProvider *label_provider4 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider4, label_css, -1, NULL);
    GtkStyleContext *label_context4 = gtk_widget_get_style_context(account_type_label);
    gtk_style_context_add_provider(label_context4, GTK_STYLE_PROVIDER(label_provider4), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider4);
    gtk_widget_set_halign(account_type_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(account_type_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), account_type_label, FALSE, FALSE, 0);
    
    entries[3] = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[3]), "savings");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[3]), "checking");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[3]), "credit");
    // Set active based on current account type
    if (account_type != NULL) {
        if (strcmp(account_type, "savings") == 0) {
            gtk_combo_box_set_active(GTK_COMBO_BOX(entries[3]), 0);
        } else if (strcmp(account_type, "checking") == 0) {
            gtk_combo_box_set_active(GTK_COMBO_BOX(entries[3]), 1);
        } else if (strcmp(account_type, "credit") == 0) {
            gtk_combo_box_set_active(GTK_COMBO_BOX(entries[3]), 2);
        } else {
            gtk_combo_box_set_active(GTK_COMBO_BOX(entries[3]), 0);
        }
    } else {
        gtk_combo_box_set_active(GTK_COMBO_BOX(entries[3]), 0);
    }
    style_entry(entries[3]);
    gtk_widget_set_margin_bottom(entries[3], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[3], FALSE, FALSE, 0);
    
    // 5. Phone Number (preload current)
    GtkWidget *phone_number_label = gtk_label_new("Phone Number:");
    GtkCssProvider *label_provider5 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider5, label_css, -1, NULL);
    GtkStyleContext *label_context5 = gtk_widget_get_style_context(phone_number_label);
    gtk_style_context_add_provider(label_context5, GTK_STYLE_PROVIDER(label_provider5), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider5);
    gtk_widget_set_halign(phone_number_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(phone_number_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), phone_number_label, FALSE, FALSE, 0);
    
    entries[4] = gtk_entry_new();
    if (phone != NULL) {
        gtk_entry_set_text(GTK_ENTRY(entries[4]), phone);
    } else {
        gtk_entry_set_placeholder_text(GTK_ENTRY(entries[4]), "Enter your phone number");
    }
    gtk_entry_set_width_chars(GTK_ENTRY(entries[4]), 30);
    style_entry(entries[4]);
    gtk_widget_set_margin_bottom(entries[4], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[4], FALSE, FALSE, 0);
    
    // 6. Name field (preload current - combined First + Second Name)
    GtkWidget *name_label = gtk_label_new("Name:");
    GtkCssProvider *label_provider6 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider6, label_css, -1, NULL);
    GtkStyleContext *label_context6 = gtk_widget_get_style_context(name_label);
    gtk_style_context_add_provider(label_context6, GTK_STYLE_PROVIDER(label_provider6), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider6);
    gtk_widget_set_halign(name_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(name_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), name_label, FALSE, FALSE, 0);
    
    entries[5] = gtk_entry_new();
    if (full_name != NULL) {
        gtk_entry_set_text(GTK_ENTRY(entries[5]), full_name);
    } else {
        gtk_entry_set_placeholder_text(GTK_ENTRY(entries[5]), "Enter your full name");
    }
    gtk_entry_set_width_chars(GTK_ENTRY(entries[5]), 30);
    style_entry(entries[5]);
    gtk_widget_set_margin_bottom(entries[5], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[5], FALSE, FALSE, 0);
    
    // 7. Birthday - Date Entry (preload current - DD/MM/YYYY format)
    GtkWidget *birthday_label = gtk_label_new("Birthday:");
    GtkCssProvider *label_provider7 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider7, label_css, -1, NULL);
    GtkStyleContext *label_context7 = gtk_widget_get_style_context(birthday_label);
    gtk_style_context_add_provider(label_context7, GTK_STYLE_PROVIDER(label_provider7), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider7);
    gtk_widget_set_halign(birthday_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(birthday_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), birthday_label, FALSE, FALSE, 0);
    
    entries[6] = gtk_entry_new();
    if (birthday_str != NULL) {
        gtk_entry_set_text(GTK_ENTRY(entries[6]), birthday_str);
    } else {
        gtk_entry_set_placeholder_text(GTK_ENTRY(entries[6]), "DD/MM/YYYY (e.g., 10/02/2025)");
    }
    gtk_entry_set_width_chars(GTK_ENTRY(entries[6]), 30);
    style_entry(entries[6]);
    gtk_widget_set_margin_bottom(entries[6], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[6], FALSE, FALSE, 0);
    
    // Free temporary strings
    g_free(birthday_str);
    g_free(full_name);
    g_object_unref(label_provider);
    
    // Buttons integrated in the form card
    GtkWidget *save_changes_button = gtk_button_new_with_label("Save Changes");
    GtkWidget *cancel_button = gtk_button_new_with_label("Cancel");
    
    // Style Save Changes button - Green
    gtk_widget_set_margin_top(save_changes_button, 5);
    gtk_widget_set_margin_bottom(save_changes_button, 5);
    gtk_widget_set_halign(save_changes_button, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(save_changes_button, TRUE);
    style_success_button(save_changes_button);
    g_signal_connect(G_OBJECT(save_changes_button), "clicked", G_CALLBACK(edit_an_account), entries);
    gtk_box_pack_start(GTK_BOX(form_card), save_changes_button, FALSE, FALSE, 0);
    
    // Style Cancel button - Red
    gtk_widget_set_margin_top(cancel_button, 5);
    gtk_widget_set_margin_bottom(cancel_button, 5);
    gtk_widget_set_halign(cancel_button, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(cancel_button, TRUE);
    style_danger_button(cancel_button);
    g_signal_connect(G_OBJECT(cancel_button), "clicked", G_CALLBACK(show_account_interface), NULL);
    g_signal_connect(G_OBJECT(cancel_button), "clicked", G_CALLBACK(close_window), edit_account_window);
    gtk_box_pack_start(GTK_BOX(form_card), cancel_button, FALSE, FALSE, 0);
    
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), main_box);
    gtk_widget_show_all(GTK_WIDGET(edit_account_window));
    if (data != NULL)
        gtk_widget_destroy(data);
}

// Create new window with all account options after the user is logged.
void show_account_interface() {

    GtkWidget *your_account_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_icon_from_file("images/bank_icon.png", NULL);
    gtk_window_set_title(GTK_WINDOW(your_account_window), "GentlixBank - Application");
    gtk_window_set_default_size(GTK_WINDOW(your_account_window), 800, 1000);
    gtk_window_maximize(GTK_WINDOW(your_account_window));

    GtkWidget *overlay_main_box = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(your_account_window), overlay_main_box);
    GtkWidget *image_app_background = gtk_image_new_from_file("images/app_background.png");
    gtk_widget_set_sensitive(image_app_background, FALSE);  // Allow clicks to pass through
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), image_app_background);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_set_spacing(GTK_BOX(main_box), 0);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 50);

    // Spacer to push header to center
    GtkWidget *spacer_top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_sensitive(spacer_top, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), spacer_top, TRUE, TRUE, 0);
    
    // Header box with logo, title and subtitle in column (centered like main menu)
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(header_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(header_box, 30);
    gtk_widget_set_margin_bottom(header_box, 20);
    
    // Logo on top
    GtkWidget *image_logo = gtk_image_new_from_file("images/bank_logo.png");
    gtk_widget_set_halign(image_logo, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(header_box), image_logo, FALSE, FALSE, 0);

    // Title below logo - "Welcome [Name]"
    gchar *title_text;
    if (currentAccount != NULL) {
        title_text = g_strdup_printf("Welcome %s", getAccountFirstName(currentAccount));
    } else {
        title_text = g_strdup("Welcome");
    }
    GtkWidget *text_title = gtk_label_new(title_text);
    g_free(title_text);
    style_title_label(text_title, "#FFDFAF");
    gtk_widget_set_halign(text_title, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(header_box), text_title, FALSE, FALSE, 10);

    // Subtitle below title - Account balance
    gchar *balance_text;
    if (currentAccount != NULL) {
        balance_text = g_strdup_printf("Account balance: %.2f$", getAccountBalance(currentAccount));
    } else {
        balance_text = g_strdup("Account balance: 0.00$");
    }
    GtkWidget *text_subtitle = gtk_label_new(balance_text);
    g_free(balance_text);
    style_subtitle_label(text_subtitle, "#E7E7E7");
    gtk_widget_set_halign(text_subtitle, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(header_box), text_subtitle, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(main_box), header_box, FALSE, FALSE, 0);
    
    // Spacer to push buttons to bottom
    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_sensitive(spacer, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), spacer, TRUE, TRUE, 0);
    
    // Group buttons at the bottom - match width with upper banner
    GtkWidget *lower_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_size_request(GTK_WIDGET(lower_box), -1, -1);
    gtk_container_set_border_width(GTK_CONTAINER(lower_box), 50);  // Match main_box border
    gtk_widget_set_halign(lower_box, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(lower_box, TRUE);  // Allow horizontal expansion
    gtk_widget_set_vexpand(lower_box, FALSE);  // Don't expand vertically
    gtk_box_pack_start(GTK_BOX(main_box), lower_box, FALSE, FALSE, 0);

    GtkWidget *new_transaction = gtk_button_new_with_label("New transaction");
    GtkWidget *transactions_button = gtk_button_new_with_label("Your transactions");
    GtkWidget *statements_button = gtk_button_new_with_label("Account statements");
    GtkWidget *edit_button = gtk_button_new_with_label("Edit account");
    GtkWidget *delete_button = gtk_button_new_with_label("Delete account");
    GtkWidget *logout_button = gtk_button_new_with_label("Logout");

    GtkWidget *buttons[] = {new_transaction, transactions_button, statements_button, edit_button, delete_button, logout_button};
    for (int i = 0; i < G_N_ELEMENTS(buttons); i++) {
        gtk_widget_set_margin_top(buttons[i], 8);
        gtk_widget_set_margin_bottom(buttons[i], 8);
        gtk_widget_set_halign(buttons[i], GTK_ALIGN_CENTER);
        gtk_widget_set_hexpand(buttons[i], TRUE);  // Allow buttons to expand horizontally
        // Style buttons based on their function
        if (i == 0) {
            style_primary_button(buttons[i]);
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(show_new_transaction_interface), your_account_window);
        } else if (i == 1) {
            style_success_button_main_menu(buttons[i]);  // Green with dark text for main menu
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(show_all_transactions_interface), your_account_window);
        } else if (i == 2) {
            style_secondary_button(buttons[i]);
            // Account statements - placeholder for future implementation
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(show_error), "Account statements feature coming soon!");
        } else if (i == 3) {
            style_warning_button(buttons[i]);
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(show_edit_account_interface), your_account_window);
        } else if (i == 4) {
            style_danger_button_main_menu(buttons[i]);  // Red with dark text for main menu
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(delete_an_account), NULL);
            GtkWidget *main_window = g_object_get_data(G_OBJECT(app), "main_window");
            if (main_window != NULL) {
                g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(show_window), main_window);
            }
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(close_window), your_account_window);
        } else if (i == 5) {
            style_secondary_button(buttons[i]);
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(logout_from_an_account), NULL);
            GtkWidget *main_window = g_object_get_data(G_OBJECT(app), "main_window");
            if (main_window != NULL) {
                g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(show_window), main_window);
            }
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(close_window), your_account_window);
        }
        gtk_box_pack_start(GTK_BOX(lower_box), buttons[i], FALSE, FALSE, 0);
    }
    // Add main_box to overlay AFTER image_app_background so it's on top and receives events
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), main_box);
    gtk_widget_show_all(GTK_WIDGET(your_account_window));
}

// Create the login interface with input fields
// GtkWidget *widget - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the window from which this menu was opened to hide it
void show_login_interface(GtkWidget *widget, gpointer data) {

    GtkWidget *login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_object_set_data(G_OBJECT(app), "last_window", login_window);

    gtk_window_set_default_icon_from_file("images/bank_icon.png", NULL);
    gtk_window_set_title(GTK_WINDOW(login_window), "GentlixBank - Application");
    gtk_window_set_default_size(GTK_WINDOW(login_window), 800, 1000);
    gtk_window_maximize(GTK_WINDOW(login_window));

    GtkWidget *overlay_main_box = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(login_window), overlay_main_box);
    GtkWidget *image_app_background = gtk_image_new_from_file("images/app_background.png");
    gtk_widget_set_sensitive(image_app_background, FALSE);  // Allow clicks to pass through
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), image_app_background);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_set_spacing(GTK_BOX(main_box), 0);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 50);

    // Spacer to push header to center
    GtkWidget *spacer_top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_sensitive(spacer_top, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), spacer_top, TRUE, TRUE, 0);
    
    // Header box with logo and title side by side (no card)
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_halign(header_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(header_box, 30);
    gtk_widget_set_margin_bottom(header_box, 20);
    
    // Logo on the left
    GtkWidget *image_logo = gtk_image_new_from_file("images/bank_logo.png");
    gtk_box_pack_start(GTK_BOX(header_box), image_logo, FALSE, FALSE, 0);

    // Title on the right (centered vertically with logo)
    GtkWidget *text_title = gtk_label_new("Login");
    style_title_label(text_title, "#FFDFAF");
    gtk_widget_set_valign(text_title, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(header_box), text_title, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(main_box), header_box, FALSE, FALSE, 0);
    
    // Small spacer between header and form card
    GtkWidget *card_spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(card_spacer, -1, 20);
    gtk_widget_set_sensitive(card_spacer, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), card_spacer, FALSE, FALSE, 0);
    
    // Form card container
    GtkWidget *form_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(form_card, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(form_card, 500, -1);
    gtk_container_set_border_width(GTK_CONTAINER(form_card), 40);
    
    // Style the form card with background and yellow border
    const gchar *card_css = 
        "box { "
        "background-color: #FFFFFF; "
        "border-radius: 12px; "
        "border: 3px solid #FFD700; "
        "padding: 30px; "
        "}";
    GtkCssProvider *card_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(card_provider, card_css, -1, NULL);
    GtkStyleContext *card_context = gtk_widget_get_style_context(form_card);
    gtk_style_context_add_provider(card_context, GTK_STYLE_PROVIDER(card_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(card_provider);
    
    gtk_box_pack_start(GTK_BOX(main_box), form_card, FALSE, FALSE, 0);

    GtkWidget **entries = (GtkWidget **)g_malloc(3 * sizeof(GtkWidget *));

    // Usertag field
    GtkWidget *username_label = gtk_label_new("Usertag:");
    const gchar *label_css = "label { font-size: 16px; color: #2C3E50; font-weight: 600; }";
    GtkCssProvider *label_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider, label_css, -1, NULL);
    GtkStyleContext *label_context = gtk_widget_get_style_context(username_label);
    gtk_style_context_add_provider(label_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider);
    gtk_widget_set_halign(username_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(username_label, 2);  // Even closer to input field
    gtk_box_pack_start(GTK_BOX(form_card), username_label, FALSE, FALSE, 0);

    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Enter your account tag");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 30);
    style_entry(entries[0]);
    gtk_widget_set_margin_bottom(entries[0], 20);
    gtk_box_pack_start(GTK_BOX(form_card), entries[0], FALSE, FALSE, 0);

    // Password field
    GtkWidget *password_label = gtk_label_new("Password:");
    GtkCssProvider *label_provider2 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider2, label_css, -1, NULL);
    GtkStyleContext *label_context2 = gtk_widget_get_style_context(password_label);
    gtk_style_context_add_provider(label_context2, GTK_STYLE_PROVIDER(label_provider2), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider2);
    gtk_widget_set_halign(password_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(password_label, 2);  // Even closer to input field
    gtk_box_pack_start(GTK_BOX(form_card), password_label, FALSE, FALSE, 0);

    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter account password");
    gtk_entry_set_visibility(GTK_ENTRY(entries[1]), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(entries[1]), '*');
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 30);
    style_entry(entries[1]);
    gtk_widget_set_margin_bottom(entries[1], 25);  // Space before buttons
    gtk_box_pack_start(GTK_BOX(form_card), entries[1], FALSE, FALSE, 0);
    
    // Buttons integrated in the form card
    GtkWidget *login_button = gtk_button_new_with_label("Login");
    GtkWidget *close_button = gtk_button_new_with_label("Close");

    // Style Login button - Green
    gtk_widget_set_margin_top(login_button, 5);
    gtk_widget_set_margin_bottom(login_button, 5);
    gtk_widget_set_halign(login_button, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(login_button, TRUE);
    style_success_button(login_button);  // Green for Login
    g_signal_connect(G_OBJECT(login_button), "clicked", G_CALLBACK(login_to_an_account), entries);
    gtk_box_pack_start(GTK_BOX(form_card), login_button, FALSE, FALSE, 0);
    
    // Style Close button - Red
    gtk_widget_set_margin_top(close_button, 5);
    gtk_widget_set_margin_bottom(close_button, 5);
    gtk_widget_set_halign(close_button, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(close_button, TRUE);
    style_danger_button(close_button);  // Red for Close
    g_signal_connect(G_OBJECT(close_button), "clicked", G_CALLBACK(show_window), data);
    g_signal_connect(G_OBJECT(close_button), "clicked", G_CALLBACK(close_window), login_window);
    gtk_box_pack_start(GTK_BOX(form_card), close_button, FALSE, FALSE, 0);
    
    // Spacer to push cards to center (bottom)
    GtkWidget *spacer_bottom = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_sensitive(spacer_bottom, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), spacer_bottom, TRUE, TRUE, 0);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), main_box);
    gtk_widget_show_all(GTK_WIDGET(login_window));
    gtk_widget_hide(data);  // Hide main menu to be able to show it anytime
}

// Create the register interface with input fields.
// GtkWidget *widget - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the window from which this menu was opened to hide it
void show_register_interface(GtkWidget *widget, gpointer data) {

    GtkWidget *register_window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // Create a new window for register interface
    g_object_set_data(G_OBJECT(app), "last_window", register_window); // Store the register window in the GtkApplication

    gtk_window_set_default_icon_from_file("images/bank_icon.png", NULL);
    gtk_window_set_title(GTK_WINDOW(register_window), "GentlixBank - Application");
    gtk_window_set_default_size(GTK_WINDOW(register_window), 800, 1000);
    gtk_window_maximize(GTK_WINDOW(register_window));

    GtkWidget *overlay_main_box = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(register_window), overlay_main_box);
    GtkWidget *image_app_background = gtk_image_new_from_file("images/app_background.png");
    gtk_widget_set_sensitive(image_app_background, FALSE);  // Allow clicks to pass through
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), image_app_background);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_set_spacing(GTK_BOX(main_box), 0);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 50);

    // Spacer to push header to center
    GtkWidget *spacer_top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_sensitive(spacer_top, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), spacer_top, TRUE, TRUE, 0);
    
    // Header box with logo and title side by side (centered like login)
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_halign(header_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(header_box, 30);
    gtk_widget_set_margin_bottom(header_box, 20);
    
    // Logo on the left
    GtkWidget *image_logo = gtk_image_new_from_file("images/bank_logo.png");
    gtk_box_pack_start(GTK_BOX(header_box), image_logo, FALSE, FALSE, 0);

    // Title on the right (centered vertically with logo)
    GtkWidget *text_title = gtk_label_new("Register");
    style_title_label(text_title, "#FFDFAF");
    gtk_widget_set_valign(text_title, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(header_box), text_title, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(main_box), header_box, FALSE, FALSE, 0);
    
    // Small spacer between header and form card
    GtkWidget *card_spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(card_spacer, -1, 20);
    gtk_widget_set_sensitive(card_spacer, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), card_spacer, FALSE, FALSE, 0);

    // Form card container (same size as login, reduced padding for space)
    GtkWidget *form_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(form_card, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(form_card, 500, -1);
    gtk_container_set_border_width(GTK_CONTAINER(form_card), 30);
    
    // Style the form card with background and yellow border
    const gchar *card_css = 
        "box { "
        "background-color: #FFFFFF; "
        "border-radius: 12px; "
        "border: 3px solid #FFD700; "
        "padding: 30px; "
        "}";
    GtkCssProvider *card_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(card_provider, card_css, -1, NULL);
    GtkStyleContext *card_context = gtk_widget_get_style_context(form_card);
    gtk_style_context_add_provider(card_context, GTK_STYLE_PROVIDER(card_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(card_provider);
    
    gtk_box_pack_start(GTK_BOX(main_box), form_card, FALSE, FALSE, 0);
    
    // Spacer to push buttons to bottom
    GtkWidget *spacer_bottom = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_sensitive(spacer_bottom, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), spacer_bottom, TRUE, TRUE, 0);

    // Allocate entries array: 0=usertag, 1=name, 2=password, 3=account_type, 4=phone, 5=birthday
    GtkWidget **entries = (GtkWidget **)g_malloc(6 * sizeof(GtkWidget *));
    
    // Label styling (smaller for space)
    const gchar *label_css = "label { font-size: 14px; color: #2C3E50; font-weight: 600; }";
    GtkCssProvider *label_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider, label_css, -1, NULL);
    
    // 1. Usertag field
    GtkWidget *username_label = gtk_label_new("Usertag:");
    GtkStyleContext *label_context = gtk_widget_get_style_context(username_label);
    gtk_style_context_add_provider(label_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_widget_set_halign(username_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(username_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), username_label, FALSE, FALSE, 0);
    
    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Enter an account tag");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 30);
    style_entry(entries[0]);
    gtk_widget_set_margin_bottom(entries[0], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[0], FALSE, FALSE, 0);
    
    // 2. Name field (combined First + Second Name)
    GtkWidget *name_label = gtk_label_new("Name:");
    GtkCssProvider *label_provider2 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider2, label_css, -1, NULL);
    GtkStyleContext *label_context2 = gtk_widget_get_style_context(name_label);
    gtk_style_context_add_provider(label_context2, GTK_STYLE_PROVIDER(label_provider2), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider2);
    gtk_widget_set_halign(name_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(name_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), name_label, FALSE, FALSE, 0);
    
    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter your full name");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 30);
    style_entry(entries[1]);
    gtk_widget_set_margin_bottom(entries[1], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[1], FALSE, FALSE, 0);
    
    // 3. Password field
    GtkWidget *password_label = gtk_label_new("Password:");
    GtkCssProvider *label_provider3 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider3, label_css, -1, NULL);
    GtkStyleContext *label_context3 = gtk_widget_get_style_context(password_label);
    gtk_style_context_add_provider(label_context3, GTK_STYLE_PROVIDER(label_provider3), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider3);
    gtk_widget_set_halign(password_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(password_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), password_label, FALSE, FALSE, 0);
    
    entries[2] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[2]), "Enter account password");
    gtk_entry_set_visibility(GTK_ENTRY(entries[2]), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(entries[2]), '*');
    gtk_entry_set_width_chars(GTK_ENTRY(entries[2]), 30);
    style_entry(entries[2]);
    gtk_widget_set_margin_bottom(entries[2], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[2], FALSE, FALSE, 0);
    
    // 4. Account Type - Combobox
    GtkWidget *account_type_label = gtk_label_new("Account Type:");
    GtkCssProvider *label_provider4 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider4, label_css, -1, NULL);
    GtkStyleContext *label_context4 = gtk_widget_get_style_context(account_type_label);
    gtk_style_context_add_provider(label_context4, GTK_STYLE_PROVIDER(label_provider4), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider4);
    gtk_widget_set_halign(account_type_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(account_type_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), account_type_label, FALSE, FALSE, 0);
    
    entries[3] = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[3]), "savings");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[3]), "checking");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[3]), "credit");
    gtk_combo_box_set_active(GTK_COMBO_BOX(entries[3]), 0);  // Default to first option
    style_entry(entries[3]);
    gtk_widget_set_margin_bottom(entries[3], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[3], FALSE, FALSE, 0);
    
    // 5. Phone Number
    GtkWidget *phone_number_label = gtk_label_new("Phone Number:");
    GtkCssProvider *label_provider5 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider5, label_css, -1, NULL);
    GtkStyleContext *label_context5 = gtk_widget_get_style_context(phone_number_label);
    gtk_style_context_add_provider(label_context5, GTK_STYLE_PROVIDER(label_provider5), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider5);
    gtk_widget_set_halign(phone_number_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(phone_number_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), phone_number_label, FALSE, FALSE, 0);
    
    entries[4] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[4]), "Enter your phone number");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[4]), 30);
    style_entry(entries[4]);
    gtk_widget_set_margin_bottom(entries[4], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[4], FALSE, FALSE, 0);
    
    // 6. Birthday - Date Entry (DD/MM/YYYY format)
    GtkWidget *birthday_label = gtk_label_new("Birthday:");
    GtkCssProvider *label_provider6 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider6, label_css, -1, NULL);
    GtkStyleContext *label_context6 = gtk_widget_get_style_context(birthday_label);
    gtk_style_context_add_provider(label_context6, GTK_STYLE_PROVIDER(label_provider6), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(label_provider6);
    gtk_widget_set_halign(birthday_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(birthday_label, 2);
    gtk_box_pack_start(GTK_BOX(form_card), birthday_label, FALSE, FALSE, 0);
    
    entries[5] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[5]), "DD/MM/YYYY (e.g., 10/02/2025)");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[5]), 30);
    style_entry(entries[5]);
    gtk_widget_set_margin_bottom(entries[5], 10);
    gtk_box_pack_start(GTK_BOX(form_card), entries[5], FALSE, FALSE, 0);
    
    // Buttons integrated in the form card
    GtkWidget *create_account_button = gtk_button_new_with_label("Create account");
    GtkWidget *close_button = gtk_button_new_with_label("Close");
    
    // Style Create account button - Green
    gtk_widget_set_margin_top(create_account_button, 5);
    gtk_widget_set_margin_bottom(create_account_button, 5);
    gtk_widget_set_halign(create_account_button, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(create_account_button, TRUE);
    style_success_button(create_account_button);
    g_signal_connect(G_OBJECT(create_account_button), "clicked", G_CALLBACK(create_an_account), entries);
    gtk_box_pack_start(GTK_BOX(form_card), create_account_button, FALSE, FALSE, 0);
    
    // Style Close button - Red
    gtk_widget_set_margin_top(close_button, 5);
    gtk_widget_set_margin_bottom(close_button, 5);
    gtk_widget_set_halign(close_button, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(close_button, TRUE);
    style_danger_button(close_button);
    g_signal_connect(G_OBJECT(close_button), "clicked", G_CALLBACK(show_window), data);
    g_signal_connect(G_OBJECT(close_button), "clicked", G_CALLBACK(close_window), register_window);
    gtk_box_pack_start(GTK_BOX(form_card), close_button, FALSE, FALSE, 0);
    
    g_object_unref(label_provider);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), main_box);
    gtk_widget_show_all(GTK_WIDGET(register_window));
    gtk_widget_hide(data); // Hide main menu to be able to show it anytime
}

void show_import_interface(GtkWidget *widget, gpointer data) {
}

void show_export_interface(GtkWidget *widget, gpointer data) {
}

// Create main window and it's widgets. Here are the first options when you open the app.
// GtkApplication *app - provides a reference to the GTK application object, which you can use to interact with the
// GTK application and perform various operations through it. Here is used only to create the main window for the linked app for int main.
// gpointer data - is a generic pointer that can be used to pass additional data to the callback function. Here is passed a NULL one.
void activate_main_menu(GtkApplication *newApp, gpointer data) {

    // More about app layout and format inside the documentation file
    GtkWidget * main_window = gtk_application_window_new(newApp); // The main_window with main menu for app
    app = newApp;
    main_menu = main_window;
    g_object_set_data(G_OBJECT(newApp), "main_window", main_window); // The main main_window is saved in a global variable to be able from any function to show or hide it

    gtk_window_set_default_icon_from_file("images/bank_icon.png", NULL);
    gtk_window_set_title(GTK_WINDOW(main_window), "GentlixBank - Application");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 1000);
    gtk_window_maximize(GTK_WINDOW(main_window));

    GtkWidget *overlay_main_box = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(main_window), overlay_main_box);
    GtkWidget *image_app_background = gtk_image_new_from_file("images/app_background.png");
    gtk_widget_set_sensitive(image_app_background, FALSE);  // Allow clicks to pass through
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), image_app_background);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_set_spacing(GTK_BOX(main_box), 0);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 50);

    // Spacer to push header to center
    GtkWidget *spacer_top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_sensitive(spacer_top, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), spacer_top, TRUE, TRUE, 0);
    
    // Header box with logo, title and subtitle in column (centered like login)
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(header_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(header_box, 30);
    gtk_widget_set_margin_bottom(header_box, 20);
    
    // Logo on top
    GtkWidget *image_logo = gtk_image_new_from_file("images/bank_logo.png");
    gtk_widget_set_halign(image_logo, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(header_box), image_logo, FALSE, FALSE, 0);

    // Title below logo
    GtkWidget *text_title = gtk_label_new("Gentlix Bank");
    style_title_label(text_title, "#FFDFAF");
    gtk_widget_set_halign(text_title, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(header_box), text_title, FALSE, FALSE, 10);

    // Subtitle below title
    GtkWidget *text_subtitle = gtk_label_new("Banking reimagined, simplified");
    style_subtitle_label(text_subtitle, "#E7E7E7");
    gtk_widget_set_halign(text_subtitle, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(header_box), text_subtitle, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(main_box), header_box, FALSE, FALSE, 0);
    
    // Spacer to push buttons to bottom
    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_sensitive(spacer, FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), spacer, TRUE, TRUE, 0);
    
    // Group buttons at the bottom - match width with upper banner
    GtkWidget *lower_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_size_request(GTK_WIDGET(lower_box), -1, -1);
    gtk_container_set_border_width(GTK_CONTAINER(lower_box), 50);  // Match main_box border
    gtk_widget_set_halign(lower_box, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(lower_box, TRUE);  // Allow horizontal expansion
    gtk_widget_set_vexpand(lower_box, FALSE);  // Don't expand vertically
    gtk_box_pack_start(GTK_BOX(main_box), lower_box, FALSE, FALSE, 0);

    GtkWidget *login_button = gtk_button_new_with_label("Login");
    GtkWidget *register_button = gtk_button_new_with_label("Register");
    GtkWidget *admin_button = gtk_button_new_with_label("Admin");
    GtkWidget *close_button = gtk_button_new_with_label("Close");

    GtkWidget *buttons[] = {login_button, register_button, admin_button, close_button};
    for (int i = 0; i < G_N_ELEMENTS(buttons); i++) {
        gtk_widget_set_margin_top(buttons[i], 8);
        gtk_widget_set_margin_bottom(buttons[i], 8);
        gtk_widget_set_halign(buttons[i], GTK_ALIGN_CENTER);
        gtk_widget_set_hexpand(buttons[i], TRUE);  // Allow buttons to expand horizontally
        if (i == 0) {
            style_primary_button(buttons[i]);
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(show_login_interface), main_window);
        } else if (i == 1) {
            style_success_button_main_menu(buttons[i]);  // Green with dark text for main menu
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(show_register_interface), main_window);
        } else if (i == 2) {
            style_warning_button(buttons[i]);
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(show_import_interface), NULL);
        } else if (i == 3) {
            style_danger_button_main_menu(buttons[i]);  // Red with dark text for main menu
            g_signal_connect(G_OBJECT(buttons[i]), "clicked", G_CALLBACK(close_window), G_OBJECT(main_window));
        }
        gtk_box_pack_start(GTK_BOX(lower_box), buttons[i], FALSE, FALSE, 0);
    }
    // Add main_box to overlay AFTER image_app_background so it's on top and receives events
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main_box), main_box);
    gtk_widget_show_all(main_window); // Show the main_window after all formats
}