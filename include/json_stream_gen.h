/**
 * @file json_stream_gen.h
 * @author TheSomeMan
 * @date 2023-07-10
 * @copyright Ruuvi Innovations Ltd, license BSD-3-Clause.
 *
 * @brief This is a library specifically designed for embedded systems with limited RAM resources.
 *  It is a stream-oriented, asynchronous JSON generation tool, enabling the construction of JSON data in small,
 *  fixed-size chunks.
 *  This library introduces an API that empowers users to dictate the sequence of JSON generation in a traditional,
 *  sequential style, whilst simultaneously managing the underlying asynchronous operational mechanism in the
 * background.
 */

#ifndef JSON_STREAM_GEN_H
#define JSON_STREAM_GEN_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JSON_STREAM_GEN_CFG_DEFAULT_MAX_CHUNK_SIZE    (1024)
#define JSON_STREAM_GEN_CFG_MIN_CHUNK_SIZE            (8)
#define JSON_STREAM_GEN_CFG_DEFAULT_MAX_NESTING_LEVEL (5U)
#define JSON_STREAM_GEN_CFG_DEFAULT_INDENTATION       (2U)
#define JSON_STREAM_GEN_CFG_DEFAULT_INDENTATION_MARK  (' ')

/**
 * @brief json_stream_gen_t is a struct that defines JSON generator object.
 */
typedef struct json_stream_gen_t json_stream_gen_t;

/**
 * @brief json_stream_gen_cb_generate_next_t is a JSON generator callback function type that generates the next chunk of
 * JSON.
 * @param p_gen is a pointer to the JSON generator object.
 * @param p_user_ctx is a pointer to user-defined data used during JSON generation.
 * @return returns true if the generation of the chunk is successful, false otherwise.
 */
typedef bool (*json_stream_gen_cb_generate_next_t)(json_stream_gen_t* const p_gen, const void* const p_user_ctx);

/**
 * @brief json_stream_gen_size_t is a type definition for the size of JSON stream generator's output.
 */
typedef int32_t json_stream_gen_size_t;

/**
 * @brief A type definition for a function pointer that represents a malloc-like function.
 *
 * @param size The number of bytes to allocate.
 * @return A pointer to the allocated memory, or NULL if the allocation fails.
 */
typedef void* (*json_stream_gen_malloc_t)(size_t size);

/**
 * @brief A type definition for a function pointer that represents a free-like function.
 *
 * @param ptr The pointer to the memory block to be freed.
 */
typedef void (*json_stream_gen_free_t)(void* ptr);

/**
 * @brief json_stream_gen_cfg_t is a struct for configuration settings of the JSON stream generator.
 */
typedef struct json_stream_gen_cfg_t
{
    json_stream_gen_size_t   max_chunk_size;      ///< Maximum size for each JSON data chunk (in bytes).
    bool                     flag_formatted_json; ///< True enables pretty printing (formatted JSON).
    char                     indentation_mark;    ///< Character for indentation in pretty printing (' ' or '\t').
    uint32_t                 indentation;         ///< Number of indentation characters per level in pretty printing.
    uint32_t                 max_nesting_level;   ///< Maximum depth for nested JSON elements (arrays, objects).
    json_stream_gen_malloc_t p_malloc;            ///< Function pointer to replace standard 'malloc'.
    json_stream_gen_free_t   p_free;              ///< Function pointer to replace standard 'free'.
} json_stream_gen_cfg_t;

typedef int json_stream_gen_ieee754_precision_t;
typedef int json_stream_gen_num_decimals_t;

/**
 * @brief Creates a new instance of json_stream_gen_t.
 * @param p_cfg is a pointer to a json_stream_gen_cfg_t configuration object.
 * @param cb_gen_next is the callback function that will be used to generate JSON.
 * @param ctx_size is the size of the user data context.
 * @param p_p_ctx is a pointer to a pointer for storing user data context.
 * @return Returns a pointer to a new instance of json_stream_gen_t.
 */
