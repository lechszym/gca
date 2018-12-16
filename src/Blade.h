#ifndef BLADE_H
#define	BLADE_H

#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iostream>

#include "Sym.h"

namespace gca {

   typedef std::vector<unsigned long> ebase_t;

   /**
    * This class represents a GA blade of value with arbitrary grade.  The grade
    * is stored as a vector of base values - from 1 to N.  Scalar blade's base
    * is empty (considered to have base value of 0).  
    * 
    * The base vector is always sorted from smallest to largest - makes the
    * iteration and comparison of blades easier.  The sign of the value of the
    * blade is flipped to be consistent with the order.
    */
   
   template <typename T = float>
   class Blade {
   public:

      
      /**
       * Initialises blade with a scalar value of zero
       */
      Blade() : _e() {
         _v = T(0.0);
      }

      /**
       * Initialises blade with a scalar value of v
       * 
       * @param v - scalar value
       */      
      Blade(T v) {
         _v = check_precision_for_zero(v);
      }

      /**
       * Initialises blade with a single-grade base
       * 
       * @param v - value
       * @param e - base vector
       */
      Blade(T v, unsigned long e) {
         _e.push_back(e);
         _v = check_precision_for_zero(v);
      }

      /**
       * Initialises blade with a multi-grade base
       * 
       * @param v - value
       * @param e - base vector
       */
      Blade(T v, const ebase_t &e) {
         _e = e;
         _v = check_precision_for_zero(v);
         sort_bases();
      }

      
       /**
       * Initialises blade with a based derived from string;
       * example string for blade of value 2 with bases 1^3
       * is "2 e1^e3".
       * 
       * @param s - string
       */
      Blade(const char *s) {
          
          std::istringstream vs(s);

          T v;

          v = vs >> v ? v : 0;

          _v = check_precision_for_zero(v);

          const char *c=s;
          bool basesSearch=false;
          while(*c != 0) {
              if(basesSearch) {
                  if(*c==' ' || *c=='+' || *c=='-') {
                      break;
                  } else if(*c=='e') {
                      std::istringstream es(c+1);
                        
                      int e;
                      e = es >> e ? e : 0;
                      _e.push_back(e);
                  }                  
              } else if(*c == ' ') {
                  basesSearch=true;
              } else if(*c == '+' || *c=='-') {
                  break;
              }
              c++;
          }
          sort_bases();
      }
      
      /**
       * Deep copy of the blade
       * 
       * @param orig
       */
      Blade(const Blade& orig) {
         _v = orig._v;
         _e = orig._e;
      }

      /**
       * Destructor
       */
      virtual ~Blade() {

      }

      /**
       * Grade getter
       * 
       * @return grade of the blade
       */
      unsigned int grade() const {
         return _e.size();
      }

      /**
       * Value getter
       * 
       * @return value of the blade
       */
      T get(void) const {
         return _v;
      }
      
      /**
       * Value setter
       * 
       * @param v set value of the blade
       */
      void set(T v) {
         _v = check_precision_for_zero(v);
      }

      /**
       * Check for common bases between two blades
       * @param A first blade
       * @param B second blade
       * @return true if blades have a common base, false otherwise
       * 
       * Scalar blades are treated as blades with base 0, non-scalar blades
       * are blades with non-zero bases.  Hence, two scalar blades
       * are sharing a base, whereas a scalar and non-scalar blades
       * do not share a base.
       */
      static bool common(const Blade &A, const Blade &B) {

         // Get pointers to blade bases
         const ebase_t *eA = &A._e;
         const ebase_t *eB = &B._e;

         // First check, if any of the blades is a scalar
         
         if (eA->empty()) {
            // If A is a scalar...
            if (eB->empty()) {
                //.. and B is scalar as well, then they share a base
               return true;
            } else {
                //...and B is not a scalar, then they do not share a base
               return false;
            }
         } else if (eB->empty()) {
             // Else if A is not a scalar but B is, then the blades do not share
             // a base
            return false;
         }         
       
         // Both blades are non-scalars
         
         // Iterate over blade bases.  If common one is found, return true.
         // If iteration finishes without finding a common base, return false.
         ebase_t::const_iterator eA_iter = eA->begin();
         ebase_t::const_iterator eB_iter = eB->begin();
         while (true) {
            if (eA_iter == eA->end()) {
               // No more bases in A, exit
               break;
            } else if (eB_iter == eB->end()) {
               // No more bases in B, exit
               break;
            } else if (*eA_iter < *eB_iter) {
               // A base is smaller than B base, go to next A base
               eA_iter++;
            } else if (*eA_iter > *eB_iter) {
               // B base is smaller than A base, go to next B base
               eB_iter++;
            } else {
               // Bases are the same - found common base
               return true;
            }
         }
         
         // Did not find common base
         return false;
      }
      
