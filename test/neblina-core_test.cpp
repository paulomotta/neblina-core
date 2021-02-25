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
    GetCLInfo("/etc/neblina/vector.cl");
    std::cin.get();
    showDevicesList();
    return 0;
}