/**
 * @file TreeAnalyzer.c
 * @author  Ishay Hilzenrat
 * @version 1.0
 * @date 21 Nov 2019
 *
 * @brief this file represent a tree with k children ds. It can take a given file and 2 nodes, parse it and print some
 * very interesting data regarding the tree and the path from node1 to node 2.
 *
 * @section LICENSE
 * This program is not a free software; it has been made with my tears.
 *
 * @section DESCRIPTION
 * this program's input is 3 args: file path, node1, node2. The progrem will parse the file and build the matching tree
 * or will return an error exit coe (and print an error message) if the args/file is incorrect.
 * After building the k children tree, the program will print the root, number of nodes and edges, len of min/max branch,
 * tree diameter and the max path for node1 to node2. All in very efficient runtime complexity.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "queue.h"
#define MAX_ROW_SIZE 1025 // max row size (1024 chars).

/**
 * invalid input msg
 */
const char INVALID_INPUT[] = "Invalid input\r\n";

/**
 * invalid usage msg
 */
const char INVALID_USAGE[] = "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n";

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

/**
 * with methods of this class except for main method are success.
 */
const int METHOD_SUCCESS = 1;

/**
 * Error codes enum
 */
typedef enum Errors
{
    VALID_TOKEN_NOT_ADD = -11,
    MALLOC_FAIL,
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

/**
 * Validates that the fail is legit. If so, returns the int of the first line (nodes count).
 * @param fileName  the file path
 * @return error code if the file is not legit, the number of nodes if legit.
 */
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
        return (int) strtol(currentRow, NULL, 10);
    }
}

/**
 * Parses the file into list of type Row.
 * @param fileName the file path
 * @param treeSize the number of nodes
 * @return a pointer to the array of Rows.
 */
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

/**
 * initiates the nodes with default values. Needs to be freed after.
 * @param treeSize
 * @return a pointer to the node array.
 */
Node *initNodes(int treeSize)
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
        nodes[i].dist = INT_MAX;
        nodes[i].prev = NULL;
    }
    return nodes;
}

/**
 * @param row of type Row
 * @return the number of children for the node.
 */
int cntChildren(Row *row)
{

    char *temp = (char *) malloc(sizeof(char) * strlen(row->row));
    strcpy(temp, row->row);

    if (strcmp(temp, "-") == 0)
    {
        return 0;
    }
    int childrenCnt = 0;
    char *ptr = strtok(temp, " \n\t\r");
    while (ptr != NULL)
    {
        childrenCnt++;
        ptr = strtok(NULL, " \n\t\r");
    }
    free(temp);
    return childrenCnt;
}

/**
 * validates that the node number in the Row is valid. If so, returns the integer.
 * @param token the token
 * @param treeSize
 */
int validateTokenInt(char *token, int treeSize)
{
    if (strlen(token) == 0)
    {
        return INVALID_ROW;
    }

    for (int i = 0; i < (int) strlen(token); ++i)
    {
        if (!isdigit(token[i]) && token[i] != '\n' && token[i] != '\r') // /r and /n are valid chars
        {
            return INVALID_ROW;
        }
    }

    int n = (int) strtol(token, NULL, 10);
    return n < treeSize ? n : INVALID_ROW;
}

/**
 * For every row, which describes a node, updating the node's children that the node is their parent, and adding them
 * to the list of pointers to the children. Needs to be freed outside.
 * @param row the Row
 * @param nodes all nodes list
 * @param treeSize
 * @return success/failure code
 */
