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

#define NODEBUG

// ... rest of includes from the system
// ... all my includes

// -------------------------- const definitions -------------------------

/**
 * max sizes for each value (token)
 */
const int MAX_TOKEN_SIZE = 41;

/**
 * max rows that the user can input
 */
const int MAX_DB_SIZE = 5001;

/**
 * max characters per row
 */
const int MAX_ROW_SIZE = 151;

/**
 * the len of a valid id str
 */
const int idValidLen = 10;

/**
 * error message for invalid id input
 */
const char INVALID_ID[] = "ID is invalid. Must have 10 digits and can't start with 0";

/**
 * error message for invalid name input
 */
const char INVALID_NAME[] = "Name is invalid. Can only include upper/lower case english letters, '-' and spaces";

/**
 * error message for invalid grade input
 */
const char INVALID_GRADE[] = "Grade is invalid. Must be between 0 and 100 (included)";

/**
 * error message for invalid age input
 */
const char INVALID_AGE[] = "Age is invalid. Must be between 18 and 120 (included)";

/**
 * error message for invalid country input
 */
const char INVALID_COUNTRY[] = "Country is invalid. Can only include upper/lower case english letters and '-'";

/**
 * error message for invalid city input
 */
const char INVALID_CITY[] = "City is invalid. Can only include upper/lower case english letters and '-'";

/**
 * error message for too many tokens
 */
const char INVALID_TOO_MANY_TOKENS[] = "Too many student params! Should be 6 params delimited by tab.";

/**
 * usage msg for too many args
 */
const char INVALID_ARGS_CNT[] = "Should only be 1 args.";

/**
 * usage msg for too invalid single args
 */
const char INVALID_ARG[] = "Invalid arg! Valid args are: best, quick, merge.";

/**
 * best student msg
 */
const char BEST_STUDENT_HEAD[] = "best student info is:   ";

/**
 * best arg
 */
const char BEST[] = "best";

/**
 * quick arg
 */
const char QUICK[] = "quick";

/**
 * merge arg
 */
const char MERGE[] = "merge";

/**
 * usage head msg
 */
const char USAGE_HEAD[] = "USAGE: ";

/**
 * error head msg
 */
const char ERROR_MSG_HEAD[] = "Error: ";

/**
 * zero char
 */
const char ZERO_CHAR = '0';

/**
 * empty string
 */
const char EMPTY_STR[] = "";

/**
 * default int for init
 */
const int DEFAULT_INT = 20;

// ------------------------------ decelerations -----------------------------
/**
 * Student struct. Holds the matching data of each student. Last = token after the valid last token. For validation
 * purposes.
 */
typedef struct Student {
    char id[MAX_TOKEN_SIZE];
    char name[MAX_TOKEN_SIZE];
    char grade[MAX_TOKEN_SIZE];
    char age[MAX_TOKEN_SIZE];
    char country[MAX_TOKEN_SIZE];
    char city[MAX_TOKEN_SIZE];
    char last[MAX_TOKEN_SIZE];
} Student;

/**
 * acts as the huji database.
 */
Student db[MAX_DB_SIZE];


// ------------------------------ functions -----------------------------


Student initStudent(const char id[], const char name[], const char grade[], const char age[],
                    const char country[], const char city[],
                    const char last[]) {
    Student s;
    for (int i = 0; i < MAX_TOKEN_SIZE; ++i) {
        s.id[i] = id[i];
        s.name[i] = name[i];
        s.country[i] = country[i];
        s.city[i] = city[i];
        s.last[i] = last[i];
        s.grade[i] = grade[i];
        s.age[i] = age[i];
    }
    return s;

}

void initDB() {
    Student s = initStudent(EMPTY_STR, EMPTY_STR, EMPTY_STR, EMPTY_STR, EMPTY_STR, EMPTY_STR, EMPTY_STR);
    for (int i = 0; i < MAX_DB_SIZE; ++i) {
        db[i] = s;
    }
}

Student parseStudent(char row[MAX_ROW_SIZE]) {
    char id[MAX_TOKEN_SIZE], name[MAX_TOKEN_SIZE], country[MAX_TOKEN_SIZE], city[MAX_TOKEN_SIZE], grade[MAX_TOKEN_SIZE],
            age[MAX_TOKEN_SIZE];
    char last[MAX_TOKEN_SIZE] = "";

    sscanf(row, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\n]", id, name, grade, age, country, city, last);

    return initStudent(id, name, grade, age, country, city, last);
}

int checkCityCountryOrName(char str[], int isName) {
    int strLen = strlen(str);
    for (int i = 0; i < strLen; ++i) {
        if ((isspace(str[i]) && isName) | isalpha(str[i]) | str[i] == '-')
            continue;
        else
            return 0;
    }
    return 1;
}

int checkID(char id[]) {
    int idLen = strlen(id);
    if (idLen != idValidLen | id[0] == ZERO_CHAR) {
        return 0;
    }

    for (int i = 0; i < idLen; ++i) {
        if (!isdigit(id[i])) {
            return 0;
        }
    }
    return 1;
}


