# Instalation passes:
- Run the 'llvm-fpga.sh' shell file at the repository, it will coenload and compile Lemon and LLVM, as well as it dependencies;
- The erros related to ILOG, COIN e SOPLEX libraries at `ckmake` phase for the Lemon library can all be ignored.
- To compile the llvm-fpga project, the following commands needs to be executed:

```
cd llvm-fpga-build
cmake -DCMAKE_INSTALL_PREFIX=../llvm-fpga-install ../llvm-fpga-source
make && make install
```

### For now it will not work because the HLS project still on its own, and will be together with the other libraries and compilation when it's ready.

# Unsupported C resources at input code:
- Functions or methods outside main;
- Non SSA (single static assignment) code;
- Templates;
