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
         _pow = 1;
      }

      Sym(const double v) {
          _v = v;
          _pow = 1;
      }
      
      Sym(const char s[]) : Sym(1.0)  {
         Sym a;
         a._v = 1.0;
         a._sym = std::string(s);
         _mult.push_back(a);
      }
      
      Sym(const Sym& orig) {
         _v = orig._v;
         _sym = orig._sym;
         _pow = orig._pow;
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
       
         if(m._mult.empty()) {
             return (*this) * m._v;
         } 
          
         if(m._mult.empty()) {
             return m * _v;
         }
         
         
         Sym a(*this);
         Sym b(m);

         a._v *= b._v;
         b._v = 1.0;

         a._mult.insert(a._mult.end(), b._mult.begin(), b._mult.end());

         std::sort(a._mult.begin(), a._mult.end(), Sym::compByString);         
         
         for(std::size_t i=0;i<a._sum.size();i++) {
            a._sum[i] = a._sum[i]*m;
         }

         for(std::size_t i=0;i<b._sum.size();i++) {
            Sym c = b._sum[i]*(*this);
            a = a+c;
         }
            
         return a;
      }

      /*bool operator==(const Sym& m) const {
         if(_mult.size() != m._mult.size()) {
            return false;
         }
         
         for(std::size_t i=0;i<_mult.size();i++) {
            if( (_mult[i]._sym != b[i]._sym) ||
                (a[i]._pow != b[i]._pow)) {
               return false;
            }
         }
         
         
         return true;

         return false;
      }*/
      
      Sym operator*(const double x) const {
         Sym a = Sym(*this);
         a._v *= x;
         return a;
      }
      
      Sym operator+(const Sym& m) const {
         Sym a;
         
         if(_v == 0.0) {
            return m;
         } else if(m._v == 0.0) {
            return *this;
         } else {
            if(equal(_mult,m._mult)) {
               if(_sum.empty()) {
                  if(m._sum.empty()) {
                     a = Sym(*this);
                     a._v += m._v;
                     return a;
                  } else {
                     a = Sym(m);
                     a._sum.push_back(Sym(_v));
                     return a;
                  }
               } else if(m._sum.empty()) {
                  a = Sym(*this);
                  a._sum.push_back(Sym(m._v));
                  return a;
               } else {
                  a = Sym(*this);
                  a._sum.insert(a._sum.end(), m._sum.begin(), m._sum.end());
                  return a;
               }
            } else {
               a = Sym(*this);
               a._sum.push_back(m);
               return a;
            }
         } 
      }

      Sym operator-(const Sym& m) const {
         Sym b = Sym(m);

         if(b._v != 0.0) {
            b._v *= -1;
            return (*this+b);            
         } else {
            return *this;
         }
      }

      bool operator>=(double v) const {
         return _v >= v;
      }

      bool operator<(double v) const {
         return _v < v;
      }
      
      bool operator>(double v) const {
         return _v > v;
      }      
            
      std::string toString() const {
        std::stringstream ss;

        if(_v == 0.0) {
           ss << "0";
           return ss.str();
        }
        
        if(_pow == -1) {
           ss << "/";
        }
        
        if(_v < 0.0) {
           ss << "-";
        }

        if(!_sum.empty()) {
           ss << "(";
        }

        if( (std::abs(_v) != 1.0) || (_mult.empty() && _sym.empty())) {
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
      
       //bool operator==(const Blade& b) const {
      
   private:
      
      bool equal(const std::vector<Sym>& a, const std::vector<Sym>& b) const {
         
         if(a.size() != b.size()) {
            return false;
         }
         
         for(std::size_t i=0;i<a.size();i++) {
            if( (a[i]._sym != b[i]._sym) ||
                (a[i]._pow != b[i]._pow)) {
               return false;
            }
         }
         
         
         return true;
      }
      
      static bool compByString(const Sym &a, const Sym &b) {
         return (a._sym<b._sym); 
      }
      
      std::string _sym;
      double      _v;
      int         _pow;
      std::vector<Sym> _mult;
      std::vector<Sym> _sum;
   };
   
   inline Sym operator*(double v, const Sym& m) {
      return m*Sym(v);
   }    

   inline Sym operator+(double v, const Sym& m) {
      return m+Sym(v);
   }       
}
   
      

#endif	/* SYM_H */

