#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hierarchy.h"


/* Adauga un angajat nou in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 * manager_name: numele sefului noului angajat
 *
 * return: intoarce ierarhia modificata. Daca tree si manager_name sunt NULL, 
           atunci employee_name e primul om din ierarhie.
 */



int f_comp(const void *a, const void *b){
    Tree tree1 = (Tree)a;
    Tree tree2 = (Tree)b;

    char *str1 = tree1->name;
    char *str2 = tree2->name;

    return strcmp(str1, str2);
}

void sort( Tree *v, int n) {
	for(int i = 0; i < n-1; i++)
		for(int j = i+1; j < n; j++)
			if( f_comp( v[i], v[j]) > 0) {
				Tree aux = v[i];
				v[i] = v[j];
				v[j] = aux;
			}
}


Tree find_manager(Tree tree, char *nume_manager){
    if(strcmp(tree->name,nume_manager)==0){
		return tree;
	}
	for(int i=0; i<tree->direct_employees_no; i++){
		Tree node = find_manager(tree->team[i],nume_manager);
		
		if(node != NULL){
			return node;
		}
	}
	return NULL;
}

Tree hire(Tree tree, char *employee_name, char *manager_name){
	Tree newNode = (Tree)malloc(sizeof(TreeNode));
    newNode->team = (Tree*)malloc(100*sizeof(Tree));
	newNode->direct_employees_no = 0;
	newNode->name = employee_name;
	
	if( tree == NULL || manager_name == NULL){
		newNode->manager=NULL;
		return newNode;
	}
	
	Tree t = find_manager(tree,manager_name);
    t->team[t->direct_employees_no]=newNode;
    t->direct_employees_no++;
	newNode->manager = t;
	return tree;
}

/* Sterge un angajat din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului concediat
 *
 * return: intoarce ierarhia modificata.
 */

Tree fire(Tree tree, char *employee_name) {
    Tree t = find_manager(tree, employee_name);
	if( t == NULL || t->manager == NULL)return tree;
	
    for(int i = 0; i < t->direct_employees_no; i++){
		t->team[i]->manager = t->manager;
		t->manager->team[t->manager->direct_employees_no] = t->team[i];
		t->manager->direct_employees_no++;
    }
	
	for(int i = 0; i<t->manager->direct_employees_no; i++)
		if( t->manager->team[i]==t){
			for(int j=i; j < t->manager->direct_employees_no-1; j++)
				t->manager->team[j]=t->manager->team[j+1];
			t->manager->direct_employees_no--;
			break;
		}
	
    free(t->team);
    free(t);
    return tree;
}

/* Promoveaza un angajat in ierarhie. Verifica faptul ca angajatul e cel putin 
 * pe nivelul 2 pentru a putea efectua operatia.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 *
 * return: intoarce ierarhia modificata.
 */
Tree promote(Tree tree, char *employee_name) {
    Tree t = find_manager(tree, employee_name);
    if(t == NULL || t->manager == NULL || t->manager->manager == NULL){
        return tree;
    }
    for(int i = 0; i < t->direct_employees_no; i++){
		t->team[i]->manager = t->manager;
		t->manager->team[t->manager->direct_employees_no] = t->team[i];
		t->manager->direct_employees_no++;
    }
	
	for(int i = 0; i<t->manager->direct_employees_no; i++)
		if( t->manager->team[i]==t){
			for(int j=i; j < t->manager->direct_employees_no-1; j++)
				t->manager->team[j]=t->manager->team[j+1];
			t->manager->direct_employees_no--;
			break;
		}
	
	t->manager->manager->team[t->manager->manager->direct_employees_no]=t;
	t->manager->manager->direct_employees_no++;
	t->manager =  t->manager->manager;
	t->direct_employees_no=0;
	
    return tree;
}

/* Muta un angajat in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_employee(Tree tree, char *employee_name, char *new_manager_name) {
    Tree t = find_manager(tree, employee_name);
    Tree m = find_manager(tree, new_manager_name);
    if(t == NULL || t->manager == NULL || !strcmp(m->name,t->manager->name)){
        return tree;
    }
    for(int i = 0; i < t->direct_employees_no; i++){
		t->team[i]->manager = t->manager;
		t->manager->team[t->manager->direct_employees_no] = t->team[i];
		t->manager->direct_employees_no++;
    }
	
	for(int i = 0; i<t->manager->direct_employees_no; i++)
		if( t->manager->team[i]==t){
			for(int j=i; j < t->manager->direct_employees_no-1; j++)
				t->manager->team[j]=t->manager->team[j+1];
			t->manager->direct_employees_no--;
			break;
		}
	
	t->direct_employees_no=0;
	m->team[m->direct_employees_no]=t;
	m->direct_employees_no++;
	t->manager =  m;
	
    return tree;
}

/* Muta o echipa in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei mutate
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_team(Tree tree, char *employee_name, char *new_manager_name) {
    Tree t = find_manager(tree, employee_name);
    Tree m = find_manager(tree, new_manager_name);
    if(t == NULL || t->manager == NULL || !strcmp(m->name,t->manager->name)){
        return tree;
    }
	
	for(int i = 0; i<t->manager->direct_employees_no; i++)
		if( t->manager->team[i]==t){
			for(int j=i; j < t->manager->direct_employees_no-1; j++)
				t->manager->team[j]=t->manager->team[j+1];
			t->manager->direct_employees_no--;
			break;
		}

	m->team[m->direct_employees_no]=t;
	m->direct_employees_no++;
	t->manager =  m;
	
    return tree;
}

/* Concediaza o echipa din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei concediate
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire_team(Tree tree, char *employee_name) {
    Tree t = find_manager(tree, employee_name);
    if(t == NULL || t->manager == NULL ){
        return tree;
    }
	
	for(int i = 0; i<t->manager->direct_employees_no; i++)
		if( t->manager->team[i]==t){
			for(int j=i; j < t->manager->direct_employees_no-1; j++)
				t->manager->team[j]=t->manager->team[j+1];
			t->manager->direct_employees_no--;
			break;
		}

	destroy_tree(t);
    return tree;
}

/* Afiseaza toti angajatii sub conducerea unui angajat.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei
 */