json_stream_gen_t*
json_stream_gen_create(
    const json_stream_gen_cfg_t* const p_cfg,
    json_stream_gen_cb_generate_next_t cb_gen_next,
    const size_t                       ctx_size,
    void**                             p_p_ctx);

/**
 * @brief Deletes an instance of json_stream_gen_t and frees the memory.
 * @param p_p_gen is a pointer to a pointer to a json_stream_gen_t instance.
 */
void
json_stream_gen_delete(json_stream_gen_t** p_p_gen);

/**
 * @brief Generates the next chunk of JSON data.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @return Returns a pointer to the next JSON chunk as a null-terminated string.
 */
const char*
json_stream_gen_get_next_chunk(json_stream_gen_t* const p_gen);

/**
 * @brief Calculates the total size of JSON data that will be generated.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @return Returns the size of the JSON data.
 */
json_stream_gen_size_t
json_stream_gen_calc_size(json_stream_gen_t* const p_gen);

/**
 * @brief Resets the json_stream_gen_t instance to its initial state.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 */
void
json_stream_gen_reset(json_stream_gen_t* const p_gen);

/**
 * @brief Retrieves the current stage of JSON generation.
 * @note This function if for internal usage only (in macro).
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @return Returns the current stage of JSON generation.
 */
int32_t
json_stream_gen_get_stage_internal(const json_stream_gen_t* const p_gen);

/**
 * @brief Increments the current stage of JSON generation.
 * @note This function if for internal usage only (in macro).
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 */
void
json_stream_gen_inc_stage_internal(json_stream_gen_t* const p_gen);

/**
 * @brief A macro that provides default configuration for a json_stream_gen_cfg_t instance.
 */
#define JSON_STREAM_GEN_CFG_DEFAULT() \
    (json_stream_gen_cfg_t) \
    { \
        .max_chunk_size = JSON_STREAM_GEN_CFG_DEFAULT_MAX_CHUNK_SIZE, .flag_formatted_json = false, \
        .indentation_mark  = JSON_STREAM_GEN_CFG_DEFAULT_INDENTATION_MARK, \
        .max_nesting_level = JSON_STREAM_GEN_CFG_DEFAULT_MAX_NESTING_LEVEL, \
        .indentation = JSON_STREAM_GEN_CFG_DEFAULT_INDENTATION, .p_malloc = &malloc, .p_free = &free, \
    }

/**
 * @brief Macro to mark the start of a JSON generator callback function.
 */
#define JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC() int32_t json_stream_gen_step = 0

/**
 * @brief Macro to mark the end of a JSON generator callbalc function.
 */
#define JSON_STREAM_GEN_END_GENERATOR_FUNC() return true

/**
 * @brief Macro to start a new JSON object.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param obj_name is the name of the new JSON object.
 */
#define JSON_STREAM_GEN_START_OBJECT(p_gen, obj_name) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_start_object(p_gen, obj_name)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON object to an existing JSON array.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 */
#define JSON_STREAM_GEN_ADD_OBJECT_TO_ARRAY(p_gen) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_start_object(p_gen, NULL)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to mark the end of a JSON object.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 */
#define JSON_STREAM_GEN_END_OBJECT(p_gen) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_end_object(p_gen)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to start a new JSON array.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param name is the name of the new JSON array.
 */
#define JSON_STREAM_GEN_START_ARRAY(p_gen, name) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_start_array(p_gen, name)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON array to an existing JSON array.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 */
#define JSON_STREAM_GEN_ADD_ARRAY_TO_ARRAY(p_gen) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_start_array(p_gen, NULL)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to mark the end of a JSON array.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 */
#define JSON_STREAM_GEN_END_ARRAY(p_gen) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_end_array(p_gen)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON string to an existing JSON object.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param key is the key of the new JSON string.
 * @param val is the value of the new JSON string.
 */
#define JSON_STREAM_GEN_ADD_STRING(p_gen, key, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_string(p_gen, key, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON string to an existing JSON array.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param val is the value of the new JSON string (special characters like '\t' or '\n' will be escaped).
 */
