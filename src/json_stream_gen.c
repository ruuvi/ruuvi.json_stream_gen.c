/**
 * @file json_stream_gen.c
 * @author TheSomeMan
 * @date 2023-07-10
 * @copyright Ruuvi Innovations Ltd, license BSD-3-Clause.
 */

#include "json_stream_gen.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <float.h>
#include <locale.h>

#define JSON_STREAM_GEN_STR_BUF_SIZE_FLOAT  (16U)
#define JSON_STREAM_GEN_STR_BUF_SIZE_DOUBLE (30U)

#define JSON_STREAM_GEN_STR_BUF_SIZE_LIMITED_FLOAT  (13U)
#define JSON_STREAM_GEN_STR_BUF_SIZE_LIMITED_DOUBLE (22U)

#define JSON_STREAM_GEN_CONST_U32_10    (10U)
#define JSON_STREAM_GEN_CONST_FLOAT_10  (10.0f)
#define JSON_STREAM_GEN_CONST_DOUBLE_10 (10.0)
#define JSON_STREAM_GEN_CONST_FLOAT_1   (1.0f)
#define JSON_STREAM_GEN_CONST_DOUBLE_1  (1.0)

typedef enum json_stream_gen_state_e
{
    JSON_STREAM_GEN_STATE_JSON_OPENING_BRACKET,
    JSON_STREAM_GEN_STATE_GENERATING_ITEMS,
    JSON_STREAM_GEN_STATE_JSON_CLOSING_BRACKET,
    JSON_STREAM_GEN_STATE_FINISHED,
    JSON_STREAM_GEN_STATE_ERROR_INSUFFICIENT_BUFFER,
} json_stream_gen_state_e;

struct json_stream_gen_t
{
    json_stream_gen_cb_generate_next_t cb_gen_next;
    void*                              p_ctx;
    json_stream_gen_cfg_t              cfg;
    char*                              p_chunk_buf;
    size_t                             chunk_buf_idx;
    uint32_t                           cur_nesting_level;
    char*                              p_indent_filling;
    bool                               flag_new_data_added;
    int32_t                            json_stream_gen_stage;
    json_stream_gen_state_e            json_gen_state;
    bool                               is_first_item;
    const char*                        p_eol;
    char                               p_delimiter[2];
};

/**
 * @brief typedef for basic 'int' type
 *
 * MISRA C:2004, 6.3 - typedefs that indicate size and signedness should be used in place of the basic types.
 * In adherence to the MISRA C:2004, 6.3 guideline, this typedef provides a layer of abstraction that
 * promotes portability and readability by denoting the size and signedness of this basic integer type.
 * Instead of using 'int' directly, using this typedef clarifies that the integer is expected to behave
 * according to these particular constraints.
 */
typedef int jsg_int_t;

static void
jsg_copy_non_zero_cfg_fields(json_stream_gen_cfg_t* const p_dst, const json_stream_gen_cfg_t* const p_src)
{
    if (0 != p_src->max_chunk_size)
    {
        p_dst->max_chunk_size = p_src->max_chunk_size;
    }
    p_dst->flag_formatted_json = p_src->flag_formatted_json;
    if (0 != p_src->max_nesting_level)
    {
        p_dst->max_nesting_level = p_src->max_nesting_level;
    }
    if ('\0' != p_src->indentation_mark)
    {
        p_dst->indentation_mark = p_src->indentation_mark;
    }
    if (0 != p_src->indentation)
    {
        p_dst->indentation = p_src->indentation;
    }
    if (NULL != p_src->p_malloc)
    {
        p_dst->p_malloc = p_src->p_malloc;
    }
    if (NULL != p_src->p_free)
    {
        p_dst->p_free = p_src->p_free;
    }
}

