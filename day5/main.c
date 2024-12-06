#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_RULES 10000
#define MAX_NUMBERS 1000

typedef struct {
    int n1;
    int n2;
} rule_t;

typedef struct {
    int *vals;
    size_t size;
} nums_t;

rule_t *create_rule_from_str(char *string, size_t size) {
    rule_t *rule = malloc(sizeof(rule_t)); 
    rule->n1 = atoi(strtok(string, "|"));
    rule->n2 = atoi(strtok(NULL, "|"));
    return rule;
}

void free_rules(rule_t *rules[]) {
    int i = 0;
    for (i = 0; i < MAX_RULES; i++) {
        if (rules[i] == NULL) break;
        free(rules[i]);
    }
}

void print_rule(rule_t *rule) {
    printf("%d - %d", rule->n1, rule->n2);
}

void print_rules(rule_t *rules[]) {
    int i = 0;
    for (i = 0; i < MAX_RULES; i++) {
        if (rules[i] == NULL) break;
        print_rule(rules[i]);
        printf("\n");
    }
}

void print_nums(nums_t *nums) {
    int i = 0;
    for (i = 0; i < nums->size; i++) {
        printf("%d,", nums->vals[i]);
    }
}

nums_t *parse_ints_from_str(char *string) {
    int buff[MAX_NUMBERS] = {0};
    int i = 0;
    char *str_num = strtok(string, ",");
    while (str_num != NULL) {
        buff[i++] = atoi(str_num);
        str_num = strtok(NULL, ",");
    }
    nums_t *nums = malloc(sizeof(nums_t));
    nums->size = i;
    int *vals = calloc(nums->size, sizeof(int));
    for (i = 0; i < nums->size; i++) {
        vals[i] = buff[i];
    }
    nums->vals = vals;
    return nums;
}

bool is_nums_valid(rule_t *rule, nums_t *nums) {
    int n = 0;
    int i = 0;
    for (i = 0; i < nums->size; i++) {
        if (nums->vals[i] == rule->n1) n = 1;
        if (nums->vals[i] == rule->n2 && n == 1) return true;
    }
    return false;
}

bool is_rule_applicable(rule_t *rule, nums_t *nums) {
    bool has_n1 = false;
    bool has_n2 = false;
    for (int i = 0; i < nums->size; i++) {
        if (nums->vals[i] == rule->n1) has_n1 = true;
        if (nums->vals[i] == rule->n2) has_n2 = true;
        if (has_n1 == true && has_n2 == true) return true;
    }
    return false;
}

bool apply_rules_to_nums(rule_t *rules[], nums_t *nums) {
    bool is_valid = true;
    int i = 0;
    int n_applicable = 0;
    int n_valid = 0;
    rule_t *rule = rules[i];
    while (rule != NULL) {
        if (is_rule_applicable(rule, nums) == true) {
            n_applicable++;
            if (is_nums_valid(rule, nums) == true) n_valid++;
        }
        i++;
        rule = rules[i];
    }
    return (n_applicable > 0 && n_applicable == n_valid);
}

void swap(nums_t *nums, int x, int y) {
    int temp = nums->vals[x];
    nums->vals[x] = nums->vals[y];
    nums->vals[y] = temp;
}

void fix_nums_by_rule(rule_t *rule, nums_t *nums) {
    int id_n1 = -1;
    int id_n2 = -1;
    int i = 0;
    for (i = 0; i < nums->size; i++) {
        if (nums->vals[i] == rule->n1) id_n1 = i;
        if (nums->vals[i] == rule->n2) id_n2 = i;
    }
    if (id_n1 >= 0 && id_n2 >= 0 && nums->vals[id_n1] > nums->vals[id_n2]) {
        swap(nums, id_n1, id_n2);
    }
}

void fix_nums_by_rules(rule_t *rules[], nums_t *nums) {
    bool changed;
    do {
        changed = false;
        for (int i = 0; i< nums->size - 1; i++) {
            for (int j = 0; rules[j] != NULL; j++) {
                rule_t *rule = rules[j];
                if (nums->vals[i] == rule->n2 && nums->vals[i+1] == rule->n1) {
                    swap(nums, i, i + 1);
                    changed = true;
                }
            }
        }
    } while(changed);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments\n");
        return -1;
    }

    char *filepath = argv[1];
    FILE *file = fopen(filepath, "r");
    char *line = NULL;
    size_t len;
    ssize_t read;
    bool is_second_part = false;

    rule_t *rules[MAX_RULES] = {NULL};
    int result = 0;
    int result_p2 = 0;
    int n_rules = 0;
    while((read = getline(&line, &len, file)) != -1) {
        if (line[0] == '\n') {
            is_second_part = true;
            continue;
        }
        if (is_second_part == false) {
            rule_t *r = create_rule_from_str(line, read);
            rules[n_rules++] = r;
        } else {
            nums_t *nums = parse_ints_from_str(line);
            if (apply_rules_to_nums(rules, nums) == true) {
                result += nums->vals[nums->size / 2];
            } else {
                fix_nums_by_rules(rules, nums);
                result_p2 += nums->vals[nums->size / 2];
            }
            free(nums);
        }
    }
    printf("Result=%d\n", result);
    printf("Result After fix=%d\n", result_p2);
    free_rules(rules);
    fclose(file);
    return 0;
}
