#include <stdio.h>
#include <string.h>
#include "contact.h"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"

// Function to load contacts from a file
// This function reads contacts from a CSV file and populates the address book
int load_contacts(AddressBook *addressBook)
{
    FILE *fp = fopen("contacts.csv", "r");
    if (fp == NULL) {
        perror(RED "Error opening file" RESET);
        return -1;
    }

    // Read number of contacts
    if (fscanf(fp, "#%d\n", &addressBook->contact_count) != 1) {
        printf(RED "Invalid CSV format.\n" RESET);
        fclose(fp);
        return -1;
    }

    // Read each contact — corrected format!
    for (int i = 0; i < addressBook->contact_count; i++) {
        fscanf(fp, " %[^,],%[^,],%[^\n]",
            addressBook->contact_details[i].Name,
            addressBook->contact_details[i].Mobile_number,
            addressBook->contact_details[i].Mail_ID);
    }

    fclose(fp);

    printf(GREEN "\nContacts loaded successfully!\n" RESET);
    return 0;
}


