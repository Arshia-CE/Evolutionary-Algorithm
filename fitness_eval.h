#pragma once

#include <systemc.h>
#include "macros.h"

SC_MODULE(FitnessEvaluator) {
    sc_in<double> solution_in[NEW_POPULATION][SOLUTION_SIZE];
    sc_in<double> weights_in[SOLUTION_SIZE];
    sc_in<double> values_in[SOLUTION_SIZE];

    sc_in <bool> clk;

    sc_out<double> total_value_out[NEW_POPULATION];
    sc_out<double> solution_out[NEW_POPULATION][SOLUTION_SIZE];


    
    void evaluate_solution() {
        double total_value[NEW_POPULATION] = {};
        double total_weight[NEW_POPULATION] = {};
        double population_temp[NEW_POPULATION][SOLUTION_SIZE];

        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                population_temp[i][j] = solution_in[i][j].read();
            }
        }


        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                if (population_temp[i][j] < 0.0) {
                    population_temp[i][j] = 0.0;
                }
                if (population_temp[i][j] > 1.0) {
                    population_temp[i][j] = 1.0;
                }
            }
        }



        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                total_value[i] += values_in[j]->read() * solution_in[i][j]->read();
                total_weight[i] += weights_in[j]->read() * solution_in[i][j]->read();
            }

        }

        // TODO : improve
        for (int i = 0; i < NEW_POPULATION; i++) {
            if (total_weight[i] > MAX_SIZE) {
                total_value[i] = 0.0;
            }
        }



        for (int i = 0; i < NEW_POPULATION; i++) {
            total_value_out[i].write(total_value[i]);
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                solution_out[i][j].write(population_temp[i][j]);
            }
        }

        
    }

    SC_CTOR(FitnessEvaluator) {
        SC_METHOD(evaluate_solution);
        sensitive << clk.pos();
    }
};
