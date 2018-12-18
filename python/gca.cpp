#include <boost/python.hpp>
#include <Mvec>

using namespace boost::python;
using namespace gca;

BOOST_PYTHON_MODULE(gca) {
   class_<Mvec<float> >("mvec", init<float, unsigned long>())
      .def(init<char *>())

      .def(self & self)
      .def(self ^ self)
      .def(self += self)
      .def(self + self)
      //.def(self -= self)
      .def(self - self)
      //.def(self *= self)
      .def(self * self)
      //.def(self /= self)
      .def(self / self)
   
   
      .def(self * float())
      .def(self += self)
      .def(self += float())
      .def(self_ns::str(self_ns::self))
   ;

}