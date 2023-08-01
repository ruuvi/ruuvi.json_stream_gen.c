/**
 * @file test_json_stream_gen_sub_funcs.cpp
 * @author TheSomeMan
 * @date 2023-08-29
 * @copyright Ruuvi Innovations Ltd, license BSD-3-Clause.
 */

#include "json_stream_gen.h"
#include "gtest/gtest.h"
#include <string>
#include "json_stream_gen_wrapper.h"

using namespace std;

/*** Google-test class implementation
 * *********************************************************************************/

class TestJsonStreamGenSF;
static TestJsonStreamGenSF* g_pTestClass;

class TestJsonStreamGenSF : public ::testing::Test
{
private:
protected:
    void
    SetUp() override
    {
        g_pTestClass       = this;
        char* p_cur_locale = setlocale(LC_ALL, nullptr);
        assert(nullptr != p_cur_locale);
        m_saved_locale = string(p_cur_locale);
    }

    void
    TearDown() override
    {
        char* p_cur_locale = setlocale(LC_ALL, m_saved_locale.c_str());
        assert(nullptr != p_cur_locale);

        g_pTestClass = nullptr;
    }

public:
    TestJsonStreamGenSF();

    ~TestJsonStreamGenSF() override;

    string m_saved_locale;
};

TestJsonStreamGenSF::TestJsonStreamGenSF()
    : Test()
{
}

TestJsonStreamGenSF::~TestJsonStreamGenSF() = default;

/*** Unit-Tests
 * *******************************************************************************************************/

static JSON_STREAM_GEN_DECL_GENERATOR_SUB_FUNC(
    cb_generate_json_sub_obj1,
    json_stream_gen_t* const p_gen,
    const void* const        p_user_ctx)
{
    (void)p_user_ctx;
    JSON_STREAM_GEN_ADD_INT32(p_gen, "key1", 100);
    JSON_STREAM_GEN_ADD_INT32(p_gen, "key2", 101);
    JSON_STREAM_GEN_END_GENERATOR_SUB_FUNC();
}

static JSON_STREAM_GEN_DECL_GENERATOR_SUB_FUNC(
    cb_generate_json_sub_obj2,
    json_stream_gen_t* const p_gen,
    const void* const        p_user_ctx)
{
    (void)p_user_ctx;
    JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key3", 10.123f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
    JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key4", 11.234f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
    JSON_STREAM_GEN_END_GENERATOR_SUB_FUNC();
}

static json_stream_gen_callback_result_t
cb_generate_json(json_stream_gen_t* const p_gen, const void* const p_user_ctx)
{
    (void)p_user_ctx;
    JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC(p_gen);
    JSON_STREAM_GEN_ADD_STRING(p_gen, "str1", "str1_val1");
    JSON_STREAM_GEN_ADD_STRING(p_gen, "str2", "str1_val2");

    JSON_STREAM_GEN_START_OBJECT(p_gen, "obj1");
    JSON_STREAM_GEN_CALL_GENERATOR_SUB_FUNC(cb_generate_json_sub_obj1, p_gen, p_user_ctx);
    JSON_STREAM_GEN_END_OBJECT(p_gen);

    JSON_STREAM_GEN_START_OBJECT(p_gen, "obj2");
    JSON_STREAM_GEN_CALL_GENERATOR_SUB_FUNC(cb_generate_json_sub_obj2, p_gen, p_user_ctx);
    JSON_STREAM_GEN_END_OBJECT(p_gen);

    JSON_STREAM_GEN_ADD_STRING(p_gen, "str3", "str1_val3");
    JSON_STREAM_GEN_END_GENERATOR_FUNC();
}

TEST_F(TestJsonStreamGenSF, test_sub_funcs) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 126; max_chunk_size > 19; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size    = max_chunk_size,
            .max_nesting_level = 4,
        };
        std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
            &cfg,
            &cb_generate_json,
            0,
            nullptr);
        json_stream_gen_t* p_gen = p_wrapper->get();

        string json_str("");
        while (true)
        {
            const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
            if (nullptr == p_chunk)
            {
                ASSERT_FALSE(nullptr == p_chunk);
            }

            if ('\0' == p_chunk[0])
            {
                break;
            }
            json_str += string(p_chunk);
        }
        ASSERT_EQ(
            string("{"
                   "\"str1\":\"str1_val1\","
                   "\"str2\":\"str1_val2\","
                   "\"obj1\":{"
                   "\"key1\":100,"
                   "\"key2\":101"
                   "},"
                   "\"obj2\":{"
                   "\"key3\":10.12,"
                   "\"key4\":11.234"
                   "},"
                   "\"str3\":\"str1_val3\""
                   "}"),
            json_str);
    }
}
