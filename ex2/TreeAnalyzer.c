//
// Created by Ishay Hil on 16/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    int nodeKey;
    struct Node *parentNode;
//    Node children[1024];
} Node;

typedef struct Row
{
    char row[1024];
    int rowNumber;
} Row;

int validateFile(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        return -1;
    }
    char currentRow[1024] = "";
    fscanf(file, "%s", currentRow);
    fclose(file);
    if (strcmp(currentRow, "") == 0)
    {
        return -2;
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
    char firstRow[1024];

    fgets(firstRow, 1024, file);
    for (int i = 0; i < treeSize; ++i)
    {
        fgets(rows[i].row, 1024, file);
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
    }
    return nodes;
}

void handleRow(Row row, Node *nodes)
{
    if (strcmp(row.row, "-") == 0)
    {
        return;
    }
    char *ptr = strtok(row.row, " ");
    while (ptr != NULL)
    {
        int num = atoi(ptr);
        nodes[num].parentNode = &nodes[row.rowNumber];
        ptr = strtok(NULL, " ");
    }
}

int main()
{
    int res = validateFile("/Users/ishayhil/huji/c/ex2/test.in");
    if (res <= 0)
    {
        return 1;
    }
    Row *rows = parseFile("/Users/ishayhil/huji/c/ex2/test.in", res);
    Node *nodes = generateNodes(res);

    for (int i = 0; i < res; ++i)
    {
        handleRow(rows[i], nodes);
    }

    printf("%d\n", nodes[1].parentNode->nodeKey);
    printf("%d\n", nodes[3].parentNode->nodeKey);

    return 0;

}