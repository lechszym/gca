/* 
 * File:   mxMvec.h
 * Author: lechszym
 *
 * Created on August 9, 2012, 8:42 PM
 */

#ifndef MXMVEC_H
#define	MXMVEC_H

#include "mex.h"
#include <Mvec>

class mxMvec : public gca::Mvecd {
public:
    mxMvec();
    //mxMvec(const char *eStr,unsigned int dim=0);
    //mxMvec(double *v,size_t length);
    mxMvec(const mxArray *m);
    mxMvec(const Mvec& orig);
    mxArray* convert2mxArray(void);
    mxArray* matVec();
    mxArray* matVec(unsigned int dim);         

private:
    static mxArray* Blade2mxArray(const gca::Bladed &b);
    static gca::Bladed&   mxArray2Blade(const mxArray *mxb);
    
};

#endif	/* MXMvec_H */

