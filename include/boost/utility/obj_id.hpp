#ifndef UTILITY_OBJ_ID_HPP
#define UTILITY_OBJ_ID_HPP
namespace utility
  {
  struct obj_id
   /**@class obj_id
    * @brief Provide unique id for objects
    */
    {
          typedef
        unsigned
      id_type
        ;
          virtual //To make subj_described* start with obj_id*
                  //See stl_container.log@@2003-06-05.0750
      ~obj_id(void)
        {}
      obj_id(void)
        :m_id(the_last_id()++)
        {}
      obj_id(obj_id const&)
        :m_id(the_last_id()++)
        {}
        void
      operator=(obj_id const&)
        {}
        id_type
      id_get(void)const
        { return m_id
        ;}
          static
        id_type&
      the_last_id(void)
        //reference to count if id's created
        ;
    private:
        id_type
      m_id
        ;
    };//end obj_id struct
  }//exit utility namespace
#endif