void print_employee(Tree tree, FILE *f){
    fprintf(f, "%s ", tree->name);
}

void add_to_queue(Tree *q, int *rear, Tree elem){
	q[*rear] = elem;
	(*rear)++;
}

void find_by_manager( Tree tree, Tree *q, int *rear, char *name) {
	Tree t = find_manager(tree, name);
	if( t!=NULL){
		add_to_queue(q,rear,t);
		for(int i=0; i<t->direct_employees_no; i++)
			find_by_manager(tree, q, rear, t->team[i]->name);
	}
}

void get_employees_by_manager(FILE *f, Tree tree, char *employee_name) {
    Tree v[100];
	int k = 0;
	
	find_by_manager(tree,v,&k,employee_name);
	sort(v, k);
	
	for(int i=0; i<k; i++)
		print_employee(v[i],f);
	fprintf(f,"\n");
}

/* Afiseaza toti angajatii de pe un nivel din ierarhie.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * level: nivelul din ierarhie
 */

void find_by_level( Tree tree, Tree *q, int *rear, char *name, int k, int level) {
	Tree t = find_manager(tree, name);
	if( t!=NULL){
		if(k == level)add_to_queue(q,rear,t);
		for(int i=0; i<t->direct_employees_no; i++)
			find_by_level(tree, q, rear, t->team[i]->name,k+1,level);
	}
}

void get_employees_by_level(FILE *f, Tree tree, int level) {
    Tree v[100];
	int k = 0;
	
	find_by_level(tree,v,&k,tree->name,0,level);
	sort(v, k);
	
	for(int i=0; i<k; i++)
		print_employee(v[i],f);
	fprintf(f,"\n");
}

/* Afiseaza angajatul cu cei mai multi oameni din echipa.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */

int get_max( Tree tree, int mx){
	if(mx < tree->direct_employees_no)mx = tree->direct_employees_no;
	for(int i=0; i<tree->direct_employees_no; i++) {
		int mxn = get_max(tree->team[i],mx);
		
		if(mxn > mx)
			mx = mxn;
	}
	return mx;
}
void find_by_max( Tree tree, Tree *q, int *rear, char *name, int mx) {
	Tree t = find_manager(tree, name);
	if( t!=NULL){
		if(t->direct_employees_no == mx)add_to_queue(q,rear,t);
		for(int i=0; i<t->direct_employees_no; i++)
			find_by_max(tree, q, rear, t->team[i]->name, mx);
	}
}

void get_best_manager(FILE *f, Tree tree){
	Tree v[100];
	int k = 0, mx = get_max( tree, 0);
	
	find_by_max(tree,v,&k,tree->name,mx);
	sort(v, k);
	
	for(int i=0; i<k; i++)
		print_employee(v[i],f);
	fprintf(f,"\n");
}

/* Reorganizarea ierarhiei cu un alt angajat in varful ierarhiei.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului care trece in varful ierarhiei
 */
Tree reorganize(Tree tree, char *employee_name){
    return tree;
}

/* Parcurge ierarhia conform parcurgerii preordine.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void print_with_manager(FILE *f, Tree tree){
    fprintf(f, "%s-%s ", tree->name, tree->manager->name);
}

void preorder_traversal(FILE *f, Tree tree){
    if(tree == NULL){
        return;
    }
    if(tree->manager == NULL){
        fprintf(f, "%s ", tree->name);
    }
    sort(tree->team, tree->direct_employees_no);
    for(int i = 0; i < tree->direct_employees_no; i++){
        print_with_manager(f, tree->team[i]);
        if(tree->team[i]->direct_employees_no > 0){
            preorder_traversal(f, tree->team[i]);
        }
    }
    if(tree->manager == NULL){
        fprintf(f ,"%s", "\n");
    }
}

/* Elibereaza memoria alocata nodurilor din arbore
 *
 * tree: ierarhia existenta
 */
void destroy_tree(Tree tree) {
    if(tree == NULL) return;
    for(int i = 0; i < tree->direct_employees_no; i++){
        destroy_tree(tree->team[i]);
    }
    free(tree->team);
    free(tree);
    tree = NULL;
    return;
}