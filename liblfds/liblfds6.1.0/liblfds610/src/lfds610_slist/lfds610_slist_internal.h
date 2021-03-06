/***** the library wide include file *****/
#include "liblfds610_internal.h"

/***** defines *****/
#define LFDS610_SLIST_USER_DATA  0
#define LFDS610_SLIST_FLAGS      1

#define LFDS610_SLIST_NO_FLAGS      0x0
#define LFDS610_SLIST_FLAG_DELETED  0x1

/***** structures *****/
#pragma pack( push, LFDS610_ALIGN_SINGLE_POINTER )

struct lfds610_slist_state
{
  struct lfds610_slist_element
    *volatile head;

  void
    (*user_data_delete_function)( void *user_data, void *user_state ),
    *user_state;
};

#pragma pack( pop )

#pragma pack( push, LFDS610_ALIGN_DOUBLE_POINTER )

/* TRD : this pragma pack doesn't seem to work under Windows
         if the structure members are the correct way round
         (next first), then user_data_and_flags ends up on
         a single pointer boundary and DCAS crashes

         accordingly, I've moved user_data_and_flags first
*/

struct lfds610_slist_element
{
  void
    *volatile user_data_and_flags[2];

  // TRD : requires volatile as is target of CAS
  struct lfds610_slist_element
    *volatile next;
};

#pragma pack( pop )

/***** private prototypes *****/
void lfds610_slist_internal_init_slist( struct lfds610_slist_state *ss, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

void lfds610_slist_internal_link_element_to_head( struct lfds610_slist_state *lfds610_slist_state, struct lfds610_slist_element *volatile se );
void lfds610_slist_internal_link_element_after_element( struct lfds610_slist_element *volatile lfds610_slist_in_list_element, struct lfds610_slist_element *volatile se );

void lfds610_slist_internal_move_to_first_undeleted_element( struct lfds610_slist_element **se );

