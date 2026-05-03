#ifndef CONTACT_H
#define CONTACT_H

/* ------------------ Structures ------------------ */

typedef struct Contact_data
{
    char Name[32];
    char Mobile_number[11];
    char Mail_ID[35];
} Contacts;

typedef struct AddressBook_Data
{
    Contacts contact_details[100];
    int contact_count;
} AddressBook;

/* ------------------ Helper Functions ------------------ */
void clear_input();
int choose_contact(AddressBook *addressbook, int mode, const char *key);

/* ------------------ File Operations ------------------ */
int load_contacts(AddressBook *addressbook);
int save_contacts(AddressBook *addressbook);

/* ------------------ CRUD Operations ------------------ */
int create_contact(AddressBook *addressbook);
void list_contacts(AddressBook *addressbook);

int search_contacts(AddressBook *addressbook);
int edit_contact(AddressBook *addressbook);
int delete_contact(AddressBook *addressbook);

void sort_contacts(AddressBook *addressbook);

/* ------------------ Search Utilities ------------------ */
int find_all_contacts(AddressBook *addressbook, const char *key, int mode, int result[]);

/* ------------------ Validation ------------------ */
int validate_name(const char *name);
int validate_mobile(const char *mobile);
int validate_email(const char *email);
int validate_duplicate_mobile(AddressBook *addressbook, const char *mobile);
int validate_duplicate_email(AddressBook *addressbook, const char *email);

#endif
