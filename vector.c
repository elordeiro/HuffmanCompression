#include "data_structures.h"

static void double_vec_cap(Vector* vec) {
    leafNode* new_ptr = realloc(vec->vec, (sizeof(leafNode) * (u_int64_t) vec->cap * 2));
    if (new_ptr == NULL) {
        printf("Error: realloc failed in vector_double_vec_cap\n");
    }
    else {
        vec->vec = new_ptr;
        vec->cap *= 2;
    }
    
    return;
}

static void halve_vec_cap(Vector* vec) {
    leafNode* new_ptr = realloc(vec->vec, (sizeof(leafNode) * (u_int64_t) vec->cap / 2));
    if (new_ptr == NULL) {
        printf("Error: realloc failed in vector_halve_vec_cap\n");
    }
    else {
        vec->vec = new_ptr;
        vec->cap /= 2;
    }
    return;
}

void vector_push(Vector* vec, leafNode x) {
    if (vec == NULL) {
        vec = ds_new_vector(1024);
    } else if (vec->cap == vec->len) {
        double_vec_cap(vec);
    }
    vec->vec[vec->len] = x;
    vec->len++;
    return;
}

char vector_peek(Vector* vec) {
    char peek = vec->vec[vec->len - 1].character;
    return peek;
}


char vector_pop(Vector* vec) {
    char tail = vec->vec[vec->len - 1].character;
    vec->len--;
    if (vec->cap > 1024 && vec->len <= (vec->cap / 4)) {
        halve_vec_cap(vec);
    }
    return tail;
}

void vector_print(Vector* vec) {
    printf("[");
    for (int i = 0; i < vec->len - 1; i++) {
        printf("%c, ", vec->vec[i].character);
    }
    printf("%c]\n", vec->vec[vec->len - 1].character);
    return;
}

void vector_print_partial(Vector* vec) {
    if (vec->len <= 10) {
        vector_print(vec);
        return;
    }
    printf("[");
    for (int i = 0; i < 5; i++) {
        printf("%c, ", vec->vec[i].character);
    }
    printf("... , ");
    for (int i = vec->len - 5; i < vec->len - 1; i++) {
        printf("%c, ", vec->vec[i].character);
    }
    printf("%c]\n", vec->vec[vec->len - 1].character);
    return;
}

void vector_remove(Vector* vec, char x) {
    int i = 0;
    while (i < vec->len) {
        if (vec->vec[i].character == x) {
            for (int j = i; j < vec->len - 1; j++) {
                vec->vec[j] = vec->vec[j + 1];
            }
            vec->len--;
            if (vec->cap > 1024 && vec->len <= (vec->cap / 4)) {
                halve_vec_cap(vec);
            }
        } else {
            i++;
        }
    }
    return;
}

leafNode vector_get(Vector* vec, int i) {
    if (i > vec->len) {
        printf("Error: index out of bounds in vector_get\n");
        return null_node;
    }
    return (vec->vec[i]);
}

void vector_set(Vector* vec, int i, leafNode x) {
    if (i > vec->len) return;
    vec->vec[i] = x;
}

void vector_destroy(Vector* vec) {
    free(vec->vec);
    vec->vec = NULL;
    free(vec);
    vec = NULL;
    return;
}

Vector* ds_new_vector(int size) {
    Vector* new_vec = malloc(sizeof(Vector));
    new_vec->vec =  malloc(size * sizeof(leafNode));
    new_vec->len = 0;
    new_vec->cap = size;
    return new_vec;
}
