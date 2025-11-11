#include "repository.h"
#include <stdlib.h>
#include <string.h>

RepositoryFormat* createRepository(){

    int defaultCapacity = 20;

    RepositoryFormat* newRepository = malloc(sizeof(RepositoryFormat));
    if(newRepository == NULL)
        return NULL;

    newRepository->capacity = defaultCapacity;
    newRepository->numberOfElements = 0;

    newRepository->accounts = malloc(sizeof(Account*) * defaultCapacity);
    if(newRepository->accounts == NULL) {
        free(newRepository);
        return NULL;
    }

    return newRepository;
}

int destroyRepository(RepositoryFormat* receivedRepository){

    if(receivedRepository == NULL)
        return -21;

    for(int i=0; i<receivedRepository->numberOfElements; i++) {
        destroyAccount(receivedRepository->accounts[i]);
        free(receivedRepository->accounts[i]);
    }

    free(receivedRepository->accounts);
    free(receivedRepository);

    return 1;
}

int resizeRepository(RepositoryFormat* receivedRepository, int newCapacity){

    if(receivedRepository == NULL)
        return -31;

    if(newCapacity <= 0)
        return -32;

    if (newCapacity < receivedRepository->numberOfElements) return -33;

    Account** increasedSizeAccounts = realloc(receivedRepository->accounts, newCapacity * sizeof(Account*));

    if(increasedSizeAccounts == NULL)
        return -34;

    receivedRepository->accounts = increasedSizeAccounts;
    receivedRepository->capacity = newCapacity;

    return 1;
}

int addAccountToRepository(RepositoryFormat* receivedRepository, Account* newAccount) {

    if (receivedRepository == NULL)
        return -41;

    if (newAccount == NULL)
        return -42;

    if (receivedRepository->numberOfElements >= receivedRepository->capacity) {
        int newCapacity = receivedRepository->capacity * 2;
        Account** newAccounts = (Account**)realloc(receivedRepository->accounts, newCapacity * sizeof(Account*));

        if (newAccounts == NULL) {
            return -43;
        }

        receivedRepository->accounts = newAccounts;
        receivedRepository->capacity = newCapacity;
    }

    receivedRepository->accounts[receivedRepository->numberOfElements] = newAccount;
    receivedRepository->numberOfElements++;

    return 1;
}

int removeAccountFromRepository(RepositoryFormat* receivedRepository, const char* accountTag) {

    if (receivedRepository == NULL)
        return -51;

    if (accountTag == NULL)
        return -52;

    int indexToRemove = -1;

    for (int i = 0; i < receivedRepository->numberOfElements; i++) {
        if (receivedRepository->accounts[i] == NULL) continue;
        
        const char* accountTagValue = getAccountTag(receivedRepository->accounts[i]);
        if (accountTagValue != NULL && strcmp(accountTagValue, accountTag) == 0) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1)
        return -53;

    // destroyAccount already calls free(account) internally, so we don't need to free it again
    destroyAccount(receivedRepository->accounts[indexToRemove]);
    receivedRepository->accounts[indexToRemove] = NULL;  // Set to NULL for safety

    for (int i = indexToRemove; i < receivedRepository->numberOfElements - 1; i++) {
        receivedRepository->accounts[i] = receivedRepository->accounts[i + 1];
    }

    receivedRepository->accounts[receivedRepository->numberOfElements - 1] = NULL;  // Clear the last element
    receivedRepository->numberOfElements--;

    return 1;
}

Account* getAccountByTag(const RepositoryFormat* receivedRepository, const char* userTag) {

    if (receivedRepository == NULL)
        return NULL;

    if (userTag == NULL)
        return NULL;

    for (int i = 0; i < receivedRepository->numberOfElements; i++) {
        if (receivedRepository->accounts[i] == NULL) continue;
        
        const char* accountTag = getAccountTag(receivedRepository->accounts[i]);
        if (accountTag != NULL && strcmp(accountTag, userTag) == 0) {
            return receivedRepository->accounts[i];
        }
    }

    return NULL;
}

