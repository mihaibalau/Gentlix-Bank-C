#include "services.h"
#include <gtk/gtk.h>
#include <string.h>
#include "../domain/domain.h"
#include "../repository/repository.h"
#include <stdlib.h>

////////////////////
//
//  Memory management functions
//
////////////////////

int addTransactionForUser(Account* account, Transaction* newTransaction) {
    if (account == NULL)
        return -201; // Invalid account

    if (newTransaction == NULL)
        return -202; // Invalid transaction

    if (account->transactionsNumber >= account->transactionsCapacity) {
        int newCapacity = account->transactionsCapacity * 2 + 1;
        Transaction** newTransactions = realloc(account->transactions, newCapacity * sizeof(Transaction*));
        if (newTransactions == NULL) {
            return -203; // Memory management error
        }
        account->transactions = newTransactions;
        account->transactionsCapacity = newCapacity;
    }

    account->transactions[account->transactionsNumber] = newTransaction;
    account->transactionsNumber++;

    return 1;
}

Transaction* getLatestTransaction(const Account* account) {
    if (account->transactionsNumber == 0) {
        return NULL;
    }

    return account->transactions[account->transactionsNumber - 1];
}

int addAffiliateToAccount(Account* account, Affiliate* newAffiliate) {
    if (account == NULL)
        return -211; // Invalid account

    if (newAffiliate == NULL)
        return -212; // Invalid transaction

    for (int i = 0; i < account->affiliatesNumber; i++) {
        if (strcmp(getAffiliatesTag(account->affiliates[i]), getAffiliatesTag(newAffiliate)) == 0) {
            return -213; // Affiliate already exists
        }
    }

    if (account->affiliatesNumber >= account->affiliatesCapacity) {
        int newCapacity = account->affiliatesCapacity * 2 + 1;
        Affiliate** newAffiliates = realloc(account->affiliates, newCapacity * sizeof(Affiliate*));
        if (newAffiliates == NULL) {
            return -214; // Memory reallocation failed
        }
        account->affiliates = newAffiliates;
        account->affiliatesCapacity = newCapacity;
    }

    account->affiliates[account->affiliatesNumber] = newAffiliate;
    account->affiliatesNumber++;

    return 1;
}


//void displayAffiliates(Account* account) {
//    for (int i = 0; i < account->affiliatesNumber; i++) {
//        Affiliate* affiliate = account->affiliates[i];
//        // Display affiliate details here, e.g., tag, name, IBAN, etc.
//        printf("Tag: %s, Name: %s %s, IBAN: %s\n", affiliate->tag, affiliate->firstName, affiliate->secondName, affiliate->iban);
//    }
//}

int removeAffiliateFromAccount(Account* account, const char* affiliateTag) {
    if (account == NULL)
        return -221; // Invalid account

    if (affiliateTag == NULL)
        return -222; // Invalid transaction

    int indexToRemove = -1;
    for (int i = 0; i < account->affiliatesNumber; i++) {
        if (strcmp(account->affiliates[i]->tag, affiliateTag) == 0) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        return -223; // Affiliate not found
    }

    destroyAffiliates(account->affiliates[indexToRemove]);
    free(account->affiliates[indexToRemove]);

    for (int i = indexToRemove; i < account->affiliatesNumber - 1; i++) {
        account->affiliates[i] = account->affiliates[i + 1];
    }

    account->affiliatesNumber--;

    return 1;
}

int addNewUserAccount(Account* account, UserAccounts* newUserAccount) {
    if (account == NULL)
        return -231; // Invalid account

    if (newUserAccount == NULL)
        return -232; // Invalid user account


    for (int i = 0; i < account->userAccountsNumber; i++) {
        if (strcmp(getUserAccountType(account->userAccounts[i]), getUserAccountType(newUserAccount)) == 0) {
            return -233; // User account already exists
        }
    }

    if (account->userAccountsNumber >= account->userAccountsCapacity) {
        int newCapacity = account->userAccountsCapacity * 2 + 1;
        UserAccounts** newUserAccounts = realloc(account->userAccounts, newCapacity * sizeof(UserAccounts*));
        if (newUserAccounts == NULL) {
            return -234; // Memory reallocation failed
        }
        account->userAccounts = newUserAccounts;
        account->userAccountsCapacity = newCapacity;
    }

    account->userAccounts[account->userAccountsNumber] = newUserAccount;
    account->userAccountsNumber++;

    return 1;
}

