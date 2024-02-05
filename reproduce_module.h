#pragma once

#include <systemc.h>
#include "macros.h"
#include <iostream>
#include <set>
#include <cstdlib>


SC_MODULE(ReproductionModule) {
    
    sc_in<double> population_in[NEW_POPULATION][SOLUTION_SIZE];
    
    sc_in <bool> clk;

    sc_out<double> reproduced_population_out[ADDED_CHILDREN][SOLUTION_SIZE];
    sc_out<int> index;


    void reproduce() {
        
        std::set<int> indexes;

        while (indexes.size() < ADDED_CHILDREN){
            indexes.insert(rand() % POPULATION_SIZE);
        }

        int k = 0;
        for (auto i : indexes) {
            for (int j = 0; j < SOLUTION_SIZE; j++){
                reproduced_population_out[k][j].write(population_in[i][j]);
            }
            k++;
        }

        index.write(rand() % SOLUTION_SIZE);
    }

    SC_CTOR(ReproductionModule) {
        SC_METHOD(reproduce);
        sensitive << clk.pos();
    }
};

