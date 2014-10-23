#include "test_genb.h"

#include <cstdlib>
#include <algorithm>

using namespace std;
using namespace gca;

Blade generate_blade(unsigned int dim) {
   
   vector<unsigned long> e;
   
   unsigned long nbases = rand() % (dim+1);

   unsigned long i=0;
   while(i < nbases) {
      unsigned long base = rand() % dim;
      base++;
      vector<unsigned long>::iterator ei = std::find(e.begin(),e.end(),base);
      if(ei == e.end()) {
         e.push_back(base);
         i++;
      }
   }
   std::sort(e.begin(),e.end());

   double v = (double) (rand() % 200) - 100;      
   
   return Blade(v, e);
}