json_stream_gen_t*
json_stream_gen_create(
    const json_stream_gen_cfg_t* const p_cfg,
    json_stream_gen_cb_generate_next_t cb_gen_next,
    const size_t                       ctx_size,
    void**                             p_p_ctx)
{
    json_stream_gen_cfg_t cfg = JSON_STREAM_GEN_CFG_DEFAULT();
    if ((0 != ctx_size) && (NULL == p_p_ctx))
    {
        return NULL;
    }
    if (NULL != p_cfg)
    {
        jsg_copy_non_zero_cfg_fields(&cfg, p_cfg);
    }
    if (cfg.max_chunk_size < JSON_STREAM_GEN_CFG_MIN_CHUNK_SIZE)
    {
        return NULL;
    }
    if (cfg.flag_formatted_json
        && ((0 == cfg.max_nesting_level) || (0 == cfg.indentation) || (0 == cfg.indentation_mark)))
    {
        return NULL;
    }
    if ((NULL == cfg.p_malloc) || (NULL == cfg.p_free))
    {
        return NULL;
    }

    size_t mem_size = sizeof(json_stream_gen_t);
    mem_size += ctx_size;
    mem_size += cfg.max_chunk_size;
    if (cfg.flag_formatted_json)
    {
        mem_size += (cfg.max_nesting_level * cfg.indentation) + 1;
    }
    else
    {
        mem_size += 1;
    }
    json_stream_gen_t* const p_gen = cfg.p_malloc(mem_size);
    if (NULL == p_gen)
    {
        return NULL;
    }
    memset(p_gen, 0, mem_size);
    p_gen->cfg         = cfg;
    p_gen->cb_gen_next = cb_gen_next;
    p_gen->p_ctx       = (char*)p_gen + sizeof(*p_gen);
    p_gen->p_chunk_buf = (char*)p_gen->p_ctx + ctx_size;
    if (0 == ctx_size)
    {
        p_gen->p_ctx = NULL;
    }

    p_gen->p_indent_filling = (char*)p_gen->p_chunk_buf + cfg.max_chunk_size;
    if (cfg.flag_formatted_json)
    {
        const size_t indent = (size_t)cfg.indentation * cfg.max_nesting_level;
        memset(p_gen->p_indent_filling, cfg.indentation_mark, indent);
        p_gen->p_indent_filling[indent] = '\0';
    }
    else
    {
        p_gen->p_indent_filling[0] = '\0';
    }

    p_gen->p_eol          = cfg.flag_formatted_json ? "\n" : "";
    p_gen->p_delimiter[0] = cfg.flag_formatted_json ? cfg.indentation_mark : '\0';
    p_gen->p_delimiter[1] = '\0';

    json_stream_gen_reset(p_gen);

    if (NULL != p_p_ctx)
    {
        *p_p_ctx = p_gen->p_ctx;
    }

    return p_gen;
}

int32_t
json_stream_gen_get_stage_internal(const json_stream_gen_t* const p_gen)
{
    return p_gen->json_stream_gen_stage;
}

void
json_stream_gen_inc_stage_internal(json_stream_gen_t* const p_gen)
{
    p_gen->json_stream_gen_stage += 1;
}

void
json_stream_gen_delete(json_stream_gen_t** p_p_gen)
{
    (*p_p_gen)->cfg.p_free(*p_p_gen);
    *p_p_gen = NULL;
}

static bool
jsg_vprintf(json_stream_gen_t* const p_gen, const size_t saved_chunk_buf_idx, const char* const p_fmt, va_list p_args)
{
    p_gen->flag_new_data_added = true;
    char* const  p_buf         = &p_gen->p_chunk_buf[p_gen->chunk_buf_idx];
    const size_t remaining_len = p_gen->cfg.max_chunk_size - p_gen->chunk_buf_idx;
    const int    len           = vsnprintf(p_buf, remaining_len, p_fmt, p_args);
    if (len >= (int)remaining_len)
    {
        p_gen->chunk_buf_idx                     = saved_chunk_buf_idx;
        p_gen->p_chunk_buf[p_gen->chunk_buf_idx] = '\0';
        return false;
    }
    p_gen->chunk_buf_idx += len;
    return true;
}

