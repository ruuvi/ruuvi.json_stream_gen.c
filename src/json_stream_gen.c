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
#include <locale.h>

#define JSON_STREAM_GEN_STR_BUF_SIZE_FLOAT  (16U)
#define JSON_STREAM_GEN_STR_BUF_SIZE_DOUBLE (28U)

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

static void
json_stream_gen_copy_non_zero_cfg_fields(json_stream_gen_cfg_t* const p_dst, const json_stream_gen_cfg_t* const p_src)
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
        json_stream_gen_copy_non_zero_cfg_fields(&cfg, p_cfg);
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
        mem_size += cfg.max_nesting_level * cfg.indentation + 1;
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
        memset(p_gen->p_indent_filling, cfg.indentation_mark, cfg.max_nesting_level * cfg.indentation);
        p_gen->p_indent_filling[cfg.max_nesting_level * cfg.indentation] = '\0';
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
json_stream_gen_vprintf(json_stream_gen_t* const p_gen, const char* const p_fmt, va_list args)
{
    p_gen->flag_new_data_added = true;
    char* const  p_buf         = &p_gen->p_chunk_buf[p_gen->chunk_buf_idx];
    const size_t remaining_len = p_gen->cfg.max_chunk_size - p_gen->chunk_buf_idx;
    const int    len           = vsnprintf(p_buf, remaining_len, p_fmt, args);
    if (len >= (int)remaining_len)
    {
        *p_buf = '\0';
        return false;
    }
    p_gen->chunk_buf_idx += len;
    return true;
}

__attribute__((format(printf, 2, 3))) static bool
json_stream_gen_printf(json_stream_gen_t* const p_gen, const char* const p_fmt, ...)
{
    va_list args;
    va_start(args, p_fmt);
    const bool res = json_stream_gen_vprintf(p_gen, p_fmt, args);
    va_end(args);
    return res;
}

static bool
json_stream_gen_step(json_stream_gen_t* const p_gen)
{
    switch (p_gen->json_gen_state)
    {
        case JSON_STREAM_GEN_STATE_JSON_OPENING_BRACKET:
            if (p_gen->cur_nesting_level >= p_gen->cfg.max_nesting_level)
            {
                return false;
            }
            if (!json_stream_gen_printf(
                    p_gen,
                    "%.*s{",
                    p_gen->cur_nesting_level * p_gen->cfg.indentation,
                    p_gen->p_indent_filling))
            {
                return false;
            }
            p_gen->json_gen_state = JSON_STREAM_GEN_STATE_GENERATING_ITEMS;
            p_gen->cur_nesting_level += 1;
            break;
        case JSON_STREAM_GEN_STATE_GENERATING_ITEMS:
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
                break;
            }
            break;
        case JSON_STREAM_GEN_STATE_JSON_CLOSING_BRACKET:
            if (0 == p_gen->cur_nesting_level)
            {
                return false;
            }
            if (!json_stream_gen_printf(
                    p_gen,
                    "%s%.*s}",
                    p_gen->p_eol,
                    (p_gen->cur_nesting_level - 1) * p_gen->cfg.indentation,
                    p_gen->p_indent_filling))
            {
                return false;
            }
            p_gen->cur_nesting_level -= 1;
            p_gen->json_gen_state = JSON_STREAM_GEN_STATE_FINISHED;
            break;
        case JSON_STREAM_GEN_STATE_FINISHED:
            return false;
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
    while (json_stream_gen_step(p_gen))
    {
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

    ssize_t     json_len = 0;
    const char* p_chunk  = NULL;
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
        json_len += (ssize_t)chunk_len;
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

bool
json_stream_gen_start_object(json_stream_gen_t* const p_gen, const char* const p_name)
{
    if (p_gen->cur_nesting_level == p_gen->cfg.max_nesting_level)
    {
        p_gen->flag_new_data_added = true;
        return false;
    }
    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s{",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s{",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling))
        {
            return false;
        }
    }
    p_gen->cur_nesting_level += 1;
    p_gen->is_first_item = true;
    return true;
}

