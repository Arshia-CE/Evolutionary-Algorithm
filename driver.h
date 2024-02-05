#pragma once

#include <systemc.h>
#include <cstdlib>
#include <iostream>
#include "macros.h"



SC_MODULE(PopulationInitializer) {

    sc_in <bool> clk;


    sc_out<double> population_out[NEW_POPULATION][SOLUTION_SIZE]; 

    void drive() {

        for (int i = 0; i < POPULATION_SIZE; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                population_out[i][j].write(m_Uniform(m_Eng));
            }
        }
        for (int i = POPULATION_SIZE; i < NEW_POPULATION; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                population_out[i][j].write(0.0);
            }
        }
    }

    SC_CTOR(PopulationInitializer) {
        m_Eng = std::mt19937(m_RandomDevice());
        m_Uniform = std::uniform_real_distribution<>(0.0, 1.0);
        SC_THREAD(drive);
        sensitive << clk.pos();
    }

private:
	std::random_device m_RandomDevice;
	std::mt19937 m_Eng;
    std::uniform_real_distribution<> m_Uniform;
};
