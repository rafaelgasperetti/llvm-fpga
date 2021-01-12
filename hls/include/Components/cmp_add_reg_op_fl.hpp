/* Copyright (c) 2009 Ricardo Menotti, All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its 
 * documentation for NON-COMERCIAL purposes and without fee is hereby granted 
 * provided that this copyright notice appears in all copies.
 *
 * RICARDO MENOTTI MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY
 * OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR 
 * NON-INFRINGEMENT. RICARDO MENOTTI SHALL NOT BE LIABLE FOR ANY DAMAGES 
 * SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING THIS 
 * SOFTWARE OR ITS DERIVATIVES. 
 */

package br.ufscar.dc.lalp.components;

import br.ufscar.dc.lalp.core.Generic;
import br.ufscar.dc.lalp.core.IComponent;
import br.ufscar.dc.lalp.core.Parameters;
import br.ufscar.dc.lalp.core.Port;
import br.ufscar.dc.lalp.core.PortType;

/**
 * registered add with signed operands
 * 
 * @author <a href="http://menotti.pro.br/">Ricardo Menotti</a>


 * @author <a href="http://www.dc.ufscar.br/">DC/UFSCar</a>
 * @version September, 2007
 */
public class add_reg_op_fl extends IComponent {
	private long initialValue = 0;
	public add_reg_op_fl() {
		this(Parameters.getDefaultDataWidth());
	}
	public add_reg_op_fl(int width) {
		super(width);
		Generic generic;
		super.addPort(new Port("clk", PortType.INPUT));
		super.addPort(new Port("reset", PortType.INPUT));
		super.addPort(new Port("we", PortType.INPUT));
		super.addPort(new Port("Sel1", PortType.INPUT, 1), true);
		generic = new Generic("w_in1", "integer", 16);
		super.addGeneric(generic);
		super.addPort(new Port("I0", PortType.INPUT, width, generic));
		generic = new Generic("w_in2", "integer", 16);
		super.addGeneric(generic);
		super.addPort(new Port("I1", PortType.INPUT, width, generic));
		generic = new Generic("w_out", "integer", 32);
		super.addGeneric(generic);
		super.addPort(new Port("O0", PortType.OUTPUT, width, generic), true);
		generic = new Generic("initial", "integer", 0);
		super.addGeneric(generic);
	}
	public add_reg_op_fl(long width) {
		this((int)width);
	}
	public add_reg_op_fl(String name) {
		this();
		super.name = name;
	}
	public add_reg_op_fl(String name, int width) {
		this(width);
		super.name = name;
	}
	public add_reg_op_fl(String name, long width) {
		this(width);
		super.name = name;
	}
	public String getVHDLDeclaration() {
		String d = new String();
		d += "component add_reg_op_fl\n";  
		d += "generic (\n";
		d += "	w_in1	: integer := 32;\n";
		d += "	w_in2	: integer := 32;\n";
		d += "	w_out	: integer := 32;\n";
		d += "	initial	: integer := 0\n";
		d += ");\n";
		d += "port (\n";
		d += "	clk	: in	std_logic;\n";
		d += "	reset	: in	std_logic;\n";
		d += "	we	: in	std_logic := '1';\n";
		d += "	Sel1	: in	std_logic_vector(0 downto 0) := \"1\";\n";
		d += "	I0	: in	std_logic_vector(w_in1-1 downto 0);\n";
		d += "	I1	: in	std_logic_vector(w_in2-1 downto 0);\n";
		d += "	O0	: out	std_logic_vector(w_out-1 downto 0)\n";
		d += ");\n";
		d += "end component;\n";
		d += "\n";
		return d;
	}
	public void setInitialValue(int initialValue) {
		this.initialValue = initialValue;
		getGeneric("initial").setValue(initialValue);
	}
	public void setInitialValue(long initialValue) {
		this.initialValue = initialValue;
		getGeneric("initial").setValue((int)initialValue);
	}
	public long getInitialValue() {
		return this.initialValue;
	}
//	public String getDotName() {
//		return "+";
//	}
}
