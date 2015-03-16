/* 
 * File:   gca.h
 * Author: lechszym
 *
 * Created on 17 March 2015, 9:25 AM
 */

#ifndef GCA_H
#define	GCA_H



namespace gca {
    
#ifndef GCA_PRECISION
#define GCA_PRECISION  1e-12
#endif

#ifndef GCA_PRECISION
#define GCA_PRECISION  1e-12
#endif

inline  double fix_precision(double x) {
    if( (x < GCA_PRECISION) && (x > -GCA_PRECISION)) {
        return 0.0;
    } else {
        return x;
    }
}
    
    
}

#endif	/* GCA_H */

