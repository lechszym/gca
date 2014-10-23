#include "Mvec.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

int main(int argc, char **argv) {

   double tot_time = 0;
   for (unsigned int d = 2; d < 13; d++) {
      double elapsed_secs = 0;
      double    *v = new double[d];
      
      cout << "# Testing dim " << d << endl;

      for(unsigned int j=0;j<d;j++) {
          v[j] = (double) (rand() % 101 - 50);
      }    
      
      Mvec m(v,d);

      clock_t begin;
      clock_t end;

      
      for(unsigned int j=0;j<d;j++) {
          for(unsigned int k=0;k<d;k++) {
              v[k] = (double) (rand() % 101 - 50);
          }    
          Mvec n(v,d);
          Mvec a(rand() % 101 - 50);
          n = n+a;
          m = m+n;
          m = m%100;
          cout << "(" << m << ")*(" << n << ")";
          begin = clock();
          m = m*n;
          end = clock();
          cout << "=" << m << endl;
          elapsed_secs += double(end - begin);
      }
      
      cout << "#-----------------------------------------------" << endl;
      cout << "Time taken " << elapsed_secs / CLOCKS_PER_SEC << " s." << endl;
      cout << "#-----------------------------------------------" << endl;
      tot_time += elapsed_secs;
      delete(v);
   }

   cout << "#-----------------------------------------------" << endl;
   cout << "Total time taken " << tot_time / CLOCKS_PER_SEC << " s." << endl << endl;
   cout << "#-----------------------------------------------" << endl;


   return 0;
}
