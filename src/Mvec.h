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


#ifndef GCA_PRECISION
#define GCA_PRECISION  1e-12
#endif

   template <typename T>
   class Mvec {
   public:

      Mvec() {
      }

      Mvec(const Blade<T> &b) {
         _blades.push_back(b);
      }

      Mvec(std::size_t N) {
         _blades = std::vector<Blade<T> >(N);
      }

      Mvec(double v, unsigned long e) {
         _blades.push_back(Blade<T>(v, e));
      }

      Mvec(const std::vector<Blade<T> >& blades) {
         _blades = blades;
         this->prune();
      }

#ifdef EIGEN_ENABLED

      Mvec(Eigen::Matrix<double, Eigen::Dynamic, 1> &v) {

      }

      Mvec(Eigen::Matrix<double, 1, Eigen::Dynamic> &v) {

      }

#endif      

      Mvec(const Mvec& orig) {
         _blades = orig._blades;
      }

      virtual ~Mvec() {

      }

      Mvec& inner(const Mvec &m) const {
         const std::vector<Blade<T> > *a = &_blades;
         const std::vector<Blade<T> > *b = &m._blades;
         std::size_t Na = a->size();
         std::size_t Nb = b->size();
         std::size_t N = Na*Nb;
         Mvec *result = new Mvec(N);

#ifdef OMP_ENABLED         
#pragma omp parallel for
#endif
         for (std::size_t n = 0; n < N; n++) {
            std::size_t ia = n / Nb;
            std::size_t ib = n % Nb;
            result->_blades[n] = a->at(ia) & b->at(ib);
         }
         return *result;
      }

      Mvec& outer(const Mvec &m) const {
         const std::vector<Blade<T> > *a = &_blades;
         const std::vector<Blade<T> > *b = &m._blades;
         std::size_t Na = a->size();
         std::size_t Nb = b->size();
         std::size_t N = Na*Nb;
         Mvec *result = new Mvec(N);

#ifdef OMP_ENABLED         
#pragma omp parallel for
#endif
         for (std::size_t n = 0; n < N; n++) {
            std::size_t ia = n / Nb;
            std::size_t ib = n % Nb;
            result->_blades[n] = a->at(ia) ^ b->at(ib);
         }
         return *result;
      }

      Mvec& mul(const Mvec &m) const {
         const std::vector<Blade<T> > *a = &_blades;
         const std::vector<Blade<T> > *b = &m._blades;
         std::size_t Na = a->size();
         std::size_t Nb = b->size();
         std::size_t N = Na*Nb;
         Mvec *result = new Mvec(2 * N);

#ifdef OMP_ENABLED         
#pragma omp parallel for
#endif
         for (std::size_t n = 0; n < N; n++) {
            std::size_t ia = n / Nb;
            std::size_t ib = n % Nb;
            result->_blades[n * 2] = a->at(ia) & b->at(ib);
            result->_blades[n * 2 + 1] = a->at(ia) ^ b->at(ib);
         }
         return *result;
      }

      Mvec& mul(const double x) const {
         Mvec *result = new Mvec(_blades);
         for (std::size_t i = 0; i < result->_blades.size(); i++) {
            result->_blades[i].set(result->_blades[i].get() * x);
         }
         return *result;
      }

      Mvec& div(const double x) const {
         Mvec *result = new Mvec(_blades);
         for (std::size_t i = 0; i < result->_blades.size(); i++) {
            result->_blades[i].set(result->_blades[i].get() / x);
         }
         return *result;
      }

      Mvec& div(const Mvec& m) const {
         double mMag = m.mag();
         Mvec mInv = m.conj();
         Mvec *result = new Mvec(this->mul(mInv.div(mMag)));
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
         return *result;
      }

      std::string toString() {
         std::stringstream ss;

         if (_blades.empty()) {
            ss << "0";
         } else {
            this->prune();
            //std::vector<Blade<T> > blades = _blades;
            sort(_blades.begin(), _blades.end());
            for (std::size_t i = 0; i < _blades.size(); i++) {
               if (i) {
                  ss << " ";
                  if (_blades[i].get() >= 0) {
                     ss << "+";
                  }
               }
               ss << _blades[i];
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

      Mvec operator-() const {
         return (Mvec()-(*this));
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

      Mvec operator[](const unsigned int nIndex) const {
         Mvec result;

         for (std::size_t i = 0; i < _blades.size(); i++) {
            if (_blades[i].grade() == nIndex) {
               result._blades.push_back(_blades[i]);
            }
         }
         result.prune();
         return result;
      }

      bool operator==(Mvec& m) {

         this->prune();
         m.prune();

         if (_blades.size() != m._blades.size()) {
            return false;
         }

         sort(_blades.begin(), _blades.end());
         sort(m._blades.begin(), m._blades.end());


         for (std::size_t i = 0; i < _blades.size(); i++) {
            double vA = _blades[i].get();
            double vB = m._blades[i].get();

            if (vA > vB + GCA_PRECISION) {
               return false;
            }

            if (vA < vB - GCA_PRECISION) {
               return false;
            }

            if (!(_blades[i] == m._blades[i])) {
               return false;
            }
         }

         return true;
      }

      friend std::ostream& operator<<(std::ostream &out, Mvec &v) {
         out << v.toString();
         return out;
      }
      
      static Mvec<T> pseudoscalar(unsigned int dim) {
          T         v(1);
          ebase_t   e;
          
          for(unsigned int i=1;i<=dim;i++) {
              e.push_back(i);
          }

          return Mvec<T>(Blade<T>(v, e));
      }

      static Mvec<T> I(unsigned int dim) {
          return Mvec<T>::pseudoscalar(dim);
      }
      
   protected:

      void prune() {

         if (_blades.size() < 2) {
            return;
         }

         //std::cout << "\n-- pruning phase 1--\n";
         std::vector<Blade<T> > unique;

         unique.push_back(_blades[0]);

         for (std::size_t i = 1; i < _blades.size(); i++) {
            //std:: cout << "  blade " << i << "/" << _blades.size();
            Blade<T> *b = &_blades[i];
            typename std::vector<Blade<T> >::iterator bi = std::find(unique.begin(), unique.end(), *b);
            if (bi != unique.end()) {
               //std::cout << "\n bi:" << *bi << "\n"; 
               //std::cout << " b:" << *b << "\n"; 
               //std::cout << " bi->get():" << bi->get() << "\n"; 
               //std::cout << " b->get():" << b->get() << "\n"; 
               //std::cout << " --reducing\n";
               bi->set(bi->get() + b->get());
               //std::cout << " done reducing--\n";
            } else {
               //std::cout << " --unique\n";
               unique.push_back(*b);
               //std::cout << " done unique--\n";
            }
         }
         
         //std::cout << "\n-- pruning phase 2--\n";

         std::vector<Blade<T> > unique_nonzero;

         for (std::size_t i = 0; i < unique.size(); i++) {
            Blade<T> *b = &unique[i];
            T v = b->get();

            if (v > GCA_PRECISION || v < (-GCA_PRECISION)) {
               unique_nonzero.push_back(*b);

            }
         }

         _blades.clear();
         if (unique_nonzero.empty()) {
            _blades.push_back(Blade<T>(0.0));
         } else {
            _blades.insert(_blades.end(), unique_nonzero.begin(), unique_nonzero.end());
         }
         //std::cout << "\n-- pruning done--\n";
      }

      std::vector<Blade<T> > _blades;


   };

#ifdef EIGEN_ENABLED

   template<> inline
   Mvec<double>::Mvec(Eigen::Matrix<double, Eigen::Dynamic, 1> &v) {
      _blades = std::vector<Blade<double> >(v.size());
      for (unsigned int i = 0; i < v.size(); i++) {
         _blades[i] = Blade<double>(v(i, 0), i + 1);
      }
      this->prune();
   }

   template<> inline
   Mvec<double>::Mvec(Eigen::Matrix<double, 1, Eigen::Dynamic> &v) {
      _blades = std::vector<Blade<double> >(v.size());
      for (unsigned int i = 0; i < v.size(); i++) {
         _blades[i] = Blade<double>(v(0, i), i + 1);
      }
      this->prune();
   }
#endif

}

#endif	/* MVEC_H */

