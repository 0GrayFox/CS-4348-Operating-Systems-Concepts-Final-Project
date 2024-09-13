/* This program solves the prefix sum problem using a parallel algorithm */

#include <iostream>
#include <math.h>
#include <string>
#include <thread>

#include "barrier.h"

int sizeOfInput = 1000;
int numberOfThreads = 8;
int numberOfPhases = 0;

// An array to store the input array, output array and intermediate results
int **data;

// An array to store information about worker threads
std::thread *workers;

// A reusable barrier for worker threads to synchronize
synchronization::barrier *rendevousz;

// The function executed by a worker thread
void doWork(int myid) {

  // compute the size of a chunk
  int sizeOfChunk = sizeOfInput / numberOfThreads;

  // compute the beginning and ending indices of my chunk
  int mystart = myid * sizeOfChunk;
  int myfinish =
      myid == numberOfThreads - 1 ? sizeOfInput : mystart + sizeOfChunk;

  // used by the prefix sum algorithm
  int fixed = 1;

  for (int i = 0; i < numberOfPhases; ++i) {
    // execute phase i
   // std::cout << "Thread " << myid << " Entering the Phase"<< i<< " " << std::endl;
    for (int j = mystart; j < myfinish; ++j) {
      // compute the new values for my chunk
      if (j < fixed) {

        // simply copy the entry
        data[i + 1][j] = data[i][j];
      } else {
        // compute the sum of two specific entries
        data[i + 1][j] = data[i][j] + data[i][j - fixed];
      }
    }
   // std::cout << " Exiting the Thread "<< myid<< " from phase " << i << std::endl;
    // double the value of fixed
    fixed = 2 * fixed;
    
    // Wait until all threads have completed their work before moving to the
    // next phase
    //std::cout<< "Thread " << myid << " Waiting for phase"<< i<< std::endl;
    rendevousz->arriveAndWait();
    
    //for (int jj = 0; jj <= numberOfPhases; ++jj) {
     // for (int ii = 0; ii < sizeOfInput; ++ii) {
      //  std::cout << data[jj][ii]<<" ";
      //}
     // std::cout << "" << std::endl;
    //}
    //std::cout << "" << std::endl;
  }

  return;
}

// The main function
int main(int argc, char **argv) {

  if (argc < 3) {
    std::cerr << "Too few arguments" << std::endl;
    exit(1);
  }

  // Read the size of the input array (n) and the number of worker threads (p)
  // to use
  sizeOfInput = std::stoi(argv[1]);
  numberOfThreads = std::stoi(argv[2]);

  // Note that an exception will be thrown if one of the arguments is not an
  // integer

  if (sizeOfInput <= 0 || numberOfThreads <= 0) {
    std::cerr << "One of the arguments is invalid" << std::endl;
    exit(1);
  }

  // Calculate the number of phases (m) in the algorithm, given by ceiling of
  // log2(n)
  numberOfPhases = ceil(log2(sizeOfInput));

  // Allocate space to store pointers to m+1 arrays
  //    data[0] stores the input array
  //    data[m] stores the output array
  //    data[1]...data[m-1] store intermediate results
  //    data[i+1] is computed from data[i] in phase i, where i = 0,...,m-1
  data = new int *[numberOfPhases + 1];

  // Allocate space for each of the m+1 individual arrays
  for (int i = 0; i < numberOfPhases + 1; ++i) {
    data[i] = new int[sizeOfInput];
  }

  // Initialize elements of the input array (data[0])
  for (int j = 0; j < sizeOfInput; ++j) {
    data[0][j] = 1;
  }

  // Create a barrier object to be used by worker threads to synchronize
  // movement from one phase to the next
  //     a thread can advance to phase i+1 only after ALL threads have completed
  //     phase i
  rendevousz = new synchronization::barrier(numberOfThreads);

  // Create worker threads
  //    first parameter is the name of the function each thread has to execute
  //    second parameter if the identifier of the worker thread

  workers = new std::thread[numberOfThreads];

  for (int i = 0; i < numberOfThreads; ++i)
    workers[i] = std::thread(doWork, i);

  std::cout << "Completed forking all threads" << std::endl;
  
  // Wait for all worker threads to terminate
  //    It is crucial to join to all the child threads in order to wait for them
  //    to complete If the main thread terminates before the child threads, it
  //    will force termination all the child threads
  for (int i = 0; i < numberOfThreads; ++i)
    workers[i].join();

  // Print the output array
  for (int j = 0; j < sizeOfInput; ++j)
    std::cout << "output[" << j << "] = " << data[numberOfPhases][j]
              << std::endl;

  // Cleanup: deallocate all dyamically allocated objects
  
  
  for (int i = 0; i < numberOfThreads; ++i) {
    delete[] workers;
    workers = NULL;
  }

  for (int i = 0; i < numberOfPhases + 1; ++i) {
    delete[] data[i];
    data[i] = NULL;
  }
  

  delete[] data;
  data = NULL;

  delete rendevousz;
  rendevousz = NULL;

  return 0;
}
