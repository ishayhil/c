#include <stdio.h>
#include "RBTree.h"
#include <stdlib.h>

#define SUCCESS (1)
#define FAILURE (0)
#define LESS (-1)
#define EQUAL (0)
#define GREATER (1)


int isLeftSon(Node *node)
{
    return node->parent->left == node;
}

void printNode(Node *node)
{
    if (node->parent == NULL)
    {
        printf("data: %d, parent data NULL, BLACK: %d\n", *(int *) node->data, node->color);
    }
    else
    {
        int isLeft = isLeftSon(node);
        printf("data: %d, parent data %d, BLACK: %d, left son: %d\n",
               *(int *) node->data,
               *(int *) node->parent->data,
               node->color,
               isLeft);
    }
}

void printTree(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    printTree(root->left);
    printNode(root);
    printTree(root->right);
}

RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    if (compFunc == NULL || freeFunc == NULL)
    {
        return NULL;
    }

    RBTree *rbTree = malloc(sizeof(RBTree));
    rbTree->compFunc = compFunc;
    rbTree->freeFunc = freeFunc;
    rbTree->root = NULL;
    rbTree->size = 0;

    return rbTree;
}

int isLeftSonOfRightSon(Node *node)
{
    return isLeftSon(node) && !isLeftSon(node->parent);
}

int isRightSonOfLeftSon(Node *node)
{
    return !isLeftSon(node) && isLeftSon(node->parent);
}

int isLeftSonOfLeftSon(Node *node)
{
    return isLeftSon(node) && isLeftSon(node->parent);
}

int isRightSonOfRightSon(Node *node)
{
    return !isLeftSon(node) && !isLeftSon(node->parent);
}

int rotateRight(Node *g)
{

    Node *p = g->left;

    // handle p:
    p->parent = g->parent;
    if (g->parent != NULL)
    {
        if (isLeftSon(g))
        {
            g->parent->left = p;
        }
        else
        {
            g->parent->right = p;
        }
    }
    g->left = p->right;
    if (g->left != NULL)
    {
        p->right->parent = g;
    }
    p->right = g;
    // handle g:
    g->parent = p;

    return SUCCESS;
}

int rotateLeft(Node *g)
{
    Node *p = g->right;

    // handle p:
    p->parent = g->parent;
    if (g->parent != NULL)
    {
        if (isLeftSon(g))
        {
            g->parent->left = p;
        }
        else
        {
            g->parent->right = p;
        }
    }
    g->right = p->left;
    if (g->right != NULL)
    {
        g->right->parent = g;
    }
    p->left = g;
    // handle g:
    g->parent = p;

    return SUCCESS;
}

int stringItLeft(Node *x)
{
    Node *p = x->parent;
    Node *g = p->parent;
    // handle g
    g->left = x;
    // handle p
    p->right = x->left;
    if (p->right != NULL)
    {
        p->right->parent = p;
    }
    p->parent = x;
    // handle x
    x->parent = g;
    x->left = p;
    return SUCCESS;
}

int stringItRight(Node *x)
{
    Node *p = x->parent;
    Node *g = p->parent;
    // handle g
    g->right = x;
    // handle p
    p->left = x->right;
    if (p->left != NULL)
    {
        p->left->parent = p;
    }
    p->parent = x;
    // handle x
    x->parent = g;
    x->right = p;
    return SUCCESS;
}

int blackUncle(Node *newNode, RBTree *tree)
{
    if (isRightSonOfRightSon(newNode) || isLeftSonOfLeftSon(newNode))
    {
        Node *oldG = newNode->parent->parent;
        if (isRightSonOfRightSon(newNode))
        {
            rotateLeft(oldG);
        }
        else
        {
            rotateRight(oldG);
        }
        if (newNode->parent->parent == NULL) // new node parent might be current root
        {
            tree->root = newNode->parent;
        }
        newNode->parent->color = BLACK;
        oldG->color = RED;
        return SUCCESS;
    }
    else if (isRightSonOfLeftSon(newNode))
    {
        stringItLeft(newNode);
        return blackUncle(newNode->left, tree);
    }
    else if (isLeftSonOfRightSon(newNode))
    {
        stringItRight(newNode);
        return blackUncle(newNode->right, tree);
    }
    return SUCCESS;
}

