extern "C" {
	#include "clang-c/Index.h"
}
#include "llvm/Support/CommandLine.h"
#include <iostream>
#include <string>
#include "algorithms/vhdlGen.h"
#include "core/design.h"
#include "core/component.h"
#include "core/signal.h"
#include "components/cmp_add_reg_op_s.h"
#include "components/cmp_block_ram.h"
#include "components/cmp_counter.h"
#include "components/cmp_delay_op.h"
#include "components/cmp_mult_op_s.h"

using namespace llvm;

// Command line namespace, receives a single paramater that
// stores the bitcode filename
static cl::opt<std::string>

FileName(cl::Positional, cl::desc("Input file"), cl::Required);

// Hardware design of the program
Design design;
std::vector<std::string> assignedSignals;
std::vector<std::string> declaredNames;

Cmp_counter* _counter;
Cmp_add_reg_op_s* _add_reg;

// TODO: Make it work for more than one loop, maybe a vector?
CXCursorSet forStmtSet = clang_createCXCursorSet();

// Instantiates rams for Arrays
// TODO: Account for different sizes of memories
void visitArray(CXCursor cursor, CXCursor parent,
			    CXClientData client_data, CXType cursorType){				
					
	CXString name = clang_getCursorSpelling(cursor);
	std::string stringName = clang_getCString(name);
	clang_disposeString(name);
	
	Cmp_block_ram* ram = new Cmp_block_ram(32);
	ram->setInstanceName(stringName);
	declaredNames.push_back(ram->getInstanceName());
	std::cout << "instantiating block_ram: " << stringName << std::endl;
	design.addComponent(ram, cursor, stringName);	
	design.addToNumberOfComponents("cmp_block_ram");
	
	// Connecting signals
	design.connectPortSignal(ram, ram->getPort("clk"), design.getSignal("clk"));

	// Setting generic maps
	ram->setGenericMap(ram->getMap("address_width"), "11");
	ram->setGenericMap(ram->getMap("data_width"), "32");
}	

void visitInt(CXCursor cursor, CXCursor parent,
			  CXClientData client_data, CXType cursorType){
	
	CXString name = clang_getCursorSpelling(cursor);
	std::string stringName = clang_getCString(name);
	clang_disposeString(name);
	
	design.addVariableName(stringName);
	std::cout << "Recognized int: " << stringName << std::endl;
}

// Instantiates counters for ForStmt
// TODO: Different names for each counter, account for counter
// parameters
void visitForStmt(CXCursor cursor){
	Cmp_counter* counter = new Cmp_counter(32);
	counter->setInstanceName("\\counter" +
	                         to_string(design.getNumberOfComponents("cmp_counter")) + 
							 "\\");
	std::cout << "instantiating counter: " << counter->getInstanceName() << std::endl;
	design.addComponent(counter, cursor, counter->getInstanceName());
	design.addToNumberOfComponents("cmp_counter");

	_counter = counter;

	// Connecting clk and rst
	design.connectPortSignal(counter, counter->getPort("clk"), design.getSignal("clk"));
	design.connectPortSignal(counter, counter->getPort("reset"), design.getSignal("reset"));
	
	// TODO: Make input and termination values variable
	Signal* signal0 = design.createUnnamedSignal("std_logic_vector", 32);;
	Signal* signal2048 = design.createUnnamedSignal("std_logic_vector", 32);;
	Signal::assignSignals(signal2048->getSignalName(), "conv_std_logic_vector(2048, 32)", assignedSignals);
	Signal::assignSignals(signal0->getSignalName(), "conv_std_logic_vector(0, 32)", assignedSignals);
	design.connectPortSignal(counter, counter->getPort("input"), signal0);
	design.connectPortSignal(counter, counter->getPort("termination"), signal2048);
	design.connectPortSignal(counter, counter->getPort("clk_en"), design.getSignal("init"));
	design.connectPortSignal(counter, counter->getPort("clk"), design.getSignal("clk"));
	design.connectPortSignal(counter, counter->getPort("reset"), design.getSignal("reset"));

	// Setting generic maps
	counter->setGenericMap(counter->getMap(0), "32");
	counter->setGenericMap(counter->getMap(1), "1");
	counter->setGenericMap(counter->getMap(2), "1");
	counter->setGenericMap(counter->getMap("down"), "0");
	counter->setGenericMap(counter->getMap("condition"), "0");
}

