#include "gtest/gtest.h"
#include "libneblina.h"
#include "clutils.h"
#include "oclvector.h"
#include "neblina_std.h"
//#include "neblina_vector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>

using namespace std;

class MatrixFixture : public ::testing::Test {
protected:
  cl_int status;
public:

    MatrixFixture() {
        // initialization code here
    }

    protected:
    static void SetUpTestSuite() {
        std::cerr << "Matrix TestSuiteSetup" << std::endl;
        cl_int err;
        cl_uint num_platforms;
        
        err = clGetPlatformIDs(0, NULL, &num_platforms);
        if (err == CL_SUCCESS) {
            //std::cout << "Success. Platforms available: " << num_platforms
            //        << std::endl;
        } else {
            //std::cout << "Error. Platforms available: " << num_platforms
            //        << std::endl;
        }

        InitCLEngine(0);
    }

    static void TearDownTestSuite() {
        ReleaseCLInfo(clinfo);
    }
    
    void SetUp() {
        
    }

    void TearDown() {
        //printf("check");
        //getchar();
        
    }

    ~MatrixFixture() {
        // cleanup any pending stuff, but no exceptions allowed
    }

};

TEST_F(MatrixFixture, mat_add) {

    int n = 3;

    matrix_t * a = matrix_new(n, n, T_FLOAT);
    matrix_t * b = matrix_new(n, n, T_FLOAT);

    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            a->value.f[i * a->ncol + j] = 3.;
            b->value.f[i * b->ncol + j] = 3.;
        }
    }

    object_t ** in = convertMatMatToObject(a, b);
    
    matrix_t * r = (matrix_t *) mat_add((void **) in, NULL);
    
    matreqhost(r);

    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            EXPECT_EQ(6., r->value.f[i * r->ncol + j]);
        }
    }
    matrix_delete(a);
    matrix_delete(b);
    matrix_delete(r);


}

TEST_F(MatrixFixture, mat_get_set_float) {

    int n = 3;

    matrix_t * a = matrix_new(n, n, T_FLOAT);

    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            matrix_set_real_value(a, i, j, (i + j) * 1.0);
        }
    }


    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            EXPECT_EQ( (i + j) * 1.0, matrix_get_real_value(a, i, j));
        }
    }
    matrix_delete(a);

}

TEST_F(MatrixFixture, mat_get_set_complex) {

    int n = 3;

    matrix_t * a = matrix_new(n, n, T_COMPLEX);

    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            matrix_set_complex_value(a, i, j, i * 1.0, j * 1.0);
        }
    }


    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            EXPECT_EQ( i * 1.0, matrix_get_complex_real_value(a, i, j));
            EXPECT_EQ( j * 1.0, matrix_get_complex_imaginary_value(a, i, j));
        }
    }
    matrix_delete(a);

}

TEST_F(MatrixFixture, mat_add_complex) {

    int n = 3;

    matrix_t * a = matrix_new(n, n, T_COMPLEX);
    matrix_t * b = matrix_new(n, n, T_COMPLEX);

    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            int idx = 2 * (i * b->ncol + j);
            a->value.f[idx] = 3.;
            a->value.f[idx + 1] = 3.;
            b->value.f[idx] = 3.;
            b->value.f[idx + 1] = 3.;
        }
    }

    object_t ** in = convertMatMatToObject(a, b);
    
    matrix_t * r = (matrix_t *) mat_add((void **) in, NULL);
    
    matreqhost(r);

    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            int idx = 2 * (i * b->ncol + j);
            EXPECT_EQ(6., r->value.f[idx]);
            EXPECT_EQ(6., r->value.f[idx + 1]);
        }
    }
    
    matrix_delete(a);
    matrix_delete(b);
    matrix_delete(r);

    
}

