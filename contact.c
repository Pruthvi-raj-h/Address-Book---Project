#include <stdio.h>
#include <string.h>
#include "contact.h"
#define RED     "\033[1;31m"
#define BLUE "\033[1;36m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"

void clear_input()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/*-----To Create a New Contact-----*/

int create_contact(AddressBook *addressbook)
{
    while (1)
    {
        printf("\n--- Add Contact ---\n");

        /* ======================= NAME ======================= */
        while (1)
        {
            printf("Enter Name: ");
            scanf(" %[^\n]", addressbook->contact_details[addressbook->contact_count].Name);

            if (!validate_name(addressbook->contact_details[addressbook->contact_count].Name))
            {
                printf(RED "Try again.\n" RESET);
                continue;
            }
            break;
        }

        /* ======================= MOBILE ======================= */
        while (1)
        {
            printf("Enter Mobile Number: ");
            scanf(" %[^\n]", addressbook->contact_details[addressbook->contact_count].Mobile_number);

            if (!validate_mobile(addressbook->contact_details[addressbook->contact_count].Mobile_number))
            {
                printf(RED "Try again.\n" RESET);
                continue;
            }

            if (!validate_duplicate_mobile(addressbook,
                addressbook->contact_details[addressbook->contact_count].Mobile_number))
            {
                printf(RED "Mobile number already exists! Enter a different one.\n" RESET);
                continue;
            }

            break;
        }

        /* ======================= EMAIL ======================= */
        while (1)
        {
            printf("Enter Mail ID: ");
            scanf(" %[^\n]", addressbook->contact_details[addressbook->contact_count].Mail_ID);

            if (!validate_email(addressbook->contact_details[addressbook->contact_count].Mail_ID))
            {
                printf(RED "Try again.\n" RESET);
                continue;
            }

            if (!validate_duplicate_email(addressbook,
                addressbook->contact_details[addressbook->contact_count].Mail_ID))
            {
                printf(RED "Email already exists! Enter a different one.\n" RESET);
                continue;
            }

            break;
        }

        /* ======================= SAVE CONTACT ======================= */
        addressbook->contact_count++;
        printf(GREEN "Contact added successfully!\n" RESET);

        int choice;
        printf("Add another contact? (1 = Yes, 0 = No): ");
        scanf("%d", &choice);

        if (choice == 0)
            break;
    }

    return 0;
}

/* ------Display Conatcts that are saved in Function-----*/

void list_contacts(AddressBook *addressbook)
{
    if (addressbook->contact_count == 0)
    {
        printf("No contacts available.\n");
        return;
    }

    sort_contacts(addressbook);  // AUTO SORT EVERY TIME BEFORE DISPLAY

    printf("\n------------ Contact List -----------\n");    

    for (int i = 0; i < addressbook->contact_count; i++)
    {
        printf(BLUE "Contact %d:\n" RESET, i + 1);
        printf(BLUE " Name   : %s\n" RESET, addressbook->contact_details[i].Name);
        printf(BLUE " Mobile : %s\n" RESET, addressbook->contact_details[i].Mobile_number);
        printf(BLUE" Mail   : %s\n" RESET, addressbook->contact_details[i].Mail_ID);
        printf("-------------------------------------\n");
    }
}

/* ------Search conatct Function For Only Search function------*/

int choose_contact(AddressBook *addressbook, int mode, const char *key)
{
    int indices[100];
    int match_count = find_all_contacts(addressbook, key, mode, indices);

    if (match_count == 0)
        return -1;   // <-- NO PRINTING HERE

    if (match_count == 1)
        return indices[0];

    printf("\nMultiple contacts found:\n");
    for (int i = 0; i < match_count; i++)
    {
        int idx = indices[i];
        printf("%d. %s | %s | %s\n",
               i + 1,
               addressbook->contact_details[idx].Name,
               addressbook->contact_details[idx].Mobile_number,
               addressbook->contact_details[idx].Mail_ID);
    }

    int choice;
    printf("Select contact number: ");
    scanf("%d", &choice);
    clear_input();

    if (choice < 1 || choice > match_count)
        return -1;

    return indices[choice - 1];
}

int search_contacts(AddressBook *addressbook)
{
    while (1)
    {
        int option;
        printf("\n--- Search Contact Menu ---\n");
        printf("1. Search by Name\n");
        printf("2. Search by Mobile\n");
        printf("3. Search by Email\n");
        printf("4. Exit Search Menu\n");
        printf("Enter option: ");
        scanf("%d", &option);
        clear_input();

        if (option == 4)
        {
            printf("Exiting Search Menu...\n");
            return 0;   // only here it exits
        }

        char key[50];
        printf("Enter value to search: ");
        scanf(" %[^\n]", key);

        int index = choose_contact(addressbook, option, key);

        if (index == -1)
        {
            printf(RED "No matching contact found.\n" RESET);
            continue;  // <-- Stay inside search menu
        }

        // If contact found show details
        printf(BLUE "\nContact Found:\n" RESET);
        printf(BLUE "Name   : %s\n" RESET, addressbook->contact_details[index].Name);
        printf(BLUE "Mobile : %s\n" RESET, addressbook->contact_details[index].Mobile_number);
        printf(BLUE "Email  : %s\n" RESET, addressbook->contact_details[index].Mail_ID);
    }
}

