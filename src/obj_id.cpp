#include "boost/utility/obj_id.hpp"
using namespace utility;
//          static
obj_id::
        id_type&
obj_id::
      the_last_id(void)
        { static id_type last_id=0
        ; return last_id
        ;}
