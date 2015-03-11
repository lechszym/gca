#include "mex.h"

#include "mxMvec.h"
#include <iostream>
#include <stdio.h>

using namespace std;

void help(void) {
    mexErrMsgTxt("Error!");
}

void mexFunction(int nlhs, mxArray *plhs[ ], int nrhs, const mxArray *prhs[]) {

   if(!mxIsChar(prhs[0])) {
       help();
   }
   
   char *cmd = (char *) mxGetPr(prhs[0]);
   
   switch(*cmd) {

       case 'n': {
           if(nrhs < 2) {
               help();
           }

           mxMvec<double> m(prhs[1]);
           plhs[0] = m.convert2mxArray();
           break;
       
       }       
       
       case 'g': {
           if(nrhs < 3) {
               help();
           }

           if(!mxIsClass(prhs[1],"mvec")) {
               help();
           }

           mxMvec<double> a(prhs[1]);
           double *grade = mxGetPr(prhs[2]);
           
           mxMvec<double> b = a[(unsigned int) (*grade)];
           plhs[0] = b.convert2mxArray();
           break;
       }
       
       case '&': 
       case '^':
       case '*':
       case '+':
       case '-':
       case '/':
       {
           if(nrhs < 3) {
               help();
           }
       
           mxMvec<double> a(prhs[1]);
           mxMvec<double> b(prhs[2]);
           mxMvec<double> c;
           
           switch(*cmd) {
            case '&': 
                c = a&b;
                break;
                
            case '^':
                c = a^b;
                break;
                
            case '*':
                c = a*b;
                break;
                
            case '/': 
                c = a/b;
                break;                
                
            case '+':
                c = a+b;
                break;
                
            case '-': 
                c = a-b;
                break;
           }
           
           plhs[0] = c.convert2mxArray();         
           break;
       }

       case '~':
       {
           if(nrhs < 2) {
               help();
           }
       
           if(!mxIsClass(prhs[1],"mvec")) {
               help();
           }
           
           mxMvec<double> a(prhs[1]);
           mxMvec<double> b = ~a;
           plhs[0] = b.convert2mxArray();         
           break;
       }       
       
       case 'v':
       {
           if(nrhs < 2) {
               help();
           }
       
           if(!mxIsClass(prhs[1],"mvec")) {
               help();
           }
           
           mxMvec<double> a(prhs[1]);
           if(nrhs > 2) {
                double *dim = mxGetPr(prhs[2]);
                plhs[0] = a.matVec((unsigned int) *dim);                        
           } else {
                plhs[0] = a.matVec();         
           }
           
           break;
       }  
       case 'p': {
           if(nrhs < 2) {
               help();
           }

           if(!mxIsClass(prhs[1],"mvec") && 
              !mxIsClass(prhs[1],"smvec")) {
               help();
           }
           
           mxMvec<double> m(prhs[1]);
           printf("%s\n", m.toString().c_str());
           break;
       }        
       default:
           mexErrMsgTxt("Unknown command");
   }
   
}
