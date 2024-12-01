#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

typedef struct node {
    int val;
    struct node *prev;
} node_t;

typedef struct {
    node_t *start;
    int size;
} tree_t;

typedef struct {
    int *array;
    int size;
} diff_t;

node_t *create_node(int val) {
    node_t* new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("new node failed");
        return NULL;
    }
    new_node->val = val;
    new_node->prev = NULL;
    return new_node;
}

tree_t *init_tree() {
    tree_t *tree = malloc(sizeof(tree_t));
    if (tree == NULL) {
        perror("tree failed");
        return NULL;
    }
    tree->start = NULL;
    tree->size = 0;
    return tree;
}

void add_node_to(node_t **node, node_t *new_node) {
    node_t *current_node = *node;
    if (current_node == NULL) {
        *node = new_node;
    } else if (current_node->val < new_node->val) {
        new_node->prev = *node;
        *node = new_node;
    } else {
        add_node_to(&current_node->prev, new_node);        
    }
}

int add_val(tree_t *tree, int val) {
    node_t *new_node = create_node(val);
    if (new_node == NULL) {
        return -1;
    }
    add_node_to(&tree->start, new_node);
    tree->size += 1;
    return 0;
}

int get_val(tree_t *tree) {
    node_t *current_node = tree->start;
    int ret_val = current_node->val;
    tree->start = current_node->prev;
    free(current_node);
    tree->size -= 1;
    return ret_val;
}

void print_trees(tree_t *tree_left, tree_t *tree_right) {
    node_t *left_node = tree_left->start;
    node_t *right_node = tree_right->start; 
    while (left_node != NULL | right_node != NULL) {
        printf("%d\t%d\n", left_node->val, right_node->val);
        left_node = left_node->prev;
        right_node = right_node->prev;
    }
}

void free_tree(tree_t *tree) {
    node_t *current_node = tree->start;
    while (current_node != NULL) {
        node_t *remove_node = current_node;
        current_node = current_node->prev;
        free(remove_node);
    }
    free(tree);
}

int *get_freqs(tree_t *tree) {
    node_t *current_node = tree->start;
    int *arr = calloc(current_node->val, sizeof(int));
    while (current_node != NULL) {
        arr[current_node->val] += 1;         
        current_node = current_node->prev;
    }
    return arr;
}

int compute_similarity(tree_t *tree_left, tree_t *tree_right) {
    int *freqs = get_freqs(tree_right);
    int max_val = tree_right->start->val;

    node_t *current_node = tree_left->start;
    int sum = 0;
    while (current_node != NULL) {
        int cv = current_node->val;
        if (cv <= max_val) {
            sum += freqs[cv] * cv; 
        }
        current_node = current_node->prev;
    }
    free(freqs);
    return sum;
}

diff_t *get_diffs(tree_t *tree_left, tree_t *tree_right) {
    diff_t *diff = malloc(sizeof(diff_t));
    if (diff == NULL) {
        perror("Failed alloc to diff");
        return NULL;
    }
    diff->size = tree_left->size;
    diff->array = calloc(tree_left->size, sizeof(int));
    if (diff->array == NULL) {
        perror("Failed calloc to diff");
        free(diff);
    }
    int i = 0;
    for (i = 0; i < diff->size; i++) {
        int l = get_val(tree_left);
        int r = get_val(tree_right);
        diff->array[i] = abs(l - r);
    }
    return diff;
}


int sum_diff(diff_t *diff) {
    int s = 0;
    int i = 0;
    for (i=0; i < diff->size; i++) {
        s += diff->array[i];
    }
    return s;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Wrong number of args\n");
        return 0;
    }

    tree_t *tree_left = init_tree();
    tree_t *tree_right = init_tree();
    if (tree_left == NULL | tree_right == NULL) {
        return -1;
    }

    const char* filepath = argv[1];
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("fopen");
        return -1;
    }

    char line[MAX_LINE];
    char *token_1;
    char *token_2;
    char *sep = "   ";
    while(fgets(line, MAX_LINE, file) != NULL) {
        token_1 = strtok(line, sep);
        token_2 = strtok(NULL, sep);   
        add_val(tree_left, atoi(token_1));
        add_val(tree_right, atoi(token_2));
    }
    fclose(file);

    int sim = compute_similarity(tree_left, tree_right);
    printf("Sim=%d\n", sim);

    diff_t *diff = get_diffs(tree_left, tree_right);
    printf("Diff = %d\n", sum_diff(diff));

    free_tree(tree_left);
    free_tree(tree_right);
    return 0;
}
