#include <iostream>

#include <boat/Config.h>
#include <boat/Server.h>

class HelloWorldHandler final : public boat::Handler {
 public:
  HelloWorldHandler() = default;
  ~HelloWorldHandler() override = default;

 public:
  boat::Response handle_request(boat::ContextPtr ctx,
                                boat::Request&& req) override {
    boat::StringResponse str_resp{boost::beast::http::status::ok, req.version()};
    str_resp.set(boost::beast::http::field::server, boat::constant::k_server_name);
    str_resp.set(boost::beast::http::field::content_type,
             boat::constant::k_mime_txt);
    str_resp.body() = "hello,world";
    str_resp.keep_alive(req.keep_alive());

    boat::Response response{std::move(str_resp)};
    return response;
  }
};

int main(int argc, char* argv[]) {
  std::cout << "hello, this is boat demo" << std::endl;

  boat::Config config{};
  config.set_port(8080);

  boat::Server server{config};

  const auto hello_world_handler = std::make_shared<HelloWorldHandler>();
  server.register_handler("/hello", boost::beast::http::verb::get, hello_world_handler);

  server.init();
  server.run();

  return 0;
}