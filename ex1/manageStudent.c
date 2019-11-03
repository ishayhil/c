/**
 * @file manageStudent.c
 * @author  Ishay Hilzenrat
 * @version 1.0
 * @date 3 Nov 2019
 *
 * @brief Giving you the ability to add students to the huji student DB, and query them in order to get the best
 * student or to sort the students based on grades to age ratio or name.
 *
 * @section LICENSE
 * This program is not a free software; it has been made with my tears.
 *
 * @section DESCRIPTION
 * Giving you the ability to add students to the huji student DB, and query them in order to get the best
 * student or to sort the students based on grades to age ratio or name.
 * Input: quick, merge, best
 * Process: Sorts the list in alphabetic order if input == quick, Sort the list in grades ascending order if
 * input == merge, returns the best student (highest grade/age) if input == best. Else returns error.
 * Output: in the above.
 */

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <string.h>
#include <ctype.h>
// ... rest of includes from the system
// ... all my includes

// -------------------------- const definitions -------------------------

const int MAX_TOKEN_SIZE = 50;

const char INVALID_ID[] = "ID is invalid. Must have 10 digits and can't start with 0.";
const char INVALID_NAME[] = "Name is invalid. Can only include upper/lower case english letters, '-' and spaces.";
const char INVALID_GRADE[] = "Grade is invalid. Must be between 0 and 100 (included).";
const char INVALID_AGE[] = "Age is invalid. Must be between 18 and 120 (included).";
const char INVALID_COUNTRY[] = "Country is invalid. Can only include upper/lower case english letters and '-'.";
const char INVALID_CITY[] = "City is invalid. Can only include upper/lower case english letters and '-'.";
const char INVALID_TOO_MANY_TOKENS[] = "Too many student params! Should be 6 params delimited by tab.";

const char ERROR_MSG_HEAD[] = "Error: ";
const char ZERO_STR[] = "0";
const char EMPTY_STR[2] = "";

// ------------------------------ functions -----------------------------

typedef struct Student {
    char id[50];
    char name[50];
    int grade;
    int age;
    char country[50];
    char city[50];
    char last[50];
} Student;

Student db[5000];

int isValidArgs(char *argv[]) {
    printf("%s", argv[0]);
    return 0;
}

Student parseStudent(char row[150]) {
    // "<ID> <name> <grade> <age> <country> <city>\n"
    int grade, age;
    char id[50], name[50], country[50], city[50];
    char last[2] = "";

    sscanf(row, "%s\t%s\t%d\t%d\t%s\t%s\t%s", id, name, &grade, &age, country, city, last);

    Student s;
    s.grade = grade;
    s.age = age;
    for (int i = 0; i < MAX_TOKEN_SIZE; ++i) {
        s.id[i] = id[i];
        s.name[i] = name[i];
        s.country[i] = country[i];
        s.city[i] = city[i];
        s.last[i] = last[i];
    }

    return s;
}

int checkStr(char name[50], int isName) {
    int nameLen = strlen(name);
    for (int i = 0; i < nameLen; ++i) {
        if ((isspace(name[i]) && isName) | isalpha(name[i]) | name[i] == '-')
            continue;
        else
            return 0;
    }
    return 1;
}

int validateTokens(Student s) {
    if (strlen(s.id) != 10 | s.id[0] == '0')
        return 1;
    else if (!checkStr(s.name, 1))
        return 2;
    else if (s.grade > 100 | s.grade < 1)
        return 3;
    else if (s.age < 18 | s.age > 120)
        return 4;
    else if (!checkStr(s.country, 0))
        return 5;
    else if (!checkStr(s.city, 0))
        return 6;
    else if (strcmp(s.last, EMPTY_STR) != 0)
        return 7;
    else
        return -1;
}

void generateStudents() {
    char row[150] = "";
    int cntLines = 0;
    int cntStudents = 0;
    int errorCode;

    while (1) {
        puts("Enter student info. To exit press q, then enter\n");
        gets(row);

        if (strcmp(row, "q") == 0)
            break;

        Student s = parseStudent(row);
        if ((errorCode = validateTokens(s)) > 0) {
            printf("%s", ERROR_MSG_HEAD);
            if (errorCode == 1)
                printf("%s", INVALID_ID);
            else if (errorCode == 2)
                printf("%s", INVALID_NAME);
            else if (errorCode == 3)
                printf("%s", INVALID_GRADE);
            else if (errorCode == 4)
                printf("%s", INVALID_AGE);
            else if (errorCode == 5)
                printf("%s", INVALID_COUNTRY);
            else if (errorCode == 6)
                printf("%s", INVALID_CITY);
            else // last problem
                printf("%s", INVALID_TOO_MANY_TOKENS);
            printf(" in line %d\n", cntLines);
        } else {
            db[cntStudents] = s;
            cntStudents++;
        }
        cntLines++;
        printf("id: %s, name: %s, grade: %d, age: %d, country: %s, city: %s, last: %s\n", s.id, s.name, s.grade, s.age,
               s.country, s.city, s.last);
    }
}


/**
 * @brief The main function. Actually, does nothing here.
 * @return 0, to tell the system the execution ended without errors.
 */
int main(int argc, char *argv[]) {
    int cnt = 0;
    char row[150];
    generateStudents();
    // "<ID> <name> <grade> <age> <country> <city>\n"
    printf("%s", db[0].city);

    return 0;
}
// 313  ishay   80  20  il  tlv