int removeAnUserAccount(Account* account, const char* userAccountType) {
    if (account == NULL)
        return -241; // Invalid account

    if (userAccountType == NULL)
        return -242; // Invalid user account type

    int indexToRemove = -1;
    for (int i = 0; i < account->userAccountsNumber; i++) {
        if (strcmp(account->userAccounts[i]->type, userAccountType) == 0) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        return -243; // User account not found
    }

    destroyUserAccount(account->userAccounts[indexToRemove]);
    free(account->userAccounts[indexToRemove]);

    for (int i = indexToRemove; i < account->userAccountsNumber - 1; i++) {
        account->userAccounts[i] = account->userAccounts[i + 1];
    }

    account->userAccountsNumber--;

    return 1;
}

//void displayUserAccounts(Account* account) {
//    for (int i = 0; i < account->userAccountsNumber; i++) {
//        UserAccounts* userAccount = account->userAccounts[i];
//        // Display user account details here, e.g., type, balance
//        printf("Type: %s, Balance: %.2f\n", userAccount->type, userAccount->accountBalance);
//    }
//}

////////////////////
//
//  Check functions
//
////////////////////

short accountTagUsed(const RepositoryFormat* receivedRepository, const gchar *checkedTag){
    const char *charString = checkedTag;
    return accountTagUsedRepo(receivedRepository, charString);
}

short stringOnlyWithLetters(const gchar *checkedString) {
    if (checkedString == NULL || *checkedString == '\0')
        return 0;  // Empty or NULL string is invalid

    size_t length = strlen(checkedString);
    for (size_t index = 0; index < length; index++) {
        if ((checkedString[index] < 'A' || checkedString[index] > 'z') ||
            (checkedString[index] > 'Z' && checkedString[index] < 'a')) {
            return 0;  // Not a letter (excluding ASCII gaps)
        }
    }
    return 1;  // Valid string
}

short stringOnlyWithDigitsExtended(const gchar *checkedString) {
    if (checkedString == NULL || *checkedString == '\0')
        return 0;  // Empty or NULL string is invalid

    for (size_t index = 0; checkedString[index] != '\0'; index++) {
        if ((checkedString[index] < '0' || checkedString[index] > '9') &&
            checkedString[index] != ',' && checkedString[index] != '.') {
            return 0;  // Not a digit, period, or comma
        }
    }
    return 1;  // Valid string
}


short stringOnlyWithDigits(const gchar *checkedString) {
    if (checkedString == NULL || *checkedString == '\0')
        return 0;  // Empty or NULL string is invalid

    for (size_t index = 0; checkedString[index] != '\0'; index++) {
        if (checkedString[index] < '0' || checkedString[index] > '9') {
            return 0;  // Not a digit
        }
    }
    return 1;  // Valid string
}

short differentPassword(const gchar *password1, const gchar *password2){
    if(strcmp(password1, password2))
        return 1;
    return 0;
}

