#pragma once

#define POPULATION_SIZE 20 // Desired mu 
#define SOLUTION_SIZE 9
#define ADDED_CHILDREN 8 // Desired lambda
#define NEW_POPULATION ADDED_CHILDREN + POPULATION_SIZE
#define MAX_SIZE 25
#define MUTATION_PROB 0.3
#define MUTATION_MODULES ADDED_CHILDREN / 2
#define THRESHOLD 0.001