TEST_F(MatrixFixture, mat_add_complex_float) {

    int n = 3;

    matrix_t * a = matrix_new(n, n, T_COMPLEX);
    matrix_t * b = matrix_new(n, n, T_FLOAT);

    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            int idx = 2 * (i * b->ncol + j);
            a->value.f[idx] = 3.;
            a->value.f[idx + 1] = 3.;
            b->value.f[i * b->ncol + j] = 2.;
        }
    }

    object_t ** in = convertMatMatToObject(a, b);
    
    matrix_t * r = (matrix_t *) mat_add((void **) in, NULL);
    
    matreqhost(r);

    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            int idx = 2 * (i * r->ncol + j);
            EXPECT_EQ(5., r->value.f[idx]);
            EXPECT_EQ(3., r->value.f[idx + 1]);
        }
    }
    
    matrix_delete(a);
    matrix_delete(b);
    matrix_delete(r);


}

TEST_F(MatrixFixture, mat_mul) {

    int n = 3;

    matrix_t * a = matrix_new(n, n, T_FLOAT);
    matrix_t * b = matrix_new(n, n, T_FLOAT);

    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            a->value.f[i * a->ncol + j] = 3.;
            b->value.f[i * b->ncol + j] = 3.;
        }
    }

    object_t ** in = convertMatMatToObject(a, b);
    
    matrix_t * r = (matrix_t *) mat_mul((void **) in, NULL);
    
    matreqhost(r);

    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            EXPECT_EQ(27., r->value.f[i * r->ncol + j]);
        }
    }
    matrix_delete(a);
    matrix_delete(b);
    matrix_delete(r);


}

TEST_F(MatrixFixture, mat_mul_withComplex) {

    int n = 2;

    matrix_t * a = matrix_new(n, n, T_COMPLEX);
    matrix_t * b = matrix_new(n, n, T_COMPLEX);

    matrix_set_complex_value(a, 0, 0, 17., 1.0);
    matrix_set_complex_value(b, 0, 0, 60., 0.0);

    matrix_set_complex_value(a, 0, 1, -3., 0.0);
    matrix_set_complex_value(b, 0, 1, -4., 1.0);
    
    matrix_set_complex_value(a, 1, 0, -7., 1.0);
    matrix_set_complex_value(b, 1, 0, -12., 0.0);
    
    matrix_set_complex_value(a, 1, 1, 1., 0.0);
    matrix_set_complex_value(b, 1, 1, 0., 1.0);
    
    
    /*matrix_t * r = matrix_new(n, n, T_COMPLEX);
    
    for(int i=0; i < a->nrow; i++) {
        for (int j=0; j < a->ncol; j++) {
            double sumre = 0;
            double sumim = 0;
            for (int k=0; k < a->ncol; k++) {
                double re1 = matrix_get_complex_real_value(a,i,k);
                double im1 = matrix_get_complex_imaginary_value(a,i,k);
                
                double re2 = matrix_get_complex_real_value(b,k,j);
                double im2 = matrix_get_complex_imaginary_value(b,k,j);
                
                sumre += re1*re2 - im1*im2;
                sumim += re1*im2 + re2*im1;
            }
            matrix_set_complex_value(r, i, j, sumre, sumim);
        }
    }*/
    
    object_t ** in = convertMatMatToObject(a, b);
    
    matrix_t * r = (matrix_t *) mat_mul((void **) in, NULL);
    
    matreqhost(r);
    
//    [[1056.+60.j  -69.+10.j]
// [-432.+60.j   27.-10.j]]

    EXPECT_EQ(1056., matrix_get_complex_real_value(r,0,0));
    EXPECT_EQ(60., matrix_get_complex_imaginary_value(r,0,0));

    EXPECT_EQ(-69., matrix_get_complex_real_value(r,0,1));
    EXPECT_EQ(10., matrix_get_complex_imaginary_value(r,0,1));

    EXPECT_EQ(-432., matrix_get_complex_real_value(r,1,0));
    EXPECT_EQ(60., matrix_get_complex_imaginary_value(r,1,0));

    EXPECT_EQ(27., matrix_get_complex_real_value(r,1,1));
    EXPECT_EQ(-10., matrix_get_complex_imaginary_value(r,1,1));

    matrix_delete(a);
    matrix_delete(b);
    matrix_delete(r);


}

