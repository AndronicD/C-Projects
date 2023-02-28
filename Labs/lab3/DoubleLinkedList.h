#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_
#include <stdlib.h>
/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------
/**
 *  Linked list representation -- R2 Curs2 (Slide 8)
 */
typedef struct ListNode{
	Item elem; // Stored node value
	struct ListNode* next; // link to next node
	struct ListNode* prev; // link to prev node
} ListNode;

/**
 *  Double linked list representation -- desen Curs 3 (Slide 19)
 */
typedef struct List{
	ListNode* first; // link to the first node
	ListNode* last; // link to the last node
}List;
// -----------------------------------------------------------------------------

/**
 * Create a new node
 *  [input]: Item
 *  [output]: List*
 */
ListNode *createNode(Item elem) {
	ListNode* node = (ListNode*)malloc(sizeof(ListNode));
	node->elem = elem;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

/**
 * Create a new (empty)  list  -- Silde-urile 13/14 Curs 2
 *  [input]: None
 *  [output]: List*
 */
List* createList(void){
	List* list = (List*)malloc(sizeof(List));
	list->first = NULL;
	list->last = NULL;
	return list;
}
// -----------------------------------------------------------------------------


/**
 * Determines if the list is empty
 *  [input]: List*
 *  [output]: 1 - empty/ 0 - not empty
 */
int isEmpty(List *list){
	return (list == NULL) || (list->first == NULL);
}
// -----------------------------------------------------------------------------


/**
 * Determines if a list contains a specified element
 *  [input]: List*, Item
 *  [output]: int (1 - contains/ 0 - not contains)
 */
int contains(List *list, Item elem){
	if(isEmpty(list)) return 0;
	ListNode* iter = list->first;
	while(iter != NULL){
		if(iter->elem == elem)
			return 1;
		iter = iter->next;
	}
	return 0;
}
// -----------------------------------------------------------------------------


/**
 * Insert a new element in the list at the specified position.
 * Note: Position numbering starts with the position at index zero
 *  [input]: List*, Item, int
 *  [output]: void
 */
void insertAt(List* list, Item elem, int pos){

	// Guard against young player errors
	if(list == NULL) return;
	
	ListNode* iter = list->first;
	
	while(pos && iter !=NULL){
		pos--;
		iter = iter->next;
	}
	if(pos == 0){
		ListNode* node = createNode(elem);
		
		if(isEmpty(list)){
			list->first = list->last = node;
			return;
		}

		if(iter == NULL){
			node->next = NULL;
			node->prev = list->last;
			list->last->next = node;
			list->last = node;
			return;
		}

		if(iter->prev == NULL){
			node->next = iter;
			node->prev = NULL;
			iter->prev = node;
			list->first = node;
			return;
		}

		node->next = iter;
		node->prev = iter->prev;
		iter->prev->next = node;
		iter->prev = node;
	}
	return;
	// TODO: Cerinta 1d
}
// -----------------------------------------------------------------------------


/**
 * Delete the first element instance form a list.
 *  [input]: List*, Item
 *  [output]: void
 */

void deleteOnce(List *list, Item elem){
// Guard against young player errors
if(list == NULL) return;
ListNode* iter = list->first;
	while(iter != NULL){
        if(iter->elem == elem){
            if(iter->prev == NULL){
                if(iter->next == NULL){
                    list->first=NULL;
                    list->last=NULL;
                    free(iter);
                    return;
                }
                
                iter->next->prev = NULL;
                list->first = iter->next;
                free(iter);
                return;
            }

            if(iter->next == NULL){
                iter->prev->next = NULL;
                list->last = iter->prev;
                free(iter);
                return;
            }
            
            iter->prev->next = iter->next;
            iter->next->prev = iter->prev;
            free(iter);
            return;
        }
		iter = iter->next;
	}

}

// -----------------------------------------------------------------------------


/**
 * Compute list length
 *  [input]: List*
 *  [output]: int
 */
int length(List *list){
	// Guard against young player errors
	if(list == NULL) return 0;
	int count = 0;
	ListNode* iter = list->first;
	while(iter != NULL){
		count+=1;
		iter = iter->next;
	}
	return count;
}
// -----------------------------------------------------------------------------



/**
 * Destroy a list.
 *  [input]: List*
 *  [output]: void
 */
List* destroyList(List* list){
	// Guard against young player errors
	if(list == NULL) return NULL;
	ListNode* iter = list->first;
	while(iter != NULL){
		ListNode* t = iter;
		//iter->next->prev = NULL;
		//list->first->next = iter->next;
		//list->first = iter->next;
		iter = iter->next;
		free(t);
	}
	list->first = NULL;
	list->last = NULL;
	free(list);
	return list;
}
// -----------------------------------------------------------------------------


#endif //_DOUBLE_LINKED_LIST_H_
