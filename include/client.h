// Copyright 2021 bartem1us filkovghjynj@mail.ru
#ifndef TEMPLATE_CLIENT_H
#define TEMPLATE_CLIENT_H
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

namespace http = boost::beast::http;  // from <boost/beast/http.hpp>
using nlohmann::json;
using error_code = boost::beast::error_code;
using io_context = boost::asio::io_context;
using tcp = boost::asio::ip::tcp;
using buffer = boost::beast::flat_buffer;
using stream = boost::beast::tcp_stream;
using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>;

class Client : public std::enable_shared_from_this<Client> {
 public:
  explicit Client(io_context &ioc);

  void run(const std::string &server, const std::string &port,
           const std::string &path, const std::string &login,
           const std::string &input);

 private:
  tcp::resolver resolver_;
  stream stream_;

  buffer buffer_;

  Request request_;
  Response response_;

  void handle_resolve(error_code err, tcp::resolver::results_type results);

  void handle_connect(error_code err,
                      tcp::resolver::results_type::endpoint_type);

  void handle_write_request(error_code err, size_t);

  void handle_read(error_code err, size_t);
};
#endif  // TEMPLATE_CLIENT_H