TEST_F(MatrixFixture, scalar_mat_float) {

    int n = 3;
    
    double scalar = 2.0;
    matrix_t * a = matrix_new(n, n, T_FLOAT);

    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            a->value.f[i * a->ncol + j] = 3.;
        }
    }

    object_t ** in = convertScaMatToObject(scalar, a);
    
    matrix_t * r = (matrix_t *) mat_mulsc((void **) in, NULL);
    
    matreqhost(r);
    
    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            EXPECT_EQ(6., r->value.f[i * r->ncol + j]);
        }
    }

    matrix_delete(a);
    matrix_delete(r);
}

TEST_F(MatrixFixture, scalar_mat_complex) {

    int n = 3;
    
    double scalar = 2.0;
    matrix_t * a = matrix_new(n, n, T_COMPLEX);

    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            int idx = 2 * (i * a->ncol + j);
            a->value.f[idx] = 3.;
            a->value.f[idx + 1] = 3.;
        }
    }

    object_t ** in = convertScaMatToObject(scalar, a);
    
    matrix_t * r = (matrix_t *) mat_mulsc((void **) in, NULL);
    
    matreqhost(r);
    
    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            int idx = 2 * (i * r->ncol + j);
            EXPECT_EQ(6., r->value.f[idx]);
            EXPECT_EQ(6., r->value.f[idx + 1]);
        }
    }

    matrix_delete(a);
    matrix_delete(r);
}

TEST_F(MatrixFixture, complex_scalar_complex_mat) {

    int n = 3;
    
    complex_t * scalar = complex_new(2.0, 2.0);
    matrix_t * a = matrix_new(n, n, T_COMPLEX);

    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            int idx = 2 * (i * a->ncol + j);
            a->value.f[idx] = 3.;
            a->value.f[idx + 1] = 3.;
        }
    }
    
    matrix_t * r = (matrix_t *) mul_complex_scalar_complex_mat(scalar, a);
    
    matreqhost(r);
    
    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            int idx = 2 * (i * r->ncol + j);
            EXPECT_EQ(6., r->value.f[idx]);
            EXPECT_EQ(6., r->value.f[idx + 1]);
        }
    }

    matrix_delete(a);
    matrix_delete(r);

    complex_delete(scalar);
}

TEST_F(MatrixFixture, complex_scalar_float_mat) {

    int n = 3;
    
    complex_t * scalar = complex_new(2.0, 2.0);
    matrix_t * a = matrix_new(n, n, T_FLOAT);

    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            int idx = (i * a->ncol + j);
            a->value.f[idx] = 3.;

        }
    }
    
    matrix_t * r = (matrix_t *) mul_complex_scalar_float_mat(scalar, a);
    
    matreqhost(r);
    
    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            int idx = 2 * (i * r->ncol + j);
            EXPECT_EQ(6., r->value.f[idx]);
            EXPECT_EQ(2., r->value.f[idx + 1]);
        }
    }

    matrix_delete(a);
    matrix_delete(r);

    complex_delete(scalar);
}

TEST_F(MatrixFixture, matvec_mul3_WithFloat) {

    int n = 3;

    vector_t * a = vector_new(n, T_FLOAT);
    matrix_t * b = matrix_new(n, n, T_FLOAT);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }

    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            b->value.f[i * b->ncol + j] = 3.;
        }
    }

    object_t ** in = convertToObject3(a, b);

    r = (vector_t *) matvec_mul3((void **) in, NULL);

    vecreqhost(r);

    for (int i = 0; i < r->len; i++) {
        EXPECT_EQ(18., r->value.f[i]);
    }
    vector_delete(a);
    matrix_delete(b);
    vector_delete(r);

}

TEST_F(MatrixFixture, matvec_mul3_WithComplex) {

    int n = 7000;

    vector_t * a = vector_new(n, T_COMPLEX);
    matrix_t * b = matrix_new(n, n, T_COMPLEX);
    vector_t * r;

    for (int i = 0; i < 2 * a->len; i += 2) {
        a->value.f[i] = 2.;
        a->value.f[i + 1] = 2.;
    }

    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            int idx = 2 * (i * b->ncol + j);
            b->value.f[idx] = 3.;
            b->value.f[idx + 1] = 3.;
        }
    }

    struct timeval stop, start;
    gettimeofday(&start, NULL);
    object_t ** in = convertToObject3(a, b);
    r = (vector_t *) matvec_mul3((void **) in, NULL);
    gettimeofday(&stop, NULL);
    //printf("\n\ntook %lu us\n\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 

    vecreqhost(r);

    //for (int i = 0; i < 2 * r->len; i += 2) {
        //EXPECT_EQ(0., r->value.f[i]);
        //EXPECT_EQ(36., r->value.f[i + 1]);
    //}
    vector_delete(a);
    matrix_delete(b);
    vector_delete(r);

}

