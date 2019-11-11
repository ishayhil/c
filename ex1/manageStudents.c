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

/**
 * max characters per row
 */
#define MAX_ROW_SIZE 151

/**
 * max rows that the user can input
 */
#define MAX_DB_SIZE 5001

/**
 * max sizes for each value (token)
 */
#define MAX_TOKEN_SIZE 41


// -------------------------- const definitions -------------------------

/**
 * the len of a valid id str
 */
const int ID_VALID_LEN = 10;

/**
 * stop key for finishing asking input from user
 */
const char STOP1[] = "q\n";

/**
 * stop key for finishing asking input from user - linux
 */
const char STOP2[] = "q\r\n";

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
const char BEST_STUDENT_HEAD[] = "best student info is: ";

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
const char ERROR_MSG_HEAD[] = "ERROR: ";

/**
 * zero char
 */
const char ZERO_CHAR = '0';

/**
 * empty string
 */
const char EMPTY_STR[] = "";

/**
 * msg to show the user each round
 */
const char USER_MSG[] = "Enter student info. To exit press q, then enter";

/**
 * exp pattern for sscanf
 */
const char SSCANF_EXP[] = "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\n]";

/**
 * "in line" for error message
 */
const char IN_LINE[] = "in line";

// ------------------------------ decelerations -----------------------------
/**
 * Student struct. Holds the matching data of each student. Last = token after the valid last token. For validation
 * purposes.
 */
typedef struct Student
{
    char id[MAX_TOKEN_SIZE];
    char name[MAX_TOKEN_SIZE];
    char gradeStr[MAX_TOKEN_SIZE];
    char ageStr[MAX_TOKEN_SIZE];
    char country[MAX_TOKEN_SIZE];
    char city[MAX_TOKEN_SIZE];
    char last[MAX_TOKEN_SIZE];
    int grade;
    int age;

} Student;

/**
 * acts as the huji database.
 */
Student db[MAX_DB_SIZE];


// ------------------------------ functions -----------------------------

/**
 * Generates a Student from a given student data.
 * @param id Student's id
 * @param name Student's name
 * @param grade Student's grade
 * @param age Student's age
 * @param country Student's country
 * @param city Student's city
 * @param last - last value of the row. Used to verify the number of tokens.
 * @return
 */
Student generateStudent(const char *id, const char *name, const char *grade, const char *age,
                        const char *country, const char *city,
                        const char *last)
{
    Student s;
    for (int i = 0; i < MAX_TOKEN_SIZE; ++i)
    {
        s.id[i] = id[i];
        s.name[i] = name[i];
        s.country[i] = country[i];
        s.city[i] = city[i];
        s.last[i] = last[i];
        s.gradeStr[i] = grade[i];
        s.ageStr[i] = age[i];
    }
    return s;
}

/**
 * Initiates the db with default Student.
 */
void initDB()
{
    Student s = generateStudent(EMPTY_STR, EMPTY_STR, EMPTY_STR, EMPTY_STR, EMPTY_STR, EMPTY_STR, EMPTY_STR);
    for (int i = 0; i < MAX_DB_SIZE; ++i)
    {
        db[i] = s;
    }
}

/**
 * Parsing the giving row to a Student sturct.
 * @param row the given student input
 * @return a Student struct.
 */
Student parseRow(char *row)
{
    char id[MAX_TOKEN_SIZE], name[MAX_TOKEN_SIZE], country[MAX_TOKEN_SIZE], city[MAX_TOKEN_SIZE], grade[MAX_TOKEN_SIZE],
        age[MAX_TOKEN_SIZE];
    char last[MAX_TOKEN_SIZE] = "";

    sscanf(row, SSCANF_EXP, id, name, grade, age, country, city, last);

    return generateStudent(id, name, grade, age, country, city, last);
}

/**
 * Validates the student's city/country/name.
 * @param str the Student's city/country/name
 * @param isName an integer == 0 if check city/country, otherwise validates name.
 * @return 1 if valid, 0 if not.
 */
int validateCityCountryOrName(char *str, int isName)
{
    int strLen = strlen(str);
    if (strLen == 0)
    {
        return 0;
    }
    for (int i = 0; i < strLen; ++i)
    {
        if ((isspace(str[i]) && isName) || isalpha(str[i]) || (str[i] == '-'))
        {
            continue;
        }
        else
        {
            puts(str);
            return 0;
        }
    }
    return 1;
}

