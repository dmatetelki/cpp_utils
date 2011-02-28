#include <iostream>

#ifndef NOTRACE
  #define TRACE(x) std::cout << x << " " << __PRETTY_FUNCTION__ <<  \
                          " : " << __LINE__ << std::endl

#else
  /// @todo Get rid of the "warning: statement has no effect" compiler msgs
  #define TRACE(x) ""
#endif
