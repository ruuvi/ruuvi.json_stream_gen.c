/**
* @file example2_sub_funcs.c
* @brief This file demonstrates the usage of the json_stream_gen library for the streaming generation of JSON data.
*   It showcases how one can generate nested JSON structures by having a main generator callback function that calls
*   sub-generators. The process involves creating a `json_stream_gen_t` object,
*   initializing user data in a custom structure, and generating JSON data in chunks.
*   It takes into account named elements, arrays, and nested objects.
*   The JSON data is generated in a streaming manner, chunk by chunk, with the size of each chunk not exceeding
*   a defined maximum. This example thus exemplifies the memory-efficient, incremental generation of complex JSON data.
* @author TheSomeMan
* @date 2023-07-30
* @copyright Ruuvi Innovations Ltd, license BSD-3-Clause.
 */

#include "../include/json_stream_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Defines a user data structure used for JSON generation.
 *
 * This structure holds various types of values that will be incorporated into the JSON data,
 * including boolean, integer, floating point and string values, as well as nested structures.
 */
typedef struct user_data_t
{
    bool     val1;
    int32_t  val2;
    struct
    {
        float    val3;
        char     val4[16];
    } obj1;
    struct
    {
        uint32_t array[10];
    } obj2;
} user_data_t;

/**
 * @brief A sub-generator function for generating the JSON representation of a specific object (obj1).
 *
 * @param p_gen A pointer to the JSON generator object.
 * @param p_ctx A pointer to user-defined data used during JSON generation.
 * @return json_stream_gen_callback_result_t indicating the outcome of the JSON generation process.
 */
static JSON_STREAM_GEN_DECL_GENERATOR_SUB_FUNC(
    cb_generate_json_sub_obj1,
    json_stream_gen_t* const p_gen,
    const user_data_t* const p_ctx)
{
    JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key3", p_ctx->obj1.val3);
    JSON_STREAM_GEN_ADD_STRING(p_gen, "key4", p_ctx->obj1.val4);
    JSON_STREAM_GEN_END_GENERATOR_SUB_FUNC();
}

/**
 * @brief A sub-generator function for generating the JSON representation of a specific object (obj2).
 *
 * @param p_gen A pointer to the JSON generator object.
 * @param p_ctx A pointer to user-defined data used during JSON generation.
 * @return json_stream_gen_callback_result_t indicating the outcome of the JSON generation process.
 */
static JSON_STREAM_GEN_DECL_GENERATOR_SUB_FUNC(
    cb_generate_json_sub_obj2,
    json_stream_gen_t* const p_gen,
    const user_data_t* const p_ctx)
{
    JSON_STREAM_GEN_START_ARRAY(p_gen, "key6");
    for (size_t i = 0; i < sizeof(p_ctx->obj2.array) / sizeof(p_ctx->obj2.array[0]); ++i)
    {
        JSON_STREAM_GEN_ADD_UINT32_TO_ARRAY(p_gen, p_ctx->obj2.array[i]);
    }
    JSON_STREAM_GEN_END_ARRAY(p_gen);
    JSON_STREAM_GEN_END_GENERATOR_SUB_FUNC();
}


/**
 * @brief Main callback function for JSON generation.
 *
 * This function is called by json_stream_gen to generate JSON data. It incorporates the use of
 * sub-generator functions to create a more complex, nested JSON structure.
 *
 * @param p_gen A pointer to the JSON generator object.
 * @param p_user_ctx A pointer to user-defined data used during JSON generation.
 * @return json_stream_gen_callback_result_t indicating the outcome of the JSON generation process.
 */
json_stream_gen_callback_result_t callback_generate_json(json_stream_gen_t* const p_gen, const void* const p_user_ctx)
{
    const user_data_t* const p_ctx = (const user_data_t*)p_user_ctx;
    JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC(p_gen);
    JSON_STREAM_GEN_ADD_BOOL(p_gen, "key1", p_ctx->val1);
    JSON_STREAM_GEN_ADD_INT32(p_gen, "key2", p_ctx->val2);

    JSON_STREAM_GEN_START_OBJECT(p_gen, "obj1");
    JSON_STREAM_GEN_CALL_GENERATOR_SUB_FUNC(cb_generate_json_sub_obj1, p_gen, p_ctx);
    JSON_STREAM_GEN_END_OBJECT(p_gen); // obj1

    JSON_STREAM_GEN_START_OBJECT(p_gen, "obj2");
    {
        JSON_STREAM_GEN_CALL_GENERATOR_SUB_FUNC(cb_generate_json_sub_obj2, p_gen, p_ctx);
    }
    JSON_STREAM_GEN_END_OBJECT(p_gen); // obj2

    JSON_STREAM_GEN_END_GENERATOR_FUNC();
}

/**
 * @brief Main function where the JSON generation process takes place.
 *
 * It configures the json_stream_gen object, initializes user data and invokes the JSON generator.
 * The JSON data is generated in a streaming manner and is printed chunk by chunk.
 * Any errors encountered during the generation process are handled and printed accordingly.
 */
int main(void)
{
    // Config for json_stream_gen
    const json_stream_gen_cfg_t cfg = {
        .max_chunk_size    = 40,
        .max_nesting_level = 3,
    };

    user_data_t* p_ctx = NULL;
    // Create json_stream_gen_t object and allocate memory for the user data context
    json_stream_gen_t* p_gen = json_stream_gen_create(
        &cfg, // Pass config
        &callback_generate_json, // Callback function
        sizeof(user_data_t), // Size of user data context
        (void**)&p_ctx); // Pointer to the allocated user data context will be returned to p_ctx
    if (NULL == p_gen)
    {
        fprintf(stderr, "Memory allocation failed. Unable to create json_stream_gen object.\n");
        return EXIT_FAILURE;
    }

    // Initialize user data
    p_ctx->val1 = true;
    p_ctx->val2 = 10;
    p_ctx->obj1.val3 = 20.5;
    snprintf(p_ctx->obj1.val4, sizeof(p_ctx->obj1.val4), "Hello");
    for (size_t i = 0; i < sizeof(p_ctx->obj2.array) / sizeof(p_ctx->obj2.array[0]); ++i)
    {
        p_ctx->obj2.array[i] = 100 + i;
    }

    const size_t json_len = json_stream_gen_calc_size(p_gen);
    printf("Generating a JSON object of length %u bytes, in chunks of no more than %u bytes each.\n",
           (unsigned)json_len, cfg.max_chunk_size);

    int32_t chunk_num = 0;
    while (true)
    {
        // Generate next chunk of data
        const char* p_chunk = json_stream_gen_get_next_chunk(p_gen);
        if (NULL == p_chunk) // Check if there is any errors
        {
            fprintf(stderr, "Error while json generation (exceeding the nesting level, etc.)\n");
            return EXIT_FAILURE;
        }
        if ('\0' == *p_chunk) // Check if generation is complete
        {
            break;
        }
        chunk_num += 1;
        const size_t chunk_len = strlen(p_chunk);
        printf("Generated chunk %u (%u bytes): '%s'\n", chunk_num, (unsigned)chunk_len, p_chunk);
    }
    printf("Json generation successfully completed.\n");
}
