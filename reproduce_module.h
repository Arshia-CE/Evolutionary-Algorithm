#pragma once
#define ASIO_STANDALONE
#include <asio.hpp>

using namespace asio;
using ip::tcp;

extern tcp::socket* my_soc;

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

        if (!m_First) {
			for (int i = 0; i < NEW_POPULATION; i++) {
				for (int j = 0; j < SOLUTION_SIZE; j++) {
					double response = population_in[i][j];
					char c[8] = {};
					memcpy(c, &response, 8);

					asio::write(*my_soc, asio::buffer(c));

				}
			}

			// Receive data from MATLAB
			for (int i = 0; i < ADDED_CHILDREN; i++) {
				for (int j = 0; j < SOLUTION_SIZE; j++) {
					asio::streambuf buffer;
					asio::read(*my_soc, buffer, asio::transfer_exactly(sizeof(double)));
					std::istream is(&buffer);

					double d;

					is.read((char*)(&d), 8);

					reproduced_population_out[i][j] = d;
				}
			}

			asio::streambuf buffer;
			asio::read(*my_soc, buffer, asio::transfer_exactly(sizeof(char)));
			std::istream is(&buffer);

			char c;
			is.read(&c, 1);
			index.write((int)c);
        }
		m_First = false;
    }

    SC_CTOR(ReproductionModule) {
        SC_METHOD(reproduce);
        sensitive << clk.pos();
    }

private:
	bool m_First = true;
};

