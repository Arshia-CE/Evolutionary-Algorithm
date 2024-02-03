#include <systemc.h>
#include <cstdlib>
#include <iostream>
#include "macros.h"



SC_MODULE(PopulationInitializer) {

    sc_out<double> population_out[POPULATION_SIZE][SOLUTION_SIZE]; 

    void drive() {

        for (int i = 0; i < POPULATION_SIZE; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                population_out[i][j].write((rand() % 101) / 100.0);
            }
        }
    }

    SC_CTOR(PopulationInitializer) {
        SC_THREAD(drive);
    }
};