short validDate(const gchar *day, const gchar *month, const gchar *year){ // available date means it is valid inside the calendar

    if(!stringOnlyWithDigits(day))
        return -121; // The day need to be a number!

    else if(!stringOnlyWithDigits(month))
        return -122; // The month need to be a number!

    else if(!stringOnlyWithDigits(year))
        return -123; // The year need to be a number!

    guint64 intDay = g_ascii_strtoull(day, NULL, 10); // transform a string formatted from digits to an integer
    guint64 intMonth = g_ascii_strtoull(month, NULL, 10); // this function is from gtk input options
    guint64 intYear = g_ascii_strtoull(year, NULL, 10);

    if(intYear < 1700)
        return -124; // The entered year is far too far away!

    else if(intYear > 2006)
        return -125; // You do not have a minimum age to open an account

    if(intMonth < 1 || intMonth > 12)
        return -126; // The month does not exist!

    if(intDay < 1 || intDay > 31)
        return -127; // The day does not exist!

    else if(intDay == 31 && (intMonth == 2 || intMonth == 4 || intMonth == 6 || intMonth == 9 || intMonth == 11) )
        return -128; // The month has only 30 days!

    else if(intDay == 30 && intMonth == 2)
        return -129; // The month can have a maximum of 29 days!

    else if(intDay == 29 && intMonth == 2 && intYear % 4 != 0)
        return -130; // The year February has a maximum of 28 days!

    return 1;
}


short validDateForTransaction(const gchar *day, const gchar *month, const gchar *year, const Account* account) {
    if (!stringOnlyWithDigits(day))
        return -141; // The day needs to be a number!
    else if (!stringOnlyWithDigits(month))
        return -142; // The month needs to be a number!
    else if (!stringOnlyWithDigits(year))
        return -143; // The year needs to be a number!

    guint64 intDay = g_ascii_strtoull(day, NULL, 10);
    guint64 intMonth = g_ascii_strtoull(month, NULL, 10);
    guint64 intYear = g_ascii_strtoull(year, NULL, 10);

    if (intYear < 1700)
        return -144; // There were no banks this year. People kept currency hidden in their homes!
    else if (intYear > 9999)
        return -145; // The year format is invalid. It must have a maximum of 4 digits!

    if (intMonth < 1 || intMonth > 12)
        return -146; // This month does not exist!

    if (intDay < 1 || intDay > 31)
        return -147; // This day does not exist!
    else if (intDay == 31 && (intMonth == 2 || intMonth == 4 || intMonth == 6 || intMonth == 9 || intMonth == 11))
        return -148; // This month has only 30 days!
    else if (intDay == 30 && intMonth == 2)
        return -149; // This month can have a maximum of 29 days!
    else if (intDay == 29 && intMonth == 2 && intYear % 4 != 0)
        return -150; // This year February has a maximum of 28 days!

    Transaction* latestTransaction = getLatestTransaction(account);
    if (latestTransaction != NULL) {
        Date latestDate = getTransactionDate(latestTransaction);
        if (intYear < latestDate.year ||
            (intYear == latestDate.year && intMonth < latestDate.month) ||
            (intYear == latestDate.year && intMonth == latestDate.month && intDay < latestDate.day))
            return -151; // The last transaction was recorded in the future. You can't introduce now something that has already happened.
    }

    return 1;
}

short availableAccountType(const char* accountType) {
    if (strcmp(accountType, "savings") == 0 || strcmp(accountType, "checking") == 0 || strcmp(accountType, "credit") == 0)
        return 1;
    return 0;
}

void generateRandomIBAN(char* iban) {
    const char* country_code = "RO";
    const char* bank_id = "GLBK";
    const char* branch_id = "0001";
    char checkDigits[2];
    char accountNumber[12];

    strcpy(iban, country_code);
    strcat(iban, checkDigits);
    strcat(iban, bank_id);
    strcat(iban, branch_id);
    for (int i = 0; i < 12; i++) {
        accountNumber[i] = '0' + (rand() % 10);
    }
    strcat(iban, accountNumber);
}

////////////////////
//
//  Services functions
//
////////////////////


int loginService(RepositoryFormat* repository, const char* username, const char* password, Account** loggedUser) {

    if (repository == NULL)
        return -301; // Internal problem with program database.
    else if(username == NULL)
        return -302; // Missing username input.
    else if(password == NULL)
        return -303; // Missing the password for your account.

    if(strlen(username) > 20)
        return -304; // Imputed account tag is too long! Maximum 20 characters.
    else if(strlen(password) > 32)
        return -305; // Imputed password is too long! Maximum 32 characters.
    else if(!stringOnlyWithLetters(username))
        return -306; // Account tag can have only letters!

    Account* foundAccount = loginRepository(repository, username, password);
    if (foundAccount == NULL) {
        return -307; // Account not found or wrong password
    }
    *loggedUser = foundAccount;
    return 1;
}

