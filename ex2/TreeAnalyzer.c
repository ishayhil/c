//
// Created by Ishay Hil on 16/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_ROW_SIZE 1024

typedef struct Node
{
    int nodeKey;
    struct Node *parentNode;
    struct Node **children;
    int childrenCnt;
} Node;

typedef struct Row
{
    char row[MAX_ROW_SIZE];
    int rowNumber;
} Row;

int validateFile(char *fileName) // validates file and get number of vertex
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        return -1; // bad file error
    }
    char currentRow[MAX_ROW_SIZE] = "";
    fscanf(file, "%s", currentRow);
    fclose(file);
    if (strcmp(currentRow, "") <= 0)
    {
        return -2; // if <= 0 then invalid n (should be natural number)
    }
    else
    {
        return atoi(currentRow);
    }
}

Row *parseFile(char *fileName, int treeSize)
{
    Row *rows = (Row *) malloc(sizeof(Row) * treeSize);
    FILE *file = fopen(fileName, "r");
    char firstRow[MAX_ROW_SIZE];

    fgets(firstRow, MAX_ROW_SIZE, file);
    for (int i = 0; i < treeSize; ++i)
    {
        fgets(rows[i].row, MAX_ROW_SIZE, file);
        rows[i].rowNumber = i;
    }
    fclose(file);
    return rows;
}

Node *generateNodes(int treeSize)
{
    Node *nodes = (Node *) malloc(sizeof(Node) * treeSize);
    for (int i = 0; i < treeSize; ++i)
    {
        nodes[i].nodeKey = i;
        nodes[i].parentNode = NULL;
        nodes[i].children = NULL;
        nodes[i].childrenCnt = 0;
    }
    return nodes;
}

int cntChildren(Row row)
{
    if (strcmp(row.row, "-") == 0)
    {
        return 0;
    }
    int childrenCnt = 0;
    char *ptr = strtok(row.row, " ");
    while (ptr != NULL)
    {
        childrenCnt++;
        ptr = strtok(NULL, " ");
    }
    return childrenCnt;
}

int validateTokenInt(char *token, int treeSize)
{
    for (int i = 0; i < (int) strlen(token); ++i)
    {
        if (!isdigit(token[i]) && token[i] != '\n' && token[i] != '\r') // /r and /n are valid chars
        {
            return -1;
        }
    }
    int n = atoi(token);
    return n < treeSize ? n : -1;
}

int handleRow(Row row, Node *nodes, int treeSize)
{
    if (strcmp(row.row, "\n") == 0 || strcmp(row.row, "\r\n") == 0)
    {
        return 0; // empty line invalid
    }

    if (strcmp(row.row, "-") == 0 || strcmp(row.row, "-\n") == 0 || strcmp(row.row, "-\r\n") == 0)
    {
        return 1; // leaf
    }

    int childrenCnt = cntChildren(row);
    nodes[row.rowNumber].childrenCnt = childrenCnt;

    Node **children = (Node **) malloc(childrenCnt * sizeof(Node *));
    int i = 0;

    char *ptr = strtok(row.row, " ");
    while (ptr != NULL)
    {
        int num = validateTokenInt(ptr, treeSize);
        if (num < 0 || nodes[num].parentNode != NULL || num == row.rowNumber)
        {
            return 0; // invalid token (size or not pos int) OR node already has parent (then not tree) OR node parent is itself
        }
        nodes[num].parentNode = &nodes[row.rowNumber]; // set parent node for child
        children[i] = &nodes[num]; // add child to current node
        ptr = strtok(NULL, " ");
        i++;
    }
    nodes[row.rowNumber].children = children;
    return 1;
}

void printTree(int treeSize, Node *nodes)
{
    for (int j = 0; j < treeSize; ++j)
    {
        int cnt = nodes[j].childrenCnt;
        printf("%d: ", nodes[j].nodeKey);
        for (int i = 0; i < cnt; ++i)
        {
            printf("%d ", nodes[j].children[i]->nodeKey);
        }
        puts("");
    }
}

Node *getRoot(Node *nodes, int treeSize)
{
    Node *root = NULL;
    int cnt = 0;
    for (int i = 0; i < treeSize; ++i)
    {
        if (nodes[i].parentNode == NULL)
        {
            root = &nodes[i];
            cnt++;
        }
    }
    return cnt == 1 ? root : NULL;
}

int initTree(Row *rows, Node *nodes, int treeSize)
{
    for (int i = 0; i < treeSize; ++i)
    {
        int code = handleRow(rows[i], nodes, treeSize);
        if (code != 1)
        {
            return 0; // bad file
        }
    }
    return 1;
}

int main()
{
    int treeSize = validateFile("/Users/ishayhil/huji/c/ex2/test.in");
    if (treeSize <= 0)
    {
        return 1; // invalid tree size
    }
    Row *rows = parseFile("/Users/ishayhil/huji/c/ex2/test.in", treeSize);
    Node *nodes = generateNodes(treeSize);

    int res = initTree(rows, nodes, treeSize);
    if (!res)
    {
        printf("%s", "Invalid input\n");
        return 2; // invalid input file
    }

    Node *root = getRoot(nodes, treeSize);
    if (root == NULL)
    {
        printf("%s", "Invalid input\n");
        return 2; // more than 1 root
    }
    printf("root: %d", root->nodeKey);

    return 0;
}