// 
//#include <iostream>
//
//#include "gtest/gtest.h"
#include "clutils.h"

//using namespace std;
//
//TEST(sample_test_case, sample_test)
//{
//    showDevicesList();
//    
//    EXPECT_EQ(1, 1);
//    
//}
#include <iostream>
#include <CL/opencl.h>
#include "oclvector.h"

int main() {
    std::cout << "Test OCL  without driver" << std::endl;
    

    cl_int err;
    cl_uint num_platforms;

    err = clGetPlatformIDs(0, NULL, &num_platforms);
    if (err == CL_SUCCESS) {
        std::cout << "Success. Platforms available: " << num_platforms
                << std::endl;
    } else {
        std::cout << "Error. Platforms available: " << num_platforms
                << std::endl;
    }

    std::cout << "Test OCL without driver" << std::endl;
    std::cout << "Press button to exit." << std::endl;
    std::cin.get();
    InitCLEngine();
    //GetCLInfo("/etc/neblina/vector.cl");
    std::cin.get();
    showDevicesList();
    int n = 3;
    double * v1 = (double *) malloc( n * sizeof(double) );
    double * v2 = (double *) malloc( n * sizeof(double) );
    double * res = (double *) malloc( n * sizeof(double) );
    
    v1[0]=1.;
    v1[1]=2.;
    v1[2]=3.;
    v2[0]=3.;
    v2[1]=2.;
    v2[2]=1.;
    
    res = addVector( v1, v2, n );
    
    return 0;
}