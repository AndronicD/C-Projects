#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <string.h>

typedef struct {
    unsigned char value;
    int left, right;
} HuffmanNode;

#define Item int

#include "heap.h"

void computeFreqs(char *text, int size, int freqs[256]) {
    int i;
    for (i = 0; i < 256; ++i) {
        freqs[i] = 0;
    }
    for (i = 0; i < size; ++i) {
        freqs[(int) text[i]]++;
    }
}

HuffmanNode *makeTree(int freqs[256], int *size) {
    int i;
    APriQueue pq = makeQueue(256);
    HuffmanNode *nodes = NULL;
    ItemType item, n1, n2;

    for (i = 0; i < 256; ++i) {
        if (freqs[i]) {
            nodes = (HuffmanNode *) realloc(nodes, (*size + 1) * sizeof(HuffmanNode));
            nodes[*size].left = nodes[*size].right = -1;
            nodes[*size].value = (unsigned char) i;

            item.content = *size;
            item.prior = freqs[i];
            insert(pq, item);

            ++(*size);
        }
    }

    if (*size == 1) {
        nodes = (HuffmanNode *) realloc(nodes, (*size + 1) * sizeof(HuffmanNode));
        nodes[*size].left = 0; // Link to prev node.
        nodes[*size].right = -1;
        nodes[*size].value = (unsigned char) 0; // Does not matter.

        ++(*size);
    }

    while (pq->size > 1) {
        n1 = removeMin(pq);
        n2 = removeMin(pq);

        item.prior = n1.prior + n2.prior;
        item.content = *size;

        nodes = (HuffmanNode *) realloc(nodes, (*size + 1) * sizeof(HuffmanNode));
        nodes[*size].value = 0; // Internal node, does not matter.
        nodes[*size].left = n1.content;
        nodes[*size].right = n2.content;

        ++(*size);
        insert(pq, item);
    }

    return nodes;
}

void makeCodes(int index, HuffmanNode *nodes, char **codes, char *code) {

    if (nodes[index].left == -1 && nodes[index].right == -1) {
        codes[nodes[index].value] = strdup(code);
    }

    if (nodes[index].left != -1) {
        // left means 0
        makeCodes(nodes[index].left, nodes, codes, strcat(code, "0"));
        // delete last 0
        code[strlen(code) - 1] = '\0';
    }

    if (nodes[index].right != -1) {
        // right means 1
        makeCodes(nodes[index].right, nodes, codes, strcat(code, "1"));
        // delete last 1
        code[strlen(code) - 1] = '\0';
    }
}

char *compress(char *text, int textSize, HuffmanNode *nodes, int numNodes) {
    char *compressed = (char *) malloc(20000 * sizeof(char));
    compressed[0] = '\0';

    if (!textSize) {
        return compressed;
    }

    char code[100] = {0};
    char **codes = (char **) calloc(256, sizeof(char *));

    makeCodes(numNodes - 1, nodes, codes, code);


    int i;
    for (i = 0; i < textSize; ++i) {
        strcat(compressed, codes[(unsigned int) text[i]]);
    }

    return compressed;
}

char *decompress(char *text, int textSize, HuffmanNode *nodes, int numNodes) {
    int rootNode = numNodes - 1;

    char *decompressed = (char *) malloc(20000 * sizeof(char));
    decompressed[0] = '\0';

    if (!textSize) {
        return decompressed;
    }

    int k = 0;
    int idx = rootNode;
    char symbol;
    for (int i = 0; i < textSize; ++i) {
        symbol = text[i];

        if (symbol == '0') {
            idx = nodes[idx].left;
        } else {
            idx = nodes[idx].right;
        }

        // leaf?
        if (nodes[idx].left == -1 && nodes[idx].right == -1) {
            decompressed[k++] = nodes[idx].value;
            idx = rootNode;
        }
    }

    decompressed[k] = '\0';
    return decompressed;
}

#endif
