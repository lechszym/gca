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
#include <algorithm>

#ifdef EIGEN_ENABLED
#include <Eigen/Dense>
#endif

#ifdef OMP_ENABLED
#include <omp.h>
#endif

namespace gca {

typedef std::vector<Blade> blades_t;

#ifndef GCA_PRECISION
#define GCA_PRECISION  1e-12
#endif

   class Mvec {
   public:

      Mvec() {
      }

      Mvec(const Blade &b) {
         _blades.push_back(b);
      }

      Mvec(std::size_t N) {
         _blades = blades_t(N);
      }

      Mvec(double v, unsigned long e) {
         _blades.push_back(Blade(v,e));
      }

      Mvec(const blades_t& blades) {
         _blades = blades;
      }

#ifdef EIGEN_ENABLED

      Mvec(Eigen::Matrix<double, Eigen::Dynamic, 1> &v) {
         _blades = blades_t(v.size());
         for (unsigned int i = 0; i < v.size(); i++) {
            _blades[i] = Blade(v(i, 0), i + 1);
         }
         this->prune();
      }

      Mvec(Eigen::Matrix<double, 1, Eigen::Dynamic> &v) {
         _blades = blades_t(v.size());
         for (unsigned int i = 0; i < v.size(); i++) {
            _blades[i] = Blade(v(0, i), i + 1);
         }
         this->prune();
      }
#endif

      Mvec(const Mvec& orig) {
         _blades = orig._blades;
      }

      virtual ~Mvec() {

      }

      Mvec& inner(const Mvec &m) const {
         const blades_t *a = &_blades;
         const blades_t *b = &m._blades;        
         std::size_t Na = a->size();
         std::size_t Nb = b->size();
         std::size_t N = Na*Nb;
         Mvec *result = new Mvec(N);

#ifdef OMP_ENABLED         
#pragma omp parallel for
#endif
         for(std::size_t n=0;n<N;n++) {
            std::size_t ia = n/Nb;
            std::size_t ib = n%Nb;
            result->_blades[n]= a->at(ia) & b->at(ib);
         }
         result->prune();
         return *result;
      }

      Mvec& outer(const Mvec &m) const {
         const blades_t *a = &_blades;
         const blades_t *b = &m._blades;        
         std::size_t Na = a->size();
         std::size_t Nb = b->size();
         std::size_t N = Na*Nb;
         Mvec *result = new Mvec(N);

#ifdef OMP_ENABLED         
#pragma omp parallel for
#endif
         for(std::size_t n=0;n<N;n++) {
            std::size_t ia = n/Nb;
            std::size_t ib = n%Nb;
            result->_blades[n] = a->at(ia) ^ b->at(ib);
         }
         result->prune();
         return *result;
      }

      Mvec& mul(const Mvec &m) const {
         const blades_t *a = &_blades;
         const blades_t *b = &m._blades;        
         std::size_t Na = a->size();
         std::size_t Nb = b->size();
         std::size_t N = Na*Nb;
         Mvec *result = new Mvec(2*N);

#ifdef OMP_ENABLED         
#pragma omp parallel for
#endif
         for(std::size_t n=0;n<N;n++) {
            std::size_t ia = n/Nb;
            std::size_t ib = n%Nb;
            result->_blades[n*2] = a->at(ia) & b->at(ib);
            result->_blades[n*2+1] = a->at(ia) ^ b->at(ib);
         }
         result->prune();
         return *result;
      }

      Mvec& mul(const double x) const {
         Mvec *result = new Mvec(_blades);
         for (std::size_t i = 0; i < result->_blades.size(); i++) {
            result->_blades[i].set(result->_blades[i].get() * x);
         }
         result->prune();
         return *result;
      }

      Mvec& div(const double x) const {
         Mvec *result = new Mvec(_blades);
         for (std::size_t i = 0; i < result->_blades.size(); i++) {
            result->_blades[i].set(result->_blades[i].get() / x);
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
         for (std::size_t i = 0; i < result->_blades.size(); i++) {
            result->_blades[i].set(result->_blades[i].get() + x);
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
         for (std::size_t i = 0; i < result->_blades.size(); i++) {
            result->_blades[i].set(result->_blades[i].get() - x);
         }
         result->prune();
         return *result;
      }

      double mag(void) const {
         double m = 0;
         for (std::size_t i = 0; i < _blades.size(); i++) {
            m += _blades[i].mag();
         }
         return m;
      }

      Mvec& conj() const {
         Mvec *result = new Mvec(_blades.size());
         for (std::size_t i = 0; i < _blades.size(); i++) {
            result->_blades[i] = _blades[i].conj();
         }
         result->prune();
         return *result;
      }

      std::string toString() const {
         std::stringstream ss;

         if (_blades.empty()) {
            ss << "0";
         } else {
            blades_t blades = _blades;
            sort(blades.begin(),blades.end());
            for (std::size_t i = 0; i < blades.size(); i++) {
               if (i) {
                  ss << " ";
                  if (blades[i].get() >= 0) {
                     ss << "+";
                  }
               }
               ss << blades[i];
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

        for(std::size_t i=0;i<_blades.size();i++) {
            if(_blades[i].grade()== nIndex) {
                result._blades.push_back(_blades[i]);
            }
        }
        return result;
      }
     

      friend std::ostream& operator<<(std::ostream &out, const Mvec &v) {
         out << v.toString();
         return out;
      }

   protected:

      void prune() {
         return;

         if(_blades.size() < 2) {
             return;
         }
          
         blades_t unique;
         
         unique.push_back(_blades[0]);
         
         for(std::size_t i=1;i<_blades.size();i++) {
             Blade *b = &_blades[i];
             blades_t::iterator bi = std::find(unique.begin(),unique.end(),*b);
             if(bi != unique.end()) {
                 bi->set(bi->get()+b->get());
             } else {
                 unique.push_back(*b);
             }
         }
         
         blades_t unique_nonzero;
         
         for(std::size_t i=0;i<unique.size();i++) {
             Blade *b = &unique[i];
             double v = b->get();

             if (v > GCA_PRECISION || v < (-GCA_PRECISION)) {
                 unique_nonzero.push_back(*b);
                 
             }
         }
         
         _blades.clear();
         if(unique_nonzero.empty()) {
             _blades.push_back(Blade(0));
         } else {
             _blades.insert(_blades.end(),unique_nonzero.begin(),unique_nonzero.end());
         }
      }

      blades_t _blades;
      
   };
}

#endif	/* MVEC_H */

