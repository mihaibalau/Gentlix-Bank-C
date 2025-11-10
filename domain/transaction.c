#include <stdlib.h>
#include <string.h>
#include "domain.h"

Transaction* createTransaction(float amount, const char* userAccount, const char* type, const char* receiver_iban,
                               const char* category, const char* description, Date date) {
    if (userAccount == NULL || type == NULL || receiver_iban == NULL || 
        category == NULL || description == NULL) {
        return NULL;
    }

    Transaction* transaction = (Transaction*)malloc(sizeof(Transaction));
    if (transaction == NULL) return NULL;

    transaction->amount = amount;
    transaction->userAccount = strdup(userAccount);
    transaction->type = strdup(type);
    transaction->receiverIBAN = strdup(receiver_iban);
    transaction->category = strdup(category);
    transaction->description = strdup(description);
    transaction->date = date;


    if (transaction->userAccount == NULL || transaction->type == NULL || 
        transaction->receiverIBAN == NULL || transaction->category == NULL || 
        transaction->description == NULL) {

        free(transaction->userAccount);
        free(transaction->type);
        free(transaction->receiverIBAN);
        free(transaction->category);
        free(transaction->description);
        free(transaction);
        return NULL;
    }

    return transaction;
}

void destroyTransaction(Transaction* transaction) {
    if (transaction == NULL) return;
    free(transaction->userAccount);
    free(transaction->type);
    free(transaction->receiverIBAN);
    free(transaction->category);
    free(transaction->description);
    free(transaction);
}

float getTransactionAmount(const Transaction* transaction) {
    if (transaction == NULL) return 0.0f;
    return transaction->amount;
}

const char* getTransactionUserAccount(const Transaction* transaction) {
    if (transaction == NULL) return NULL;
    return transaction->userAccount;
}

const char* getTransactionType(const Transaction* transaction) {
    if (transaction == NULL) return NULL;
    return transaction->type;
}

const char* getTransactionReceiverIban(const Transaction* transaction) {
    if (transaction == NULL) return NULL;
    return transaction->receiverIBAN;
}

const char* getTransactionCategory(const Transaction* transaction) {
    if (transaction == NULL) return NULL;
    return transaction->category;
}

const char* getTransactionDescription(const Transaction* transaction) {
    if (transaction == NULL) return NULL;
    return transaction->description;
}

Date getTransactionDate(const Transaction* transaction) {
    Date emptyDate = {0, 0, 0};
    if (transaction == NULL) return emptyDate;
    return transaction->date;
}

void setTransactionAmount(Transaction* transaction, float amount) {
    if (transaction == NULL) return;
    transaction->amount = amount;
}

void setTransactionUserAccount(Transaction* transaction, const char* userAccount) {
    if (transaction == NULL || userAccount == NULL) return;
    char* newUserAccount = strdup(userAccount);
    if (newUserAccount == NULL) return; // strdup failed, keep old value
    free(transaction->userAccount);
    transaction->userAccount = newUserAccount;
}

void setTransactionType(Transaction* transaction, const char* type) {
    if (transaction == NULL || type == NULL) return;
    char* newType = strdup(type);
    if (newType == NULL) return;
    free(transaction->type);
    transaction->type = newType;
}

void setTransactionReceiverIban(Transaction* transaction, const char* receiver_iban) {
    if (transaction == NULL || receiver_iban == NULL) return;
    char* newReceiverIban = strdup(receiver_iban);
    if (newReceiverIban == NULL) return;
    free(transaction->receiverIBAN);
    transaction->receiverIBAN = newReceiverIban;
}

void setTransactionCategory(Transaction* transaction, const char* category) {
    if (transaction == NULL || category == NULL) return;
    char* newCategory = strdup(category);
    if (newCategory == NULL) return;
    free(transaction->category);
    transaction->category = newCategory;
}

void setTransactionDescription(Transaction* transaction, const char* description) {
    if (transaction == NULL || description == NULL) return;
    char* newDescription = strdup(description);
    if (newDescription == NULL) return;
    free(transaction->description);
    transaction->description = newDescription;
}

void setTransactionDate(Transaction* transaction, Date date) {
    if (transaction == NULL) return;
    transaction->date = date;
}
