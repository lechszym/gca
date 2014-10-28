/* 
 * File:   Mvec.h
 * Author: lechszym
 *
 * Created on 16 October 2014, 9:09 PM
 */

#ifndef MVEC_H
#define	MVEC_H

#include "Blade.h"
#include <list>
#include <sstream>
#include <math.h>
#include <iostream>

#ifdef EIGEN_ENABLED
#include <Eigen/Dense>
#endif

#ifdef OMP_ENABLED
#include <omp.h>
#endif

namespace gca {

typedef std::list<Blade> blades_t;

#ifndef GCA_PRECISION
#define GCA_PRECISION  1e-12
#endif

   class Mvec {
   public:

      Mvec() {
#ifdef OMP_ENABLED
         omp_init_lock(&plock);
#endif
      }

      Mvec(const Blade &b) {
#ifdef OMP_ENABLED
         omp_init_lock(&plock);
#endif
         _blades.push_back(b);
      }

      Mvec(double v) {
#ifdef OMP_ENABLED
         omp_init_lock(&plock);
#endif
         _blades.push_back(Blade(v));
      }

      Mvec(double v, unsigned long e) : Mvec() {
#ifdef OMP_ENABLED
         omp_init_lock(&plock);
#endif
         _blades.push_back(Blade(v,e));
      }

      Mvec(const blades_t& blades) {
#ifdef OMP_ENABLED
         omp_init_lock(&plock);
#endif
         _blades = blades;
      }

#ifdef EIGEN_ENABLED

      Mvec(Eigen::Matrix<double, Eigen::Dynamic, 1> &v) {
#ifdef OMP_ENABLED
         omp_init_lock(&plock);
#endif
         for (unsigned int i = 0; i < v.size(); i++) {
            _blades.push_back(Blade(v(i, 0), i + 1));
         }
         this->prune();
      }

      Mvec(Eigen::Matrix<double, 1, Eigen::Dynamic> &v) {
         for (unsigned int i = 0; i < v.size(); i++) {
            _blades.push_back(Blade(v(0, i), i + 1));
         }
         this->prune();
      }
#endif

      Mvec(const Mvec& orig) {
#ifdef OMP_ENABLED
         omp_init_lock(&plock);
#endif
         _blades = orig._blades;
      }

      virtual ~Mvec() {
#ifdef OMP_ENABLED
         omp_destroy_lock(&plock);
#endif
      }

      Mvec& inner(const Mvec &m) const {
         Mvec *result = new Mvec();

         blades_t::const_iterator i;
         blades_t::const_iterator j;

#ifdef OMP_ENABLED         
         int P=omp_get_num_procs(); 
         omp_lock_t writelock;
         omp_init_lock(&writelock);
         int k=0;
#pragma omp parallel for shared(result,_blades,m) private(i,j)
         for(int p=0;p<P;p++) {
            Mvec *presult = new Mvec();
#else
            Mvec *presult = result;
#endif
            for (i = _blades.begin(); i != this->_blades.end(); i++) {
               for (j = m._blades.begin(); j != m._blades.end(); j++) {
#ifdef OMP_ENABLED
                  if(k++%P != p) {
                     continue;
                  }
#endif               
                  presult->_blades.push_back((*i)&(*j));
               }
            }
#ifdef OMP_ENABLED
            omp_set_lock(&plock);      
            result->insert(result->end(),presult->begin(),presult->end());
            omp_unset_lock(&plock);
         }
#endif
         result->prune();
         return *result;
      }

      Mvec& outer(const Mvec &m) const {
         Mvec *result = new Mvec();

         blades_t::const_iterator i;
         blades_t::const_iterator j;

         for (i = _blades.begin(); i != this->_blades.end(); i++) {
            for (j = m._blades.begin(); j != m._blades.end(); j++) {
               result->_blades.push_back((*i)^(*j));
            }
         }
         result->prune();
         return *result;
      }

      Mvec& mul(const Mvec &m) const {
         Mvec *result = new Mvec();

         blades_t::const_iterator i;
         blades_t::const_iterator j;

         for (i = _blades.begin(); i != this->_blades.end(); i++) {
            for (j = m._blades.begin(); j != m._blades.end(); j++) {
               //std::cout << *i << " & " << *j << " = ";
               result->_blades.push_back((*i)&(*j));
               //std::cout << result->_blades.back() << std::endl;
               //std::cout << *i << " ^ " << *j << " = ";
               result->_blades.push_back((*i)^(*j));
               //std::cout << result->_blades.back() << std::endl;
            }
         }
         result->prune();
         return *result;
      }

      Mvec& mul(const double x) const {
         Mvec *result = new Mvec();

         blades_t::const_iterator i;

         for (i = _blades.begin(); i != this->_blades.end(); i++) {
            result->_blades.push_back((*i) * x);
         }
         result->prune();
         return *result;
      }

