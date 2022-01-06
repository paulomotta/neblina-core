#include "clutils.h"
#include "neblina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


size_t roundUp(int group_size, int global_size)
{   if( group_size < global_size )
        return global_size;
    int r = global_size % group_size;
    if(r == 0)
    {
        return (group_size/global_size)*global_size;
    } else
    {
        return (group_size/global_size + 1)*global_size;
    }
}


void checkErrors(cl_int status, char *label, int line)
{
  switch (status)
  {
      case CL_SUCCESS:
        return;
      case CL_BUILD_PROGRAM_FAILURE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_BUILD_PROGRAM_FAILURE\n", label, line);
        break;
      case CL_COMPILER_NOT_AVAILABLE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_COMPILER_NOT_AVAILABLE\n", label, line);
        break;
      case CL_DEVICE_NOT_AVAILABLE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_DEVICE_NOT_AVAILABLE\n", label, line);
        break;
      case CL_DEVICE_NOT_FOUND:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_DEVICE_NOT_FOUND\n", label, line);
        break;
      case CL_IMAGE_FORMAT_MISMATCH:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_IMAGE_FORMAT_MISMATCH\n", label, line);
        break;
      case CL_IMAGE_FORMAT_NOT_SUPPORTED:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_IMAGE_FORMAT_NOT_SUPPORTED\n", label, line);
        break;
      case CL_INVALID_ARG_INDEX:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_ARG_INDEX\n", label, line);
        break;
      case CL_INVALID_ARG_SIZE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_ARG_SIZE\n", label, line);
        break;
      case CL_INVALID_ARG_VALUE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_ARG_VALUE\n", label, line);
        break;
      case CL_INVALID_BINARY:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_BINARY\n", label, line);
        break;
      case CL_INVALID_BUFFER_SIZE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_BUFFER_SIZE\n", label, line);
        break;
      case CL_INVALID_BUILD_OPTIONS:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_BUILD_OPTIONS\n", label, line);
        break;
      case CL_INVALID_COMMAND_QUEUE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_COMMAND_QUEUE\n", label, line);
        break;
      case CL_INVALID_CONTEXT:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_CONTEXT\n", label, line);
        break;
      case CL_INVALID_DEVICE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_DEVICE\n", label, line);
        break;
      case CL_INVALID_DEVICE_TYPE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_DEVICE_TYPE\n", label, line);
        break;
      case CL_INVALID_EVENT:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_EVENT\n", label, line);
        break;
      case CL_INVALID_EVENT_WAIT_LIST:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_EVENT_WAIT_LIST\n", label, line);
        break;
      case CL_INVALID_GL_OBJECT:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_GL_OBJECT\n", label, line);
        break;
      case CL_INVALID_GLOBAL_OFFSET:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_GLOBAL_OFFSET\n", label, line);
        break;
      case CL_INVALID_HOST_PTR:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_HOST_PTR\n", label, line);
        break;
      case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_IMAGE_FORMAT_DESCRIPTOR\n", label, line);
        break;
      case CL_INVALID_IMAGE_SIZE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_IMAGE_SIZE\n", label, line);
        break;
      case CL_INVALID_KERNEL_NAME:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_KERNEL_NAME\n", label, line);
        break;
      case CL_INVALID_KERNEL:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_KERNEL\n", label, line);
        break;
      case CL_INVALID_KERNEL_ARGS:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_KERNEL_ARGS\n", label, line);
        break;
      case CL_INVALID_KERNEL_DEFINITION:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_KERNEL_DEFINITION\n", label, line);
        break;
      case CL_INVALID_MEM_OBJECT:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_MEM_OBJECT\n", label, line);
        break;
      case CL_INVALID_OPERATION:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_OPERATION\n", label, line);
        break;
      case CL_INVALID_PLATFORM:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_PLATFORM\n", label, line);
        break;
      case CL_INVALID_PROGRAM:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_PROGRAM\n", label, line);
        break;
      case CL_INVALID_PROGRAM_EXECUTABLE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_PROGRAM_EXECUTABLE\n", label, line);
        break;
      case CL_INVALID_QUEUE_PROPERTIES:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_QUEUE_PROPERTIES\n", label, line);
        break;
      case CL_INVALID_SAMPLER:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_SAMPLER\n", label, line);
        break;
      case CL_INVALID_VALUE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_VALUE\n", label, line);
        break;
      case CL_INVALID_WORK_DIMENSION:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_WORK_DIMENSION\n", label, line);
        break;
      case CL_INVALID_WORK_GROUP_SIZE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_WORK_GROUP_SIZE\n", label, line);
        break;
      case CL_INVALID_WORK_ITEM_SIZE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_INVALID_WORK_ITEM_SIZE\n", label, line);
        break;
      case CL_MAP_FAILURE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_MAP_FAILURE\n", label, line);
        break;
      case CL_MEM_OBJECT_ALLOCATION_FAILURE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_MEM_OBJECT_ALLOCATION_FAILURE\n", label, line);
        break;
      case CL_MEM_COPY_OVERLAP:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_MEM_COPY_OVERLAP\n", label, line);
        break;
      case CL_OUT_OF_HOST_MEMORY:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_OUT_OF_HOST_MEMORY\n", label, line);
        break;
      case CL_OUT_OF_RESOURCES:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_OUT_OF_RESOURCES\n", label, line);
        break;
      case CL_PROFILING_INFO_NOT_AVAILABLE:
        fprintf(stderr, "Neblina OpenCL runtime error (at %s, line %d): CL_PROFILING_INFO_NOT_AVAILABLE\n", label, line);
        break;
      default:
        fprintf(stderr, "OpenCL unknow error (at %s, line %d) ERROR: %d\n", label, line, status);
        break;
    
  }
  exit(status);
}


