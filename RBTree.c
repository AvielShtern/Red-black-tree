/**
* @file RBTree.c
* @author Aviel Shtern Aviel.Shtern@mail.huji.ac.il
* @version 1.0
* @date 3 jun 2020
* @brief A skeleton of a binary search tree called "Red Black Tree." The running time of insert
* delete and searching in the tree is O(logn).
*/

#include <stdio.h>
#include <stdlib.h>
#include "RBTree.h"
#include <stdbool.h>

/**
 *@def ONE_NODE_IN_TREE 1
 *@brief Used in case there is only one child in the tree.
 */
#define ONE_NODE_IN_TREE 1

/**
 *@def ZERO_NODE_IN_TREE 0
 *@brief Used in case there is zero nodes in tree.
 */
#define ZERO_NODE_IN_TREE 0

/**
 *@def LEFT_BLACK_RIGHT_BLACK 0
 *@brief When deleting the tree in some cases we want to know the colors of the children.
 * If they are both BLACK We will use 0 to Say it
 */
#define LEFT_BLACK_RIGHT_BLACK 0

/**
 *@def LEFT_BLACK_RIGHT_RED 1
 *@brief When deleting the tree in some cases we want to know the colors of the children.
 * if the left child is BLACK and the right child is RED We will use 1 to Say it
 */
#define LEFT_BLACK_RIGHT_RED 1

/**
 *@def LEFT_RED_RIGHT_BLACK 2
 *@brief When deleting the tree in some cases we want to know the colors of the children.
 * if the left child is RED and the right child is BLACK We will use 2 to Say it
 */
#define LEFT_RED_RIGHT_BLACK 2

/**
 * Allocates memory to a new tree
 * @return pointer of type RBTree
 */
RBTree *treeAlloc();

/**
 * Given information, we will create a new node and format its color to red
 * @param data the data
 * @return the new node. if allocata not success retuen NULL
 */
Node *createNode(void *data);

/**
 * Makes the process of inserting value into a standard binary tree. Comes to where the value
 * "should" be. If the value is already in the tree We don't do anything, else we Creates a new
 * node with the value and inserts it where we want. and than return the node.
 * @param tree the tree
 * @param parent the parent of Current node (this function recursive)
 * @param node Current node (this function recursive)
 * @param data the data we want to insert
 * @return the new node we create and insert. and NULL if the value already there.
 */
Node *insertToNormalBst(RBTree *tree, Node *parent, Node **node, void *data);

/**
 * After "normal" income into the tree. We will check if the tree reserve has been breached in
 * red and black and repaired as needed
 * @param tree the tree
 * @param n The new node we entered
 */
void fixInsertToRBTree(RBTree *tree, Node *n);

/**
 * Makes a right rotation in the tree on a given node
 * @param tree the tree
 * @param y The node on which we want to turn
 */
void rightRotation(RBTree *tree, Node *y);

/**
 * Makes a left rotation in the tree on a given node
 * @param tree the tree
 * @param x The node on which we want to turn
 */
void leftRotation(RBTree *tree, Node *x);

/**
 * Given the node the function will return its brother
 * @param node current node
 * @return the brother of current node (can be NULL!!)
 */
Node *getBrother(const Node *node);

/**
 * Given the node the function will return its uncle
 * @param node current node
 * @return the uncle of current node (can be NULL!!)
 */
Node *getUncle(const Node *node);

/**
 * Given the node the function will return its grandpa
 * @param node current node
 * @return the grandpa of current node (can be NULL!!)
 */
Node *getGrandpa(const Node *node);

/**
 * Performs a standard binary tree search process. And returns the node where the value is
 * @param tree the tree
 * @param node the current node Who wants to search his sub tree (this function recursive)
 * @param data The information that the node contains
 * @return NULL if the data not contain in the tree. else the node who contains tha data.
 */
Node *findXNormalBST(const RBTree *tree, Node *node, const void *data);

/**
 * Finds the node that contains the minimum data in the tree
 * @param node the current node Who wants to search his sub tree (this function recursive)
 * @return the node that contains the minimum data in the tree
 */
