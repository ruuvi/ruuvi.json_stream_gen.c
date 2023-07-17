/**
 * @file test_json_stram_gen_formatted.cpp
 * @author TheSomeMan
 * @date 2023-08-11
 * @copyright Ruuvi Innovations Ltd, license BSD-3-Clause.
 */

#include "json_stream_gen.h"
#include "gtest/gtest.h"
#include <string>
#include "json_stream_gen_wrapper.h"

using namespace std;

/*** Google-test class implementation
 * *********************************************************************************/

class TestJsonStreamGenF;
static TestJsonStreamGenF* g_pTestClass;

class TestJsonStreamGenF : public ::testing::Test
{
private:
protected:
    void
    SetUp() override
    {
        g_pTestClass = this;
    }

    void
    TearDown() override
    {
        g_pTestClass = nullptr;
    }

public:
    TestJsonStreamGenF();

    ~TestJsonStreamGenF() override;
};

TestJsonStreamGenF::TestJsonStreamGenF()
    : Test()
{
}

TestJsonStreamGenF::~TestJsonStreamGenF() = default;

/*** Unit-Tests
 * *******************************************************************************************************/

static bool
cb_generate_empty_json(json_stream_gen_t* const p_gen, const void* const p_user_ctx)
{
    (void)p_gen;
    (void)p_user_ctx;
    return true;
}

TEST_F(TestJsonStreamGenF, test_generate_json_empty_formatted) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(&cfg, &cb_generate_empty_json, 0, nullptr);
    json_stream_gen_t*   p_gen   = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "}"),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(string(""), string(p_chunk));
}

typedef struct generate_single_string_t
{
    const char* p_name;
    const char* p_val;
} generate_single_string_t;

static bool
cb_generate_single_string(json_stream_gen_t* const p_gen, const void* const p_user_ctx)
{
    auto p_ctx = static_cast<const generate_single_string_t*>(p_user_ctx);
    JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
    JSON_STREAM_GEN_ADD_STRING(p_gen, p_ctx->p_name, p_ctx->p_val);
    JSON_STREAM_GEN_END_GENERATOR_FUNC();
}

