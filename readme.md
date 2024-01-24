# Boat

Boat is a simple HTTP application framework that bases on Boost.

## Features

- Based on Boost.
- HTTP routing table.
- User-defined HTTP handlers.
- User-defined HTTP middlewares.
- Fully asynchronous.

## Requirement

- Boost 1.80+
- OpenSSL 1.0+
- CMake 3.20+
- C++ 20

## Get Started

### Hello World

Here comes a HelloWorld:

```c++
#include <boat/Config.h>
#include <boat/Server.h>

class HelloWorldHandler : public blank::Handler {
public:
  HelloWorldHandler() : blank::Handler() {}
  virtual ~HelloWorldHandler() = default;

public:
  virtual blank::Response handle_request(blank::ContextPtr ctx,
                                         blank::Request &&req) {
    using namespace blank;
    StringResponse resp{http::status::ok, req.version()};
    resp.set(http::field::server, blank::constant::k_server_name);
    resp.set(http::field::content_type, blank::constant::k_mime_txt);
    resp.body() = "hello, world";
    resp.keep_alive(req.keep_alive());

    return resp;
  }
};

int main(int argc, char *argv[]) {
  // setup a http server config
  blank::Config config{};
  // for more extra option, check Config.h
  config.set_port(8080);

  // setup the http server
  blank::Server server{config};

  // makeup the helloworld http handler
  auto hello_world_handler = std::make_shared<HelloWorldHandler>();
  server.register_handler("/hello", blank::http::verb::get, hello_world_handler);
    
  // initialize & run
  server.init();
  server.run();

  return 0;
}

```

When the server is running on port `8080`, we can make a request via CURL like:

```shell
curl http://0.0.0.0:8080/hello -i
```

And get:

```shell
HTTP/1.1 200 OK
Server: Boat
Content-Type: text/plain
Content-Length: 12

hello, world
```

Meanwhile, the server will give logs like:

```shell
[2021-Oct-21 11:10:40.376325] [0x00007f0363937c00] [info] running BlankHTTPServer with config: {"log_level":1,"threads":8,"address":"0.0.0.0","port":8080,"timeout":30,"log_filename":"","enable_ssl":false,"cert_path":"","pri_key_path":"","request_header_limit":8192,"request_body_limit":65536,"enable_spin":false}
[2021-Oct-21 11:10:46.121793] [0x00007f035ee2c700] [info] [200] [GET] [/hello] [1.83e-05]
```

That's it!



### User-defined Handler

Like the `HelloWorldHandler` we has defined in the previous section, it is easy to defined another one in the same pattern:

```cpp
#include <boat/Handler.h>
#include <boat/Response.h>

class EchoHandler : public blank::Handler {
public:
  EchoHandler() : Handler() {}
  virtual ~EchoHandler() = default;

public:
  virtual blank::Response handle_request(blank::ContextPtr ctx,
                                         blank::Request &&req) {
    using namespace blank;
    StringResponse resp{http::status::ok, req.version()};
    resp.set(http::field::server, blank::constant::k_server_name);
    resp.set(http::field::content_type, blank::constant::k_mime_txt);
    resp.body() = req.body();
    resp.keep_alive(req.keep_alive());

    return resp;
  }
};
```

Then register it into server:

```shell
  auto echo_handler = std::make_shared<EchoHandler>();
  server.register_handler("/echo", blank::http::verb::post, echo_handler);
```

Use CURL to validate it:

```shel
curl -X POST -d 'hello, repeater' http://0.0.0.0:8080/echo -i
```

The repeater says:

```shell
HTTP/1.1 200 OK
Server: Boat
Content-Type: text/plain
Content-Length: 15

hello, repeater
```

Done!



### Wildcard Mapping

Yep, Boat allows wildcard mapping. For example, there is a API path designed as:

```reStructuredText
/hello/[DEPARTMENT]/[USER]
```

Here comes the handler method:

```c++
class HelloUserHandler : public blank::Handler {
public:
  HelloUserHandler() : Handler() {}
  virtual ~HelloUserHandler() = default;

public:
  virtual blank::Response handle_request(blank::ContextPtr ctx,
                                         blank::Request &&req) {
  using namespace blank;
    StringResponse resp{http::status::ok, req.version()};
    resp.set(http::field::server, blank::constant::k_server_name);
    resp.set(http::field::content_type, blank::constant::k_mime_txt);

    std::stringstream ss{};
    ss << "Hello, " << ctx->get_param("user") << " from "
       << ctx->get_param("department");
    resp.body() = ss.str();

    resp.keep_alive(req.keep_alive());
    return resp;
  }
};
```

Then register it in server:

```cpp
  auto hello_user_handler = std::make_shared<HelloUserHandler>();
  server.register_handler("/hello/:department/:user", blank::http::verb::get,
                          hello_user_handler);
```

The wildcard is defined in format `:param` in register path like above.

Use CURL to validate it:

```shel
curl http://0.0.0.0:8080/hello/it/blank -i
```

Will get:

```shell
HTTP/1.1 200 OK
Server: Boat
Content-Type: text/plain
Content-Length: 20

Hello, blank from it
```

Done!



### Handler Context

It's easy to get request path, query and other useful information of current request from `Context`:

