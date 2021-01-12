library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;
entity vecsum is
	port(
		\init\	: in	std_logic;
		\done\	: out	std_logic;
		\result\	: out	std_logic_vector(31 downto 0);
		\clk\		: in	std_logic;
		\reset\	: in	std_logic;
		\clear\	: in	std_logic
	); 
end vecsum;

architecture behavior of vecsum is 

component delay_op 
generic ( 
        bits        : integer := 8; 
        delay       : integer := 1 
); 
port ( 
        a		: in	std_logic_vector(bits-1 downto 0); 
        clk		: in	std_logic; 
        reset	: in	std_logic; 
        a_delayed	: out	std_logic_vector(bits-1 downto 0) := (others=>'0') 
); 
end component; 

component add_op_s
generic (
	w_in1	: integer := 8;
	w_in2	: integer := 8;
	w_out	: integer := 16
);
port (
	I0	: in	std_logic_vector(w_in1-1 downto 0);
	I1	: in	std_logic_vector(w_in2-1 downto 0);
	O0	: out	std_logic_vector(w_out-1 downto 0)
);
end component;

component block_ram 
generic ( 
        data_width          : integer := 8; 
        address_width	: integer := 8 
); 
port ( 
        clk                 : in	std_logic; 
        we                  : in	std_logic := '0'; 
        oe                  : in	std_logic := '1'; 
        address             : in	std_logic_vector(address_width-1 downto 0); 
        data_in             : in	std_logic_vector(data_width-1 downto 0) := (others => '0'); 
        data_out            : out	std_logic_vector(data_width-1 downto 0) 
); 
end component; 

component block_ram_x 
generic ( 
        data_width          : integer := 8; 
        address_width	: integer := 8 
); 
port ( 
        clk                 : in	std_logic; 
        we                  : in	std_logic := '0'; 
        oe                  : in	std_logic := '1'; 
        address             : in	std_logic_vector(address_width-1 downto 0); 
        data_in             : in	std_logic_vector(data_width-1 downto 0) := (others => '0'); 
        data_out            : out	std_logic_vector(data_width-1 downto 0) 
); 
end component; 

component block_ram_y 
generic ( 
        data_width          : integer := 8; 
        address_width	: integer := 8 
); 
port ( 
        clk                 : in	std_logic; 
        we                  : in	std_logic := '0'; 
        oe                  : in	std_logic := '1'; 
        address             : in	std_logic_vector(address_width-1 downto 0); 
        data_in             : in	std_logic_vector(data_width-1 downto 0) := (others => '0'); 
        data_out            : out	std_logic_vector(data_width-1 downto 0) 
); 
end component; 

component counter 
generic (
        bits		: integer := 8;
        steps		: integer := 1;
        increment           : integer := 1;
        down                : integer := 0;
        condition           : integer := 0
);
port (
        input		: in	std_logic_vector(bits-1 downto 0);
        termination         : in	std_logic_vector(bits-1 downto 0);
        clk                 : in	std_logic;
        clk_en		: in	std_logic := '1';
        reset		: in	std_logic;
        load		: in	std_logic := '0';
        step		: out	std_logic;
        done		: out	std_logic;
        output		: out	std_logic_vector(bits-1 downto 0)
); 
end component; 

signal s0	: std_logic_vector(15 downto 0);
signal s1	: std_logic_vector(15 downto 0);
signal s2	: std_logic;
signal s6	: std_logic_vector(31 downto 0);
signal s7	: std_logic_vector(31 downto 0);
signal s8	: std_logic_vector(31 downto 0);
signal s9	: std_logic_vector(31 downto 0);
signal s10	: std_logic_vector(15 downto 0);
signal s11	: std_logic_vector(15 downto 0);
signal s12	: std_logic;
signal s13	: std_logic_vector(0 downto 0);
signal s14	: std_logic;
signal s15	: std_logic_vector(0 downto 0);

begin

\c13\: delay_op
generic map (
	bits => 16,
	delay => 2
)
port map (
	a => s10,
	clk => \clk\,
	reset => \reset\,
	a_delayed => s11
);

\c14\: delay_op
generic map (
	bits => 1,
	delay => 2
)
port map (
	a(0) => s12,
	clk => \clk\,
	reset => \reset\,
	a_delayed => s13
);

\c15\: delay_op
generic map (
	bits => 1,
	delay => 4
)
port map (
	a(0) => s14,
	clk => \clk\,
	reset => \reset\,
	a_delayed => s15
);

\c_add_op_s_y\: add_op_s
generic map (
	w_in1 => 32,
	w_in2 => 32,
	w_out => 32
)
port map (
	I0 => s6,
	I1 => s7,
	O0 => s8
);

\z\: block_ram
generic map (
	data_width => 32,
	address_width => 11
)
port map (
	clk => \clk\,
	we => s13(0),
	address(10 downto 0) => s11(10 downto 0),
	data_in => s8,
	data_out => s9
);

\x\: block_ram_x
generic map (
	data_width => 32,
	address_width => 11
)
port map (
	clk => \clk\,
	address(10 downto 0) => s10(10 downto 0),
	data_out => s6
);

\y\: block_ram_y
generic map (
	data_width => 32,
	address_width => 11
)
port map (
	clk => \clk\,
	address(10 downto 0) => s10(10 downto 0),
	data_out => s7
);

\i\: counter
generic map (
	bits => 16,
	steps => 1,
	increment => 1,
	down => 0,
	condition => 0
)
port map (
	input => s0,
	termination => s1,
	clk => \clk\,
	clk_en => s2,
	reset => \reset\,
	step => s12,
	done => s14,
	output => s10
);

s2 <= \init\;
\done\ <= s15(0);
s1 <= conv_std_logic_vector(2048, 16);
s0 <= conv_std_logic_vector(0, 16);
\result\ <= s9;
end behavior;