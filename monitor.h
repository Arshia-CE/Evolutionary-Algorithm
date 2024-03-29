#pragma once
#include <systemc.h>
#include "macros.h"


SC_MODULE(EvolutionaryAlgorithmMonitor) {
    sc_in<double> best_solution[SOLUTION_SIZE];
    sc_in<double> total_value_out_fitness[NEW_POPULATION];
    sc_in<double> total_value_out_sorter[NEW_POPULATION];
    sc_in<double> selected_population[NEW_POPULATION][SOLUTION_SIZE];
    sc_in<double> reproduced_population[ADDED_CHILDREN][SOLUTION_SIZE];
    sc_in<double> crossover_children[ADDED_CHILDREN][SOLUTION_SIZE];
    sc_in<double> population_in_fitness[NEW_POPULATION][SOLUTION_SIZE];

    SC_CTOR(EvolutionaryAlgorithmMonitor) {
        SC_METHOD(monitor);
        //for (int i = 0;  i < SOLUTION_SIZE;  i++){
          //sensitive << best_solution[i];
        //}
        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
              sensitive << selected_population[i][j] ;
           }
        }
        //for (int i = 0; i < ADDED_CHILDREN; i++) {
            //for (int j = 0; j < SOLUTION_SIZE; j++) {
            //    sensitive << reproduced_population[i][j] << crossover_children[i][j];
          //  }
        //}
    }

    void monitor() {
        
        cout << "Total Value Out Fitness: " << endl;
        for (int i = 0; i < NEW_POPULATION; ++i) {
            cout << total_value_out_fitness[i].read() << " ";
        }
        cout << endl;
          
        cout << "Selected Population: " << endl;
        for (int i = 0; i < NEW_POPULATION; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                cout << selected_population[i][j].read() << " ";
            }
            cout << "| Total Value After Sorting: " << total_value_out_sorter[i].read() << endl;
        }

        cout << "Reproduced Population: " << endl;
        for (int i = 0; i < ADDED_CHILDREN; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                cout << reproduced_population[i][j].read() << " ";
            }
            cout << endl;
        }

        cout << "Crossover Children: " << endl;
        for (int i = 0; i < ADDED_CHILDREN; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                cout << crossover_children[i][j].read() << " ";
            }
            cout << endl;
        }

        cout << "Population In Fitness: " << endl;
        for (int i = 0; i < NEW_POPULATION; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                cout << population_in_fitness[i][j].read() << " ";
            }
            cout << endl;
        }

        cout << "------------------------" << endl;
    }
};