      /**
       * Inner product of two blades
       * 
       * @param A First blade
       * @param B Second blade
       * @return Result blade of inner product of A and B
       */
      static Blade inner(const Blade &A, const Blade &B) {

         // Get pointers to blade bases
         const ebase_t *eA = &A._e;
         const ebase_t *eB = &B._e;

         // Check if any of the blades is a scalar
         if (eA->empty()) {
            if (eB->empty()) {
               // Inner product of two scalars is their scalar multiple
               return Blade(A._v * B._v, *eA);
            } else {
               // Inner product of scalar and non-zero grade blade
               // is zero
               return Blade();
            }
         } else if (eB->empty()) {
            // Inner product of non-zero grade blade and scalar
            // is zero
            return Blade();
         }

         int sign = 1;
         bool common = false;

         ebase_t eC;
         ebase_t eD;
         eC.reserve(eA->size());
         eD.reserve(eB->size());

         unsigned int iA = eA->size();
         unsigned int iB = 0;
         unsigned int flip;
         
         ebase_t::const_iterator eA_iter = eA->begin();
         ebase_t::const_iterator eB_iter = eB->begin();

         while (true) {
            if (eA_iter == eA->end()) {
               eD.insert(eD.end(), eB_iter, eB->end());
               break;
            } else if (eB_iter == eB->end()) {
               eC.insert(eC.end(), eA_iter, eA->end());
               break;
            } else if (*eA_iter < *eB_iter) {
               eC.push_back(*eA_iter);
               iA--;
               eA_iter++;
            } else if (*eA_iter > *eB_iter) {
               eD.push_back(*eB_iter);
               iB++;
               eB_iter++;
            } else {
               iA--;
               flip = iA % 2;
               if (flip != 0) {
                  sign *= -1;
               }
               flip = iB % 2;
               if (flip != 0) {
                  sign *= -1;
               }
               common = true;
               eA_iter++;
               eB_iter++;
            }
         }

         if (!common) {
            return Blade();
         } else if(eC.empty()) {
            if(eD.empty()) {
               return Blade(A._v * sign * B._v);
            } else {
               return Blade(A._v * sign * B._v, eD);
            }            
         } else if(eD.empty()) {
            return Blade(A._v * sign * B._v, eC);
         } else {
            return Blade::outer(Blade(A._v * sign, eC),Blade(B._v, eD));
         }
      }

      static Blade outer(const Blade &A, const Blade &B) {

         const ebase_t *eA = &A._e;
         const ebase_t *eB = &B._e;

         // Check if any of the numbers is a scalar
         if (eA->empty()) {
            if (eB->empty()) {
               // Outer product of two scalars is zero
               return Blade();
            } else {
               // Outer product of scalar and non-zero grade blade
               // is a scalar multiple of that blade
               return Blade(A._v * B._v, *eB);
            }
         } else if (eB->empty()) {
            // Outer product of scalar and non-zero grade blade
            // is a scalar multiple of that blade
            return Blade(A._v * B._v, A._e);
         }

         int sign = 1;

         ebase_t* eC = new ebase_t();
         eC->reserve(eA->size() + eB->size());

         unsigned int iA = eA->size();

         ebase_t::const_iterator eA_iter = eA->begin();
         ebase_t::const_iterator eB_iter = eB->begin();

         while (true) {
            if (eA_iter == eA->end()) {
               eC->insert(eC->end(), eB_iter, eB->end());
               break;
            } else if (eB_iter == eB->end()) {
               eC->insert(eC->end(), eA_iter, eA->end());
               break;
            } else if (*eA_iter < *eB_iter) {
               eC->push_back(*eA_iter);
               iA--;
               eA_iter++;
            } else if (*eA_iter > *eB_iter) {
               unsigned int flip = iA % 2;
               if (flip != 0) {
                  sign *= -1;
               }
               eC->push_back(*eB_iter);
               eB_iter++;
            } else {
               // Common base, outer product is 0
               delete(eC);
               return Blade();
            }
         }

         Blade C(A._v * B._v * sign, *eC);
         delete(eC);
         return C;

      }

