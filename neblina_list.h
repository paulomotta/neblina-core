#ifndef __NEBLINA_LIST_H__
#define __NEBLINA_LIST_H__
#include "neblina.h"


list_t * list_new(); 
list_t * list_append( list_t * L, object_t o ) ;

int list_len( list_t * L ) ;

object_t list_get( list_t * L, int i) ;

#endif
