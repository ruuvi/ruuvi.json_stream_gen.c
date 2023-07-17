/**
 * @file test_json_stram_gen_unformatted.cpp
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

class TestJsonStreamGenU;
static TestJsonStreamGenU* g_pTestClass;

class TestJsonStreamGenU : public ::testing::Test
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
    TestJsonStreamGenU();

    ~TestJsonStreamGenU() override;
};

TestJsonStreamGenU::TestJsonStreamGenU()
    : Test()
{
}

TestJsonStreamGenU::~TestJsonStreamGenU() = default;

/*** Unit-Tests
 * *******************************************************************************************************/

static bool
cb_generate_empty_json(json_stream_gen_t* const p_gen, const void* const p_user_ctx)
{
    (void)p_gen;
    (void)p_user_ctx;
    return true;
}

TEST_F(TestJsonStreamGenU, test_generate_json_empty_unformatted) // NOLINT
{
    JsonStreamGenWrapper wrapper = JsonStreamGenWrapper(nullptr, &cb_generate_empty_json, 0, nullptr);
    json_stream_gen_t*   p_gen   = wrapper.get();

    const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("{}"), string(p_chunk));

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

TEST_F(TestJsonStreamGenU, test_generate_json_single_string_unformatted__max_chunk_size_14) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size = 14,
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
    ASSERT_EQ(string("{\"key\":\"val\"}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_single_string_unformatted__max_chunk_size_13) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size = 13,
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
    ASSERT_EQ(string("{\"key\":\"val\""), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_single_string_unformatted__max_chunk_size_12) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size = 12,
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
    ASSERT_EQ(string("\"key\":\"val\""), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_single_string_unformatted__max_chunk_size_11) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size = 11,
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

TEST_F(TestJsonStreamGenU, test_generate_json_two_strings_unformatted__max_chunk_size_30) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size = 30,
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
    ASSERT_EQ(string("{\"key1\":\"val1\",\"key2\":\"val2\"}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_two_strings_unformatted__max_chunk_size_29) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size = 29,
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
    ASSERT_EQ(string("{\"key1\":\"val1\",\"key2\":\"val2\""), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_two_strings_unformatted__max_chunk_size_28) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size = 28,
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
    ASSERT_EQ(string("{\"key1\":\"val1\""), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(",\"key2\":\"val2\"}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_two_strings_unformatted__max_chunk_size_16) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size = 16,
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
    ASSERT_EQ(string("{\"key1\":\"val1\""), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(",\"key2\":\"val2\"}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_two_strings_unformatted__max_chunk_size_15) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size = 15,
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
    ASSERT_EQ(string("{\"key1\":\"val1\""), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(",\"key2\":\"val2\""), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string("}"), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_NE(nullptr, p_chunk);
    ASSERT_EQ(string(""), string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_two_strings_unformatted__max_chunk_size_14) // NOLINT
{
    json_stream_gen_cfg_t cfg = {
        .max_chunk_size = 14,
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
    ASSERT_EQ(string("\"key1\":\"val1\""), string(p_chunk));

    p_chunk = json_stream_gen_get_next_chunk(p_gen);
    ASSERT_EQ(nullptr, p_chunk);
}

TEST_F(TestJsonStreamGenU, test_generate_json_string_null) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 39; max_chunk_size > 12; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size = max_chunk_size,
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
        ASSERT_EQ(string("{\"key0\":\"ABC\",\"key1\":null,\"key2\":null}"), json_str);
    }
}

TEST_F(TestJsonStreamGenU, test_generate_json_string_unformatted__with_escaping) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 70; max_chunk_size > 34; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size = max_chunk_size,
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
            string("{\"key0\":\"ABCDEFGIJKLMNOPQRSTUVWXYZ\",\"key1\":\"val\\\" \\\\ \\b \\f \\n \\r \\t\"}"),
            json_str);
    }
}

TEST_F(TestJsonStreamGenU, test_generate_json_string_unformatted__without_escaping) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 63; max_chunk_size > 34; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size = max_chunk_size,
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
        ASSERT_EQ(string("{\"key0\":\"ABCDEFGIJKLMNOPQRSTUVWXYZ\",\"key1\":\"val\" \\ \b \f \n \r \t\"}"), json_str);
    }
}

TEST_F(TestJsonStreamGenU, test_generate_objects1__cycle) // NOLINT
{
    typedef struct generate_objects1_t
    {
        const char* p_name1;
        const char* p_val1;
    } generate_objects1_t;

    for (json_stream_gen_size_t max_chunk_size = 35; max_chunk_size >= 14; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size    = max_chunk_size,
            .max_nesting_level = 2,
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
        p_ctx->p_name1 = "key1";
        p_ctx->p_val1  = "val1";

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
        ASSERT_EQ(string("{\"obj1\":{\"key1\":\"val1\"},\"obj2\":{}}"), json_str);
        ASSERT_EQ(json_len, json_str.length());
    }
}

TEST_F(TestJsonStreamGenU, test_generate_objects2__cycle) // NOLINT
{
    typedef struct generate_objects2_t
    {
        const char* p_name1;
        const char* p_val1;
        const char* p_name2;
        const char* p_val2;
    } generate_objects2_t;

    for (json_stream_gen_size_t max_chunk_size = 48; max_chunk_size >= 14; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size    = max_chunk_size,
            .max_nesting_level = 2,
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
        ASSERT_EQ(string("{\"obj1\":{\"key1\":\"val1\"},\"obj2\":{\"key2\":\"val2\"}}"), json_str);
    }
}

TEST_F(TestJsonStreamGenU, test_generate_objects3__cycle) // NOLINT
{
    typedef struct generate_objects3_t
    {
        const char* p_name1;
        const char* p_val1;
        const char* p_name2;
        const char* p_val2;
    } generate_objects3_t;

    for (json_stream_gen_size_t max_chunk_size = 49; max_chunk_size >= 15; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size    = max_chunk_size,
            .max_nesting_level = 2,
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
        ASSERT_EQ(string("{\"obj1\":{\"key1\":\"val1\",\"key2\":\"val2\"},\"obj2\":{}}"), json_str);
    }
}

TEST_F(TestJsonStreamGenU, test_generate_objects4__cycle) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 121; max_chunk_size >= 19; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size    = max_chunk_size,
            .max_nesting_level = 2,
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
            string("{\"objA\":{\"key1\":\"val1\"},\"objB\":{\"key1\":\"val1\",\"key22\":\"val22\"},\"objC\":{\"key1\":"
                   "\"val1\",\"key22\":\"val22\",\"key333\":\"val333\"}}"),
            json_str);
    }
}

