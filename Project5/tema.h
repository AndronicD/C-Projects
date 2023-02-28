#ifndef TEMA_H
#define TEMA_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct carte{
    char nume;
    char simbol;
    struct carte* next;
}TCarteCel,*TCarte;

void lipire(TCarte pack1, TCarte pack2){
    TCarte iter = pack1;
    while(iter->next != NULL){
        iter = iter->next;
    }
    iter->next = pack2;
}

TCarte initCarte(char a, char b){
    TCarte head = (TCarte)malloc(sizeof(TCarteCel));
    head->next = NULL;
    head->nume = a;
    head->simbol = b;
    return head;
}

TCarte createPack(){
    char carte[13]={'2','3','4','5','6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
    char simbol[4]={'d','c','h','s'}; 
    int i, j;
	
	TCarte pack = NULL;
	
    for(i=0; i < 13; i++){
        for(j=0; j<4; j++){
            TCarte node = initCarte(carte[i],simbol[j]);
			node->next = pack;
			pack = node;
        }
    }
    return pack;
}

void printPack(TCarte pack){
    TCarte iter = pack;
    while(iter != NULL){
        printf("%c%c ", iter->nume, iter->simbol);
        iter = iter->next;
    }
}

void printPackOut(TCarte pack, FILE* outputfile){
    TCarte iter = pack;
    while(iter != NULL){
        fprintf(outputfile, "%c%c ", iter->nume, iter->simbol);
        iter = iter->next;
    }
}

TCarte freePack(TCarte pack){
    TCarte next = pack;
    TCarte t;
    while(next != NULL){
        t = next;
        next = next->next;
        free(t);
    }
    return NULL;
}

TCarte copyPack(TCarte pack){
	if(pack == NULL) return NULL;
	TCarte newCard = initCarte(pack->nume,pack->simbol);
	newCard->next=copyPack(pack->next);
	return newCard;
}

TCarte inversare(TCarte pack){
    TCarte x = pack, y = x, t, r=NULL;
    while (y != NULL)
    {
        t = y->next;
        y->next = r;
        r = y;
        y = t;
    }
    t = r;
    pack = t;
    return pack;
}

int nr_elem(TCarte pack){
    int count = 0;
    while(pack != NULL){
        count++;
        pack = pack->next;
    }
    return count;
}

TCarte intercalare(TCarte pack){
    TCarte left = copyPack(pack);
    TCarte right = left;
	
	for(int i=0; i<25; i++)
		right=right->next;
	
	TCarte temp = right;
	right = right->next;
	temp->next = NULL;
	
	freePack(pack);
	
	TCarte cl = left;
	TCarte cr = right;
	
	left = left->next;
	right = right->next;
	
	cl->next = cr;
	cr->next = NULL;
	
	pack = cl;
	temp = cr;
	
	for(int i=0; i<25; i++){
		cl = left;
		cr = right;
	
		left = left->next;
		right = right->next;
		
		cl->next = cr;
		cr->next = NULL;
		
		temp->next = cl;
		temp = cr;
		
	}
	
	return pack;
}

TCarte mongean(TCarte pack){
	TCarte left, right, aux;
	
	right = pack;
	pack = pack -> next -> next;
	left = right -> next;
	left -> next = right;
	right -> next = NULL;
	
	while(pack != NULL){
		right -> next = pack;
		right = right -> next;
		pack = pack -> next -> next;
		right -> next -> next = left;
		left = right -> next;
		right -> next = NULL;
	}
	
	return left;
}

TCarte npiles(TCarte pack, FILE* inputfile){
    int n, *v;
	TCarte *multe, aux;
	
	fscanf(inputfile,"%d",&n);
	
	v = (int*)malloc(n*sizeof(int));
	multe = (TCarte*)malloc(n*sizeof(TCarte));
	
	for(int i=0; i<n; i++){
		fscanf(inputfile,"%d",&v[i]);
		multe[i] = NULL;
	}
	
	pack = inversare(pack);
	
	for(int i=n-1; i>=0; i--){
		for(int j=0; j<52/n; j++){
			aux = pack;
			pack = pack->next;
			aux->next=multe[i];
			multe[i]=aux;
		}
	}
	
	for(int i=1; i<n; i++)
		lipire(multe[v[i-1]-1],multe[v[i]-1]);
	pack = multe[v[0]-1];
	
	free(v); free(multe);
	return pack;
}

#endif 