char* filetobuf(char *file)
{
    FILE *fptr;
    long length;
    char *buf;
 
    fptr = fopen(file, "rb"); 
    if (!fptr)
        return NULL;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr); 
    buf = (char*)malloc(length+1);
    fseek(fptr, 0, SEEK_SET);
    int a = fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;
 
    return buf;
}

cl_info GetCLInfo( char * filename ) {
    #undef DEBUGCL
    cl_int status;
    cl_uint numPlatforms;
    cl_uint num_compute_units;
    int k = 0;    
    status = clGetPlatformIDs (0, NULL, &numPlatforms);
    //printf("%u",numPlatforms);
    CLERR
    cl_device_type targetDeviceType = proctype;
    int id = 0;
    if (numPlatforms <= 0) {
        fprintf (stderr, "Neblina Neblina OpenCL runtime error: No platform found.\n");
        exit (-1);
    }
    int findDevice = 0;
    // get information about all platforms in this system
    cl_platform_id* allPlatforms = (cl_platform_id*) malloc (numPlatforms*sizeof(cl_platform_id));
    status = clGetPlatformIDs (numPlatforms, allPlatforms, NULL);
    CLERR

    // iterate over platforms and choose first device that matches
    // the type chosen above (greedy search)
    cl_device_id currentDevice = NULL;
    cl_platform_id currentPlatform = NULL;
    cl_device_id* allDevices = NULL;
    cl_uint iplatform;
    for (iplatform=0; iplatform < numPlatforms && findDevice == 0; iplatform++)
    {
        currentPlatform = allPlatforms[iplatform];
#ifdef DEBUGCL
            // query more detailed info about current platfor


            char profile[100];
            status = clGetPlatformInfo (currentPlatform, CL_PLATFORM_PROFILE, sizeof(profile), profile, NULL);
            CLERR
            char version[100];
            status = clGetPlatformInfo (currentPlatform, CL_PLATFORM_VERSION, sizeof(version), version, NULL);
            CLERR
            char name[100];
            status = clGetPlatformInfo (currentPlatform, CL_PLATFORM_NAME, sizeof(name), name, NULL);
            CLERR
            char vendor[100];
            status = clGetPlatformInfo(currentPlatform, CL_PLATFORM_VENDOR,  sizeof(vendor), vendor, NULL);
            CLERR
            printf ("OpenCL Platform found:\n");
            printf ("   Name: %s\n", name);
            printf (" Vendor: %s\n", vendor);
            printf ("Version: %s\n", version);
            printf ("Profile: %s\n", profile);
#endif
        // get number of devices available on current platform that match target type
        cl_uint numDevices;
        status = clGetDeviceIDs (currentPlatform, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);


        if (status != CL_DEVICE_NOT_FOUND) {
          CLERR
        }
        /*
        if( numDevices <= 0 ) {
            targetDeviceType = CL_DEVICE_TYPE_GPU;
            status = clGetDeviceIDs (currentPlatform, targetDeviceType, 0, NULL, &numDevices);
            if (status != CL_DEVICE_NOT_FOUND) {
              CLERR
            }
        }*/
#ifdef DEBUGCL
        printf ("Number of devices in this platform that match specified type: %d\n", numDevices);
#endif
        // get information of all devices available on current platform
        if (numDevices > 0)
        {
            allDevices = (cl_device_id*) malloc (numDevices*sizeof(cl_device_id));
            status = clGetDeviceIDs (currentPlatform, CL_DEVICE_TYPE_ALL, numDevices, allDevices, NULL);
            checkErrors (status, "clGetDeviceIDs", __LINE__);
            // greedily pick first device
            
            for( k = 0; k < numDevices; k ++ ) {
               // printf("id = %d id_device = %d\n", id, id_device );        
                if( id_device == id ) {                 
                    currentDevice = allDevices[k];
                    findDevice = 1;
                    break;                
                }                
                id++;            
            }
        }
    }
    if (!findDevice) {
        fprintf (stderr, "Neblina OpenCL runtime error: No matching device found! \n");
        exit( -1);
    }    
#ifdef DEBUGCL
    // print out some information on the device we'll be using.
    // Check OpenCL documentation for all supported parameters (there's a lot of them)
    char name[100];
    status = clGetDeviceInfo (currentDevice, CL_DEVICE_NAME, sizeof(name), name, NULL);
    checkErrors (status, "clGetDeviceInfo", __LINE__);
    printf ("Matching device found:\n");
    printf ("--------------> Name: %s\n", name);
#endif
    
    cl_device_fp_config fp_config;
    clGetDeviceInfo(currentDevice, CL_DEVICE_DOUBLE_FP_CONFIG, sizeof(fp_config), &fp_config, NULL);
    CLERR

    clGetDeviceInfo(currentDevice, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(num_compute_units), &num_compute_units, NULL);            
    CLERR
    // clean up and exit loop over platforms
    free (allPlatforms);

    

    
    cl_context_properties contextProperties[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)currentPlatform, 0 };
    cl_context context;
    context = clCreateContext (contextProperties, 1, &currentDevice, NULL, NULL, &status);
    CLERR
    cl_command_queue commandQueue;  
    commandQueue = clCreateCommandQueue (context, currentDevice, 0, &status);
    CLERR
    const char * source  = filetobuf( filename );
    int i;
    cl_program program = clCreateProgramWithSource (context, 1, &source, NULL, &status);
    CLERR
    free( source );
    const char options[] = "-Werror -cl-std=CL1.1";
    status = clBuildProgram (program, 1, &currentDevice, options, NULL, NULL);
    if (status == CL_BUILD_PROGRAM_FAILURE) {
        char buildLog[1024];
        status = clGetProgramBuildInfo (program, currentDevice, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
        CLERR
        fprintf (stderr, "Program Build Log:\n%s\n", buildLog);
        status = CL_BUILD_PROGRAM_FAILURE;
    }
    
    CLERR
    cl_info info;
    info.c = context;
    info.p = program;
    info.q = commandQueue;
    info.n = num_compute_units;
    info.d = currentDevice;
    if( (fp_config & CL_FP_FMA) | (fp_config & CL_FP_ROUND_TO_NEAREST) | (fp_config & CL_FP_ROUND_TO_ZERO) | (fp_config & CL_FP_ROUND_TO_INF) | (fp_config & CL_FP_INF_NAN) | (fp_config & CL_FP_DENORM) ) 
        info.fp64 = 1;
    else
        info.fp64 = 0;
    //printf("FP64 ENABLE: %d %d\n", info.fp64, fp_config  );
#undef DEBUGCL
    return info; 
}


