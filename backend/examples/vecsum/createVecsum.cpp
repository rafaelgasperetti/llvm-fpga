/*
*	-------------------------------------------------------------
*	Test file, creates max.vhdl
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
#include "components/cmp_add_op_s.h"
#include "components/cmp_block_ram.h"
#include "components/cmp_counter.h"
#include "components/cmp_delay_op.h"

int main() {
	std::vector<Component*> components;
	std::vector<Signal*> signals;

	// Instatiates the necessary components and pushes to a vector
	Cmp_delay_op* delay_op = new Cmp_delay_op(32);
	delay_op->setInstanceName("\\c13\\");
	components.push_back(delay_op);
	Cmp_delay_op* delay_op2 = new Cmp_delay_op(32);
	delay_op2->setInstanceName("\\c14\\");
	components.push_back(delay_op2);
	Cmp_delay_op* delay_op3 = new Cmp_delay_op(32);
	delay_op3->setInstanceName("\\c15\\");
	components.push_back(delay_op3);
	Cmp_add_op_s*  add_op = new Cmp_add_op_s(32);
	add_op->setInstanceName("\\c_add_op_s_y\\");
	components.push_back(add_op);
	Cmp_block_ram* ram_z = new Cmp_block_ram(32);
	ram_z->setInstanceName("\\z\\");
	components.push_back(ram_z);
	Cmp_block_ram* ram_x = new Cmp_block_ram(32);
	ram_x->setInstanceName("\\x\\");
	components.push_back(ram_x);
	Cmp_block_ram* ram_y = new Cmp_block_ram(32);
	ram_y->setInstanceName("\\y\\");
	components.push_back(ram_y);
	Cmp_counter* counter = new Cmp_counter(32);
	counter->setInstanceName("\\i\\");
	components.push_back(counter);

	// Creates the signals
	Signal* s0 = new Signal("s0", "std_logic_vector", 16);
	signals.push_back(s0);
	Signal* s1 = new Signal("s1", "std_logic_vector", 16);
	signals.push_back(s1);
	Signal* s2 = new Signal("s2", "std_logic", 1);
	signals.push_back(s2);
	Signal* s6 = new Signal("s6", "std_logic_vector", 32);
	signals.push_back(s6);
	Signal* s7 = new Signal("s7", "std_logic_vector", 32);
	signals.push_back(s7);
	Signal* s8 = new Signal("s8", "std_logic_vector", 32);
	signals.push_back(s8);
	Signal* s9 = new Signal("s9", "std_logic_vector", 32);
	signals.push_back(s9);
	Signal* s10 = new Signal("s10", "std_logic_vector", 16);
	signals.push_back(s10);
	Signal* s11 = new Signal("s11", "std_logic_vector", 16);
	signals.push_back(s11);
	Signal* s12 = new Signal("s12", "std_logic", 1);
	signals.push_back(s12);
	Signal* s13 = new Signal("s13", "std_logic_vector", 1);
	signals.push_back(s13);
	Signal* s14 = new Signal("s14", "std_logic", 1);
	signals.push_back(s14);
	Signal* s15 = new Signal("s15", "std_logic_vector", 1);
	signals.push_back(s15);

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
	counter->connectPortSignal(counter->getPort(0), s0);
	counter->connectPortSignal(counter->getPort(1), s1);
	counter->connectPortSignal(counter->getPort(2), clkSignal);
	counter->connectPortSignal(counter->getPort(3), s2);
	counter->connectPortSignal(counter->getPort(4), resetSignal);
	counter->connectPortSignal(counter->getPort(6), s12);
	counter->connectPortSignal(counter->getPort(7), s14);
	counter->connectPortSignal(counter->getPort(8), s10);

	add_op->connectPortSignal(add_op->getPort(0), s6);
	add_op->connectPortSignal(add_op->getPort(1), s7);
	add_op->connectPortSignal(add_op->getPort(2), s8);

	ram_x->connectPortSignal(ram_x->getPort(0), clkSignal);
	ram_x->connectPortSignal(ram_x->getPort(3), s10, "10", "0");
	ram_x->connectPortSignal(ram_x->getPort(5), s6);

	ram_y->connectPortSignal(ram_y->getPort(0), clkSignal);
	ram_y->connectPortSignal(ram_y->getPort(3), s10, "10", "0");
	ram_y->connectPortSignal(ram_y->getPort(5), s7);

	ram_z->connectPortSignal(ram_z->getPort(0), clkSignal);
	ram_z->connectPortSignal(ram_z->getPort(2), s13);
	ram_z->connectPortSignal(ram_z->getPort(3), s11, "10", "0");
	ram_z->connectPortSignal(ram_z->getPort(4), s8);
	ram_z->connectPortSignal(ram_z->getPort(5), s9);

	delay_op->connectPortSignal(delay_op->getPort(0), s10);
	delay_op->connectPortSignal(delay_op->getPort(1), clkSignal);
	delay_op->connectPortSignal(delay_op->getPort(2), resetSignal);
	delay_op->connectPortSignal(delay_op->getPort(3), s11);

	delay_op2->connectPortSignal(delay_op2->getPort(0), s12, "0");
	delay_op2->connectPortSignal(delay_op2->getPort(1), clkSignal);
	delay_op2->connectPortSignal(delay_op2->getPort(2), resetSignal);
	delay_op2->connectPortSignal(delay_op2->getPort(3), s13);

	delay_op3->connectPortSignal(delay_op3->getPort(0), s14, "0");
	delay_op3->connectPortSignal(delay_op3->getPort(1), clkSignal);
	delay_op3->connectPortSignal(delay_op3->getPort(2), resetSignal);
	delay_op3->connectPortSignal(delay_op3->getPort(3), s15);

	// Set the generic maps
	counter->setGenericMap(counter->getMap(0), "16");
	counter->setGenericMap(counter->getMap(1), "1");
	counter->setGenericMap(counter->getMap(2), "1");
	counter->setGenericMap(counter->getMap(3), "0");
	counter->setGenericMap(counter->getMap(4), "0");

	add_op->setGenericMap(add_op->getMap(0), "32");
	add_op->setGenericMap(add_op->getMap(1), "32");
	add_op->setGenericMap(add_op->getMap(2), "32");

	ram_x->setGenericMap(ram_x->getMap(0), "32");
	ram_x->setGenericMap(ram_x->getMap(1), "11");

	ram_y->setGenericMap(ram_y->getMap(0), "32");
	ram_y->setGenericMap(ram_y->getMap(1), "11");

	ram_z->setGenericMap(ram_z->getMap(0), "32");
	ram_z->setGenericMap(ram_z->getMap(1), "11");

	delay_op->setGenericMap(delay_op->getMap(0), "16");
	delay_op->setGenericMap(delay_op->getMap(1), "2");

	delay_op2->setGenericMap(delay_op2->getMap(0), "1");
	delay_op2->setGenericMap(delay_op2->getMap(1), "2");

	delay_op3->setGenericMap(delay_op3->getMap(0), "1");
	delay_op3->setGenericMap(delay_op3->getMap(1), "4");


	// Assign signals
	std::vector<std::string> assignedSignals;
	Signal::assignSignals("s2", "\\init\\", assignedSignals);
	Signal::assignSignals("\\done\\", "s15(0)", assignedSignals);
	Signal::assignSignals("s1", "conv_std_logic_vector(2048, 16)", assignedSignals);
	Signal::assignSignals("s0", "conv_std_logic_vector(0, 16)", assignedSignals);
	Signal::assignSignals("\\result\\", "s9", assignedSignals);

	VHDLGen::Instance()->writeVhdlFile("vecsum", components, signals, assignedSignals);

	return 0;
}
