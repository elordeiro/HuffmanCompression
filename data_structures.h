#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hufffman tree node
typedef struct leafNode
{
    int frequency;
    char character;
    struct leafNode* left;
    struct leafNode* right;
} leafNode;

// Huffman code
typedef struct huffman_code {
    int code;
    int len;
} huffman_code;

// NULL node
static leafNode null_node = {0, '\0', NULL, NULL};

// Resizeble array: 
// Doubles in size when at full capacity and halves its size when capicity is 1 / 4 full
typedef struct vector {
    leafNode* vec;
    int len;
    int cap;
} Vector;

// Max or Min heap should be specified at creation time via the "type" parameter
typedef struct Heap {
    char* type;
    Vector* vec;
    int head;
    int tail;
    int len;
    int cap;
} Heap;


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Vector API:
void     vector_push(Vector* vec, leafNode x);
char     vector_peek(Vector* vec);
char     vector_pop(Vector* vec);
void     vector_remove(Vector* vec, char x);
void     vector_set(Vector* vec, int i, leafNode x);
void     vector_print(Vector* vec);
void     vector_print_partial(Vector* vec);
void     vector_destroy(Vector* vec);
Vector*  ds_new_vector(int size);
leafNode vector_get(Vector* vec, int i);
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Heap API:
void       heap_push(Heap* Heap, leafNode content);
void       heap_print(Heap* Heap);
void       heap_print_partial(Heap* Heap);
void       heap_destroy(Heap* heap);
Heap*      ds_new_heap(char* type);
leafNode   heap_peek(Heap* Heap);
leafNode   heap_pop(Heap* Heap);
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
