#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    UP = 1,
    RIGHT,
    DOWN,
    LEFT,
    TURN
} dir_e;

typedef struct {
    int *vals;
    int *steps;
    int rows;
    int cols;
    int start;
} map_t;

map_t *create_matrix_from_file(char *filepath) {
    char *line = NULL;
    size_t len;
    ssize_t read;

    int cols = 0;
    int rows = 0;
    FILE *file = fopen(filepath, "r");
    while ((read = getline(&line, &len, file)) != -1) {
        if (cols == 0) cols = read;
        rows++;
    }

    map_t *map = malloc(sizeof(map_t));
    map->cols = cols;
    map->rows = rows;

    int *vals = calloc(rows * cols, sizeof(int));
    int *steps = calloc(rows * cols, sizeof(int));
    map->steps = steps;
    
    rewind(file);
    int i = 0;
    int j = 0;
    int pos;
    while ((read = getline(&line, &len, file)) != -1) {
        for (j = 0; j < read; j++) {
            pos = i * cols + j;
            if (line[j] == '#') vals[pos] = 1;
            if (line[j] == '^') {
                map->start = pos;
                map->steps[pos] = 1;
            }
        }
        i++;
    }
    map->vals = vals;
    fclose(file);
    return map;
}

int predict_steps(map_t *map) {
    int cur = map->start;
    int next_step = 0;
    int dir_x = 0;
    int dir_y = -1;
    bool is_exit = false;
    dir_e prev_dir = UP;
    dir_e dir = UP;
    int max_loops = map->rows * map->cols;
    int loops = 0;
    while (is_exit == false) {
        if (loops > max_loops) return -1;
        if (prev_dir != dir) {
            map->steps[cur] = TURN;
            prev_dir = dir;
        } else {
            map->steps[cur] = dir;
        }
        switch (dir) {
            case UP:
                next_step = cur - map->cols;
                break;
            case DOWN:
                next_step = cur + map->cols;
                break;
            case LEFT:
                next_step = cur - 1;
                break;
            case RIGHT:
                next_step = cur + 1;
                break;
            default:
                break;
        }
        if (map->vals[next_step] == 1) {
            prev_dir = dir;
            switch (dir) {
                case UP:
                    dir = RIGHT;
                    break;
                case RIGHT:
                    dir = DOWN;
                    break;
                case DOWN:
                    dir = LEFT;
                    break;
                case LEFT:
                    dir = UP;
                    break;
                default:
                    break;
            }
        } else {
            if ((next_step <= map->cols) 
                || (next_step >= map->rows * map->cols - map->cols)
                || (next_step % map->cols == 0)
                || (next_step % map->cols == map->cols - 1)
            ) is_exit = true;
            cur = next_step;
        }
        loops++;
    }
    return 0;
}

void print_map(map_t *map) {
    int i = 0;
    int j = 0;
    int val = 0;
    int pos = 0;
    for (i = 0; i < map->rows; i++) {
        for (j=0; j < map->cols; j++) {
            pos = i * map->cols + j;
            val = map->vals[pos];
            if (pos == map->start) {
                printf("^");
            } else if (map->steps[pos] > 0) {
                switch (map->steps[pos]) {
                    case UP:
                        printf("|");
                        break;
                    case DOWN:
                        printf("|");
                        break;
                    case LEFT:
                        printf("-");
                        break;
                    case RIGHT:
                        printf("-");
                        break;
                    case TURN:
                        printf("+");
                        break;
                }
            } else if (val == 0) {
                printf(".");
            } else if (val == 1) {
                printf("#");
            }
        }
        printf("\n");
    }
}

int simulate_loops(char *filepath) {
    int n_loops = 0;
    map_t *map = create_matrix_from_file(filepath);
    int rows = map->rows;
    int cols = map->cols;
    for (int i = 0; i < rows * cols; i++) {
        map_t *map = create_matrix_from_file(filepath);
        if (map->vals[i] == 0) map->vals[i] = 1;
        if (predict_steps(map) == -1) n_loops++;
    }
    return n_loops;
}

int calc_paths(char *filepath) {
    int sum = 0;
    int i = 0;
    map_t *map = create_matrix_from_file(filepath);
    predict_steps(map);
    for (i = 0; i < map->rows * map->cols; i++) {
        if (map->steps[i] > 0) sum += 1;
    }
    return sum + 1;
}

void test(char *filepath) {
    map_t *map = create_matrix_from_file(filepath);
    map->vals[69] = 1;
    print_map(map);
    predict_steps(map);
}

int main(int argc, char *argv[]) {

    if (argc !=2) {
        printf("Wrong number of args");
        return -1;
    }

    printf("Loop=%d\n", simulate_loops(argv[1]));
    printf("Result=%d\n", calc_paths(argv[1]));
    return 0;
}
