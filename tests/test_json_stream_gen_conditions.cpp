/**
 * @file test_json_stram_gen_conditions.cpp
 * @author TheSomeMan
 * @date 2023-08-20
 * @copyright Ruuvi Innovations Ltd, license BSD-3-Clause.
 */

#include "json_stream_gen.h"
#include "gtest/gtest.h"
#include <string>
#include "json_stream_gen_wrapper.h"

using namespace std;

/*** Google-test class implementation
 * *********************************************************************************/

class TestJsonStreamGenC;
static TestJsonStreamGenC* g_pTestClass;

class TestJsonStreamGenC : public ::testing::Test
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
    TestJsonStreamGenC();

    ~TestJsonStreamGenC() override;

    string m_saved_locale;
};

TestJsonStreamGenC::TestJsonStreamGenC()
    : Test()
{
}

TestJsonStreamGenC::~TestJsonStreamGenC() = default;

/*** Unit-Tests
 * *******************************************************************************************************/

TEST_F(TestJsonStreamGenC, test_indentation_mark_tab) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
        .indentation_mark    = '\t',
        .indentation         = 1,
    };
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_STRING(p_gen, "key0", "abc");
            JSON_STREAM_GEN_ADD_STRING(p_gen, "key1", "def");
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "\t\"key0\":\t\"abc\",\n"
               "\t\"key1\":\t\"def\"\n"
               "}"),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenC, test_extra_end_object) // NOLINT
{
    json_stream_gen_cfg_t                 cfg       = {};
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_STRING(p_gen, "key0", "abc");
            JSON_STREAM_GEN_END_OBJECT(p_gen);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);
}

TEST_F(TestJsonStreamGenC, test_extra_end_object2) // NOLINT
{
    json_stream_gen_cfg_t                 cfg       = {};
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_END_OBJECT(p_gen);
            JSON_STREAM_GEN_ADD_STRING(p_gen, "key0", "abc");
            JSON_STREAM_GEN_END_OBJECT(p_gen);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);
}

TEST_F(TestJsonStreamGenC, test_extra_end_object3) // NOLINT
{
    json_stream_gen_cfg_t                 cfg       = {};
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_START_OBJECT(p_gen, "obj");
            JSON_STREAM_GEN_ADD_STRING(p_gen, "key0", "abc");
            JSON_STREAM_GEN_END_OBJECT(p_gen);
            JSON_STREAM_GEN_END_OBJECT(p_gen);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);
}

TEST_F(TestJsonStreamGenC, test_no_end_object) // NOLINT
{
    json_stream_gen_cfg_t                 cfg       = {};
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_START_OBJECT(p_gen, "obj");
            JSON_STREAM_GEN_ADD_STRING(p_gen, "key0", "abc");
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);
}

TEST_F(TestJsonStreamGenC, test_extra_end_array) // NOLINT
{
    json_stream_gen_cfg_t                 cfg       = {};
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_STRING(p_gen, "key0", "abc");
            JSON_STREAM_GEN_END_ARRAY(p_gen);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);
}

TEST_F(TestJsonStreamGenC, test_extra_end_array2) // NOLINT
{
    json_stream_gen_cfg_t                 cfg       = {};
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_END_ARRAY(p_gen);
            JSON_STREAM_GEN_ADD_STRING(p_gen, "key0", "abc");
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);
}

TEST_F(TestJsonStreamGenC, test_extra_end_array3) // NOLINT
{
    json_stream_gen_cfg_t                 cfg       = {};
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_START_ARRAY(p_gen, "arr");
            JSON_STREAM_GEN_ADD_STRING_TO_ARRAY(p_gen, "abc");
            JSON_STREAM_GEN_END_ARRAY(p_gen);
            JSON_STREAM_GEN_ADD_STRING_TO_ARRAY(p_gen, "abc");
            JSON_STREAM_GEN_END_ARRAY(p_gen);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);
}

TEST_F(TestJsonStreamGenC, test_no_end_array3) // NOLINT
{
    json_stream_gen_cfg_t                 cfg       = {};
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_START_ARRAY(p_gen, "arr");
            JSON_STREAM_GEN_ADD_STRING_TO_ARRAY(p_gen, "abc");
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);
}

TEST_F(TestJsonStreamGenC, test_calc_size_while_generating_chunks) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 20U,
        .flag_formatted_json = true,
    };
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_STRING(p_gen, "key0", "abc");
            JSON_STREAM_GEN_ADD_STRING(p_gen, "key1", "def");
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const size_t json_len1 = json_stream_gen_calc_size(p_gen);
    ASSERT_EQ(36, json_len1);

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key0\": \"abc\""),
        string(p_chunk));

    // It's impossible to call 'calc_size' while generating chunks is in progress
    const size_t json_len2 = json_stream_gen_calc_size(p_gen);
    ASSERT_EQ(-1, json_len2);

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(
        string(",\n"
               "  \"key1\": \"def\"\n"
               "}"),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(string(""), string(p_chunk));

    json_stream_gen_reset(p_gen);
    const size_t json_len3 = json_stream_gen_calc_size(p_gen);
    ASSERT_EQ(36, json_len3);
}

