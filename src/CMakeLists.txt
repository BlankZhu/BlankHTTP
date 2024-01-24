add_compile_options(-DBOOST_LOG_DYN_LINK)

add_library(boat SHARED
        boat/core/Context.cpp
        boat/core/Handler.cpp
        boat/core/HandleChain.cpp
        boat/core/Middleware.cpp
        boat/core/Response.cpp
        boat/core/Router.cpp
        boat/core/Session.cpp
        boat/log/AsyncLogger.cpp
        boat/log/LoggerInterface.cpp
        boat/log/SyncLogger.cpp
        boat/Client.hpp
        boat/Config.cpp
        boat/Server.cpp
        boat/Utilities.cpp
)

target_link_libraries(boat Boost::atomic
        Boost::context
        Boost::coroutine
        Boost::log
        Boost::log_setup
        Boost::json
        Boost::thread
        Boost::url)
target_link_libraries(boat OpenSSL::Crypto OpenSSL::SSL)