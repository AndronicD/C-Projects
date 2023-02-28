#ifndef ORDERED_SET_H
#define ORDERED_SET_H

#include <stdio.h>                                          // needed for printf
#include <stdlib.h>                           // needed for alloc, realloc, free

/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------

/*
  OrderedSet represents a set with elements of type T stored in memory
  in an ordered fashion.
*/

typedef struct OrderedSet {
  T* elements;                    // pointer to where the elements are in memory
  long size;                                // the number of elements in the set
  long capacity;                      // the current capacity (allocated memory)
} OrderedSet;

// -----------------------------------------------------------------------------

/*
  Function that creates an OrderedSet with elements of type T with a given
  initial capacity.
*/

OrderedSet* createOrderedSet(long initialCapacity) {

  OrderedSet* newSet = (OrderedSet*) malloc(sizeof(OrderedSet));

  newSet->size = 0;                        // initially, the set is empty
  newSet->capacity = initialCapacity;
  newSet->elements = (T*) malloc(initialCapacity * sizeof(T));

  return newSet;
}

// -----------------------------------------------------------------------------

/*
  Function that checks if an element exists in a given set. As the
  elements of the set are ordered in memory, this function uses binary
  search.

  Function returns 1 if the given element exists in the set, and 0
  otherwise.
*/

int contains(OrderedSet* set, const T element) {
  long left = 0;                                 // start of the search interval
  long right = set->size - 1;                      // end of the search interval
  long middle;                                  // middle of the search interval

  while (left <= right) {              // while the search interval is not empty
    middle = left + (right - left) / 2;    // find middle between left and right
    if (set->elements[middle] == element)
      return 1;
    if (set->elements[middle] < element)
      left = middle + 1;            // search continues to the left from $middle
    else
      right = middle - 1;          // search continues to the right from $middle
  }
  return 0;
}

// -----------------------------------------------------------------------------

/*
  Function that adds a new element to a set. If the element was
  already in the set, nothing changes. Otherwise, the element is added
  to the set. In the end, the elements must be in ascending order.
*/
void doubleIfFull(OrderedSet* set) {
  if (set->size == set->capacity) {
    set->capacity = set->capacity * 2;
    set->elements = (T*) realloc(set->elements, set->capacity * sizeof(T));
  }
}

void add(OrderedSet* set, const T newElement) {
  if (contains(set, newElement))
    return;

  doubleIfFull(set);                 

  set->elements[set->size] = newElement;
  set->size++;

  long idx = set->size - 1;
  while (idx > 0 && set->elements[idx - 1] > newElement) {
    set->elements[idx] = set->elements[idx - 1];
    idx--;
  }
  set->elements[idx] = newElement;
}

// -----------------------------------------------------------------------------

/*
  This function takes two ordered sets with elements of type T and it
  returns the addres of a new ordered set representing the union of
  the two
*/

OrderedSet* unionOrderedSets(OrderedSet* s1, OrderedSet* s2) {
  OrderedSet* s = createOrderedSet(s1->size + s2->size);

  long idx = 0l, idx1 = 0l, idx2 = 0l;

  while (idx1 < s1->size && idx2 < s2->size) { // while elements in both s1 & s2
    if (s1->elements[idx1] < s2->elements[idx2])        // add the smallest to s
      s->elements[idx++] = s1->elements[idx1++];
    else if (s1->elements[idx1] > s2->elements[idx2])
      s->elements[idx++] = s2->elements[idx2++];
    else
      idx2++;
  }
  while (idx1 < s1->size)                      // add the rest of elements in s1
    s->elements[idx++] = s1->elements[idx1++];

  while (idx2 < s2->size)                      // add the rest of elements in s2
    s->elements[idx++] = s2->elements[idx2++];

  s->size = idx;
  return s;
}


OrderedSet* _unionOrderedSets(OrderedSet* s1, OrderedSet* s2) {
  long idx;
  OrderedSet* s = createOrderedSet(s1->size + s2->size);

  for (idx = 0; idx < s1->size; idx++) add(s, s1->elements[idx]);
  for (idx = 0; idx < s2->size; idx++) add(s, s2->elements[idx]);

  return s;
}

// -----------------------------------------------------------------------------

/*
  This function takes two ordered sets with elements of type T and it
  returns the addres of a new ordered set representing the
  intersection of the two
*/

OrderedSet* intersectOrderedSets(OrderedSet* s1, OrderedSet* s2) {
  long idx = 0l, idx1 = 0l, idx2 = 0l;

  OrderedSet* s = createOrderedSet(s1->size);

  while (idx1 < s1->size && idx2 < s2->size) {
    if (s1->elements[idx1] == s2->elements[idx2]) {
      s->elements[idx] = s1->elements[idx1];
      idx++; idx1++; idx2++;
    } else if (s1->elements[idx1] < s2->elements[idx2]) {
      idx1++;        // skip 1st element in s1 if smaller than 1st element in s2
    } else {
      idx2++;        // skip 1st element in s2 if smaller than 1st element in s1
    }
  }

  s->size = idx;
  return s;
}

OrderedSet* _intersectOrderedSets(OrderedSet* s1, OrderedSet* s2) {
  long idx = 0l;

  OrderedSet* s = createOrderedSet(s1->size);

  for (idx = 0l; idx < s1->size; idx++)
    if (contains(s2, s1->elements[idx]))
      add(s, s1->elements[idx]);

  return s;
}


// -----------------------------------------------------------------------------

#endif
