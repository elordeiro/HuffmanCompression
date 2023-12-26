#include "data_structures.h"
#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i)    \
    (((i) & 0x80ll) ? '1' : '0'), \
    (((i) & 0x40ll) ? '1' : '0'), \
    (((i) & 0x20ll) ? '1' : '0'), \
    (((i) & 0x10ll) ? '1' : '0'), \
    (((i) & 0x08ll) ? '1' : '0'), \
    (((i) & 0x04ll) ? '1' : '0'), \
    (((i) & 0x02ll) ? '1' : '0'), \
    (((i) & 0x01ll) ? '1' : '0')

#define PRINTF_BINARY_PATTERN_INT16 \
    PRINTF_BINARY_PATTERN_INT8              PRINTF_BINARY_PATTERN_INT8
#define PRINTF_BYTE_TO_BINARY_INT16(i) \
    PRINTF_BYTE_TO_BINARY_INT8((i) >> 8),   PRINTF_BYTE_TO_BINARY_INT8(i)
#define PRINTF_BINARY_PATTERN_INT32 \
    PRINTF_BINARY_PATTERN_INT16             PRINTF_BINARY_PATTERN_INT16
#define PRINTF_BYTE_TO_BINARY_INT32(i) \
    PRINTF_BYTE_TO_BINARY_INT16((i) >> 16), PRINTF_BYTE_TO_BINARY_INT16(i)
#define PRINTF_BINARY_PATTERN_INT64    \
    PRINTF_BINARY_PATTERN_INT32             PRINTF_BINARY_PATTERN_INT32
#define PRINTF_BYTE_TO_BINARY_INT64(i) \
    PRINTF_BYTE_TO_BINARY_INT32((i) >> 32), PRINTF_BYTE_TO_BINARY_INT32(i)

// define DEBUG to 1 to enable debug output
#define DEBUG 0

// Helpful command
// xxd -b huffman.out > huffman.txt

// Globals -----------------------------------------------------------------------------------------
Heap* heap;                                    // min priority queue
int frequencies[256] = {0};                    // frequencies of each character
// huffman_code is a struct containing the encoding and the length of the encoding
huffman_code char_encoding[256] = {0};         // huffman encoding of each character 
int char_count;                                // number of unique chars in file
int bytes_needed;                              // number of bytes needed to store largest frequency
int b_include_freq = 0;                        // (bool) include frequency table in output file
char* input_file = "completeShakespeare.txt";  // default input file
char* output_file = "huffman.out";             // default output file
//-------------------------------------------------------------------------------------------------

// helper function
void node_copy (leafNode* src, leafNode* dest) {
    dest->frequency = src->frequency;
    dest->character = src->character;
    dest->left = src->left;
    dest->right = src->right;
    return;
}

void read_file (FILE *fp) {
    char c;
    while ((c = fgetc(fp)) != EOF) {
        frequencies[(int) c]++;
    }
    fclose(fp);
    if (DEBUG) {
        printf("\nFrequencies:\n");
        for (int i = 0; i < 256; i++) {
            if (frequencies[i] > 0) {
                printf("%c: %d\n", i, frequencies[i]);
            }
        }
    }
}

// Note: encoding is stored in reverse order (i.e. 0101 is stored as 1010)
void map_characters_recursive (leafNode* root, int encoding, int encoding_len) {
    if (root == NULL) return;
    if (root->character != '\0') {
        if (root->character == '4') printf("Here\n");
        char_encoding[(int) root->character].code = encoding;
        char_encoding[(int) root->character].len = encoding_len;
    }
    map_characters_recursive(root->left, (encoding << 1), encoding_len + 1);
    map_characters_recursive(root->right, (encoding << 1) + 1, encoding_len + 1);
    return;
}

void map_characters (leafNode* root, int encoding, int encoding_len) {
    map_characters_recursive(root, encoding, encoding_len);
    if (DEBUG) {
        printf("\nCharacter Encoding Hex:\n");
        for (int i = 0; i < 256; i++) {
            if (char_encoding[i].len > 0) {
                printf("%c: %4x size: %d\n", i, char_encoding[i].code, char_encoding[i].len);
            }
        }
        printf("\nCharacter Encoding Binary:\n");
        for (int i = 0; i < 256; i++) {
            if (char_encoding[i].len > 0) {
                printf("%c: " PRINTF_BINARY_PATTERN_INT32 " size: %d\n", i, PRINTF_BYTE_TO_BINARY_INT32(char_encoding[i].code), char_encoding[i].len);
            }
        }
    }
}