int handleRow(Row *row, Node *nodes, int treeSize)
{
    if (strcmp(row->row, "\n") == 0 || strcmp(row->row, "\r\n") == 0)
    {
        return INVALID_ROW; // empty line invalid
    }

    if (strcmp(row->row, "-") == 0 || strcmp(row->row, "-\n") == 0 || strcmp(row->row, "-\r\n") == 0)
    {
        return METHOD_SUCCESS; // leaf
    }

    int childrenCnt = cntChildren(row);
    nodes[row->rowNumber].childrenCnt = childrenCnt;

    Node **children = (Node **) malloc(childrenCnt * sizeof(Node *));
    if (children == NULL)
    {
        return MALLOC_FAIL; // malloc failed
    }

    int i = 0;
    char *temp = (char *) malloc(sizeof(char) * strlen(row->row));
    strcpy(temp, row->row);
    char *ptr = strtok(temp, " \n\t\r");

    while (ptr != NULL)
    {
        int num = validateTokenInt(ptr, treeSize);
        if (num == INVALID_ROW)
        {
            return INVALID_ROW;
        }
        else if (num == VALID_TOKEN_NOT_ADD)
        {
            return METHOD_SUCCESS;
        }

        if (num < 0 || nodes[num].parentNode != NULL || num == row->rowNumber)
        {
            return INVALID_ROW; // invalid token (size or not pos int) OR node already has parent (then not tree) OR node parent is itself
        }
        nodes[num].parentNode = &nodes[row->rowNumber]; // set parent node for child
        children[i] = &nodes[num]; // add child to current node
        ptr = strtok(NULL, " \n\t\r");
        i++;
    }
    nodes[row->rowNumber].children = children;
    free(temp);
    return METHOD_SUCCESS;
}

/**
 * @param nodes
 * @param treeSize
 * @return the root of the tree
 */
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

/**
 * Initiates the tree by handling each row of the file.
 * @param rows
 * @param nodes
 * @param treeSize
 * @return success code/failure code.
 */
int initTree(Row *rows, Node *nodes, int treeSize)
{
    for (int i = 0; i < treeSize; ++i)
    {
        int code = handleRow(&rows[i], nodes, treeSize);
        if (code == INVALID_ROW)
        {
            return INVALID_ROW; // bad file
        }
    }
    return METHOD_SUCCESS;
}

/**
 * @param treeSize
 * @return number of edges
 */
int countEdges(int treeSize)
{
    return treeSize - 1;
}

/**
 * performing a bfs on a given node.
 * @param theNode the given node
 * @param nodes
 * @param treeSize
 * @return the max distance from the given
 */
unsigned int bfs(unsigned int theNode, Node *nodes, int treeSize) // O(V + E)
{
    for (int i = 0; i < treeSize; ++i) // set all dist to INT8_MAX (inf)
    {
        nodes[i].dist = INT_MAX;
    }

    nodes[theNode].dist = 0;
    Queue *queue = allocQueue();
    enqueue(queue, theNode);
    while (!queueIsEmpty(queue)) // handle what's left in the queue
    {
        Node *node = &nodes[dequeue(queue)];
        for (unsigned int i = 0; i < node->childrenCnt; ++i) // handle children nodes
        {
            if (node->children[i]->dist == INT_MAX)
            {
                enqueue(queue, node->children[i]->nodeKey);
                node->children[i]->dist = node->dist + 1;
                node->children[i]->prev = node;
            }
        }
        if (node->parentNode != NULL) // handle parent theNode
        {
            if (node->parentNode->dist == INT_MAX)
            {
                enqueue(queue, node->parentNode->nodeKey);
                node->parentNode->dist = node->dist + 1;
                node->parentNode->prev = node;
            }
        }
    }
    unsigned int best = 0; // extract the highest dist
    for (int i = 0; i < treeSize; ++i)
    {
        if (nodes[i].dist > best)
        {
            best = nodes[i].dist;
        }
    }
    freeQueue(&queue);
    return best;
}

/**
 * @param rootKey
 * @param nodes
 * @param treeSize
 * @param isMax 1 if get max, 0 otherwise
 * @return the min/max tree height.
 */
unsigned int getTreeHeight(int rootKey, Node *nodes, int treeSize, int isMax)
{
    if (nodes[rootKey].childrenCnt == 0)
    {
        return 0;
    }
    unsigned max = bfs(rootKey, nodes, treeSize);
    if (isMax)
    {
        return max;
    }

    unsigned int min = INT_MAX;
    for (int i = 0; i < treeSize; ++i)
    {
        if (nodes[i].dist < min && nodes[i].childrenCnt == 0) // leaf and < min
        {
            min = nodes[i].dist;
        }
    }
    return min;
}

/**
 * @param nodes
 * @param treeSize
 * @return returns the tree diameter
 */
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

