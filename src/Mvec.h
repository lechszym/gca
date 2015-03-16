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
            if ( v != 0.0) {
                for (std::size_t i = 0; i < _blades.size(); i++) {
                    if (_blades[i] == b) {
                        //std::cout << "vba: " << _blades[i].get() << "\n";
                        //std::cout << "vbb: " << b.get() << "\n";

                        _blades[i].set(_blades[i].get() + b.get());
                        //std::cout << "vbc: " << v << "\n";
                        if (_blades[i].get() == 0.0) {
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
                //if (n == 15) {
                //    std::cout << "zz:\n";
                //}
                std::size_t ia = n / Nb;
                std::size_t ib = n % Nb;
                //std::cout << "(" << a->at(ia) << "&" << b->at(ib) << ")=";
                //std::cout << (a->at(ia) & b->at(ib)) << "\n";
                result += (a->at(ia) & b->at(ib));
                //std::cout << "result: " << result << "\n";
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
            T vf = fix_precision(v);
            if (vf != 0.0) {
                for (std::size_t i = 0; i < _blades.size(); i++) {
                    Blade<T> b = _blades[i] * vf;
                    if (b.get() != 0.0) {
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
            T vf = fix_precision(v);
            if(vf == 0.0) {
                return Mvec();
            }

            Mvec result;
            for (std::size_t i = 0; i < _blades.size(); i++) {
                Blade<T> b = _blades[i].get() / vf;
                if (b.get() != 0.0) {
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
            T vf = fix_precision(v);
            
            if (vf != 0.0) {
                result += Blade<T>(vf);
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
            T vf = fix_precision(v);
            
            if (vf != 0.0) {
                result += Blade<T>(-vf);
            }
            return result;
        }

        T mag(void) const {
            T m(0.0);
            for (std::size_t i = 0; i < _blades.size(); i++) {
                m = (m + _blades[i].mag());
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
            T v(1);
            ebase_t e;

            for (unsigned int i = 1; i <= dim; i++) {
                e.push_back(i);
            }

            return Mvec<T>(Blade<T>(v, e));
        }

        static Mvec<T> I(unsigned int dim) {
            return Mvec<T>::pseudoscalar(dim);
        }

    protected:

        std::vector<Blade<T> > _blades;


    };

#ifdef EIGEN_ENABLED

    template<> inline
    Mvec<double>::Mvec(Eigen::Matrix<double, Eigen::Dynamic, 1> &v) {
        _blades = std::vector<Blade<double> >(v.size());
        for (int i = 0; i < v.size(); i++) {
            _blades[i] = Blade<double>(v(i, 0), i + 1);
        }
    }

    template<> inline
    Mvec<double>::Mvec(Eigen::Matrix<double, 1, Eigen::Dynamic> &v) {
        _blades = std::vector<Blade<double> >(v.size());
        for (int i = 0; i < v.size(); i++) {
            _blades[i] = Blade<double>(v(0, i), i + 1);
        }
    }
#endif

}

#endif	/* MVEC_H */

