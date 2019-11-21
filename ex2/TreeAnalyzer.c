//
// Created by Ishay Hil on 16/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "queue.h"
#define MAX_ROW_SIZE 1024

/**
 * defines a Node inside the tree.
 */
typedef struct Node
{
    unsigned int nodeKey; // the node's number
    struct Node *parentNode; // a pointer to the parent node of the node, default is null
    struct Node **children; // a pointer array to the node's children
    unsigned int childrenCnt; // the number of children
    unsigned int dist; // the dist - for BFS. By default is INT8_MAX (inf)
    struct Node *prev; // previous node for BFS - recovering the path.
} Node;

const int METHOD_SUCCESS = 1;

typedef enum Errors
{
    MALLOC_FAIL = 3,
    INVALID_FILE,
    INVALID_TREE_SIZE,
    INVALID_ROW,
    INVALID_ARG
} ErrorCodes;

/**
 * defines a row in the input file.
 */
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
        return INVALID_FILE; // bad file error
    }
    char currentRow[MAX_ROW_SIZE] = "";
    fscanf(file, "%s", currentRow);
    fclose(file); // closes the file
    if (strcmp(currentRow, "") <= 0)
    {
        return INVALID_TREE_SIZE; // if <= 0 then invalid n (should be natural number)
    }
    else
    {
        return atoi(currentRow);
    }
}

Row *parseFile(char *fileName, int treeSize)
{
    Row *rows = (Row *) malloc(sizeof(Row) * treeSize);
    if (rows == NULL)
    {
        return NULL;
    }
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
    if (nodes == NULL)
    {
        return NULL;
    }
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
            return INVALID_ROW;
        }
    }
    int n = atoi(token);
    return n < treeSize ? n : INVALID_ROW;
}

int handleRow(Row row, Node *nodes, int treeSize)
{
    if (strcmp(row.row, "\n") == 0 || strcmp(row.row, "\r\n") == 0)
    {
        return INVALID_ROW; // empty line invalid
    }

    if (strcmp(row.row, "-") == 0 || strcmp(row.row, "-\n") == 0 || strcmp(row.row, "-\r\n") == 0)
    {
        return METHOD_SUCCESS; // leaf
    }

    int childrenCnt = cntChildren(row);
    nodes[row.rowNumber].childrenCnt = childrenCnt;

    Node **children = (Node **) malloc(childrenCnt * sizeof(Node *));
    if (children == NULL)
    {
        return MALLOC_FAIL; // malloc failed
    }
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
    return METHOD_SUCCESS;
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
        if (code == INVALID_ROW)
        {
            return INVALID_ROW; // bad file
        }
    }
    return METHOD_SUCCESS;
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
    for (int i = 0; i < treeSize; ++i) // set all dist to INT8_MAX (inf)
    {
        nodes[i].dist = INT8_MAX;
    }

    nodes[firstNode].dist = 0;
    Queue *queue = allocQueue();
    enqueue(queue, firstNode);
    while (!queueIsEmpty(queue)) // handle what's left in the queue
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
    unsigned int max = 0; // extract the highest dist
    for (int i = 0; i < treeSize; ++i)
    {
        if (nodes[i].dist > max)
        {
            max = nodes[i].dist;
        }
    }
    freeQueue(&queue);
    return max;
}

unsigned int getTreeDiameter(Node *nodes, int treeSize) // O(n^2)
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
    for (unsigned int i = 1; i < len / 2; ++i)
    {
        unsigned int temp = lst[i];
        lst[i] = lst[len - i - 1];
        lst[len - i - 1] = temp;
    }
}

unsigned int *getPathBetweenNodes(unsigned int v, unsigned int u, Node *nodes, int treeSize) // O(n)
{
    bfs(v, nodes, treeSize); // inits the dist between v and all nodes
    unsigned int dist = nodes[u].dist;
    Node *current = &nodes[u];
    unsigned int *path = (unsigned int *) malloc((dist + 2) * sizeof(unsigned int));
    if (path == NULL)
    {
        return NULL;
    }

    path[0] = dist + 2;
    for (unsigned int i = 1; i < path[0]; ++i)
    {
        path[i] = current->nodeKey;
        current = current->prev;
    }
//    reverseList(path, path[0]);
    return path;
}

int invalidInput()
{
    printf("Invalid input\n");
    return EXIT_FAILURE;
}

void freeTree(Node *nodes, int treeSize)
{
    for (int i = 0; i < treeSize; ++i)
    {
        free(nodes[i].children);
        nodes[i].children = NULL;
    }
    free(nodes);
}

int validateArgs(char *args[])
{
    for (int i = 2; i < 4; ++i)
    {
        for (int j = 0; j < (int) strlen(args[i]); ++j)
        {
            if (!isdigit(args[i][j]))
            {
                return INVALID_ARG;
            }
        }
    }
    return METHOD_SUCCESS;
}

void printPath(unsigned int *path)
{
    printf("%s", "Shortest Path Between 4 and 3: ");
    int len = (int) path[0];
    for (int i = len - 1; i > 0; --i)
    {
        if (i == 1)
        {
            printf("%d", path[i]);
        }
        else
        {
            printf("%d ", path[i]);
        }
    }
    free(path);
}

void printMessages(Node *nodes, int treeSize, Node *root, unsigned int *path)
{
    int vertices = treeSize;
    int edges = countEdges(vertices);
    int minBranch = getTreeHeight(root, 0, 0);
    int maxBranch = getTreeHeight(root, 0, 1);
    unsigned int diameter = getTreeDiameter(nodes, treeSize);

    printf("Root Vertex: %d\n", root->nodeKey);
    printf("Vertices Count: %d\n", vertices);
    printf("Edges Count: %d\n", edges);
    printf("Length of Minimal Branch: %d\n", minBranch);
    printf("Length of Maximal Branch: %d\n", maxBranch);
    printf("Diameter Length: %d\n", diameter);
    printPath(path);
}

int main(int argc, char *args[])
{

    if (argc != 4)
    {
        printf("%s", "Usage:  TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n");
        return EXIT_FAILURE;
    }
    else if (!validateArgs(args))
    {
        return invalidInput();
    }

    int treeSize = validateFile(args[1]);
    if (treeSize == INVALID_TREE_SIZE)
    {
        return invalidInput(); // invalid tree size
    }
    Row *rows = parseFile(args[1], treeSize);
    Node *nodes = generateNodes(treeSize);

    int res = initTree(rows, nodes, treeSize);
    if (res == INVALID_ROW)
    {
        return invalidInput();
    }

    Node *root = getRoot(nodes, treeSize);
    if (root == NULL)
    {
        return invalidInput();
    }

    // already verified to be legit:
    int v = atoi(args[2]);
    int u = atoi(args[3]);
    unsigned int *path = getPathBetweenNodes(v, u, nodes, treeSize);
    if (path == NULL)
    {
        return invalidInput();
    }
    printMessages(nodes, treeSize, root, path);

    freeTree(nodes, treeSize);

    return EXIT_SUCCESS;
}