int createAccountService(RepositoryFormat* repository, const char* accountTag, const char* password, const char* passwordConfirm, const char* accountType, const char* phoneNumber,
                         const char* firstName, const char* secondName, const char* day, const char* month, const char* year, Account** loggedAccount) {


    if (repository == NULL)
        return -311; // Repository is NULL
    else if (accountTag == NULL)
        return -312; // Missing account tag input
    else if (password == NULL)
        return -313; // Missing password input
    else if (passwordConfirm == NULL)
        return -314; // Missing password confirmation input
    else if (accountType == NULL)
        return -315; // Missing account type input
    else if (phoneNumber == NULL)
        return -316; // Missing phone number input
    else if (firstName == NULL)
        return -317; // Missing first name input
    else if (secondName == NULL)
        return -318; // Missing second name input
    else if (day == NULL)
        return -319; // Missing day input
    else if (month == NULL)
        return -320; // Missing month input
    else if (year == NULL)
        return -321; // Missing year input

    if (accountTagUsed(repository, accountTag))
        return -322; // Account tag already used

    if (validDate(day, month, year) != 1) {
        return -323; // Invalid birthday.
    }

    if(!stringOnlyWithLetters(accountTag))
        return -324; // Account tag can have only letters!
    else if(differentPassword(password, passwordConfirm))
        return -325; // The passwords do not match!
    else if(!availableAccountType(accountType))
        return -326; // Invalid account type!\nAvailable types: savings, checking, credit.
    else if(!stringOnlyWithLetters(firstName))
        return -327; // First name can have only letters!
    else if(!stringOnlyWithLetters(secondName))
        return -328; // Second name can have only letters!
    else if(!stringOnlyWithDigits(phoneNumber))
        return -329; // Phone number can have only digits!"

    Date birthday = createDate(atoi(day), atoi(month), atoi(year));

    char iban[30];
    char* uniqueIBAN = NULL;
    while (1) {
        generateRandomIBAN(iban);
        if (!ibanUsedInRepository(repository, iban)) {
            uniqueIBAN = malloc(strlen(iban) + 1);
            if (uniqueIBAN == NULL) {
                return -333; // Memory allocation failed
            }
            strcpy(uniqueIBAN, iban);
            break;
        }
    }

    Account* newAccount = createAccount(0.0, accountTag, firstName, secondName, password, uniqueIBAN, phoneNumber, birthday);
    free(uniqueIBAN);

    if (newAccount == NULL)
        return -330; // Failed to create an account

    if (addAccountToRepository(repository, newAccount) != 1) {
        destroyAccount(newAccount);
        free(newAccount);
        return -331; // Failed to add account to repository
    }

    // Create a new user account and link it to the new account
    UserAccounts* newUserAccount = createUserAccount(0.0, accountType);
    if (newUserAccount == NULL) {
        destroyAccount(newAccount);
        removeAccountFromRepository(repository, accountTag);
        return -332; // Failed to create user account
    }

    int resultCode = addNewUserAccount(newAccount, newUserAccount);
    if (resultCode != 1) {
        destroyAccount(newAccount);
        removeAccountFromRepository(repository, accountTag);
        destroyUserAccount(newUserAccount);
        free(newUserAccount);
        return resultCode; // Failed to add user account
    }

    *loggedAccount = newAccount;
    return 1;
}

int deleteAccountService(RepositoryFormat* repository, Account** loggedAccount){
    if (repository == NULL || loggedAccount == NULL || *loggedAccount == NULL)
        return -351; // Invalid parameters
    
    const char* accountTag = getAccountTag(*loggedAccount);
    if (accountTag == NULL)
        return -352; // Invalid account tag
    
    int result = removeAccountFromRepository(repository, accountTag);
    if (result == 1) {
        *loggedAccount = NULL; // Clear the logged account pointer
    }
    return result;
}