__attribute__((format(printf, 3, 4))) static bool
jsg_printf(json_stream_gen_t* const p_gen, const size_t saved_chunk_buf_idx, const char* const p_fmt, ...)
{
    va_list args;
    va_start(args, p_fmt);
    const bool res = jsg_vprintf(p_gen, saved_chunk_buf_idx, p_fmt, args);
    va_end(args);
    return res;
}

static bool
jsg_step_json_opening_bracket(json_stream_gen_t* const p_gen)
{
    if (p_gen->cur_nesting_level >= p_gen->cfg.max_nesting_level)
    {
        return false;
    }
    const jsg_int_t indent = (jsg_int_t)p_gen->cur_nesting_level * (jsg_int_t)p_gen->cfg.indentation;
    if (!jsg_printf(p_gen, p_gen->chunk_buf_idx, "%.*s{", indent, p_gen->p_indent_filling))
    {
        return false;
    }
    p_gen->json_gen_state = JSON_STREAM_GEN_STATE_GENERATING_ITEMS;
    p_gen->cur_nesting_level += 1;
    return true;
}

static bool
jsg_step_generating_items(json_stream_gen_t* const p_gen)
{
    p_gen->flag_new_data_added = false;
    if (!p_gen->cb_gen_next(p_gen, p_gen->p_ctx))
    {
        if (p_gen->flag_new_data_added && (0 == p_gen->chunk_buf_idx))
        {
            p_gen->json_gen_state = JSON_STREAM_GEN_STATE_ERROR_INSUFFICIENT_BUFFER;
        }
        return false;
    }
    if (!p_gen->flag_new_data_added)
    {
        p_gen->json_gen_state = JSON_STREAM_GEN_STATE_JSON_CLOSING_BRACKET;
    }
    return true;
}

static bool
jsg_step_json_closing_bracket(json_stream_gen_t* const p_gen)
{
    if (0 == p_gen->cur_nesting_level)
    {
        return false;
    }
    const jsg_int_t indent = (jsg_int_t)((p_gen->cur_nesting_level - 1) * p_gen->cfg.indentation);
    if (!jsg_printf(p_gen, p_gen->chunk_buf_idx, "%s%.*s}", p_gen->p_eol, indent, p_gen->p_indent_filling))
    {
        return false;
    }
    p_gen->cur_nesting_level -= 1;
    p_gen->json_gen_state = JSON_STREAM_GEN_STATE_FINISHED;
    return true;
}

static bool
jsg_get_next_chunk_step(json_stream_gen_t* const p_gen)
{
    switch (p_gen->json_gen_state)
    {
        case JSON_STREAM_GEN_STATE_JSON_OPENING_BRACKET:
            if (!jsg_step_json_opening_bracket(p_gen))
            {
                return false;
            }
            break;
        case JSON_STREAM_GEN_STATE_GENERATING_ITEMS:
            if (!jsg_step_generating_items(p_gen))
            {
                return false;
            }
            break;
        case JSON_STREAM_GEN_STATE_JSON_CLOSING_BRACKET:
            if (!jsg_step_json_closing_bracket(p_gen))
            {
                return false;
            }
            break;
        case JSON_STREAM_GEN_STATE_FINISHED:
        case JSON_STREAM_GEN_STATE_ERROR_INSUFFICIENT_BUFFER:
            return false;
    }
    return true;
}

const char*
json_stream_gen_get_next_chunk(json_stream_gen_t* const p_gen)
{
    p_gen->chunk_buf_idx  = 0;
    p_gen->p_chunk_buf[0] = '\0';
    while (jsg_get_next_chunk_step(p_gen))
    {
        // Continuously fetch and add the next portion of data to the chunk as long as such data is available.
        // The loop will stop if the chunk becomes overflowed.
    }
    if (JSON_STREAM_GEN_STATE_ERROR_INSUFFICIENT_BUFFER == p_gen->json_gen_state)
    {
        return NULL;
    }
    return p_gen->p_chunk_buf;
}