TEST_F(TestJsonStreamGenU, test_generate_json_int32) // NOLINT
{
    json_stream_gen_cfg_t cfg = {};

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
        string("{\"zero\":0,\"one\":1,\"minus_one\":-1,\"max\":2147483647,\"min\":-2147483648}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_uint32) // NOLINT
{
    json_stream_gen_cfg_t cfg = {};

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
    ASSERT_EQ(string("{\"zero\":0,\"one\":1,\"max\":4294967295}"), string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_int64) // NOLINT
{
    json_stream_gen_cfg_t cfg = {};

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
        string("{\"zero\":0,\"one\":1,\"minus_one\":-1,\"max\":9223372036854775807,\"min\":-9223372036854775808}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_uint64) // NOLINT
{
    json_stream_gen_cfg_t cfg = {};

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
    ASSERT_EQ(string("{\"zero\":0,\"one\":1,\"max\":18446744073709551615}"), string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_bool) // NOLINT
{
    json_stream_gen_cfg_t cfg = {};

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
    ASSERT_EQ(string("{\"true\":true,\"false\":false}"), string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_null) // NOLINT
{
    json_stream_gen_cfg_t cfg = {};

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
    ASSERT_EQ(string("{\"key\":null}"), string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_hex_buf) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 17; max_chunk_size > 14; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size    = max_chunk_size,
            .max_nesting_level = 2,
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
        ASSERT_EQ(string("{\"key\":\"0180CC\"}"), json_str);
    }
}

TEST_F(TestJsonStreamGenU, test_generate_json_floats) // NOLINT
{
    json_stream_gen_cfg_t cfg = {};

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
        string("{"
               "\"key_0.0\":0,"
               "\"key_0.1\":0.1,"
               "\"key_0.3\":0.3,"
               "\"key_0.55555555\":0.5555555," // 0.55555555 -> 0.5555555
               "\"key_1.0\":1,"
               "\"key_-1.0\":-1,"
               "\"key_16777215.0\":16777215,"
               "\"key_-16777215.0\":-16777215,"
               "\"key_16777216.0\":16777216,"
               "\"key_-16777216.0\":-16777216,"
               "\"key_0.12345678\":0.123456784,"   // 0.12345678 -> 0.123456784
               "\"key_-0.12345678\":-0.123456784," // -0.12345678 -> -0.123456784
               "\"key_100000.9\":100000.9,"
               "\"key_100000.99\":100000.992,"
               "\"key_100000.999\":100001,"
               "\"key_-100000.9\":-100000.9,"
               "\"key_-100000.99\":-100000.992,"
               "\"key_-100000.999\":-100001"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_floats_with_precision) // NOLINT
{
    json_stream_gen_cfg_t cfg = {};

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
        string("{"
               "\"key_0.0\":0,"
               "\"key_0.1_2\":0.1,"         // precision 2
               "\"key_0.1_8\":0.1,"         // precision 8
               "\"key_0.1_9\":0.100000001," // precision 9
               "\"key_0.55555555\":0.56,"   // 0.55555555 -> 0.56
               "\"key_1.0\":1,"
               "\"key_-1.0\":-1,"
               "\"key_10.3_2\":10,"         // precision 2
               "\"key_10.3_8\":10.3,"       // precision 8
               "\"key_10.3_9\":10.3000002," // precision 9
               "\"key_16777215.0\":1.7e+07,"
               "\"key_-16777215.0\":-1.7e+07,"
               "\"key_16777216.0\":1.7e+07,"
               "\"key_-16777216.0\":-1.7e+07,"
               "\"key_0.12345678\":0.12,"   // 0.12345678 -> 0.12
               "\"key_-0.12345678\":-0.12," // -0.12345678 -> -0.12
               "\"key_100000.9\":1e+05,"
               "\"key_100000.99\":1e+05,"
               "\"key_100000.999\":1e+05,"
               "\"key_-100000.9\":-1e+05,"
               "\"key_-100000.99\":-1e+05,"
               "\"key_-100000.999\":-1e+05"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_doubles) // NOLINT
{
    json_stream_gen_cfg_t cfg = {};

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
        string("{"
               "\"key_0.0\":0,"
               "\"key_0.1\":0.1,"
               "\"key_0.3\":0.3,"
               "\"key_0.5555555555555555\":0.55555555555555547," // 0.5555555555555555 -> 0.55555555555555547
               "\"key_1.0\":1,"
               "\"key_-1.0\":-1,"
               "\"key_9007199254740992.0\":9007199254740992,"
               "\"key_-9007199254740992.0\":-9007199254740992,"
               "\"key_9007199254740993.0\":9007199254740992,"   // 9007199254740993 -> 9007199254740992
               "\"key_-9007199254740993.0\":-9007199254740992," // -9007199254740993 -> -9007199254740992
               "\"key_0.12345678912345678\":0.12345678912345678,"
               "\"key_-0.12345678912345678\":-0.12345678912345678,"
               "\"key_16777215.9999999\":16777215.9999999,"
               "\"key_16777215.99999999\":16777215.999999991,"
               "\"key_16777215.999999999\":16777215.999999998,"
               "\"key_16777215.9999999999\":16777216,"
               "\"key_-16777215.9999999\":-16777215.9999999,"
               "\"key_-16777215.99999999\":-16777215.999999991,"
               "\"key_-16777215.999999999\":-16777215.999999998,"
               "\"key_-16777215.9999999999\":-16777216"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_doubles_with_precision) // NOLINT
{
    json_stream_gen_cfg_t cfg = {};

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
        string("{"
               "\"key_0.0\":0,"
               "\"key_0.1_2\":0.1,"
               "\"key_0.1_16\":0.1,"
               "\"key_0.1_17\":0.10000000000000001,"
               "\"key_0.5555555555555555\":0.56," // 0.5555555555555555 -> 0.56
               "\"key_1.0\":1,"
               "\"key_-1.0\":-1,"
               "\"key_10.3_2\":10,"
               "\"key_10.3_16\":10.3,"
               "\"key_10.3_17\":10.300000000000001,"
               "\"key_9007199254740992.0\":9e+15,"
               "\"key_-9007199254740992.0\":-9e+15,"
               "\"key_9007199254740993.0\":9e+15,"   // 9007199254740993 -> 9007199254740992
               "\"key_-9007199254740993.0\":-9e+15," // -9007199254740993 -> -9007199254740992
               "\"key_0.12345678912345678\":0.12,"
               "\"key_-0.12345678912345678\":-0.12,"
               "\"key_16777215.9999999\":1.7e+07,"
               "\"key_16777215.99999999\":1.7e+07,"
               "\"key_16777215.999999999\":1.7e+07,"
               "\"key_16777215.9999999999\":1.7e+07,"
               "\"key_-16777215.9999999\":-1.7e+07,"
               "\"key_-16777215.99999999\":-1.7e+07,"
               "\"key_-16777215.999999999\":-1.7e+07,"
               "\"key_-16777215.9999999999\":-1.7e+07"
               "}"),
        string(p_chunk));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_0p0) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
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
        string("{"
               "\"key_0\":0,"
               "\"key_1\":0.0,"
               "\"key_2\":0.00,"
               "\"key_3\":0.000,"
               "\"key_4\":0.0000,"
               "\"key_5\":0.00000,"
               "\"key_6\":0.000000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_1p0) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", 1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", 1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", 1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", 1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", 1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", 1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", 1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":1,"
               "\"key_1\":1.0,"
               "\"key_2\":1.00,"
               "\"key_3\":1.000,"
               "\"key_4\":1.0000,"
               "\"key_5\":1.00000,"
               "\"key_6\":1.000000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_minus1p0) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", -1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", -1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", -1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", -1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", -1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", -1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", -1.0f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-1,"
               "\"key_1\":-1.0,"
               "\"key_2\":-1.00,"
               "\"key_3\":-1.000,"
               "\"key_4\":-1.0000,"
               "\"key_5\":-1.00000,"
               "\"key_6\":-1.000000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_0p01) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", 0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", 0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", 0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", 0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", 0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", 0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", 0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":0,"
               "\"key_1\":0.0,"
               "\"key_2\":0.01,"
               "\"key_3\":0.010,"
               "\"key_4\":0.0100,"
               "\"key_5\":0.01000,"
               "\"key_6\":0.010000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_minus0p01) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", -0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", -0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", -0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", -0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", -0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", -0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", -0.01f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-0,"
               "\"key_1\":-0.0,"
               "\"key_2\":-0.01,"
               "\"key_3\":-0.010,"
               "\"key_4\":-0.0100,"
               "\"key_5\":-0.01000,"
               "\"key_6\":-0.010000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_0p06) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", 0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", 0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", 0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", 0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", 0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", 0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", 0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":0,"
               "\"key_1\":0.1,"
               "\"key_2\":0.06,"
               "\"key_3\":0.060,"
               "\"key_4\":0.0600,"
               "\"key_5\":0.06000,"
               "\"key_6\":0.060000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_minus0p06) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", -0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", -0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", -0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", -0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", -0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", -0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", -0.06f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-0,"
               "\"key_1\":-0.1,"
               "\"key_2\":-0.06,"
               "\"key_3\":-0.060,"
               "\"key_4\":-0.0600,"
               "\"key_5\":-0.06000,"
               "\"key_6\":-0.060000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_0p555555555) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", 0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", 0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", 0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", 0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", 0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", 0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", 0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":1,"
               "\"key_1\":0.6,"
               "\"key_2\":0.56,"
               "\"key_3\":0.556,"
               "\"key_4\":0.5556,"
               "\"key_5\":0.55556,"
               "\"key_6\":0.555556"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_minus0p555555555) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", -0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", -0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", -0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", -0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", -0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", -0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", -0.555555555f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-1,"
               "\"key_1\":-0.6,"
               "\"key_2\":-0.56,"
               "\"key_3\":-0.556,"
               "\"key_4\":-0.5556,"
               "\"key_5\":-0.55556,"
               "\"key_6\":-0.555556"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_10p3) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", 10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", 10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", 10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", 10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", 10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", 10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", 10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":10,"
               "\"key_1\":10.3,"
               "\"key_2\":10.30,"
               "\"key_3\":10.300,"
               "\"key_4\":10.3000,"
               "\"key_5\":10.30000,"
               "\"key_6\":10.300000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_minus10p3) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", -10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", -10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", -10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", -10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", -10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", -10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", -10.3f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-10,"
               "\"key_1\":-10.3,"
               "\"key_2\":-10.30,"
               "\"key_3\":-10.300,"
               "\"key_4\":-10.3000,"
               "\"key_5\":-10.30000,"
               "\"key_6\":-10.300000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_max_val_without_loosing_precision) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
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
        string("{"
               "\"key_0\":16777216,"
               "\"key_1\":16777215,"
               "\"key_2\":-16777216,"
               "\"key_3\":-16777215"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_1677721p4) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_0", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_1", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_2", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_3", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_4", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_5", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_6", 1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":1677721,"
               "\"key_0\":1677721,"
               "\"key_1\":1677721.4,"
               "\"key_1\":1677721.4,"
               "\"key_2\":1677721.38,"
               "\"key_2\":1677721.4,"
               "\"key_3\":1677721.375,"
               "\"key_3\":1677721.4,"
               "\"key_4\":1677721.3750,"
               "\"key_4\":1677721.4,"
               "\"key_5\":1677721.37500,"
               "\"key_5\":1677721.4,"
               "\"key_6\":1677721.375000,"
               "\"key_6\":1677721.4"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_minus1677721p4) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_0", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_1", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_2", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_3", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_4", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_5", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_6", -1677721.4f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-1677721,"
               "\"key_0\":-1677721,"
               "\"key_1\":-1677721.4,"
               "\"key_1\":-1677721.4,"
               "\"key_2\":-1677721.38,"
               "\"key_2\":-1677721.4,"
               "\"key_3\":-1677721.375,"
               "\"key_3\":-1677721.4,"
               "\"key_4\":-1677721.3750,"
               "\"key_4\":-1677721.4,"
               "\"key_5\":-1677721.37500,"
               "\"key_5\":-1677721.4,"
               "\"key_6\":-1677721.375000,"
               "\"key_6\":-1677721.4"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_1677721p59) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_0", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_1", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_2", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_3", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_4", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_5", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_6", 1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":1677722,"
               "\"key_0\":1677722,"
               "\"key_1\":1677721.6,"
               "\"key_1\":1677721.6,"
               "\"key_2\":1677721.62,"
               "\"key_2\":1677721.6,"
               "\"key_3\":1677721.625,"
               "\"key_3\":1677721.6,"
               "\"key_4\":1677721.6250,"
               "\"key_4\":1677721.6,"
               "\"key_5\":1677721.62500,"
               "\"key_5\":1677721.6,"
               "\"key_6\":1677721.625000,"
               "\"key_6\":1677721.6"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_minus1677721p59) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_0", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_1", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_2", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_3", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_4", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_5", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_6", -1677721.59f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-1677722,"
               "\"key_0\":-1677722,"
               "\"key_1\":-1677721.6,"
               "\"key_1\":-1677721.6,"
               "\"key_2\":-1677721.62,"
               "\"key_2\":-1677721.6,"
               "\"key_3\":-1677721.625,"
               "\"key_3\":-1677721.6,"
               "\"key_4\":-1677721.6250,"
               "\"key_4\":-1677721.6,"
               "\"key_5\":-1677721.62500,"
               "\"key_5\":-1677721.6,"
               "\"key_6\":-1677721.625000,"
               "\"key_6\":-1677721.6"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_uint32_max) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", (float)429496.7295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_0", (float)429496.7295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", (float)429496.7295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_1", (float)429496.7295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", (float)4294967.295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_2", (float)4294967.295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", (float)4294967.295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_3", (float)4294967.295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", (float)42949672.95f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_4", (float)42949672.95f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", (float)42949672.95f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_5", (float)42949672.95f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", (float)429496729.5f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_6", (float)429496729.5f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_7", (float)429496729.5f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_7", (float)429496729.5f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_8", (float)4294967295, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_8", (float)4294967295, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(
                p_gen,
                "key_9",
                (float)4294967295 * 10.0f,
                JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(
                p_gen,
                "key_9",
                (float)4294967295 * 10.0f,
                JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":429496.7188,"
               "\"key_0\":429496.7,"
               "\"key_1\":429496.719,"
               "\"key_1\":429496.7,"
               "\"key_2\":4294967.500,"
               "\"key_2\":4294968,"
               "\"key_3\":4294967.50,"
               "\"key_3\":4294968,"
               "\"key_4\":42949672.00,"
               "\"key_4\":42949670,"
               "\"key_5\":42949672.0,"
               "\"key_5\":42949670,"
               "\"key_6\":429496736.0,"
               "\"key_6\":429496800,"
               "\"key_7\":429496736,"
               "\"key_7\":429496800,"
               "\"key_8\":4294967296.0,"
               "\"key_8\":4294968000,"
               "\"key_9\":42949672960,"
               "\"key_9\":null"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_minus_uint32_max) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", -(float)429496.7295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_0", -(float)429496.7295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_1", -(float)429496.7295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_1", -(float)429496.7295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_2", -(float)4294967.295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_2", -(float)4294967.295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_3", -(float)4294967.295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_3", -(float)4294967.295f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_4", -(float)42949672.95f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_4", -(float)42949672.95f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_5", -(float)42949672.95f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_5", -(float)42949672.95f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_6", -(float)429496729.5f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_6", -(float)429496729.5f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_7", -(float)429496729.5f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_7", -(float)429496729.5f, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_8", -(float)4294967295, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_8", -(float)4294967295, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(
                p_gen,
                "key_9",
                -(float)4294967295 * 10.0f,
                JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(
                p_gen,
                "key_9",
                -(float)4294967295 * 10.0f,
                JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-429496.7188,"
               "\"key_0\":-429496.7,"
               "\"key_1\":-429496.719,"
               "\"key_1\":-429496.7,"
               "\"key_2\":-4294967.500,"
               "\"key_2\":-4294968,"
               "\"key_3\":-4294967.50,"
               "\"key_3\":-4294968,"
               "\"key_4\":-42949672.00,"
               "\"key_4\":-42949670,"
               "\"key_5\":-42949672.0,"
               "\"key_5\":-42949670,"
               "\"key_6\":-429496736.0,"
               "\"key_6\":-429496800,"
               "\"key_7\":-429496736,"
               "\"key_7\":-429496800,"
               "\"key_8\":-4294967296.0,"
               "\"key_8\":-4294968000,"
               "\"key_9\":-42949672960,"
               "\"key_9\":null"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_float_out_of_range) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, "key_0", (float)UINT32_MAX, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, "key_0", (float)UINT32_MAX, JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(
                p_gen,
                "key_1",
                (float)UINT32_MAX + 256.0f,
                JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(
                p_gen,
                "key_1",
                (float)UINT32_MAX + 256.0f,
                JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(
                p_gen,
                "key_2",
                (float)UINT32_MAX + 257.0f,
                JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(
                p_gen,
                "key_2",
                (float)UINT32_MAX + 257.0f,
                JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":4294967296,"
               "\"key_0\":4294968000,"
               "\"key_1\":4294967296,"
               "\"key_1\":4294968000,"
               "\"key_2\":4294967808,"
               "\"key_2\":null"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_0p0) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
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
        string("{"
               "\"key_0\":0,"
               "\"key_1\":0.0,"
               "\"key_2\":0.00,"
               "\"key_3\":0.000,"
               "\"key_4\":0.0000,"
               "\"key_5\":0.00000,"
               "\"key_6\":0.000000,"
               "\"key_7\":0.0000000,"
               "\"key_8\":0.00000000,"
               "\"key_9\":0.000000000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_1p0) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", 1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", 1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", 1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", 1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", 1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", 1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", 1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", 1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", 1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", 1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":1,"
               "\"key_1\":1.0,"
               "\"key_2\":1.00,"
               "\"key_3\":1.000,"
               "\"key_4\":1.0000,"
               "\"key_5\":1.00000,"
               "\"key_6\":1.000000,"
               "\"key_7\":1.0000000,"
               "\"key_8\":1.00000000,"
               "\"key_9\":1.000000000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_minus1p0) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", -1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", -1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", -1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", -1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", -1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", -1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", -1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", -1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", -1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", -1.0, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-1,"
               "\"key_1\":-1.0,"
               "\"key_2\":-1.00,"
               "\"key_3\":-1.000,"
               "\"key_4\":-1.0000,"
               "\"key_5\":-1.00000,"
               "\"key_6\":-1.000000,"
               "\"key_7\":-1.0000000,"
               "\"key_8\":-1.00000000,"
               "\"key_9\":-1.000000000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_0p01) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", 0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", 0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", 0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", 0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", 0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", 0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", 0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", 0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", 0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", 0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":0,"
               "\"key_1\":0.0,"
               "\"key_2\":0.01,"
               "\"key_3\":0.010,"
               "\"key_4\":0.0100,"
               "\"key_5\":0.01000,"
               "\"key_6\":0.010000,"
               "\"key_7\":0.0100000,"
               "\"key_8\":0.01000000,"
               "\"key_9\":0.010000000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_minus0p01) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", -0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", -0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", -0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", -0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", -0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", -0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", -0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", -0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", -0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", -0.01, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-0,"
               "\"key_1\":-0.0,"
               "\"key_2\":-0.01,"
               "\"key_3\":-0.010,"
               "\"key_4\":-0.0100,"
               "\"key_5\":-0.01000,"
               "\"key_6\":-0.010000,"
               "\"key_7\":-0.0100000,"
               "\"key_8\":-0.01000000,"
               "\"key_9\":-0.010000000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_0p06) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", 0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", 0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", 0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", 0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", 0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", 0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", 0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", 0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", 0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", 0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":0,"
               "\"key_1\":0.1,"
               "\"key_2\":0.06,"
               "\"key_3\":0.060,"
               "\"key_4\":0.0600,"
               "\"key_5\":0.06000,"
               "\"key_6\":0.060000,"
               "\"key_7\":0.0600000,"
               "\"key_8\":0.06000000,"
               "\"key_9\":0.060000000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_minus0p06) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", -0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", -0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", -0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", -0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", -0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", -0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", -0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", -0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", -0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", -0.06, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-0,"
               "\"key_1\":-0.1,"
               "\"key_2\":-0.06,"
               "\"key_3\":-0.060,"
               "\"key_4\":-0.0600,"
               "\"key_5\":-0.06000,"
               "\"key_6\":-0.060000,"
               "\"key_7\":-0.0600000,"
               "\"key_8\":-0.06000000,"
               "\"key_9\":-0.060000000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_0p555555555555) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", 0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", 0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", 0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", 0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", 0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", 0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", 0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", 0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", 0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", 0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":1,"
               "\"key_1\":0.6,"
               "\"key_2\":0.56,"
               "\"key_3\":0.556,"
               "\"key_4\":0.5556,"
               "\"key_5\":0.55556,"
               "\"key_6\":0.555556,"
               "\"key_7\":0.5555556,"
               "\"key_8\":0.55555556,"
               "\"key_9\":0.555555556"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_minus0p555555555555) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", -0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", -0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", -0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", -0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", -0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", -0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", -0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", -0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", -0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", -0.555555555555, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-1,"
               "\"key_1\":-0.6,"
               "\"key_2\":-0.56,"
               "\"key_3\":-0.556,"
               "\"key_4\":-0.5556,"
               "\"key_5\":-0.55556,"
               "\"key_6\":-0.555556,"
               "\"key_7\":-0.5555556,"
               "\"key_8\":-0.55555556,"
               "\"key_9\":-0.555555556"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", 10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", 10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", 10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", 10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", 10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", 10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", 10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", 10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", 10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", 10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":10,"
               "\"key_1\":10.3,"
               "\"key_2\":10.30,"
               "\"key_3\":10.300,"
               "\"key_4\":10.3000,"
               "\"key_5\":10.30000,"
               "\"key_6\":10.300000,"
               "\"key_7\":10.3000000,"
               "\"key_8\":10.30000000,"
               "\"key_9\":10.300000000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_minus10p3) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", -10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", -10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", -10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", -10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", -10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", -10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", -10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", -10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", -10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", -10.3, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-10,"
               "\"key_1\":-10.3,"
               "\"key_2\":-10.30,"
               "\"key_3\":-10.300,"
               "\"key_4\":-10.3000,"
               "\"key_5\":-10.30000,"
               "\"key_6\":-10.300000,"
               "\"key_7\":-10.3000000,"
               "\"key_8\":-10.30000000,"
               "\"key_9\":-10.300000000"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_max_val_without_loosing_precision) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
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
        string("{"
               "\"key_0\":9007199254740992,"
               "\"key_1\":9007199254740991,"
               "\"key_2\":-9007199254740992,"
               "\"key_3\":-9007199254740991"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_900719925474099p2) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_0", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_1", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_2", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_3", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_4", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_5", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_6", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_7", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_8", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_9", 900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":900719925474099,"
               "\"key_0\":900719925474099,"
               "\"key_1\":900719925474099.2,"
               "\"key_1\":900719925474099.2,"
               "\"key_2\":900719925474099.25,"
               "\"key_2\":900719925474099.2,"
               "\"key_3\":900719925474099.250,"
               "\"key_3\":900719925474099.2,"
               "\"key_4\":900719925474099.2500,"
               "\"key_4\":900719925474099.2,"
               "\"key_5\":900719925474099.25000,"
               "\"key_5\":900719925474099.2,"
               "\"key_6\":900719925474099.250000,"
               "\"key_6\":900719925474099.2,"
               "\"key_7\":900719925474099.2500000,"
               "\"key_7\":900719925474099.2,"
               "\"key_8\":900719925474099.25000000,"
               "\"key_8\":900719925474099.2,"
               "\"key_9\":900719925474099.250000000,"
               "\"key_9\":900719925474099.2"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_minus900719925474099p2) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_0", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_1", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_2", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_3", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_4", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_5", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_6", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_7", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_8", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_9", -900719925474099.2, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-900719925474099,"
               "\"key_0\":-900719925474099,"
               "\"key_1\":-900719925474099.2,"
               "\"key_1\":-900719925474099.2,"
               "\"key_2\":-900719925474099.25,"
               "\"key_2\":-900719925474099.2,"
               "\"key_3\":-900719925474099.250,"
               "\"key_3\":-900719925474099.2,"
               "\"key_4\":-900719925474099.2500,"
               "\"key_4\":-900719925474099.2,"
               "\"key_5\":-900719925474099.25000,"
               "\"key_5\":-900719925474099.2,"
               "\"key_6\":-900719925474099.250000,"
               "\"key_6\":-900719925474099.2,"
               "\"key_7\":-900719925474099.2500000,"
               "\"key_7\":-900719925474099.2,"
               "\"key_8\":-900719925474099.25000000,"
               "\"key_8\":-900719925474099.2,"
               "\"key_9\":-900719925474099.250000000,"
               "\"key_9\":-900719925474099.2"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_900719925474099p59) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_0", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_1", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_2", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_3", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_4", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_5", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_6", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_7", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_8", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_9", 900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":900719925474100,"
               "\"key_0\":900719925474100,"
               "\"key_1\":900719925474099.6,"
               "\"key_1\":900719925474100,"
               "\"key_2\":900719925474099.62,"
               "\"key_2\":900719925474100,"
               "\"key_3\":900719925474099.625,"
               "\"key_3\":900719925474100,"
               "\"key_4\":900719925474099.6250,"
               "\"key_4\":900719925474100,"
               "\"key_5\":900719925474099.62500,"
               "\"key_5\":900719925474100,"
               "\"key_6\":900719925474099.625000,"
               "\"key_6\":900719925474100,"
               "\"key_7\":900719925474099.6250000,"
               "\"key_7\":900719925474100,"
               "\"key_8\":900719925474099.62500000,"
               "\"key_8\":900719925474100,"
               "\"key_9\":900719925474099.625000000,"
               "\"key_9\":900719925474100"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_minus900719925474099p59) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_0", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_1", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_1", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_2", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_2", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_3", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_3", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_4", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_4", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_5", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_5", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_6", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_6", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_7", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_7", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_8", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_8", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_9", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_9", -900719925474099.59, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-900719925474100,"
               "\"key_0\":-900719925474100,"
               "\"key_1\":-900719925474099.6,"
               "\"key_1\":-900719925474100,"
               "\"key_2\":-900719925474099.62,"
               "\"key_2\":-900719925474100,"
               "\"key_3\":-900719925474099.625,"
               "\"key_3\":-900719925474100,"
               "\"key_4\":-900719925474099.6250,"
               "\"key_4\":-900719925474100,"
               "\"key_5\":-900719925474099.62500,"
               "\"key_5\":-900719925474100,"
               "\"key_6\":-900719925474099.625000,"
               "\"key_6\":-900719925474100,"
               "\"key_7\":-900719925474099.6250000,"
               "\"key_7\":-900719925474100,"
               "\"key_8\":-900719925474099.62500000,"
               "\"key_8\":-900719925474100,"
               "\"key_9\":-900719925474099.625000000,"
               "\"key_9\":-900719925474100"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_unit64_max) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_0",
                (double)1844674407370955.1615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_0",
                (double)1844674407370955.1615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_1",
                (double)1844674407370955.1615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_1",
                (double)1844674407370955.1615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_2",
                (double)18446744073709551.615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_2",
                (double)18446744073709551.615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_3",
                (double)18446744073709551.615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_3",
                (double)18446744073709551.615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_4",
                (double)184467440737095516.15,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_4",
                (double)184467440737095516.15,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_5",
                (double)184467440737095516.15,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_5",
                (double)184467440737095516.15,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_6",
                (double)1844674407370955161.5,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_6",
                (double)1844674407370955161.5,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_7",
                (double)1844674407370955161.5,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_7",
                (double)1844674407370955161.5,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_8",
                (double)18446744073709551615U,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_8",
                (double)18446744073709551615U,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_9",
                (double)18446744073709551615U * 10.0,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_9",
                (double)18446744073709551615U * 10.0,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":1844674407370955.2500,"
               "\"key_0\":1844674407370955,"
               "\"key_1\":1844674407370955.250,"
               "\"key_1\":1844674407370955,"
               "\"key_2\":18446744073709552.000,"
               "\"key_2\":18446744073709550,"
               "\"key_3\":18446744073709552.00,"
               "\"key_3\":18446744073709550,"
               "\"key_4\":184467440737095520.00,"
               "\"key_4\":184467440737095500,"
               "\"key_5\":184467440737095520.0,"
               "\"key_5\":184467440737095500,"
               "\"key_6\":1844674407370955264.0,"
               "\"key_6\":1844674407370955000,"
               "\"key_7\":1844674407370955264,"
               "\"key_7\":1844674407370955000,"
               "\"key_8\":18446744073709551616.0,"
               "\"key_8\":18446744073709550000,"
               "\"key_9\":184467440737095516160,"
               "\"key_9\":null"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_minus_unit64_max) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_0",
                -(double)1844674407370955.1615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_0",
                -(double)1844674407370955.1615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_1",
                -(double)1844674407370955.1615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_1",
                -(double)1844674407370955.1615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_2",
                -(double)18446744073709551.615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_2",
                -(double)18446744073709551.615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_3",
                -(double)18446744073709551.615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_3",
                -(double)18446744073709551.615,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_4",
                -(double)184467440737095516.15,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_4",
                -(double)184467440737095516.15,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_5",
                -(double)184467440737095516.15,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_5",
                -(double)184467440737095516.15,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_6",
                -(double)1844674407370955161.5,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_6",
                -(double)1844674407370955161.5,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_7",
                -(double)1844674407370955161.5,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_7",
                -(double)1844674407370955161.5,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_8",
                -(double)18446744073709551615U,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_8",
                -(double)18446744073709551615U,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_9",
                -(double)18446744073709551615U * 10.0,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_9",
                -(double)18446744073709551615U * 10.0,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":-1844674407370955.2500,"
               "\"key_0\":-1844674407370955,"
               "\"key_1\":-1844674407370955.250,"
               "\"key_1\":-1844674407370955,"
               "\"key_2\":-18446744073709552.000,"
               "\"key_2\":-18446744073709550,"
               "\"key_3\":-18446744073709552.00,"
               "\"key_3\":-18446744073709550,"
               "\"key_4\":-184467440737095520.00,"
               "\"key_4\":-184467440737095500,"
               "\"key_5\":-184467440737095520.0,"
               "\"key_5\":-184467440737095500,"
               "\"key_6\":-1844674407370955264.0,"
               "\"key_6\":-1844674407370955000,"
               "\"key_7\":-1844674407370955264,"
               "\"key_7\":-1844674407370955000,"
               "\"key_8\":-18446744073709551616.0,"
               "\"key_8\":-18446744073709550000,"
               "\"key_9\":-184467440737095516160,"
               "\"key_9\":null"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_json_fixed_double_out_of_range) // NOLINT
{
    const json_stream_gen_cfg_t cfg     = {};
    JsonStreamGenWrapper        wrapper = JsonStreamGenWrapper(
        &cfg,
        [](json_stream_gen_t* const p_gen, const void* const p_user_ctx) -> bool {
            (void)p_user_ctx;
            JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, "key_0", (double)UINT64_MAX, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, "key_0", (double)UINT64_MAX, JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_1",
                (double)UINT64_MAX + 2048.0,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_1",
                (double)UINT64_MAX + 2048.0,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(
                p_gen,
                "key_2",
                (double)UINT64_MAX + 2049.0,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(
                p_gen,
                "key_2",
                (double)UINT64_MAX + 2049.0,
                JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0);
            JSON_STREAM_GEN_END_GENERATOR_FUNC();
        },
        0,
        nullptr);
    ASSERT_EQ(
        string("{"
               "\"key_0\":18446744073709551616,"
               "\"key_0\":18446744073709550000,"
               "\"key_1\":18446744073709551616,"
               "\"key_1\":18446744073709550000,"
               "\"key_2\":18446744073709555712,"
               "\"key_2\":null"
               "}"),
        string(json_stream_gen_get_next_chunk(wrapper.get())));
}

TEST_F(TestJsonStreamGenU, test_generate_array1) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 260; max_chunk_size > 21; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size    = max_chunk_size,
            .max_nesting_level = 2,
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
            string("{"
                   "\"arr_empty\":[],"
                   "\"arr_str1\":[\"val1\"],"
                   "\"arr_str2\":[\"val1\\t\",\"val2\t\",null,null],"
                   "\"arr_u32\":[1,2],"
                   "\"arr_i32\":[-1,-2],"
                   "\"arr_u64\":[1],"
                   "\"arr_i64\":[-1],"
                   "\"arr_bool\":[true,false],"
                   "\"arr_null\":[null],"
                   "\"arr_float\":[1.5,1.5],"
                   "\"arr_double\":[1.5,1.5],"
                   "\"arr_fixed_float\":[1.50,1.50],"
                   "\"arr_fixed_double\":[1.50,1.50],"
                   "\"arr_hex_buf\":[\"0102AACC\",\"1112DD\"]"
                   "}"),
            json_str);
    }
}

TEST_F(TestJsonStreamGenU, test_generate_sub_array_and_subobjects) // NOLINT
{
    for (json_stream_gen_size_t max_chunk_size = 99; max_chunk_size > 18; max_chunk_size--)
    {
        json_stream_gen_cfg_t cfg = {
            .max_chunk_size    = max_chunk_size,
            .max_nesting_level = 4,
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
            string("{"
                   "\"arr_of_objects\":[{\"key1\":\"valA\"},{\"key1\":\"valB\"}],"
                   "\"arr_of_arrays\":[[\"valA\",\"valB\"],[\"valC\"],[]]"
                   "}"),
            json_str);
    }
}
