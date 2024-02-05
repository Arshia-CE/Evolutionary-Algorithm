#pragma once
#define ASIO_STANDALONE
#include <asio.hpp>

using namespace asio;
using ip::tcp;

extern tcp::socket* my_soc;

#include <systemc.h>
#include <cstdlib>
#include <iostream>
#include "macros.h"

SC_MODULE(PopulationInitializer) {

    sc_in <bool> clk;

    sc_out<double> population_out[NEW_POPULATION][SOLUTION_SIZE]; 

    void drive() {
		for (int i = 0; i < NEW_POPULATION; i++) {
			std::cout << i << " : ";
			for (int j = 0; j < SOLUTION_SIZE; j++) {
				asio::streambuf buffer;
				asio::read(*my_soc, buffer, asio::transfer_exactly(sizeof(double)));
				std::istream is(&buffer);

				double d;

				is.read((char*)(&d), 8);

				population_out[i][j] = d;
			}
		}
    }

    SC_CTOR(PopulationInitializer) {
        SC_THREAD(drive);
        sensitive << clk.pos();
    }

};
