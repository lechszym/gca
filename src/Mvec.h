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

typedef std::vector<Blade *> blades_t;

#ifndef GCA_PRECISION
#define GCA_PRECISION  1e-12
#endif

   class Mvec {
   public:

      Mvec() {
      }

      Mvec(const Blade &b) {
         _blades.push_back(new Blade(b));
      }

      Mvec(double v) {
         _blades.push_back(new Blade(v));
      }

      Mvec(double v, unsigned long e) {
         _blades.push_back(new Blade(v,e));
      }

      Mvec(const blades_t& blades) {
         for(std::size_t i=0;i<blades.size();i++) {
            _blades.push_back(new Blade(*blades[i]));
         }
      }

#ifdef EIGEN_ENABLED

      Mvec(Eigen::Matrix<double, Eigen::Dynamic, 1> &v) {
         for (unsigned int i = 0; i < v.size(); i++) {
            _blades.push_back(new Blade(v(i, 0), i + 1));
         }
         this->prune();
      }

      Mvec(Eigen::Matrix<double, 1, Eigen::Dynamic> &v) {
         for (unsigned int i = 0; i < v.size(); i++) {
            _blades.push_back(new Blade(v(0, i), i + 1));
         }
         this->prune();
      }
#endif

      Mvec(const Mvec& orig) {
         for(std::size_t i=0;i<orig._blades.size();i++) {
            _blades.push_back(new Blade(*orig._blades[i]));
         }
      }

      virtual ~Mvec() {
         for(std::size_t i=0;i<_blades.size();i++) {
            delete _blades[i];
         }
      }

      Mvec& inner(const Mvec &m) const {
         Mvec *result = new Mvec();
         const blades_t *mB = &_blades;
         const blades_t *nB = &m._blades;
//#ifdef OMP_ENABLED
         //std::size_t  nops = 
         
//#else
        blades_t::const_iterator i;
        blades_t::const_iterator j;

        for (i = mB->begin(); i != mB->end(); i++) {
           for (j = nB->begin(); j != nB->end(); j++) {
               Blade *a = *i;
               Blade *b = *j;
               Blade *c = &a->inner(*b);
               result->_blades.push_back(c);
            }
        }
//#endif
          
/*          
          
          const blades_t *nb = &_blades;
         const blades_t *mb = &m._blades;


#ifdef OMP_ENABLED         
         int P=std::min(omp_get_num_procs(),(int) (nb->size()*mb->size()) ); 
#pragma omp parallel for shared(result,nb,mb) private(i,j)
         for(int p=0;p<P;p++) {
            blades_t *rb = new blades_t();
            int k=0;
#else
            blades_t *rb = &result->_blades;
#endif
            for (i = nb->begin(); i != nb->end(); i++) {
               for (j = mb->begin(); j != mb->end(); j++) {
#ifdef OMP_ENABLED
                  if(k++%P != p) {
                     continue;
                  }
#endif               
                  rb->push_back((*i)&(*j));
               }
            }
#ifdef OMP_ENABLED
            #pragma omp critical
            {
               result->_blades.insert(result->_blades.end(),rb->begin(),rb->end());
            }
         }
#endif*/
         result->prune();
         return *result;
      }

      Mvec& outer(const Mvec &m) const {
         Mvec *result = new Mvec();

         blades_t::const_iterator i;
         blades_t::const_iterator j;

         for (i = _blades.begin(); i != this->_blades.end(); i++) {
            for (j = m._blades.begin(); j != m._blades.end(); j++) {
               Blade *a = *i;
               Blade *b = *j;
               Blade *c = &a->outer(*b);
               result->_blades.push_back(c);
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
               Blade *a = *i;
               Blade *b = *j;
               Blade *c = &a->inner(*b);
               result->_blades.push_back(c);
               c = &a->outer(*b);
               result->_blades.push_back(c);
            }
         }
         result->prune();
         return *result;
      }

      Mvec& mul(const double x) const {
         Mvec *result = new Mvec();

         blades_t::const_iterator i;

         for (i = _blades.begin(); i != this->_blades.end(); i++) {
            Blade *a = *i;
            Blade *c = new Blade(*a);
            c->set(c->get() * x);
            result->_blades.push_back(c);
         }
         result->prune();
         return *result;
      }

      Mvec& div(const double x) const {
         Mvec *result = new Mvec();

         blades_t::const_iterator i;

         for (i = _blades.begin(); i != this->_blades.end(); i++) {
            Blade *a = *i;
            Blade *c = new Blade(*a);
            c->set(c->get() / x);
            result->_blades.push_back(c);
         }
         result->prune();
         return *result;
      }

      Mvec& div(const Mvec& m) const {
         double mMag = m.mag();
         Mvec mInv = m.conj();
         mInv = mInv.div(mMag);
         Mvec *result = new Mvec(this->mul(mInv));

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


         blades_t::iterator i = result->_blades.begin();
         while (i != result->_blades.end()) {
            Blade *a = *i;
            a->set(a->get() + x);
            i++;
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

         blades_t::iterator i = result->_blades.begin();
         while (i != result->_blades.end()) {
            Blade *a = *i;
            a->set(a->get() - x);
            i++;
         }
         result->prune();
         return *result;
      }

      double mag(void) const {
         double m = 0;
         blades_t::const_iterator i;
         for (i = this->_blades.begin(); i != this->_blades.end(); i++) {
            m += (*i)->mag();
         }
         return m;
      }

      Mvec& conj() const {
         Mvec *result = new Mvec();

         blades_t::const_iterator i;

         for (i = _blades.begin(); i != this->_blades.end(); i++) {
            Blade *a = *i;
            Blade *c = &a->conj();
            result->_blades.push_back(c);
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
            //sort(_blades.begin(),_blades.end());
            blades_t::iterator i;
            for (i = _blades.begin(); i != _blades.end(); i++) {
               if (!beg) {
                  ss << " ";
                  if ((*i)->get() >= 0) {
                     ss << "+";
                  }
               }
               beg = false;
               ss << **i;
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
           for(std::size_t i=0;i<m._blades.size();i++) {
               _blades.push_back(new Blade(*m._blades[i]));
           }
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
            if((*i)->grade()== nIndex) {
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
         if(_blades.size() < 2) {
             return;
         }
          
         blades_t unique;
         
         unique.push_back(_blades[0]);
         
         for(std::size_t i=1;i<_blades.size();i++) {
             Blade *a = _blades[i];
             bool reduction = false;
             for(std::size_t j=0;j<unique.size();j++) {
                Blade *b = unique[j];
                if(*a == *b) {
                   b->set(a->get()+b->get());
                   reduction = true;
                   break;
                }
             }
             if(reduction) {
                delete a;
             } else {
                unique.push_back(a);
             }
         }
         
         blades_t unique_nonzero;
         
         for(std::size_t i=0;i<unique.size();i++) {
             Blade *a = unique[i];
             double v = a->get();

             if (v > GCA_PRECISION || v < (-GCA_PRECISION)) {
                unique_nonzero.push_back(a);
             } else {
                delete a;
             }
         }
         
         _blades.clear();
         if(unique_nonzero.empty()) {
             _blades.push_back(new Blade(0));
         } else {
             _blades.insert(_blades.end(),unique_nonzero.begin(),unique_nonzero.end());
         }
         
          /*
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
         }*/
      }

      blades_t _blades;
      
   };
}

#endif	/* MVEC_H */