int editAccountService(Account** loggedAccount, const char* currentPassword, const char* password, const char* passwordConfirm, const char* accountType, const char* phoneNumber,
                         const char* firstName, const char* secondName, const char* day, const char* month, const char* year) {

    if (loggedAccount == NULL || *loggedAccount == NULL)
        return -340; // Invalid account
    
    if (currentPassword == NULL || strlen(currentPassword) == 0)
        return -341; // Missing current password
    
    if(differentPassword(currentPassword, getAccountPassword(*loggedAccount)))
        return -341; // The imputed password is wrong!
    
    if (password != NULL && strlen(password) > 0) {
        if (passwordConfirm == NULL || strlen(passwordConfirm) == 0)
            return -342; // Missing password confirmation
        if(differentPassword(password, passwordConfirm))
            return -342; // The passwords do not match!
    }
    
    if (accountType != NULL && strlen(accountType) > 0 && !availableAccountType(accountType))
        return -343; // Invalid account type!\nAvailable types: savings, checking, credit.
    
    if (firstName != NULL && strlen(firstName) > 0 && !stringOnlyWithLetters(firstName))
        return -344; // First name can have only letters!
    
    if (secondName != NULL && strlen(secondName) > 0 && !stringOnlyWithLetters(secondName))
        return -345; // Second name can have only letters!
    
    if (phoneNumber != NULL && strlen(phoneNumber) > 0 && !stringOnlyWithDigits(phoneNumber))
        return -346; // Phone number can have only digits!

    if (password != NULL && strlen(password) > 0)
        setAccountPassword(*loggedAccount, password);
    if (phoneNumber != NULL && strlen(phoneNumber) > 0)
        setAccountPhoneNumber(*loggedAccount, phoneNumber);
    if (firstName != NULL && strlen(firstName) > 0)
        setAccountFirstName(*loggedAccount, firstName);
    if (secondName != NULL && strlen(secondName) > 0)
        setAccountSecondName(*loggedAccount, secondName);
    
    return 1;
}

////////////////////
//
//  Transaction services
//
////////////////////

int depositService(Account* account, const char* amount, const char* description, const char* day, const char* month, const char* year) {
    if (account == NULL)
        return -401; // Invalid account
    
    if (amount == NULL || strlen(amount) == 0)
        return -402; // Missing amount
    
    if (description == NULL)
        return -403; // Missing description
    
    if (strlen(description) > 99)
        return -404; // Description too long
    
    if (!stringOnlyWithDigitsExtended(amount))
        return -405; // Amount is not a number
    
    gdouble moneyAmount = g_ascii_strtod(amount, NULL);
    if (moneyAmount <= 0)
        return -406; // Invalid amount
    
    int dateResult = validDateForTransaction(day, month, year, account);
    if (dateResult != 1)
        return dateResult; // Invalid date
    
    Date transactionDate = createDate((short)atoi(day), (short)atoi(month), (short)atoi(year));
    
    Transaction* newTransaction = createTransaction(moneyAmount, "main", "deposit", "", "deposit", description, transactionDate);
    if (newTransaction == NULL)
        return -407; // Failed to create transaction
    
    int result = addTransactionForUser(account, newTransaction);
    if (result != 1) {
        destroyTransaction(newTransaction);
        free(newTransaction);
        return result;
    }
    
    setAccountBalance(account, getAccountBalance(account) + moneyAmount);
    
    return 1;
}