TEST_F(TestJsonStreamGenF, test_generate_json_single_string_formatted__max_chunk_size_19) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 19,
        .flag_formatted_json = true,
    };
    generate_single_string_t* p_ctx   = nullptr;
    JsonStreamGenWrapper      wrapper = JsonStreamGenWrapper(
        &cfg,
        &cb_generate_single_string,
        sizeof(*p_ctx),
        (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name            = "key";
    p_ctx->p_val             = "val";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("{\n  \"key\": \"val\"\n}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_single_string_formatted__max_chunk_size_18) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 18,
        .flag_formatted_json = true,
    };
    generate_single_string_t* p_ctx   = nullptr;
    JsonStreamGenWrapper      wrapper = JsonStreamGenWrapper(
        &cfg,
        &cb_generate_single_string,
        sizeof(*p_ctx),
        (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name            = "key";
    p_ctx->p_val             = "val";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("{\n  \"key\": \"val\""), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("\n}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_single_string_formatted__max_chunk_size_17) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 17,
        .flag_formatted_json = true,
    };
    generate_single_string_t* p_ctx   = nullptr;
    JsonStreamGenWrapper      wrapper = JsonStreamGenWrapper(
        &cfg,
        &cb_generate_single_string,
        sizeof(*p_ctx),
        (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name            = "key";
    p_ctx->p_val             = "val";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("{\n  \"key\": \"val\""), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("\n}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_single_string_formatted__max_chunk_size_16) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 16,
        .flag_formatted_json = true,
    };
    generate_single_string_t* p_ctx   = nullptr;
    JsonStreamGenWrapper      wrapper = JsonStreamGenWrapper(
        &cfg,
        &cb_generate_single_string,
        sizeof(*p_ctx),
        (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name            = "key";
    p_ctx->p_val             = "val";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("{"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("\n  \"key\": \"val\""), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("\n}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_single_string_formatted__max_chunk_size_15) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 15,
        .flag_formatted_json = true,
    };
    generate_single_string_t* p_ctx   = nullptr;
    JsonStreamGenWrapper      wrapper = JsonStreamGenWrapper(
        &cfg,
        &cb_generate_single_string,
        sizeof(*p_ctx),
        (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name            = "key";
    p_ctx->p_val             = "val";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("{"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);
}

typedef struct generate_two_strings_t
{
    const char* p_name1;
    const char* p_val1;
    const char* p_name2;
    const char* p_val2;
} generate_two_strings_t;

static bool
cb_generate_two_strings(json_stream_gen_t* const p_gen, const void* const p_user_ctx)
{
    auto p_ctx = static_cast<const generate_two_strings_t*>(p_user_ctx);
    JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
    JSON_STREAM_GEN_ADD_STRING(p_gen, p_ctx->p_name1, p_ctx->p_val1);
    JSON_STREAM_GEN_ADD_STRING(p_gen, p_ctx->p_name2, p_ctx->p_val2);
    JSON_STREAM_GEN_END_GENERATOR_FUNC();
}

TEST_F(TestJsonStreamGenF, test_generate_json_two_strings_formatted__max_chunk_size_39) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 39,
        .flag_formatted_json = true,
    };

    generate_two_strings_t* p_ctx = nullptr;
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(&cfg, &cb_generate_two_strings, sizeof(*p_ctx), (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name1           = "key1";
    p_ctx->p_val1            = "val1";
    p_ctx->p_name2           = "key2";
    p_ctx->p_val2            = "val2";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key1\": \"val1\",\n"
               "  \"key2\": \"val2\"\n"
               "}"),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_two_strings_formatted__max_chunk_size_38) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 38,
        .flag_formatted_json = true,
    };

    generate_two_strings_t* p_ctx = nullptr;
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(&cfg, &cb_generate_two_strings, sizeof(*p_ctx), (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name1           = "key1";
    p_ctx->p_val1            = "val1";
    p_ctx->p_name2           = "key2";
    p_ctx->p_val2            = "val2";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key1\": \"val1\",\n"
               "  \"key2\": \"val2\""),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("\n}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_two_strings_formatted__max_chunk_size_37) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 37,
        .flag_formatted_json = true,
    };

    generate_two_strings_t* p_ctx = nullptr;
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(&cfg, &cb_generate_two_strings, sizeof(*p_ctx), (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name1           = "key1";
    p_ctx->p_val1            = "val1";
    p_ctx->p_name2           = "key2";
    p_ctx->p_val2            = "val2";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key1\": \"val1\",\n"
               "  \"key2\": \"val2\""),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("\n}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_two_strings_formatted__max_chunk_size_36) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 36,
        .flag_formatted_json = true,
    };

    generate_two_strings_t* p_ctx = nullptr;
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(&cfg, &cb_generate_two_strings, sizeof(*p_ctx), (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name1           = "key1";
    p_ctx->p_val1            = "val1";
    p_ctx->p_name2           = "key2";
    p_ctx->p_val2            = "val2";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key1\": \"val1\""),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string(",\n"
               "  \"key2\": \"val2\"\n"
               "}"),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_two_strings_formatted__max_chunk_size_21) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 21,
        .flag_formatted_json = true,
    };

    generate_two_strings_t* p_ctx = nullptr;
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(&cfg, &cb_generate_two_strings, sizeof(*p_ctx), (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name1           = "key1";
    p_ctx->p_val1            = "val1";
    p_ctx->p_name2           = "key2";
    p_ctx->p_val2            = "val2";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key1\": \"val1\""),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string(",\n"
               "  \"key2\": \"val2\"\n"
               "}"),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_two_strings_formatted__max_chunk_size_20) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 20,
        .flag_formatted_json = true,
    };

    generate_two_strings_t* p_ctx = nullptr;
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(&cfg, &cb_generate_two_strings, sizeof(*p_ctx), (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name1           = "key1";
    p_ctx->p_val1            = "val1";
    p_ctx->p_name2           = "key2";
    p_ctx->p_val2            = "val2";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key1\": \"val1\""),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string(",\n"
               "  \"key2\": \"val2\""),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("\n"
               "}"),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_two_strings_formatted__max_chunk_size_19) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 19,
        .flag_formatted_json = true,
    };

    generate_two_strings_t* p_ctx = nullptr;
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(&cfg, &cb_generate_two_strings, sizeof(*p_ctx), (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name1           = "key1";
    p_ctx->p_val1            = "val1";
    p_ctx->p_name2           = "key2";
    p_ctx->p_val2            = "val2";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key1\": \"val1\""),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string(",\n"
               "  \"key2\": \"val2\""),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("\n"
               "}"),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_two_strings_formatted__max_chunk_size_18) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size      = 18,
        .flag_formatted_json = true,
    };

    generate_two_strings_t* p_ctx = nullptr;
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(&cfg, &cb_generate_two_strings, sizeof(*p_ctx), (void**)&p_ctx);
    ASSERT_NE(nullptr, p_ctx);
    p_ctx->p_name1           = "key1";
    p_ctx->p_val1            = "val1";
    p_ctx->p_name2           = "key2";
    p_ctx->p_val2            = "val2";
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("{"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("\n"
               "  \"key1\": \"val1\""),
        string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);
}

TEST_F(TestJsonStreamGenF, test_generate_objects1__insufficient_nesting_level) // NOLINT
{
    typedef struct generate_objects1_t
    {
        const char* p_name1;
        const char* p_val1;
    } generate_objects1_t;

    generate_objects1_t*  p_ctx = nullptr;
    json_stream_gen_cfg_t cfg   = {
          .max_chunk_size      = 256,
          .flag_formatted_json = true,
          .max_nesting_level   = 1,
    };
    std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            auto p_ctx = static_cast<const generate_objects1_t*>(p_user_ctx);
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_START_OBJECT(p_gen, "obj1");
            JSON_STREAM_GEN_ADD_STRING(p_gen, p_ctx->p_name1, p_ctx->p_val1);
            JSON_STREAM_GEN_END_OBJECT(p_gen);
            JSON_STREAM_GEN_START_OBJECT(p_gen, "obj2");
            JSON_STREAM_GEN_END_OBJECT(p_gen);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        sizeof(*p_ctx),
        (void**)&p_ctx);
    p_ctx->p_name1           = "key1";
    p_ctx->p_val1            = "val1";
    json_stream_gen_t* p_gen = p_wrapper->get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(string("{"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);
}

TEST_F(TestJsonStreamGenF, test_generate_json_string_null) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 52; max_chunk_size > 16; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size      = max_chunk_size,
            .flag_formatted_json = true,
        };
        std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
            &cfg,
            [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
                (void)p_user_ctx;
                JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
                JSON_STREAM_GEN_ADD_STRING(p_gen, "key0", "ABC");
                JSON_STREAM_GEN_ADD_STRING(p_gen, "key1", nullptr);
                JSON_STREAM_GEN_ADD_RAW_STRING(p_gen, "key2", nullptr);
                JSON_STREAM_GEN_END_GENERATOR_FUNC();
            },
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
            string("{\n"
                   "  \"key0\": \"ABC\",\n"
                   "  \"key1\": null,\n"
                   "  \"key2\": null\n"
                   "}"),
            json_str);
    }
}

TEST_F(TestJsonStreamGenF, test_generate_json_string_unformatted__with_escaping) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 79; max_chunk_size > 38; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size      = max_chunk_size,
            .flag_formatted_json = true,
        };
        std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
            &cfg,
            [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
                (void)p_user_ctx;
                JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
                JSON_STREAM_GEN_ADD_STRING(p_gen, "key0", "ABCDEFGIJKLMNOPQRSTUVWXYZ");
                JSON_STREAM_GEN_ADD_STRING(p_gen, "key1", "val\" \\ \b \f \n \r \t");
                JSON_STREAM_GEN_END_GENERATOR_FUNC();
            },
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
            string("{\n"
                   "  \"key0\": \"ABCDEFGIJKLMNOPQRSTUVWXYZ\",\n"
                   "  \"key1\": \"val\\\" \\\\ \\b \\f \\n \\r \\t\"\n"
                   "}"),
            json_str);
    }
}

