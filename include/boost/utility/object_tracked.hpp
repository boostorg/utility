//Simple class which keeps track of members
#ifndef UTILITY_OBJECT_TRACKED_HPP_LJE20040112
#define UTILITY_OBJECT_TRACKED_HPP_LJE20040112
#include <set>
#include "boost/utility/obj_id.hpp"
#include "boost/io/filters/mout.hpp"
namespace utility
{
class object_tracked
: public obj_id
{
 public:
      struct 
    set_of_objects_tracked
      : public std::set<object_tracked const*>
    {
            typedef 
          std::set<object_tracked const*>
        super_type
        ;
        set_of_objects_tracked(void)
        {}
          void
        reset(void)
        {
            super_type::clear();
            obj_id::the_last_id() = 0;
        }
          bool
        contains(object_tracked const*& a_obj)const
        {
            return find(a_obj) != end();
        }
        
    };
        static
      set_of_objects_tracked
    our_members
    ;
        static
      unsigned
    members_size(void)
    {
        return our_members.size();
    }
    
    object_tracked(void)
    {
        our_members.insert(this);
      #ifdef UTILITY_OBJECT_TRACKED_TRACE_MODE
        mout()<<"object_tracked+:id="<<id_get()<<"\n";
      #endif
    }
    #if 0
        virtual 
    #endif
    ~object_tracked(void) 
    { 
      #ifdef UTILITY_OBJECT_TRACKED_TRACE_MODE
        mout()<<"object_tracked-:id="<<id_get()<<":am_i_live="<<am_i_live()<<"\n";
      #endif  
        utility::object_tracked const*me=this;
        our_members.erase(me); 
    }
        bool
    am_i_live(void)const
    //Purpose:
    //  Used to check for multiple destructor calls.  Should always return true
    //  if only place where our_members.erase is called is in
    //  ~object_tracked.
    {
        utility::object_tracked const*me=this;
        bool result=utility::object_tracked::our_members.contains(me);
        return result;
    }
        static
      bool
    is_live(utility::object_tracked const*a_ot)
    {
        bool result=utility::object_tracked::our_members.contains(a_ot);
        return result;
    }

};

}//exit utility namespace

#endif

