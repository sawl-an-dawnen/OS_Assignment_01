#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
/*
class PPG {
   int size;

   public:
   int **ary = new int*[size];
   PPG (int k);//constructor
   bool addEdge (int x, int y);
   bool deleteEdge (int x, int y);
};
PPG::PPG (int k)  {
   size = k;
   for (int i = 0; i < k; i++) {
      ary[i] = new int[k];
   }
   for (int i = 0; i < k; i++) {
      for (int j = 0; i < k; i++) {
         ary[i][j] = 0;
      }
   }
}
bool PPG::addEdge  (int x, int y)   {
   ary[x][y] = 1;
   return true;
}
bool PPG::deleteEdge  (int x, int y)   {
   ary[x][y] = 0;
   return true;
}
*/

#define PROCESS_NUM 10

int main(int argc, char** argv) {

   int pids[PROCESS_NUM];
   int pipes[PROCESS_NUM + 1][2];
   //making pipes for each process +1 for the parent pipe
   for (int i = 0; i < PROCESS_NUM + 1; i++) {
      if (pipe(pipes[i]) == -1) {
         cout << "ERROR CREATING PIPES..." << endl;
         return 1;
      }
   }

   //creating process with for loop
   for (int i = 0; i < PROCESS_NUM; i++)   {
      pids[i] = fork(); //creates child process and stores ID in array

      //process creation validation
      if (pids[i] == -1)   {
         cout << "ERROR CREATING PROCESS..." << endl;
         return 2;
      }

      //child process
      if (pids[i] == 0) {  

         //variables being using in process
         int x;
         
         //each process needs to close the majority of the pipes created in it's process.
         for (int j = 0; j < PROCESS_NUM + 1; j++) {
            //close reading end of pipe
            //this will leave the reading end of the pipe associated with the i'th process open
            if (i != j) {
                  close(pipes[j][0]);
            }
            //close writing end of pipe
            //this will leave the writing end the next pipe open for writing from the i'th process
            if (i + 1 != j)   {
                  close(pipes[j][1]);
            }
         }
         
         //READING PROCESS with read validation
         //in the i'th process we're reading from the i'th pipe (index 0), and storing the value in x
         if (read(pipes[i][0], &x, sizeof(int)) == -1)   {
            cout << "ERROR READING..." << endl;
            return 3;
         }

         //debug check
         cout << i << " GOT " << x << endl; 

         //function
         x++;

         //WRITING PROCESS with read validation
         //in the i'th process we're writing to the i'th + 1 pipe's writing (index 1), with the value x once it's been modified
         if (write(pipes[i + 1][1], &x, sizeof(int)) == -1)   {
            cout << "ERROR WRITING..." << endl;
            return 4;
         }

         //debug check
         cout << i << " SENT " << x << endl;

         //closing the remaining pipes that we're used in the current process
         //close read
         close(pipes[i][0]);
         //close write
         close(pipes[i + 1][1]);

         return 0; //could be break too if there is more i'd like it to do
      }
   }

   for (int j = 0; j < PROCESS_NUM + 1; j++) {
      if (j != PROCESS_NUM) {
         close(pipes[j][0]);
      }
      if (j != 0)   {
         close(pipes[j][1]);
      }
   }

   // Main process

   //value sent into the function
   int y = 5;
   cout << "MAIN PROCESS SENDS: " << y << endl;

   //Main process writes to the first pipe
   if (write(pipes[0][1], &y, sizeof(int)) == -1) {
      cout << "ERROR WRITING..." << endl;
      return 4;
   }

   //Main process reads from the last pipe
   if (read(pipes[PROCESS_NUM][0], &y, sizeof(int)) == -1) {
      cout << "ERROR READING..." << endl;
      return 3;
   }

   //result check
   cout << "FINAL RESULT: " << y << endl;

   //closing pipes opened by the main process
   close(pipes[0][1]);
   close(pipes[PROCESS_NUM][0]);

   //wating for all child processes to finish execution
   for (int i = 0; i < PROCESS_NUM; i++) {
      wait(NULL);
   }

   return 0;
}
