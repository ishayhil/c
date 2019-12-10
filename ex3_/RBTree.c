#include <stdio.h>
#include "RBTree.h"
#include "Structs.h"
#include <stdlib.h>

#define SUCCESS (1)
#define FAILURE (0)
#define LESS (-1)
#define EQUAL (0)
#define GREATER (1)

/**
 * @param node
 * @return checks if node is left son or right son.
 */
static int isLeftSon(Node *node)
{
    return node->parent->left == node;
}

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
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

/**
 * checks if the given node is left son of right son
 * @param node
 */
static int isLeftSonOfRightSon(Node *node)
{
    return isLeftSon(node) && !isLeftSon(node->parent);
}

/**
 * checks if the given node is right son of left son
 * @param node
 */
static int isRightSonOfLeftSon(Node *node)
{
    return !isLeftSon(node) && isLeftSon(node->parent);
}

/**
 * checks if the given node is left son of left son
 * @param node
 */
static int isLeftSonOfLeftSon(Node *node)
{
    return isLeftSon(node) && isLeftSon(node->parent);
}

/**
 * checks if the given node is right son of right son
 * @param node
 */
static int isRightSonOfRightSon(Node *node)
{
    return !isLeftSon(node) && !isLeftSon(node->parent);
}

/**
 * rotates the tree to the left (where g is the root of the subtree)
 * @param g
 * @return 1 if success 0 else
 */
static int rotateRight(Node *g)
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

/**
 * rotates the tree to the right (where g is the root of the subtree)
 * @param g
 * @return 1 if success 0 else
 */
static int rotateLeft(Node *g)
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

/**
 * where x is right son of left son, this function will replace places so x and p will be left sons.
 * @param x
 * @return 1 if success 0 else
 */
static int stringItLeft(Node *x)
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

/**
 * where x is left son of right son, this function will replace places so x and p will be right sons.
 * @param x
 * @return 1 if success 0 else
 */
static int stringItRight(Node *x)
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

/**
 * when the uncle of a new node is black, this function will fix the tree
 * @param newNode
 * @param tree
 * @return 1 if success, 0 else
 */
static int blackUncle(Node *newNode, RBTree *tree)
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

/**
 * find the uncle of the given node.
 * @param node
 * @return the uncle
 */
static Node *findUncle(Node *node) // node distance from root is at least 2 when invoked!
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

/**
 * fixes the tree after a new insert
 * @param tree
 * @param newNode
 * @return 1 if success, 0 else
 */
static int fixTree(RBTree *tree, Node *newNode)
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
    { // parent is red, uncle is black
        return blackUncle(newNode, tree);
    }
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
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

    if (tree->root == NULL)
    {
        tree->root = node;
        tree->size++;
        return fixTree(tree, node);
    }

    Node *currentNode = tree->root;
    while (1)
    {
        int comp = tree->compFunc(currentNode->data, data);
        if (comp == 0)
        {
            free(node);
            return FAILURE;
        }
        else if (comp > 0) // node >= newNode
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
    tree->size++;
    return fixTree(tree, node);
}

/**
 * check whether the tree contains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
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

/**
 * actives the for each function on every node, in order.
 * @param root
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return 1 if success, 0 else.
 */
static int forEachRBTreeRecursive(Node *root, forEachFunc func, void *args)
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

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
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

/**
 * frees the tree using the free function (recursive).
 * @param root
 * @param freeFunc the free func
 */
static void freeTreeRecursive(Node *root, FreeFunc freeFunc)
{
    if (root == NULL)
    {
        return;
    }
    freeTreeRecursive(root->left, freeFunc);
    freeTreeRecursive(root->right, freeFunc);
    freeFunc(root->data);
    free(root);
}

/**
 * free all memory of the data structure.
 * @param tree: the tree to free.
 */
void freeRBTree(RBTree *tree)
{
    freeTreeRecursive(tree->root, tree->freeFunc);
    tree->root = NULL;
    free(tree);
}