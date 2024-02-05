#pragma once

#include <systemc.h>
#include "macros.h" 

SC_MODULE(SelectionModule) {
    sc_in<double> population_in[NEW_POPULATION][SOLUTION_SIZE];
    sc_inout<double> total_value_in[NEW_POPULATION];
    
    sc_out<double> selected_population_out[NEW_POPULATION][SOLUTION_SIZE];

    void select_best_individuals() {

        double population_temp[NEW_POPULATION][SOLUTION_SIZE];
        double value_temp[NEW_POPULATION];

        for (int i = 0; i < NEW_POPULATION; i++) {
            value_temp[i] = total_value_in[i].read(); 
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                population_temp[i][j] = population_in[i][j].read();
            }
        }

        for (int i = 0; i < NEW_POPULATION - 1; i++) {
            for (int j = 0; j < NEW_POPULATION - i - 1; j++) {
                if (value_temp[j] < value_temp[j+1]) {
                    for (int k = 0; k < SOLUTION_SIZE; k++) {
                        double temp = population_temp[j][k];
                        population_temp[j][k] = population_temp[j + 1][k];
                        population_temp[j + 1][k] = temp;  

                        double temp2 = value_temp[j];
                        value_temp[j] = value_temp[j + 1];
                        value_temp[j + 1] = temp2;
                    }
                }
            }
        }

        for (int i = POPULATION_SIZE; i < NEW_POPULATION; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                population_temp[i][j] = 0.0;
            }
            value_temp[i] = 0.0;
        }

        for (int i = 0; i < NEW_POPULATION; i++) {
            total_value_in[i].write(value_temp[i]);
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                selected_population_out[i][j].write(population_temp[i][j]);
            }
        }
    }

    SC_CTOR(SelectionModule) {
        SC_METHOD(select_best_individuals);
        for (int i = 0; i < NEW_POPULATION; i++) {
            sensitive << total_value_in[i];
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                sensitive << population_in[i][j];
            }
        }
    }
};