/*-----Searching the conatcts only for Edit/delete Conatct----*/

int find_all_contacts(AddressBook *addressbook, const char *key, int mode, int result[])
{
    int count = 0;

    for (int i = 0; i < addressbook->contact_count; i++)
    {
        if (mode == 1 && strcmp(addressbook->contact_details[i].Name, key) == 0)
            result[count++] = i;

        if (mode == 2 && strcmp(addressbook->contact_details[i].Mobile_number, key) == 0)
            result[count++] = i;

        if (mode == 3 && strcmp(addressbook->contact_details[i].Mail_ID, key) == 0)
            result[count++] = i;
    }

    return count; // number of matches found
}

/* ----Edit Function----*/
int edit_contact(AddressBook *addressbook)
{
    while (1)
    {
        int option;
        printf("\n--- Edit Contact Menu ---\n");
        printf("1. Search by Name\n");
        printf("2. Search by Mobile\n");
        printf("3. Search by Email\n");
        printf("4. Edit ALL Fields\n");
        printf("5. Exit Edit Menu\n");
        printf("Enter option: ");
        scanf("%d", &option);
        clear_input();

        if (option == 5)
        {
            printf("Exiting Edit Menu...\n");
            return 0;
        }

        int index = -1;

        /* ---- SEARCH FOR CONTACT FOR OPTIONS 1–3 ---- */
        if (option >= 1 && option <= 3)
        {
            char key[50];
            printf("Enter value to search: ");
            scanf(" %[^\n]", key);

            index = choose_contact(addressbook, option, key);

            if (index == -1)
            {
                printf(RED "No matching contact found.\n" RESET);
                continue;
            }
        }
        /* ---- EDIT ALL (FIRST SELECT CONTACT) ---- */
        else if (option == 4)
        {
            printf("\nSelect search mode for Edit ALL:\n");
            printf("1. Name\n");
            printf("2. Mobile\n");
            printf("3. Email\n");
            printf("Enter: ");

            int smode;
            scanf("%d", &smode);
            clear_input();

            if (smode < 1 || smode > 3)
            {
                printf(RED "Invalid search mode.\n" RESET);
                continue;
            }

            char key2[50];
            printf("Enter value to search: ");
            scanf(" %[^\n]", key2);

            index = choose_contact(addressbook, smode, key2);

            if (index == -1)
            {
                printf(RED "No matching contact found.\n" RESET);
                continue;
            }
        }
        else
        {
            printf(RED "Invalid option.\n" RESET);
            continue;
        }


        /* ======= ASK WHAT TO EDIT ======= */

        printf("\nWhat do you want to edit?\n");
        printf("1. Edit Name\n");
        printf("2. Edit Mobile Number\n");
        printf("3. Edit Email\n");
        printf("4. Edit ALL\n");
        printf("5. Cancel\n");

        int edit_opt;
        printf("Enter option: ");
        scanf("%d", &edit_opt);
        clear_input();

        if (edit_opt == 5)
        {
            printf(YELLOW "Edit cancelled.\n" RESET);
            continue;
        }

        /* ===================== EDIT NAME ===================== */
        if (edit_opt == 1 || edit_opt == 4)
        {
            while (1)
            {
                char newName[50];
                printf("Enter new Name: ");
                scanf(" %[^\n]", newName);

                if (strcmp(newName, addressbook->contact_details[index].Name) == 0)
                {
                    printf(YELLOW "New name cannot be same as old name.\n" RESET);
                    continue;
                }

                if (!validate_name(newName))
                {
                    printf(RED "Invalid name! Try again.\n" RESET);
                    continue;
                }

                strcpy(addressbook->contact_details[index].Name, newName);
                printf(GREEN "Name updated.\n" RESET);
                break;
            }
        }

        /* ===================== EDIT MOBILE ===================== */
        if (edit_opt == 2 || edit_opt == 4)
        {
            while (1)
            {
                char newMobile[20];
                printf("Enter new Mobile Number: ");
                scanf(" %[^\n]", newMobile);

                if (strcmp(newMobile, addressbook->contact_details[index].Mobile_number) == 0)
                {
                    printf(YELLOW "New mobile number cannot be same as old one.\n" RESET);
                    continue;
                }

                if (!validate_mobile(newMobile))
                {
                    printf(RED "Invalid mobile number! Try again.\n" RESET);
                    continue;
                }

                if (!validate_duplicate_mobile(addressbook, newMobile))
                {
                    printf(RED "This mobile number already exists!\n" RESET);
                    continue;
                }

                strcpy(addressbook->contact_details[index].Mobile_number, newMobile);
                printf(GREEN "Mobile updated.\n" RESET);
                break;
            }
        }

        /* ===================== EDIT EMAIL ===================== */
        if (edit_opt == 3 || edit_opt == 4)
        {
            while (1)
            {
                char newEmail[50];
                printf("Enter new Email: ");
                scanf(" %[^\n]", newEmail);

                if (strcmp(newEmail, addressbook->contact_details[index].Mail_ID) == 0)
                {
                    printf(YELLOW "New email cannot be same as old one.\n" RESET);
                    continue;
                }

                if (!validate_email(newEmail))
                {
                    printf(RED "Invalid email! Try again.\n" RESET);
                    continue;
                }

                if (!validate_duplicate_email(addressbook, newEmail))
                {
                    printf(RED "This email already exists!\n" RESET);
                    continue;
                }

                strcpy(addressbook->contact_details[index].Mail_ID, newEmail);
                printf(GREEN "Email updated.\n" RESET);
                break;
            }
        }

        if (edit_opt == 4)
            printf(GREEN "\nAll fields updated successfully!\n" RESET);
    }
}

