#include "test_genb.h"

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <sstream>
#include <ctime>
#include <sys/time.h>

using namespace std;
using namespace gca;

const unsigned int numTests = 5;

double timediff(struct timeval &start, struct timeval &end) {
   return ((end.tv_sec - start.tv_sec) * 1000000u +
           end.tv_usec - start.tv_usec) / 1.e6;
}

int main(int argc, char **argv) {

   double tot_time = 0;
   vector<Mvecd> mvecs;

   srand(1);
   
   for (unsigned int i = 0; i < numTests; i++) {
      mvecs.push_back(generate_mvec(100,58));
   }

   for (size_t i = 0; i < mvecs.size(); i++) {
      for (size_t ii = 0; ii < mvecs.size(); ii++) {
         double elapsed_secs = 0;

               struct timeval start, end;

               Mvecd A = mvecs[i];
               Mvecd B = mvecs[ii];
               Mvecd nA = ~A;
               Mvecd nB = ~B;
               Mvecd C;

               gettimeofday(&start, NULL);
               C = A&B;
               gettimeofday(&end, NULL);
               elapsed_secs += timediff(start,end);

               //cout << "(" << A << ")&(" << B << ")=" << C << endl;

               gettimeofday(&start, NULL);
               C = A&nB;
               gettimeofday(&end, NULL);
               elapsed_secs += timediff(start,end);

               //cout << "(" << A << ")&~(" << B << ")=" << C << endl;

               gettimeofday(&start, NULL);
               C = A^B;
               gettimeofday(&end, NULL);
               elapsed_secs += timediff(start,end);
               //cout << "(" << A << ")^(" << B << ")=" << C << endl;

               gettimeofday(&start, NULL);
               C = A^nB;
               gettimeofday(&end, NULL);
               elapsed_secs += timediff(start,end);
               //cout << "(" << A << ")^~(" << B << ")=" << C << endl;

               gettimeofday(&start, NULL);
               C = A*B;
               gettimeofday(&end, NULL);
               elapsed_secs += timediff(start,end);
               //cout << "(" << A << ")*(" << B << ")=" << C << endl;
               
               gettimeofday(&start, NULL);
               C = A+nB;
               gettimeofday(&end, NULL);
               elapsed_secs += timediff(start,end);
               //cout << "(" << A << ")*(" << B << ")=" << C << endl;
               
               gettimeofday(&start, NULL);
               C = A+B;
               gettimeofday(&end, NULL);
               elapsed_secs += timediff(start,end);
               //cout << "(" << A << ")+(" << B << ")=" << C << endl;
               
               gettimeofday(&start, NULL);
               C = A+nB;
               gettimeofday(&end, NULL);
               elapsed_secs += timediff(start,end);               
               //cout << "(" << A << ")+~(" << B << ")=" << C << endl;

               gettimeofday(&start, NULL);
               C = A-B;
               gettimeofday(&end, NULL);
               elapsed_secs += timediff(start,end);
               //cout << "(" << A << ")-(" << B << ")=" << C << endl;

               gettimeofday(&start, NULL);
               C = A-nB;
               gettimeofday(&end, NULL);
               elapsed_secs += timediff(start,end);
               //cout << "(" << A << ")-~(" << B << ")=" << C << endl;
               
               //cout << "#-----------------------------------------------" << endl;
         //cout << "Time taken " << elapsed_secs / CLOCKS_PER_SEC << " s." << endl;
         //cout << "#-----------------------------------------------" << endl;
         tot_time += elapsed_secs;
      }
   }

   cout << "#-----------------------------------------------" << endl;
   printf("Total time taken %.1fs\n", tot_time);
   cout << "#-----------------------------------------------" << endl;

   return 0;
}
