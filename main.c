/*
Name:Pruthvi Raj H
Date:8/12/2025
Sample Input: (Name:Pruthvi,Mobile:8618763448,Email:pruthvi200314@gmail.com)
Sample Output:Contact saved successfully
Adressbook is program to store contact details like name,mobile number,email.
Name should be only alphabetic characters and spaces.
Mobile number should be exactly 10 digits.
Email should follow some standard format.
*/
#include <stdio.h>
#include "contact.h"

int main()
{
    int option;
    AddressBook addressbook;
    addressbook.contact_count = 0;

    /* Load contacts at startup */
    load_contacts(&addressbook);

    while (1)
    {
        printf("\n--- Address Book Menu ---\n");
        printf("1. Add Contact\n");
        printf("2. Search Contact\n");
        printf("3. Edit Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Display Contacts\n");
        printf("6. Save Contacts\n");
        printf("7. Exit\n");
        printf("Enter option: ");

        scanf("%d", &option);
        clear_input();  // fix buffer issue

        switch (option)
        {
            case 1:
                create_contact(&addressbook);
                break;

            case 2:
                search_contacts(&addressbook);
                break;

            case 3:
                edit_contact(&addressbook);
                break;

            case 4:
                delete_contact(&addressbook);
                break;

            case 5:
                list_contacts(&addressbook);
                break;

            case 6:
                save_contacts(&addressbook);
                break;

            case 7:
                printf("Exiting Address Book...\n");
                return 0;

            default:
                printf("Invalid option! Try again.\n");
        }
    }

    return 0;
}