TEST_F(TestJsonStreamGenF, test_generate_json_string_unformatted__without_escaping) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 72; max_chunk_size > 38; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size      = max_chunk_size,
            .flag_formatted_json = true,
        };
        std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
            &cfg,
            [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
                (void)p_user_ctx;
                JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
                JSON_STREAM_GEN_ADD_RAW_STRING(p_gen, "key0", "ABCDEFGIJKLMNOPQRSTUVWXYZ");
                JSON_STREAM_GEN_ADD_RAW_STRING(p_gen, "key1", "val\" \\ \b \f \n \r \t");
                JSON_STREAM_GEN_END_GENERATOR_FUNC();
            },
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
            string("{\n"
                   "  \"key0\": \"ABCDEFGIJKLMNOPQRSTUVWXYZ\",\n"
                   "  \"key1\": \"val\" \\ \b \f \n \r \t\"\n"
                   "}"),
            json_str);
    }
}

TEST_F(TestJsonStreamGenF, test_generate_objects1__cycle) // NOLINT
{
    typedef struct generate_objects1_t
    {
        const char* p_name1;
        const char* p_val1;
    } generate_objects1_t;

    for (json_stream_gen_size_t max_chunk_size = 56; max_chunk_size >= 20; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size      = max_chunk_size,
            .flag_formatted_json = true,
            .max_nesting_level   = 2,
        };
        generate_objects1_t*                  p_ctx     = nullptr;
        std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
            &cfg,
            [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
                auto p_ctx = static_cast<const generate_objects1_t*>(p_user_ctx);
                JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
                JSON_STREAM_GEN_START_OBJECT(p_gen, "obj1");
                JSON_STREAM_GEN_ADD_STRING(p_gen, p_ctx->p_name1, p_ctx->p_val1);
                JSON_STREAM_GEN_END_OBJECT(p_gen);
                JSON_STREAM_GEN_START_OBJECT(p_gen, "obj2");
                JSON_STREAM_GEN_END_OBJECT(p_gen);
                JSON_STREAM_GEN_END_GENERATOR_FUNC();
            },
            sizeof(*p_ctx),
            (void**)&p_ctx);
        p_ctx->p_name1           = "key1";
        p_ctx->p_val1            = "val1";
        json_stream_gen_t* p_gen = p_wrapper->get();

        const json_stream_gen_size_t json_len = json_stream_gen_calc_size(p_gen);

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
            string("{\n"
                   "  \"obj1\": {\n"
                   "    \"key1\": \"val1\"\n"
                   "  },\n"
                   "  \"obj2\": {}\n"
                   "}"),
            json_str);
        ASSERT_EQ(json_len, json_str.length());
    }
}

TEST_F(TestJsonStreamGenF, test_generate_objects2__cycle) // NOLINT
{
    typedef struct generate_objects2_t
    {
        const char* p_name1;
        const char* p_val1;
        const char* p_name2;
        const char* p_val2;
    } generate_objects2_t;

    for (json_stream_gen_size_t max_chunk_size = 75; max_chunk_size >= 20; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size      = max_chunk_size,
            .flag_formatted_json = true,
            .max_nesting_level   = 2,
        };
        generate_objects2_t*                  p_ctx     = nullptr;
        std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
            &cfg,
            [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
                auto p_ctx = static_cast<const generate_objects2_t*>(p_user_ctx);
                JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
                JSON_STREAM_GEN_START_OBJECT(p_gen, "obj1");
                JSON_STREAM_GEN_ADD_STRING(p_gen, p_ctx->p_name1, p_ctx->p_val1);
                JSON_STREAM_GEN_END_OBJECT(p_gen);
                JSON_STREAM_GEN_START_OBJECT(p_gen, "obj2");
                JSON_STREAM_GEN_ADD_STRING(p_gen, p_ctx->p_name2, p_ctx->p_val2);
                JSON_STREAM_GEN_END_OBJECT(p_gen);
                JSON_STREAM_GEN_END_GENERATOR_FUNC();
            },
            sizeof(*p_ctx),
            (void**)&p_ctx);
        p_ctx->p_name1           = "key1";
        p_ctx->p_val1            = "val1";
        p_ctx->p_name2           = "key2";
        p_ctx->p_val2            = "val2";
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
            string("{\n"
                   "  \"obj1\": {\n"
                   "    \"key1\": \"val1\"\n"
                   "  },\n"
                   "  \"obj2\": {\n"
                   "    \"key2\": \"val2\"\n"
                   "  }\n"
                   "}"),
            json_str);
    }
}

