#include <stdlib.h>
#include <string.h>
#include "domain.h"

Account* createAccount(float mainAccountBalance, const char* tag, const char* firstName,
                       const char* second_name, const char* password, const char* iban,
                       const char* phone_number, Date birthday) {
    if (tag == NULL || firstName == NULL || second_name == NULL || 
        password == NULL || iban == NULL || phone_number == NULL) {
        return NULL;
    }

    Account* account = (Account*)malloc(sizeof(Account));
    if (account == NULL) return NULL;

    account->mainAccountBalance = mainAccountBalance;
    account->tag = strdup(tag);
    account->firstName = strdup(firstName);
    account->secondName = strdup(second_name);
    account->password = strdup(password);
    account->iban = strdup(iban);
    account->phoneNumber = strdup(phone_number);
    account->birthday = birthday;

    if (account->tag == NULL || account->firstName == NULL || account->secondName == NULL ||
        account->password == NULL || account->iban == NULL || account->phoneNumber == NULL) {
        
        free(account->tag);
        free(account->firstName);
        free(account->secondName);
        free(account->password);
        free(account->iban);
        free(account->phoneNumber);
        free(account);
        return NULL;
    }

    account->transactionsCapacity = 512;
    account->affiliatesCapacity = 128;
    account->userAccountsCapacity = 8;

    account->affiliates = malloc(account->affiliatesCapacity * sizeof(Affiliate*));
    account->transactions = malloc(account->transactionsCapacity * sizeof(Transaction *));
    account->userAccounts = malloc(account->userAccountsCapacity * sizeof(UserAccounts *));

    if (account->affiliates == NULL || account->transactions == NULL || account->userAccounts == NULL) {
        free(account->affiliates);
        free(account->transactions);
        free(account->userAccounts);
        free(account->tag);
        free(account->firstName);
        free(account->secondName);
        free(account->password);
        free(account->iban);
        free(account->phoneNumber);
        free(account);
        return NULL;
    }

    account->transactionsNumber = 0;
    account->affiliatesNumber = 0;
    account->userAccountsNumber = 0;

    return account;
}

void destroyAccount(Account* account) {
    if (account == NULL) return;

    free(account->tag);
    free(account->firstName);
    free(account->secondName);
    free(account->password);
    free(account->iban);
    free(account->phoneNumber);

    for (int i = 0; i < account->affiliatesNumber; i++) {
        destroyAffiliates(account->affiliates[i]);
    }
    free(account->affiliates);

    for (int i = 0; i < account->transactionsNumber; i++) {
        destroyTransaction(account->transactions[i]);
    }
    free(account->transactions);

    for (int i = 0; i < account->userAccountsNumber; i++) {
        destroyUserAccount(account->userAccounts[i]);
    }
    free(account->userAccounts);

    free(account);
}


// Getters
float getAccountBalance(const Account* account) {
    if (account == NULL) return 0.0f;
    return account->mainAccountBalance;
}

const char* getAccountTag(const Account* account) {
    if (account == NULL) return NULL;
    return account->tag;
}

const char* getAccountFirstName(const Account* account) {
    if (account == NULL) return NULL;
    return account->firstName;
}

const char* getAccountSecondName(const Account* account) {
    if (account == NULL) return NULL;
    return account->secondName;
}

const char* getAccountPassword(const Account* account) {
    if (account == NULL) return NULL;
    return account->password;
}

const char* getAccountIban(const Account* account) {
    if (account == NULL) return NULL;
    return account->iban;
}

const char* getAccountPhoneNumber(const Account* account) {
    if (account == NULL) return NULL;
    return account->phoneNumber;
}

Date getAccountBirthday(const Account* account) {
    Date emptyDate = {0, 0, 0};
    if (account == NULL) return emptyDate;
    return account->birthday;
}

int getAccountTransactionsNumber(const Account* account) {
    if (account == NULL) return 0;
    return account->transactionsNumber;
}

int getAccountAffiliatesNumber(const Account* account) {
    if (account == NULL) return 0;
    return account->affiliatesNumber;
}

int getAccountUserAccountsNumber(const Account* account) {
    if (account == NULL) return 0;
    return account->userAccountsNumber;
}

int getAccountTransactionsCapacity(const Account* account) {
    if (account == NULL) return 0;
    return account->transactionsCapacity;
}

int getAccountAffiliatesCapacity(const Account* account) {
    if (account == NULL) return 0;
    return account->affiliatesCapacity;
}

int getAccountUserAccountsCapacity(const Account* account) {
    if (account == NULL) return 0;
    return account->userAccountsCapacity;
}


// Setters
void setAccountBalance(Account* account, float balance) {
    if (account == NULL) return;
    account->mainAccountBalance = balance;
}

void setAccountTag(Account* account, const char* tag) {
    if (account == NULL || tag == NULL) return;
    char* newTag = strdup(tag);
    if (newTag == NULL) return; // strdup failed, keep old value
    free(account->tag);
    account->tag = newTag;
}

void setAccountFirstName(Account* account, const char* firstName) {
    if (account == NULL || firstName == NULL) return;
    char* newFirstName = strdup(firstName);
    if (newFirstName == NULL) return;
    free(account->firstName);
    account->firstName = newFirstName;
}

void setAccountSecondName(Account* account, const char* second_name) {
    if (account == NULL || second_name == NULL) return;
    char* newSecondName = strdup(second_name);
    if (newSecondName == NULL) return;
    free(account->secondName);
    account->secondName = newSecondName;
}

void setAccountPassword(Account* account, const char* password) {
    if (account == NULL || password == NULL) return;
    char* newPassword = strdup(password);
    if (newPassword == NULL) return;
    free(account->password);
    account->password = newPassword;
}

void setAccountIban(Account* account, const char* iban) {
    if (account == NULL || iban == NULL) return;
    char* newIban = strdup(iban);
    if (newIban == NULL) return;
    free(account->iban);
    account->iban = newIban;
}

void setAccountPhoneNumber(Account* account, const char* phone_number) {
    if (account == NULL || phone_number == NULL) return;
    char* newPhoneNumber = strdup(phone_number);
    if (newPhoneNumber == NULL) return;
    free(account->phoneNumber);
    account->phoneNumber = newPhoneNumber;
}

void setAccountBirthday(Account* account, Date birthday) {
    if (account == NULL) return;
    account->birthday = birthday;
}
