/*
*	Created by Nan Mihai on 28.03.2021
*	Copyright (c) 2021 Nan Mihai. All rights reserved.
*	Laborator 5 - Structuri de date
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2020-2021, Seria CD
*/
#include "tree.h"

/*
*	Funcție care creează un arbore cu un singur nod
*/
TreeNode* createTree(Item value) {
	TreeNode *root = malloc(sizeof(TreeNode));
	root->value = value;
	root->left = NULL;
	root->right = NULL;
	return root;
}

/*
*	Funcție care inițializează un nod de arbore
*		- îi alocă memorie
*		- îi setează câmpul valoare
*		- setează left și right să pointeze către NULL
*/
void init(Tree *root, Item value) {
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	node->left = node->right = NULL;
	node->value = value;
	*root = node;
}

/*
*	Funcție care inserează o valoare într-un arbore binar, respectând
* proprietățile unui arbore binar de căutare
*/
Tree insert(Tree root, Item value){
	if(root == NULL){
		init(&root, value);
		return root;
		//return createTree(value);
	}
	if(root->value == value){
		return root;
	}
	if(value < root->value) {
		if(root->left == NULL){
			init(&root->left, value);
			//root->left == createTree(value);
			return root;
		}
		else{
			root->left = insert(root->left, value);
			return root;
		}
	}
	else{
		if(root->right == NULL){
			init(&root->right, value);
			//root->right == createTree(value);
			return root;
		}
		else{
			root->right = insert(root->right, value);
			return root;
		}
	}
}

/*
*	Funcție care afișează nodurile folosind parcurgerea în postordine
*/
void printPostorder(TreeNode* root) {
	// TODO 3
	if(root == NULL) return;
	printPostorder(root->left);
	printPostorder(root->right);
	printf("%d ", root->value);
}

/*
*	Funcție care afișează nodurile folosind parcurgerea în preordine
*/
void printPreorder(Tree root) {
	// TODO 4
	if(root == NULL) return;
	printf("%d ", root->value);
	printPreorder(root->left);
	printPreorder(root->right);
}

/*
*	Funcție care afișează nodurile folosind parcurgerea în inordine
*/
void printInorder(Tree root) {
	if(root == NULL) return;
	printInorder(root->left);
	printf("%d ", root->value);
	printInorder(root->right);
}

/*
*	Funcție care dealocă întreaga memorie alocată pentru un arbore binar
*		- root va pointa către NULL după ce se va apela funcția
*/
void freeTree(Tree *root) {
	if(*root == NULL) return;
	freeTree(&((*root)->left));
	freeTree(&((*root)->right));
	free(*root);
	*root = NULL;
}


/*
*	Funcție care determină numărul de noduri dintr-un arbore binar
*/
int size(Tree root) {
	// TODO 7
	if(root == NULL) return 0;
	return 1 + size(root->left) + size(root->right);
}

/*
*	Funcție care returnează adâncimea maximă a arborelui
*/
int maxDepth(Tree root) {
	// TODO 8
	if(root == NULL){
		return -1;
	}
	int l_depth = maxDepth(root->left);
	int r_depth = maxDepth(root->right);
	if(l_depth > r_depth) return l_depth + 1;
	else return r_depth + 1;
}

/*
*	Funcție care construiește oglinditul unui arbore binar
*/
void mirror(Tree root) {
	// TODO 9
	if(root == NULL) return;
	mirror(root->left);
	mirror(root->right);
	TreeNode *aux = root->left;
	root->left = root->right;
	root->right = aux;
}

/*
*	Funcție care verifică dacă doi arbori binari sunt identici
*/
int sameTree(Tree root1, Tree root2) {
	// TODO 10
	if(root1 == NULL && root2 == NULL)
	 	return 1;
	if(root1 == NULL || root2 == NULL)
		return 0;
	if(root1->value != root2->value){
		return 0;
	}
	int l_res = sameTree(root1->left, root2->left);
	int r_res = sameTree(root1->right,root2->right);
	return (l_res && r_res);
}
