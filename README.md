# External Sorting Algorithms

A C program that implements and benchmarks external sorting algorithms for large datasets. Part of the BCC203 course at UFOP (Federal University of Ouro Preto).

## Overview

This project implements three external sorting algorithms that process student registration records stored on disk, sorting them by grade when the data exceeds available main memory. Each record contains:

- Registration number (`inscricao`)
- Grade (`nota`)
- State (`estado`)
- City (`cidade`)
- Course (`curso`)

## Algorithms

- **Balanced Merge Sort (Intercalação Balanceada)** – Classic external merge sort using multiple tapes. Reads records into memory blocks, sorts them, distributes across output tapes, then merges them back iteratively until the file is fully sorted.

- **Balanced Merge Sort with Selective Replacement (Intercalação Balanceada com Substituição por Seleção)** – An optimized variant that uses a heap-based selection mechanism during the merge phase to reduce the number of tape passes.

- **External QuickSort (QuickSort Externo)** – A disk-based partitioning approach that uses an in-memory area to hold elements during partitioning, writing smaller elements to one subfile and larger elements to another, then recursing on each partition.

## Authors

- **Kaua Souza Morais**
- **Marcos Vinicius Sacramento Dos Santos**
- **Paulo Gabriel Marques Jardim**
- **Pedro Henrique Fonseca Peixoto**
- **Yuri Henrique Gomes dos Reis**

## Project Structure

```
├── src/            # Implementation files
│   ├── main.c              # Entry point
│   ├── Executavel.c        # CLI validation, timer, orchestration
│   ├── IntBalanceada.c     # Balanced Merge Sort implementation
│   ├── QuickSortExterno.c  # External QuickSort implementation
│   └── arquivos.c          # File I/O, tape management, record parsing
├── include/          # Header files
│   ├── struct.h          # Data structures and constants
│   ├── Executavel.h      # Executable interface
│   ├── IntBalanceada.h   # Merge sort interface
│   ├── QuickSortExterno.h# External QuickSort interface
│   └── arquivos.h        # File operations interface
├── test/             # Test and benchmark scripts
│   ├── test-run.sh       # Test runner
│   ├── bench.sh          # Benchmark script
│   └── verifica.c        # Verification utility
├── data/             # Input/output data directory
├── logs/             # Test log files
├── build/            # Compiled objects and binaries
├── tmp_testes/       # Temporary test data
└── Makefile          # Build configuration
```

## Building

```bash
make
```

This compiles the project and produces the `ordena` executable in the project root.

## Usage

```bash
./ordena <method> <quantity> <situation> [-P]
```

### Arguments

| Argument      | Values           | Description                   |
| ------------- | ---------------- | ----------------------------- |
| **method**    | `1`, `2`, or `3` | Sorting algorithm (see below) |
| **quantity**  | Positive integer | Number of records to process  |
| **situation** | `1`, `2`, or `3` | Input file type (see below)   |
| **-P**        | Optional         | Print records to stdout       |

### Methods

| Value | Algorithm                                      |
| ----- | ---------------------------------------------- |
| `1`   | Balanced Merge Sort                            |
| `2`   | Balanced Merge Sort with Selective Replacement |
| `3`   | External QuickSort                             |

### Situations

| Value | Input File Type       |
| ----- | --------------------- |
| `1`   | Ascending order file  |
| `2`   | Descending order file |
| `3`   | Random order file     |

### Example

```bash
./ordena 1 1000 3
./ordena 3 5000 1 -P
```

## Testing

Run the test suite:

```bash
make test
```

This executes the program with default parameters and runs the verification utility to check the output. Logs are saved in the `logs/` directory.

## Benchmarking

Run the full benchmark suite:

```bash
make bench
```

This runs multiple test configurations with scaled sizes and repeated executions. Performance metrics include:

- **Read transfers** – Number of disk read operations
- **Write transfers** – Number of disk write operations
- **Comparisons** – Number of record comparisons performed
- **Execution time** – Wall-clock time in seconds

## Verification

A standalone verification tool is compiled alongside the main program:

```bash
./build/verifica <situation> <quantity>
```

## Memory Management

To check for memory leaks:

```bash
make valgrind
```

## Cleaning

```bash
make clean
```

Removes compiled objects, binaries, and log files.

## Implementation Details

- **Record size**: Each `Registro` struct holds a student record with fields for registration, grade, state, city, and course.
- **Block size**: 4096 bytes (`BLOCK_SIZE`) – the number of records per I/O block.
- **Tapes**: 20 tape pairs (40 total files) are used for the merge sort implementation.
- **Memory area**: The External QuickSort uses a fixed in-memory area of 20 records (`TAMAREA`) for partitioning.
- **Timer**: Execution time is measured using `CLOCK_MONOTONIC` for high-resolution timing.
