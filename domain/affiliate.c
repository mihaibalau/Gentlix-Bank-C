#include <stdlib.h>
#include <string.h>
#include "domain.h"

Affiliate* createAffiliates(const char* tag, const char* firstName, const char* secondName,
                             const char* iban, const char* activityDomain, const char* phone) {
    if (tag == NULL || firstName == NULL || secondName == NULL || 
        iban == NULL || activityDomain == NULL || phone == NULL) {
        return NULL;
    }

    Affiliate* affiliate = (Affiliate*)malloc(sizeof(Affiliate));
    if (affiliate == NULL) return NULL;

    affiliate->tag = strdup(tag);
    affiliate->firstName = strdup(firstName);
    affiliate->secondName = strdup(secondName);
    affiliate->iban = strdup(iban);
    affiliate->activityDomain = strdup(activityDomain);
    affiliate->phone = strdup(phone);

    // Check if any strdup failed
    if (affiliate->tag == NULL || affiliate->firstName == NULL || affiliate->secondName == NULL ||
        affiliate->iban == NULL || affiliate->activityDomain == NULL || affiliate->phone == NULL) {
        // Clean up what was allocated
        free(affiliate->tag);
        free(affiliate->firstName);
        free(affiliate->secondName);
        free(affiliate->iban);
        free(affiliate->activityDomain);
        free(affiliate->phone);
        free(affiliate);
        return NULL;
    }

    return affiliate;
}

void destroyAffiliates(Affiliate* affiliates) {
    if (affiliates == NULL) return;
    free(affiliates->tag);
    free(affiliates->firstName);
    free(affiliates->secondName);
    free(affiliates->iban);
    free(affiliates->activityDomain);
    free(affiliates->phone);
    free(affiliates);
}

const char* getAffiliatesTag(const Affiliate* affiliates) {
    if (affiliates == NULL) return NULL;
    return affiliates->tag;
}

const char* getAffiliatesFirstName(const Affiliate* affiliates) {
    if (affiliates == NULL) return NULL;
    return affiliates->firstName;
}

const char* getAffiliatesSecondName(const Affiliate* affiliates) {
    if (affiliates == NULL) return NULL;
    return affiliates->secondName;
}

const char* getAffiliatesIban(const Affiliate* affiliates) {
    if (affiliates == NULL) return NULL;
    return affiliates->iban;
}

const char* getAffiliatesActivityDomain(const Affiliate* affiliates) {
    if (affiliates == NULL) return NULL;
    return affiliates->activityDomain;
}

const char* getAffiliatesPhone(const Affiliate* affiliates) {
    if (affiliates == NULL) return NULL;
    return affiliates->phone;
}

void setAffiliatesTag(Affiliate* affiliates, const char* tag) {
    if (affiliates == NULL || tag == NULL) return;
    char* newTag = strdup(tag);
    if (newTag == NULL) return; // strdup failed, keep old value
    free(affiliates->tag);
    affiliates->tag = newTag;
}

void setAffiliatesFirstName(Affiliate* affiliates, const char* firstName) {
    if (affiliates == NULL || firstName == NULL) return;
    char* newFirstName = strdup(firstName);
    if (newFirstName == NULL) return;
    free(affiliates->firstName);
    affiliates->firstName = newFirstName;
}

void setAffiliatesSecondName(Affiliate* affiliates, const char* secondName) {
    if (affiliates == NULL || secondName == NULL) return;
    char* newSecondName = strdup(secondName);
    if (newSecondName == NULL) return;
    free(affiliates->secondName);
    affiliates->secondName = newSecondName;
}

void setAffiliatesIban(Affiliate* affiliates, const char* iban) {
    if (affiliates == NULL || iban == NULL) return;
    char* newIban = strdup(iban);
    if (newIban == NULL) return;
    free(affiliates->iban);
    affiliates->iban = newIban;
}

void setAffiliatesActivityDomain(Affiliate* affiliates, const char* activityDomain) {
    if (affiliates == NULL || activityDomain == NULL) return;
    char* newActivityDomain = strdup(activityDomain);
    if (newActivityDomain == NULL) return;
    free(affiliates->activityDomain);
    affiliates->activityDomain = newActivityDomain;
}

void setAffiliatesPhone(Affiliate* affiliates, const char* phone) {
    if (affiliates == NULL || phone == NULL) return;
    char* newPhone = strdup(phone);
    if (newPhone == NULL) return;
    free(affiliates->phone);
    affiliates->phone = newPhone;
}