TEST_F(MatrixFixture, matvec_mul3_WithComplex_reusingResult) {

    int n = 15000;

    vector_t * a = vector_new(n, T_COMPLEX);
    matrix_t * b = matrix_new(n, n, T_COMPLEX);
    vector_t * r;

    for (int i = 0; i < 2 * a->len; i += 2) {
        a->value.f[i] = 2.;
        a->value.f[i + 1] = 2.;
    }

    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            int idx = 2 * (i * b->ncol + j);
            b->value.f[idx] = 3.;
            b->value.f[idx + 1] = 3.;
        }
    }

    object_t ** in = convertToObject3(a, b);
    r = (vector_t *) matvec_mul3((void **) in, NULL);
    for (int i=0; i < 100; i++) {
        object_t ** in = convertToObject3(r, b);
        vector_t * tmp = (vector_t *) matvec_mul3((void **) in, NULL);
        vector_delete(r);
        r = tmp;
        
    }

    vecreqhost(r);
    
    //for (int i = 0; i < 2 * r->len; i += 2) {
        //EXPECT_EQ(0., r->value.f[i]);
        //EXPECT_EQ(36., r->value.f[i + 1]);
    //}
    vector_delete(r);
    vector_delete(a);
    matrix_delete(b);
    
}

TEST_F(MatrixFixture, large_matvec_mul3_WithComplex) {

    int n = 1024;

    vector_t * a = vector_new(n, T_COMPLEX);
    matrix_t * b = matrix_new(n, n, T_COMPLEX);
    vector_t * r;
    vector_t * truth = vector_new(n, T_COMPLEX);

    int i = 0; 
    string line;
    ifstream myfile("initial_vector.txt");
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            istringstream iss(line);
            string s;
            int count = 0;
            while (getline(iss, s, ' ')) {
                //s.c_str()
                stringstream ss(s);
                double d = 0;
                ss >> d;
                a->value.f[i + count] = d;
                count++;
            }
            i += 2;
        }
        myfile.close();
    }

    i = 0; 
    ifstream truth_file("final_vector.txt");
    if (truth_file.is_open()) {
        while (getline(truth_file, line)) {
            istringstream iss(line);
            string s;
            int count = 0;
            while (getline(iss, s, ' ')) {
                //s.c_str()
                stringstream ss(s);
                double d = 0;
                ss >> d;
                truth->value.f[i + count] = d;
                count++;
            }
            i += 2;
        }
        truth_file.close();
    }
    i = 0; 
    ifstream matrix_file("unitary.txt");
    if (matrix_file.is_open()) {
        while (getline(matrix_file, line)) {
            istringstream iss(line);
            string s;
            int count = 0;
            int j = 0;
            while (getline(iss, s, ' ')) {
                //
                
                stringstream ss(s);
                double d = 0;
                //0.000000000000000000
                ss >> d;
                int idx = 2 * (i * b->ncol + j);
                b->value.f[idx + count] = d;
                count++;
                if (count == 2) {
                    count = 0;
                    j++;
                }
            }
            i++;
        }
        matrix_file.close();
    }

    object_t ** in = convertToObject3(a, b);

    r = (vector_t *) matvec_mul3((void **) in, NULL);
    
    
    vecreqhost(r);
    for (int i = 0; i < 2 * a->len; i += 2) {
        //printf("%d %4.18e %4.18e\n",i,truth->value.f[i],out->value.f[i]);
        //printf("%d %4.18e %4.18e\n",i,truth->value.f[i+1],out->value.f[i+1]);
        //EXPECT_EQ(truth->value.f[i], out->value.f[i]);
        //EXPECT_EQ(truth->value.f[i + 1], out->value.f[i + 1]);
    }
    vector_delete(a);
    matrix_delete(b);
    vector_delete(r);

}
