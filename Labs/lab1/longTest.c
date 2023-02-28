#include <stdio.h>
#include <string.h>

typedef long T;
#include "OrderedSet.h"

OrderedSet* interval(long inf, long sup, long numar)
{
    long idx, count=0;
    for(idx = inf; idx <= sup; idx++)
    {
        if(idx % numar == 0)
        count++;
    }
    OrderedSet* intervalM = createOrderedSet(count);
    for(idx = inf; idx <= sup; idx++)
    {
        if(idx % numar == 0)
        add(intervalM, idx);
    }
    return intervalM;
}

void printOrderedLongSet(OrderedSet* intervalM) {
  long idx;
  printf("There are %ld numbers:", intervalM->size);                 
  for(idx = 0; idx < intervalM->size; idx++)
    printf(" %ld", intervalM->elements[idx]);                     
  printf("\n");
}

int main(int argc, char* argv[]) {
  OrderedSet *i1, *i2, *iU, *iI;

  i1 = interval(4,25,3);
  printOrderedLongSet(i1);

  i2 = interval(5,30,4);
  printOrderedLongSet(i2);

  iU = unionOrderedSets(i1, i2);
  printOrderedLongSet(iU);

  iI = intersectOrderedSets(i1, i2);
  printOrderedLongSet(iI);

  return 0;
}