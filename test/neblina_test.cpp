#include "gtest/gtest.h"
#include "libneblina.h"
#include "bridge_api.h"
#include "neblina_std.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>

using namespace std;

class NeblinaCoreFixture : public ::testing::Test {
protected:
public:
    bridge_manager_t m;
    int idx;

    NeblinaCoreFixture() {
        idx = 0;
        load_plugin(&m, "/usr/local/lib64/libneblina-cpu-bridge.so", idx);
        m.bridges[idx].InitEngine_f(0);
    }

    protected:
    static void SetUpTestSuite() {
        std::cerr << "TestSuiteSetup" << std::endl;
    }

    static void TearDownTestSuite() {
    }
    
    void SetUp() {
        
    }

    void TearDown() {
    }

    ~NeblinaCoreFixture() {
    }

};

TEST_F(NeblinaCoreFixture, vector_new) {

    int n = 4;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 0 );

    ASSERT_EQ(4, a->len);
    ASSERT_EQ(LOCHOS, a->location);
    ASSERT_EQ(T_FLOAT, a->type);
    ASSERT_TRUE(&(a->value.f) != nullptr);
    ASSERT_EQ(NULL, a->extra);

    m.bridges[idx].vector_delete(a);

}

TEST_F(NeblinaCoreFixture, vector_new_empty) {

    int n = 4;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 0);

    ASSERT_EQ(4, a->len);
    ASSERT_EQ(LOCHOS, a->location);
    ASSERT_EQ(T_FLOAT, a->type);
    ASSERT_TRUE(&(a->value.f) != nullptr);
    ASSERT_EQ(NULL, a->extra);

    m.bridges[idx].vector_delete(a);

}

TEST_F(NeblinaCoreFixture, vector_vecreqdev) {

    int n = 4;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 0 );

    printf("vecreqdev %p\n",(void *)(a->value.f));
    printf("vecreqdev %p\n",a->extra);

    m.bridges[idx].vecreqdev(a);

    printf("vecreqdev %p\n",(void *)(a->value.f));
    printf("vecreqdev %p\n",a->extra);

    ASSERT_EQ(LOCDEV, a->location);
    ASSERT_TRUE(a->extra != nullptr);
    ASSERT_TRUE(a->value.f == nullptr);

    m.bridges[idx].vector_delete(a);

}

TEST_F(NeblinaCoreFixture, matrix_new) {

    int n = 4;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1);

    ASSERT_EQ(4, a->ncol);
    ASSERT_EQ(4, a->nrow);
    ASSERT_EQ(LOCHOS, a->location);
    ASSERT_EQ(T_FLOAT, a->type);
    ASSERT_TRUE(&(a->value.f) != nullptr);
    ASSERT_EQ(NULL, a->extra);

    m.bridges[idx].matrix_delete(a);

}

TEST_F(NeblinaCoreFixture, matrix_matreqdev) {

    int n = 4;

    matrix_t * a = m.bridges[idx].matrix_new(n, n, T_FLOAT, 0);

    printf("matreqdev %p\n",(void *)(a->value.f));
    printf("matreqdev %p\n",a->extra);

    m.bridges[idx].matreqdev(a);

    printf("matreqdev %p\n",(void *)(a->value.f));
    printf("matreqdev %p\n",a->extra);

    ASSERT_EQ(LOCDEV, a->location);
    ASSERT_TRUE(a->extra != nullptr);
    ASSERT_TRUE(a->value.f == nullptr);

    m.bridges[idx].matrix_delete(a);

}

TEST_F(NeblinaCoreFixture, convertToObject_withNULL) {

    int n = 4;
    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 0 );
    object_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }

    object_t ** in = convertToObject(a, NULL);

    ASSERT_EQ(T_VECTOR, in[0]->type);
    vector_t * tmp = (vector_t *) in[0]->value.v;
    ASSERT_EQ(4, tmp->len);

    delete_object_array(in, 1);
    m.bridges[idx].vector_delete(a);

}