# Operating Systems: Homework and Quizzes

Theory submissions and C programming exercises completed during the fifth semester of my B.Sc. in Computer Engineering at Iran University of Science and Technology. Instructor: Dr. Reza Entezari-Maleki, with support from the teaching assistants. Author: Farzan Rahmani.

## Contents

| Path | Focus |
| --- | --- |
| `1_practical/` | Introductory C exercises and assignment statement |
| `2_practical/` | Process/thread sorting, server examples, and file-reading exercises |
| `3_practical/` | Concurrency and synchronization, including mutex/semaphore variants |
| `4_practical/` | Interprocess communication, pipe-based logging, and distributed merge-sort examples |
| `1_theory/`–`5_theory/` | Written homework statements and available submissions |
| `OS_quiz1/`–`OS_quiz4/` | Quiz code, including client/logger examples |

## Environment

Use a Linux/POSIX environment with a C compiler. Process, pthread, semaphore, and IPC APIs used in these exercises are not supported by native Windows C runtimes in the same way; use Linux, a Linux VM, or a suitable WSL environment.

There is no root Makefile or unified executable. Compile each exercise separately and inspect the statement/source for command-line arguments, input data, and any companion processes.

## Build Examples

From the repository root, compile the first C exercise:

```sh
gcc -Wall -Wextra 1_practical/1.c -o gift_exchange
./gift_exchange
```

It reads its input from the terminal. For pthread-based programs, include the threading flag when compiling, for example:

```sh
gcc -Wall -Wextra -pthread 3_practical/q2/with_mutex/q2_mutex.c -o matrix_mutex
```

Run IPC/client-server examples from the directory expected by their relative paths, and follow the assignment's process startup order. Avoid overwriting provided input files or treating saved executables as portable builds.

## Study Notes and License

These are historical learning exercises and alternative implementations, including synchronization experiments that should be checked independently for correctness. Reports and comments may be in Persian. See [LICENSE](LICENSE) for license terms.
