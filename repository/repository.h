#ifndef GENTLIX_BANK_REPOSITORY_H
#define GENTLIX_BANK_REPOSITORY_H

#include "../domain/domain.h"

typedef struct {
    int capacity, numberOfElements;
    Account** accounts;
} RepositoryFormat;

RepositoryFormat* createRepository();
int destroyRepository(RepositoryFormat* receivedRepository);
int resizeRepository(RepositoryFormat* receivedRepository, int newCapacity);
int addAccountToRepository(RepositoryFormat* receivedRepository, Account* newAccount);
int removeAccountFromRepository(RepositoryFormat* receivedRepository, const char* accountTag);
int updateAccountDetails(RepositoryFormat* receivedRepository, const char* userTag, float newBalance, const char* newFirstName, const char* newSecondName, const char* newPassword, const char* newPhoneNumber);
Account* getAccountByTag(const RepositoryFormat* receivedRepository, const char* userTag);
int getRepositorySize(const RepositoryFormat* receivedRepository);
int isRepositoryFull(const RepositoryFormat* receivedRepository);
int accountTagUsedRepo(const RepositoryFormat* receivedRepository, const char *checked_tag);
int ibanUsedInRepository(const RepositoryFormat* repository, const char* iban);
Account* loginRepository(RepositoryFormat* repository, const char* username, const char* password);

// Additional utility functions
int getRepositoryCapacity(const RepositoryFormat* receivedRepository);
Account* getAccountByIndex(const RepositoryFormat* receivedRepository, int index);
Account* findAccountByIban(const RepositoryFormat* receivedRepository, const char* iban);
int clearRepository(RepositoryFormat* receivedRepository);

#endif