json_stream_gen_size_t
json_stream_gen_calc_size(json_stream_gen_t* const p_gen)
{
    if (0 != p_gen->json_stream_gen_stage)
    {
        return -1;
    }

    json_stream_gen_size_t json_len = 0;
    const char*            p_chunk  = NULL;
    while (true)
    {
        p_chunk = json_stream_gen_get_next_chunk(p_gen);
        if (NULL == p_chunk)
        {
            json_len = -1;
            break;
        }
        const size_t chunk_len = strlen(p_chunk);
        if (0 == chunk_len)
        {
            break;
        }
        json_len += (json_stream_gen_size_t)chunk_len;
    }

    json_stream_gen_reset(p_gen);
    return json_len;
}

void
json_stream_gen_reset(json_stream_gen_t* const p_gen)
{
    p_gen->json_stream_gen_stage = 0;
    p_gen->json_gen_state        = JSON_STREAM_GEN_STATE_JSON_OPENING_BRACKET;
    p_gen->cur_nesting_level     = 0;
    p_gen->is_first_item         = true;
    p_gen->chunk_buf_idx         = 0;
}

static bool
jsg_print_prefix(json_stream_gen_t* const p_gen, const size_t saved_chunk_buf_idx, const char* const p_name)
{
    const jsg_int_t   indent = (jsg_int_t)(p_gen->cur_nesting_level * p_gen->cfg.indentation);
    const char* const p_sep  = p_gen->is_first_item ? "" : ",";
    if (NULL != p_name)
    {
        if (!jsg_printf(
                p_gen,
                saved_chunk_buf_idx,
                "%s%s%.*s\"%s\":%s",
                p_sep,
                p_gen->p_eol,
                indent,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter))
        {
            return false;
        }
    }
    else
    {
        if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%s%s%.*s", p_sep, p_gen->p_eol, indent, p_gen->p_indent_filling))
        {
            return false;
        }
    }
    return true;
}

static bool
jsg_start_obj_or_arr(json_stream_gen_t* const p_gen, const char* const p_name, const char symbol)
{
    if (p_gen->cur_nesting_level == p_gen->cfg.max_nesting_level)
    {
        p_gen->flag_new_data_added = true;
        return false;
    }

    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%c", symbol))
    {
        return false;
    }

    p_gen->cur_nesting_level += 1;
    p_gen->is_first_item = true;
    return true;
}

bool
json_stream_gen_start_object(json_stream_gen_t* const p_gen, const char* const p_name)
{
    return jsg_start_obj_or_arr(p_gen, p_name, '{');
}

bool
json_stream_gen_start_array(json_stream_gen_t* const p_gen, const char* const p_name)
{
    return jsg_start_obj_or_arr(p_gen, p_name, '[');
}