/*-------Delete Contact Function------*/

int delete_contact(AddressBook *addressbook)
{
    while (1)
    {
        int option;
        printf(BLUE "\n--- Delete Contact Menu ---\n" RESET);
        printf("1. Delete by Name\n");
        printf("2. Delete by Mobile\n");
        printf("3. Delete by Email\n");
        printf("4. Exit Delete Menu\n");
        printf("Enter option: ");
        scanf("%d", &option);
        clear_input();

        if (option == 4)
        {
            printf("Exiting Delete Menu...\n");
            return 0;   // exit ONLY here
        }

        char key[50];
        printf("Enter value to search: ");
        scanf(" %[^\n]", key);

        // Use unified selector
        int index = choose_contact(addressbook, option, key);

        if (index == -1)
        {
            printf(RED "No matching contact found.\n" RESET);
            continue;   // <-- stay inside delete menu
        }

        // Show selected contact
        printf("\nSelected Contact:\n");
        printf("Name   : %s\n", addressbook->contact_details[index].Name);
        printf("Mobile : %s\n", addressbook->contact_details[index].Mobile_number);
        printf("Email  : %s\n", addressbook->contact_details[index].Mail_ID);

        // Confirm delete
        printf("\nAre you sure you want to delete this contact? (Y/N): ");
        char confirm;
        scanf(" %c", &confirm);

        if (confirm != 'Y' && confirm != 'y')
        {
            printf(YELLOW "Delete cancelled.\n" RESET);
            continue;   // back to delete menu
        }

        // Perform delete
        for (int i = index; i < addressbook->contact_count - 1; i++)
        {
            addressbook->contact_details[i] = addressbook->contact_details[i + 1];
        }

        addressbook->contact_count--;

        printf(GREEN "Contact deleted successfully!\n" RESET);
    }
}

/* ----Save Contact Function-----*/

int save_contacts(AddressBook *addressbook)
{
    sort_contacts(addressbook);   // Corrected

    FILE *fp = fopen("contacts.csv", "w");

    if (!fp)
    {
        printf(RED "Failed to save contacts.\n" RESET);
        return -1;
    }
    
    fprintf(fp, "#%d\n",addressbook->contact_count);

    for (int i = 0; i < addressbook->contact_count; i++)
    {
        fprintf(fp, "%s,%s,%s\n",
                addressbook->contact_details[i].Name,
                addressbook->contact_details[i].Mobile_number,
                addressbook->contact_details[i].Mail_ID);
    }

    fclose(fp);
    printf(GREEN "Contacts saved in CSV file successfully!\n" RESET);

    return 0;
}


/*Sorting function used to sort the Contacts*/

void sort_contacts(AddressBook *addressbook)
{
    if (addressbook->contact_count <= 1)
    {
        printf(YELLOW "Not enough contacts to sort.\n" RESET);
        return;
    }

    for (int i = 0; i < addressbook->contact_count - 1; i++)
    {
        for (int j = 0; j < addressbook->contact_count - i - 1; j++)
        {
            // Compare names (case-sensitive). Use strcasecmp for case-insensitive if needed.
            if (strcmp(addressbook->contact_details[j].Name,
                       addressbook->contact_details[j + 1].Name) > 0)
            {
                // Swap entire contact structures
                Contacts temp = addressbook->contact_details[j];
                addressbook->contact_details[j] = addressbook->contact_details[j + 1];
                addressbook->contact_details[j + 1] = temp;
            }
        }
    }

    printf(GREEN "Contacts sorted by Name successfully!\n" RESET);
}