Node *findMin(Node *node);

/**
 * Given a node, we return its successive node in the tree. ("successor")
 * @param n node
 * @return the successor of the node
 */
Node *getSuccessor(Node *n);

/**
 * frees all the node's allocates in the tree (frees the allocate for data field)
 * @param node the current node Who wants to free his sub tree (this function recursive)
 * @param freeFunc The function who responsible for free the allocate in a data field
 */
void freeNodes(Node **node, FreeFunc freeFunc);

/**
 * Performs the first step of deleting node from a regular binary tree. If there are two
 * children returns his successor. Otherwise, do nothing
 * @param n the node who contains the value we wont to delete
 * @return If there are two children returns his successor. Otherwise, return NULL
 */
Node *deleteNormalBST(Node *node);

/**
 * Before the actual deletion. We will replace the parental child we want to erase with his father.
 * @param n the node we want to delete
 * @param child the child of node we want to delete (can be NULL)
 */
void replaceNode(Node *n, Node *child);

/**
 * After the initial process of deletion. We will stay with a node that has at most one child.
 * The function will erase the node and balance the tree
 * @param tree the tree
 * @param n the node we want to delete
 */
void deleteOneChild(RBTree *tree, Node **n);

/**
 * Handles the case ״N is the new root״
 * @param tree the tree
 * @param node the child of node we wont to delete (after the "replaceNode")
 * @param parent the parent of node we want to delete. (after the "replaceNode" the parent of node)
 */
void deleteLevel1(RBTree *tree, Node *node, Node *parent);

/**
 * Handles the case ״S is RED" do left rotation and after that right rotation on the parent
 * in any case we move on to deleteLevel3
 * @param tree the tree
 * @param node the child of node we wont to delete (after the "replaceNode")
 * @param parent the parent of node we want to delete. (after the "replaceNode" the parent of node)
 */
void deleteLevel2(RBTree *tree, Node *node, Node *parent);

/**
 * Handles the case ״P is BLACK S is RED and S's children are BLACK" we replase the color of S
 * and back to deleteLevel1
 * @param tree the tree
 * @param node the child of node we wont to delete (after the "replaceNode")
 * @param parent the parent of node we want to delete. (after the "replaceNode" the parent of node)
 */
void deleteLevel3(RBTree *tree, Node *node, Node *parent);

/**
 * Handles the case ״P is RED S is RED and S's children are BLACK" we replase the color of S and P
 * @param tree the tree
 * @param node the child of node we wont to delete (after the "replaceNode")
 * @param parent the parent of node we want to delete. (after the "replaceNode" the parent of node)
 */
void deleteLevel4(RBTree *tree, Node *node, Node *parent);

/**
 * Handles the case "S is RED and S's left ("the "closer" son) child are RED and S's right BLACK"
 * @param tree the tree
 * @param node the child of node we wont to delete (after the "replaceNode")
 * @param parent the parent of node we want to delete. (after the "replaceNode" the parent of node)
 */
void deleteLevel5(RBTree *tree, Node *node, Node *parent);

/**
 * After the deleteLevel5 changes, on P and S accordingly
 * @param tree the tree
 * @param node the child of node we wont to delete (after the "replaceNode")
 * @param parent the parent of node we want to delete. (after the "replaceNode" the parent of node)
 */
void deleteLevel6(RBTree *tree, Node *node, Node *parent);

/**
 * Checking the colors of the children. We refer to a BLACK "child" even if it is not a node but a
 * NULL leaf.
 * @param node object with type Node
 * @return 1 if the left child is BLACK and the right child is RED.
 *         2 if the left child is RED and the right child is BLACK.
 *         0 if tho children are BLACK.
 */
int theColorOfchildrenAre(Node *node);

/**
 * chack if node and to of are children BLACK
 * @param node the node we want to chack
 * @return true if the node are BLACK and two of are children BLACK. else return false
 */
int nodeAndthoSunsAreBlack(Node* node);

RBTree *treeAlloc()
{
	RBTree *tree = (RBTree *) calloc(1, sizeof(RBTree));
	return tree;
}

RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
	RBTree *tree = treeAlloc();
	tree->compFunc = compFunc;
	tree->freeFunc = freeFunc;
	return tree;
}

Node *createNode(void *data)
{
	Node *newNode = (Node *) malloc(sizeof(Node));
	if (newNode == NULL)
	{
		return NULL;
	}
	newNode->data = data;
	newNode->parent = NULL;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->color = RED;

	return newNode;
}

int insertToRBTree(RBTree *tree, void *data)
{
	if (data == NULL)
	{
		return false;
	}
	Node *n = insertToNormalBst(tree, NULL, &(tree->root), data); // insert normal node, after we
	// fix from m
	if (n == NULL)
	{
		return false;
	}
	tree->size++;
	fixInsertToRBTree(tree, n);
	return true;
}


Node *insertToNormalBst(RBTree *tree, Node *parent, Node **node, void *data)
{
	if (tree == NULL)
	{
		return NULL;
	}
	if (*node == NULL)
	{
		*node = createNode(data);
		if (*node == NULL)
		{
			return NULL;
		}
		(*node)->parent = parent;
		return *node;
	}
	int res = tree->compFunc(data, (*node)->data);
	if (res == 0)
	{
		return NULL; // data exit
	}
	else if (res > 0)
	{
		return insertToNormalBst(tree, *node, &((*node)->right), data);
	}
	else // res <0
	{
		return insertToNormalBst(tree, *node, &((*node)->left), data);
	}
}


void rightRotation(RBTree *tree, Node *y)
{
	if (y == NULL || y->left == NULL)
	{
		return;
	}
	Node *x = y->left;
	Node *t2 = x->right;

	x->parent = y->parent;
	if (x->parent != NULL)
	{
		if (x->parent->left == y)
		{
			x->parent->left = x;
		}
		else // y was right child
		{
			x->parent->right = x;
		}
	}
	else // x is the new root in the tree
	{
		tree->root = x;
	}
	x->right = y;
	y->parent = x;
	y->left = t2;
	if (t2 != NULL)
	{
		t2->parent = y;
	}

}

void leftRotation(RBTree *tree, Node *x)
{
	if (x == NULL || x->right == NULL)
	{
		return;
	}
	Node *y = x->right;
	Node *t2 = y->left;

	y->parent = x->parent;
	if (y->parent != NULL)
	{
		if (y->parent->left == x)
		{
			y->parent->left = y;
		}
		else // x was right child
		{
			y->parent->right = y;
		}
	}
	else // y is the new root in the tree
	{
		tree->root = y;
	}
	y->left = x;
	x->parent = y;
	x->right = t2;
	if (t2 != NULL)
	{
		t2->parent = x;
	}
}

Node *getUncle(const Node *node)
{
	if (node == NULL)
	{
		return NULL;
	}
	return getBrother(node->parent);
}

Node *getBrother(const Node *node)
{
	if (node == NULL || node->parent == NULL)
	{
		return NULL;
	}
	else if (node->parent->left == node)
	{
		return node->parent->right;
	}
	else // node is right child
	{
		return node->parent->left;
	}
}

Node *getGrandpa(const Node *node)
{
	if (node == NULL || node->parent == NULL)
	{
		return NULL;
	}
	return node->parent->parent;
}

void fixInsertToRBTree(RBTree *tree, Node *n)
{
	Node *parent = n->parent;
	Node *grandpa = getGrandpa(n);
	Node *uncle = getUncle(n);
	if (parent == NULL)
	{
		n->color = BLACK;
		return;
	}
	else if (parent->color == BLACK)
	{
		return;
	}
	else if (uncle != NULL && uncle->color == RED)
	{
		parent->color = BLACK;
		uncle->color = BLACK;
		grandpa->color = RED;
		return fixInsertToRBTree(tree, grandpa);
	}
	else if (grandpa->right == parent && parent->right == n)
	{
		leftRotation(tree, grandpa);
		parent->color = BLACK;
		grandpa->color = RED;
		return;
	}
	else if (grandpa->left == parent && parent->left == n)
	{
		rightRotation(tree, grandpa);
		parent->color = BLACK;
		grandpa->color = RED;
		return;
	}
	else if (grandpa->right == parent && parent->left == n)
	{
		rightRotation(tree, parent);
		leftRotation(tree, grandpa);
		n->color = BLACK;
		grandpa->color = RED;
		return;
	}
	else // grandpa->left->right == n
	{
		leftRotation(tree, parent);
		rightRotation(tree, grandpa);
		n->color = BLACK;
		grandpa->color = RED;
		return;
	}
}

