#pragma once

#include <systemc.h>
#include "macros.h"

SC_MODULE(FitnessEvaluator) {
    sc_in<double> solution_in[NEW_POPULATION][SOLUTION_SIZE];
    sc_in<double> weights_in[SOLUTION_SIZE];
    sc_in<double> values_in[SOLUTION_SIZE];

    sc_out<double> total_value_out[NEW_POPULATION];

    
    void evaluate_solution() {
        double total_value[NEW_POPULATION] = {};
        double total_weight[NEW_POPULATION] = {};

        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                total_value[i] += values_in[j]->read() * solution_in[i][j]->read();
                total_weight[i] += weights_in[j]->read() * solution_in[i][j]->read();
            }
            if (total_weight[i] > MAX_SIZE) {
                total_value[i] = 0.0;
            }
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                if (solution_in[i][j] > 1.0) {
                    total_value[i] = 0.0;
                }
            }
        }

        for (int i = 0; i < NEW_POPULATION; i++) {
            total_value_out[i].write(total_value[i]);
        }

    }

    SC_CTOR(FitnessEvaluator) {
        SC_METHOD(evaluate_solution);
        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < POPULATION_SIZE; j++) {
                sensitive << weights_in[i] << values_in[i] << solution_in[i][j];
            }
        }
    }
};
