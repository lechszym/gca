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

int main(int argc, char **argv) {

   vector<Mvec> mvecs;

   srand(1);
   
   for (unsigned int i = 0; i < numTests; i++) {
      mvecs.push_back(generate_mvec(4,4));
   }

   for (size_t i = 0; i < mvecs.size(); i++) {
      for (size_t ii = 0; ii < mvecs.size(); ii++) {

               Mvec A = mvecs[i];
               Mvec B = mvecs[ii];
               Mvec nA = ~A;
               Mvec nB = ~B;
               Mvec C;
               
               Mvec AB = A*B;
               Mvec BA = B*A;
               Mvec AdB = A&B;
               Mvec BdA = B&A;
               Mvec AwB = A^B;
               Mvec BwA = B^A;
               
               if( !(AdB == BdA)) {
                  cout << "Error!\n";
                  cout << " (" << A << ")&(" << B << ")=(" << AdB << "), but\n";
                  cout << " (" << B << ")&(" << A << ")=(" << BdA << ").\n";
                  cout << "Should be the same\n";
                  return -1;
               }
               
               //C = (AB+BA)*0.5;
               //if(!(C == AdB)) {
               //   cout << "Error \n";
               //}
               
               
               
               
               //cout << "(" << A << ")&(" << B << ")=" << C << endl;

               //C = A&nB;

               //cout << "(" << A << ")&~(" << B << ")=" << C << endl;

               //C = A^B;
               //cout << "(" << A << ")^(" << B << ")=" << C << endl;

               //C = A^nB;
               //cout << "(" << A << ")^~(" << B << ")=" << C << endl;

               //C = A*B;
               //cout << "(" << A << ")*(" << B << ")=" << C << endl;
               
               //C = A+nB;
               //cout << "(" << A << ")*(" << B << ")=" << C << endl;
               
               //C = A+B;
               //cout << "(" << A << ")+(" << B << ")=" << C << endl;
               
               //C = A+nB;
               //cout << "(" << A << ")+~(" << B << ")=" << C << endl;

               //C = A-B;
               //cout << "(" << A << ")-(" << B << ")=" << C << endl;

               //C = A-nB;
               //cout << "(" << A << ")-~(" << B << ")=" << C << endl;
               
      }
   }

   return 0;
}