int withdrawService(Account* account, const char* amount, const char* description, const char* day, const char* month, const char* year) {
    if (account == NULL)
        return -411; // Invalid account
    
    if (amount == NULL || strlen(amount) == 0)
        return -412; // Missing amount
    
    if (description == NULL)
        return -413; // Missing description
    
    if (strlen(description) > 99)
        return -414; // Description too long
    
    if (!stringOnlyWithDigitsExtended(amount))
        return -415; // Amount is not a number
    
    gdouble moneyAmount = g_ascii_strtod(amount, NULL);
    if (moneyAmount <= 0)
        return -416; // Invalid amount
    
    if (getAccountBalance(account) < moneyAmount)
        return -417; // Insufficient balance
    
    int dateResult = validDateForTransaction(day, month, year, account);
    if (dateResult != 1)
        return dateResult; // Invalid date
    
    Date transactionDate = createDate((short)atoi(day), (short)atoi(month), (short)atoi(year));
    
    Transaction* newTransaction = createTransaction(moneyAmount, "main", "withdraw", "", "withdraw", description, transactionDate);
    if (newTransaction == NULL)
        return -418; // Failed to create transaction
    
    int result = addTransactionForUser(account, newTransaction);
    if (result != 1) {
        destroyTransaction(newTransaction);
        free(newTransaction);
        return result;
    }
    
    setAccountBalance(account, getAccountBalance(account) - moneyAmount);
    
    return 1;
}

int transferService(Account* account, const char* amount, const char* description, const char* receiverIBAN, const char* day, const char* month, const char* year) {
    if (account == NULL)
        return -421; // Invalid account
    
    if (amount == NULL || strlen(amount) == 0)
        return -422; // Missing amount
    
    if (description == NULL)
        return -423; // Missing description
    
    if (receiverIBAN == NULL || strlen(receiverIBAN) == 0)
        return -424; // Missing receiver IBAN
    
    if (strlen(description) > 99)
        return -425; // Description too long
    
    if (!stringOnlyWithDigitsExtended(amount))
        return -426; // Amount is not a number
    
    gdouble moneyAmount = g_ascii_strtod(amount, NULL);
    if (moneyAmount <= 0)
        return -427; // Invalid amount
    
    if (getAccountBalance(account) < moneyAmount)
        return -428; // Insufficient balance
    
    int dateResult = validDateForTransaction(day, month, year, account);
    if (dateResult != 1)
        return dateResult; // Invalid date
    
    Date transactionDate = createDate((short)atoi(day), (short)atoi(month), (short)atoi(year));
    
    Transaction* newTransaction = createTransaction(moneyAmount, "main", "transfer", receiverIBAN, "transfer", description, transactionDate);
    if (newTransaction == NULL)
        return -429; // Failed to create transaction
    
    int result = addTransactionForUser(account, newTransaction);
    if (result != 1) {
        destroyTransaction(newTransaction);
        free(newTransaction);
        return result;
    }
    
    setAccountBalance(account, getAccountBalance(account) - moneyAmount);
    
    return 1;
}

int paymentService(Account* account, const char* amount, const char* description, const char* day, const char* month, const char* year) {
    if (account == NULL)
        return -431; // Invalid account
    
    if (amount == NULL || strlen(amount) == 0)
        return -432; // Missing amount
    
    if (description == NULL)
        return -433; // Missing description
    
    if (strlen(description) > 99)
        return -434; // Description too long
    
    if (!stringOnlyWithDigitsExtended(amount))
        return -435; // Amount is not a number
    
    gdouble moneyAmount = g_ascii_strtod(amount, NULL);
    if (moneyAmount <= 0)
        return -436; // Invalid amount
    
    if (getAccountBalance(account) < moneyAmount)
        return -437; // Insufficient balance
    
    int dateResult = validDateForTransaction(day, month, year, account);
    if (dateResult != 1)
        return dateResult; // Invalid date
    
    Date transactionDate = createDate((short)atoi(day), (short)atoi(month), (short)atoi(year));
    
    Transaction* newTransaction = createTransaction(moneyAmount, "main", "payment", "", "payment", description, transactionDate);
    if (newTransaction == NULL)
        return -438; // Failed to create transaction
    
    int result = addTransactionForUser(account, newTransaction);
    if (result != 1) {
        destroyTransaction(newTransaction);
        free(newTransaction);
        return result;
    }
    
    setAccountBalance(account, getAccountBalance(account) - moneyAmount);
    
    return 1;
}