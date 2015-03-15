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

      Mvec(double v, unsigned long e) {
         _blades.push_back(Blade<T>(v, e));
      }

      Mvec(const std::vector<Blade<T> >& blades) {
         _blades = blades;
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

      Mvec& operator+=(const Blade<T>& b) {
         T v = b.get();
         if ( (v > GCA_PRECISION) || (v < (-GCA_PRECISION))) {
            for (std::size_t i = 0; i < _blades.size(); i++) {
               if(_blades[i] == b) {
                  v = _blades[i].get() + b.get();
                  if ( (v > GCA_PRECISION) || (v < (-GCA_PRECISION))) {
                     _blades[i].set(v);
                  } else {
                     _blades.erase(_blades.begin() + i);
                  }
                  return *this;
               }
            }      
            _blades.push_back(b);
         }         
         return *this;
      }
      
      Mvec& operator+=(const Mvec& m) {
         for (std::size_t i = 0; i < m._blades.size(); i++) {
            (*this) += m._blades[i];
         }      
         return (*this);
      }      

      Mvec& operator+=(T v) {
         (*this) += Blade<T>(v);
         return (*this);
      }   

      Mvec operator-() const {
         Mvec result(*this);
         for (std::size_t i = 0; i < result._blades.size(); i++) {
            result._blades[i].set(result._blades[i].get()*-1.0);
         }      
         return result;
      }

      Mvec inner(const Mvec &m) const {
         const std::vector<Blade<T> > *a = &_blades;
         const std::vector<Blade<T> > *b = &m._blades;
         std::size_t Na = a->size();
         std::size_t Nb = b->size();
         std::size_t N = Na*Nb;
         Mvec result;

#ifdef OMP_ENABLED         
#pragma omp parallel for reduction (+: result)
#endif
         for (std::size_t n = 0; n < N; n++) {
            std::size_t ia = n / Nb;
            std::size_t ib = n % Nb;
            result += (a->at(ia) & b->at(ib));
         }
         return result;
      }

      Mvec outer(const Mvec &m) const {
         const std::vector<Blade<T> > *a = &_blades;
         const std::vector<Blade<T> > *b = &m._blades;
         std::size_t Na = a->size();
         std::size_t Nb = b->size();
         std::size_t N = Na*Nb;
         Mvec result;

#ifdef OMP_ENABLED         
#pragma omp parallel for reduction (+: result)
#endif
         for (std::size_t n = 0; n < N; n++) {
            std::size_t ia = n / Nb;
            std::size_t ib = n % Nb;
            result += (a->at(ia) ^ b->at(ib));
         }
         return result;
      }

       Mvec operator*(const Mvec& m) const {
         const std::vector<Blade<T> > *a = &_blades;
         const std::vector<Blade<T> > *b = &m._blades;
         std::size_t Na = a->size();
         std::size_t Nb = b->size();
         std::size_t N = Na*Nb;
         Mvec result;

#ifdef OMP_ENABLED         
#pragma omp parallel for reduction (+: result)
#endif
         for (std::size_t n = 0; n < N; n++) {
            std::size_t ia = n / Nb;
            std::size_t ib = n % Nb;
            result += (a->at(ia) & b->at(ib));
            result += (a->at(ia) ^ b->at(ib));         
         }
         return result;
      }

      Mvec operator*(const T v) const {
         Mvec result;
         if (v < GCA_PRECISION && v > (-GCA_PRECISION)) {
             for (std::size_t i = 0; i < _blades.size(); i++) {
                Blade<T> b = _blades[i] * v;
                T vtemp = b.get();
                if( (vtemp > GCA_PRECISION) || (vtemp < (-GCA_PRECISION))) {
                    result._blades.push_back(b);
                }
            }
         }
         return result;
      }

      Mvec operator/(const Mvec& m) const {
         Mvec mInv = m.conj() / m.mag();
         Mvec result = (*this) * mInv;
         return result;
      }
       
      Mvec operator/(const T v) const {
         if (v < GCA_PRECISION && v > (-GCA_PRECISION)) {
            return Mvec();
         }

         Mvec result;
         for(std::size_t i=0;i<_blades.size();i++) {
            Blade<T> b = _blades[i].get() * v;
            T vtemp = b.get();
            if(vtemp > GCA_PRECISION || vtemp < (-GCA_PRECISION)) {
               result._blades.push_back(b);
            }
         }
         return result;
      }

      Mvec operator+(const Mvec& m) const {
         Mvec result(*this);
         for (std::size_t i = 0; i < m._blades.size(); i++) {
            result += m._blades[i];
         }
         return result;
      }

      Mvec operator+(const T v) const {
         
         Mvec result(*this);
         
         if (v > GCA_PRECISION || v < (-GCA_PRECISION)) {
            result += Blade<T>(v);
         }
         return result;
      }

      Mvec operator-(const Mvec& m) const {
         Mvec result(*this);
         for (std::size_t i = 0; i < m._blades.size(); i++) {
            result += (m._blades[i] * -1.0);
         }
         return result;
      }

      Mvec operator-(const T v) const {
         Mvec result(*this);
         
         if (v > GCA_PRECISION || v < (-GCA_PRECISION)) {
            result += Blade<T>(-v);
         }
         return result;
      }

      double mag(void) const {
         double m = 0;
         for (std::size_t i = 0; i < _blades.size(); i++) {
            m += _blades[i].mag();
         }
         return m;
      }

      Mvec& conj() const {
         Mvec *result = new Mvec();
         for (std::size_t i = 0; i < _blades.size(); i++) {
            result->_blades.push_back(_blades[i].conj());
         }
         return *result;
      }

      std::string toString() {
         std::stringstream ss;

         if (_blades.empty()) {
            ss << "0";
         } else {
            //this->prune();
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
         return result;
      }

      bool operator==(Mvec& m) {

         if (_blades.size() != m._blades.size()) {
            return false;
         }

         std::sort(_blades.begin(), _blades.end());
         std::sort(m._blades.begin(), m._blades.end());


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

#if 0      
      void prune() {

         if (_blades.size() < 2) {
            return;
         }

         /*bool unique[_blades.size()];

         for (std::size_t i = 0; i < _blades.size(); i++) {
            unique[i] = true;
         }      

         for (std::size_t i = 0; i < _blades.size(); i++) {
            if(!unique[i]) {
               continue;
            }
            
            for(std::size_t j=i+1; j < _blades.size(); j++) {
               if(!unique[j]) {
                  continue;
               }

               if(_blades[i] == _blades[j]) {
                  _blades[i].set(_blades[i].get()+_blades[j].get());
                  unique[j] = false;
               }
            }
         }

         bool iszero = true;
         for (std::size_t i = 0; i < _blades.size(); i++) {
            if(unique[i]) {
               T v = _blades[i].get();
               if (v < GCA_PRECISION && v > (-GCA_PRECISION)) {
                  unique[i] = false;
               } else {
                  iszero = false;
               }
            }
         }      

         if(iszero) {
            _blades.clear();
            _blades.push_back(Blade<T>(0.0));
         } else {
             std::size_t removed = 0;
             std::size_t last = _blades.size();
             for (std::size_t i = 0; i < _blades.size(); i++) {
                if(!unique[i]) {
                   std::size_t lastToKeep = last;
                   for (std::size_t j=last-1; j>i; j--) {
                      if(unique[j]) {
                         lastToKeep = j;
                         break;
                      } else {
                         last--;
                         removed++;
                      }
                   }
                   if(lastToKeep < last) {
                      _blades[i] = _blades[lastToKeep];
                      last = lastToKeep;
                      removed++;
                   } else {
                      break;
                   }
                }
             }
             if(removed) {
                _blades.resize(_blades.size()-removed);
             }
         }*/
         std::vector<Blade<T> > unique;
         unique.push_back(_blades[0]);
         for (std::size_t i = 0; i < _blades.size(); i++) {
            Blade<T> *b = &_blades[i];
            typename std::vector<Blade<T> >::iterator bi = std::find(unique.begin(), unique.end(), *b);
            if (bi != unique.end()) {
               bi->set(bi->get() + b->get());
            } else {
               unique.push_back(*b);
            }
         }
         
         std::vector<Blade<T> > unique_nonzero;

         for (std::size_t i = 0; i < unique.size(); i++) {
            Blade<T> *b = &unique[i];
            T v = b->get();

            if ( (v > GCA_PRECISION) || (v < (-GCA_PRECISION))) {
               unique_nonzero.push_back(*b);
            }
         }

         _blades.clear();
         if (unique_nonzero.empty()) {
            _blades.push_back(Blade<T>(0.0));
         } else {
            _blades.insert(_blades.end(), unique_nonzero.begin(), unique_nonzero.end());
         }
      }
#endif
      
      std::vector<Blade<T> > _blades;


   };

#ifdef EIGEN_ENABLED

   template<> inline
   Mvec<double>::Mvec(Eigen::Matrix<double, Eigen::Dynamic, 1> &v) {
      _blades = std::vector<Blade<double> >(v.size());
      for(int i = 0; i < v.size(); i++) {
         _blades[i] = Blade<double>(v(i, 0), i + 1);
      }
      //this->prune();
   }

   template<> inline
   Mvec<double>::Mvec(Eigen::Matrix<double, 1, Eigen::Dynamic> &v) {
      _blades = std::vector<Blade<double> >(v.size());
      for (int i = 0; i < v.size(); i++) {
         _blades[i] = Blade<double>(v(0, i), i + 1);
      }
      //this->prune();
   }
#endif

}

#endif	/* MVEC_H */

