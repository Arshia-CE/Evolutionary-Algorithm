#pragma once

#include <systemc.h>
#include <cstdlib>
#include <iostream>
#include "macros.h"



SC_MODULE(PopulationInitializer) {

    sc_out<double> population_out[NEW_POPULATION][SOLUTION_SIZE]; 

    void drive() {

        for (int i = 0; i < POPULATION_SIZE; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                population_out[i][j].write((rand() % 101) / 100.0);
            }
        }
        for (int i = POPULATION_SIZE; i < NEW_POPULATION; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                population_out[i][j].write(0.0);
            }
        }
    }

    SC_CTOR(PopulationInitializer) {
        SC_THREAD(drive);
    }
};