void visitMultOperator(std::string tokenSpelling, CXCursor cursor){
	Cmp_mult_op_s* mult = new Cmp_mult_op_s(32);
	mult->setInstanceName("\\mult" +
						  to_string(design.getNumberOfComponents("cmp_mult_op_s")) + 
						  "\\");
	std::cout << "instantiating multiplier: " << mult->getInstanceName() << std::endl;
	design.addComponent(mult, cursor, mult->getInstanceName());
	design.addToNumberOfComponents("cmp_mult_op_s");
	
	int i = 0;
	std::string var1, var2;

	// Checking operands
	int portsConnected = 0;
	for(int i = 0; i < design.components.size(); i++){
		// TODO: Find other way to compare
		if(design.components[i].component->getInstanceName().find("adder") != -1){
			if(mult->getInstanceName().compare(design.components[i].component->getInstanceName())){
				std::cout << "Connecting ports from : " << mult->getInstanceName() << " to "
							<<  design.components[i].component->getInstanceName() << std::endl;
				Signal* signal = design.createUnnamedSignal("std_logic_vector", 32);
				design.connectPorts(mult, 
									mult->getPort("O0"),
									design.components[i].component,
									design.components[i].component->getPort("I1"),
									signal);	
			}
		}

		if(tokenSpelling.find(design.components[i].component->getInstanceName()) != -1){
			std::cout << "Connecting ports : "  << mult->getInstanceName() << " to "
				      <<  design.components[i].component->getInstanceName() << std::endl;
			Signal* signal = design.createUnnamedSignal("std_logic_vector", 32);
			design.connectPorts(mult, 
								mult->getPort("I" + to_string(portsConnected++)),
								design.components[i].component,
								design.components[i].component->getPort("data_out"),
								signal);
		}
	}

	// Setting generic maps
	mult->setGenericMap(mult->getMap("w_in1"), "32");
	mult->setGenericMap(mult->getMap("w_in2"), "32");
	mult->setGenericMap(mult->getMap("w_out"), "32");
}

// TODO: Other types of compound sums
void visitCompoundSum(std::string tokenSpelling, CXCursor cursor){
	Cmp_add_reg_op_s* add_reg = new Cmp_add_reg_op_s(32);
	add_reg->setInstanceName("\\adder" +
							 to_string(design.getNumberOfComponents("cmp_add_reg_op_s")) + 
							 "\\");
	std::cout << "instantiating add_reg(compound): " << add_reg->getInstanceName() << std::endl;
	design.addComponent(add_reg, cursor, add_reg->getInstanceName());
	design.addToNumberOfComponents("cmp_add_reg_s");
	
	_add_reg = add_reg;

	// Connecting clk and rst
	design.connectPortSignal(add_reg, add_reg->getPort("clk"), design.getSignal("clk"));
	design.connectPortSignal(add_reg, add_reg->getPort("reset"), design.getSignal("reset"));

	// Connecting the output in one of the inputs
	Signal* signal = design.createUnnamedSignal("std_logic_vector", 32);
	design.connectPortSignal(add_reg, add_reg->getPort("I0"), signal);
	design.connectPortSignal(add_reg, add_reg->getPort("O0"), signal);

	// TODO: Find return to connect to result
	Signal::assignSignals("\\result\\", signal->getSignalName(), assignedSignals);

	// Setting generic maps
	add_reg->setGenericMap(add_reg->getMap("initial"), "0");
	add_reg->setGenericMap(add_reg->getMap("w_in1"), "32");
	add_reg->setGenericMap(add_reg->getMap("w_in2"), "32");
	add_reg->setGenericMap(add_reg->getMap("w_out"), "32");
}