TEST_F(TestJsonStreamGenF, test_generate_objects3__cycle) // NOLINT
{
    typedef struct generate_objects3_t
    {
        const char* p_name1;
        const char* p_val1;
        const char* p_name2;
        const char* p_val2;
    } generate_objects3_t;

    for (json_stream_gen_size_t max_chunk_size = 76; max_chunk_size >= 21; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size      = max_chunk_size,
            .flag_formatted_json = true,
            .max_nesting_level   = 2,
        };
        generate_objects3_t*                  p_ctx     = nullptr;
        std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
            &cfg,
            [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
                auto p_ctx = static_cast<const generate_objects3_t*>(p_user_ctx);
                JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
                JSON_STREAM_GEN_START_OBJECT(p_gen, "obj1");
                JSON_STREAM_GEN_ADD_STRING(p_gen, p_ctx->p_name1, p_ctx->p_val1);
                JSON_STREAM_GEN_ADD_STRING(p_gen, p_ctx->p_name2, p_ctx->p_val2);
                JSON_STREAM_GEN_END_OBJECT(p_gen);
                JSON_STREAM_GEN_START_OBJECT(p_gen, "obj2");
                JSON_STREAM_GEN_END_OBJECT(p_gen);
                JSON_STREAM_GEN_END_GENERATOR_FUNC();
            },
            sizeof(*p_ctx),
            (void**)&p_ctx);
        p_ctx->p_name1           = "key1";
        p_ctx->p_val1            = "val1";
        p_ctx->p_name2           = "key2";
        p_ctx->p_val2            = "val2";
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
            string("{\n"
                   "  \"obj1\": {\n"
                   "    \"key1\": \"val1\",\n"
                   "    \"key2\": \"val2\"\n"
                   "  },\n"
                   "  \"obj2\": {}\n"
                   "}"),
            json_str);
    }
}

TEST_F(TestJsonStreamGenF, test_generate_objects4__cycle) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 179; max_chunk_size >= 25; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size      = max_chunk_size,
            .flag_formatted_json = true,
            .max_nesting_level   = 2,
        };
        std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
            &cfg,
            [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
                (void)p_user_ctx;
                JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
                for (int i = 0; i < 3; ++i)
                {
                    char obj_name[16];
                    snprintf(obj_name, sizeof(obj_name), "obj%c", 'A' + i);
                    JSON_STREAM_GEN_START_OBJECT(p_gen, obj_name);
                    for (int j = 0; j <= i; j++)
                    {
                        char key[16];
                        char val[16];
                        char fill[16];
                        memset(fill, '1' + j, sizeof(fill));
                        snprintf(key, sizeof(key), "key%.*s", j + 1, fill);
                        snprintf(val, sizeof(val), "val%.*s", j + 1, fill);
                        JSON_STREAM_GEN_ADD_STRING(p_gen, key, val);
                    }
                    JSON_STREAM_GEN_END_OBJECT(p_gen);
                }
                JSON_STREAM_GEN_END_GENERATOR_FUNC();
            },
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
            string("{\n"
                   "  \"objA\": {\n"
                   "    \"key1\": \"val1\"\n"
                   "  },\n"
                   "  \"objB\": {\n"
                   "    \"key1\": \"val1\",\n"
                   "    \"key22\": \"val22\"\n"
                   "  },\n"
                   "  \"objC\": {\n"
                   "    \"key1\": \"val1\",\n"
                   "    \"key22\": \"val22\",\n"
                   "    \"key333\": \"val333\"\n"
                   "  }\n"
                   "}"),
            json_str);
    }
}

