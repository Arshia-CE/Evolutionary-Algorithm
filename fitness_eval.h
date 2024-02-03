#include <systemc.h>

SC_MODULE(FitnessEvaluator) {
    sc_in<double**> solution_in;
    sc_in<double*> weights_in;
    sc_in<double*> values_in;
    sc_out<double*> total_value_out;
    sc_out<double*> total_weight_out;

    const int solution_size = 9;
    const int mu = 20;

    void evaluate_solution() {
        double* total_value = new double[mu] {};
        double* total_weight = new double[mu] {};

        for (int i = 0; i < mu; i++) {
            for (int j = 0; j < solution_size; j++) {
                total_value[i] += values_in->read()[j] * solution_in->read()[i][j];
                total_weight[i] += weights_in->read()[j] * solution_in->read()[i][j];
            }
            //if ()
        }

        total_value_out.write(total_value);
        total_weight_out.write(total_weight);
    }

    SC_CTOR(FitnessEvaluator) {
        SC_METHOD(evaluate_solution);
    }
};
