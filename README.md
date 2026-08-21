*This project has been created as part of the 42 curriculum by icoman.*

## Description
**Codexion** is a concurrent programming project inspired by the classic Dining Philosophers problem, adapted into a modern software development scenario. Instead of philosophers eating with forks, this simulation models coders attempting to work using shared resources (dongles). The primary goal of the project is to deepen the understanding of multi-threading, concurrency, mutexes, condition variables, and advanced scheduling algorithms in C.

Coders must acquire two adjacent dongles to perform their work cycle: compiling, debugging, and refactoring. A global monitor ensures that no coder burns out (starves) by enforcing strict timing deadlines. The simulation features two distinct schedulers (FIFO and EDF) utilizing priority queues to dictate dongle acquisition fairly and efficiently.

## Instructions

### Compilation
The project includes a `Makefile`. To compile the source code and generate the executable, simply run:
```bash
make
```
This will compile the files inside the `srcs` directory and produce the `codexion` executable. Additional rules like `make clean`, `make fclean`, and `make re` are also available for project maintenance.

### Execution
Run the simulation with the following arguments:
```bash
./codexion [number_of_coders] [time_to_burnout] [time_to_compile] [time_to_debug] [time_to_refactor] [number_of_compiles_required] [dongle_cooldown] [scheduler]
```
- **number_of_coders**: The number of coders (and dongles) in the simulation.
- **time_to_burnout**: Time (in ms) a coder can survive without compiling before burning out.
- **time_to_compile**: Time (in ms) it takes to compile (requires 2 dongles).
- **time_to_debug**: Time (in ms) it takes to debug (does not require dongles).
- **time_to_refactor**: Time (in ms) it takes to refactor (does not require dongles).
- **number_of_compiles_required**: The simulation stops if all coders reach this number of compiles.
- **dongle_cooldown**: Mandatory cooldown time (in ms) before a dongle can be used again.
- **scheduler**: The dongle acquisition policy. Must be `fifo` (First-In, First-Out) or `edf` (Earliest Deadline First).

## Resources
- **POSIX Threads (pthreads) Documentation**: Standard references for `pthread_create`, `pthread_mutex_init`, `pthread_cond_timedwait`, etc.
- **Dining Philosophers Problem**: Classic concurrency problems, resource hierarchies, and deadlock solutions.
- **Data Handling Concepts**: For EDF (Earliest Deadline First) and FIFO scheduling principles.
- **AI Usage**: AI was used during the development of this project primarily for generating code walkthroughs and assisting in the drafting of this README documentation.

## Blocking Cases Handled
Critical concurrency and synchronization issues addressed in this project's implementation:

- **Deadlock Prevention (Coffman's Conditions)**: Circular wait is prevented by enforcing a strict resource acquisition order. Coders with an even ID acquire their left dongle first, then the right. Coders with an odd ID acquire their right dongle first, then the left. This breaks the symmetry required for a deadlock.
- **Starvation Prevention**: Starvation is mitigated through our scheduling policies (FIFO/EDF). Dongles maintain an array-based priority queue (`t_queue_node`), ensuring that access is granted systematically based on wait time or urgency, rather than leaving it to the unpredictable nature of OS thread scheduling.
- **Cooldown Handling**: Implemented via a strict timestamp check (`dongle_cooldown`). Condition variables in `acquire_dongle` wake up threads, but they only proceed if the dongle's release time plus the cooldown period has fully elapsed.
- **Precise Burnout Detection**: A dedicated monitor thread polls the state of all coders at a high frequency. If `elapsed_ms(data) - last_compile >= time_to_burnout`, it immediately broadcasts a stop signal, logs the burnout, and halts the simulation cleanly.
- **Log Serialization**: All console output (`printf`) is strictly protected by a `log_mutex`. This ensures that messages from different threads do not interleave, preventing garbled output, and guarantees that the burnout message is definitively the final output before termination.

## Thread Synchronization Mechanisms
This implementation relies heavily on POSIX thread primitives to coordinate access to shared resources safely:

- **`pthread_mutex_t`**: Used to ensure mutual exclusion. 
  - Each dongle has its own mutex to protect its state variables (`available`, `release_time`, and `wait_queue`).
  - A `log_mutex` prevents terminal output race conditions.
  - A `stop_mutex` protects the global `sim_stop` flag to ensure thread-safe reads and writes across the monitor and coders.
  - A `state_mutex` for each coder protects its local state variables (`last_compile`, `compile_count`) from concurrent reads by the monitor and writes by the coder thread, preventing data races.
- **`pthread_cond_t`**: Used for signaling state changes efficiently without busy-waiting. When a coder finishes with a dongle, `pthread_cond_broadcast` wakes up all waiting threads. 
- **Custom Event Queue / Wait Loop**: The waiting threads use `pthread_cond_timedwait` (inside `wait_dongle_loop`) to suspend execution. They wake up and attempt to claim the dongle only when:
  1. The dongle is available.
  2. The coder is at the head of the priority queue (`queue_peek`).
  3. The cooldown has expired.
If the conditions are not met, they recalculate their wait time and go back to sleep, achieving robust thread-safe communication between the monitor, the coders, and the shared dongles.