int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
	if (tree == NULL)
	{
		return false;
	}
	if (tree->root == NULL)
	{
		return true;
	}
	Node *curNode = findMin(tree->root);
	while (curNode != NULL)
	{
		if (func(curNode->data, args) == 0)
		{
			return false;
		};
		curNode = getSuccessor(curNode);
	}
	return true;
}

int RBTreeContains(const RBTree *tree, const void *data)
{
	if (tree == NULL || data == NULL || findXNormalBST(tree, tree->root, data) == NULL)
	{
		return false;
	}
	return true;
}

Node *findXNormalBST(const RBTree *tree, Node *node, const void *data)
{
	if (node == NULL || data == NULL)
	{
		return NULL;
	}
	int res = tree->compFunc(data, node->data);
	if (res == 0)
	{
		return node;
	}
	else if (res > 0)
	{
		return findXNormalBST(tree, node->right, data);
	}
	else // res <0
	{
		return findXNormalBST(tree, node->left, data);
	}
}

Node *findMin(Node *node)
{
	if (node->left == NULL)
	{
		return node;
	}
	return findMin(node->left);
}

Node *getSuccessor(Node *n)
{
	if (n->right != NULL)
	{
		return findMin(n->right);
	}
	Node *p = n->parent;
	while (p != NULL && n == p->right)
	{
		n = p;
		p = p->parent;
	}
	return p;
}

int deleteFromRBTree(RBTree *tree, void *data)
{
	if (tree == NULL)
	{
		return false;
	}
	Node *initNode = findXNormalBST(tree, tree->root, data);
	if (initNode == NULL) // the value not in tree!!
	{
		return false;
	}
	Node *node = deleteNormalBST(initNode); // from now. to node have 1 chiled in worst case
	deleteOneChild(tree, &node);

	return true;
}

Node *deleteNormalBST(Node *node)
{
	if (node->left != NULL && node->right != NULL)
	{
		Node *successor = getSuccessor(node);
		void *nodeData = node->data;
		node->data = successor->data;
		successor->data = nodeData;
		return successor;
	}
	return node;
}

void replaceNode(Node *n, Node *child)
{
	Node *parent = n->parent;
	if (child == NULL && parent == NULL)
	{
		return;
	}
	else if (child != NULL && parent == NULL)
	{
		child->parent = parent;
		return;
	}
	else if (child == NULL) // parent != NULL
	{
		if (n->parent->left == n)
		{
			n->parent->left = child;
		}
		else // n is right sum
		{
			n->parent->right = child;
		}
	}
	else // child != NULL && parent != NULL
	{
		child->parent = parent;
		if (n->parent->left == n)
		{
			n->parent->left = child;
		}
		else // n is right sum
		{
			n->parent->right = child;
		}
	}
}


void deleteOneChild(RBTree *tree, Node **n)
{
	Node *child = ((*n)->right == NULL) ? (*n)->left : (*n)->right;

	replaceNode(*n, child);
	if ((*n)->color == BLACK)
	{
		if (child != NULL && child->color == RED)
		{
			child->color = BLACK;
		}
		else
		{
			deleteLevel1(tree, child, (*n)->parent);
		}
	}

	tree->freeFunc((*n)->data);
	free(*n);
	*n = NULL;
	tree->size = tree->size - 1;
	if (tree->size == ONE_NODE_IN_TREE && child != NULL)
	{
		tree->root = child;
	}
	if (tree->size == ZERO_NODE_IN_TREE)
	{
		tree->root = NULL;
	}
}

