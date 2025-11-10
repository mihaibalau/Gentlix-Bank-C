#ifndef GENTLIX_BANK_SERVICES_H
#define GENTLIX_BANK_SERVICES_H

#include <gtk/gtk.h>
#include "../domain/domain.h"
#include "../repository/repository.h"

// Memory management functions
int addTransactionForUser(Account* account, Transaction* newTransaction);
Transaction* getLatestTransaction(const Account* account);
int addAffiliateToAccount(Account* account, Affiliate* newAffiliate);
int removeAffiliateFromAccount(Account* account, const char* affiliateTag);
int addNewUserAccount(Account* account, UserAccounts* newUserAccount);
int removeAnUserAccount(Account* account, const char* userAccountType);

// Check functions
short accountTagUsed(const RepositoryFormat* receivedRepository, const gchar *checkedTag);
short stringOnlyWithLetters(const gchar *checkedString);
short stringOnlyWithDigitsExtended(const gchar *checkedString);
short stringOnlyWithDigits(const gchar *checkedString);
short differentPassword(const gchar *password1, const gchar *password2);
short validDate(const gchar *day, const gchar *month, const gchar *year);
short validDateForTransaction(const gchar *day, const gchar *month, const gchar *year, const Account* account);
short availableAccountType(const char* accountType);

// Utility function
void generateRandomIBAN(char* iban);

// Services functions
int loginService(RepositoryFormat* repository, const char* username, const char* password, Account** loggedUser);
int createAccountService(RepositoryFormat* repository, const char* accountTag, const char* password, const char* passwordConfirm, const char* accountType, const char* phoneNumber,
                         const char* firstName, const char* secondName, const char* day, const char* month, const char* year, Account** loggedAccount);
int deleteAccountService(RepositoryFormat* repository, Account** loggedAccount);
int editAccountService(Account** loggedAccount, const char* currentPassword, const char* password, const char* passwordConfirm, const char* accountType, const char* phoneNumber,
                       const char* firstName, const char* secondName, const char* day, const char* month, const char* year);

// Transaction services
int depositService(Account* account, const char* amount, const char* description, const char* day, const char* month, const char* year);
int withdrawService(Account* account, const char* amount, const char* description, const char* day, const char* month, const char* year);
int transferService(Account* account, const char* amount, const char* description, const char* receiverIBAN, const char* day, const char* month, const char* year);
int paymentService(Account* account, const char* amount, const char* description, const char* day, const char* month, const char* year);

#endif
