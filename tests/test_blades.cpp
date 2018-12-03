#include "test_genb.h"

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;
using namespace gca;

const unsigned int numTests = 50;

int main(int argc, char **argv) {

   int dim[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 21, 33, 58};
   double tot_time = 0;
   vector<Blade<> > blades[14];
   //Bladed* blades[14];

   for (int i = 0; i < 14; i++) {
      for (unsigned int j = 0; j < numTests; j++) {
         blades[i].push_back(generate_blade(dim[i]));
      }
   }

   for (int i = 0; i < 14; i++) {
      for (int ii = 0; ii < 14; ii++) {
         double elapsed_secs = 0;

         cout << endl << "# Testing dim " << dim[i] << " on " << dim[ii] << endl;
         cout << "#-----------------------------------------------" << endl;

         for (unsigned int j = 0; j < numTests; j++) {
            for (unsigned int jj = 0; jj < numTests; jj++) {

               clock_t begin;
               clock_t end;

               Blade<> A = blades[i][j];
               Blade<> B = blades[ii][jj];
               Blade<> nA = ~A;
               Blade<> nB = ~B;
               Blade<> C;

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
            }
         }
         cout << "#-----------------------------------------------" << endl;
         cout << "Time taken " << elapsed_secs / CLOCKS_PER_SEC << " s." << endl;
         cout << "#-----------------------------------------------" << endl;
         tot_time += elapsed_secs;
      }
   }

   cout << "#-----------------------------------------------" << endl;
   cout << "Total time taken " << tot_time / CLOCKS_PER_SEC << " s." << endl << endl;
   cout << "#-----------------------------------------------" << endl;

   return 0;
}
