#pragma once
#include <systemc.h>
#include <random>
#include <cmath>

#include "macros.h"

SC_MODULE(MutationModule) {

	sc_in<double> population_in[NEW_POPULATION][SOLUTION_SIZE];

	sc_in <bool> clk;


	sc_out<double> population_mutated_out[NEW_POPULATION][SOLUTION_SIZE];

	void mutate() {
		wait(2);
		std::normal_distribution<double> n_dist(0.0, 1.0);
		std::uniform_real_distribution<double> u_dist(0.0, 1.0);

		double population[NEW_POPULATION][SOLUTION_SIZE] = {};

		double stddev_norm_value = n_dist(m_Eng);


		for (int i = 0; i < NEW_POPULATION; i++) {
			for (int j = 0; j < SOLUTION_SIZE; j++) {

				if (u_dist(m_Eng) <= MUTATION_PROB) {
					population[i][j] = population_in[i][j]->read() + m_DevTable[i][j] * n_dist(m_Eng);
					m_DevTable[i][j] *= std::exp(stddev_norm_value - n_dist(m_Eng));
				}
				else {
					population[i][j] = population_in[i][j]->read();
				}
			}
		}

		for (int i = 0; i < NEW_POPULATION; i++) {
			for (int j = 0; j < SOLUTION_SIZE; j++) {

				population_mutated_out[i][j].write(population[i][j]);
			}
		}
	}

	SC_CTOR(MutationModule) {
		m_Eng = std::mt19937(m_RandomDevice());

		for (int i = 0; i < NEW_POPULATION; i++) {
			for (int j = 0; j < SOLUTION_SIZE; j++) {
				m_DevTable[i][j] = 10.0;
			}
		}

		SC_THREAD(mutate);
		sensitive << clk.pos();
	}

private:
	double m_DevTable[NEW_POPULATION][SOLUTION_SIZE] = { 1.0 };

	std::random_device m_RandomDevice;
	std::mt19937 m_Eng;
};