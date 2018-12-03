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

const unsigned int numTests = 10;

int main(int argc, char **argv) {

   vector<Mvec<float>> mvecs;

   srand(1);
   
   for (unsigned int i = 0; i < numTests; i++) {
      mvecs.push_back(generate_mvec(20,1,1));
   }

   for (size_t i = 0; i < mvecs.size(); i++) {
      
      Mvec<float> A = mvecs[i];
      Mvec<float> nA = ~A;
      Mvec<float> C,D;
      C = A*A;
      D = C[0];
      if( !(C == D)) {
         cout << "Error\n";
         return -1;
      }
      
      C = A/A;
      D = Mvec<float>(Bladef(1));
      if( !(C == D)) {
         cout << "Error\n";
         return -1;
      }
      
      for (size_t ii = 0; ii < mvecs.size(); ii++) {

               Mvec<float> B = mvecs[ii];
               Mvec<float> nB = ~B;
               
               Mvec<float> AB = A*B;
               Mvec<float> BA = B*A;
               Mvec<float> AdB = A&B;
               Mvec<float> BdA = B&A;
               Mvec<float> AwB = A^B;
               Mvec<float> BwA = B^A;
               
               C = (AB+BA)*0.5;
               if( !(C == AdB) ) {
                  cout << "Error!\n";
                  //cout << " (" << A << ")*(" << B << ")=(" << AdB << "), but\n";
                  //cout << " (" << B << ")&(" << A << ")=(" << BdA << ").\n";
                  //cout << "Should be the same\n";
                  return -1;
               }
               
               C = (AB-BA)*0.5;
               if( !(C == AwB) ) {
                  cout << "Error!\n";
                  //cout << " (" << A << ")*(" << B << ")=(" << AdB << "), but\n";
                  //cout << " (" << B << ")&(" << A << ")=(" << BdA << ").\n";
                  //cout << "Should be the same\n";
                  return -1;
               }
                   
               C = (A/B)*B;
               if( !(C == A) ) {
                  cout << "Error!\n";
                  return -1;
               }
               
               /*C = -AB;
               if( !(C == BA) ) {
                  cout << "Error!\n";
                  cout << " (AB)=" << AB << "\n";
                  cout << " (BA)=" << C << "\n";
                  //cout << "Should be the same\n";
                  return -1;
               }*/
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
