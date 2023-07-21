/**
 * @file test_json_stram_gen_create_and_delete.cpp
 * @author TheSomeMan
 * @date 2023-08-11
 * @copyright Ruuvi Innovations Ltd, license BSD-3-Clause.
 */

#include "json_stream_gen.h"
#include "gtest/gtest.h"
#include <string>
#include "json_stream_gen_wrapper.h"

using namespace std;

class MemAllocTrace
{
    vector<void*> allocated_mem;

    std::vector<void*>::iterator
    find(void* ptr)
    {
        for (auto iter = this->allocated_mem.begin(); iter != this->allocated_mem.end(); ++iter)
        {
            if (*iter == ptr)
            {
                return iter;
            }
        }
        return this->allocated_mem.end();
    }

public:
    void
    add(void* ptr)
    {
        auto iter = find(ptr);
        assert(iter == this->allocated_mem.end()); // ptr was found in the list of allocated memory blocks
        this->allocated_mem.push_back(ptr);
    }

    void
    remove(void* ptr)
    {
        auto iter = find(ptr);
        assert(iter != this->allocated_mem.end()); // ptr was not found in the list of allocated memory blocks
        this->allocated_mem.erase(iter);
    }

    bool
    is_empty()
    {
        return this->allocated_mem.empty();
    }

    void
    clear()
    {
        this->allocated_mem.clear();
    }
};

class TestJsonStreamGenM;
static TestJsonStreamGenM* g_pTestClass;

class TestJsonStreamGenM : public ::testing::Test
{
private:
protected:
    void
    SetUp() override
    {
        g_pTestClass               = this;
        this->m_malloc_cnt         = 0;
        this->m_malloc_fail_on_cnt = 0;
    }

    void
    TearDown() override
    {
        this->m_mem_alloc_trace.clear();
        g_pTestClass = nullptr;
    }

public:
    MemAllocTrace m_mem_alloc_trace;
    uint32_t      m_malloc_cnt {};
    uint32_t      m_malloc_fail_on_cnt {};

    TestJsonStreamGenM();

    ~TestJsonStreamGenM() override;
};

TestJsonStreamGenM::TestJsonStreamGenM()
    : Test()
{
}

TestJsonStreamGenM::~TestJsonStreamGenM() = default;

extern "C" {

static void*
my_malloc(const size_t size)
{
    assert(nullptr != g_pTestClass);
    if (++g_pTestClass->m_malloc_cnt == g_pTestClass->m_malloc_fail_on_cnt)
    {
        return nullptr;
    }
    void* ptr = malloc(size);
    assert(nullptr != ptr);
    g_pTestClass->m_mem_alloc_trace.add(ptr);
    return ptr;
}

static void
my_free(void* ptr)
{
    assert(nullptr != g_pTestClass);
    g_pTestClass->m_mem_alloc_trace.remove(ptr);
    free(ptr);
}

} // extern "C"

/*** Unit-Tests
 * *******************************************************************************************************/

static bool
cb_generate_empty_json(json_stream_gen_t* const p_gen, const void* const p_user_ctx)
{
    (void)p_gen;
    (void)p_user_ctx;
    return true;
}

TEST_F(TestJsonStreamGenM, test_create_and_delete_without_user_context) // NOLINT
{
    const json_stream_gen_cfg_t cfg = {
        .p_malloc = &my_malloc,
        .p_free   = &my_free,
    };
    void*              p_ctx = nullptr;
    json_stream_gen_t* p_gen = json_stream_gen_create(&cfg, &cb_generate_empty_json, 0, &p_ctx);
    ASSERT_NE(nullptr, p_gen);
    ASSERT_EQ(nullptr, p_ctx);

    const json_stream_gen_size_t json_len = json_stream_gen_calc_size(p_gen);
    ASSERT_EQ(2, json_len);

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("{}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(string(""), string(p_chunk));

    json_stream_gen_delete(&p_gen);
    ASSERT_TRUE(g_pTestClass->m_mem_alloc_trace.is_empty());
}

TEST_F(TestJsonStreamGenM, test_create_and_delete_with_user_context) // NOLINT
{
    typedef struct test_user_ctx_t
    {
        int32_t val1;
    } test_user_ctx_t;
    const json_stream_gen_cfg_t cfg = {
        .p_malloc = &my_malloc,
        .p_free   = &my_free,
    };
    test_user_ctx_t*   p_ctx = nullptr;
    json_stream_gen_t* p_gen = json_stream_gen_create(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            const test_user_ctx_t* const p_ctx = static_cast<const test_user_ctx_t*>(p_user_ctx);
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_INT32(p_gen, "key0", p_ctx->val1);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        sizeof(test_user_ctx_t),
        (void**)&p_ctx);
    ASSERT_NE(nullptr, p_gen);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->val1 = 125;

    const json_stream_gen_size_t json_len = json_stream_gen_calc_size(p_gen);

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    const string expected_json("{\"key0\":125}");
    ASSERT_EQ(expected_json, string(p_chunk));
    ASSERT_EQ(expected_json.length(), json_len);

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(string(""), string(p_chunk));

    json_stream_gen_delete(&p_gen);
    ASSERT_TRUE(g_pTestClass->m_mem_alloc_trace.is_empty());
}

TEST_F(TestJsonStreamGenM, test_create_and_delete_with_user_context_null_ptr) // NOLINT
{
    typedef struct test_user_ctx_t
    {
        int32_t val1;
    } test_user_ctx_t;
    const json_stream_gen_cfg_t cfg = {
        .p_malloc = &my_malloc,
        .p_free   = &my_free,
    };
    json_stream_gen_t* p_gen = json_stream_gen_create(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            const test_user_ctx_t* const p_ctx = static_cast<const test_user_ctx_t*>(p_user_ctx);
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_INT32(p_gen, "key0", p_ctx->val1);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        sizeof(test_user_ctx_t),
        nullptr);
    ASSERT_EQ(nullptr, p_gen);
    ASSERT_TRUE(g_pTestClass->m_mem_alloc_trace.is_empty());
}

TEST_F(TestJsonStreamGenM, test_create_with_small_chunk_size) // NOLINT
{
    const json_stream_gen_cfg_t cfg = {
        .max_chunk_size = 2,
        .p_malloc       = &my_malloc,
        .p_free         = &my_free,
    };
    json_stream_gen_t* p_gen = json_stream_gen_create(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_INT32(p_gen, "key0", 125);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(nullptr, p_gen);
    ASSERT_TRUE(g_pTestClass->m_mem_alloc_trace.is_empty());
}

TEST_F(TestJsonStreamGenM, test_create_malloc_failed) // NOLINT
{
    this->m_malloc_fail_on_cnt      = 1;
    const json_stream_gen_cfg_t cfg = {
        .p_malloc = &my_malloc,
        .p_free   = &my_free,
    };
    json_stream_gen_t* p_gen = json_stream_gen_create(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_INT32(p_gen, "key0", 125);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(nullptr, p_gen);
    ASSERT_TRUE(g_pTestClass->m_mem_alloc_trace.is_empty());
}
