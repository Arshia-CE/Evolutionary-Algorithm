#pragma once

#include <systemc.h>
#include <cmath>
#include "mutation_module.h"
#include "reproduce_module.h"
#include "sorter_module.h"
#include "crossover_module.h"
#include "fitness_eval.h"
#include "mutation_module.h"
#include "driver.h"
#include "monitor.h"
#include "module_selector.h"

SC_MODULE(EvolutionaryAlgorithm) {
    
    sc_in<double> weights[SOLUTION_SIZE];
    sc_in<double> values[SOLUTION_SIZE];

    sc_in<bool> main_clk;

    sc_out<double> best_solution[SOLUTION_SIZE];

    sc_signal<double , SC_MANY_WRITERS> population_in_fitness[NEW_POPULATION][SOLUTION_SIZE];
    sc_signal<double> total_value_out_fitness[NEW_POPULATION];
    sc_signal<double> total_value_out_sorter[NEW_POPULATION];
    sc_signal<double , SC_MANY_WRITERS> selected_population[NEW_POPULATION][SOLUTION_SIZE];
    sc_signal<double> sorter_in[NEW_POPULATION][SOLUTION_SIZE];
    sc_signal<double> reproduced_in[NEW_POPULATION][SOLUTION_SIZE];
    sc_signal<double> reproduced_population[ADDED_CHILDREN][SOLUTION_SIZE];
    sc_signal<int> index_signal;
    sc_signal<bool> en;
    sc_signal<bool> sel_clk;
    sc_signal<bool> clk[MODULE_COUNT];


    SelectionModule *selection_module;
    ReproductionModule *reproduction_module;
    CrossoverModule *crossover_module[MUTATION_MODULES];
    FitnessEvaluator *fitness_evaluator;
    PopulationInitializer *population_initializer;
    MutationModule *mutation_module;
    EvolutionaryAlgorithmMonitor *monitor;
    ModuleSelector* selector;

    void kill_algorithm() {
        if (iter < max_iter || total_iter < 5){ 
            //reproduction in
            for (int i = 0; i < POPULATION_SIZE; i++) {
                for (int j = 0; j < SOLUTION_SIZE; j++) {
                    reproduced_in[i][j].write(selected_population[i][j]);
                }
            }

            double x = total_value_out_sorter[0].read();
            if (abs(best_solution_value - x) < THRESHOLD) {
                iter++;
                best_solution_value = x;
            }
            else if (abs(x - best_solution_value) >= THRESHOLD) {
                iter = 0;
                for (int j = 0; j < SOLUTION_SIZE; j++) {
                    best_solution_internal[j] = selected_population[0][j].read();
                }
                best_solution_value = x;
            }     
        }
        else {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                best_solution[j].write(best_solution_internal[j]);
            }
            en.write(1);
        }
        total_iter++;
    }

    void on_clk() {
        sel_clk.write(main_clk.read());
    }

    SC_CTOR(EvolutionaryAlgorithm) {
        selection_module = new SelectionModule("selection_module");
        reproduction_module = new ReproductionModule("reproduction_module");
        for (int i = 0; i < MUTATION_MODULES; i++) {
            crossover_module[i] = new CrossoverModule("crossover_module");
        }
        fitness_evaluator = new FitnessEvaluator("fitness_evaluator");
        population_initializer = new PopulationInitializer("population_initializer");
        mutation_module = new MutationModule("mutation_module");
        monitor = new EvolutionaryAlgorithmMonitor("monitor");
        selector = new ModuleSelector("selector");

        //selector
        for (int i = 0; i < MODULE_COUNT; i++){
            selector->sel_out[i](clk[i]);
        }
        selector->clk_in(sel_clk);
        selector->n_en_in(en);

        

        //initializer
        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                population_initializer->population_out[i][j](population_in_fitness[i][j]);
            }
        }
        population_initializer->clk(clk[0]);
    
        //fitness in
        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                fitness_evaluator->solution_in[i][j](population_in_fitness[i][j]);
            }
        }
        for (int i = 0; i < SOLUTION_SIZE; i++) {
            fitness_evaluator->values_in[i](values[i]);
            fitness_evaluator->weights_in[i](weights[i]);
        }
        fitness_evaluator->clk(clk[1]);

        //fitness out
        for (int i = 0; i < NEW_POPULATION; i++) {
            fitness_evaluator->total_value_out[i](total_value_out_fitness[i]);
        }
        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                fitness_evaluator->solution_out[i][j](sorter_in[i][j]);
            }
        }
        //sorter in
        for (int i = 0; i < NEW_POPULATION; i++) {
            selection_module->total_value_in[i](total_value_out_fitness[i]);
        }
        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                selection_module->population_in[i][j](sorter_in[i][j]);
            }
        }
        selection_module->clk(clk[2]);

        //sorter out
        for (int i = 0; i < NEW_POPULATION; i++) {
            selection_module->total_value_out[i](total_value_out_sorter[i]);
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                selection_module->selected_population_out[i][j](selected_population[i][j]);
            }
        }

        //reproduction in
        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                reproduction_module->population_in[i][j](reproduced_in[i][j]);
            }
        }
        reproduction_module->clk(clk[4]);


        //reproduction out
        for (int i = 0; i < ADDED_CHILDREN; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                reproduction_module->reproduced_population_out[i][j](reproduced_population[i][j]);
            }
        }
        reproduction_module->index(index_signal);

        //crossover in
        for (int i = 0; i < ADDED_CHILDREN - 1; i = i + 2) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                crossover_module[i / 2]->parent1_in[j](reproduced_population[i][j]);
                crossover_module[i / 2]->parent2_in[j](reproduced_population[i + 1][j]);

            }
            crossover_module[i / 2]->crossover_point_in(index_signal);
            crossover_module[i / 2]->clk(clk[5]);

        }
        
        

        //crossover out
        for (int i = 0; i < ADDED_CHILDREN - 1; i = i + 2) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                crossover_module[i / 2]->child1_out[j](reproduced_in[i + POPULATION_SIZE][j]);
                crossover_module[i / 2]->child2_out[j](reproduced_in[i + POPULATION_SIZE + 1][j]);
            }
        }

        //mutation in
        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                mutation_module->population_in[i][j](reproduced_in[i][j]);
            }
        }
        mutation_module->clk(clk[6]);
        
        //mutation out
        for (int i = 0; i < NEW_POPULATION; i++) {
            for (int j = 0; j < SOLUTION_SIZE; j++) {
                mutation_module->population_mutated_out[i][j](population_in_fitness[i][j]);
            }
        }
        
        //connect monitor
        for (int i = 0; i < SOLUTION_SIZE; ++i) {
            monitor->best_solution[i](best_solution[i]);
        }

        for (int i = 0; i < NEW_POPULATION; ++i) {
            monitor -> total_value_out_fitness[i](total_value_out_fitness[i]);
            monitor -> total_value_out_sorter[i](total_value_out_sorter[i]);
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                monitor->selected_population[i][j](reproduced_in[i][j]);
                monitor->population_in_fitness[i][j](population_in_fitness[i][j]);
            }
        }

        for (int i = 0; i < ADDED_CHILDREN; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                monitor->reproduced_population[i][j](reproduced_population[i][j]);
            }
        }
        
        for (int i = POPULATION_SIZE; i < NEW_POPULATION; ++i) {
            for (int j = 0; j < SOLUTION_SIZE; ++j) {
                monitor->crossover_children[i-POPULATION_SIZE][j](reproduced_in[i][j]);
            }
        }       

        SC_METHOD(kill_algorithm);
        sensitive << clk[3];

        SC_METHOD(on_clk);
        sensitive << main_clk;
    }

private:
	const int max_iter = 5;
	double best_solution_value = 0.0;
	double best_solution_internal[SOLUTION_SIZE] = {};
	int iter = 0;
    unsigned long total_iter = 0;
};