void visitLoopArray(std::string tokenSpelling, CXCursor cursor){
	std::string var;
	int index = 0;

	// TODO: Catch an error to create
	Signal* signal1 = design.getSignal("s00"); 
	if(signal1->getSignalName().compare("s00")){ 
		signal1 = new Signal("s00", "std_logic_vector", 32); 
		design.addNamedSignal(signal1, "s00");
	}

	// Sequence to identify the variables(working for arrays)
	while(tokenSpelling.compare(index, 1, " ") != 0){
		var += tokenSpelling[index];
		index++;
	}
	if(tokenSpelling.compare(index, 3, "[ i") &&
		clang_CXCursorSet_contains(forStmtSet, cursor))
	{
		for(int i = 0; i < design.components.size(); i++){
			CXCursor componentCursor = design.components[i].nodeReference;
			CXCursorKind componentCursorKind = clang_getCursorKind(componentCursor);
			
			// TODO: Make it work for more than one ForStmt, different widths
			if(clang_CXCursorSet_contains(forStmtSet, componentCursor) &&
				componentCursorKind == CXCursor_ForStmt)
			{
				design.connectPortSignal(design.components[i].component,
										 design.components[i].component->getPort("output"), 
										 signal1);
				std::cout << "Connecting port : "  
				          << design.components[i].component->getInstanceName() 
						  << " to " << signal1->getSignalName() << endl;
				
				for(int j = 0; j < design.components.size(); j++){
					Component* selectedComponent = design.components[j].component;
					if(!selectedComponent->getInstanceName().compare(var)){
						design.connectPortSignal(selectedComponent,
												 selectedComponent->getPort("address"), 
												 signal1, 
												"10", 
										 		"0");
				
						std::cout << "Connecting port : "  
						          << design.components[j].component->getInstanceName() 
							  	  << " to " << signal1->getSignalName() << endl;
					}
				}
				break;
			}	
		}	
	}
}

std::string getTokenSpelling(CXCursor cursor){
	
	CXSourceRange range = clang_getCursorExtent(cursor);
	CXTranslationUnit translationUnit = clang_Cursor_getTranslationUnit(cursor);
	CXToken *tokens = 0;
	unsigned int nTokens = 0;
	clang_tokenize(translationUnit, range, &tokens, &nTokens);
	
	std::string tokenSpelling;
	std::string auxString;
	
	for (unsigned int i = 0; i < nTokens; i++)
	{
		CXString spelling = clang_getTokenSpelling(translationUnit, tokens[i]);
		auxString = clang_getCString(spelling);
		tokenSpelling += auxString + " ";
		clang_disposeString(spelling);
	}
	clang_disposeTokens(translationUnit, tokens, nTokens);
	
	return tokenSpelling;	
}

// Function defines what to do when visiting each node.
enum CXChildVisitResult visitNode (CXCursor cursor, CXCursor parent,
						    	   CXClientData client_data) {	
	CXType cursorType = clang_getCursorType(cursor);
	CXCursorKind cursorKind = clang_getCursorKind(cursor);

	CXType parentType = clang_getCursorType(parent);
	CXCursorKind parentKind = clang_getCursorKind(parent);

	if(clang_CXCursorSet_contains(forStmtSet, parent)){
		clang_CXCursorSet_insert(forStmtSet, cursor);
	}

	// If is an for stmt
	if (cursorKind == CXCursor_ForStmt) {
		clang_CXCursorSet_insert(forStmtSet, cursor);
		visitForStmt(cursor);
	}

	if (cursorKind == CXCursor_VarDecl) {
		// If variable is an array 
		if(cursorType.kind == CXType_ConstantArray| 
		   cursorType.kind == CXType_VariableArray|
		   cursorType.kind == CXType_DependentSizedArray){		
			
			visitArray(cursor, parent, client_data, cursorType);
		}
		
		if(cursorType.kind == CXType_Int){
			visitInt(cursor, parent, client_data, cursorType);
		}
	}
	
	// If is a variable operator
	if	(cursorKind == CXCursor_BinaryOperator) { 
		std::string tokenSpelling = getTokenSpelling(cursor);	 
		
		if(parentKind == CXCursor_ForStmt){
			std::cout << "Operator for ForStmt, ignoring for now" << std::endl;
			return CXChildVisit_Continue;
		}
		
		if(tokenSpelling.find("*") != -1){
			visitMultOperator(tokenSpelling, cursor);
		}	
	}
	
	if (cursorKind == CXCursor_CompoundAssignOperator) {
		std::string tokenSpelling = getTokenSpelling(cursor);	 
		//std::cout << tokenSpelling << std::endl;
		
		if(tokenSpelling.find("+=") != -1){
			visitCompoundSum(tokenSpelling, cursor);
		}
	} 

	CXString string = clang_getCursorKindSpelling(cursorKind);
	if(cursorKind == CXCursor_ArraySubscriptExpr){
		std::string tokenSpelling = getTokenSpelling(cursor);
		visitLoopArray(tokenSpelling, cursor);
	}

	return CXChildVisit_Recurse;
}