#define JSON_STREAM_GEN_ADD_STRING_TO_ARRAY(p_gen, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_string(p_gen, NULL, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new raw JSON string to an existing JSON object.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param key is the key of the new raw JSON string.
 * @param val is the value of the new raw JSON string.
 */
#define JSON_STREAM_GEN_ADD_RAW_STRING(p_gen, key, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_raw_string(p_gen, key, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new raw JSON string to an existing JSON array.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param val is the value of the new raw JSON string.
 */
#define JSON_STREAM_GEN_ADD_RAW_STRING_TO_ARRAY(p_gen, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_raw_string(p_gen, NULL, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON int32 to an existing JSON object.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param key is the key of the new JSON int32.
 * @param val is the value of the new JSON int32.
 */
#define JSON_STREAM_GEN_ADD_INT32(p_gen, key, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_int32(p_gen, key, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON int32 to an existing JSON array.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param val is the value of the new JSON int32.
 */
#define JSON_STREAM_GEN_ADD_INT32_TO_ARRAY(p_gen, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_int32(p_gen, NULL, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON uint32 to an existing JSON object.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param key is the key of the new JSON uint32.
 * @param val is the value of the new JSON uint32.
 */
#define JSON_STREAM_GEN_ADD_UINT32(p_gen, key, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_uint32(p_gen, key, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON uint32 to an existing JSON array.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param val is the value of the new JSON uint32.
 */
#define JSON_STREAM_GEN_ADD_UINT32_TO_ARRAY(p_gen, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_uint32(p_gen, NULL, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON int64 to an existing JSON object.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param key is the key of the new JSON int64.
 * @param val is the value of the new JSON int64.
 */
#define JSON_STREAM_GEN_ADD_INT64(p_gen, key, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_int64(p_gen, key, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON int64 to an existing JSON array.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param val is the value of the new JSON int64.
 */
#define JSON_STREAM_GEN_ADD_INT64_TO_ARRAY(p_gen, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_int64(p_gen, NULL, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON uint64 to an existing JSON object.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param key is the key of the new JSON uint64.
 * @param val is the value of the new JSON uint64.
 */
#define JSON_STREAM_GEN_ADD_UINT64(p_gen, key, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_uint64(p_gen, key, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON uint64 to an existing JSON array.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param val is the value of the new JSON uint64.
 */
#define JSON_STREAM_GEN_ADD_UINT64_TO_ARRAY(p_gen, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_uint64(p_gen, NULL, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON bool to an existing JSON object.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param key is the key of the new JSON bool.
 * @param val is the value of the new JSON bool.
 */
#define JSON_STREAM_GEN_ADD_BOOL(p_gen, key, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_bool(p_gen, key, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON bool to an existing JSON array.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param val is the value of the new JSON bool.
 */
#define JSON_STREAM_GEN_ADD_BOOL_TO_ARRAY(p_gen, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_bool(p_gen, NULL, val)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON null to an existing JSON object.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 * @param key is the key of the new JSON null.
 */
#define JSON_STREAM_GEN_ADD_NULL(p_gen, key) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_null(p_gen, key)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Macro to add a new JSON null to an existing JSON array.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen is a pointer to a json_stream_gen_t instance.
 */
#define JSON_STREAM_GEN_ADD_NULL_TO_ARRAY(p_gen) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_null(p_gen, NULL)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Add a float value to a JSON stream generator.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen    The JSON stream generator instance.
 * @param key      The key string of the value to be added.
 * @param val      The float value to be added.
 */
#define JSON_STREAM_GEN_ADD_FLOAT(p_gen, key, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_float(p_gen, key, val, -1)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Add a float value to an array in the JSON stream generator.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen    The JSON stream generator instance.
 * @param val      The float value to be added.
 */
#define JSON_STREAM_GEN_ADD_FLOAT_TO_ARRAY(p_gen, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_float(p_gen, NULL, val, -1)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Add a float value with a precision to a JSON stream generator.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen      The JSON stream generator instance.
 * @param key        The key string of the value to be added.
 * @param val        The float value to be added.
 * @param precision  The precision of the float value.
 */
