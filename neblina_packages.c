#include "neblina_packages.h"


int package_add_default() {
    packages[npackages++] = getStdPackage();
    return 0;
}


int package_add( char name[256] ) {
    if( package_is_defined( name ) == 1 )
        return 0;
        
    /*if( strcmp( name, "str") == 0 ) {
        packages[npackages] = getStrPackage();
        npackages++;
        return 0;
    }*/ 
    
    if( strcmp( name, "time") == 0 ) {
        packages[npackages] = getTimePackage();
        npackages++;
        return 0;
    } 
    if( strcmp( name, "io") == 0 ) {
        packages[npackages] = getIOPackage();
        npackages++;
        return 0;
    }
    
    if( strcmp( name, "math") == 0 ) {
        packages[npackages] = getMathPackage();
        npackages++;
        return 0;
    } 
    
    return -1;
}

