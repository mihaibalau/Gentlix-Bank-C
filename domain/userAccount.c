#include <stdlib.h>
#include <string.h>
#include "domain.h"

UserAccounts* createUserAccount(float balance, const char* type) {
    if (type == NULL) return NULL;

    UserAccounts* account = (UserAccounts*)malloc(sizeof(UserAccounts));
    if (account == NULL) return NULL;

    account->accountBalance = balance;
    account->type = strdup(type);

    // Check if strdup failed
    if (account->type == NULL) {
        free(account);
        return NULL;
    }

    return account;
}

void destroyUserAccount(UserAccounts* account) {
    if (account == NULL) return;
    free(account->type);
    free(account);
}

float getUserAccountBalance(const UserAccounts* account) {
    if (account == NULL) return 0.0f;
    return account->accountBalance;
}

const char* getUserAccountType(const UserAccounts* account) {
    if (account == NULL) return NULL;
    return account->type;
}

void setUserAccountBalance(UserAccounts* account, float balance) {
    if (account == NULL) return;
    account->accountBalance = balance;
}

void setUserAccountType(UserAccounts* account, const char* type) {
    if (account == NULL || type == NULL) return;
    char* newType = strdup(type);
    if (newType == NULL) return; // strdup failed, keep old value
    free(account->type);
    account->type = newType;
}