void rebuildProgram(char * filename) {
    int status;
    char * source  = filetobuf( filename );
    int w;
    for(w=0; w<nkernelsRmat; w++ ) {
        int nlen = strlen(source) + strlen(strkernelsRmat[w]) + 10;
        char * tmp = (char *) malloc( sizeof(char) * nlen );
        strcpy( tmp, source );    
        strcat( tmp, "\n" );
        strcat( tmp, strkernelsRmat[w] );
        free( source );
        source = tmp;
    }
    
    cl_program program = clCreateProgramWithSource (clinfo.c, 1, (const char **) &source, NULL, &status);
    CLERR
    const char options[] = "";
    status = clBuildProgram (program, 1, &clinfo.d, options, NULL, NULL);
    if (status == CL_BUILD_PROGRAM_FAILURE) {
        char buildLog[1024];
        status = clGetProgramBuildInfo (program, clinfo.d, CL_PROGRAM_BUILD_LOG, 1024, buildLog, NULL);
        fprintf (stderr, "Program Build Log:\n%s\n", buildLog);
        
        CLERR
        status = CL_BUILD_PROGRAM_FAILURE;
    }
    clinfo.p = program;
    free( source );
}

void ReleaseCLInfo( cl_info info ) {
    cl_int status;
    status = clReleaseProgram (info.p); 
    CLERR
    //status = clUnloadCompiler ();
    //CLERR
    status = clReleaseCommandQueue (info.q);
    CLERR
    status = clReleaseContext (info.c);
    CLERR
}