bool
json_stream_gen_end_object(json_stream_gen_t* const p_gen)
{
    if (0 == p_gen->cur_nesting_level)
    {
        p_gen->flag_new_data_added = true;
        return false;
    }
    if (p_gen->is_first_item)
    {
        if (!json_stream_gen_printf(p_gen, "}"))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%.*s}",
                p_gen->p_eol,
                (p_gen->cur_nesting_level - 1) * p_gen->cfg.indentation,
                p_gen->p_indent_filling))
        {
            return false;
        }
    }
    p_gen->cur_nesting_level -= 1;
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_start_array(json_stream_gen_t* const p_gen, const char* const p_name)
{
    if (p_gen->cur_nesting_level == p_gen->cfg.max_nesting_level)
    {
        p_gen->flag_new_data_added = true;
        return false;
    }
    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s[",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s[",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling))
        {
            return false;
        }
    }
    p_gen->cur_nesting_level += 1;
    p_gen->is_first_item = true;
    return true;
}

bool
json_stream_gen_end_array(json_stream_gen_t* const p_gen)
{
    if (0 == p_gen->cur_nesting_level)
    {
        p_gen->flag_new_data_added = true;
        return false;
    }
    if (p_gen->is_first_item)
    {
        if (!json_stream_gen_printf(p_gen, "]"))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%.*s]",
                p_gen->p_eol,
                (p_gen->cur_nesting_level - 1) * p_gen->cfg.indentation,
                p_gen->p_indent_filling))
        {
            return false;
        }
    }
    p_gen->cur_nesting_level -= 1;
    p_gen->is_first_item = false;
    return true;
}

static bool
json_stream_gen_check_if_str_need_escaping(const char* const p_val)
{
    for (const char* p_char = p_val; '\0' != *p_char; p_char++)
    {
        switch (*p_char)
        {
            case '\"':
            case '\\':
            case '\b':
            case '\f':
            case '\n':
            case '\r':
            case '\t':
                return true;
            default:
                break;
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
    if (!json_stream_gen_check_if_str_need_escaping(p_val))
    {
        return json_stream_gen_add_raw_string(p_gen, p_name, p_val);
    }

    const size_t saved_chunk_buf_idx = p_gen->chunk_buf_idx;

    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s\"",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter))
        {
            p_gen->chunk_buf_idx                     = saved_chunk_buf_idx;
            p_gen->p_chunk_buf[p_gen->chunk_buf_idx] = '\0';
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling))
        {
            p_gen->chunk_buf_idx                     = saved_chunk_buf_idx;
            p_gen->p_chunk_buf[p_gen->chunk_buf_idx] = '\0';
            return false;
        }
    }
    for (const char* p_char = p_val; '\0' != *p_char; p_char++)
    {
        bool flag_escape = true;
        char output_char = '\0';
        switch (*p_char)
        {
            case '\"':
                output_char = '\"';
                break;
            case '\\':
                output_char = '\\';
                break;
            case '\b':
                output_char = 'b';
                break;
            case '\f':
                output_char = 'f';
                break;
            case '\n':
                output_char = 'n';
                break;
            case '\r':
                output_char = 'r';
                break;
            case '\t':
                output_char = 't';
                break;
            default:
                flag_escape = false;
                break;
        }
        if (flag_escape)
        {
            if (!json_stream_gen_printf(p_gen, "\\%c", output_char))
            {
                p_gen->chunk_buf_idx                     = saved_chunk_buf_idx;
                p_gen->p_chunk_buf[p_gen->chunk_buf_idx] = '\0';
                return false;
            }
        }
        else
        {
            if (!json_stream_gen_printf(p_gen, "%c", *p_char))
            {
                p_gen->chunk_buf_idx                     = saved_chunk_buf_idx;
                p_gen->p_chunk_buf[p_gen->chunk_buf_idx] = '\0';
                return false;
            }
        }
    }
    if (!json_stream_gen_printf(p_gen, "\""))
    {
        p_gen->chunk_buf_idx                     = saved_chunk_buf_idx;
        p_gen->p_chunk_buf[p_gen->chunk_buf_idx] = '\0';
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
    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s\"%s\"",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter,
                p_val))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\"",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_val))
        {
            return false;
        }
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_int32(json_stream_gen_t* const p_gen, const char* const p_name, const int32_t val)
{
    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s"
                "%" PRId32,
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter,
                val))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s"
                "%" PRId32,
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                val))
        {
            return false;
        }
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_uint32(json_stream_gen_t* const p_gen, const char* const p_name, const uint32_t val)
{
    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s"
                "%" PRIu32,
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter,
                val))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s"
                "%" PRIu32,
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                val))
        {
            return false;
        }
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_int64(json_stream_gen_t* const p_gen, const char* const p_name, const int64_t val)
{
    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s"
                "%" PRId64,
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter,
                val))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s"
                "%" PRId64,
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                val))
        {
            return false;
        }
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_uint64(json_stream_gen_t* const p_gen, const char* const p_name, const uint64_t val)
{
    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s"
                "%" PRIu64,
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter,
                val))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s"
                "%" PRIu64,
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                val))
        {
            return false;
        }
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_bool(json_stream_gen_t* const p_gen, const char* const p_name, const bool val)
{
    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s"
                "%s",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter,
                val ? "true" : "false"))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s"
                "%s",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                val ? "true" : "false"))
        {
            return false;
        }
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_null(json_stream_gen_t* const p_gen, const char* const p_name)
{
    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s"
                "%s",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter,
                "null"))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s"
                "%s",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                "null"))
        {
            return false;
        }
    }
    p_gen->is_first_item = false;
    return true;
}