      Mvec& div(const double x) const {
         Mvec *result = new Mvec();

         blades_t::const_iterator i;

         for (i = _blades.begin(); i != this->_blades.end(); i++) {
            result->_blades.push_back((*i) / x);
         }
         result->prune();
         return *result;
      }

      Mvec& div(const Mvec& m) const {
         double mMag = m.mag();
         Mvec mInv = m.conj();

         Mvec *result = new Mvec(this->mul(mInv.div(mMag)));

         result->prune();
         return *result;
      }

      Mvec& add(const Mvec& m) const {
         Mvec *result = new Mvec(_blades);
         result->_blades.insert(result->_blades.end(), m._blades.begin(), m._blades.end());

         result->prune();
         return *result;
      }

      Mvec& add(double x) const {
         Mvec *result = new Mvec(_blades);


         blades_t::iterator bi = result->_blades.begin();
         while (bi != result->_blades.end()) {
            bi->set(bi->get() + x);
            bi++;
         }
         result->prune();
         return *result;
      }

      Mvec& sub(const Mvec& m) const {
         Mvec *result = new Mvec(m.mul(-1));

         result->_blades.insert(result->_blades.end(), _blades.begin(), _blades.end());

         result->prune();
         return *result;
      }

      Mvec& sub(double x) const {
         Mvec *result = new Mvec(_blades);

         blades_t::iterator bi = result->_blades.begin();
         while (bi != result->_blades.end()) {
            bi->set(bi->get() - x);
            bi++;
         }
         result->prune();
         return *result;
      }

      double mag(void) const {
         double m = 0;
         blades_t::const_iterator i;
         for (i = this->_blades.begin(); i != this->_blades.end(); i++) {
            m += i->mag();
         }
         return m;
      }

      Mvec& conj() const {
         Mvec *result = new Mvec();

         blades_t::const_iterator i;

         for (i = _blades.begin(); i != this->_blades.end(); i++) {
            result->_blades.push_back(i->conj());
         }
         result->prune();
         return *result;

      }

      std::string toString() {
         std::stringstream ss;
         bool beg = true;

         if (_blades.empty()) {
            ss << "0";
         } else {
            _blades.sort();
            blades_t::iterator i;
            for (i = _blades.begin(); i != _blades.end(); i++) {
               if (!beg) {
                  ss << " ";
                  if (i->get() >= 0) {
                     ss << "+";
                  }
               }
               beg = false;
               ss << *i;
            }
         }
         return ss.str();
      }

      Mvec operator&(const Mvec& m) const {
         return this->inner(m);
      }

      Mvec operator^ (const Mvec& m) const {
         return this->outer(m);
      }

      Mvec operator*(const Mvec& m) const {
         return this->mul(m);
      }

      Mvec operator*(const double x) const {
         return this->mul(x);
      }

      Mvec operator/(const Mvec& m) const {
         return this->div(m);
      }

      Mvec operator/(double x) const {
         return this->div(x);
      }

      Mvec operator+(const Mvec& m) const {
         return this->add(m);
      }

      Mvec operator+(const double x) const {
         return this->add(x);
      }

      Mvec operator-(const Mvec& m) const {
         return this->sub(m);
      }

      Mvec operator-(double x) const {
         return this->sub(x);
      }

      Mvec& operator=(const Mvec& m) {
         if (this != &m) {
            _blades = m._blades;
         }
         return *this;

      }

      Mvec& operator~(void) {
        return this->conj();
      }
      
      Mvec operator[] (const unsigned int nIndex) const {
        Mvec result;

        blades_t::const_iterator i;
        for(i=_blades.begin();i!=_blades.end();i++) {
            if(i->grade()== nIndex) {
                result._blades.push_back(*i);
            }
        }
        return result;
      }
     

      friend std::ostream& operator<<(std::ostream &out, Mvec &v) {
         out << v.toString();
         return out;
      }

   protected:

      void prune() {
         blades_t::iterator i;
         blades_t::iterator j;
         blades_t::iterator k;

         i = this->_blades.begin();

         while (i != this->_blades.end()) {
            double v = i->get();
            if (v < GCA_PRECISION && v > (-GCA_PRECISION)) {
               k = i;
               k++;
               this->_blades.erase(i);
               i = k;
               continue;
            }

            j = i;
            j++;
            while (j != this->_blades.end()) {
               if ((*i) == (*j)) {
                  i->set(i->get() + j->get());
                  k = j;
                  k++;
                  this->_blades.erase(j);
                  j = k;
               } else {
                  j++;
               }
            }
            v = i->get();
            if (v < GCA_PRECISION && v > (-GCA_PRECISION)) {
               k = i;
               k++;
               this->_blades.erase(i);
               i = k;
            } else {
               i++;
            }
         }
      }

      blades_t _blades;
#ifdef OMP_ENABLED         
      omp_lock_t plock;
#endif
      
   };
}

#endif	/* MVEC_H */