#define JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION(p_gen, key, val, precision) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_float(p_gen, key, val, precision)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Add a float value with a precision to an array in the JSON stream generator.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen      The JSON stream generator instance.
 * @param val        The float value to be added.
 * @param precision  The precision of the float value.
 */
#define JSON_STREAM_GEN_ADD_FLOAT_WITH_PRECISION_TO_ARRAY(p_gen, val, precision) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_float(p_gen, NULL, val, precision)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Add a double value to a JSON stream generator.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen    The JSON stream generator instance.
 * @param key      The key string of the value to be added.
 * @param val      The double value to be added.
 */
#define JSON_STREAM_GEN_ADD_DOUBLE(p_gen, key, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_double(p_gen, key, val, -1)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Add a double value to an array in the JSON stream generator.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen    The JSON stream generator instance.
 * @param val      The double value to be added.
 */
#define JSON_STREAM_GEN_ADD_DOUBLE_TO_ARRAY(p_gen, val) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_double(p_gen, NULL, val, -1)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Add a double value with a precision to a JSON stream generator.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen      The JSON stream generator instance.
 * @param key        The key string of the value to be added.
 * @param val        The double value to be added.
 * @param precision  The precision of the double value.
 */
#define JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION(p_gen, key, val, precision) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_double(p_gen, key, val, precision)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Add a double value with a precision to an array in the JSON stream generator.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen      The JSON stream generator instance.
 * @param val        The double value to be added.
 * @param precision  The precision of the double value.
 */
#define JSON_STREAM_GEN_ADD_DOUBLE_WITH_PRECISION_TO_ARRAY(p_gen, val, precision) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_double(p_gen, NULL, val, precision)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Enumeration of the possible number of decimal points for a float value in a JSON stream generator.
 */
typedef enum json_stream_gen_num_decimals_float_e
{
    JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_0 = 0,
    JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_1 = 1,
    JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_2 = 2,
    JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_3 = 3,
    JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_4 = 4,
    JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_5 = 5,
    JSON_STREAM_GEN_NUM_DECIMALS_FLOAT_6 = 6,
} json_stream_gen_num_decimals_float_e;

/**
 * @brief Add a float value as a fixed point to a JSON stream generator.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen         The JSON stream generator instance.
 * @param key           The key string of the value to be added.
 * @param val           The float value to be added.
 * @param num_decimals  The number of decimal points for the float value.
 */
#define JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT(p_gen, key, val, num_decimals) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_float_fixed_point(p_gen, key, val, num_decimals)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Add a float value as a fixed point to an array in the JSON stream generator.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen         The JSON stream generator instance.
 * @param val           The float value to be added.
 * @param num_decimals  The number of decimal points for the float value.
 */
#define JSON_STREAM_GEN_ADD_FLOAT_FIXED_POINT_TO_ARRAY(p_gen, val, num_decimals) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_float_fixed_point(p_gen, NULL, val, num_decimals)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @def JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, key, val, num_decimals)
 *
 * @brief A macro that adds a floating-point number to the JSON stream in a limited fixed point notation.
 *
 * @note This macro is a part of the generator's state machine. It checks the current generator stage,
 * and if the stage matches, it calls the function to add the floating-point number to the JSON stream.
 * It is expected to be used inside the main JSON generation function, that controls the generation flow.
 *
 * @param p_gen         Pointer to the JSON stream generator.
 * @param key           Key of the JSON item.
 * @param val           Floating-point value to be added.
 * @param num_decimals  Number of decimal places in the fixed point notation.
 */
#define JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT(p_gen, key, val, num_decimals) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_float_limited_fixed_point(p_gen, key, val, num_decimals)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @def JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT_TO_ARRAY(p_gen, val, num_decimals)
 *
 * @brief A macro that adds a floating-point number to a JSON array in the stream in a limited fixed point notation.
 *
 * @note This macro is a part of the generator's state machine. It checks the current generator stage,
 * and if the stage matches, it calls the function to add the floating-point number to the JSON array.
 * It is expected to be used inside the main JSON generation function, that controls the generation flow.
 *
 * @param p_gen         Pointer to the JSON stream generator.
 * @param val           Floating-point value to be added.
 * @param num_decimals  Number of decimal places in the fixed point notation.
 */