void InitCLEngine(int device) {
    cl_int status;
    proctype = CL_DEVICE_TYPE_ALL;
    id_device = device;
    if( nkernelsRmat == 0 )
        clinfo = GetCLInfo("/usr/local/include/vector.cl");
    else
        rebuildProgram("/usr/local/include/vector.cl");    
    int ii;
    for(ii=0;ii<32;ii++)
        rmatstat[ii] = 0;

    clkernels[0] = clCreateKernel (clinfo.p, "addVector", &status);
    CLERR
    clkernels[1] = clCreateKernel (clinfo.p, "subVector", &status);   
    CLERR    
    clkernels[2] = clCreateKernel (clinfo.p, "mulScalarVector", &status);
    CLERR   
    clkernels[3] = clCreateKernel (clinfo.p, "matVecMul3", &status);
    CLERR 
    clkernels[4] = clCreateKernel (clinfo.p, "matTranspose", &status);
    CLERR
    clkernels[5] = clCreateKernel (clinfo.p, "sumVector", &status);
    CLERR 
  clkernels[6] = clCreateKernel (clinfo.p, "matMul1", &status);
    CLERR
    clkernels[7] = clCreateKernel (clinfo.p, "normVector", &status);
    CLERR 
    clkernels[8] = clCreateKernel (clinfo.p, "dotVector", &status);
    CLERR
    clkernels[9] = clCreateKernel (clinfo.p, "matMul2", &status);
    CLERR
    clkernels[10] = clCreateKernel (clinfo.p, "matVecMul3Complex", &status);
    CLERR
    clkernels[11] = clCreateKernel (clinfo.p, "sparseMatMulVec", &status);
    CLERR
    clkernels[12] = clCreateKernel (clinfo.p, "sparseComplexMatMulVec", &status);
    CLERR
    clkernels[13] = clCreateKernel (clinfo.p, "luDecomp", &status);
    CLERR
    clkernels[14] = clCreateKernel (clinfo.p, "prodVector", &status);
    CLERR
    clkernels[15] = clCreateKernel (clinfo.p, "prodComplexVector", &status);
    CLERR
    clkernels[16] = clCreateKernel (clinfo.p, "vecConj", &status);
    CLERR
    clkernels[17] = clCreateKernel (clinfo.p, "vec_add_off2", &status);
    CLERR
    clkernels[18] = clCreateKernel (clinfo.p, "vec_add_off", &status);
    CLERR
    clkernels[19] = clCreateKernel (clinfo.p, "sparseMatMatMul", &status);
    CLERR
    clkernels[20] = clCreateKernel (clinfo.p, "dotVectorComplex", &status);
    CLERR
    clkernels[21] = clCreateKernel (clinfo.p, "vecConjugate", &status);
    CLERR
    clkernels[22] = clCreateKernel (clinfo.p, "matMulComplex", &status);
    CLERR
    clkernels[23] = clCreateKernel (clinfo.p, "matMulFloatComplex", &status);
    CLERR
    clkernels[24] = clCreateKernel (clinfo.p, "addVectorComplex", &status);
    CLERR
    clkernels[25] = clCreateKernel (clinfo.p, "mulScalarMatRow", &status);
    CLERR
    clkernels[26] = clCreateKernel (clinfo.p, "mulScalarMatCol", &status);
    CLERR
    clkernels[KER_MUL_COMPLEX_SCALAR_FLOAT_VET] = clCreateKernel (clinfo.p, "mulComplexScalarFloatVet", &status);
    CLERR
    clkernels[KER_MUL_COMPLEX_SCALAR_COMPLEX_VET] = clCreateKernel (clinfo.p, "mulComplexScalarComplexVet", &status);
    CLERR
    clkernels[KER_MUL_FLOAT_SCALAR_COMPLEX_VET] = clCreateKernel (clinfo.p, "mulFloatScalarComplexVet", &status);
    CLERR
    clkernels[KER_MAT_ADD_FLOAT_COMPLEX] = clCreateKernel (clinfo.p, "addVectorFloatComplex", &status);
    CLERR
    clkernels[KER_VET_SUB_COMPLEX] = clCreateKernel (clinfo.p, "subVectorComplex", &status);
    CLERR
    
}

 
void showDevicesList() {
    
//    int i, j;
//    char* value;
//    size_t valueSize;
//    cl_uint platformCount;
//    cl_platform_id* platforms;
//    cl_uint deviceCount;
//    cl_device_id* devices;
//    cl_uint maxComputeUnits;

    cl_int status;
    cl_uint numPlatforms;
    cl_uint num_compute_units;
    int k = 0;    
    status = clGetPlatformIDs (0, NULL, &numPlatforms);
    //printf("%u",numPlatforms);
    if (status == CL_SUCCESS) {
        //printf("Success. Platforms available: %u", numPlatforms);
                
    } else {
        //printf("Error. Platforms available: %u", numPlatforms);
    }
//    platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
//    clGetPlatformIDs(platformCount, platforms, NULL);
//    printf(NEB_PRESENT);
//    if(  platformCount > 0 )
//        printf("Available OpenCL devices\n");
//    else
//        printf("No OpenCL devices found\n");
//     
//     int id = 0;
//    for (i = 0; i < platformCount; i++) {
//        char buffer1[1024];
//        char buffer2[1024];
//	    //clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, 10240, buffer, NULL);
//		//printf("  PROFILE = %s\n", buffer);
//		//clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 10240, buffer, NULL);
//		//printf("  VERSION = %s\n", buffer);
//		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 1024, buffer1, NULL);
//        clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 1024, buffer2, NULL);
//				
//        printf("Platform %d: %s - %s\n", i, buffer1, buffer2);
//		
//        // get all devices
//        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
//        devices = (cl_device_id*) malloc(sizeof(cl_device_id) * deviceCount);
//        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);
//
//        // for each device print critical attributes
//        for (j = 0; j < deviceCount; j++) {
//
//            // print device name
//            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, NULL, &valueSize);
//            value = (char*) malloc(valueSize);
//            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, valueSize, value, NULL);
//            printf("\tid: %d\n", id++);
//            printf("\t     Device: %s\n",value);
//            free(value);
//
//            // print hardware device version
//            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, 0, NULL, &valueSize);
//            value = (char*) malloc(valueSize);
//            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, valueSize, value, NULL);
//            printf("\t     Hardware version: %s\n", value);
//            free(value);
//
//            // print software driver version
//            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, 0, NULL, &valueSize);
//            value = (char*) malloc(valueSize);
//            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, valueSize, value, NULL);
//            printf("\t     Software version: %s\n", value);
//            free(value);
//
//            // print c version supported by compiler for device
//            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &valueSize);
//            value = (char*) malloc(valueSize);
//            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, valueSize, value, NULL);
//            printf("\t     OpenCL C version: %s\n", value);
//            free(value);
//
//            // print parallel compute units
//            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS,
//                    sizeof(maxComputeUnits), &maxComputeUnits, NULL);
//            printf("\t     Parallel compute units: %d\n",  maxComputeUnits);
//
//
//             // print parallel compute units
//            cl_uint dim;
//            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
//                    sizeof(dim), &dim, NULL);
//            size_t v[10];// = (size_t * ) malloc( sizeof(size_t)*dim );
//            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_SIZES,
//                    sizeof(v), &v, NULL);            
//            
//            int s;
//            printf("\t     Max. work item sizes: (");
//            for(s=0;s<dim;s++) {
//                if( s != dim - 1 )
//                    printf( "%d, ", (int) v[s] );
//                else 
//                    printf("%d)\n", (int) v[s] );
//            }            
//            //free( v );
//
//            
//
//        // print parallel compute units
//            size_t size_wg;
//            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE,
//                    sizeof(size_wg), &size_wg, NULL);
//            printf("\t     Max. work group size: %d\n",  (int)size_wg);
//            // print parallel compute units
//            cl_ulong mem;
//            clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE,
//                    sizeof(mem), &mem, NULL);
//            printf("\t     Global memory: %ldM\n\n",  mem/(1024*1024));            
//            
//        }
//
//        free(devices);
//
//    }
//
//    free(platforms);

}