/**
 * using bfs. O(n) run time.
 * @param v
 * @param u
 * @param nodes
 * @param treeSize
 * @return Returns a pointer to an int array represents the path from u to v.
 */
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
    return path;
}

/**
 * Frees the tree malloc
 * @param nodes
 * @param treeSize
 */
void freeTree(Node *nodes, int treeSize)
{
    for (int i = 0; i < treeSize; ++i)
    {
        free(nodes[i].children);
        nodes[i].children = NULL;
    }
    free(nodes);
}

/**
 * prints invalid input message.
 * @param nodes
 * @param treeSize
 * @return EXIT_FAILURE
 */
int invalidInput(Node *nodes, int treeSize)
{
    fprintf(stderr, "%s", INVALID_INPUT);
    if (treeSize > 0)
    {
        freeTree(nodes, treeSize);
    }
    return EXIT_FAILURE;
}

/**
 * validates the args.
 * @param args
 * @return METHOD_SUCCESS is valid, INVALID_ARG if invalid.
 */
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

/**
 * prints the path from v to u
 * @param path the path from u to v
 */
void printPath(unsigned int *path, int v, int u)
{
    printf("Shortest Path Between %d and %d: ", v, u);
    int len = (int) path[0];
    for (int i = len - 1; i > 0; --i)
    {
        if (i == 1)
        {
            printf("%d\n", path[i]);
        }
        else
        {
            printf("%d ", path[i]);
        }
    }
}

/**
 * prints out the matching messages to stdout..
 * @param nodes
 * @param treeSize
 * @param root
 * @param path
 */
void printMessages(Node *nodes, int treeSize, Node *root, unsigned int *path, int v, int u)
{
    int vertices = treeSize;
    int edges = countEdges(vertices);
    unsigned int minBranch = getTreeHeight(root->nodeKey, nodes, treeSize, 0);
    unsigned int maxBranch = getTreeHeight(root->nodeKey, nodes, treeSize, 1);
    unsigned int diameter = getTreeDiameter(nodes, treeSize);

    printf("Root Vertex: %d\n", root->nodeKey);
    printf("Vertices Count: %d\n", vertices);
    printf("Edges Count: %d\n", edges);
    printf("Length of Minimal Branch: %d\n", minBranch);
    printf("Length of Maximal Branch: %d\n", maxBranch);
    printf("Diameter Length: %d\n", diameter);
    printPath(path, v, u);
    free(path);
}

/**
 * the main function
 * @param argc args count
 * @param args
 * @return EXIT_FAILURE if program fails, EXIT_SUCCESS otherwise.
 */
int main(int argc, char *args[])
{
    if (argc != 4) // <program name> <file path> <v> <u>
    {
        fprintf(stderr, "%s", INVALID_USAGE);
        return EXIT_FAILURE;
    }
    else if (validateArgs(args) == INVALID_ARG)
    {
        return invalidInput(NULL, 0);
    }

    int treeSize = validateFile(args[1]);
    if (treeSize == INVALID_TREE_SIZE || treeSize == INVALID_FILE)
    {
        return invalidInput(NULL, 0); // invalid tree size
    }
    Row *rows = parseFile(args[1], treeSize);

    Node *nodes = initNodes(treeSize);

    int res = initTree(rows, nodes, treeSize);
    free(rows);
    if (res == INVALID_ROW)
    {
        return invalidInput(nodes, treeSize);
    }

    Node *root = getRoot(nodes, treeSize);
    if (root == NULL)
    {
        return invalidInput(nodes, treeSize);
    }

    // already verified to be legit numeric:
    int v = (int) strtol(args[2], NULL, 10);
    int u = (int) strtol(args[3], NULL, 10);
    if (v < 0 || u < 0 || v >= treeSize || u >= treeSize)
    {
        return invalidInput(nodes, treeSize);
    }

    unsigned int *path = getPathBetweenNodes(v, u, nodes, treeSize);
    if (path == NULL)
    {
        return invalidInput(nodes, treeSize);
    }
    printMessages(nodes, treeSize, root, path, v, u);

    freeTree(nodes, treeSize);
    nodes = NULL;

    return EXIT_SUCCESS;
}