```c++
class CtxHandler : public blank::Handler {
public:
  CtxHandler() : blank::Handler() {}
  virtual ~CtxHandler() = default;

public:
  virtual blank::Response handle_request(blank::ContextPtr ctx,
                                         blank::Request &&req) {
    using namespace blank;
    StringResponse resp{http::status::ok, req.version()};
    resp.set(http::field::server, blank::constant::k_server_name);
    resp.set(http::field::content_type, blank::constant::k_mime_txt);
    
    std::stringstream ss{};
    ss << "Path: " << ctx->get_path() << "\n"
      << "Query: " << ctx->get_query() << "\n";
    auto values = ctx->get_query("key");
    if (!values.empty()) {
      ss << "query key with value(s):";
      for (auto &v : values) {
        ss << v << ' ';
      }
    }
    
    resp.body() = ss.str();
    resp.keep_alive(req.keep_alive());
    return resp;
  }
};
```

By requesting via CURL:

```shell
curl 'http://0.0.0.0:8080/ctx?key=v1,v2,v3' -i
```

Request related details will be printed:

```shell
HTTP/1.1 200 OK
Server: Boat
Content-Type: text/plain
Content-Length: 64

Path: /ctx
Query: key=v1,v2,v3
query key with value(s):v1 v2 v3
```

(some codes may get adapted to Boost.URL in the future)



### Middlewares & Handle Chain

A `handle chain` is the sequential combination of 0-N `middleware` following with one final `handler` at then end, which allows reusing of small by useful logic blocks.

For example, we can define a body repeater like:

```c++
class RepeaterMiddleware : public blank::Middleware {
public:
  RepeaterMiddleware() : Middleware(){};
  virtual ~RepeaterMiddleware() = default;

public:
  virtual blank::Response handle_request(blank::ContextPtr ctx,
                                         blank::Request &&req) {
    auto resp = next(ctx, std::move(req));

    if (resp.is_string_response()) {
      auto &str_resp = resp.get_string_response_ref().value();
      auto body = str_resp.body();
      str_resp.body() += body;
    }

    return resp;
  }
};
```

Then setup a handle chain:

```c++
  auto repeater = std::make_shared<RepeaterMiddleware>();
  std::vector<blank::MiddlewarePtr> middlewares{repeater};
  auto chain =
      std::make_shared<blank::HandleChain>(middlewares, hello_world_handler);
  server.register_chain("/hello/repeat", blank::http::verb::get, chain);
```

By requesting:

```shell
curl http://0.0.0.0:8080/hello/repeat -i
```

Will get:

```shell
HTTP/1.1 200 OK
Server: Boat
Content-Type: text/plain
Content-Length: 24

hello, worldhello, world
```



### Carry Shared Data

It's important to pass shared data through the whole handle chain in some circumstances.

To set a shared data in the handle chain, use:

```shell
blank::Response
SharedDataSetterMiddleware::handle_request(blank::ContextPtr ctx,
                                           blank::Request &&req) {
  auto now = std::chrono::system_clock::now();
  ctx->set_shared_data(now);
  auto resp = next(ctx, std::move(req));

  return resp;
}
```

Then get it from the other middleware or handler:

```shell
blank::Response
SharedDataGetterMiddleware::handle_request(blank::ContextPtr ctx,
                                           blank::Request &&req) {
  auto resp = next(ctx, std::move(req));

  auto shared_data = ctx->get_shared_data();
  auto time = std::any_cast<std::chrono::system_clock::time_point>(shared_data);

  std::cout << time.time_since_epoch().count() << std::endl;

  return resp;
}
```



### HTTP Client

The Boat also contains a HTTP client:

```c++
blank::Response BoostHandler::handle_request(blank::ContextPtr ctx,
                                             blank::Request &&req) {
  using namespace blank;

  Request boost_org_get_request{};
  boost_org_get_request.version(11);
  boost_org_get_request.method(http::verb::get);
  boost_org_get_request.target("/");
  boost_org_get_request.set(http::field::host, "www.boost.org");

  Client cli{"www.boost.org", "80", std::chrono::seconds{30},
             ctx->get_executor()};
  StringResponse resp;
  beast::error_code ec;
  cli.DoRequest(boost_org_get_request, resp, ctx->get_yield_context(), ec);
  if (ec) {
    StringResponse err_resp{http::status::internal_server_error, req.version()};
    return err_resp;
  }

  return resp;
}
```

Just do it in the Beast way.



### Logging

The Boat use Boost.Log as logging module. By default, the Boat `Server` initialized with an asynchronous logger, and passes the logger to all `Context`.

Besides, a default middleware is wrapped with all `Handler` and `HandleChain`.

If you want to use synchronous logger, add the synchronous option while initializing the `Server`:

```c++
server.init(blank::LoggerType::Sync);
```

Or, if you don't want any handler/middleware log, you may disable the access logging like:

```c++
server.register_chain("/hello/shared", blank::http::verb::get, chain, false);
server.register_handler("/hello", blank::http::verb::get, handler, false);
```

However, by now, you can't disable the logging when 404 not found.



## Build & Install

To build & install Boat, take the following steps:

```shell
cd Boat/
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/where/to/install -DCMAKE_BUILD_TYPE=Release ..
make -j4
make install
```

You may adapt `CMAKE_INSTALL_PREFIX` and `CMAKE_BUILD_TYPE` to your own environment.



## Benchmark

(TODO: with rust-actix & go-gin)


## Todo
- default logging behaviour option on server


## Others

 Feel free to make any issue or pull request. 

 Hope this project helps you build your own application! 