#include <boost/python.hpp>
#include <Mvec>

using namespace boost::python;
using namespace gca;

BOOST_PYTHON_MODULE(gca) {
   class_<Mvec<float> >("_cMvec", init<float, unsigned long>())
           ;

}