#include "gtest/gtest.h"
#include "libneblina.h"
#include "bridge_api.h"
#include "neblina_std.h"
//#include "neblina_vector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>

using namespace std;

class MatrixFixture : public ::testing::Test {
protected:
public:
    bridge_manager_t m;
    int idx;

    MatrixFixture() {
        idx = 0;
        string plugin_name = "/usr/local/lib64/libneblina-cpu-bridge.so";
        load_plugin(&m, const_cast<char *>(plugin_name.c_str()), idx);
        m.bridges[idx].InitEngine_f(0);
    }

    protected:
    static void SetUpTestSuite() {
        std::cerr << "Matrix TestSuiteSetup" << std::endl;
    }

    static void TearDownTestSuite() {
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

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);
    matrix_t * b = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            a->value.f[i * a->ncol + j] = 3.;
            b->value.f[i * b->ncol + j] = 3.;
        }
    }

    object_t ** in = convertMatMatToObject(a, b);
    
    matrix_t * r = (matrix_t *) mat_add(&m, idx, (void **) in, NULL);
    
    m.bridges[idx].matreqhost(r);

    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            EXPECT_EQ(6., r->value.f[i * r->ncol + j]);
        }
    }
    delete_object_array(in, 2);
    m.bridges[idx].matrix_delete(a);
    m.bridges[idx].matrix_delete(b);
    m.bridges[idx].matrix_delete(r);


}

TEST_F(MatrixFixture, mat_get_set_float) {

    int n = 3;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            m.bridges[idx].matrix_set_real_value(a, i, j, (i + j) * 1.0);
        }
    }


    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            EXPECT_EQ( (i + j) * 1.0, m.bridges[idx].matrix_get_real_value(a, i, j));
        }
    }
    m.bridges[idx].matrix_delete(a);

}

TEST_F(MatrixFixture, mat_get_set_complex) {

    int n = 3;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_COMPLEX, 1, NULL);

    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            m.bridges[idx].matrix_set_complex_value(a, i, j, i * 1.0, j * 1.0);
        }
    }


    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            EXPECT_EQ( i * 1.0, m.bridges[idx].matrix_get_complex_real_value(a, i, j));
            EXPECT_EQ( j * 1.0, m.bridges[idx].matrix_get_complex_imaginary_value(a, i, j));
        }
    }
    m.bridges[idx].matrix_delete(a);

}

TEST_F(MatrixFixture, mat_add_complex) {

    int n = 3;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_COMPLEX, 1, NULL);
    matrix_t * b = m.bridges[idx].matrix_new(n, n, T_COMPLEX, 1, NULL);

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
    
    matrix_t * r = (matrix_t *) mat_add(&m, idx, (void **) in, NULL);
    
    m.bridges[idx].matreqhost(r);

    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            int idx = 2 * (i * b->ncol + j);
            EXPECT_EQ(6., r->value.f[idx]);
            EXPECT_EQ(6., r->value.f[idx + 1]);
        }
    }
    delete_object_array(in, 2);
    m.bridges[idx].matrix_delete(a);
    m.bridges[idx].matrix_delete(b);
    m.bridges[idx].matrix_delete(r);

    
}

TEST_F(MatrixFixture, mat_add_complex_float) {

    int n = 3;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_COMPLEX, 1, NULL);
    matrix_t * b = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            int idx = 2 * (i * b->ncol + j);
            a->value.f[idx] = 3.;
            a->value.f[idx + 1] = 3.;
            b->value.f[i * b->ncol + j] = 2.;
        }
    }

    object_t ** in = convertMatMatToObject(a, b);
    
    matrix_t * r = (matrix_t *) mat_add(&m, idx, (void **) in, NULL);
    
    m.bridges[idx].matreqhost(r);

    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            int idx = 2 * (i * r->ncol + j);
            EXPECT_EQ(5., r->value.f[idx]);
            EXPECT_EQ(3., r->value.f[idx + 1]);
        }
    }
    delete_object_array(in, 2);
    m.bridges[idx].matrix_delete(a);
    m.bridges[idx].matrix_delete(b);
    m.bridges[idx].matrix_delete(r);


}