static char
json_stream_gen_get_decimal_point(void)
{
    struct lconv* const p_lc = localeconv();
    if (NULL == p_lc)
    {
        return '.';
    }
    return *p_lc->decimal_point;
}

typedef struct json_stream_gen_float_str_buf_t
{
    char buffer[JSON_STREAM_GEN_STR_BUF_SIZE_FLOAT];
} json_stream_gen_float_str_buf_t;

static bool
json_stream_gen_float_to_str(
    const float                               val,
    const bool                                flag_fixed_point,
    const json_stream_gen_ieee754_precision_t precision,
    json_stream_gen_float_str_buf_t* const    p_str)
{
    if (isnanf(val) || isinff(val))
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
            float test = NAN;
            if ((sscanf(p_str->buffer, "%g", &test) != 1) || (*(const uint32_t*)&test != *(const uint32_t*)&val))
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

    char* p_decimal_point = strchr(p_str->buffer, json_stream_gen_get_decimal_point());
    if (NULL != p_decimal_point)
    {
        *p_decimal_point = '.';
    }
    return true;
}

static bool
json_stream_gen_add_float_with_precision(
    json_stream_gen_t* const                  p_gen,
    const char* const                         p_name,
    const float                               val,
    const bool                                flag_fixed_point,
    const json_stream_gen_ieee754_precision_t precision)
{
    p_gen->flag_new_data_added = true;

    json_stream_gen_float_str_buf_t float_str = { 0 };
    if (!json_stream_gen_float_to_str(val, flag_fixed_point, precision, &float_str))
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }

    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s"
                "%s",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter,
                float_str.buffer))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s"
                "%s",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                float_str.buffer))
        {
            return false;
        }
    }
    p_gen->is_first_item = false;
    return true;
}

typedef struct json_stream_gen_double_str_buf_t
{
    char buffer[JSON_STREAM_GEN_STR_BUF_SIZE_DOUBLE];
} json_stream_gen_double_str_buf_t;

static bool
json_stream_gen_double_to_str(
    const double                              val,
    const bool                                flag_fixed_point,
    const json_stream_gen_ieee754_precision_t precision,
    json_stream_gen_double_str_buf_t* const   p_str)
{
    if (isnan(val) || isinf(val))
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
            double test = NAN;
            if ((sscanf(p_str->buffer, "%lg", &test) != 1) || (*(const uint64_t*)&test != *(const uint64_t*)&val))
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

    char* p_decimal_point = strchr(p_str->buffer, json_stream_gen_get_decimal_point());
    if (NULL != p_decimal_point)
    {
        *p_decimal_point = '.';
    }
    return true;
}

static bool
json_stream_gen_add_double_with_precision(
    json_stream_gen_t* const            p_gen,
    const char* const                   p_name,
    const double                        val,
    const bool                          flag_fixed_point,
    json_stream_gen_ieee754_precision_t precision)
{
    p_gen->flag_new_data_added = true;

    json_stream_gen_double_str_buf_t double_str = { 0 };
    if (!json_stream_gen_double_to_str(val, flag_fixed_point, precision, &double_str))
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }

    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s"
                "%s",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter,
                double_str.buffer))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s"
                "%s",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                double_str.buffer))
        {
            return false;
        }
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
    return json_stream_gen_add_float_with_precision(p_gen, p_name, val, false, precision);
}

