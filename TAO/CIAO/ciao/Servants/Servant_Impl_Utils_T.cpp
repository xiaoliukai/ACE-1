// $Id: Servant_Impl_Utils_T.cpp 92180 2010-10-08 07:51:17Z msmit $

#ifndef CIAO_SERVANT_IMPL_UTILS_T_C
#define CIAO_SERVANT_IMPL_UTILS_T_C

#include "ciao/Servants/Servant_Impl_Base.h"
#include "ciao/Valuetype_Factories/Cookies.h"
#include "ciao/Logger/Log_Macros.h"

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
namespace CIAO
{
  template<typename T_var>
  void
  Servant::describe_pub_event_source (
    const char *port_name,
    const char *port_type_repo_id,
    std::map<ptrdiff_t, T_var> &consumers,
    ::Components::PublisherDescriptions_var &descriptions,
    CORBA::ULong slot)
  {
    CIAO_TRACE ("Servant::describe_pub_event_source");

    ::Components::PublisherDescription *elem = 0;
    ACE_NEW_THROW_EX (elem,
                      ::OBV_Components::PublisherDescription,
                      CORBA::NO_MEMORY ());

    ::Components::PublisherDescription_var safe_elem = elem;

    elem->name (port_name);
    elem->type_id (port_type_repo_id);
    elem->consumers ().length (consumers.size ());

    ptrdiff_t map_slot = 0UL;
    ::Components::SubscriberDescription *sub = 0;

    typedef typename std::map<ptrdiff_t, T_var>::const_iterator
      CONST_ITERATOR;

    for (CONST_ITERATOR iter = consumers.begin ();
         iter != consumers.end ();
         ++iter, ++map_slot)
      {
        ACE_NEW_THROW_EX (sub,
                          ::OBV_Components::SubscriberDescription,
                          CORBA::NO_MEMORY ());
        ::Components::SubscriberDescription_var safe_sub = sub;

        ::Components::Cookie_var key_cookie;
        ACE_NEW (key_cookie,
                 CIAO::Cookie_Impl (iter->first));

        sub->ck (key_cookie.in ());

        sub->consumer (iter->second.in ());

        elem->consumers ()[map_slot] = safe_sub._retn ();
      }

    descriptions[slot] = safe_elem._retn ();
  }

  template<typename T_var>
  void
  Servant::describe_emit_event_source (
      const char *port_name,
      const char *port_type_repo_id,
      T_var &consumer_ref,
      ::Components::EmitterDescriptions_var &descriptions,
      CORBA::ULong slot)
  {
    CIAO_TRACE ("Servant::describe_emit_event_source");
    ::Components::EmitterDescription *elem = 0;
    ACE_NEW_THROW_EX (elem,
                      ::OBV_Components::EmitterDescription,
                      CORBA::NO_MEMORY ());
    ::Components::EmitterDescription_var safe_elem = elem;

    elem->name (port_name);
    elem->type_id (port_type_repo_id);

    // Valuetype 'set' call increments the reference count.
    elem->consumer (consumer_ref.in ());

    descriptions[slot] = safe_elem._retn ();
  }
}
#endif

#endif /* CIAO_SERVANT_IMPL_UTILS_T_C */