void deleteLevel1(RBTree *tree, Node *node, Node *parent)
{
	if (parent != NULL)
	{
		deleteLevel2(tree, node, parent);
	}
}

void deleteLevel2(RBTree *tree, Node *node, Node *parent)
{
	Node *s = (parent->left == node) ? parent->right : parent->left;
	if (s != NULL && s->color == RED)
	{
		parent->color = RED;
		s->color = BLACK;
		if (parent->left == node)
		{
			leftRotation(tree, parent);
		}
		else
		{
			rightRotation(tree, parent);
		}
	}
	deleteLevel3(tree, node, parent);
}

int nodeAndthoSunsAreBlack(Node* node)
{
	if (node == NULL)
	{
		return true;
	}
	else if (node->color == BLACK &&
			 (node->left == NULL || node->left->color == BLACK) &&
			 (node->right == NULL || node->right->color == BLACK))
	{
		return true;
	}
	return false;
}
void deleteLevel3(RBTree *tree, Node *node, Node *parent)
{
	Node *s = (parent->left == node) ? parent->right : parent->left;
	if (parent->color == BLACK && nodeAndthoSunsAreBlack(s) == 1)
	{
		if (s != NULL)
		{
			s->color = RED;
		}
		deleteLevel1(tree, parent, parent->parent);
	}
	else
	{
		deleteLevel4(tree, node, parent);
	}
}

void deleteLevel4(RBTree *tree, Node *node, Node *parent)
{
	Node *s = (parent->left == node) ? parent->right : parent->left;
	if (parent->color == RED && nodeAndthoSunsAreBlack(s) == 1)
	{
		if (s != NULL)
		{
			s->color = RED;
		}
		parent->color = BLACK;
	}
	else
	{
		deleteLevel5(tree, node, parent);
	}
}

int theColorOfchildrenAre(Node *node)
{
	if (node == NULL)
	{
		return LEFT_BLACK_RIGHT_BLACK;
	}
	else if ((node->right == NULL || node->right->color == BLACK) && (node->left != NULL &&
	         node->left->color == RED))
	{
		return LEFT_RED_RIGHT_BLACK;
	}
	else if ((node->left == NULL || node->left->color == BLACK) && (node->right != NULL &&
	         node->right->color == RED))
	{
		return LEFT_BLACK_RIGHT_RED;
	}
	return LEFT_BLACK_RIGHT_BLACK;
}

void deleteLevel5(RBTree *tree, Node *node, Node *parent)
{
	Node *s = (parent->left == node) ? parent->right : parent->left;
	if (s == NULL || s->color == BLACK)
	{
		if (parent->left == node && theColorOfchildrenAre(s) == 2)
		{
			s->color = RED;
			s->left->color = BLACK;
			rightRotation(tree, s);

		}
		else if (parent->right == node && theColorOfchildrenAre(s) == 1)
		{
			s->color = RED;
			s->right->color = BLACK;
			leftRotation(tree, s);
		}
	}
	deleteLevel6(tree, node, parent);
}

void deleteLevel6(RBTree *tree, Node *node, Node *parent)
{
	Node *s = (parent->left == node) ? parent->right : parent->left;
	s->color = parent->color;
	parent->color = BLACK;
	if (parent->left == node)
	{
		s->right->color = BLACK;
		leftRotation(tree, parent);
	}
	else
	{
		s->left->color = BLACK;
		rightRotation(tree, parent);
	}
}

void freeNodes(Node **node, FreeFunc freeFunc)
{
	if ((*node) == NULL)
	{
		return;
	}
	freeNodes(&((*node)->left), freeFunc);
	freeNodes(&((*node)->right), freeFunc);
	freeFunc((*node)->data);
	free(*node);
	*node = NULL;
}

void freeRBTree(RBTree **tree)
{
	if (*tree == NULL)
	{
		return;
	}
	else if ((*tree)->root == NULL)
	{
		free(*tree);
		*tree = NULL;
		return;
	}
	else// there are node to be free allocated
	{
		freeNodes(&(*tree)->root, (*tree)->freeFunc);
		free(*tree);
		*tree = NULL;
	}
}