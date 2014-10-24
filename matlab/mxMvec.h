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

class mxMvec : public gca::Mvec {
public:
    mxMvec();
    //mxMvec(const char *eStr,unsigned int dim=0);
    //mxMvec(double *v,size_t length);
    mxMvec(const mxArray *m);
    mxMvec(const Mvec& orig);
    mxArray* matConvert(void);
    mxArray* matVec(unsigned int dim);         

private:
    mxArray* matConvert(const gca::Blade &b);
   
    
};

#endif	/* MXMvec_H */

