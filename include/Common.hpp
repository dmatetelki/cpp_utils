#include <iostream>

#ifndef NOTRACE
  #define TRACE(x) std::cout << x << " " << __PRETTY_FUNCTION__ <<  \
                          " @ " << __FILE__ << " : " << __LINE__ << std::endl
#endif