TEST_F(TestJsonStreamGenC, test_insufficient_buffer_for_one_string) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 10U,
        .flag_formatted_json = true,
    };
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_STRING(p_gen, "key0", "abc");
            JSON_STREAM_GEN_ADD_STRING(p_gen, "key1", "def");
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("{"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);

    json_stream_gen_reset(p_gen);
    const size_t json_len = json_stream_gen_calc_size(p_gen);
    ASSERT_EQ(-1, json_len);
}

TEST_F(TestJsonStreamGenC, test_different_locale_with_comma_as_decimal_separator) // NOLINT
{
    std::array<char, 20> tmp_buf {};
    // By default, a float number will be printed with a period as decimal separator
    (void)snprintf(tmp_buf.data(), tmp_buf.size(), "%.3f", 1.234f);
    ASSERT_EQ(string("1.234"), string(tmp_buf.data()));

    char* p_cur_locale = setlocale(LC_ALL, "de_DE.utf8");
    // If "de_DE.utf8" locale is not available, install it using following commands (Ubuntu):
    // sudo apt-get install -y locales
    // sudo locale-gen de_DE.UTF-8
    ASSERT_NE(nullptr, p_cur_locale);
    ASSERT_EQ(string("de_DE.utf8"), string(p_cur_locale));

    // Now the float number will be printed with a comma as decimal separator
    (void)snprintf(tmp_buf.data(), tmp_buf.size(), "%.3f", 1.234f);
    ASSERT_EQ(string("1,234"), string(tmp_buf.data()));

    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key0", 1.234f);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key1", 1.234f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key2", 1.234f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key3", 1.234f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key4", 1.234);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key5", 1.234, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key6", 1.234, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key7", 1.234, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key0\": 1.234,\n"
               "  \"key1\": 1.2,\n"
               "  \"key2\": 1.23,\n"
               "  \"key3\": 1.23,\n"
               "  \"key4\": 1.234,\n"
               "  \"key5\": 1.2,\n"
               "  \"key6\": 1.23,\n"
               "  \"key7\": 1.23\n"
               "}"),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(string(""), string(p_chunk));
}

static struct lconv*
my_localeconv_null(void)
{
    return nullptr;
}

TEST_F(TestJsonStreamGenC, test_localeconv_null) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
        .p_localeconv        = &my_localeconv_null,
    };
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key0", 1.234f);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key1", 1.234f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key2", 1.234f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key3", 1.234f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key4", 1.234);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key5", 1.234, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key6", 1.234, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key7", 1.234, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key0\": 1.234,\n"
               "  \"key1\": 1.2,\n"
               "  \"key2\": 1.23,\n"
               "  \"key3\": 1.23,\n"
               "  \"key4\": 1.234,\n"
               "  \"key5\": 1.2,\n"
               "  \"key6\": 1.23,\n"
               "  \"key7\": 1.23\n"
               "}"),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenC, test_float_nan_inf) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();

            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "f_NAN_1", NAN);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "f_NAN_2", NAN, 2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "f_NAN_3", NAN, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "f_NAN_4", NAN, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);

            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "f_INF_1", INFINITY);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "f_INF_2", INFINITY, 2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "f_INF_3", INFINITY, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(
                p_gen,
                "f_INF_4",
                INFINITY,
                JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);

            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "d_NAN_1", NAN);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "d_NAN_2", NAN, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "d_NAN_3", NAN, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "d_NAN_4",
                NAN,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);

            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "d_INF_1", INFINITY);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "d_INF_2", INFINITY, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "d_INF_3", INFINITY, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "d_INF_4",
                INFINITY,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);

            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"f_NAN_1\": null,\n"
               "  \"f_NAN_2\": null,\n"
               "  \"f_NAN_3\": null,\n"
               "  \"f_NAN_4\": null,\n"
               "  \"f_INF_1\": null,\n"
               "  \"f_INF_2\": null,\n"
               "  \"f_INF_3\": null,\n"
               "  \"f_INF_4\": null,\n"
               "  \"d_NAN_1\": null,\n"
               "  \"d_NAN_2\": null,\n"
               "  \"d_NAN_3\": null,\n"
               "  \"d_NAN_4\": null,\n"
               "  \"d_INF_1\": null,\n"
               "  \"d_INF_2\": null,\n"
               "  \"d_INF_3\": null,\n"
               "  \"d_INF_4\": null\n"
               "}"),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenC, test_float_limited_fixed_point_with_num_decimals_out_of_range) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();

            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(
                p_gen,
                "key1",
                1.23412341234,
                (json_stream_gen_num_decimals_float_e)(JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6 + 1));
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key2",
                1.23412341234123412341234,
                (json_stream_gen_num_decimals_double_e)(JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_12 + 1));
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key1\": null,\n"
               "  \"key2\": null\n"
               "}"),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(string(""), string(p_chunk));
}