#define JSON_STREAM_GEN_ADD_FLOAT_LIMITED_FIXED_POINT_TO_ARRAY(p_gen, val, num_decimals) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_float_limited_fixed_point(p_gen, NULL, val, num_decimals)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Enumeration of the possible number of decimal points for a double value in a JSON stream generator.
 */
typedef enum json_stream_gen_num_decimals_double_e
{
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_0  = 0,
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_1  = 1,
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_2  = 2,
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_3  = 3,
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_4  = 4,
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_5  = 5,
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_6  = 6,
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_7  = 7,
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_8  = 8,
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_9  = 9,
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_10 = 10,
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_11 = 11,
    JSON_STREAM_GEN_NUM_DECIMALS_DOUBLE_12 = 12,
} json_stream_gen_num_decimals_double_e;

/**
 * @brief Add a double value as a fixed point to a JSON stream generator.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen         The JSON stream generator instance.
 * @param key           The key string of the value to be added.
 * @param val           The double value to be added.
 * @param num_decimals  The number of decimal points for the double value.
 */
#define JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT(p_gen, key, val, num_decimals) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_double_fixed_point(p_gen, key, val, num_decimals)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Add a double value as a fixed point to an array in the JSON stream generator.
 * @note This macro should be used only inside a JSON generator callback function.
 * @param p_gen         The JSON stream generator instance.
 * @param val           The double value to be added.
 * @param num_decimals  The number of decimal points for the double value.
 */
#define JSON_STREAM_GEN_ADD_DOUBLE_FIXED_POINT_TO_ARRAY(p_gen, val, num_decimals) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_double_fixed_point(p_gen, NULL, val, num_decimals)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @def JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, key, val, num_decimals)
 *
 * @brief A macro that adds a floating-point(double) number to the JSON stream in a limited fixed point notation.
 *
 * @note This macro is a part of the generator's state machine. It checks the current generator stage,
 * and if the stage matches, it calls the function to add the floating-point number to the JSON stream.
 * It is expected to be used inside the main JSON generation function, that controls the generation flow.
 *
 * @param p_gen         Pointer to the JSON stream generator.
 * @param key           Key of the JSON item.
 * @param val           Floating-point(double) value to be added.
 * @param num_decimals  Number of decimal places in the fixed point notation.
 */
#define JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT(p_gen, key, val, num_decimals) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_double_limited_fixed_point(p_gen, key, val, num_decimals)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @def JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT_TO_ARRAY(p_gen, val, num_decimals)
 *
 * @brief A macro that adds a floating-point(double) number to a JSON array in the stream in a limited fixed point
 * notation.
 *
 * @note This macro is a part of the generator's state machine. It checks the current generator stage,
 * and if the stage matches, it calls the function to add the floating-point number to the JSON array.
 * It is expected to be used inside the main JSON generation function, that controls the generation flow.
 *
 * @param p_gen         Pointer to the JSON stream generator.
 * @param val           Floating-point(double) value to be added.
 * @param num_decimals  Number of decimal places in the fixed point notation.
 */
#define JSON_STREAM_GEN_ADD_DOUBLE_LIMITED_FIXED_POINT_TO_ARRAY(p_gen, val, num_decimals) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_double_limited_fixed_point(p_gen, NULL, val, num_decimals)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief A macro that adds a hexadecimal buffer to the JSON stream.
 *
 * This macro is a part of the generator's state machine. It checks the current generator stage,
 * and if the stage matches, it calls the function to add the hexadecimal buffer to the JSON stream.
 * It is expected to be used inside the main JSON generation function, that controls the generation flow.
 *
 * @param p_gen   Pointer to the JSON stream generator.
 * @param key     Key of the JSON item.
 * @param p_buf   Pointer to the buffer to be added.
 * @param buf_len Length of the buffer.
 */
