#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define N_DIGITS 3

bool is_mul_enabled = true;

int calc_mul_from_str(char *string) {
    bool is_bracket_open = false;
    bool is_colon = false;
    int pos = 0;
    char ch = string[0];
    char *num_1 = calloc(N_DIGITS, sizeof(char));
    char *num_2 = calloc(N_DIGITS, sizeof(char));
    int num_i = 0;
    int i = 0;
    int sum = 0;

    int enable_pos = 0;
    while ((ch = string[i++]) != '\0') {
        if (ch == 'd') {
            enable_pos = 1;
        } else if (ch == 'o' && enable_pos == 1) {
            enable_pos++;
        } else if (ch == 'n' && enable_pos == 2) {
            enable_pos++;
        } else if (ch == '\'' && enable_pos == 3) {
            enable_pos++;
        } else if (ch == 't' && enable_pos == 4) {
            enable_pos++;
        } else if (ch == '(' && enable_pos >= 2) {
            enable_pos++;
        } else if (ch == ')' && enable_pos >= 2) {
            if (enable_pos == 3) {
                is_mul_enabled = true;
            } else if (enable_pos == 6) {
                is_mul_enabled = false;
            }
            enable_pos = 0;
        } else {
            enable_pos = 0;
        }

        if (ch == 'm' && pos == 0) {
            pos++;
        } else if (ch == 'm' && pos >= 1) {
            pos = 1;
            is_bracket_open = false;
            is_colon = false;
            num_i = 0;
            memset(num_1, '\0', N_DIGITS);
            memset(num_2, '\0', N_DIGITS);
        } else if (ch == 'u' && pos == 1) {
            pos++;
        } else if (ch == 'l' && pos == 2) {
            pos++;
        } else if (ch == '(' && pos == 3 && is_bracket_open == false) {
            is_bracket_open = true;
        } else if (ch == ')' && is_bracket_open == true && is_colon == true) {
            if (is_mul_enabled == true) {
                printf("%d * %d\n", atoi(num_1), atoi(num_2));
                sum += atoi(num_1) * atoi(num_2);
            }
            pos = 0;
            is_bracket_open = false;
            is_colon = false;
            num_i = 0;
            memset(num_1, '\0', N_DIGITS);
            memset(num_2, '\0', N_DIGITS);
        } else if (ch == ',' && is_bracket_open == true) {
            is_colon = true;
            num_i = 0;
        } else if (ch >= 48 && ch <= 57) {
            if (is_colon == false) {
                num_1[num_i++] = ch;
            } else {
                num_2[num_i++] = ch;
            }
        } else {
            pos = 0;
            is_bracket_open = false;
            is_colon = false;
            num_i = 0;
            memset(num_1, '\0', N_DIGITS);
            memset(num_2, '\0', N_DIGITS);
        }
    }
    return sum;
}


void test() {
    char *test = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
    int test_val = 161;
    int val = calc_mul_from_str(test);
    if (val == test_val) {
        printf("Test passed\n");
    }
}

void test2() {
    char *test = "mmul(2,3)";
    int test_val = 6;
    int val = calc_mul_from_str(test);
    if (val == test_val) {
        printf("Test passed\n");
    } else {
        printf("Test not passed\n");
    }
}

void test3() {
    char *test = "mmul(2,3mul(3,4))";
    int test_val = 12;
    int val = calc_mul_from_str(test);
    if (val == test_val) {
        printf("Test passed\n");
    } else {
        printf("Test not passed\n");
    }
}

void test4() {
    char *test = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";
    int test_val = 48;
    int val = calc_mul_from_str(test);
    if (val == test_val) {
        printf("Test passed\n");
    } else {
        printf("Test not passed\n");
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        test();
        test2();
        test3();
        test4();
        return 0;
    } 

    char *filepath = argv[1];
    FILE *file = fopen(filepath, "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int sum = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        sum += calc_mul_from_str(line);
        printf("Sum=%d\n", sum);
    }
    fclose(file);

    printf("%d\n", sum);
    return 0;
}
