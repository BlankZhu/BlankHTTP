#include "BlankHttpResponse.h"

namespace blank
{
    StringResponsePtr BlankHttpResponse::get_string_response() const
    {
        if (is_string_response_)
        {
            return string_response_;
        }
        return nullptr;
    }
    void BlankHttpResponse::set_string_response(StringResponsePtr resp)
    {
        string_response_ = resp;
        is_string_response_ = true;
        file_response_ = nullptr;
        is_file_response_ = false;
    }
    FileResponsePtr BlankHttpResponse::get_file_response() const
    {
        if (is_file_response_)
        {
            return file_response_;
        }
        return nullptr;
    }
    void BlankHttpResponse::set_file_response(FileResponsePtr resp)
    {
        file_response_ = resp;
        is_file_response_ = true;
        string_response_ = nullptr;
        is_string_response_ = false;
    }

    bool BlankHttpResponse::is_string_response() const
    {
        return is_string_response_;
    }

    bool BlankHttpResponse::is_file_response() const
    {
        return is_file_response_;
    }

    void BlankHttpResponse::prepare_payload()
    {
        if (file_response_ != nullptr)
        {
            file_response_->prepare_payload();
            return;
        }
        if (string_response_ != nullptr)
        {
            string_response_->prepare_payload();
            return;
        }
    }

    unsigned BlankHttpResponse::get_status_code() const
    {
        if (file_response_ != nullptr)
        {
            return file_response_->result_int();
        }
        if (string_response_ != nullptr)
        {
            return string_response_->result_int();
        }
        return static_cast<unsigned>(http::status::internal_server_error);
    }
};