bool
json_stream_gen_add_double(
    json_stream_gen_t* const            p_gen,
    const char* const                   p_name,
    const double                        val,
    json_stream_gen_ieee754_precision_t precision)
{
    return json_stream_gen_add_double_with_precision(p_gen, p_name, val, false, precision);
}

bool
json_stream_gen_add_float_fixed_point(
    json_stream_gen_t* const             p_gen,
    const char* const                    p_name,
    const float                          val,
    json_stream_gen_num_decimals_float_e num_decimals)
{
    return json_stream_gen_add_float_with_precision(
        p_gen,
        p_name,
        val,
        true,
        (json_stream_gen_ieee754_precision_t)num_decimals);
}

bool
json_stream_gen_add_double_fixed_point(
    json_stream_gen_t* const              p_gen,
    const char* const                     p_name,
    const double                          val,
    json_stream_gen_num_decimals_double_e num_decimals)
{
    return json_stream_gen_add_double_with_precision(
        p_gen,
        p_name,
        val,
        true,
        (json_stream_gen_ieee754_precision_t)num_decimals);
}

bool
json_stream_gen_add_float_limited_fixed_point(
    json_stream_gen_t* const                   p_gen,
    const char* const                          p_name,
    const float                                val,
    const json_stream_gen_num_decimals_float_e num_decimals)
{
    static const uint32_t multipliers[10] = { 1e+0, 1e+1, 1e+2, 1e+3, 1e+4, 1e+5, 1e+6, 1e+7, 1e+8, 1e+9 };

    p_gen->flag_new_data_added = true;

    if (num_decimals >= sizeof(multipliers) / sizeof(multipliers[0]))
    {
        return false;
    }
    if (isnanf(val) || isinff(val))
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }
    const float abs_val = fabsf(val);
    if (abs_val > (float)UINT32_MAX)
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }

    uint32_t multiplier  = multipliers[num_decimals];
    int32_t  divider_cnt = 0;
    float    divider     = 1.0f;
    while (((abs_val * (float)multiplier / divider) > (float)(1U << 24U)))
    {
        if (multiplier > 1)
        {
            multiplier /= 10;
        }
        else
        {
            divider_cnt += 1;
            divider *= 10.0f;
        }
    }
    const char zeros_str[] = "000";
    if (divider_cnt > (int32_t)(sizeof(zeros_str) - 1))
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }

    const uint32_t val_u32         = (uint32_t)lrintf(abs_val * (float)multiplier / divider);
    const uint32_t integral_part   = val_u32 / multiplier;
    const uint32_t fractional_part = val_u32 % multiplier;

    char tmp_buffer[JSON_STREAM_GEN_STR_BUF_SIZE_FLOAT];
    int  len = 0;
    if (1 == multiplier)
    {
        if (0 == divider_cnt)
        {
            len = snprintf(
                tmp_buffer,
                sizeof(tmp_buffer),
                "%s"
                "%" PRIu32,
                (val < 0) ? "-" : "",
                integral_part);
        }
        else
        {
            len = snprintf(
                tmp_buffer,
                sizeof(tmp_buffer),
                "%s"
                "%" PRIu32 "%.*s",
                (val < 0) ? "-" : "",
                integral_part,
                divider_cnt,
                zeros_str);
        }
    }
    else
    {
        json_stream_gen_num_decimals_t actual_num_decimals = 0;
        while (multiplier > 1)
        {
            multiplier /= 10;
            actual_num_decimals += 1;
        }
        len = snprintf(
            tmp_buffer,
            sizeof(tmp_buffer),
            "%s"
            "%" PRIu32
            "."
            "%.*" PRIu32,
            (val < 0) ? "-" : "",
            integral_part,
            actual_num_decimals,
            fractional_part);
    }

    if ((len < 0) || ((size_t)len >= sizeof(tmp_buffer)))
    {
        return false;
    }

    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s"
                "%s",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter,
                tmp_buffer))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s"
                "%s",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                tmp_buffer))
        {
            return false;
        }
    }
    p_gen->is_first_item = false;
    return true;
}