// Calculates the number of bytes needed to store the largest frequency
void calc_bytes_needed () {
    int largest = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > largest) {
            largest = frequencies[i];
        }
    }
    int bytes = -1;
    while (largest > 0) {
        bytes++;
        largest = largest >> 8;
    }
    bytes_needed = bytes + 1;
}

// Calculates the number of unique characters in the frequency table
void calc_char_count () {
    int count = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            count++;
        }
    }
    char_count = count;
}

// Writes the frequency table to the output file if b_include_freq is true
// Format:
// 0x00 0xFF char_count bytes_needed
// char1 frequency1 (only the [bytes_needed] most significant bytes are written)
// char2 frequency2 (only the [bytes_needed] most significant bytes are written)
// ...
void include_freq (FILE *dest) {
    fputc(0x00, dest);
    fputc(0xFF, dest);
    calc_bytes_needed();
    calc_char_count();
    fputc(char_count, dest);
    fputc(bytes_needed, dest);

    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            fputc(i, dest);
            for (int j = 0; j < bytes_needed; j++) {
                fputc(frequencies[i] >> (8 * (bytes_needed - j - 1)), dest);
            }
        }
    }
}

// Encodes the input file and writes the encoded data to the output file
void write_file (FILE *src, FILE *dest) {
    if (b_include_freq)
        include_freq(dest);
    int buffer = 0;
    int buffer_size = 0;
    char c;
    while ((c = fgetc(src)) != EOF) {
        buffer = buffer << char_encoding[(int) c].len;
        buffer += char_encoding[(int) c].code;
        buffer_size += char_encoding[(int) c].len;
        while (buffer_size >= 8) {
            if (buffer_size > 32) {
                printf("Error: buffer overflow\n");
                printf("Last character: %c\n", c);
                printf("buffer: " PRINTF_BINARY_PATTERN_INT32 "\n", PRINTF_BYTE_TO_BINARY_INT32(buffer));
            }
            int overflow = buffer_size - 8;
            int mask = 0;
            for (int i = 0; i < overflow; i++) {
                mask = (mask << 1) + 1;
            }
            fputc(buffer >> overflow, dest);
            buffer = buffer & mask;
            buffer_size = overflow;
        }
    }
    if (buffer_size > 0) {
        buffer = buffer << (8 - buffer_size);
        fputc(buffer, dest);
    }
    fclose(src);
    fclose(dest);
}

// Builds the priority queue from the frequency table
void build_priority_queue () {
    heap = ds_new_heap("min");
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            leafNode *newNode = malloc(sizeof(struct leafNode));
            newNode->frequency = frequencies[i];
            newNode->character = i;
            newNode->left = NULL;
            newNode->right = NULL;
            heap_push(heap, *newNode);
        }
    }
    if (DEBUG) {
        printf("\nPrinting Heap:\n");
        heap_print(heap);
    }
}

void print_huffman_tree (leafNode* root) {
    if (root == NULL) return;
    if (root->character != '\0') {
        printf("Node: %c, Freq: %d\n", root->character, root->frequency);
    }
    else {
        printf("NULL: %d\n", root->frequency);
    }
    print_huffman_tree(root->left);
    print_huffman_tree(root->right);
    return;
}

// Builds the huffman tree from the priority queue
// Root of the tree is stored at heap->vec->vec[1]
void build_huffman_tree (Heap* heap) {
    while (heap->len > 1) {
        leafNode parent = null_node;
        leafNode temp;
        parent.left = malloc(sizeof(leafNode));
        parent.right = malloc(sizeof(leafNode));

        temp = heap_pop(heap);
        node_copy(&temp, parent.left);
        temp = heap_pop(heap);
        node_copy(&temp, parent.right);

        parent.frequency = parent.left->frequency + parent.right->frequency;
        heap_push(heap, parent);
    }
    if (DEBUG) {
        printf("\nPrinting Huffman Tree:\n");
        printf("NULL represents an internal node\n\n");
        print_huffman_tree(&heap->vec->vec[1]);
    }
    return;
}

