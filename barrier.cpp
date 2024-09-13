#include "barrier.h"

namespace synchronization {
  // Constructor
barrier::barrier(int numberOfThreads) {
  // Write your code here
  sem_init(&mutex, 0, 1);
  sem_init(&mutex2, 0, 1);
  sem_init(&gate1, 0, 0);
  sem_init(&gate2, 0, 1);
  count = 0;
  count2 = 0;
  numOfThreads = numberOfThreads;
  return;
}
//destroy barrier
barrier::~barrier() {
  // Write your code here
  sem_destroy(&mutex);
  sem_destroy(&mutex2);
  sem_destroy(&gate1);
  sem_destroy(&gate2);
  return;
}
  //barrier0 to wait for all threads to arrive and remove all the threads in systematic manner
void barrier::barrier0(bool x) {
  if (x) {
    // Write your code here

    sem_wait(&mutex);
    // mutex to lock the count
    count++;
    // increment the count value to count all the threads
    if (count == numOfThreads) {
      // checks if the count value is equal to the number of threads
      sem_wait(&gate2);
      // lock gate2 to make sure it doesn't run
      sem_post(&gate1);
      // unlocks gate 1 which starts the chain of threads and starts creating
      // running the different threads
    }
    sem_post(&mutex);
    // mutex to unlock the count
    sem_wait(&gate1);
    // make sure gate 1 locks so that threads n-1 doesn't run
    sem_post(&gate1);
    // make sure gate 1 unlocks so that threads n-1 do run
  } else {
    sem_wait(&mutex);
    // mutex to lock the count
    count--;
    // decrement the count value to make sure all threads are passed through
    if (count == 0) {
      // checks if count is equal to 0
      sem_wait(&gate1);
      // makes sure the gate 1 is locked which means that threads can't go and
      // loop back
      for (int i = 0; i <= numOfThreads; i++) {
        sem_post(&gate2);
        // unlocks gate 2 which makes sure threads can go through multiple times
        // depending on number of threads
      }
    }
    // sem_post(&gate2);
    sem_post(&mutex);
    // mutex to unlock the count
    sem_wait(&gate2);
    // locks the gate 2 for the next phase to run
  }
  return;
}
  //arriveanwait to allow barrier function to run
void barrier::arriveAndWait(void) {
  // Write your code here
  barrier0(1);
  barrier0(0);
  // critical section
  return;
}

} // namespace synchronization