TEST_F(MatrixFixture, mat_mul) {

    int n = 3;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);
    matrix_t * b = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            a->value.f[i * a->ncol + j] = 3.;
            b->value.f[i * b->ncol + j] = 3.;
        }
    }

    object_t ** in = convertMatMatToObject(a, b);
    
    matrix_t * r = (matrix_t *) mat_mul(&m, idx, (void **) in, NULL);
    
    m.bridges[idx].matreqhost(r);

    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            EXPECT_EQ(27., r->value.f[i * r->ncol + j]);
        }
    }
    delete_object_array(in, 2);
    m.bridges[idx].matrix_delete(a);
    m.bridges[idx].matrix_delete(b);
    m.bridges[idx].matrix_delete(r);


}

TEST_F(MatrixFixture, mat_mul_withComplex) {

    int n = 2;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_COMPLEX, 1, NULL);
    matrix_t * b = m.bridges[idx].matrix_new(n, n, T_COMPLEX, 1, NULL);

    m.bridges[idx].matrix_set_complex_value(a, 0, 0, 17., 1.0);
    m.bridges[idx].matrix_set_complex_value(b, 0, 0, 60., 0.0);

    m.bridges[idx].matrix_set_complex_value(a, 0, 1, -3., 0.0);
    m.bridges[idx].matrix_set_complex_value(b, 0, 1, -4., 1.0);
    
    m.bridges[idx].matrix_set_complex_value(a, 1, 0, -7., 1.0);
    m.bridges[idx].matrix_set_complex_value(b, 1, 0, -12., 0.0);
    
    m.bridges[idx].matrix_set_complex_value(a, 1, 1, 1., 0.0);
    m.bridges[idx].matrix_set_complex_value(b, 1, 1, 0., 1.0);
    
    
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
    
    matrix_t * r = (matrix_t *) mat_mul(&m, idx, (void **) in, NULL);
    
    m.bridges[idx].matreqhost(r);
    
//    [[1056.+60.j  -69.+10.j]
// [-432.+60.j   27.-10.j]]

    EXPECT_EQ(1056., m.bridges[idx].matrix_get_complex_real_value(r,0,0));
    EXPECT_EQ(60., m.bridges[idx].matrix_get_complex_imaginary_value(r,0,0));

    EXPECT_EQ(-69., m.bridges[idx].matrix_get_complex_real_value(r,0,1));
    EXPECT_EQ(10., m.bridges[idx].matrix_get_complex_imaginary_value(r,0,1));

    EXPECT_EQ(-432., m.bridges[idx].matrix_get_complex_real_value(r,1,0));
    EXPECT_EQ(60., m.bridges[idx].matrix_get_complex_imaginary_value(r,1,0));

    EXPECT_EQ(27., m.bridges[idx].matrix_get_complex_real_value(r,1,1));
    EXPECT_EQ(-10., m.bridges[idx].matrix_get_complex_imaginary_value(r,1,1));

    delete_object_array(in, 2);
    m.bridges[idx].matrix_delete(a);
    m.bridges[idx].matrix_delete(b);
    m.bridges[idx].matrix_delete(r);


}

TEST_F(MatrixFixture, scalar_mat_float) {

    int n = 3;
    
    double scalar = 2.0;
    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            a->value.f[i * a->ncol + j] = 3.;
        }
    }

    object_t ** in = convertScaMatToObject(scalar, a);
    
    matrix_t * r = (matrix_t *) mat_mulsc(&m, idx, (void **) in, NULL);
    
    m.bridges[idx].matreqhost(r);
    
    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            EXPECT_EQ(6., r->value.f[i * r->ncol + j]);
        }
    }

    delete_object_array(in, 2);
    m.bridges[idx].matrix_delete(a);
    m.bridges[idx].matrix_delete(r);
}