Node *findUncle(Node *node) // node distance from root is at least 2 when invoked!
{
    Node *parent = node->parent;
    if (parent->parent->left == parent)
    {
        return parent->parent->right;
    }
    else
    {
        return parent->parent->left;
    }
}

int fixTree(RBTree *tree, Node *newNode)
{
    Node *uncle;
    if (newNode == tree->root)
    {
        newNode->color = BLACK;
        return SUCCESS;
    }
    else if (newNode->parent->color == BLACK)
    {
        return SUCCESS;
    }
    else if ((uncle = findUncle(newNode)) != NULL && uncle->color == RED)
    {
        uncle->color = BLACK;
        newNode->parent->color = BLACK;
        newNode->parent->parent->color = RED;
        return fixTree(tree, newNode->parent->parent);
    } // parent is red, check if uncle is read too
    else
    { // parent is red, uncle is black TODO check if null == black?
        return blackUncle(newNode, tree);
    }
}

int addToRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL)
    {
        return FAILURE;
    }

    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->color = RED;
    node->right = NULL;
    node->left = NULL;
    node->parent = NULL;

    tree->size++;
    if (tree->root == NULL)
    {
        tree->root = node;
        return fixTree(tree, node);
    }

    Node *currentNode = tree->root;
    while (1)
    {
        if (tree->compFunc(currentNode->data, data) > 0) // node >= newNode
        {

            if (currentNode->left == NULL)
            {
                node->parent = currentNode;
                currentNode->left = node;
                break;
            }
            else
            {
                currentNode = currentNode->left;
            }
        }
        else
        {
            if (currentNode->right == NULL)
            {
                node->parent = currentNode;
                currentNode->right = node;
                break;
            }
            else
            {
                currentNode = currentNode->right;
            }
        }
    }
    return fixTree(tree, node);
}

int containsRBTree(RBTree *tree, void *data)
{
    if (tree->root == NULL || data == NULL)
    {
        return FAILURE;
    }

    Node *current = tree->root;
    while (current != NULL)
    {
        int comp = tree->compFunc(current->data, data);
        if (comp == 0)
        {
            return SUCCESS;
        }
        else if (comp > 0)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return FAILURE;
}

int forEachRBTreeRecursive(Node *root, forEachFunc func, void *args)
{
    if (root == NULL)
    {
        return SUCCESS;
    }

    int t1 = forEachRBTreeRecursive(root->left, func, args);
    if (t1 == 0)
    {
        return t1;
    }
    int t = func(root->data, args);
    int t2 = forEachRBTreeRecursive(root->right, func, args);
    if (t2 == 0)
    {
        return t2;
    }
    return t;
}

int forEachRBTree(RBTree *tree, forEachFunc func, void *args)
{
    if (tree->root == NULL || func == NULL)
    {
        return FAILURE;
    }
    else
    {
        return forEachRBTreeRecursive(tree->root, func, args);
    }
}

int compare(const void *a, const void *b)
{
    int *x = (int *) a;
    int *y = (int *) b;
    if (*x == *y)
    {
        return EQUAL;
    }
    return *x > *y ? GREATER : LESS;
}

void freeTree(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void freeRBTree(RBTree *tree)
{

    freeTree(tree->root);
    tree->root = NULL;
    free(tree);
}

int forEachFunc1(const void *object, void *args)
{
    int *x = (int *) object;
    int *y = (int *) args;

    *y += *x;

    return SUCCESS;
}

void freeFunc1(void *data)
{
    free(data);
}

// typedef int (*CompareFunc)(const void *a, const void *b);
//int main()
//{
//    RBTree *tree = newRBTree(compare, freeFunc1);
////    int data[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
////
//    for (int i = 1 ; i < 21; i++)
//    {
//        int *a = (int *) malloc(sizeof(int));
//        *a = i;
//        addToRBTree(tree, a);
//    }
//
////    addToRBTree(tree, &(data[9]));
////    addToRBTree(tree, &(data[9]));
////    addToRBTree(tree, &(data[7]));
////    addToRBTree(tree, &(data[8]));
////    printf("%d", *(int *) tree->root->data);
//    printTree(tree->root);
//
//    int a = 0;
//    forEachRBTree(tree, forEachFunc1, &a);
//    printf("%d", a);
//
////    printf("%d", 8 % 2);
//}