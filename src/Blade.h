#ifndef BLADE_H
#define	BLADE_H

#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include <iostream>

namespace gca
{

typedef std::vector<unsigned long> ebase_t;

class Blade {
public:

   Blade() : _e() {
      _v = 0;
   }

   Blade(double v) {
      _v = v;
   }

   Blade(double v, unsigned long e) : _e({e}) {
      _v = v;
   }

   Blade(double v, const ebase_t &e) : _e(e.begin(), e.end()) {
      _v = v;
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

   void set(double v) {
      _v = v;
   }

   double get(void) const {
      return _v;
   }

   Blade& dot(const Blade &B) const {
      double v = _v * B._v;

      // If one of the numbers is scalar, we can speed up
      // calculation
      if (_e.empty()) {
         if (B._e.empty()) {
            return *(new Blade(v));
         } else {
            return *(new Blade());
         }
      } else if (B._e.empty()) {
         return *(new Blade());
      }

      int sign;
      bool common = false;
      ebase_t e = this->emult(_e, B._e, &sign, &common);

      if (common) {
         return *(new Blade(v * sign, e));
      } else {
         return *(new Blade());
      }
   }

   Blade& wedge(const Blade &B) const {
      double v = _v * B._v;

      // If one of the numbers is scalar, we can speed up
      // calculation
      if (_e.empty()) {
         if (B._e.empty()) {
            return *(new Blade());
         } else {
            return *(new Blade(v, B._e));
         }
      } else if (B._e.empty()) {
         return *(new Blade(v, _e));
      }

      int sign;
      bool common = true;
      ebase_t e = this->emult(_e, B._e, &sign, &common);

      if (common) {
         return *(new Blade());
      } else {
         return *(new Blade(v * sign, e));
      }
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

   double mag(void) const {
      /* Magnitude of a blade, it's the dot product
         of a blade and its conjugate
         mag = A&(~A) */

      Blade result = this->dot(this->conj());
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
      double v = ceil(_v * 10000);
      v = v / 10000;


      ss << v << " ";

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
      return this->dot(B);
   }

   Blade operator^ (const Blade& B) const {
      return this->wedge(B);
   }

   Blade operator/(double x) const {
      return Blade(_v / x, _e);
   }

   Blade operator*(double x) const {
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

   unsigned int operator[] (const unsigned int gIndex) const {
        return _e[gIndex];
   }

   friend std::ostream& operator<<(std::ostream &out, const Blade &b) {
      out << b.toString();
      return out;
   }

private:

   ebase_t& emult(const ebase_t &eA,
           const ebase_t &eB,
           int *sign,
           bool *common) const {

      bool wedge = *common;

      ebase_t *e = new ebase_t();
      e->reserve(eA.size() + eB.size());

      ebase_t eIntersect;
      eIntersect.reserve(eA.size() + eB.size());

      ebase_t eBnot;
      eBnot.reserve(eA.size());

      ebase_t eAnot;
      eAnot.reserve(eB.size());

      unsigned int iA = 0;
      unsigned int iB = 0;

      *sign = 1;

      ebase_t::const_iterator eA_iter = eA.begin();
      ebase_t::const_iterator eB_iter = eB.begin();

      while (true) {
         if (eA_iter == eA.end()) {
            eAnot.insert(eAnot.end(), eB_iter, eB.end());
            break;
         } else if (eB_iter == eB.end()) {
            eBnot.insert(eBnot.end(), eA_iter, eA.end());
            break;
         } else if (*eA_iter < *eB_iter) {
            eBnot.push_back(*eA_iter);
            iA++;
            eA_iter++;
         } else if (*eB_iter < *eA_iter) {
            eAnot.push_back(*eB_iter);
            iB++;
            eB_iter++;
         } else {
            //If doing wedge, then we're done - it will be zero
            *common = true;
            if (wedge) {
               return *e;
            }
            eIntersect.push_back(*eA_iter);
            unsigned int flip = (iA + iB) % 2;
            if (flip != 0) {
               *sign = -(*sign);
            }
            eA_iter++;
            eB_iter++;
         }
      }

      // If doing dot, then we're done - it will be zero
      if (eIntersect.empty()) {
         *common = false;
         if (!wedge) {
            return *e;
         }
      }

      iA = eBnot.size();

      eA_iter = eBnot.begin();
      eB_iter = eAnot.begin();

      while (true) {
         if (eA_iter == eBnot.end()) {
            while (eB_iter != eAnot.end()) {
               e->push_back(*eB_iter++);
            }
            break;
         } else if (eB_iter == eAnot.end()) {
            while (eA_iter != eBnot.end()) {
               e->push_back(*eA_iter++);
            }
            break;
         } else if (*eA_iter < *eB_iter) {
            e->push_back(*eA_iter);
            iA--;
            eA_iter++;
         } else if (*eB_iter < *eA_iter) {
            e->push_back(*eB_iter);
            unsigned int flip = iA % 2;
            if (flip != 0) {
               *sign = -(*sign);
            }
            eB_iter++;
         } else {
            std::cout << "SHOULD NOT GET HERE!!!!!";
         }
      }

      return *e;
   }

protected:
   ebase_t _e;
   double _v;

};
}

#endif