      Blade conj(void) const {
         int sign = 1;
         unsigned int k = this->grade();
         if (k > 0) {
            k = k * (k - 1) / 2;

            if (k % 2 == 1) {
               sign = -1;
            }
         }

         return Blade(_v * sign, _e);
      }

      T mag(void) const {
         /* Magnitude of a blade, it's the inner product
            of a blade and its conjugate
            mag = A&(~A) */

         Blade result = Blade::inner(*this,this->conj());
         return result._v;
      }

      Blade inv(void) const {
         /* Inverse of a blade, it's the conjugate of 
            a blade divided by its magnitude 
            Ainv = ~A/(A&(~A)) */
         return Blade(this->conj() / this->mag());
      }

      std::string toString() const {
         std::stringstream ss;

         ss.precision(4);
         //T v = ceil(_v * 10000);
         //v = v / 10000;


         ss << _v;

         if (!_e.empty()) {
            ss << " ";
         }


         ebase_t::const_iterator eA_iter = _e.begin();

         while (eA_iter != _e.end()) {
            ss << "e" << *eA_iter;
            eA_iter++;
            if (eA_iter != _e.end()) {
               ss << "^";
            }
         }

         return ss.str();
      }

      Blade operator&(const Blade& B) const {
         return Blade::inner(*this,B);
      }

      Blade operator^ (const Blade& B) const {
         return Blade::outer(*this,B);
      }

      Blade operator/(T x) const {
         return Blade(_v / x, _e);
      }

      Blade operator*(T x) const {
         return Blade(_v * x, _e);
      }

      Blade operator~(void) const {
         return this->conj();
      }

      Blade& operator=(const Blade& B) {
         if (this != &B) {
            _e = B._e;
            _v = B._v;
         }
         return *this;

      }

      bool operator==(const Blade& b) const {

         /* Comparison of two blades - check if blade bases are identical,
            the blade value are ignored */
         if (this->grade() != b.grade()) {
            return false;
         }

         for (size_t i = 0; i < _e.size(); i++) {
            if (_e[i] != b._e[i]) {
               return false;
            }
         }

         return true;
      }

      bool operator<(const Blade& b) const {

         if (this->grade() < b.grade()) {
            return true;
         } else if (this->grade() > b.grade()) {
            return false;
         }

         for (size_t i = 0; i < _e.size(); i++) {
            if (_e[i] < b._e[i]) {
               return true;
            } else if (_e[i] > b._e[i]) {
               return false;
            }
         }

         return false;
      }

      unsigned int at(const unsigned int gIndex) const {
         return _e[gIndex];
      }

      unsigned int operator[](const unsigned int gIndex) const {
         return this->at(gIndex);
      }

      friend std::ostream& operator<<(std::ostream &out, const Blade &b) {
         out << b.toString();
         return out;
      }

   private:
       
       void sort_bases() {
         
           int sign = 1;
           for(size_t i=0;i<_e.size();i++) {
               unsigned int min_e = _e[i];
               unsigned int min_k = i;
               for(size_t j=i+1;j<_e.size();j++) {
                   if(_e[j]<min_e) {
                       min_e = _e[j];
                       min_k=j;
                   }
               }
               if(min_k != i) {
                   int tmp = _e[i];
                   _e[i] = _e[min_k];
                   _e[min_k] = tmp;
                   sign *= -1;                   
               }
           }
           if(sign == -1) {
               _v = -_v;
           }
       }
       

   protected:
      ebase_t _e;
      T _v;

   };
}

#endif