bool
json_stream_gen_add_double_limited_fixed_point(
    json_stream_gen_t* const                    p_gen,
    const char* const                           p_name,
    const double                                val,
    const json_stream_gen_num_decimals_double_e num_decimals)
{
    static const uint64_t multipliers[20] = { 1e+0,  1e+1,  1e+2,  1e+3,  1e+4,  1e+5,  1e+6,  1e+7,  1e+8,  1e+9,
                                              1e+10, 1e+11, 1e+12, 1e+13, 1e+14, 1e+15, 1e+16, 1e+17, 1e+18, 1e+19 };

    p_gen->flag_new_data_added = true;

    if (num_decimals >= sizeof(multipliers) / sizeof(multipliers[0]))
    {
        return false;
    }
    if (isnan(val) || isinf(val))
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }
    const double abs_val = fabs(val);
    if (abs_val > (double)UINT64_MAX)
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }

    uint32_t multiplier  = multipliers[num_decimals];
    int32_t  divider_cnt = 0;
    double   divider     = 1.0;
    while (((abs_val * (double)multiplier / divider) > (double)((uint64_t)1LU << 53U)))
    {
        if (multiplier > 1)
        {
            multiplier /= 10;
        }
        else
        {
            divider_cnt += 1;
            divider *= 10.0;
        }
    }
    const char zeros_str[] = "0000";
    if (divider_cnt > (int32_t)(sizeof(zeros_str) - 1))
    {
        return json_stream_gen_add_null(p_gen, p_name);
    }

    const uint64_t val_u64         = (uint64_t)lrint(abs_val * (double)multiplier / divider);
    const uint64_t integral_part   = val_u64 / multiplier;
    const uint64_t fractional_part = val_u64 % multiplier;

    char tmp_buffer[JSON_STREAM_GEN_STR_BUF_SIZE_DOUBLE];
    int  len = 0;
    if (1 == multiplier)
    {
        if (0 == divider_cnt)
        {
            len = snprintf(
                tmp_buffer,
                sizeof(tmp_buffer),
                "%s"
                "%" PRIu64,
                (val < 0) ? "-" : "",
                integral_part);
        }
        else
        {
            len = snprintf(
                tmp_buffer,
                sizeof(tmp_buffer),
                "%s"
                "%" PRIu64 "%.*s",
                (val < 0) ? "-" : "",
                integral_part,
                divider_cnt,
                zeros_str);
        }
    }
    else
    {
        json_stream_gen_num_decimals_t actual_num_decimals = 0;
        while (multiplier > 1)
        {
            multiplier /= 10;
            actual_num_decimals += 1;
        }
        len = snprintf(
            tmp_buffer,
            sizeof(tmp_buffer),
            "%s"
            "%" PRIu64
            "."
            "%.*" PRIu64,
            (val < 0) ? "-" : "",
            integral_part,
            actual_num_decimals,
            fractional_part);
    }

    if ((len < 0) || ((size_t)len >= sizeof(tmp_buffer)))
    {
        return false;
    }

    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s"
                "%s",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter,
                tmp_buffer))
        {
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s"
                "%s",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                tmp_buffer))
        {
            return false;
        }
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

    if (NULL != p_name)
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"%s\":%s\"",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling,
                p_name,
                p_gen->p_delimiter))
        {
            p_gen->chunk_buf_idx                     = saved_chunk_buf_idx;
            p_gen->p_chunk_buf[p_gen->chunk_buf_idx] = '\0';
            return false;
        }
    }
    else
    {
        if (!json_stream_gen_printf(
                p_gen,
                "%s%s%.*s\"",
                p_gen->is_first_item ? "" : ",",
                p_gen->p_eol,
                p_gen->cur_nesting_level * p_gen->cfg.indentation,
                p_gen->p_indent_filling))
        {
            p_gen->chunk_buf_idx                     = saved_chunk_buf_idx;
            p_gen->p_chunk_buf[p_gen->chunk_buf_idx] = '\0';
            return false;
        }
    }
    for (size_t i = 0; i < buf_len; ++i)
    {
        if (!json_stream_gen_printf(p_gen, "%02X", p_buf[i]))
        {
            p_gen->chunk_buf_idx                     = saved_chunk_buf_idx;
            p_gen->p_chunk_buf[p_gen->chunk_buf_idx] = '\0';
            return false;
        }
    }
    if (!json_stream_gen_printf(p_gen, "\""))
    {
        p_gen->chunk_buf_idx                     = saved_chunk_buf_idx;
        p_gen->p_chunk_buf[p_gen->chunk_buf_idx] = '\0';
        return false;
    }
    p_gen->is_first_item = false;
    return true;
}
