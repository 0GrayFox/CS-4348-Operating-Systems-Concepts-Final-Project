#ifndef BARRIER_H
#define BARRIER_H

#include <semaphore.h>

namespace synchronization {

// Provides a reusable barrier
class barrier {
private:
  sem_t mutex;
  sem_t mutex2;
  sem_t gate1;
  sem_t gate2;
  int count;
  int count2;
  int numOfThreads;
  // Declare your variables here

public:
  // Constructor
  barrier(int numberOfThreads);

  // Destructor
  ~barrier();

  // Function to wait at the barrier until all threads have reached the barrier
  void arriveAndWait(void);
  void barrier0(bool);
  
  
};

} // namespace synchronization

#endif
