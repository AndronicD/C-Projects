
#ifndef AVLTREE_H_
#define AVLTREE_H_

#include<stdlib.h>

// An AVL tree node
typedef struct AVLNode{
	Item elem;
	int height;
	struct AVLNode *l;
	struct AVLNode *r;
	struct AVLNode *p;
}AVLNode;

typedef struct AVLTree{
	long size;
	AVLNode* root;
	AVLNode* nil;
	int (*comp)(Item a, Item b);
}AVLTree;

AVLTree* avlCreateTree(int (*comp) (Item1,Item1))
{
	AVLTree* newTree = (AVLTree*) malloc(sizeof(AVLTree));
	newTree->comp = comp;


	newTree->nil = (AVLNode*) malloc(sizeof(AVLNode));
	newTree->nil->p = newTree->nil->l = newTree->nil->r = newTree->nil;
	newTree->nil->height = 0;


	newTree->root = (AVLNode*) malloc(sizeof(AVLNode));
	newTree->root->p = newTree->root->l = newTree->root->r = newTree->nil;
	newTree->root->height = 0;

	newTree->size = 0;

	return newTree;
}

int avlIsEmpty(AVLTree* tree){
	return (tree->root->l == tree->nil);
}

AVLNode* avlNewNode(AVLTree* tree){
	AVLNode* newNode = (AVLNode*) malloc(sizeof(AVLNode));
	// Initialize the new node to be used in the tree
	newNode->p = newNode->r = newNode->l = tree->nil;
	newNode->height = 1;

	return newNode;
}



// A utility function to get maximum of two integers
int max(int a, int b){
	return (a > b)? a : b;
}

// A utility function to right rotate subtree rooted with y
void avlRightRotate(AVLTree *tree,  AVLNode *y){
	//TO DO HERE
	AVLNode *x = y->l; 


	// Link-uri beta
	y->l = x->r;
	if(x->r != tree->nil)
		x->r->p = y;

	// Link-uri parinte
	x->p = y->p;
	if(y->p->l == y)
		y->p->l = x;
	else
		y->p->r = x;

	//Link-uri intre noduri
	x->r = y;
	y->p = x;

	y->height = max(y->l->height, y->r->height) + 1;
	x->height = max(x->l->height, x->r->height) + 1;
}

// A utility function to left rotate subtree rooted with x
void avlLeftRotate(AVLTree *tree, AVLNode *x){
	//TO DO HERE
	AVLNode *y = x->r;

	x->r = y->l;
	if(y->l != tree->nil)
		y->l->p = x;

	y->p = x->p;
	if(x->p->l == x)
		x->p->l = y;
	else
		x->p->r = y;

	y->l = x;
	x->p = y;

	y->height = max(y->l->height, y->r->height) + 1;
	x->height = max(x->l->height, x->r->height) + 1;
}

// Get Balance factor of node x
int avlGetBalance(AVLNode *x){
	if (x == NULL)
		return 0;
	return x->l->height - x->r->height;
}

AVLNode * avlMinimum(AVLTree* tree, AVLNode* x){
	while (x->l != tree->nil)
		x = x->l;
	return x;
}

AVLNode* avlMaximum(AVLTree* tree, AVLNode* x){
	while(x->r != tree->nil){
		x = x->r;
	}
	return x;
}

void avlInsertFixUp(AVLTree* tree, AVLNode* y, Item elem){
	while(y != tree->root){
		y->height = max(y->l->height, y->r->height) + 1;
		int balance = avlGetBalance(y);

		if(balance > 1 && (-1 == tree->comp(elem, y->l->elem)))
			avlRightRotate(tree, y);

		if(balance < -1 && (1 == tree->comp(elem, y->r->elem)))
			avlLeftRotate(tree, y);

		if(balance > 1 && (1 == tree->comp(elem, y->l->elem))){
			avlLeftRotate(tree, y->l);
			avlRightRotate(tree, y);
		}

	    if(balance < -1 && (-1 == tree->comp(elem, y->r->elem))){
			avlRightRotate(tree, y->l);
			avlLeftRotate(tree, y);
		}

		y = y->p;
    }
}

void avlInsert(struct AVLTree* tree, Item elem){
	//TO DO HERE
	AVLNode* x = tree->root->l;
	AVLNode* y = tree->root;

	while(x != tree->nil){
		y = x;
		if(1 == tree->comp(x->elem, elem))
			x = x->l;
		else if(-1 ==tree->comp(x->elem, elem))
			x = x->r;
		else
			return;
	}


	AVLNode *z = avlNewNode(tree);
	z->elem = elem;

	z->p = y;
	if(y == tree->root || (1 == tree->comp(y->elem, z->elem)))
		y->l = z;
	else
		y->r = z;

	tree->size++;

	avlInsertFixUp(tree, y, elem);
}




void avlDeleteNode(AVLTree *tree, AVLNode* node){
	destroyElem(node->elem);
	free(node);
}


void avlDestroyTreeHelper(AVLTree* tree, AVLNode* x) {
	if (x != tree->nil) {
		avlDestroyTreeHelper(tree,x->l);
		avlDestroyTreeHelper(tree,x->r);
		avlDeleteNode(tree,x);
	}
}

void avlDestroyTree(AVLTree* tree){
	avlDestroyTreeHelper(tree, tree->root->l);
	free(tree->root);
	free(tree->nil);
	free(tree);
}

#endif /* AVLTREE_H_ */