static bool
jsg_end_obj_or_array(json_stream_gen_t* const p_gen, const char symbol)
{
    if (0 == p_gen->cur_nesting_level)
    {
        p_gen->flag_new_data_added = true;
        return false;
    }
    if (p_gen->is_first_item)
    {
        if (!jsg_printf(p_gen, p_gen->chunk_buf_idx, "%c", symbol))
        {
            return false;
        }
    }
    else
    {
        const jsg_int_t indent = (jsg_int_t)((p_gen->cur_nesting_level - 1) * p_gen->cfg.indentation);
        if (!jsg_printf(p_gen, p_gen->chunk_buf_idx, "%s%.*s%c", p_gen->p_eol, indent, p_gen->p_indent_filling, symbol))
        {
            return false;
        }
    }
    p_gen->cur_nesting_level -= 1;
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_end_object(json_stream_gen_t* const p_gen)
{
    return jsg_end_obj_or_array(p_gen, '}');
}

bool
json_stream_gen_end_array(json_stream_gen_t* const p_gen)
{
    return jsg_end_obj_or_array(p_gen, ']');
}

static bool
jsg_check_char_escaping(const char input_char, char* const p_output_char)
{
    char output_char        = input_char;
    bool flag_need_escaping = false;
    switch (input_char)
    {
        case '\"':
            output_char        = '\"';
            flag_need_escaping = true;
            break;
        case '\\':
            output_char        = '\\';
            flag_need_escaping = true;
            break;
        case '\b':
            output_char        = 'b';
            flag_need_escaping = true;
            break;
        case '\f':
            output_char        = 'f';
            flag_need_escaping = true;
            break;
        case '\n':
            output_char        = 'n';
            flag_need_escaping = true;
            break;
        case '\r':
            output_char        = 'r';
            flag_need_escaping = true;
            break;
        case '\t':
            output_char        = 't';
            flag_need_escaping = true;
            break;
        default:
            break;
    }
    if (NULL != p_output_char)
    {
        *p_output_char = output_char;
    }
    return flag_need_escaping;
}

static bool
jsg_check_if_str_need_escaping(const char* const p_val)
{
    for (const char* p_char = p_val; '\0' != *p_char; p_char++)
    {
        if (jsg_check_char_escaping(*p_char, NULL))
        {
            return true;
        }
    }
    return false;
}

bool
json_stream_gen_add_string(json_stream_gen_t* const p_gen, const char* const p_name, const char* const p_val)
{
    if (NULL == p_val)
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }
    if (!jsg_check_if_str_need_escaping(p_val))
    {
        return json_stream_gen_add_raw_string(p_gen, p_name, p_val);
    }

    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "\""))
    {
        return false;
    }

    for (const char* p_char = p_val; '\0' != *p_char; p_char++)
    {
        char       output_char = '\0';
        const bool flag_escape = jsg_check_char_escaping(*p_char, &output_char);
        if (flag_escape)
        {
            if (!jsg_printf(p_gen, saved_chunk_buf_idx, "\\%c", output_char))
            {
                return false;
            }
        }
        else
        {
            if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%c", *p_char))
            {
                return false;
            }
        }
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "\""))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_raw_string(json_stream_gen_t* const p_gen, const char* const p_name, const char* const p_val)
{
    if (NULL == p_val)
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }
    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "\"%s\"", p_val))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_int32(json_stream_gen_t* const p_gen, const char* const p_name, const int32_t val)
{
    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%" PRId32, val))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_uint32(json_stream_gen_t* const p_gen, const char* const p_name, const uint32_t val)
{
    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%" PRIu32, val))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_int64(json_stream_gen_t* const p_gen, const char* const p_name, const int64_t val)
{
    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%" PRId64, val))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_uint64(json_stream_gen_t* const p_gen, const char* const p_name, const uint64_t val)
{
    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%" PRIu64, val))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_bool(json_stream_gen_t* const p_gen, const char* const p_name, const bool val)
{
    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%s", val ? "true" : "false"))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_null(json_stream_gen_t* const p_gen, const char* const p_name)
{
    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%s", "null"))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}

static char
jsg_get_decimal_point(void)
{
    struct lconv* const p_lc = localeconv();
    if (NULL == p_lc)
    {
        return '.';
    }
    return *p_lc->decimal_point;
}

typedef struct jsg_float_str_buf_t
{
    char buffer[JSON_STREAM_GEN_STR_BUF_SIZE_FLOAT];
} jsg_float_str_buf_t;

static bool
jsg_float_to_str(
    const float                               val,
    const bool                                flag_fixed_point,
    const json_stream_gen_ieee754_precision_t precision,
    jsg_float_str_buf_t* const                p_str)
{
    if ((0 != isnanf(val)) || (0 != isinff(val)))
    {
        return false;
    }
    int len = 0;
    if (flag_fixed_point)
    {
        len = snprintf(p_str->buffer, sizeof(p_str->buffer), "%.*f", precision, val);
    }
    else
    {
        if (precision < 0)
        {
            len = snprintf(p_str->buffer, sizeof(p_str->buffer), "%1.7g", val);

            // Check if the original double value can be recovered
            char*       p_end = NULL;
            const float test  = strtof(p_str->buffer, &p_end);
            if (('\0' != *p_end) || (*(const uint32_t*)&test != *(const uint32_t*)&val))
            {
                // If not, print with more decimal places of precision
                len = snprintf(p_str->buffer, sizeof(p_str->buffer), "%1.9g", val);
            }
        }
        else
        {
            len = snprintf(p_str->buffer, sizeof(p_str->buffer), "%1.*g", precision, val);
        }
    }

    if ((len < 0) || ((size_t)len >= sizeof(p_str->buffer)))
    {
        return false;
    }

    char* p_decimal_point = strchr(p_str->buffer, jsg_get_decimal_point());
    if (NULL != p_decimal_point)
    {
        *p_decimal_point = '.';
    }
    return true;
}

static bool
jsg_add_float_with_precision(
    json_stream_gen_t* const                  p_gen,
    const char* const                         p_name,
    const float                               val,
    const bool                                flag_fixed_point,
    const json_stream_gen_ieee754_precision_t precision)
{
    p_gen->flag_new_data_added = true;

    jsg_float_str_buf_t float_str = { 0 };
    if (!jsg_float_to_str(val, flag_fixed_point, precision, &float_str))
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }

    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%s", float_str.buffer))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}

typedef struct jsg_double_str_buf_t
{
    char buffer[JSON_STREAM_GEN_STR_BUF_SIZE_DOUBLE];
} jsg_double_str_buf_t;

static bool
jsg_double_to_str(
    const double                              val,
    const bool                                flag_fixed_point,
    const json_stream_gen_ieee754_precision_t precision,
    jsg_double_str_buf_t* const               p_str)
{
    if ((0 != isnan(val)) || (0 != isinf(val)))
    {
        return false;
    }
    int len = 0;
    if (flag_fixed_point)
    {
        len = snprintf(p_str->buffer, sizeof(p_str->buffer), "%.*f", precision, val);
    }
    else
    {
        if (precision < 0)
        {
            len = snprintf(p_str->buffer, sizeof(p_str->buffer), "%1.15g", val);

            // Check if the original double value can be recovered
            char*        p_end = NULL;
            const double test  = strtod(p_str->buffer, &p_end);
            if (('\0' != *p_end) || (*(const uint64_t*)&test != *(const uint64_t*)&val))
            {
                // If not, print with more decimal places of precision
                len = snprintf(p_str->buffer, sizeof(p_str->buffer), "%1.17g", val);
            }
        }
        else
        {
            len = snprintf(p_str->buffer, sizeof(p_str->buffer), "%1.*g", precision, val);
        }
    }

    if ((len < 0) || ((size_t)len >= sizeof(p_str->buffer)))
    {
        return false;
    }

    char* p_decimal_point = strchr(p_str->buffer, jsg_get_decimal_point());
    if (NULL != p_decimal_point)
    {
        *p_decimal_point = '.';
    }
    return true;
}

static bool
jsg_add_double_with_precision(
    json_stream_gen_t* const            p_gen,
    const char* const                   p_name,
    const double                        val,
    const bool                          flag_fixed_point,
    json_stream_gen_ieee754_precision_t precision)
{
    p_gen->flag_new_data_added = true;

    jsg_double_str_buf_t double_str = { 0 };
    if (!jsg_double_to_str(val, flag_fixed_point, precision, &double_str))
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }
    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%s", double_str.buffer))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_float(
    json_stream_gen_t* const            p_gen,
    const char* const                   p_name,
    const float                         val,
    json_stream_gen_ieee754_precision_t precision)
{
    return jsg_add_float_with_precision(p_gen, p_name, val, false, precision);
}

bool
json_stream_gen_add_double(
    json_stream_gen_t* const            p_gen,
    const char* const                   p_name,
    const double                        val,
    json_stream_gen_ieee754_precision_t precision)
{
    return jsg_add_double_with_precision(p_gen, p_name, val, false, precision);
}

bool
json_stream_gen_add_float_fixed_point(
    json_stream_gen_t* const             p_gen,
    const char* const                    p_name,
    const float                          val,
    json_stream_gen_num_decimals_float_e num_decimals)
{
    return jsg_add_float_with_precision(p_gen, p_name, val, true, (json_stream_gen_ieee754_precision_t)num_decimals);
}

bool
json_stream_gen_add_double_fixed_point(
    json_stream_gen_t* const              p_gen,
    const char* const                     p_name,
    const double                          val,
    json_stream_gen_num_decimals_double_e num_decimals)
{
    return jsg_add_double_with_precision(p_gen, p_name, val, true, (json_stream_gen_ieee754_precision_t)num_decimals);
}

typedef struct jsg_limited_float_str_buf_t
{
    char buffer[JSON_STREAM_GEN_STR_BUF_SIZE_LIMITED_FLOAT];
} jsg_limited_float_str_buf_t;

static bool
jsg_limited_float_to_str(
    const float                                val,
    const json_stream_gen_num_decimals_float_e num_decimals,
    jsg_limited_float_str_buf_t* const         p_str)
{
    static const uint32_t g_multipliers_u32[10] = { (uint32_t)1e+0, (uint32_t)1e+1, (uint32_t)1e+2, (uint32_t)1e+3,
                                                    (uint32_t)1e+4, (uint32_t)1e+5, (uint32_t)1e+6, (uint32_t)1e+7,
                                                    (uint32_t)1e+8, (uint32_t)1e+9 };

    if (num_decimals >= (sizeof(g_multipliers_u32) / sizeof(g_multipliers_u32[0])))
    {
        return false;
    }
    if ((0 != isnanf(val)) || (0 != isinff(val)))
    {
        return false;
    }
    const float abs_val = fabsf(val);
    if (abs_val > (float)UINT32_MAX)
    {
        return false;
    }

    uint32_t multiplier  = g_multipliers_u32[num_decimals];
    int32_t  divider_cnt = 0;
    float    divider     = JSON_STREAM_GEN_CONST_FLOAT_1;
    while (((abs_val * (float)multiplier) / divider) > (float)(1U << (uint32_t)FLT_MANT_DIG))
    {
        if (multiplier > 1)
        {
            multiplier /= JSON_STREAM_GEN_CONST_U32_10;
        }
        else
        {
            divider_cnt += 1;
            divider *= JSON_STREAM_GEN_CONST_FLOAT_10;
        }
    }
    const char zeros_str[] = "000";
    if (divider_cnt >= (int32_t)(sizeof(zeros_str)))
    {
        return false;
    }

    const uint32_t val_u32         = (uint32_t)lrintf((abs_val * (float)multiplier) / divider);
    const uint32_t integral_part   = val_u32 / multiplier;
    const uint32_t fractional_part = val_u32 % multiplier;

    json_stream_gen_num_decimals_t actual_num_decimals = 0;
    while (multiplier > 1)
    {
        multiplier /= JSON_STREAM_GEN_CONST_U32_10;
        actual_num_decimals += 1;
    }

    const int len = snprintf(
        p_str->buffer,
        sizeof(p_str->buffer),
        "%s"
        "%" PRIu32
        "%.*s"
        "%s"
        "%.*" PRIu32,
        (val < 0) ? "-" : "",
        integral_part,
        divider_cnt,
        zeros_str,
        (0 != actual_num_decimals) ? "." : "",
        actual_num_decimals,
        fractional_part);
    if ((len < 0) || ((size_t)len >= sizeof(p_str->buffer)))
    {
        return false;
    }
    return true;
}

bool
json_stream_gen_add_float_limited_fixed_point(
    json_stream_gen_t* const                   p_gen,
    const char* const                          p_name,
    const float                                val,
    const json_stream_gen_num_decimals_float_e num_decimals)
{
    p_gen->flag_new_data_added = true;

    jsg_limited_float_str_buf_t float_str = { 0 };
    if (!jsg_limited_float_to_str(val, num_decimals, &float_str))
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }

    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%s", float_str.buffer))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}

typedef struct jsg_limited_double_str_buf_t
{
    char buffer[JSON_STREAM_GEN_STR_BUF_SIZE_LIMITED_DOUBLE];
} jsg_limited_double_str_buf_t;

static bool
jsg_limited_double_to_str(
    const double                                val,
    const json_stream_gen_num_decimals_double_e num_decimals,
    jsg_limited_double_str_buf_t* const         p_str)
{
    static const uint64_t g_multipliers_u64[20] = {
        (uint64_t)1e+0,  (uint64_t)1e+1,  (uint64_t)1e+2,  (uint64_t)1e+3,  (uint64_t)1e+4,
        (uint64_t)1e+5,  (uint64_t)1e+6,  (uint64_t)1e+7,  (uint64_t)1e+8,  (uint64_t)1e+9,
        (uint64_t)1e+10, (uint64_t)1e+11, (uint64_t)1e+12, (uint64_t)1e+13, (uint64_t)1e+14,
        (uint64_t)1e+15, (uint64_t)1e+16, (uint64_t)1e+17, (uint64_t)1e+18, (uint64_t)1e+19
    };

    if (num_decimals >= (sizeof(g_multipliers_u64) / sizeof(g_multipliers_u64[0])))
    {
        return false;
    }
    if ((0 != isnan(val)) || (0 != isinf(val)))
    {
        return false;
    }
    const double abs_val = fabs(val);
    if (abs_val > (double)UINT64_MAX)
    {
        return false;
    }

    uint64_t multiplier  = g_multipliers_u64[num_decimals];
    int32_t  divider_cnt = 0;
    double   divider     = JSON_STREAM_GEN_CONST_DOUBLE_1;
    while (((abs_val * (double)multiplier) / divider) > (double)((uint64_t)1LU << (uint32_t)DBL_MANT_DIG))
    {
        if (multiplier > 1)
        {
            multiplier /= JSON_STREAM_GEN_CONST_U32_10;
        }
        else
        {
            divider_cnt += 1;
            divider *= JSON_STREAM_GEN_CONST_DOUBLE_10;
        }
    }
    const char zeros_str[] = "0000";
    if (divider_cnt >= (int32_t)(sizeof(zeros_str)))
    {
        return false;
    }

    const uint64_t val_u64         = (uint64_t)lrint((abs_val * (double)multiplier) / divider);
    const uint64_t integral_part   = val_u64 / multiplier;
    const uint64_t fractional_part = val_u64 % multiplier;

    json_stream_gen_num_decimals_t actual_num_decimals = 0;
    while (multiplier > 1)
    {
        multiplier /= JSON_STREAM_GEN_CONST_U32_10;
        actual_num_decimals += 1;
    }
    const int len = snprintf(
        p_str->buffer,
        sizeof(p_str->buffer),
        "%s"
        "%" PRIu64
        "%.*s"
        "%s"
        "%.*" PRIu64,
        (val < 0) ? "-" : "",
        integral_part,
        divider_cnt,
        zeros_str,
        (0 != actual_num_decimals) ? "." : "",
        actual_num_decimals,
        fractional_part);

    if ((len < 0) || ((size_t)len >= sizeof(p_str->buffer)))
    {
        return false;
    }
    return true;
}

bool
json_stream_gen_add_double_limited_fixed_point(
    json_stream_gen_t* const                    p_gen,
    const char* const                           p_name,
    const double                                val,
    const json_stream_gen_num_decimals_double_e num_decimals)
{
    p_gen->flag_new_data_added = true;

    jsg_limited_double_str_buf_t double_str = { 0 };
    if (!jsg_limited_double_to_str(val, num_decimals, &double_str))
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }

    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%s", double_str.buffer))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_hex_buf(
    json_stream_gen_t* const p_gen,
    const char* const        p_name,
    const uint8_t* const     p_buf,
    size_t                   buf_len)
{
    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;
    if (!jsg_print_prefix(p_gen, saved_chunk_buf_idx, p_name))
    {
        return false;
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "\""))
    {
        return false;
    }
    for (size_t i = 0; i < buf_len; ++i)
    {
        if (!jsg_printf(p_gen, saved_chunk_buf_idx, "%02X", p_buf[i]))
        {
            return false;
        }
    }
    if (!jsg_printf(p_gen, saved_chunk_buf_idx, "\""))
    {
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}
