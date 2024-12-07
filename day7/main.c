#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    PLUS = 0,
    MINUS,
    MUL,
    DIV,
    CONCAT,
    NA
} operation_e;

typedef struct {
    long long *nums;
    size_t size;
    long long test_val;
} equation_t;

long long concat_numbers(long long a, long long b) {
    long long temp = b;
    long long multiplier = 1;
    while (temp > 0) {
        multiplier *= 10;
        temp /= 10;
    }
    return a * multiplier + b;
}

equation_t *init_equation_from_str(char *str) {
    equation_t *eq = malloc(sizeof(equation_t));
    char *test_s;
    char *nums_s;

    test_s = strtok(str, ":");
    nums_s = strtok(NULL, ":");

    int i = 1;
    int s = 0;
    while (nums_s[i] != '\n') {
        if (nums_s[i] == ' ') s++;
        i++;
    }

    long long *nums = calloc(s + 1, sizeof(long long));
    int j = 0;
    char *num_c = strtok(nums_s, " ");
    while (num_c != NULL) {
        nums[j++] = atoll(num_c);
        num_c = strtok(NULL, " ");
    }
    eq->nums = nums;
    eq->test_val = atoll(test_s);
    eq->size = s + 1;
    return eq;
}

bool is_applicable(equation_t *eq, int n, long long result, operation_e op) {
    if (n == 0) {
        int temp = eq->nums[0];
        result = temp;
    } else {
        int temp = eq->nums[n];
        switch (op) {
            case PLUS:
                result = result + temp;
                break;
            case MINUS:
                result = result - temp;
                break;
            case MUL:
                result = result * temp;
                break;
            case DIV:
                result = result / temp;
                break;
            case CONCAT:
                result = concat_numbers(result, temp);
                break;
            default:
                return false;
        }
    }
    if (n == eq->size - 1) {
        return result == eq->test_val;
    } else {
        return (
            is_applicable(eq, n + 1, result, PLUS)
            || is_applicable(eq, n + 1, result, MUL)
            || is_applicable(eq, n + 1, result, CONCAT)
        );
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Wrong number of args\n");
        return -1;
    }

    long long result = 0;

    char *filepath = argv[1];
    char *line = NULL;
    size_t len;
    ssize_t read;

    FILE *file = fopen(filepath, "r");
    while ((read = getline(&line, &len, file)) != -1) {
        equation_t *eq = init_equation_from_str(line);
        if (is_applicable(eq, 0, 0, NA) == true) {
            result += eq->test_val;
        }
    }
    fclose(file);

    printf("Result=%lld\n", result);
    return 0;
}
