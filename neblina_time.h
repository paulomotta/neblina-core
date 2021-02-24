#ifndef __NEBLINATIME__
#define __NEBLINATIME__

#include "package.h"

/**
 * Time Package
 */
package_t * getTimePackage();

void ** neblina_time_sec( void ** i, int * status );
void ** neblina_time_milli( void ** i, int * status );
void ** neblina_time_micro( void ** i, int * status );

#endif
