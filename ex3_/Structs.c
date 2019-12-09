//
// Created by Ishay Hil on 07/12/2019.
//

#include "Structs.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define LESS (-1)
#define EQUAL (0)
#define GREATER (1)
#define SUCCESS (1)
#define FAILURE (0)

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

double calculateNorm(const void *vector)
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
        v2->vector = v1->vector;
        v2->len = v1->len;
        return SUCCESS;
    }
    return FAILURE;
}

Vector *findMaxNormVectorInTree(RBTree *tree) // needs to free the vector outside!
{
    Vector *maxVector = (Vector *) malloc(sizeof(Vector));
    maxVector->len = 0;
    maxVector->vector = NULL;

    forEachRBTree(tree, copyIfNormIsLarger, maxVector);
    return maxVector->len != 0 ? maxVector : NULL;
}

void printVector(Vector *v)
{
    printf("%d %d %d %d",
           (int) v->vector[0],
           (int) v->vector[1],
           (int) v->vector[2],
           (int) v->vector[3]
    );
}

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

int stringCompare(const void *a, const void *b)
{
    char *x = (char *) a;
    char *y = (char *) b;
    return strcmp(x, y);
}

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

void freeString(void *s)
{
    if (s == NULL)
    {
        return;
    }
    free(s);
}