TEST_F(MatrixFixture, scalar_mat_complex) {

    int n = 3;
    
    double scalar = 2.0;
    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_COMPLEX, 1, NULL);

    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            int idx = 2 * (i * a->ncol + j);
            a->value.f[idx] = 3.;
            a->value.f[idx + 1] = 3.;
        }
    }

    object_t ** in = convertScaMatToObject(scalar, a);
    
    matrix_t * r = (matrix_t *) mat_mulsc(&m, idx, (void **) in, NULL);
    
    m.bridges[idx].matreqhost(r);
    
    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            int idx = 2 * (i * r->ncol + j);
            EXPECT_EQ(6., r->value.f[idx]);
            EXPECT_EQ(6., r->value.f[idx + 1]);
        }
    }

    delete_object_array(in, 2);
    m.bridges[idx].matrix_delete(a);
    m.bridges[idx].matrix_delete(r);
}

TEST_F(MatrixFixture, complex_scalar_complex_mat) {

    int n = 3;
    
    complex_t * scalar = m.bridges[idx].complex_new(2.0, 2.0);
    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_COMPLEX, 1, NULL);

    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            int idx = 2 * (i * a->ncol + j);
            a->value.f[idx] = 3.;
            a->value.f[idx + 1] = 3.;
        }
    }
    
    matrix_t * r = (matrix_t *) mul_complex_scalar_complex_mat(&m, idx, scalar, a);
    
    m.bridges[idx].matreqhost(r);
    
    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            int idx = 2 * (i * r->ncol + j);
            EXPECT_EQ(6., r->value.f[idx]);
            EXPECT_EQ(6., r->value.f[idx + 1]);
        }
    }

    m.bridges[idx].matrix_delete(a);
    m.bridges[idx].matrix_delete(r);

    m.bridges[idx].complex_delete(scalar);
}

TEST_F(MatrixFixture, complex_scalar_float_mat) {

    int n = 3;
    
    complex_t * scalar = m.bridges[idx].complex_new(2.0, 2.0);
    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            int idx = (i * a->ncol + j);
            a->value.f[idx] = 3.;

        }
    }
    
    matrix_t * r = (matrix_t *) mul_complex_scalar_float_mat(&m, idx, scalar, a);
    
    m.bridges[idx].matreqhost(r);
    
    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            int idx = 2 * (i * r->ncol + j);
            EXPECT_EQ(6., r->value.f[idx]);
            EXPECT_EQ(2., r->value.f[idx + 1]);
        }
    }

    m.bridges[idx].matrix_delete(a);
    m.bridges[idx].matrix_delete(r);

    m.bridges[idx].complex_delete(scalar);
}

TEST_F(MatrixFixture, matvec_mul3_WithFloat) {

    int n = 3;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1, NULL );
    matrix_t * b = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);
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

    r = (vector_t *) matvec_mul3(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

    for (int i = 0; i < r->len; i++) {
        EXPECT_EQ(18., r->value.f[i]);
    }
    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].matrix_delete(b);
    m.bridges[idx].vector_delete(r);

}

TEST_F(MatrixFixture, matvec_mul3_WithComplex) {

    int n = 700;

//    printf("matvec_mul3_WithComplex\n");
    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1, NULL );
    // printf("aqui teste vector\n");
    matrix_t * b = m.bridges[idx].matrix_new(n, n, T_COMPLEX, 1, NULL);
//    printf("aqui teste matrix\n");
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
    r = (vector_t *) matvec_mul3(&m, idx, (void **) in, NULL);
    // printf("aqui teste voltou\n");
    gettimeofday(&stop, NULL);
    //printf("\n\ntook %lu us\n\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 

    m.bridges[idx].vecreqhost(r);
    // printf("aqui teste moveu\n");
    for (int i = 0; i < 2 * r->len; i += 2) {
        // printf("i=%d\n",i);
        EXPECT_EQ(0., r->value.f[i]);
        EXPECT_EQ(8400., r->value.f[i + 1]);
    }
    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    // printf("delete a\n");
    m.bridges[idx].matrix_delete(b);
    // printf("delete b\n");
    m.bridges[idx].vector_delete(r);
    // printf("delete r\n");

}