TEST_F(TestJsonStreamGenF, test_generate_json_int32) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };

    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_INT32(p_gen, "zero", 0);
            JSON_STREAM_GEN_ADD_INT32(p_gen, "one", 1);
            JSON_STREAM_GEN_ADD_INT32(p_gen, "minus_one", -1);
            JSON_STREAM_GEN_ADD_INT32(p_gen, "max", INT32_MAX);
            JSON_STREAM_GEN_ADD_INT32(p_gen, "min", INT32_MIN);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"zero\": 0,\n"
               "  \"one\": 1,\n"
               "  \"minus_one\": -1,\n"
               "  \"max\": 2147483647,\n"
               "  \"min\": -2147483648\n"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_uint32) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };

    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_UINT32(p_gen, "zero", 0U);
            JSON_STREAM_GEN_ADD_UINT32(p_gen, "one", 1U);
            JSON_STREAM_GEN_ADD_UINT32(p_gen, "max", UINT32_MAX);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"zero\": 0,\n"
               "  \"one\": 1,\n"
               "  \"max\": 4294967295\n"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_int64) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };

    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_INT64(p_gen, "zero", 0);
            JSON_STREAM_GEN_ADD_INT64(p_gen, "one", 1);
            JSON_STREAM_GEN_ADD_INT64(p_gen, "minus_one", -1);
            JSON_STREAM_GEN_ADD_INT64(p_gen, "max", INT64_MAX);
            JSON_STREAM_GEN_ADD_INT64(p_gen, "min", INT64_MIN);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"zero\": 0,\n"
               "  \"one\": 1,\n"
               "  \"minus_one\": -1,\n"
               "  \"max\": 9223372036854775807,\n"
               "  \"min\": -9223372036854775808\n"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_uint64) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };

    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_UINT64(p_gen, "zero", 0U);
            JSON_STREAM_GEN_ADD_UINT64(p_gen, "one", 1U);
            JSON_STREAM_GEN_ADD_UINT64(p_gen, "max", UINT64_MAX);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"zero\": 0,\n"
               "  \"one\": 1,\n"
               "  \"max\": 18446744073709551615\n"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_bool) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };

    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_BOOL(p_gen, "true", true);
            JSON_STREAM_GEN_ADD_BOOL(p_gen, "false", false);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"true\": true,\n"
               "  \"false\": false\n"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_null) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };

    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_NULL(p_gen, "key");
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key\": null\n"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_hex_buf) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 22; max_chunk_size > 18; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size      = max_chunk_size,
            .flag_formatted_json = true,
            .max_nesting_level   = 2,
        };
        std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
            &cfg,
            [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
                (void)p_user_ctx;
                const uint8_t buf[3] = {
                    0x01,
                    0x80,
                    0xCC,
                };
                JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
                JSON_STREAM_GEN_ADD_HEX_BUF(p_gen, "key", buf, sizeof(buf));
                JSON_STREAM_GEN_END_GENERATOR_FUNC();
            },
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
            string("{\n"
                   "  \"key\": \"0180CC\"\n"
                   "}"),
            json_str);
    }
}

