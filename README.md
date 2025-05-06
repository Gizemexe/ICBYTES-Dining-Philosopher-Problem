# Dining Philosophers Problem - GUI Simulation
The **Dining Philosophers Problem** is a classic concurrency problem proposed by Edsger Dijkstra in 1965 to illustrate synchronization issues in multithreaded systems.

In this problem, a number of philosophers sit around a circular table. Each philosopher alternates between thinking and eating. There is a fork between each pair of philosophers, and a philosopher needs both the left and right forks to eat. The challenge is to design a system where:
- No philosopher starves (i.e., everyone eventually gets to eat),
- No deadlock occurs (i.e., no circular wait on resources),
- The system is efficient and fair.

> The challenge is to prevent deadlock and starvation while allowing maximum concurrency.
  
This project is a graphical simulation of the classic Dining Philosophers problem using *C++* and the *ICBYTES* library.

The simulation demonstrates:
- A deadlock scenario where all philosophers are waiting for forks.
- A semaphore-based solution where philosophers eat without causing deadlock.

- **Critical Section**: A part of code where shared resources (e.g., forks) are accessed and must not be concurrently modified.
- **Mutual Exclusion**: Ensuring that no two philosophers can use the same fork at the same time.
- **Semaphore**: A low-level synchronization primitive used to control access to shared resources.
- **Deadlock**: A state where each philosopher holds one fork and waits indefinitely for the other, causing circular blocking.
- **Starvation**: A condition where some philosophers never get to eat due to resource hoarding by others.
- **Concurrency**: Managing multiple threads (philosophers) trying to perform actions (eating) simultaneously.

## Features

- Visual representation of philosophers, forks, and plate.
- Two execution modes:
  - **Deadlock Simulation**: Shows how improper resource handling leads to deadlock.
  - **Semaphore Simulation**: Uses semaphores to prevent deadlock and starvation.
- Supports up to 5 philosophers and forks around a circular table.

## Requirements

- Windows operating system
- C++ compiler with Windows API support
- [ICBYTES library](https://github.com/cembaykal/ICBYTES)

## Build Instructions

1. Clone or download the project.
2. Open the source code in your Visual Studio 2022.
3. Setup the ICBYTES library.
4. Build and run the application.

## Usage Instructions

1. Click **"Yükle"** to load and initialize the simulation.
2. To test deadlock behavior:
   - Click **"Deadlock Başlat"**
   - Watch how philosophers may enter a deadlock state
3. Close and restart the program to reset the state.
4. To run the semaphore-controlled version:
   - Click **"Yükle"**
   - Then click **"Semaphore Başlat"**
   - Observe philosophers eating in turns without deadlock
  
## Visuals
> Deadlock Simulation

![Deadlock (1) (2)](https://github.com/user-attachments/assets/f6299fda-c119-44e1-bd72-71d518c5b606)


> Semaphore Simulation

![Semaphore (1) (1)](https://github.com/user-attachments/assets/8bda3f3f-9d85-485c-8446-7aed19aa8534)


## Notes

- Forks are managed using individual semaphores.
- A global semaphore allows only 2 philosophers to eat at the same time.
- Philosophers must acquire both forks (left and right) before eating.

## License

This project is for educational use only.
