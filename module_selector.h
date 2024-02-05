#pragma once

#include <systemc.h>
#include "macros.h"

SC_MODULE(ModuleSelector) {

	sc_in<bool> clk_in;
	sc_in<bool> n_en_in;

	sc_out<bool> sel_out[MODULE_COUNT];

	void on_clk() {
		if (!n_en_in->read()) {
			for (int i = 0; i < MODULE_COUNT; i++) {
				sel_out->write(i == sel);
			}
			sel++;
			if (sel == MODULE_COUNT) {
				sel = 1;
			}
		}
	}

	SC_CTOR(ModuleSelector) {
		SC_METHOD(on_clk);
		sensitive << clk_in.pos() << n_en_in;
	}

private:
	int sel = 0;
};