// Reads the header of the encoded file and builds the frequency table
void read_header (FILE* src) {
    int byte1 = fgetc(src);
    int byte2 = fgetc(src);
    if (byte1 != 0x00 || byte2 != 0xFF) {
        printf("Error: File is not encoded with huffman\n");
        exit(1);
    }
    char_count = fgetc(src);
    bytes_needed = fgetc(src);
    for (int i = 0; i < char_count; i++) {
        int c = fgetc(src);
        int freq = 0;
        for (int j = 0; j < bytes_needed; j++) {
            freq = (freq << 8) + fgetc(src);
        }
        frequencies[c] = freq;
    }
    if (DEBUG) {
        printf("\nFrequencies:\n");
        for (int i = 0; i < 256; i++) {
            if (frequencies[i] > 0) {
                printf("%c: %d\n", i, frequencies[i]);
            }
        }
    }
}

// Decodes the input file and writes the decoded data to the output file
void write_decoded_file (FILE* src, FILE* dest) {
    leafNode* root = &heap->vec->vec[1];
    leafNode* curr = root;
    int buffer = 0;
    int mask = 0x80;
    while ((buffer = fgetc(src)) != EOF) {
        for(int i = 0; i < 8; i++) {
            int bit = buffer & mask;
            buffer = buffer << 1;
            if (bit == 0) {
                if (curr->left == NULL) {
                    printf("Error: Invalid encoding\n");
                }
                else {
                    curr = curr->left;
                }
            }
            else {
                if (curr->right == NULL) {
                    printf("Error: Invalid encoding\n");
                }
                else {
                    curr = curr->right;
                }
            } 
            if (curr->character != '\0') {
                fputc(curr->character, dest);
                curr = root;
            }
        }
    }
    fclose(src);
    fclose(dest);
}

// main function for decoding
void decode(FILE* src, FILE* dest) {
    printf("Decoding...\n");
    read_header(src);
    build_priority_queue();
    build_huffman_tree(heap);
    map_characters(&heap->vec->vec[1], 0, 0);
    write_decoded_file(src, dest);
    printf("Done!\n");
    printf("use `diff completeShakespeare.txt huffman_decoded.txt` to compare files\n");
}

// Reads command line arguments
// If -d is specified, decode the file and exit 
void read_command_line_args(int argc, char *argv[]) {
    int to_decode = 0;
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'h':
                    printf("Usage: ./huffman [options]\n");
                    printf("Options:\n");
                    printf("\t-h: Display this help message\n");
                    printf("\t-d: Decode a file (requires frequency table in header. Encode using '-f')\n");
                    printf("\t-f: Include frequency table in output file\n");
                    printf("\t-i: Specify input file (Default w/out -d flag: \"completeShakespeare.txt\") (Default w/ -d flag: \"huffman.out\")\n");
                    printf("\t-o: Specify output file (Default w/out -d flag: \"huffman.out\") (Default w/ -d flag: \"huffman_decoded.txt\")\n");
                    exit(0);
                    break;
                case 'd':
                    to_decode = 1;
                    input_file = "huffman.out";
                    output_file = "huffman_decoded.txt";
                    break;
                case 'f':
                    b_include_freq = 1;
                    break;
                case 'i':
                    input_file = argv[i+1];
                    break;
                case 'o':
                    output_file = argv[i+1];
                    break;
            }
        }
    }
    if (to_decode) {
        decode(fopen(input_file, "r"), fopen(output_file, "w"));
        exit(0);
    }
    return;
}

// main function for encoding
int main (int argc, char *argv[]) {
    read_command_line_args(argc, argv);
    read_file(fopen(input_file, "r"));
    build_priority_queue();
    build_huffman_tree(heap);
    map_characters(&heap->vec->vec[1], 0, 0);
    write_file(fopen(input_file, "r"), fopen(output_file, "w"));
    return 0;
}
