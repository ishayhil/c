//
// Created by Ishay Hil on 07/12/2019.
//

#include "Structs.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define LESS (-1)
#define EQUAL (0)
#define GREATER (1)
#define SUCCESS (1)
#define FAILURE (0)

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector *v1 = (Vector *) a;
    Vector *v2 = (Vector *) b;

    int len = (int) fmin(v1->len, v2->len);
    for (int i = 0; i < len; ++i)
    {
        if (v1->vector[i] > v2->vector[i])
        {
            return GREATER;
        }
        else if (v1->vector[i] < v2->vector[i])
        {
            return LESS;
        }
    }

    if (v2->len > v1->len)
    {
        return LESS;
    }
    else if (v2->len < v1->len)
    {
        return GREATER;
    }
    return EQUAL;
}

/**
 * @param vector
 * @return the first norm of the given vector
 */
static double calculateNorm(const void *vector)
{
    Vector *v = (Vector *) vector;
    if (v->len == 0)
    {
        return LESS;
    }
    double norm = 0;
    for (int i = 0; i < v->len; ++i)
    {
        norm += v->vector[i] * v->vector[i];

    }
    return norm;
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *vector, void *maxVector)
{
    if (vector == NULL || maxVector == NULL)
    {
        return FAILURE;
    }
    const Vector *v1 = (Vector *) vector;
    Vector *v2 = (Vector *) maxVector;

    if (calculateNorm(v1) > calculateNorm(v2))
    {
        free(v2->vector);
        double* v2V = (double*) malloc(sizeof(double) * v1->len);
        for (int i=0; i<v1->len; i++) {
            v2V[i] = v1->vector[i];
        }
        v2->vector = v2V;
        v2->len = v1->len;
        return SUCCESS;
    }
    return FAILURE;
}

/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree) // needs to free the vector outside!
{
    Vector *maxVector = (Vector *) malloc(sizeof(Vector));
    maxVector->len = 0;
    maxVector->vector = NULL;

    forEachRBTree(tree, copyIfNormIsLarger, maxVector);
    return maxVector->len != 0 ? maxVector : NULL;
}


//void printVector(Vector *v)
//{
//    printf("%d %d %d %d",
//           (int) v->vector[0],
//           (int) v->vector[1],
//           (int) v->vector[2],
//           (int) v->vector[3]
//    );
//}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
void freeVector(void *vector)
{
    if (vector == NULL)
    {
        return;
    }
    Vector *v = (Vector *) vector;
    free(v->vector);
    v->vector = NULL;
    free(vector);
}

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    char *x = (char *) a;
    char *y = (char *) b;
    return strcmp(x, y);
}

/**
 * ForEach function that concatenates the given word to pConcatenated. pConcatenated is already allocated with
 * enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    if (word == NULL || pConcatenated == NULL)
    {
        return EQUAL;
    }
    char *dest = (char *) pConcatenated;
    char *from = (char *) word;
    char *p = strcat(dest, from);
    if (strcmp(dest, "") != 0)
    {
        p = strcat(dest, "\n");
    }
    if (p == NULL)
    {
        return EQUAL;
    }
    return GREATER;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    if (s == NULL)
    {
        return;
    }
    free(s);
}
