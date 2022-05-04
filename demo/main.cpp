// Copyright 2021 bartem1us filkovghjynj@mail.ru
#include <iostream>

#include "client.h"
int main(int argc, char *argv[]) {
  try {
    if (argc != 6) {
      std::cout << "Usage: <server> <port> <target> <login> <input>\n";
      std::cout << "Example:\n";
      std::cout << " 0.0.0.0 3030 /v1/api/suggest <your login> <request>\n";
      return 0;
    }

    boost::asio::io_context io_context_;

    std::make_shared<Client>(io_context_)
        ->run(argv[1], argv[2], argv[3], argv[4], argv[5]);

    io_context_.run();
  } catch (std::exception &e) {
    std::cout << "Exception: " << e.what() << "\n";
  }
}