TEST_F(MatrixFixture, matvec_mul3_WithComplex_reusingResult) {


    int n = 15;

    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1, NULL );
    matrix_t * b = m.bridges[idx].matrix_new(n, n, T_COMPLEX, 1, NULL);
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
    r = (vector_t *) matvec_mul3(&m, idx, (void **) in, NULL);
    delete_object_array(in, 2);
    for (int i=0; i < 100; i++) {
        object_t ** in = convertToObject3(r, b);
        vector_t * tmp = (vector_t *) matvec_mul3(&m, idx, (void **) in, NULL);
        delete_object_array(in, 2);
        m.bridges[idx].vector_delete(r);
        r = tmp;
        
    }

    m.bridges[idx].vecreqhost(r);
    
    //for (int i = 0; i < 2 * r->len; i += 2) {
        //EXPECT_EQ(0., r->value.f[i]);
        //EXPECT_EQ(36., r->value.f[i + 1]);
    //}
    m.bridges[idx].vector_delete(r);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].matrix_delete(b);
    
}

TEST_F(MatrixFixture, large_matvec_mul3_WithComplex) {


    int n = 1024;

    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1, NULL );
    matrix_t * b = m.bridges[idx].matrix_new(n, n, T_COMPLEX, 1, NULL);
    vector_t * r;
    vector_t * truth = m.bridges[idx].vector_new(n, T_COMPLEX, 1, NULL );

    printf("primeiro arquivo\n");
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

    printf("segundo arquivo\n");
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

    printf("terceiro arquivo\n");
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
    printf("matvec_mul3\n");
    r = (vector_t *) matvec_mul3(&m, idx, (void **) in, NULL);
    printf("matvec_mul3back\n");
    
    
    m.bridges[idx].vecreqhost(r);
    for (int i = 0; i < 2 * a->len; i += 2) {
        //printf("%d %4.18e %4.18e\n",i,truth->value.f[i],out->value.f[i]);
        //printf("%d %4.18e %4.18e\n",i,truth->value.f[i+1],out->value.f[i+1]);
        //EXPECT_EQ(truth->value.f[i], out->value.f[i]);
        //EXPECT_EQ(truth->value.f[i + 1], out->value.f[i + 1]);
    }
    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].matrix_delete(b);
    m.bridges[idx].vector_delete(r);
    m.bridges[idx].vector_delete(truth);

}

TEST_F(MatrixFixture, matrix_copy_col) {

    int n = 3;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    int value = 1;
    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            a->value.f[i * a->ncol + j] = value;
            value++;
        }
    }

    double * col = m.bridges[idx].matrix_copy_col(a, 1, 0, n);

    int expected = 2;
    for (int i = 0; i < n; i++) {
        EXPECT_EQ(expected, col[i]);
        expected += 3;
    }

    m.bridges[idx].matrix_delete(a);
    free(col);
}

TEST_F(MatrixFixture, matrix_copy_col_with_larger_size) {

    int n = 3;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    int value = 1;
    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            a->value.f[i * a->ncol + j] = value;
            value++;
        }
    }

    double * col = m.bridges[idx].matrix_copy_col(a, 1, 0, 4);

    int expected = 2;
    for (int i = 0; i < n; i++) {
        EXPECT_EQ(expected, col[i]);
        expected += 3;
    }

    m.bridges[idx].matrix_delete(a);
    free(col);
}

TEST_F(MatrixFixture, matrix_get_copy_with_half_size) {

    int n = 4;
    int size = n/2;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    int value = 1;
    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            a->value.f[i * a->ncol + j] = value;
            value++;
        }
    }

    double * col = m.bridges[idx].matrix_copy_col(a, 1, 2, size);

    int expected = 10;
    for (int i = 0; i < size; i++) {
        EXPECT_EQ(expected, col[i]);
        expected += 4;
    }

    m.bridges[idx].matrix_delete(a);
    free(col);
}

