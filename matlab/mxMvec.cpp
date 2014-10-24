/* 
 * File:   mxMvec.cpp
 * Author: lechszym
 * 
 * Created on August 9, 2012, 8:41 PM
 */

#include "mxMvec.h"

using namespace gca;

mxMvec::mxMvec() : Mvec() {

}

/*mxMvec::mxMvec(const char *eStr,unsigned int dim) {
    unsigned int i=0;
    bool beg=true;
    
    while(i<strlen(eStr)) {
        while(eStr[i]==' ') {
            i++;
        }
        if(beg || eStr[i]=='-' || eStr[i]=='+') {
            mxBlade b(eStr+i,dim);
            this->blades.push_back(b);
        }
        beg = false;
        i++;
    }
    this->prune(); 
}

mxMvec::mxMvec(double *v,size_t length) {
    for(unsigned int i=1;i<=length;i++) {
        if(*v>GCA_PRECISION || *v<(-GCA_PRECISION)) {
            mxBlade b(*v,i,length);
            this->blades.push_back(b);
        }
        v++;
    }
}*/
    

mxMvec::mxMvec(const mxArray *a) {

    
    /*mxArray *m = mxGetProperty(mvec,0,"gcamdata");
    if(!m) {
       mexErrMsgTxt("mvec format invalid!"); 
    }
    
    mwSize nblades = mxGetNumberOfElements(m);     

    for(unsigned int i=0;i<nblades;i++) {
        mxArray *bmx = mxGetCell(m,i);
        
        if(!bmx) {
            mexErrMsgTxt("Failed to read mvec cell");
        }
        mxBlade b(bmx);
        this->blades.push_back(b);
    } */   
}

mxMvec::mxMvec(const Mvec& orig) : Mvec(orig) {
    
}

//::~mxMvec() {
//}

mxArray *mxMvec::matConvert(const Blade &b) {

    const char *BfieldNames[] = {"dim", "v", "e"};
    mwSize Bdims[2] = {1, 1};
    mxArray *smxB = mxCreateStructArray(2, Bdims, 3, BfieldNames);
    if (!smxB) {
        mexErrMsgTxt("Failed to create mex blade!");
    }
    
    unsigned int grade = b.grade();
    
    //Set the dim
    int fieldN = mxGetFieldNumber(smxB, "dim");
    mxSetFieldByNumber(smxB, 0, fieldN, mxCreateDoubleScalar(grade));

    //Set the v
    fieldN = mxGetFieldNumber(smxB, "v");
    mxSetFieldByNumber(smxB, 0, fieldN, mxCreateDoubleScalar(b.get()));

    mxArray *cmxE = mxCreateNumericMatrix(1, b.grade(), mxUINT32_CLASS, mxREAL);
    if(!cmxE) {
        mexErrMsgTxt("Failed to allocate memory for blade during conversion.");
    }
            
    char *ew = (char *) mxGetData(cmxE);
    for(unsigned int i=0;i<grade;i++) {
        *ew++ = b[i];
    }
     
    fieldN = mxGetFieldNumber(smxB, "e");
    mxSetFieldByNumber(smxB, 0, fieldN, cmxE);
    return smxB;
}


mxArray *mxMvec::matConvert(void) {
    //mwSize cellDims[2] = {1, this->blades.size()};
    mxArray *cmxM = mxCreateCellMatrix(1, this->_blades.size());
    
    
    if(!cmxM) {
        mexErrMsgTxt("Failed to allocate memory for mvec cell!");
    }
    
    std::list<Blade>::iterator i;
    mwIndex cindx=0;
    
    for(i=_blades.begin();i!=_blades.end();i++) {
        mxSetCell(cmxM,cindx++,this->matConvert(*i));
    }
    return cmxM;
}


mxArray *mxMvec::matVec(unsigned int dim){
    mxArray *mxV = mxCreateDoubleMatrix(dim,1,mxREAL);
    if(!mxV) {
        mexErrMsgTxt("Failed to allocate memory for mx vector!");        
    }
    /*
    double *v = mxGetPr(mxV);
    std::list<Blade>::iterator i;
    for(i=this->blades.begin();i!=this->blades.end();i++) {
        if(i->grade()==1) {
            unsigned int e = i->eFirst();
            if((e > 0) && (e <= dim)) {
                v[e-1] = i->getV(); 
            }
        }
    }*/
    
    return mxV;
}
