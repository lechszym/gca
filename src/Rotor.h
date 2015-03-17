/* 
 * File:   Rotor.h
 * Author: lechadmin
 *
 * Created on March 17, 2015, 2:34 PM
 */

#ifndef ROTOR_H
#define	ROTOR_H

#include "Mvec.h"


namespace gca {

    template <typename T>
    class Rotor: public Mvec<T> {
    public:
        Rotor() {
        }

        Rotor(const Mvec<T> &n, const Mvec<T> &m) {
           Mvec<T> _n =  n[1];
           Mvec<T> _m =  m[1];
           
           _n = _n.norm();
           _m = _m.norm();
           
           Rotor<T> R = _n*_m;
           
           this->_blades = R._blades;
           
        }

        Rotor(double angleX, double angleY, double angleZ, bool radians=true) {

           Mvec<T> w;
           Mvec<T> B;
           Rotor<T> Rx,Ry,Rz,R;

           if(~radians) {
              angleX /= 180 * M_PI;
              angleY /= 180 * M_PI;
              angleZ /= 180 * M_PI;
           }
           
           w = Mvec<T>(cos(angleX/2));
           B = Mvec<T>(1,2)*Mvec<T>(1,3)*(-sin(angleX/2));
           
           Rx += w;
           Rx += B;
           
           w = Mvec<T>(cos(angleY/2));
           B = Mvec<T>(1,1)*Mvec<T>(1,3)*(-sin(angleY/2));

           Ry += w;
           Ry += B;

           w = Mvec<T>(cos(angleZ/2));
           B = Mvec<T>(1,1)*Mvec<T>(1,2)*(-sin(angleZ/2));

           Rz += w;
           Rz += B;

           Mvec<T> bz = Rx*Ry*Rz;
           
           R = Rotor(Rx*Ry*Rz);
           
           this->_blades = R._blades;

        }

        Rotor(const Mvec<T>& orig) : Mvec<T>(orig) {
           
        }
        
        virtual ~Rotor() {

        }
        
        
        
    };
}

#endif	/* ROTOR_H */

