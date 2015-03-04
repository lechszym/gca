/* 
 * File:   Sym.h
 * Author: lechszym
 *
 * Created on 3 March 2015, 9:47 PM
 */

#ifndef SYM_H
#define	SYM_H

#include <string>

namespace gca {
  
   class Sym {
   public:

      Sym() {
         _v = 0.0;
         _inv = false;
      }

      Sym(const char s[]) {
         _v = 1.0;
         _sym = std::string(s);
         _inv = false;
      }
      
      Sym(const Sym& orig) {
         _v = orig._v;
         _sym = orig._sym;
         _inv = orig._inv;
         _mult = orig._mult;
         _sum = orig._sum;
      }

      virtual ~Sym() {

      }

      /*std::string toString() const {
         std::stringstream ss;

         ss << _sym.c_str();
         
         return ss.str();
      }*/
      
      Sym operator*(const Sym& m) const {
         Sym a(*this);
         Sym b(m);

         a._v *= b._v;
         b._v = 1.0;
         a._mult.push_back(b);
         
         for(std::size_t i=0;i<a._sum.size();i++) {
            a._sum[i] = a._sum[i]*m;
         }

         for(std::size_t i=0;i<b._sum.size();i++) {
            a._sum.push_back(b._sum[i]*(*this));
         }
         
         return a;
      }

      Sym operator*(const double x) const {
         Sym a = Sym(*this);
         a._v *= x;
         return a;
      }
      
      Sym operator+(const Sym& m) const {
         Sym a = Sym(*this);
         a._sum.push_back(m);
         return a;
      }
 
      std::string toString() const {
        std::stringstream ss;

        if(_inv) {
           ss << "/";
        }
        
        if(_v < 0.0) {
           ss << "-";
        }

        if(!_sum.empty()) {
           ss << "(";
        }

        if(std::abs(_v) != 1.0) {
           ss << std::abs(_v);
        }
        
        ss << _sym;

        for(std::size_t i=0;i<_mult.size();i++) {
           ss << _mult[i];
        } 
        
        for(std::size_t i=0;i<_sum.size();i++) {
           if(_sum[i]._v > 0.0) {
              ss << "+";
           }
           ss << _sum[i];
        }
        
        if(!_sum.empty()) {
           ss << ")";
        }
        
        return ss.str();
        
      }
      
      friend std::ostream& operator<<(std::ostream &out, const Sym &s) {      
         out << s.toString();
         return out;      
      }
   
   private:
      std::string _sym;
      double      _v;
      bool        _inv;
      std::vector<Sym> _mult;
      std::vector<Sym> _sum;
   };
   
}


#endif	/* SYM_H */

