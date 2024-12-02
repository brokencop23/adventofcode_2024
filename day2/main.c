#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_GAP 3
#define MAX_LINE 1024
#define BUFF_SIZE 200

typedef enum {
    SAFE,
    UNSAFE
} status_e;

status_e is_safe_sequence(int *nums, int size) {
    if (size < 2) return SAFE;
    int is_increasing = nums[1] > nums[0];
    int i = 0;
    int diff = 0;
    for (i = 1; i < size; i++) {
        diff = abs(nums[i] - nums[i-1]);
        if (diff == 0) return UNSAFE;
        if (diff > MAX_GAP) return UNSAFE;
        if ((nums[i] > nums[i-1]) != is_increasing) return UNSAFE;
    }
    return SAFE;
}

status_e is_safe_weak(int *nums, int size) {
    if (is_safe_sequence(nums, size) == SAFE) return SAFE;
    int *temp = calloc(size-1, sizeof(int));
    int i, j, idx;
    for (i = 0; i < size; i++) {
        idx = 0;
        for (j = 0; j < size; j++) {
            if (i != j) {
                temp[idx++] = nums[j];
            }
        }
        if (is_safe_sequence(temp, size-1) == SAFE) {
            free(temp);
            return SAFE;
        } 
    }
    free(temp);
    return UNSAFE;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Wrong number of args");
        return -1;
    }

    int n_safe = 0;
    int n_safe_weak = 0;
    char *filepath = argv[1];
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("fopen");
        return -1;
    }

    char line[MAX_LINE];
    char *token;
    int buff[BUFF_SIZE] = {0};
    while (fgets(line, MAX_LINE, file) != NULL) {
        token = strtok(line, " ");
        int size = 0;
        int i = 0;
        while (token != NULL) {
            buff[size] = atoi(token);
            token = strtok(NULL, " ");
            size += 1;
        }
        int *seq = calloc(size, sizeof(int));
        for (i = 0; i < size; i++) {
            seq[i] = buff[i];
        }
        if (is_safe_sequence(seq, size) == SAFE) n_safe += 1;
        if (is_safe_weak(seq, size) == SAFE) n_safe_weak += 1;
        free(seq);
    }
    printf("Safe reports: %d\n", n_safe);
    printf("Safe reports (weak): %d\n", n_safe_weak);
    return 0;
}