#define JSON_STREAM_GEN_ADD_HEX_BUF(p_gen, key, p_buf, buf_len) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_hex_buf(p_gen, key, p_buf, buf_len)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief A macro that adds a hexadecimal buffer to a JSON array in the stream.
 *
 * This macro is a part of the generator's state machine. It checks the current generator stage,
 * and if the stage matches, it calls the function to add the hexadecimal buffer to the JSON array.
 * It is expected to be used inside the main JSON generation function, that controls the generation flow.
 *
 * @param p_gen   Pointer to the JSON stream generator.
 * @param p_buf   Pointer to the buffer to be added.
 * @param buf_len Length of the buffer.
 */
#define JSON_STREAM_GEN_ADD_HEX_BUF_TO_ARRAY(p_gen, p_buf, buf_len) \
    do \
    { \
        if (json_stream_gen_step++ == json_stream_gen_get_stage_internal(p_gen)) \
        { \
            if (!json_stream_gen_add_hex_buf(p_gen, NULL, p_buf, buf_len)) \
            { \
                return false; \
            } \
            json_stream_gen_inc_stage_internal(p_gen); \
            return true; \
        } \
    } while (0)

/**
 * @brief Starts a new sub-object. If a name is provided, the object is added as a member to the current object,
 * otherwise, it is added as an unnamed object to the current array.
 * @details The function fails if the current nesting level has reached the configured maximum.
 * @param p_gen A pointer to the JSON stream generator.
 * @param p_name The name of the object. Can be NULL for unnamed objects (e.g., in arrays).
 * @return Returns true if the sub-object was started successfully; otherwise, it returns false.
 */
bool
json_stream_gen_start_object(json_stream_gen_t* const p_gen, const char* const p_name);

/**
 * @brief Ends the last started sub-object.
 * @details The function fails if there are no started objects left to end.
 * @param p_gen A pointer to the JSON stream generator.
 * @return Returns true if the object was ended successfully; otherwise, it returns false.
 */
bool
json_stream_gen_end_object(json_stream_gen_t* const p_gen);

/**
 * @brief Starts a new JSON array. If a name is provided, the array is added as a member to the current object,
 * otherwise, it is added as an unnamed array to the current array.
 * @details The function fails if the current nesting level has reached the configured maximum.
 * @param p_gen A pointer to the JSON stream generator.
 * @param p_name The name of the array. Can be NULL for unnamed arrays (e.g., in arrays).
 * @return Returns true if the array was started successfully; otherwise, it returns false.
 */
bool
json_stream_gen_start_array(json_stream_gen_t* const p_gen, const char* const p_name);

/**
 * @brief Ends the last started JSON array.
 * @details The function fails if there are no started arrays left to end.
 * @param p_gen A pointer to the JSON stream generator.
 * @return Returns true if the array was ended successfully; otherwise, it returns false.
 */
bool
json_stream_gen_end_array(json_stream_gen_t* const p_gen);

/**
 * @brief Adds a JSON string.
 * @details If the value is NULL, it adds a null. If the string doesn't need escaping, it adds the string directly.
 * Otherwise, it adds the string with characters escaped.
 * @param p_gen Pointer to the JSON stream generator.
 * @param p_name Name of the string to be added. If this is NULL, the string is added without a name (into an array).
 * @param p_val Value of the string to be added.
 * @return Returns true if the string was added successfully; otherwise, returns false.
 */
bool
json_stream_gen_add_string(json_stream_gen_t* const p_gen, const char* const p_name, const char* const p_val);

/**
 * @brief Adds a raw JSON string.
 * @details The string is added without any escaping. If the value is NULL, it adds a null.
 * @param p_gen Pointer to the JSON stream generator.
 * @param p_name Name of the string to be added. If this is NULL, the string is added without a name (into an array).
 * @param p_val Value of the string to be added.
 * @return Returns true if the string was added successfully; otherwise, returns false.
 */
bool
json_stream_gen_add_raw_string(json_stream_gen_t* const p_gen, const char* const p_name, const char* const p_val);

