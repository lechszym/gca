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

   template <typename T>
   class Blade {
   public:

      Blade() : _e() {
         _v = T(0.0);
      }

      Blade(T v) {
         _v = fix_precision(v);
      }

      Blade(T v, unsigned long e) {
         _e.push_back(e);
         _v = fix_precision(v);
      }

      Blade(T v, const ebase_t &e) {
         _e = e;
         _v = fix_precision(v);
      }

      Blade(const Blade& orig) {
         _v = orig._v;
         _e = orig._e;
      }

      virtual ~Blade() {

      }

      unsigned int grade() const {
         return _e.size();
      }

      void set(T v) {
         _v = fix_precision(v);
      }

      T get(void) const {
         return _v;
      }
      
      static bool common(const Blade &A, const Blade &B) {

         const ebase_t *eA = &A._e;
         const ebase_t *eB = &B._e;

         // Check if any of the numbers is a scalar
         if (eA->empty()) {
            if (eB->empty()) {
               return true;
            } else {
               return false;
            }
         } else if (eB->empty()) {
            // Outer product of scalar and non-zero grade blade
            // is a scalar multiple of that blade
            return false;
         }         
       
         ebase_t::const_iterator eA_iter = eA->begin();
         ebase_t::const_iterator eB_iter = eB->begin();

         while (true) {
            if (eA_iter == eA->end()) {
               break;
            } else if (eB_iter == eB->end()) {
               break;
            } else if (*eA_iter < *eB_iter) {
               eA_iter++;
            } else if (*eA_iter > *eB_iter) {
               eB_iter++;
            } else {
               return true;
            }
         }
         
         return false;
      }
      
      Blade& inner(const Blade &B) const {

         const ebase_t *eA = &_e;
         const ebase_t *eB = &B._e;

         // Check if any of the numbers is a scalar
         if (eA->empty()) {
            if (eB->empty()) {
               // Inner product of two scalars is their scalar multiple
               return *(new Blade(_v * B._v, *eA));
            } else {
               // Inner product of scalar and non-zero grade blade
               // is zero
               return *(new Blade());
            }
         } else if (eB->empty()) {
            // Inner product of scalar and non-zero grade blade
            // is zero
            return *(new Blade());
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
            return *(new Blade());
         } else if(eC.empty()) {
            if(eD.empty()) {
               return *(new Blade(_v * sign * B._v));
            } else {
               return *(new Blade(_v * sign * B._v, eD));
            }            
         } else if(eD.empty()) {
            return *(new Blade(_v * sign * B._v, eC));
         } else {
            return Blade(_v * sign, eC).outer(Blade(B._v, eD));
         }
      }

      Blade& outer(const Blade &B) const {

         const ebase_t *eA = &_e;
         const ebase_t *eB = &B._e;

         // Check if any of the numbers is a scalar
         if (eA->empty()) {
            if (eB->empty()) {
               // Outer product of two scalars is zero
               return *(new Blade());
            } else {
               // Outer product of scalar and non-zero grade blade
               // is a scalar multiple of that blade
               return *(new Blade(_v * B._v, *eB));
            }
         } else if (eB->empty()) {
            // Outer product of scalar and non-zero grade blade
            // is a scalar multiple of that blade
            return *(new Blade(_v * B._v, _e));
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
               return *(new Blade());
            }
         }

         Blade *C = new Blade(_v * B._v * sign, *eC);
         delete(eC);
         return *C;

      }

      Blade& conj(void) const {
         int sign = 1;
         unsigned int k = this->grade();
         if (k > 0) {
            k = k * (k - 1) / 2;

            if (k % 2 == 1) {
               sign = -1;
            }
         }

         return *(new Blade(_v * sign, _e));
      }

      T mag(void) const {
         /* Magnitude of a blade, it's the inner product
            of a blade and its conjugate
            mag = A&(~A) */

         Blade result = this->inner(this->conj());
         return result._v;
      }

      Blade& inv(void) const {
         /* Inverse of a blade, it's the conjugate of 
            a blade divided by its magnitude 
            Ainv = ~A/(A&(~A)) */
         return *(new Blade(this->conj() / this->mag()));
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
         return this->inner(B);
      }

      Blade operator^ (const Blade& B) const {
         return this->outer(B);
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

   protected:
      ebase_t _e;
      T _v;

   };
}

#endif