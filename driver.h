#include <systemc.h>
#include <cstdlib> 
#include <iostream> 


SC_MODULE(PopulationInitializer) {

    sc_out<double**> population_out;

    const int population_size = 20; // the desired mu input
    const int solution_size = 9;

    void drive() {
        double** population = new double* [population_size];
        for (int i = 0; i < population_size; ++i) {
            population[i] = new double[solution_size];
            for (int j = 0; j < solution_size; ++j) {
                population[i][j] = (rand() % 101) / 100.0;
            }
        }

        population_out.write(population);
    }

    SC_CTOR(PopulationInitializer) {
        SC_THREAD(drive);
    }

};