/**
 * @brief Adds a 32-bit integer to the JSON stream.
 * @details This function adds a 32-bit integer to the JSON stream.
 * @param p_gen Pointer to the JSON stream generator.
 * @param p_name Name of the integer to be added. If this is `NULL`, the integer is added without a name (into an
 * array).
 * @param val The 32-bit integer value to be added to the JSON stream.
 * @return Returns `true` if the value was successfully added to the JSON stream; otherwise, returns `false`.
 */
bool
json_stream_gen_add_int32(json_stream_gen_t* const p_gen, const char* const p_name, const int32_t val);

/**
 * @brief Adds a 32-bit unsigned integer to the JSON stream.
 * @details This function adds a 32-bit unsigned integer to the JSON stream.
 * @param p_gen Pointer to the JSON stream generator.
 * @param p_name Name of the integer to be added. If this is `NULL`, the integer is added without a name (into an
 * array).
 * @param val The 32-bit unsigned integer value to be added to the JSON stream.
 * @return Returns `true` if the value was successfully added to the JSON stream; otherwise, returns `false`.
 */
bool
json_stream_gen_add_uint32(json_stream_gen_t* const p_gen, const char* const p_name, const uint32_t val);

/**
 * @brief Adds a 64-bit integer to the JSON stream.
 * @details This function adds a 64-bit integer to the JSON stream.
 * @param p_gen Pointer to the JSON stream generator.
 * @param p_name Name of the integer to be added. If this is `NULL`, the integer is added without a name (into an
 * array).
 * @param val The 64-bit integer value to be added to the JSON stream.
 * @return Returns `true` if the value was successfully added to the JSON stream; otherwise, returns `false`.
 */
bool
json_stream_gen_add_int64(json_stream_gen_t* const p_gen, const char* const p_name, const int64_t val);

/**
 * @brief Adds a 64-bit unsigned integer to the JSON stream.
 * @details This function adds a 64-bit unsigned integer to the JSON stream.
 * @param p_gen Pointer to the JSON stream generator.
 * @param p_name Name of the integer to be added. If this is `NULL`, the integer is added without a name (into an
 * array).
 * @param val The 64-bit unsigned integer value to be added to the JSON stream.
 * @return Returns `true` if the value was successfully added to the JSON stream; otherwise, returns `false`.
 */
bool
json_stream_gen_add_uint64(json_stream_gen_t* const p_gen, const char* const p_name, const uint64_t val);

/**
 * @brief Adds a boolean to the JSON stream.
 * @details This function adds a boolean value to the JSON stream.
 * @param p_gen Pointer to the JSON stream generator.
 * @param p_name Name of the boolean to be added. If this is `NULL`, the boolean is added without a name (into an
 * array).
 * @param val The boolean value to be added to the JSON stream.
 * @return Returns `true` if the boolean was successfully added to the JSON stream; otherwise, returns `false`.
 */
bool
json_stream_gen_add_bool(json_stream_gen_t* const p_gen, const char* const p_name, const bool val);

/**
 * @brief Adds a null to the JSON stream.
 * @details This function adds a null value to the JSON stream.
 * @param p_gen Pointer to the JSON stream generator.
 * @param p_name Name of the null value to be added. If this is `NULL`, the null value is added without a name (into an
 * array).
 * @return Returns `true` if the null value was successfully added to the JSON stream; otherwise, returns `false`.
 */
bool
json_stream_gen_add_null(json_stream_gen_t* const p_gen, const char* const p_name);

/**
 * @brief Function to add a float to a JSON stream generator.
 *
 * @param p_gen          The JSON stream generator instance.
 * @param p_name         The key string of the value to be added.
 * @param val            The float value to be added.
 * @param precision      The precision of the float value.
 *
 * @return true if the float value was successfully added, false otherwise.
 */
bool
json_stream_gen_add_float(
    json_stream_gen_t* const            p_gen,
    const char* const                   p_name,
    const float                         val,
    json_stream_gen_ieee754_precision_t precision);

/**
 * @brief Function to add a double to a JSON stream generator.
 *
 * @param p_gen          The JSON stream generator instance.
 * @param p_name         The key string of the value to be added.
 * @param val            The double value to be added.
 * @param precision      The precision of the double value.
 *
 * @return true if the double value was successfully added, false otherwise.
 */
