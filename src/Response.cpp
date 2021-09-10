#include "Response.h"

namespace blank
{
    StringResponsePtr Response::get_string_response() const
    {
        if (is_string_response_)
        {
            return string_response_;
        }
        return nullptr;
    }
    void Response::set_string_response(StringResponsePtr resp)
    {
        string_response_ = resp;
        is_string_response_ = true;
        file_response_ = nullptr;
        is_file_response_ = false;
    }
    FileResponsePtr Response::get_file_response() const
    {
        if (is_file_response_)
        {
            return file_response_;
        }
        return nullptr;
    }
    void Response::set_file_response(FileResponsePtr resp)
    {
        file_response_ = resp;
        is_file_response_ = true;
        string_response_ = nullptr;
        is_string_response_ = false;
    }

    bool Response::is_string_response() const
    {
        return is_string_response_;
    }

    bool Response::is_file_response() const
    {
        return is_file_response_;
    }

    void Response::prepare_payload()
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

    unsigned Response::get_status_code() const
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