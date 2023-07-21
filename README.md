# Ruuvi JSON Stream Generator

## Overview

The **ruuvi.json_stream_gen.c** is a library specifically designed for embedded systems with limited RAM resources. 
It is a stream-oriented, asynchronous JSON generation tool, enabling the construction of JSON data in small, 
fixed-size chunks.

This library introduces an API that empowers users to dictate the sequence of JSON generation in a traditional, 
sequential style, whilst simultaneously managing the underlying asynchronous operational mechanism in the background.

## Usage

Here is an example from "examples/example1.c"

```C
#include "../include/json_stream_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// user_data_t: Struct to hold data for JSON generation.
typedef struct user_data_t
{
    bool     val1;
    int32_t  val2;
    float    val3;
    char     val4[16];
    uint32_t array[10];
} user_data_t;

// callback_generate_json: Called by json_stream_gen to generate JSON data.
bool callback_generate_json(json_stream_gen_t* const p_gen, const void* p_user_ctx)
{
    const user_data_t* const p_ctx = (const user_data_t*)p_user_ctx;
    JSON_STREAM_GEN_BEGIN_GENERATOR_FUNC();
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