bool
json_stream_gen_add_double(
    json_stream_gen_t* const            p_gen,
    const char* const                   p_name,
    const double                        val,
    json_stream_gen_ieee754_precision_t precision);

/**
 * @brief Function to add a float as a fixed point to a JSON stream generator.
 *
 * @param p_gen          The JSON stream generator instance.
 * @param p_name         The key string of the value to be added.
 * @param val            The float value to be added.
 * @param num_decimals   The number of decimal points for the float value.
 *
 * @return true if the float value was successfully added, false otherwise.
 */
bool
json_stream_gen_add_float_fixed_point(
    json_stream_gen_t* const             p_gen,
    const char* const                    p_name,
    const float                          val,
    json_stream_gen_num_decimals_float_e num_decimals);

/**
 * @brief Function to add a double as a fixed point to a JSON stream generator.
 *
 * @param p_gen          The JSON stream generator instance.
 * @param p_name         The key string of the value to be added.
 * @param val            The double value to be added.
 * @param num_decimals   The number of decimal points for the double value.
 *
 * @return true if the double value was successfully added, false otherwise.
 */
bool
json_stream_gen_add_double_fixed_point(
    json_stream_gen_t* const              p_gen,
    const char* const                     p_name,
    const double                          val,
    json_stream_gen_num_decimals_double_e num_decimals);

/**
 * @brief Adds a floating-point number to the JSON stream in a limited fixed point notation.
 *
 * If the provided floating-point value is a special value (NaN or Inf), it adds a null.
 * If the value is too large to be represented accurately in a fixed point notation,
 * it also adds a null. Otherwise, it adds the floating-point number in a limited fixed point notation.
 *
 * @param p_gen         Pointer to the JSON stream generator.
 * @param p_name        Name of the JSON item.
 * @param val           Floating-point value to be added.
 * @param num_decimals  Number of decimal places in the fixed point notation.
 *
 * @return true if the floating-point number was successfully added, false otherwise.
 */
bool
json_stream_gen_add_float_limited_fixed_point(
    json_stream_gen_t* const             p_gen,
    const char* const                    p_name,
    const float                          val,
    json_stream_gen_num_decimals_float_e num_decimals);

/**
 * @brief Adds a floating-point(double) number to the JSON stream in a limited fixed point notation.
 *
 * If the provided floating-point value is a special value (NaN or Inf), it adds a null.
 * If the value is too large to be represented accurately in a fixed point notation,
 * it also adds a null. Otherwise, it adds the floating-point number in a limited fixed point notation.
 *
 * @param p_gen         Pointer to the JSON stream generator.
 * @param p_name        Name of the JSON item.
 * @param val           Floating-point(double) value to be added.
 * @param num_decimals  Number of decimal places in the fixed point notation.
 *
 * @return true if the floating-point number was successfully added, false otherwise.
 */
bool
json_stream_gen_add_double_limited_fixed_point(
    json_stream_gen_t* const              p_gen,
    const char* const                     p_name,
    const double                          val,
    json_stream_gen_num_decimals_double_e num_decimals);

/**
 * @brief Adds a hexadecimal buffer to the JSON stream.
 *
 * This function adds a binary buffer to the JSON stream, where the buffer is represented as a string of hexadecimal
 * digits. If the buffer cannot be added due to size limitations, it resets the generator state to the state before the
 * function call.
 *
 * @param p_gen   Pointer to the JSON stream generator.
 * @param p_name  Name of the JSON item.
 * @param p_buf   Pointer to the buffer to be added.
 * @param buf_len Length of the buffer.
 *
 * @return true if the buffer was successfully added, false otherwise.
 */
bool
json_stream_gen_add_hex_buf(
    json_stream_gen_t* const p_gen,
    const char* const        p_name,
    const uint8_t* const     p_buf,
    size_t                   buf_len);

#ifdef __cplusplus
}
#endif

#endif // JSON_STREAM_GEN_H
