#include <stdio.h>
#include <string.h>
#include "contact.h"

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"

/* Check that name contains only alphabets A-Z/a-z and spaces */
int validate_name(const char *name)
{
    int len = strlen(name);
    if (len == 0)
    {
        printf(RED "Name cannot be empty.\n" RESET);
        return 0;
    }

    int nonspace = 0;
    for (int i = 0; i < len; i++)
    {
        char ch = name[i];

        if (ch != ' ')
            nonspace = 1;

        if (!((ch >= 'A' && ch <= 'Z') ||
              (ch >= 'a' && ch <= 'z') ||
              ch == ' ' || ch == '-' || ch == '\''))
        {
            printf(RED "Name contains invalid characters.\n" RESET);
            return 0;
        }
    }

    if (!nonspace)
    {
        printf(RED "Name cannot be only spaces.\n" RESET);
        return 0;
    }

    if (name[0] == '-' || name[0] == '\'' ||
        name[len - 1] == '-' || name[len - 1] == '\'')
    {
        printf(RED "Name cannot start or end with '-' or '''.\n" RESET);
        return 0;
    }

    return 1;
}

/* Must be exactly 10 digits and start with 6-9 */
int validate_mobile(const char *mobile)
{
    if (strlen(mobile) != 10)
    {
        printf(RED "Mobile Number must be exactly 10 digits.\n" RESET);
        return 0;
    }

    if (mobile[0] < '6' || mobile[0] > '9')
    {
        printf(RED "Mobile number must start with digits 6-9.\n" RESET);
        return 0;
    }

    for (int i = 0; i < 10; i++)
    {
        char ch = mobile[i];
        if (!(ch >= '0' && ch <= '9'))
        {
            printf(RED "Mobile number must contain only digits.\n" RESET);
            return 0;
        }
        if (ch == ' ')
        {
            printf("Mobile Number cannot start with space\n");
            return 0;
        }
        
    }
    return 1;
}

/* email validation (lowercase letters, digits, '_', '-', '.', '@') */
int validate_email(const char *email)
{
    int at_index = -1, dot_index = -1;
    int len = strlen(email);
    int dot_count = 0;

    /* Must start with a letter */
    if (!(email[0] >= 'a' && email[0] <= 'z'))
    {
        printf("\033[1;31mEmail must start with a lowercase letter.\033[0m\n");
        return 0;
    }

    /* Basic length check */
    if (len < 5)
    {
        printf("\033[1;31mEmail is too short (minimum 5 characters).\033[0m\n");
        return 0;
    }

    /* Validate characters */
    for (int i = 0; email[i] != '\0'; i++)
    {
        char c = email[i];

        if (!((c >= 'a' && c <= 'z') || 
              (c >= '0' && c <= '9') || 
              c == '@' || c == '.' || 
              c == '_' || c == '-'))
        {
            printf("\033[1;31mInvalid character '%c'. Allowed: a-z, 0-9, @ . _ -\033[0m\n", c);
            return 0;
        }

        /* No consecutive dots */
        if (c == '.' && email[i + 1] == '.')
        {
            printf("\033[1;31mEmail cannot contain consecutive dots '..'.\033[0m\n");
            return 0;
        }

        /* Track @ */
        if (c == '@')
        {
            if (at_index != -1)
            {
                printf("\033[1;31mEmail cannot contain more than one '@'.\033[0m\n");
                return 0;
            }
            at_index = i;
        }

        /* Track dot count */
        if (c == '.')
        {
            dot_index = i;
            dot_count++;
        }
    }

    /* Must contain @ */
    if (at_index == -1)
    {
        printf("\033[1;31mEmail must contain '@'.\033[0m\n");
        return 0;
    }

    /* No only digits before @ */
    int only_digits_before_at = 1;
    for (int i = 0; i < at_index; i++)
    {
        if (!(email[i] >= '0' && email[i] <= '9'))
        {
            only_digits_before_at = 0;
            break;
        }
    }
    if (only_digits_before_at)
    {
        printf("\033[1;31mPart before '@' cannot contain only numbers.\033[0m\n");
        return 0;
    }

    /* Dot must be after @ */
    if (dot_index < at_index)
    {
        printf("\033[1;31mA '.' must appear after '@'.\033[0m\n");
        return 0;
    }

    /* Only one dot allowed after @ */
    if (dot_count > 1)
    {
        printf("\033[1;31mEmail cannot contain more than one '.' (dot) in domain.\033[0m\n");
        return 0;
    }

    /* Cannot end with special characters */
    char last = email[len - 1];
    if (last == '.' || last == '-' || last == '_' || last == '@')
    {
        printf("\033[1;31mEmail cannot end with '%c'.\033[0m\n", last);
        return 0;
    }

    /* Domain must be at least 2 characters */
    int domain_length = len - at_index - 1;
    if (domain_length < 3) // must include at least "x.y"
    {
        printf("\033[1;31mDomain part is too short.\033[0m\n");
        return 0;
    }

    /* TLD must be at least 2 characters */
    if (len - dot_index - 1 < 2)
    {
        printf("\033[1;31mTop-level domain (after '.') must be at least 2 characters.\033[0m\n");
        return 0;
    }

    /* Domain cannot start or end with hyphen */
    if (email[at_index + 1] == '-' || email[dot_index - 1] == '-')
    {
        printf("\033[1;31mDomain cannot start or end with '-'.\033[0m\n");
        return 0;
    }

    return 1; // VALID EMAIL
}

/* Duplicate mobile check */
int validate_duplicate_mobile(AddressBook *addressbook, const char *mobile)
{
    for (int i = 0; i < addressbook->contact_count; i++)
    {
        if (strcmp(addressbook->contact_details[i].Mobile_number, mobile) == 0)
            return 0;   // duplicate found
    }
    return 1;   // OK
}

/* Duplicate email check */
int validate_duplicate_email(AddressBook *addressbook, const char *email)
{
    for (int i = 0; i < addressbook->contact_count; i++)
    {
        if (strcmp(addressbook->contact_details[i].Mail_ID, email) == 0)
            return 0;
    }
    return 1;
}