int checkNumeric(char numericStrRep[], int lowest, int highest) {
    int strLen = strlen(numericStrRep);
    for (int i = 0; i < strLen; ++i) {
        if (!isdigit(numericStrRep[i]))
            return 0;
    }
    int numeric;
    sscanf(numericStrRep, "%d", &numeric);
    return numeric >= lowest && numeric <= highest;
}

int validateTokens(Student s) {
    if (!checkID(s.id))
        return 1;
    else if (!checkCityCountryOrName(s.name, 1))
        return 2;
    else if (!checkNumeric(s.grade, 0, 100))
        return 3;
    else if (!checkNumeric(s.age, 18, 120))
        return 4;
    else if (!checkCityCountryOrName(s.country, 0))
        return 5;
    else if (!checkCityCountryOrName(s.city, 0))
        return 6;
    else if (strcmp(s.last, EMPTY_STR) != 0)
        return 7;
    else
        return -1;
}

void printStudent(Student s) {
    printf("%s\t%s\t%s\t%s\t%s\t%s\t\n", s.id, s.name, s.grade, s.age,
           s.country, s.city);
}


int generateStudents() {
    char row[MAX_ROW_SIZE];
    int cntLines = 0;
    int cntStudents = 0;
    int errorCode;
    initDB();

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
            printf(" in line %d.\n", cntLines);
        } else {
            db[cntStudents] = s;
            cntStudents++;
        }
        cntLines++;

    }
    return cntStudents;
}


void printBest(int studentCnt) {
    Student bestS = db[0];
    int age, grade;
    sscanf(db[0].grade, "%d", &grade);
    sscanf(db[0].age, "%d", &age);
    double currentBestRatio = grade * 1.0 / age;
    double currentRatio;
    for (int i = 1; i < studentCnt; ++i) {
        sscanf(db[i].grade, "%d", &grade);
        sscanf(db[i].age, "%d", &age);
        if (currentBestRatio < (currentRatio = grade * 1.0 / age)) {
            bestS = db[i];
            currentBestRatio = currentRatio;
        }
    }
    printf("%s", BEST_STUDENT_HEAD);
    printStudent(bestS);
}


void merge(int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // helper arrays
    Student L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = db[left + i];
    for (j = 0; j < n2; j++)
        R[j] = db[middle + 1 + j];

    // merge the back helpers into db
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i].grade <= R[j].grade) {
            db[k] = L[i];
            i++;
        } else {
            db[k] = R[j];
            j++;
        }
        k++;
    }

    // if any elements left, copy them.
    while (i < n1) {
        db[k] = L[i];
        k++;
        i++;
    }

    while (j < n2) {
        db[k] = R[j];
        k++;
        j++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        // recursive calls:
        mergeSort(l, m);
        mergeSort(m + 1, r);

        merge(l, m, r);
    }
}

// --------------------------------------

void swap(int i, int j) {
    Student temp = db[i];
    db[i] = db[j];
    db[j] = temp;
}


int partition(int low, int high) {
    Student pivot = db[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        // If current element is smaller than the pivot
        if (strcmp(db[j].name, pivot.name) < 0) { // db[j] < pivot)
            i++;    // increment index of smaller element
            swap(i, j);
        }
    }
    swap(i + 1, high);
    return (i + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(int low, int high) {
    if (low < high) {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(low, pi - 1);
        quickSort(pi + 1, high);
    }
}

void printDB(int studentCnt) {
    for (int i = 0; i < studentCnt; ++i) {
        printStudent(db[i]);
    }
}

/**
 * @brief The main function. Actually, does nothing here.
 * @return 0, to tell the system the execution ended without errors.
 */
int main(int argc, char *argv[]) {

#ifdef NODEBUG
    int studentCnt = generateStudents();
#else
    Student s1 = parseStudent("3135343981\tgshay\t8\t20\til\ttlv\t");
    Student s2 = parseStudent("3135343981\tfshay\t4\t20\til\ttlv\t");
    Student s3 = parseStudent("3135343981\tbhay\t23\t20\til\ttlv\t");
    Student s4 = parseStudent("3135343981\tashay\t1\t18\til\ttlv\t");

    db[0] = s1;
    db[1] = s2;
    db[2] = s3;
    db[3] = s4;

    int studentCnt = 4;
#endif

    if (argc != 2) {
        printf("%s%s", USAGE_HEAD, INVALID_ARGS_CNT);
        return 1;
    } else if (strcmp(argv[1], BEST) == 0) {
        printBest(studentCnt);
        return 0;
    } else if (strcmp(argv[1], MERGE) == 0) {
        mergeSort(0, studentCnt - 1);
        printDB(studentCnt);
        return 0;
    } else if (strcmp(argv[1], QUICK) == 0) {
        quickSort(0, studentCnt - 1);
        printDB(studentCnt);
        return 0;
    } else { // invalid argument!
        printf("%s%s", USAGE_HEAD, INVALID_ARG);
        return 1;
    }
}

// 3135343981   gshay isa	8	12	il	tlv