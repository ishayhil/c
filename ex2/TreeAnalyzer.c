//
// Created by Ishay Hil on 16/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "queue.h"
#define MAX_ROW_SIZE 1024

typedef struct Node
{
    unsigned int nodeKey;
    struct Node *parentNode;
    struct Node **children;
    unsigned int childrenCnt;
    unsigned int dist;
    struct Node *prev;
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
        nodes[i].dist = INT8_MAX;
        nodes[i].prev = NULL;
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
//            printf("node: %d, parent: %d\n", nodes[row.rowNumber].nodeKey, nodes[num].parentNode->nodeKey);
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
        unsigned cnt = nodes[j].childrenCnt;
        printf("%d: ", nodes[j].nodeKey);
        for (unsigned int i = 0; i < cnt; ++i)
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

int countEdges(int treeSize)
{
    return treeSize - 1;
}

int heightOperator(int a, int b, int isMax)
{
    return isMax ? a > b : a < b;
}

int getTreeHeight(Node *root, int current, int isMax)
{
    if (root->childrenCnt == 0)
    {
        return current;
    }
    int bestForNode = isMax ? INT8_MIN : INT8_MAX;
    for (unsigned int i = 0; i < root->childrenCnt; ++i)
    {
        int this = getTreeHeight(root->children[i], current + 1, isMax);
        if (heightOperator(current, bestForNode, isMax))
        {
            bestForNode = this;
        }
    }
    return bestForNode;
}

unsigned int bfs(unsigned int firstNode, Node *nodes, int treeSize) // O(V + E)
{
    for (int i = 0; i < treeSize; ++i) // set all dist to inf
    {
        nodes[i].dist = INT8_MAX;
    }

    nodes[firstNode].dist = 0;
    Queue *queue = allocQueue();
    enqueue(queue, firstNode);
    while (!queueIsEmpty(queue))
    {
        Node *node = &nodes[dequeue(queue)];
        for (unsigned int i = 0; i < node->childrenCnt; ++i) // handle children nodes
        {
            if (node->children[i]->dist == INT8_MAX)
            {
                enqueue(queue, node->children[i]->nodeKey);
                node->children[i]->dist = node->dist + 1;
                node->children[i]->prev = node;
            }
        }
        if (node->parentNode != NULL) // handle parent node
        {
            if (node->parentNode->dist == INT8_MAX)
            {
                enqueue(queue, node->parentNode->nodeKey);
                node->parentNode->dist = node->dist + 1;
                node->parentNode->prev = node;
            }
        }
    }
    unsigned int max = 0; // extract the highest dest
    for (int i = 0; i < treeSize; ++i)
    {
        if (nodes[i].dist > max)
        {
            max = nodes[i].dist;
        }
    }
    return max;
}

unsigned int getTreeWidth(Node *nodes, int treeSize) // O(n^2)
{
    unsigned int max = 0;
    unsigned int res;
    for (int i = 0; i < treeSize; ++i)
    {
        if ((res = bfs(i, nodes, treeSize)) > max)
        {
            max = res;
        }
    }
    return max;
}

void reverseList(unsigned int *lst, unsigned int len)
{
    for (unsigned int i = 0; i < len / 2; ++i)
    {
        unsigned int temp = lst[i];
        lst[i] = lst[len - i - 1];
        lst[len - i - 1] = temp;
    }
}

void printPathBetweenNode(unsigned int v, unsigned int u, Node *nodes, int treeSize) // O(n)
{
    bfs(v, nodes, treeSize);
    unsigned int dist = nodes[u].dist;
    Node *current = &nodes[u];
    unsigned int *path = (unsigned int *) malloc((dist + 1) * sizeof(unsigned int));

    for (unsigned int i = 0; i < dist + 1; ++i)
    {
        path[i] = current->nodeKey;
        current = current->prev;
    }
    reverseList(path, dist + 1);
    for (unsigned int j = 0; j < dist + 1; ++j)
    {
        printf("%d ", path[j]);
    }

    free(path);
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
    int edges = countEdges(treeSize);
    printf("root: %d\n", root->nodeKey);
    printf("edges: %d\n", edges);
    int max = getTreeHeight(root, 0, 1);
    int min = getTreeHeight(root, 0, 0);
    printf("max height: %d\n", max);
    printf("min height: %d\n", min);
    unsigned int treeW = getTreeWidth(nodes, treeSize);
    printf("tree width: %d\n", treeW);

    printPathBetweenNode(1, 6, nodes, treeSize);

    return 0;
}