int updateAccountDetails(RepositoryFormat* receivedRepository, const char* userTag,
                  float newBalance, const char* newFirstName, const char* newSecondName,
                  const char* newPassword, const char* newPhoneNumber) {

    if (receivedRepository == NULL)
        return -61;

    if (userTag == NULL)
        return -62;

    Account* userAccount = getAccountByTag(receivedRepository, userTag);

    if (userAccount == NULL) {
        return -63;
    }

    if (newBalance >= 0) setAccountBalance(userAccount, newBalance);
    if (newFirstName != NULL) setAccountFirstName(userAccount, newFirstName);
    if (newSecondName != NULL) setAccountSecondName(userAccount, newSecondName);
    if (newPassword != NULL) setAccountPassword(userAccount, newPassword);
    if (newPhoneNumber != NULL) setAccountPhoneNumber(userAccount, newPhoneNumber);

    return 1;
}

int getRepositorySize(const RepositoryFormat* receivedRepository) {

    if (receivedRepository == NULL) {
        return -1;
    }

    return receivedRepository->numberOfElements;
}

int isRepositoryFull(const RepositoryFormat* receivedRepository) {

    if (receivedRepository == NULL) {
        return -1;
    }

    return (receivedRepository->numberOfElements >= receivedRepository->capacity) ? 1 : 0;
}

int accountTagUsedRepo(const RepositoryFormat* receivedRepository, const char *checked_tag){
    if (receivedRepository == NULL || checked_tag == NULL)
        return 0;
    
    for(int index = 0; index < receivedRepository->numberOfElements; index++) {
        const char* accountTag = getAccountTag(receivedRepository->accounts[index]);
        if (accountTag != NULL && strcmp(checked_tag, accountTag) == 0)
            return 1;
    }
    return 0;
}

Account* loginRepository(RepositoryFormat* repository, const char* username, const char* password) {
    if (repository == NULL || username == NULL || password == NULL)
        return NULL;

    for (int i = 0; i < repository->numberOfElements; i++) {
        Account* account = repository->accounts[i];
        if (account == NULL) continue;
        
        const char* accountTag = getAccountTag(account);
        const char* accountPassword = getAccountPassword(account);
        
        if (accountTag != NULL && accountPassword != NULL &&
            strcmp(accountTag, username) == 0 && strcmp(accountPassword, password) == 0) {
            return account; // Credentials match
        }
    }

    return NULL; // No matching account found
}

int ibanUsedInRepository(const RepositoryFormat* repository, const char* iban) {
    if (repository == NULL || iban == NULL)
        return 0;

    for (int i = 0; i < repository->numberOfElements; i++) {
        if (repository->accounts[i] == NULL) continue;
        
        const char* accountIban = getAccountIban(repository->accounts[i]);
        if (accountIban != NULL && strcmp(accountIban, iban) == 0) {
            return 1;
        }
    }

    return 0;
}

int getRepositoryCapacity(const RepositoryFormat* receivedRepository) {
    if (receivedRepository == NULL)
        return -1;
    return receivedRepository->capacity;
}

Account* getAccountByIndex(const RepositoryFormat* receivedRepository, int index) {
    if (receivedRepository == NULL)
        return NULL;
    
    if (index < 0 || index >= receivedRepository->numberOfElements)
        return NULL;
    
    return receivedRepository->accounts[index];
}

Account* findAccountByIban(const RepositoryFormat* receivedRepository, const char* iban) {
    if (receivedRepository == NULL || iban == NULL)
        return NULL;

    for (int i = 0; i < receivedRepository->numberOfElements; i++) {
        if (receivedRepository->accounts[i] == NULL) continue;
        
        const char* accountIban = getAccountIban(receivedRepository->accounts[i]);
        if (accountIban != NULL && strcmp(accountIban, iban) == 0) {
            return receivedRepository->accounts[i];
        }
    }

    return NULL;
}

int clearRepository(RepositoryFormat* receivedRepository) {
    if (receivedRepository == NULL)
        return -1;

    // Destroy all accounts
    for (int i = 0; i < receivedRepository->numberOfElements; i++) {
        if (receivedRepository->accounts[i] != NULL) {
            destroyAccount(receivedRepository->accounts[i]);
            free(receivedRepository->accounts[i]);
        }
    }

    receivedRepository->numberOfElements = 0;
    return 1;
}



