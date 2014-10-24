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

    if(mxIsClass(a,"mvec")) {
        mxArray *m = mxGetProperty(a,0,"gcamdata");
        if(!m) {
           mexErrMsgTxt("mvec format invalid!"); 
        }

        mwSize nblades = mxGetNumberOfElements(m);     

        for(unsigned int i=0;i<nblades;i++) {
            mxArray *bmx = mxGetCell(m,i);
            
            if(!bmx) {
                mexErrMsgTxt("Failed to read mvec cell");
            }
            //Blade b(bmx);
            _blades.push_back(mxArray2Blade(bmx));
        }
    } else {
        size_t N = mxGetN(a);
        size_t M = mxGetM(a);
        
        if( (N!=1) && (M!=1) ) {
            mexErrMsgTxt("mvec can be initialised with a vector");
        }
        
        size_t L = N*M;
           
        double *v = mxGetPr(a);        
        
        for(size_t l=0;l<L;l++) {
            _blades.push_back(Blade(*v++,l+1));        
        }
        this->prune();
    }
}

mxMvec::mxMvec(const Mvec& orig) : Mvec(orig) {
    
}

//::~mxMvec() {
//}

mxArray *mxMvec::Blade2mxArray(const Blade &b) {

    const char *BfieldNames[] = {"grade", "v", "e"};
    mwSize Bdims[2] = {1, 1};
    mxArray *smxB = mxCreateStructArray(2, Bdims, 3, BfieldNames);
    if (!smxB) {
        mexErrMsgTxt("Failed to create mex blade!");
    }
    
    unsigned int grade = b.grade();
    
    //Set the dim
    int fieldN = mxGetFieldNumber(smxB, "grade");
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

Blade& mxMvec::mxArray2Blade(const mxArray *B) {
    mxArray *mxA = mxGetField(B, 0, "grade");
    if (!mxA || !mxIsClass(mxA,"double")) {
        mexErrMsgTxt("Failed to read field 'grade'!");
    }
    double *data_ptr = mxGetPr(mxA);
    unsigned int grade = (unsigned int) (*data_ptr);

    mxA = mxGetField(B, 0, "v");
    if (!mxA || !mxIsClass(mxA,"double")) {
        mexErrMsgTxt("Failed to read field 'v'!");
    }
    data_ptr = mxGetPr(mxA);
    double v = *data_ptr;
    
    mxA = mxGetField(B, 0, "e");
    if (!mxA || !mxIsClass(mxA,"uint32")) {
        mexErrMsgTxt("Failed to read field 'e'!");
    }
    unsigned int *e_p = (unsigned int *) mxGetData(mxA);
    
    std::vector<unsigned long> e;
    
    for(std::size_t i=0;i<grade;i++) {
        e.push_back(*e_p++);
    }

    return *(new Blade(v,e));
}

mxArray *mxMvec::convert2mxArray(void) {
    //mwSize cellDims[2] = {1, this->blades.size()};
    mxArray *cmxM = mxCreateCellMatrix(1, this->_blades.size());
    
    
    if(!cmxM) {
        mexErrMsgTxt("Failed to allocate memory for mvec cell!");
    }
    
    std::list<Blade>::iterator i;
    mwIndex cindx=0;
    
    for(i=_blades.begin();i!=_blades.end();i++) {
        mxSetCell(cmxM,cindx++,this->Blade2mxArray(*i));
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
