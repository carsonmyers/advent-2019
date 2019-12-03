#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day1.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No input masses provided\n");
        return 0;
    }

    int len = argc - 1;
    int *numbers = read_numbers(len, argv + 1);
    
    printf(" > running basic equation...\n");
    int basic_fuel = basic_equation(len, numbers);
    printf("---\n > total fuel required: %d\n\n", basic_fuel);

    printf(" > running rocket equation...");
    int total_fuel = rocket_equation(len, numbers);
    printf("---\n > total fuel required: %d\n\n", total_fuel);

    return 0;
}

int *read_numbers(int len, char *numbers[]) {
    int *results = (int *)malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) {
        results[i] = atoi(numbers[i]);
    }

    return results;
}

int basic_equation(int len, int masses[]) {
    int total_fuel = 0;

    for (int i = 0; i < len; i++) {
        int fuel = fuel_required(masses[i]);
        printf("%d kg requires %d kg of fuel\n", masses[i], fuel);

        total_fuel += fuel;
    }

    return total_fuel;
}

int rocket_equation(int len, int masses[]) {
    int total_fuel = 0;

    for (int i = 0; i < len; i++) {
        int fuel = 0;
        int last_amount = fuel_required(masses[i]);
        while (last_amount > 0) {
            fuel += last_amount;
            last_amount = fuel_required(last_amount);
        }

        printf("%d kg requires %d kg of fuel\n", masses[i], fuel);
        total_fuel += fuel;
    }

    return total_fuel;
}

int fuel_required(int mass) {
    int result = (mass / 3) - 2;
    return result > 0 ? result : 0;
}