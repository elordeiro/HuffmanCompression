#include "data_structures.h"

static void swim(Heap** Heap, int index) {
    if (index == 1) return;
    leafNode current = vector_get((*Heap)->vec, index);
    int current_frequency = vector_get((*Heap)->vec, index).frequency;
    
    leafNode parent = vector_get((*Heap)->vec, index / 2);
    int parent_frequency = vector_get((*Heap)->vec, index / 2).frequency;
    
    if (strcmp(((*Heap)->type), "max") == 0) {
        if (current_frequency > parent_frequency) {
            leafNode temp = current;
            vector_set((*Heap)->vec, index, parent);
            vector_set((*Heap)->vec, index / 2, temp);
            swim(Heap, (index / 2));
        }
    } else {
        if (current_frequency < parent_frequency) {
            leafNode temp = current;
            vector_set((*Heap)->vec, index, parent);
            vector_set((*Heap)->vec, index / 2, temp);
            swim(Heap, (index / 2));
        }
    }
    return;
    
}

static void sink(Heap** Heap, int index) {
    if (index * 2 > (*Heap)->tail) return;
    leafNode parent = vector_get((*Heap)->vec, index);
    int parent_frequency = vector_get((*Heap)->vec, index).frequency;
    
    leafNode left_child = vector_get((*Heap)->vec, index * 2);
    int left_child_frequency = vector_get((*Heap)->vec, index * 2).frequency;
    
    leafNode right_child = vector_get((*Heap)->vec, index * 2 + 1);
    int right_child_frequency = vector_get((*Heap)->vec, index * 2 + 1).frequency;
    
    if (strcmp((*Heap)->type, "max") == 0) {
        if (parent_frequency > left_child_frequency && parent_frequency > right_child_frequency) return;
        if (left_child_frequency > right_child_frequency) {
            leafNode temp = parent;
            vector_set((*Heap)->vec, index, left_child);
            vector_set((*Heap)->vec, index * 2, temp);
            sink(Heap, (index * 2));
        } else {
            leafNode temp = parent;
            vector_set((*Heap)->vec, index, right_child);
            vector_set((*Heap)->vec, index*2 + 1, temp);
            sink(Heap, (index*2 + 1));
        }
    } else {
        if (parent_frequency < left_child_frequency && parent_frequency < right_child_frequency) return;
        if (left_child_frequency < right_child_frequency) {
            leafNode temp = parent;
            vector_set((*Heap)->vec, index, left_child);
            vector_set((*Heap)->vec, index * 2, temp);
            sink(Heap, (index * 2));
        } else {
            leafNode temp = parent;
            vector_set((*Heap)->vec, index, right_child);
            vector_set((*Heap)->vec, index * 2 + 1, temp);
            sink(Heap, (index*2 + 1));
        }
    }
    return;
}

leafNode heap_peek(Heap* Heap) {
    return vector_get(Heap->vec, Heap->head);
}

void heap_push(Heap* Heap, leafNode content) {
    if (Heap->head < 0) {
        vector_push(Heap->vec, null_node);
        Heap->head = 1;
        Heap->tail = 1;
        Heap->len = 1;
        vector_push(Heap->vec, content);
        return;
    }
    Heap->tail++;
    Heap->len++;
    vector_push(Heap->vec, content);
    swim(&Heap, Heap->tail);
    return;
}

leafNode heap_pop(Heap* Heap) {
    leafNode max_or_min = vector_get(Heap->vec, Heap->head);
    vector_set(Heap->vec, Heap->head, vector_get(Heap->vec, Heap->tail--));
    vector_pop(Heap->vec);
    sink(&Heap, Heap->head);
    Heap->len--;
    return max_or_min;
}

void heap_print_partial(Heap* Heap) {
    if (Heap == NULL) return;
    if (Heap->vec->len <= 10) {
        heap_print(Heap);
        return;
    }
    printf("[");
    for (int i = 1; i < 6; i++) {
        printf("%c, ", vector_get(Heap->vec, i).character);
    }
    printf("... , ");
    for (int i = Heap->tail - 4; i < Heap->tail; i++) {
        printf("%c, ", vector_get(Heap->vec, i).character);
    }
    printf("%c]\n", vector_get(Heap->vec, Heap->tail).character);
    return;
}

void heap_print(Heap* Heap) {
    if (Heap == NULL) return;
    printf("[");
    for (int i = 1; i < Heap->tail; i++) {
        printf("%c, ", Heap->vec->vec[i].character);
    }
    printf("%c]\n", vector_get(Heap->vec, Heap->tail).character);
    return;
}

void heap_destroy(Heap* heap) {
    vector_destroy(heap->vec);
    free(heap);
    heap = NULL;
    return;
}

Heap* ds_new_heap(char* type) {
    Heap* new_Heap = malloc(sizeof(Heap));
    new_Heap->type = type;
    new_Heap->vec = ds_new_vector(1024);
    new_Heap->head = -1;
    new_Heap->tail = -1;
    return new_Heap;
}
