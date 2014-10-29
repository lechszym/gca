#include "test_genb.h"

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;
using namespace gca;

const unsigned int numTests = 10;

int main(int argc, char **argv) {

   double tot_time = 0;
   vector<Mvec> mvecs;

   srand(1);
   
   for (unsigned int i = 0; i < numTests; i++) {
      mvecs.push_back(generate_mvec(100,58));
   }

   for (size_t i = 0; i < mvecs.size(); i++) {
      for (size_t ii = 0; ii < mvecs.size(); ii++) {
         double elapsed_secs = 0;

               clock_t begin;
               clock_t end;

               Mvec A = mvecs[i];
               Mvec B = mvecs[ii];
               Mvec nA = ~A;
               Mvec nB = ~B;
               Mvec C;

               begin = clock();
               C = A&B;
               end = clock();
               elapsed_secs += double(end - begin);

               cout << "(" << A << ")&(" << B << ")=" << C << endl;

               begin = clock();
               C = A&nB;
               end = clock();
               elapsed_secs += double(end - begin);
               cout << "(" << A << ")&~(" << B << ")=" << C << endl;

               begin = clock();
               C = A^B;
               end = clock();
               elapsed_secs += double(end - begin);
               cout << "(" << A << ")^(" << B << ")=" << C << endl;

               begin = clock();
               C = A^nB;
               end = clock();
               elapsed_secs += double(end - begin);
               cout << "(" << A << ")^~(" << B << ")=" << C << endl;

               begin = clock();
               C = A*B;
               end = clock();
               elapsed_secs += double(end - begin);
               cout << "(" << A << ")*(" << B << ")=" << C << endl;
               
               begin = clock();
               C = A+nB;
               end = clock();
               elapsed_secs += double(end - begin);
               cout << "(" << A << ")*(" << B << ")=" << C << endl;
               
               begin = clock();
               C = A+B;
               end = clock();
               elapsed_secs += double(end - begin);
               cout << "(" << A << ")+(" << B << ")=" << C << endl;
               
               begin = clock();
               C = A+nB;
               end = clock();
               elapsed_secs += double(end - begin);
               cout << "(" << A << ")+~(" << B << ")=" << C << endl;

               begin = clock();
               C = A-B;
               end = clock();
               elapsed_secs += double(end - begin);
               cout << "(" << A << ")-(" << B << ")=" << C << endl;

               begin = clock();
               C = A-nB;
               end = clock();
               elapsed_secs += double(end - begin);
               cout << "(" << A << ")-~(" << B << ")=" << C << endl;
               
               //cout << "#-----------------------------------------------" << endl;
         //cout << "Time taken " << elapsed_secs / CLOCKS_PER_SEC << " s." << endl;
         //cout << "#-----------------------------------------------" << endl;
         tot_time += elapsed_secs;
      }
   }

   cout << "#-----------------------------------------------" << endl;
   cout << "Total time taken " << tot_time / CLOCKS_PER_SEC << " s." << endl << endl;
   cout << "#-----------------------------------------------" << endl;

   return 0;
}
