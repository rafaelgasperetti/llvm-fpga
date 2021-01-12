cd ~
clear

echo "Iniciando instalação do projeto e dependências..."
echo "Configurar o SSH na máquina antes de rodar esse script. O ssh deve ser adicionado a sua conta do BitBucket."

if [ ! -d "llvm-fpga-home" ]; then
	echo "Criando a pasta llvm-fpga-home..."
	mkdir llvm-fpga-home
	echo "Criada a pasta llvm-fpga-home."
fi

cd llvm-fpga-home

if [ -d "$Configuracao.txt" ]; then
	echo "Criando arquivo de validação de configuração..."
	> Configuracao.txt
	echo "Criado arquivo de validação de configuração."
fi

if ! grep -q "dependencias instaladas" Configuracao.txt; then
	echo "Instalando dependências..."
	sudo apt-get install git
	sudo apt-get install cmake
	sudo apt-get install xclip -y
	sudo apt-get install subversion
	sudo apt-get install gcc
	sudo apt-get install clang
	sudo apt-get install build-essential
	sudo apt-get install libpng-dev
	sudo apt-get install zlib1g-dev
	sudo apt-get install python3
	sudo apt-get install doxygen
	sudo apt-get install python-glpk
	sudo apt-get install glpk-utils
	sudo apt-get install libncurses5-dev
	sudo apt-get install libclang-dev
	sudo apt-get install graphviz xdot
	sudo apt-get install libgnat-5
	sudo apt-get install gtkwave
	wget 'https://sourceforge.net/projects/ghdl-updates/files/Builds/ghdl-0.33/debian/ghdl_0.33-1jessie1_amd64.deb/download' -O 'ghdl.deb'
	sudo dpkg -i 'ghdl.deb'
	echo "dependencias instaladas ($(date +"%d/%m/%Y %T"))." >> Configuracao.txt
	echo "Dependências instaladas."
fi

if ! grep -q "lemon configurado" Configuracao.txt; then
	echo "Baixando código fonte versão 1.3.1 da biblioteca lemon..."
	wget  'lemon.cs.elte.hu/pub/sources/lemon-1.3.1.tar.gz'
	echo "Baixado o código fonte versão 1.3.1 da biblioteca lemon."
	echo "Extraindo a biblioteca lemon..."
	tar -xvzf lemon-1.3.1.tar.gz
	echo "Biblioteca lemon extraída."
	echo "Excluindo códifo fonte compactado da biblioteca lemon..."
	rm lemon-1.3.1.tar.gz
	echo "Código fonte compactado excluído."
	echo "Criando a pasta lemonbuild..."
	mkdir lemonbuild
	echo "Criada a pasta lemonbuild."
	echo "Criando a pasta lemoninstall..."
	mkdir lemoninstall
	echo "Criada a pasta lemoninstall."
	echo "Configurando a biblioteca lemon..."
	cd lemonbuild
	cmake -DCMAKE_INSTALL_PREFIX=../lemoninstall ../lemon-1.3.1
	cd ..
	echo "Configurada a biblioteca lemon."
	echo "lemon configurado ($(date +"%d/%m/%Y %T"))." >> Configuracao.txt
fi

echo "Compilando e instalando a biblioteca lemon em lemoninstall..."
cd lemonbuild
sudo make && make install -j4
echo "Instalada a biblioteca lemon em lemoninstall."
echo "Copiando a biblioteca lemon para a pasta usr..."
cd ../lemoninstall
sudo cp -a bin /usr
sudo cp -a include /usr
sudo cp -a lib /usr
sudo cp -a share /usr
cd ..
echo "Compilada e instalada a biblioteca lemon."

if ! grep -q "llvm-fpga clonado" Configuracao.txt; then
	echo "Clonando llvm-fpga de git@bitbucket.org:dc-ufscar/llvm-fpga.git em llvm-fpga-source..."
	git clone git@bitbucket.org:dc-ufscar/llvm-fpga.git llvm-fpga-source
	echo "llvm-fpga clonado."
	echo "Criando a pasta llvm-fpga-build..."
	mkdir llvm-fpga-build
	echo "Criada a pasta llvm-fpga-build."
	echo "Criando a pasta llvm-fpga-install..."
	mkdir llvm-fpga-install
	echo "Criada a pasta llvm-fpga-install."
	echo "Configurando o código do llvm-fpga..."
	cd llvm-fpga-build
	cmake -DCMAKE_INSTALL_PREFIX=../llvm-fpga-install ../llvm-fpga-source
	cd ..
	echo "Configurado o código do llvm-fpga..."
	echo "llvm-fpga clonado ($(date +"%d/%m/%Y %T"))." >> Configuracao.txt
fi

echo "Compilando o código do llvm-fpga..."
cd llvm-fpga-build
make && make install
cd ..
sudo cp ~/llvm-fpga-home/llvminstall/lib/libclang.so.3.9 /usr/lib
echo "Compilado o código do llvm-fpga..."

if ! grep -q "llvm configurado" Configuracao.txt; then
	echo "Criando a pasta llvmbuild..."
	mkdir llvmbuild
	echo "Criada a pasta llvmbuild."
	echo "Criando a pasta llvminstall..."
	mkdir llvminstall
	echo "Criada a pasta llvminstall."
	echo "Baixando código fonte da versão mais recente disponível do LLVM..."
	svn co http://llvm.org/svn/llvm-project/llvm/trunk llvmsource
	cd llvmsource/tools
	svn co http://llvm.org/svn/llvm-project/cfe/trunk clang
	cd ../projects
	svn co http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt
	cd ../tools/clang/tools
	svn co http://llvm.org/svn/llvm-project/clang-tools-extra/trunk extra
	echo "Baixado o código fonte do LLVM."
	echo "Configurando o LLVM..."
	cd ../../../../llvmbuild
	cmake -DCMAKE_INSTALL_PREFIX=../llvminstall ../llvmsource
	cd ..
	echo "LLVM configurado."
	echo "llvm configurado ($(date +"%d/%m/%Y %T"))." >> Configuracao.txt
fi

echo "Compilando o LLVM..."
cd llvmbuild
sudo make && make install -j4
echo "Compilado o LLVM."
echo "Projeto configurado e instalado."
