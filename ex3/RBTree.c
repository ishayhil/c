#include "RBTree.h"
#include <stdlib.h>

RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree *rbTree = malloc(sizeof(RBTree));
    rbTree->compFunc = compFunc;
    rbTree->freeFunc = freeFunc;
    rbTree->root = NULL;
    rbTree->size = 0;

    return rbTree;
}

int fixTree(RBTree *tree, Node *newNode)
{
    Node *uncle;
    Node *parent = newNode->parent;
    if (newNode == tree->root)
    {
        newNode->color = BLACK;
        return 1;
    }
    else if (newNode->parent->color == BLACK)
    {
        return 1;
    }
    else if ((uncle = findUncle(newNode))->color == RED)
    {
        uncle->color = BLACK;
        newNode->parent->color = BLACK;
        newNode->parent->parent->color = RED;
        return fixTree(tree, newNode->parent->parent);
    } // parent is red, check if uncle is read too
    else
    { // parent is red, uncle is black
        if (!(isRightSonOfLeftSon(newNode) && isLeftSonOfRightSon(newNode)))
        {
            return 1;
        }
        else
        {
            return rotate(tree, newNode, parent);
        }
    }
}

int addToRBTree(RBTree *tree, void *data)
{
    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->color = RED;
    node->right = NULL;
    node->left = NULL;

    if (tree->size == 0)
    {
        tree->root = node;
        tree->size++;
        return 1;
    }

    Node *currentNode = tree->root;
    for (int i = 0; i < tree->size; ++i)
    {
        if (tree->compFunc(currentNode->data, data) >= 0) // node >= newNode
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

Node *findUncle(Node *node) // node distance from root is at least 2 when invoked!
{
    Node *parent = node->parent;
    if (parent->parent->left == parent)
    {
        return parent->right;
    }
    else
    {
        return parent->left;
    }
}

int isLeftSon(Node *node)
{
    return node->parent->left == node;
}

int isLeftSonOfRightSon(Node *node)
{
    return isLeftSon(node) && !isLeftSon(node->parent);
}

int isRightSonOfLeftSon(Node *node)
{
    return !isLeftSon(node) && isLeftSon(node->parent);
}

int rotate(RBTree *tree, Node *newNode, Node *parent)
{
    if (!(isRightSonOfLeftSon(newNode) && isLeftSonOfRightSon(newNode)))
    {
        return 1;
    }
    else if (isRightSonOfLeftSon(newNode))
    {
        newNode->left = parent;
        newNode->parent = parent->parent;
        parent->parent->left = newNode;
        parent->parent = newNode;
        parent->right = NULL;

        newNode->parent->left = newNode->right;
        newNode->right->parent = newNode->parent;
        newNode->right = newNode->parent;
        newNode->parent = newNode->right->parent;
        newNode->right->parent = newNode;
    }
    else
    {
        newNode->right = parent;
        newNode->parent = parent->parent;
        parent->parent->right = newNode;
        parent->parent = newNode;
        parent->left = NULL;

        newNode->parent->right = newNode->left;
        newNode->left->parent = newNode->parent;
        newNode->left = newNode->parent;
        newNode->parent = newNode->left->parent;
        newNode->left->parent = newNode;
    }
    return 1;
}

int compare(void *a, void *b)
{
    int *x = (int *) a;
    int *y = (int *) b;
    return *x > *y;
}

int forEachFunc1(const void *object, void *args)
{
    return 1;
}

int main()
{
    RBTree *tree = newRBTree(compare, forEachFunc1);
}