TEST_F(MatrixFixture, matrix_copy_row) {

    int n = 3;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    int value = 1;
    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            a->value.f[i * a->ncol + j] = value;
            value++;
        }
    }

    double * row = m.bridges[idx].matrix_copy_row(a, 1, 0, n);

    int expected = 4;
    for (int i = 0; i < n; i++) {
        EXPECT_EQ(expected, row[i]);
        expected += 1;
    }

    m.bridges[idx].matrix_delete(a);
    free(row);
}

TEST_F(MatrixFixture, matrix_copy_row_with_larger_size) {

    int n = 3;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    int value = 1;
    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            a->value.f[i * a->ncol + j] = value;
            value++;
        }
    }

    double * row = m.bridges[idx].matrix_copy_row(a, 1, 0, 4);

    int expected = 4;
    for (int i = 0; i < n; i++) {
        EXPECT_EQ(expected, row[i]);
        expected += 1;
    }

    m.bridges[idx].matrix_delete(a);
    free(row);
}

TEST_F(MatrixFixture, matrix_copy_row_with_half_size) {

    int n = 4;
    int size = n/2;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    int value = 1;
    for (int i = 0; i < a->ncol; i++) {
        for (int j = 0; j < a->nrow; j++) {
            a->value.f[i * a->ncol + j] = value;
            value++;
        }
    }

    double * row = m.bridges[idx].matrix_copy_row(a, 1, 2, size);

    int expected = 7;
    for (int i = 0; i < size; i++) {
        EXPECT_EQ(expected, row[i]);
        expected += 1;
    }

    m.bridges[idx].matrix_delete(a);
    free(row);
}

long mock_memory() {
    return 2 * sizeof(double);
}

TEST_F(MatrixFixture, matrix_mul_larger_than_device_memory) {

    m.bridges[idx].get_Engine_Max_Memory_Allocation_f = mock_memory;
    
    int n = 4;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);
    matrix_t * b = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            a->value.f[i * a->ncol + j] = 3.;
            b->value.f[i * b->ncol + j] = 3.;
        }
    }

    object_t ** in = convertMatMatToObject(a, b);
    
    matrix_t * r = (matrix_t *) mat_mul(&m, idx, (void **) in, NULL);
    
    m.bridges[idx].matreqhost(r);

    for (int i = 0; i < r->ncol; i++) {
        for (int j = 0; j < r->nrow; j++) {
            EXPECT_EQ(36., r->value.f[i * r->ncol + j]);
        }
    }
    delete_object_array(in, 2);
    m.bridges[idx].matrix_delete(a);
    m.bridges[idx].matrix_delete(b);
    m.bridges[idx].matrix_delete(r);

}

// TEST_F(MatrixFixture, matrix_mul_larger_than_device_memory_real_device) {

//     m.bridges[idx].get_Engine_Max_Memory_Allocation_f = mock_memory;
    
//     int n = 13900;

//     matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);
//     matrix_t * b = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);

//     for (int i = 0; i < b->ncol; i++) {
//         for (int j = 0; j < b->nrow; j++) {
//             a->value.f[i * a->ncol + j] = 3.;
//             b->value.f[i * b->ncol + j] = 3.;
//         }
//     }

//     object_t ** in = convertMatMatToObject(a, b);
    
//     matrix_t * r = (matrix_t *) mat_mul(&m, idx, (void **) in, NULL);
    
//     m.bridges[idx].matreqhost(r);

//     // for (int i = 0; i < r->ncol; i++) {
//     //     for (int j = 0; j < r->nrow; j++) {
//     //         EXPECT_EQ(36., r->value.f[i * r->ncol + j]);
//     //     }
//     // }
//     delete_object_array(in, 2);
//     m.bridges[idx].matrix_delete(a);
//     m.bridges[idx].matrix_delete(b);
//     m.bridges[idx].matrix_delete(r);

// }