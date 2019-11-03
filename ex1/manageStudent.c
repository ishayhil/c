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

const int NUMBER_OF_TOKENS = 6;
const char DELIMITER[] = "\t";
const int MAX_TOKEN_SIZE = 50;

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

int isValidArgs(char *argv[]) {
    printf("%s", argv[0]);
    return 0;
}

Student parseStudent(char row[150]) {
    // "<ID> <name> <grade> <age> <country> <city>\n"
    int grade, age;
    char id[50], name[50], country[50], city[50], last[50] = "";

    sscanf(row, "%s\t%s\t%d\t%d\t%s\t%s\t%s", id, name, &grade, &age, country, city, last);

    Student s;
    s.grade = grade;
    s.age = age;
    for (int i = 0; i < MAX_TOKEN_SIZE; ++i) {
        s.id[i] = id[i];
        s.name[i] = name[i];
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
    if (strlen(s.id) != 10 | s.id[0] != '0')
        return 1;
    else if (!checkStr(s.name, 1))
        return 2;
    else if (s.age > 100 | s.age < 0)
        return 3;
    else if (s.age < 18 | s.age > 120)
        return 4;
    else if (!checkStr(s.country, 0))
        return 5;
    else if (!checkStr(s.city, 0))
        return 6;
    else
        return -1;
}

void getStudents() {
    char row[150] = "";

    while (strcmp(row, "q") != 0) {
        puts("Enter student info. To exit press q, then enter\n");
        gets(row);
        Student s = parseStudent(row);

        if (validateTokens(s) > 0) {
            puts("Error: ");
        }


        printf("id: %d, name: %s, grade: %d, age: %d, country: %s, city: %s, last: %s\n", s.id, s.name, s.grade, s.age,
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
    getStudents();
    // "<ID> <name> <grade> <age> <country> <city>\n"
    return 0;
}
// 313  ishay   80  20  il  tlv