int main(int argc, char** argv)
{
	cl::ParseCommandLineOptions(argc, argv, "AST Traversal Example");
	CXIndex index = clang_createIndex(0, 0);
	
	const char *args[] = {
		"-I/usr/include",
		"-I."
	};
	
	CXTranslationUnit translationUnit = clang_parseTranslationUnit
		(index, FileName.c_str(), args, 2, NULL, 0,
		 CXTranslationUnit_None);
	CXCursor cur = clang_getTranslationUnitCursor(translationUnit);
	
	// clang_visistChildren visits recursively all child nodes of the cursor
	// passed as a parameter, calling a callback function in each visit
	clang_visitChildren(cur, visitNode, NULL);
	clang_disposeTranslationUnit(translationUnit);
	clang_disposeIndex(index);

	// Putting only the components on the vector
	std::vector<Component*> componentArray;
	for(int i = 0; i < design.components.size(); i++){
		componentArray.push_back(design.components[i].component);
	}

	// Putting only the necessary signals on the vector
	// std::vector<Signal*> signalArray;
	// for(int i = 0; i < design.signals.size(); i++){
	// 	if(design.signals[i]->getSignalName().find("//") != -1)
	// 		signalArray.push_back(design.signals[i]);
	// }

	// Delays specific to the example (no scheduler yet)
	Cmp_delay_op* delay_op = new Cmp_delay_op(32);
	delay_op->setInstanceName("\\dly_16\\");
	componentArray.push_back(delay_op);
	Cmp_delay_op* delay_op2 = new Cmp_delay_op(32);
	delay_op2->setInstanceName("\\dly_17\\");
	componentArray.push_back(delay_op2);

	delay_op->setGenericMap(delay_op->getMap(0), "1");
	delay_op->setGenericMap(delay_op->getMap(1), "2");

	delay_op2->setGenericMap(delay_op2->getMap(0), "1");
	delay_op2->setGenericMap(delay_op2->getMap(1), "4");



	Signal* s8 = design.createUnnamedSignal("std_logic", 1);
	delay_op->connectPortSignal(delay_op->getPort(0), s8, "0");
	_counter->connectPortSignal(_counter->getPort(6), s8);
	delay_op->connectPortSignal(delay_op->getPort(1), design.getSignal("clk"));
	delay_op->connectPortSignal(delay_op->getPort(2), design.getSignal("reset"));
	Signal* s9 = design.createUnnamedSignal("std_logic", 1);
	_add_reg->connectPortSignal(_add_reg->getPort(2), s9);
	delay_op->connectPortSignal(delay_op->getPort(3), s9);

	Signal* s11 = design.createUnnamedSignal("std_logic", 1);
	delay_op2->connectPortSignal(delay_op2->getPort(0), s11, "0");
	_counter->connectPortSignal(_counter->getPort(7), s11);
	delay_op2->connectPortSignal(delay_op2->getPort(1), design.getSignal("clk"));
	delay_op2->connectPortSignal(delay_op2->getPort(2), design.getSignal("reset"));
	delay_op2->connectPortSignal(delay_op2->getPort(3), design.getSignal("done"));

	VHDLGen::Instance()->writeVhdlFile("programa", 
									   componentArray, 
									   design.signals, 
									   assignedSignals);
	
	return 0;
}
