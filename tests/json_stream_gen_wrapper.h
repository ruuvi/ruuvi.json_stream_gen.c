/**
 * @file json_stream_gen_wrapper.h
 * @author TheSomeMan
 * @date 2023-08-11
 * @copyright Ruuvi Innovations Ltd, license BSD-3-Clause.
 */

#ifndef JSON_STREAM_GEN_WRAPPER_H
#define JSON_STREAM_GEN_WRAPPER_H

extern "C" {
#include "json_stream_gen.h"
}

class JsonStreamGenWrapper
{
public:
    JsonStreamGenWrapper(
        const json_stream_gen_cfg_t* const p_cfg,
        json_stream_gen_cb_generate_next_t cb_gen_next,
        const size_t                       ctx_size,
        void**                             p_p_ctx)
    {
        p_obj = json_stream_gen_create(p_cfg, cb_gen_next, ctx_size, p_p_ctx);
        if (!p_obj)
        {
            throw std::runtime_error("json_stream_gen_create failed");
        }
    }

    ~JsonStreamGenWrapper()
    {
        if (p_obj)
        {
            json_stream_gen_delete(&p_obj);
        }
    }

    // copy constructor and copy assignment operator are deleted to avoid double deletion
    JsonStreamGenWrapper(const JsonStreamGenWrapper&) = delete;
    JsonStreamGenWrapper&
    operator=(const JsonStreamGenWrapper&)
        = delete;

    // Move constructor and move assignment operator
    JsonStreamGenWrapper(JsonStreamGenWrapper&& other) noexcept
        : p_obj(other.p_obj)
    {
        other.p_obj = nullptr;
    }

    JsonStreamGenWrapper&
    operator=(JsonStreamGenWrapper&& other) noexcept
    {
        if (&other != this)
        {
            if (p_obj)
            {
                json_stream_gen_delete(&p_obj);
            }
            p_obj       = other.p_obj;
            other.p_obj = nullptr;
        }
        return *this;
    }

    // Getter for p_obj
    json_stream_gen_t*
    get() const
    {
        return p_obj;
    }

private:
    json_stream_gen_t* p_obj;
};

#endif // JSON_STREAM_GEN_WRAPPER_H
