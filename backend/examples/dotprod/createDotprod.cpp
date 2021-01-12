/*
*	-------------------------------------------------------------
*	Test file, creates dotprod.vhdl
*	-------------------------------------------------------------
*/

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "algorithms/vhdlGen.h"
#include "core/component.h"
#include "core/port.h"
#include "core/signal.h"
#include "core/genericMap.h"
#include "components/cmp_add_reg_op_s.h"
#include "components/cmp_block_ram.h"
#include "components/cmp_counter.h"
#include "components/cmp_delay_op.h"
#include "components/cmp_mult_op_s.h"

int main() {
	std::vector<Component*> components;
	std::vector<Signal*> signals;

	// Instatiates the necessary components and pushes to a vector
	Cmp_add_reg_op_s* add_reg = new Cmp_add_reg_op_s(32);
	add_reg->setInstanceName("\\sum\\");
	components.push_back(add_reg);
	Cmp_block_ram* ram_a = new Cmp_block_ram(32);
	ram_a->setInstanceName("\\a\\");
	components.push_back(ram_a);
	Cmp_block_ram* ram_b = new Cmp_block_ram(32);
	ram_b->setInstanceName("\\b\\");
	components.push_back(ram_b);
	Cmp_counter* counter = new Cmp_counter(32);
	counter->setInstanceName("\\i\\");
	components.push_back(counter);
	Cmp_delay_op* delay_op = new Cmp_delay_op(32);
	delay_op->setInstanceName("\\dly_16\\");
	components.push_back(delay_op);
	Cmp_delay_op* delay_op2 = new Cmp_delay_op(32);
	delay_op2->setInstanceName("\\dly_17\\");
	components.push_back(delay_op2);
	Cmp_mult_op_s* mult = new Cmp_mult_op_s(32);
	mult->setInstanceName("\\a_mult_op_s_b\\");
	components.push_back(mult);

	// Creates the signals
	Signal* s1 = new Signal("s1", "std_logic_vector", 32);
	signals.push_back(s1);
	Signal* s2 = new Signal("s2", "std_logic_vector", 32);
	signals.push_back(s2);
	//Signal* s3 = new Signal("s3", "std_logic_vector", 32);
	//signals.push_back(s3);
	Signal* s4 = new Signal("s4", "std_logic_vector", 32);
	signals.push_back(s4);
	Signal* s5 = new Signal("s5", "std_logic_vector", 32);
	signals.push_back(s5);
	Signal* s6 = new Signal("s6", "std_logic_vector", 32);
	signals.push_back(s6);
	Signal* s7 = new Signal("s7", "std_logic_vector", 32);
	signals.push_back(s7);
	Signal* s8 = new Signal("s8", "std_logic", 1);
	signals.push_back(s8);
	Signal* s9 = new Signal("s9", "std_logic_vector", 1);
	signals.push_back(s9);
	Signal* s10 = new Signal("s10", "std_logic", 1);
	signals.push_back(s10);
	Signal* s11 = new Signal("s11", "std_logic", 1);
	signals.push_back(s11);
	Signal* s12 = new Signal("s12", "std_logic_vector", 1);
	signals.push_back(s12);
	Signal* s13 = new Signal("s13", "std_logic_vector", 32);
	signals.push_back(s13);

	// Creates the entity ports and signals
	Port* initPort = new Port("\\init\\", "in", "std_logic", "1");
	Port* donePort = new Port("\\done\\", "out", "std_logic", "1");
	Port* resultPort = new Port("\\result\\", "out", "std_logic_vector", "32");
	//Port* clkPort = new Port("clk", "in", "std_logic", "1");
	Signal* clkSignal = new Signal("\\clk\\", "std_logic", 1);
	//Port* resetPort = new Port("reset", "in", "std_logic", "1");
	Signal* resetSignal = new Signal("\\reset\\", "std_logic", 1);
	Port* clear = new Port("\\clear\\", "in", "std_logic", "1");

	// Connect the signals to the ports
	counter->connectPortSignal(counter->getPort(0), s7);
	counter->connectPortSignal(counter->getPort(1), s6);
	counter->connectPortSignal(counter->getPort(2), clkSignal);
	counter->connectPortSignal(counter->getPort(3), s10);
	counter->connectPortSignal(counter->getPort(4), resetSignal);
	counter->connectPortSignal(counter->getPort(6), s8);
	counter->connectPortSignal(counter->getPort(7), s11);
	counter->connectPortSignal(counter->getPort(8), s1);

	add_reg->connectPortSignal(add_reg->getPort(0), clkSignal);
	add_reg->connectPortSignal(add_reg->getPort(1), resetSignal);
	add_reg->connectPortSignal(add_reg->getPort(2), s9);
	add_reg->connectPortSignal(add_reg->getPort(4), s13);
	add_reg->connectPortSignal(add_reg->getPort(5), s2);
	add_reg->connectPortSignal(add_reg->getPort(6), s13);

	mult->connectPortSignal(mult->getPort(0), s4);
	mult->connectPortSignal(mult->getPort(1), s5);
	mult->connectPortSignal(mult->getPort(2), s2);

	ram_a->connectPortSignal(ram_a->getPort(0), clkSignal);
	ram_a->connectPortSignal(ram_a->getPort(3), s1, "10", "0");
	ram_a->connectPortSignal(ram_a->getPort(5), s4);

	ram_b->connectPortSignal(ram_b->getPort(0), clkSignal);
	ram_b->connectPortSignal(ram_b->getPort(3), s1, "10", "0");
	ram_b->connectPortSignal(ram_b->getPort(5), s5);

	delay_op->connectPortSignal(delay_op->getPort(0), s8, "0");
	delay_op->connectPortSignal(delay_op->getPort(1), clkSignal);
	delay_op->connectPortSignal(delay_op->getPort(2), resetSignal);
	delay_op->connectPortSignal(delay_op->getPort(3), s9);

	delay_op2->connectPortSignal(delay_op2->getPort(0), s11, "0");
	delay_op2->connectPortSignal(delay_op2->getPort(1), clkSignal);
	delay_op2->connectPortSignal(delay_op2->getPort(2), resetSignal);
	delay_op2->connectPortSignal(delay_op2->getPort(3), s12);

	// Set the generic maps
	counter->setGenericMap(counter->getMap(0), "32");
	counter->setGenericMap(counter->getMap(1), "1");
	counter->setGenericMap(counter->getMap(2), "1");
	counter->setGenericMap(counter->getMap(3), "0");
	counter->setGenericMap(counter->getMap(4), "0");

	add_reg->setGenericMap(add_reg->getMap(0), "0");
	add_reg->setGenericMap(add_reg->getMap(1), "32");
	add_reg->setGenericMap(add_reg->getMap(2), "32");
	add_reg->setGenericMap(add_reg->getMap(3), "32");

	mult->setGenericMap(mult->getMap(0), "32");
	mult->setGenericMap(mult->getMap(1), "32");
	mult->setGenericMap(mult->getMap(2), "32");

	ram_a->setGenericMap(ram_a->getMap(0), "32");
	ram_a->setGenericMap(ram_a->getMap(1), "11");

	ram_b->setGenericMap(ram_b->getMap(0), "32");
	ram_b->setGenericMap(ram_b->getMap(1), "11");

	delay_op->setGenericMap(delay_op->getMap(0), "1");
	delay_op->setGenericMap(delay_op->getMap(1), "2");

	delay_op2->setGenericMap(delay_op2->getMap(0), "1");
	delay_op2->setGenericMap(delay_op2->getMap(1), "4");

	// Assign signals
	std::vector<std::string> assignedSignals;
	Signal::assignSignals("s10", "\\init\\", assignedSignals);
	Signal::assignSignals("\\done\\", "s12(0)", assignedSignals);
	Signal::assignSignals("s6", "conv_std_logic_vector(2048, 32)", assignedSignals);
	Signal::assignSignals("s7", "conv_std_logic_vector(0, 32)", assignedSignals);
	Signal::assignSignals("\\result\\", "s13", assignedSignals);

	VHDLGen::Instance()->writeVhdlFile("dotprod", components, signals, assignedSignals);

	return 0;
}