/**
 * validates the Student's id.
 * @param id the Student's ID.
 * @return 1 if valid, 0 if not.
 */
int validateID(char *id)
{
    int idLen = strlen(id);
    if (idLen != ID_VALID_LEN || (id[0] == ZERO_CHAR))
    {
        return 0;
    }

    for (int i = 0; i < idLen; ++i)
    {
        if (!isdigit(id[i]))
        {
            return 0;
        }
    }
    return 1;
}

/**
 * validates the Student's age/grade.
 * @param numericStrRep  the string repr of the numeric value.
 * @param lowest    the lowest valid numeric val.
 * @param highest   the lowest valid numeric val.
 * @return 1 if valid, 0 if not.
 */
int checkNumeric(char numericStrRep[], int lowest, int highest)
{
    int strLen = strlen(numericStrRep);
    for (int i = 0; i < strLen; ++i)
    {
        if (!isdigit(numericStrRep[i]))
        {
            return 0;
        }
    }
    int numeric;
    sscanf(numericStrRep, "%d", &numeric);
    return numeric >= lowest && numeric <= highest;
}

/**
 * validates all the given Student's data members.
 * @param s the Student.
 * @return
 *  1 if id is invalid
 *  2 if name if invalid
 *  3 if grade is invalid
 *  4 if age is invalid
 *  5 if country is invalid
 *  6 if city is invalid
 *  7 if too many tokens
 *  -1 if all is legit.
 */
int validateTokens(Student s)
{
    if (!validateID(s.id))
    {
        return 1;
    }
    else if (!validateCityCountryOrName(s.name, 1))
    {
        return 2;
    }
    else if (!checkNumeric(s.gradeStr, 0, 100))
    {
        return 3;
    }
    else if (!checkNumeric(s.ageStr, 18, 120))
    {
        return 4;
    }
    else if (!validateCityCountryOrName(s.country, 0))
    {
        return 5;
    }
    else if (!validateCityCountryOrName(s.city, 0))
    {
        return 6;
    }
    else if (strcmp(s.last, EMPTY_STR) != 0)
    {
        return 7;
    }
    else
    {
        return -1;
    }
}

/**
 * print the Student's data.
 * @param s the Student to print.
 */
void printStudent(Student s)
{
    printf("%s\t%s\t%s\t%s\t%s\t%s\t\n", s.id, s.name, s.gradeStr, s.ageStr,
           s.country, s.city);
}

/**
 * generates the student database from the user's input.
 * @return Student count in db.
 */
int generateStudents()
{
    char row[MAX_ROW_SIZE];
    int cntLines = 0;
    int cntStudents = 0;
    int errorCode;
    initDB();

    while (1)
    {
        puts(USER_MSG);
        fgets(row, MAX_ROW_SIZE, stdin);

        if (strcmp(row, STOP1) == 0 || strcmp(row, STOP2) == 0)
        {
            break;
        }

        Student s = parseRow(row);
        if ((errorCode = validateTokens(s)) > 0)
        {
            printf("%s", ERROR_MSG_HEAD);
            if (errorCode == 1)
            {
                printf("%s\n", INVALID_ID);
            }
            else if (errorCode == 2)
            {
                printf("%s\n", INVALID_NAME);
            }
            else if (errorCode == 3)
            {
                printf("%s\n", INVALID_GRADE);
            }
            else if (errorCode == 4)
            {
                printf("%s\n", INVALID_AGE);
            }
            else if (errorCode == 5)
            {
                printf("%s\n", INVALID_COUNTRY);
            }
            else if (errorCode == 6)
            {
                printf("%s\n", INVALID_CITY);
            }
            else
            { // last problem
                printf("%s\n", INVALID_TOO_MANY_TOKENS);
            }
            printf("%s %d\n", IN_LINE, cntLines);
        }
        else
        {
            sscanf(s.gradeStr, "%d", &s.grade);
            sscanf(s.ageStr, "%d", &s.age);
            db[cntStudents] = s;
            cntStudents++;
        }
        cntLines++;

    }
    return cntStudents;
}

/**
 * Print's the best student based on his grade/age ratio.
 * @param studentCnt the number of students in db.
 */
