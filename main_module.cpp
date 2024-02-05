#define ASIO_STANDALONE
#include <asio.hpp>
#include <systemc.h>
#include "algorithm.h"

using namespace asio;
using ip::tcp;

tcp::socket* my_soc;

int sc_main(int argc, char* argv[]) {

	io_service io_tcp;

	// Create a TCP acceptor
	tcp::acceptor acceptor(io_tcp, tcp::endpoint(tcp::v4(), 12345));

	// Accept connection
	my_soc = new tcp::socket(io_tcp);
	acceptor.accept(*my_soc);

    sc_clock main_clk("main_clk", 1, SC_MS); 

    sc_signal<double> weights[SOLUTION_SIZE];
    sc_signal<double> values[SOLUTION_SIZE];
    sc_signal<double> best_solution[SOLUTION_SIZE];

    double item_weights[] = { 2, 3, 6, 7, 5, 9, 3, 4, 5 };
    double item_values[] = { 6, 5, 8, 9, 6, 7, 3, 6, 8 };

    for (int i = 0; i < SOLUTION_SIZE; ++i) {
        weights[i].write(item_weights[i]);
        values[i].write(item_values[i]);
    }

    EvolutionaryAlgorithm evolutionary_algorithm("evolutionary_algorithm");

    for (int i = 0; i < SOLUTION_SIZE; ++i) {
        evolutionary_algorithm.weights[i](weights[i]);
        evolutionary_algorithm.values[i](values[i]);
        evolutionary_algorithm.best_solution[i](best_solution[i]);
    }

    evolutionary_algorithm.main_clk(main_clk);
    
    sc_start(10, SC_SEC);

    cout << "Best Solution: ";
    for (int i = 0; i < SOLUTION_SIZE; ++i) {
        cout << best_solution[i].read() << " ";
    }
    cout << endl;

    return 0;
}
