# Ruuvi JSON Stream Generator

## Overview

The **ruuvi.json_stream_gen.c** is a library specifically designed for embedded systems with limited RAM resources. 
It is a stream-oriented, asynchronous JSON generation tool, enabling the construction of JSON data in small, 
fixed-size chunks.

This library introduces an API that empowers users to dictate the sequence of JSON generation in a traditional, 
sequential style, whilst simultaneously managing the underlying asynchronous operational mechanism in the background.

## Usage

Here is a simple example from "examples/example1.c"

```C
/**
* @file example1.c
* @brief This file demonstrates the use of the json_stream_gen library for streaming generation of JSON data.
*   It showcases the creation and initialization of a `json_stream_gen_t` object, populating user data into
*   a custom structure, and the generation of JSON data in a structured manner. The example takes into account
*   the inclusion of named elements, arrays, and nested objects within the JSON data.
*   The JSON data is incrementally generated in chunks, with each chunk's size not exceeding a predetermined maximum.
*   This example serves as a demonstration of how the json_stream_gen library can facilitate memory-efficient,
*   incremental generation of complex JSON data.
* @author TheSomeMan
* @date 2023-07-10
* @copyright Ruuvi Innovations Ltd, license BSD-3-Clause.
 */

#include "../include/json_stream_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief A user data structure designed to hold data for JSON generation.
 *
 * This structure stores various types of data that will be included in the JSON output.
 * It includes boolean, integer, float, string, and array types.
 */
typedef struct user_data_t
{
    bool     val1;
    int32_t  val2;
    float    val3;
    char     val4[16];
    uint32_t array[10];
} user_data_t;

/**
 * @brief Callback function for JSON generation.
 *
 * This function is invoked by json_stream_gen to create JSON data. It adds various data types to the JSON structure
 * including boolean, integer, float, string, and array types.
 *
 * @param p_gen A pointer to the JSON generator object.
 * @param p_user_ctx A pointer to the user-defined data used during JSON generation.
 * @return json_stream_gen_callback_result_t indicating the outcome of the JSON generation process.
 */
json_stream_gen_callback_result_t callback_generate_json(json_stream_gen_t* const p_gen, const void* const p_user_ctx)
{
    const user_data_t* const p_ctx = (const user_data_t*)p_user_ctx;
    JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC(p_gen);
    JSON_STREAM_GEN_ADD_BOOL(p_gen, "key1", p_ctx->val1);
    JSON_STREAM_GEN_ADD_INT32(p_gen, "key2", p_ctx->val2);
    JSON_STREAM_GEN_ADD_FLOAT(p_gen, "key3", p_ctx->val3);
    JSON_STREAM_GEN_ADD_STRING(p_gen, "key4", p_ctx->val4);
    JSON_STREAM_GEN_START_OBJECT(p_gen, "key5");
    {
        JSON_STREAM_GEN_START_ARRAY(p_gen, "key6");
        for (size_t i = 0; i < sizeof(p_ctx->array) / sizeof(p_ctx->array[0]); ++i)
        {
            JSON_STREAM_GEN_ADD_UINT32_TO_ARRAY(p_gen, p_ctx->array[i]);
        }
        JSON_STREAM_GEN_END_ARRAY(p_gen);
    }
    JSON_STREAM_GEN_END_OBJECT(p_gen);
    JSON_STREAM_GEN_END_GENERATOR_FUNC();
}

/**
 * @brief The main function orchestrating the JSON generation process.
 * 
 * This function sets up the configuration for the json_stream_gen object, initializes user data, and triggers the JSON 
 * generator. The JSON data is generated in a streaming manner and printed out in chunks. Errors encountered during the 
 * generation process are handled and printed to stderr.
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
    p_ctx->val3 = 20.5;
    snprintf(p_ctx->val4, sizeof(p_ctx->val4), "Hello");
    for (size_t i = 0; i < sizeof(p_ctx->array) / sizeof(p_ctx->array[0]); ++i)
    {
        p_ctx->array[i] = 100 + i;
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
```

You can use the following command to compile it from the 'examples' folder:
```shell
gcc example1.c ../src/json_stream_gen.c -I ../include -lm -o example1
```

'example1' generates following output:
```text
Generating a JSON object of length 108 bytes, in chunks of no more than 40 bytes each.
Generated chunk 1 (34 bytes): '{"key1":true,"key2":10,"key3":20.5'
Generated chunk 2 (39 bytes): ',"key4":"Hello","key5":{"key6":[100,101'
Generated chunk 3 (35 bytes): ',102,103,104,105,106,107,108,109]}}'
Json generation successfully completed.
```

This is another example from "examples/example2_sub_funcs.c" that demonstrates using of sub-generators:

```C
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
```

You can use the following command to compile it from the 'examples' folder:
```shell
gcc example2_sub_funcs.c ../src/json_stream_gen.c -I ../include -lm -o example2_sub_funcs
```

'example2_sub_funcs' generates following output:
```text
Generating a JSON object of length 117 bytes, in chunks of no more than 40 bytes each.
Generated chunk 1 (31 bytes): '{"key1":true,"key2":10,"obj1":{'
Generated chunk 2 (36 bytes): '"key3":20.5,"key4":"Hello"},"obj2":{'
Generated chunk 3 (39 bytes): '"key6":[100,101,102,103,104,105,106,107'
Generated chunk 4 (11 bytes): ',108,109]}}'
Json generation successfully completed.
```
