// Copyright 2021 bartem1us filkovghjynj@mail.ru
#include "client.h"

Client::Client(io_context &ioc)
    : resolver_(boost::asio::make_strand(ioc)),
      stream_(boost::asio::make_strand(ioc)) {}

void Client::run(const std::string &server, const std::string &port,
                 const std::string &path, const std::string &login,
                 const std::string &input) {
  json data;
  data["input"] = input;
  request_.version(10);
  request_.method(http::verb::post);
  request_.target(path);
  request_.set(http::field::host, server);
  request_.set(http::field::content_type, path);
  request_.content_length(to_string(data).length());
  request_.set(http::field::authorization, login);
  request_.body() = to_string(data);

  resolver_.async_resolve(server, port,
                          boost::beast::bind_front_handler(
                              &Client::handle_resolve, shared_from_this()));
}

void Client::handle_resolve(error_code err,
                            tcp::resolver::results_type results) {
  if (!err) {
    stream_.async_connect(
        results, boost::beast::bind_front_handler(&Client::handle_connect,
                                                  shared_from_this()));
  } else {
    std::cout << "Error: " << err.message() << "\n";
  }
}

void Client::handle_connect(boost::beast::error_code err,
                            tcp::resolver::results_type::endpoint_type) {
  if (!err) {
    http::async_write(stream_, request_,
                      boost::beast::bind_front_handler(
                          &Client::handle_write_request, shared_from_this()));
  } else {
    std::cout << "Error: " << err.message() << "\n";
  }
}

void Client::handle_write_request(error_code err, size_t) {
  if (!err) {
    http::async_read(stream_, buffer_, response_,
                     boost::beast::bind_front_handler(&Client::handle_read,
                                                      shared_from_this()));
  } else {
    std::cout << "Error: " << err.message() << "\n";
  }
}

void Client::handle_read(error_code err, size_t) {
  if (!err) {
    std::cout << response_ << std::endl;

    stream_.socket().shutdown(tcp::socket::shutdown_both, err);

    if (err && err != boost::beast::errc::not_connected)
      std::cout << err << "shutdown";
  } else {
    std::cout << "Error: " << err << "\n";
  }
}
