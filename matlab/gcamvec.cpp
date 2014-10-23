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

           mxMvec m(prhs[1]);
           plhs[0] = m.matConvert();
           break;
       
       }       
       
       case 'g': {
           if(nrhs < 3) {
               help();
           }

           if(!mxIsClass(prhs[1],"mvec")) {
               help();
           }

           mxMvec a(prhs[1]);
           double *grade = mxGetPr(prhs[2]);
           
           mxMvec b = a[(unsigned int) (*grade)];
           plhs[0] = b.matConvert();
           break;
       }
       
       case '&': 
       case '^':
       {
           if(nrhs < 3) {
               help();
           }
       
           
           if(!mxIsClass(prhs[1],"mvec")) {
               help();
           }

           if(!mxIsClass(prhs[2],"mvec")) {
               help();
           }

           mxMvec a(prhs[1]);
           mxMvec b(prhs[2]);
           mxMvec c;
           if(*cmd == '&') {
              c=a&b;
           } else {
              c = a^b;
           }
           plhs[0] = c.matConvert();         
           break;
       }

       case '*':
       {

           mxMvec *a;
           mxMvec *b;
           mxMvec c;
           
           if(mxIsClass(prhs[1],"mvec")) {
               a = new mxMvec(prhs[1]);
               if(mxIsClass(prhs[2],"mvec")) {
                   b = new mxMvec(prhs[2]);
                   c = (*a)*(*b);
                   delete(b);
               } else if(mxIsClass(prhs[2],"double")) {
                   double *v = mxGetPr(prhs[2]);
                   c = (*a)*(*v);
               } else {
                   help();
               }
               delete(a);
           } else if(mxIsClass(prhs[1],"double") && mxIsClass(prhs[2],"mvec")) {
               double *v = mxGetPr(prhs[1]);
               b = new mxMvec(prhs[2]);
               c = (*b)*(*v);
               delete(b);
           } else {
               help();
           }
           plhs[0] = c.matConvert();         
           break;
       }
       
       case '-': 
       case '+':
       {
           if(nrhs < 3) {
                help();
           }
       
           mxMvec *a;
           mxMvec *b;
           
           if(mxIsClass(prhs[1],"mvec")) {
               a = new mxMvec(prhs[1]);               
           } else {
               if(mxIsClass(prhs[1],"double")) {
                   double *v = mxGetPr(prhs[1]);
                   mxBlade B(*v);
                   a = new mxMvec();
                   a->addBlade(B);
               } else {
                   help();
               }
           }

           if(mxIsClass(prhs[2],"mvec")) {
               b = new mxMvec(prhs[2]);               
           } else {
               if(mxIsClass(prhs[2],"double")) {
                   double *v = mxGetPr(prhs[2]);
                   mxBlade B(*v);
                   b = new mxMvec();
                   b->addBlade(B);
               } else {
                   help();
               }
           }

           mxMvec c;
           if(*cmd == '+') {
              c = (*a)+(*b);
           } else {
              c = (*a)-(*b);
           }
           plhs[0] = c.matConvert();         
           delete(a);
           delete(b);
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
           
           mxMvec a(prhs[1]);
           a.conj();
           plhs[0] = a.matConvert();         
           break;
       }       
       
       case 'v':
       {
           if(nrhs < 3) {
               help();
           }
       
           if(!mxIsClass(prhs[1],"mvec")) {
               help();
           }
           
           mxMvec a(prhs[1]);
           double *dim = mxGetPr(prhs[2]);
           plhs[0] = a.matVec((unsigned int) *dim);         
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
           
           mxMvec m(prhs[1]);
           cout << m << endl;
           break;
       }        
       default:
           mexErrMsgTxt("Unknown command");
   }
   
}
