#pragma once
#include <systemc.h>
#include "macros.h"


SC_MODULE(EvolutionaryAlgorithmMonitor) {
    sc_in<double> best_solution[SOLUTION_SIZE];
    sc_in<double> total_value_out_fitness[NEW_POPULATION];
    sc_in<double> selected_population[NEW_POPULATION][SOLUTION_SIZE];
    sc_in<double> reproduced_population[ADDED_CHILDREN][SOLUTION_SIZE];
    sc_in<double> crossover_children[ADDED_CHILDREN][SOLUTION_SIZE];
    sc_in<double> population_in_fitness[NEW_POPULATION][SOLUTION_SIZE];

    SC_CTOR(EvolutionaryAlgorithmMonitor) {
        SC_METHOD(monitor);
        sensitive << best_solution[0] << total_value_out_fitness[0] << selected_population[0][0]
            << reproduced_population[0][0] << crossover_children[0][0] << population_in_fitness[0][0];
    }

    void monitor() {
        cout << "Best Solution: ";
        for (int i = 0; i < SOLUTION_SIZE; ++i) {
            cout << best_solution[i].read() << " ";
        }
        cout << endl;

        cout << "Total Value Out Fitness: " << total_value_out_fitness[0].read() << endl;

        cout << "Selected Population: ";
        for (int i = 0; i < NEW_POPULATION; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                cout << selected_population[i][j].read() << " ";
            }
            cout << "| ";
        }
        cout << endl;

        cout << "Reproduced Population: ";
        for (int i = 0; i < ADDED_CHILDREN; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                cout << reproduced_population[i][j].read() << " ";
            }
            cout << "| ";
        }
        cout << endl;

        cout << "Crossover Children: ";
        for (int i = 0; i < ADDED_CHILDREN; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                cout << crossover_children[i][j].read() << " ";
            }
            cout << "| ";
        }
        cout << endl;

        cout << "Population In Fitness: ";
        for (int i = 0; i < NEW_POPULATION; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                cout << population_in_fitness[i][j].read() << " ";
            }
            cout << "| ";
        }
        cout << endl;

        cout << "------------------------" << endl;
    }
};
