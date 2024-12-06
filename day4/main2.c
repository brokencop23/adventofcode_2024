#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define GRID_AT(g,r,c) ((g)->vals[(r)*(g)->cols+(c)])
#define GRID_SIZE(g) ((g)->rows * (g)->cols)

typedef struct {
    int rows;
    int cols;
    char vals[];
} grid_t;

grid_t *create_grid(int rows, int cols) {
    grid_t *grid = malloc(sizeof(grid_t) + (rows * cols * sizeof(char)));
    if (grid) {
        grid->rows = rows;
        grid->cols = cols;
    }
    return grid;
}

grid_t *read_from_file(char *filepath) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *file = fopen(filepath, "r");

    int cols = 0;
    int rows = 0;

    // get the size of file
    while ((read = getline(&line, &len, file)) != -1) {
        if (rows == 0) cols = read - 1;
        rows++;
    }

    grid_t *grid = create_grid(rows, cols);
    rewind(file);

    int i, j = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        for (j = 0; j < cols; j++) {
            GRID_AT(grid, i, j) = line[j];
        }
        i++;
    }

    fclose(file);
    return grid;
}

void print_grid(grid_t *grid) {
    int i, j = 0;
    for (i = 0; i < grid->rows; i++) {
        for (j = 0; j < grid->cols; j++) {
            printf("%c", GRID_AT(grid, i, j));
        }
        printf("\n");
    }
    printf("Rows=%d\tCols=%d\n", grid->rows, grid->cols);
}

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Wrong number of arguments\n");
    }

    char *filepath = argv[1];
    grid_t *grid = read_from_file(filepath); 

    print_grid(grid);

    const int dx[] = {-1, -1,  1, 1};
    const int dy[] = {-1,  1, -1, 1};

    int current_row = 0, current_col = 0;
    int row = 0, col = 0;
    const char target[] = "MAS";
    int *result = calloc(GRID_SIZE(grid), sizeof(int));
    for (int i = 0; i < GRID_SIZE(grid); i++) {
        if (grid->vals[i] == 'M') {
            current_row = i / grid->cols;
            current_col = i % grid->cols;
            for (int d = 0; d < 4; d++) {
                char buff[4] = {0};
                for (int j = 0; j < 3; j++) {
                    row = current_row + (j * dy[d]);
                    col = current_col + (j * dx[d]);
                    if (row >= 0 && col >= 0 && row < grid->rows && col < grid->cols) {
                        buff[j] = grid->vals[row * grid->cols + col];
                    } 
                }
                if (strcmp(buff, target) == 0) {
                    int a_row = current_row + dy[d];
                    int a_col = current_col + dx[d];
                    result[a_row * grid->cols + a_col]++;
                } 
            }
        }
    }
    
    int r = 0;
    for (int i = 0; i < GRID_SIZE(grid); i++) {
        if (result[i] == 2) {
            r++;
        }
    }
    printf("Result=%d\n", r);
    return 0;
}
