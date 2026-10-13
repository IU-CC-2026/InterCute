# InterCute
InterCute (Interpreter for Cution PL) interpretes and runs programs written in Cution PL

## Build and run

Build the project and print the AST for one file:

```bash
bash run.sh --ast tests/functions/oneliner.cuti
```

After building:

```bash
./cute_compiler --ast tests/loops/for.cuti
```

Run all examples from `tests/` and save their ASTs to `results_ast/`:

```bash
./cute_compiler --ast
```

Without `--ast`, the program prints source-like output.
