#include "test_genb.h"

#include <cstdlib>
#include <algorithm>

using namespace std;
using namespace gca;

Bladed generate_blade(unsigned int maxGrade, unsigned int minGrade, unsigned int maxBase) {
   
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
   
   return Bladed(v, e);
}

Mvecd generate_mvec(unsigned int maxBladeds, unsigned int maxGrade, unsigned int minGrade) {
   
   std::vector<Bladed> blades;
   
   unsigned long nblades = maxBladeds;
   if(maxBladeds > 1) {
      nblades = (rand() % (maxBladeds))+1;
   }
   
   unsigned long i=0;
   while(i < nblades) {
      Bladed b = generate_blade(maxGrade, minGrade, maxBladeds);
      std::vector<Bladed>::iterator bi = std::find(blades.begin(),blades.end(),b);
      if(bi == blades.end()) {
         blades.push_back(b);
         i++;
      }
   }
   return Mvecd(blades);
}