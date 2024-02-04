#pragma once

#include <systemc.h>
#include "macros.h"

SC_MODULE(CrossoverModule) {
    sc_in<double> parent1_in[SOLUTION_SIZE];
    sc_in<double> parent2_in[SOLUTION_SIZE];
    sc_in<int> crossover_point_in; 
    sc_out<double> child1_out[SOLUTION_SIZE];
    sc_out<double> child2_out[SOLUTION_SIZE];


    void perform_crossover() {

        for (int i = 0; i < SOLUTION_SIZE; i++) {
            if (i < crossover_point_in.read()) {
                child1_out[i]->write(parent1_in[i]);
                child2_out[i]->write(parent2_in[i]);
            }
            else {
                child1_out[i]->write(parent2_in[i]);
                child2_out[i]->write(parent1_in[i]);;
            }
        }
    }

    SC_CTOR(CrossoverModule) {
        SC_METHOD(perform_crossover);
        for (int i = 0; i < SOLUTION_SIZE; i++) {
            sensitive << parent2_in[i] << parent1_in[i] << crossover_point_in;
        }
    }
};