void printBest(int studentCnt)
{
    if (studentCnt == 0)
    {
        return;
    }

    Student bestS = db[0];
    double currentBestRatio = db[0].grade * 1.0 / db[0].age;
    double currentRatio;
    for (int i = 1; i < studentCnt; ++i)
    {
        if (currentBestRatio < (currentRatio = db[i].grade * 1.0 / db[i].age))
        {
            bestS = db[i];
            currentBestRatio = currentRatio;
        }
    }
    printf("%s", BEST_STUDENT_HEAD);
    printStudent(bestS);
}

/**
 * merges two sorted arrays of Student[] into one sorted array. In this case, merges these two lists back into the db.
 * Sort key is the student's grade.
 * @param left index
 * @param middle index
 * @param right last
 */
void merge(int left, int middle, int right)
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // helper arrays
    Student L[MAX_ROW_SIZE], R[MAX_ROW_SIZE];

    for (i = 0; i < n1; i++)
    {
        L[i] = db[left + i];
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = db[middle + 1 + j];
    }

    // merge the array helpers into db
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
    {
        if (L[i].grade <= R[i].grade)
        {
            db[k] = L[i];
            i++;
        }
        else
        {
            db[k] = R[j];
            j++;
        }
        k++;
    }

    // if any elements left, copy them.
    while (i < n1)
    {
        db[k] = L[i];
        k++;
        i++;
    }

    while (j < n2)
    {
        db[k] = R[j];
        k++;
        j++;
    }
}

/**
 * Sorts an array of Student[] by their grade (ascending). Run time: O(nlogn)
 * @param l start of subarray
 * @param r end index of subarray
 */
void mergeSort(int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        // recursive calls:
        mergeSort(l, m);
        mergeSort(m + 1, r);

        merge(l, m, r);
    }
}

/**
 * Swap between to elements in the db array.
 * @param i first element ind
 * @param j second element ind
 */
void swap(int i, int j)
{
    Student temp = db[i];
    db[i] = db[j];
    db[j] = temp;
}

/**
 *  Chooses (deterministically) the db[high] element as the pivot. After that, moves all the students with 'less than'
 *  name to the left of the pivot, and all the student with 'high than' name to the right of the pivot.
 * @param low start ind of subarray
 * @param high end ind of subarray
 * @return the pivot ind after this movement.
 */
int partition(int low, int high)
{
    Student pivot = db[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (strcmp(db[j].name, pivot.name) < 0)
        { // db[j] < pivot)
            i++;    // increment index of smaller element
            swap(i, j);
        }
    }
    swap(i + 1, high);
    return (i + 1);
}

/**
 * Sorting a Student[] array by lexicographic name order using quick sort algorithm. Run time: O(n^2).
 * @param low start ind
 * @param high end ind
 */
void quickSort(int low, int high)
{
    if (low < high)
    {
        int pi = partition(low, high);

        // sort elements before the partition and after the partition
        quickSort(low, pi - 1);
        quickSort(pi + 1, high);
    }
}

/**
 * Prints all the students in the db by the order they are in the db.
 * @param studentCnt the number of students.
 */
void printDB(int studentCnt)
{
    for (int i = 0; i < studentCnt; ++i)
    {
        printStudent(db[i]);
    }
}

/**
 * @brief The main function. Invokes all the main logic.
 * @return 1 if fail (too many or invalid runtime args), 0 otherwise.
 * @param argc number of runtime args.
 * @param argv run time args.
 */
int main(int argc, char *argv[])
{

    int studentCnt = generateStudents();

    if (argc != 2)
    {
        printf("%s%s", USAGE_HEAD, INVALID_ARGS_CNT);
        return 1;
    }
    else if (strcmp(argv[1], BEST) == 0)
    {
        printBest(studentCnt);
        return 0;
    }
    else if (strcmp(argv[1], MERGE) == 0)
    {
        mergeSort(0, studentCnt - 1);
        printDB(studentCnt);
        return 0;
    }
    else if (strcmp(argv[1], QUICK) == 0)
    {
        quickSort(0, studentCnt - 1);
        printDB(studentCnt);
        return 0;
    }
    else
    { // invalid argument!
        printf("%s%s", USAGE_HEAD, INVALID_ARG);
        return 1;
    }
}