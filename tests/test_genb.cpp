#include "test_genb.h"

#include <cstdlib>
#include <algorithm>

using namespace std;
using namespace gca;

Blade generate_blade(unsigned int maxGrade, unsigned int minGrade, unsigned int maxBase) {
   
   vector<unsigned long> e;
   
   if(maxBase == 0) {
      maxBase = maxGrade;
   }
   
   unsigned long grade;
   while(1) {
      grade = rand() % (maxGrade+1);
      if(grade >= minGrade) {
         break;
      }
   }
      
   unsigned long i=0;
   while(i < grade) {
      unsigned long base = rand() % maxBase;
      base++;
      //cout << "base=" << base << "\n";
      
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

Mvec generate_mvec(unsigned int maxBlades, unsigned int maxGrade, unsigned int minGrade) {
   
   blades_t blades;
   
   unsigned long nblades = maxBlades;
   if(maxBlades > 1) {
      nblades = (rand() % (maxBlades))+1;
   }
   
   unsigned long i=0;
   while(i < nblades) {
      Blade b = generate_blade(maxGrade, minGrade, maxBlades);
      blades_t::iterator bi = std::find(blades.begin(),blades.end(),b);
      if(bi == blades.end()) {
         blades.push_back(b);
         i++;
      }
   }
   return Mvec(blades);
}