TEST_F(TestJsonStreamGenF, test_generate_json_floats) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };

    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_0.0", 0.0f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_0.1", 0.1f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_0.3", 0.3f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_0.55555555", 0.55555555f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_1.0", 1.0f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_-1.0", -1.0f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_16777215.0", 16777215.0f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_-16777215.0", -16777215.0f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_16777216.0", 16777216.0f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_-16777216.0", -16777216.0f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_0.12345678", 0.12345678f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_-0.12345678", -0.12345678f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_100000.9", 100000.9f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_100000.99", 100000.99f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_100000.999", 100000.999f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_-100000.9", -100000.9f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_-100000.99", -100000.99f);
            JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key_-100000.999", -100000.999f);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key_0.0\": 0,\n"
               "  \"key_0.1\": 0.1,\n"
               "  \"key_0.3\": 0.3,\n"
               "  \"key_0.55555555\": 0.5555555,\n" // 0.55555555 -> 0.5555555
               "  \"key_1.0\": 1,\n"
               "  \"key_-1.0\": -1,\n"
               "  \"key_16777215.0\": 16777215,\n"
               "  \"key_-16777215.0\": -16777215,\n"
               "  \"key_16777216.0\": 16777216,\n"
               "  \"key_-16777216.0\": -16777216,\n"
               "  \"key_0.12345678\": 0.123456784,\n"   // 0.12345678 -> 0.123456784
               "  \"key_-0.12345678\": -0.123456784,\n" // -0.12345678 -> -0.123456784
               "  \"key_100000.9\": 100000.9,\n"
               "  \"key_100000.99\": 100000.992,\n"
               "  \"key_100000.999\": 100001,\n"
               "  \"key_-100000.9\": -100000.9,\n"
               "  \"key_-100000.99\": -100000.992,\n"
               "  \"key_-100000.999\": -100001\n"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_floats_with_precision) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };

    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_0.0", 0.0f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_0.1_2", 0.1f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_0.1_8", 0.1f, 8);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_0.1_9", 0.1f, 9);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_0.55555555", 0.55555555f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_1.0", 1.0f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_-1.0", -1.0f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_10.3_2", 10.3f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_10.3_8", 10.3f, 8);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_10.3_9", 10.3f, 9);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_16777215.0", 16777215.0f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_-16777215.0", -16777215.0f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_16777216.0", 16777216.0f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_-16777216.0", -16777216.0f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_0.12345678", 0.12345678f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_-0.12345678", -0.12345678f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_100000.9", 100000.9f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_100000.99", 100000.99f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_100000.999", 100000.999f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_-100000.9", -100000.9f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_-100000.99", -100000.99f, 2);
            JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, "key_-100000.999", -100000.999f, 2);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key_0.0\": 0,\n"
               "  \"key_0.1_2\": 0.1,\n"         // precision 2
               "  \"key_0.1_8\": 0.1,\n"         // precision 8
               "  \"key_0.1_9\": 0.100000001,\n" // precision 9
               "  \"key_0.55555555\": 0.56,\n"   // 0.55555555 -> 0.56
               "  \"key_1.0\": 1,\n"
               "  \"key_-1.0\": -1,\n"
               "  \"key_10.3_2\": 10,\n"         // precision 2
               "  \"key_10.3_8\": 10.3,\n"       // precision 8
               "  \"key_10.3_9\": 10.3000002,\n" // precision 9
               "  \"key_16777215.0\": 1.7e+07,\n"
               "  \"key_-16777215.0\": -1.7e+07,\n"
               "  \"key_16777216.0\": 1.7e+07,\n"
               "  \"key_-16777216.0\": -1.7e+07,\n"
               "  \"key_0.12345678\": 0.12,\n"   // 0.12345678 -> 0.12
               "  \"key_-0.12345678\": -0.12,\n" // -0.12345678 -> -0.12
               "  \"key_100000.9\": 1e+05,\n"
               "  \"key_100000.99\": 1e+05,\n"
               "  \"key_100000.999\": 1e+05,\n"
               "  \"key_-100000.9\": -1e+05,\n"
               "  \"key_-100000.99\": -1e+05,\n"
               "  \"key_-100000.999\": -1e+05\n"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_doubles) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };

    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_0.0", 0.0);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_0.1", 0.1);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_0.3", 0.3);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_0.5555555555555555", 0.5555555555555555);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_1.0", 1.0);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_-1.0", -1.0);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_9007199254740992.0", 9007199254740992.0);   // 2^53
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_-9007199254740992.0", -9007199254740992.0); // -2^53
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_9007199254740993.0", 9007199254740993.0);   // 2^53+1
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_-9007199254740993.0", -9007199254740993.0); // -2^53-1
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_0.12345678912345678", 0.12345678912345678);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_-0.12345678912345678", -0.12345678912345678);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_16777215.9999999", 16777215.9999999);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_16777215.99999999", 16777215.99999999);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_16777215.999999999", 16777215.999999999);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_16777215.9999999999", 16777215.9999999999);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_-16777215.9999999", -16777215.9999999);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_-16777215.99999999", -16777215.99999999);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_-16777215.999999999", -16777215.999999999);
            JSON_STREAM_GEN_ADD_DOUBLE(p_gen, "key_-16777215.9999999999", -16777215.9999999999);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key_0.0\": 0,\n"
               "  \"key_0.1\": 0.1,\n"
               "  \"key_0.3\": 0.3,\n"
               "  \"key_0.5555555555555555\": 0.55555555555555547,\n" // 0.5555555555555555 -> 0.55555555555555547
               "  \"key_1.0\": 1,\n"
               "  \"key_-1.0\": -1,\n"
               "  \"key_9007199254740992.0\": 9007199254740992,\n"
               "  \"key_-9007199254740992.0\": -9007199254740992,\n"
               "  \"key_9007199254740993.0\": 9007199254740992,\n"   // 9007199254740993 -> 9007199254740992
               "  \"key_-9007199254740993.0\": -9007199254740992,\n" // -9007199254740993 -> -9007199254740992
               "  \"key_0.12345678912345678\": 0.12345678912345678,\n"
               "  \"key_-0.12345678912345678\": -0.12345678912345678,\n"
               "  \"key_16777215.9999999\": 16777215.9999999,\n"
               "  \"key_16777215.99999999\": 16777215.999999991,\n"
               "  \"key_16777215.999999999\": 16777215.999999998,\n"
               "  \"key_16777215.9999999999\": 16777216,\n"
               "  \"key_-16777215.9999999\": -16777215.9999999,\n"
               "  \"key_-16777215.99999999\": -16777215.999999991,\n"
               "  \"key_-16777215.999999999\": -16777215.999999998,\n"
               "  \"key_-16777215.9999999999\": -16777216\n"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_doubles_with_precision) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };

    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_0.0", 0.0, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_0.1_2", 0.1, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_0.1_16", 0.1, 16);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_0.1_17", 0.1, 17);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_0.5555555555555555", 0.5555555555555555, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_1.0", 1.0, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_-1.0", -1.0, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_10.3_2", 10.3, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_10.3_16", 10.3, 16);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_10.3_17", 10.3, 17);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_9007199254740992.0", 9007199254740992.0, 2); // 2^53
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(
                p_gen,
                "key_-9007199254740992.0",
                -9007199254740992.0,
                2);                                                                                            // -2^53
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_9007199254740993.0", 9007199254740993.0, 2); // 2^53+1
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(
                p_gen,
                "key_-9007199254740993.0",
                -9007199254740993.0,
                2); // -2^53-1
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_0.12345678912345678", 0.12345678912345678, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_-0.12345678912345678", -0.12345678912345678, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_16777215.9999999", 16777215.9999999, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_16777215.99999999", 16777215.99999999, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_16777215.999999999", 16777215.999999999, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_16777215.9999999999", 16777215.9999999999, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_-16777215.9999999", -16777215.9999999, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_-16777215.99999999", -16777215.99999999, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_-16777215.999999999", -16777215.999999999, 2);
            JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, "key_-16777215.9999999999", -16777215.9999999999, 2);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    json_stream_gen_t* p_gen = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(
        string("{\n"
               "  \"key_0.0\": 0,\n"
               "  \"key_0.1_2\": 0.1,\n"
               "  \"key_0.1_16\": 0.1,\n"
               "  \"key_0.1_17\": 0.10000000000000001,\n"
               "  \"key_0.5555555555555555\": 0.56,\n" // 0.5555555555555555 -> 0.56
               "  \"key_1.0\": 1,\n"
               "  \"key_-1.0\": -1,\n"
               "  \"key_10.3_2\": 10,\n"
               "  \"key_10.3_16\": 10.3,\n"
               "  \"key_10.3_17\": 10.300000000000001,\n"
               "  \"key_9007199254740992.0\": 9e+15,\n"
               "  \"key_-9007199254740992.0\": -9e+15,\n"
               "  \"key_9007199254740993.0\": 9e+15,\n"   // 9007199254740993 -> 9007199254740992
               "  \"key_-9007199254740993.0\": -9e+15,\n" // -9007199254740993 -> -9007199254740992
               "  \"key_0.12345678912345678\": 0.12,\n"
               "  \"key_-0.12345678912345678\": -0.12,\n"
               "  \"key_16777215.9999999\": 1.7e+07,\n"
               "  \"key_16777215.99999999\": 1.7e+07,\n"
               "  \"key_16777215.999999999\": 1.7e+07,\n"
               "  \"key_16777215.9999999999\": 1.7e+07,\n"
               "  \"key_-16777215.9999999\": -1.7e+07,\n"
               "  \"key_-16777215.99999999\": -1.7e+07,\n"
               "  \"key_-16777215.999999999\": -1.7e+07,\n"
               "  \"key_-16777215.9999999999\": -1.7e+07\n"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenF, test_generate_json_fixed_float_0p0) // NOLINT
{
    const json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", 0.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", 0.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", 0.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", 0.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", 0.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", 0.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", 0.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{\n"
               "  \"key_0\": 0,\n"
               "  \"key_1\": 0.0,\n"
               "  \"key_2\": 0.00,\n"
               "  \"key_3\": 0.000,\n"
               "  \"key_4\": 0.0000,\n"
               "  \"key_5\": 0.00000,\n"
               "  \"key_6\": 0.000000\n"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenF, test_generate_json_fixed_float_max_val_without_loosing_precision) // NOLINT
{
    const json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", 16777216 /* 2^23 */, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(
                p_gen,
                "key_1",
                16777215 /* 2^23-1 */,
                JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(
                p_gen,
                "key_2",
                -16777216 /* -2^23 */,
                JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(
                p_gen,
                "key_3",
                -16777215 /* -2^23-1 */,
                JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{\n"
               "  \"key_0\": 16777216,\n"
               "  \"key_1\": 16777215,\n"
               "  \"key_2\": -16777216,\n"
               "  \"key_3\": -16777215\n"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenF, test_generate_json_fixed_double_0p0) // NOLINT
{
    const json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", 0.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", 0.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", 0.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", 0.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", 0.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", 0.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", 0.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", 0.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", 0.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", 0.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{\n"
               "  \"key_0\": 0,\n"
               "  \"key_1\": 0.0,\n"
               "  \"key_2\": 0.00,\n"
               "  \"key_3\": 0.000,\n"
               "  \"key_4\": 0.0000,\n"
               "  \"key_5\": 0.00000,\n"
               "  \"key_6\": 0.000000,\n"
               "  \"key_7\": 0.0000000,\n"
               "  \"key_8\": 0.00000000,\n"
               "  \"key_9\": 0.000000000\n"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenF, test_generate_json_fixed_double_max_val_without_loosing_precision) // NOLINT
{
    const json_stream_gen_cfg_t cfg = {
        .flag_formatted_json = true,
    };
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_0",
                9007199254740992 /* 2^53 */,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_1",
                9007199254740991 /* 2^53-1 */,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_2",
                -9007199254740992 /* -2^53 */,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_3",
                -9007199254740991 /* -2^53-1 */,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{\n"
               "  \"key_0\": 9007199254740992,\n"
               "  \"key_1\": 9007199254740991,\n"
               "  \"key_2\": -9007199254740992,\n"
               "  \"key_3\": -9007199254740991\n"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenF, test_generate_array1) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 429; max_chunk_size > 25; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size      = max_chunk_size,
            .flag_formatted_json = true,
            .max_nesting_level   = 2,
        };
        std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
            &cfg,
            [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
                (void)p_user_ctx;
                JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_empty");
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_str1");
                JSON_STREAM_GEN_ADD_STRING_TO_ARRAY(p_gen, "val1");
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_str2");
                JSON_STREAM_GEN_ADD_STRING_TO_ARRAY(p_gen, "val1\t");
                JSON_STREAM_GEN_ADD_RAW_STRING_TO_ARRAY(p_gen, "val2\t");
                JSON_STREAM_GEN_ADD_STRING_TO_ARRAY(p_gen, nullptr);
                JSON_STREAM_GEN_ADD_RAW_STRING_TO_ARRAY(p_gen, nullptr);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_u32");
                JSON_STREAM_GEN_ADD_UINT32_TO_ARRAY(p_gen, 1);
                JSON_STREAM_GEN_ADD_UINT32_TO_ARRAY(p_gen, 2);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_i32");
                JSON_STREAM_GEN_ADD_INT32_TO_ARRAY(p_gen, -1);
                JSON_STREAM_GEN_ADD_INT32_TO_ARRAY(p_gen, -2);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_u64");
                JSON_STREAM_GEN_ADD_UINT64_TO_ARRAY(p_gen, 1);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_i64");
                JSON_STREAM_GEN_ADD_INT64_TO_ARRAY(p_gen, -1);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_bool");
                JSON_STREAM_GEN_ADD_BOOL_TO_ARRAY(p_gen, true);
                JSON_STREAM_GEN_ADD_BOOL_TO_ARRAY(p_gen, false);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_null");
                JSON_STREAM_GEN_ADD_NULL_TO_ARRAY(p_gen);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_float");
                JSON_STREAM_GEN_ADD_FLOAT_TO_ARRAY(p_gen, 1.5f);
                JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION_TO_ARRAY(p_gen, 1.5f, 3);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_double");
                JSON_STREAM_GEN_ADD_DOUBLE_TO_ARRAY(p_gen, 1.5);
                JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION_TO_ARRAY(p_gen, 1.5, 3);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_fixed_float");
                JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT_TO_ARRAY(p_gen, 1.5f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
                JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT_TO_ARRAY(p_gen, 1.5f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_fixed_double");
                JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT_TO_ARRAY(p_gen, 1.5, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
                JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT_TO_ARRAY(p_gen, 1.5, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                const uint8_t buf1[4] = { 0x01, 0x02, 0xaa, 0xcc };
                const uint8_t buf2[3] = { 0x11, 0x12, 0xdd };
                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_hex_buf");
                JSON_STREAM_GEN_ADD_HEX_BUF_TO_ARRAY(p_gen, buf1, sizeof(buf1));
                JSON_STREAM_GEN_ADD_HEX_BUF_TO_ARRAY(p_gen, buf2, sizeof(buf2));
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_END_GENERATOR_FUNC();
            },
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
            string("{\n"
                   "  \"arr_empty\": [],\n"
                   "  \"arr_str1\": [\n"
                   "    \"val1\"\n"
                   "  ],\n"
                   "  \"arr_str2\": [\n"
                   "    \"val1\\t\",\n"
                   "    \"val2\t\",\n"
                   "    null,\n"
                   "    null\n"
                   "  ],\n"
                   "  \"arr_u32\": [\n"
                   "    1,\n"
                   "    2\n"
                   "  ],\n"
                   "  \"arr_i32\": [\n"
                   "    -1,\n"
                   "    -2\n"
                   "  ],\n"
                   "  \"arr_u64\": [\n"
                   "    1\n"
                   "  ],\n"
                   "  \"arr_i64\": [\n"
                   "    -1\n"
                   "  ],\n"
                   "  \"arr_bool\": [\n"
                   "    true,\n"
                   "    false\n"
                   "  ],\n"
                   "  \"arr_null\": [\n"
                   "    null\n"
                   "  ],\n"
                   "  \"arr_float\": [\n"
                   "    1.5,\n"
                   "    1.5\n"
                   "  ],\n"
                   "  \"arr_double\": [\n"
                   "    1.5,\n"
                   "    1.5\n"
                   "  ],\n"
                   "  \"arr_fixed_float\": [\n"
                   "    1.50,\n"
                   "    1.50\n"
                   "  ],\n"
                   "  \"arr_fixed_double\": [\n"
                   "    1.50,\n"
                   "    1.50\n"
                   "  ],\n"
                   "  \"arr_hex_buf\": [\n"
                   "    \"0102AACC\",\n"
                   "    \"1112DD\"\n"
                   "  ]\n"
                   "}"),
            json_str);
    }
}

TEST_F(TestJsonStreamGenF, test_generate_sub_array_and_subobjects) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 196; max_chunk_size > 22; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size      = max_chunk_size,
            .flag_formatted_json = true,
            .max_nesting_level   = 4,
        };
        std::unique_ptr<JsonStreamGenWrapper> p_wrapper = std::make_unique<JsonStreamGenWrapper>(
            &cfg,
            [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
                (void)p_user_ctx;
                JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_of_objects");
                JSON_STREAM_GEN_ADD_OBJECT_TO_ARRAY(p_gen);
                JSON_STREAM_GEN_ADD_STRING(p_gen, "key1", "valA");
                JSON_STREAM_GEN_END_OBJECT(p_gen);
                JSON_STREAM_GEN_ADD_OBJECT_TO_ARRAY(p_gen);
                JSON_STREAM_GEN_ADD_STRING(p_gen, "key1", "valB");
                JSON_STREAM_GEN_END_OBJECT(p_gen);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_START_ARRAY(p_gen, "arr_of_arrays");
                JSON_STREAM_GEN_ADD_ARRAY_TO_ARRAY(p_gen);
                JSON_STREAM_GEN_ADD_STRING_TO_ARRAY(p_gen, "valA");
                JSON_STREAM_GEN_ADD_STRING_TO_ARRAY(p_gen, "valB");
                JSON_STREAM_GEN_END_ARRAY(p_gen);
                JSON_STREAM_GEN_ADD_ARRAY_TO_ARRAY(p_gen);
                JSON_STREAM_GEN_ADD_STRING_TO_ARRAY(p_gen, "valC");
                JSON_STREAM_GEN_END_ARRAY(p_gen);
                JSON_STREAM_GEN_ADD_ARRAY_TO_ARRAY(p_gen);
                JSON_STREAM_GEN_END_ARRAY(p_gen);
                JSON_STREAM_GEN_END_ARRAY(p_gen);

                JSON_STREAM_GEN_END_GENERATOR_FUNC();
            },
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
            string("{\n"
                   "  \"arr_of_objects\": [\n"
                   "    {\n"
                   "      \"key1\": \"valA\"\n"
                   "    },\n"
                   "    {\n"
                   "      \"key1\": \"valB\"\n"
                   "    }\n"
                   "  ],\n"
                   "  \"arr_of_arrays\": [\n"
                   "    [\n"
                   "      \"valA\",\n"
                   "      \"valB\"\n"
                   "    ],\n"
                   "    [\n"
                   "      \"valC\"\n"
                   "    ],\n"
                   "    []\n"
                   "  ]\n"
                   "}"),
            json_str);
    }
}
