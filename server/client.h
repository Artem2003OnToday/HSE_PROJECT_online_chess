#ifndef CLIENT_H_
#define CLIENT_H_

#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <server/thing.grpc.pb.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::Status;

using namespace std::chrono;
using pii = std::pair<int, int>;

class GameClient {
public:
  GameClient(std::shared_ptr<Channel> channel)
      : stub_(Game::NewStub(channel)) {}
  std::string Auth(const std::string &login, const std::string &password) {
    AuthRequest request;
    request.set_login(login);
    request.set_password(password);
    request.set_time(std::to_string(
        duration_cast<milliseconds>(system_clock::now().time_since_epoch())
            .count()));

    ClientContext context;
    AuthResponse response;
    Status status = stub_->Auth(&context, request, &response);
    if (status.ok()) {
      return response.token();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }
  bool StartGame() {
    StartGameRequest request;
    request.set_token(current_session_token);

    ClientContext context;
    StartGameResponse response;
    Status status = stub_->StartGame(&context, request, &response);
    if (!status.ok()) {
      std::cout << "StartGame fail...: " << status.error_code() << ": "
                << status.error_message() << std::endl;
    }
    return response.right_insert();
  }
  bool MakeTurn(int a, int b, int c, int d) {
    MakeTurnRequest request;
    request.set_first_figure_row(a);
    request.set_first_figure_column(b);
    request.set_secon_figure_row(c);
    request.set_secon_figure_column(d);
    request.set_token(current_session_token);

    ClientContext context;
    MakeTurnResponse response;
    Status status = stub_->MakeTurn(&context, request, &response);
    if (!status.ok()) {
      std::cout << "MakeTurn fail...: " << status.error_code() << ": "
                << status.error_message() << std::endl;
    }
    return response.correct_motion();
  }
  std::pair<int, int> SearchGame() {
    SearchGameRequest request;
    request.set_token(current_session_token);

    ClientContext context;
    SearchGameResponse response;
    Status status = stub_->SearchGame(&context, request, &response);
    if (!status.ok()) {
      std::cout << "StartGame fail...: " << status.error_code() << ": "
                << status.error_message() << std::endl;
    }
    return {response.search_accept(), response.color()};
  }
  std::pair<int, std::pair<pii, pii>> IsAlive() {
    IsAliveRequest request;
    request.set_token(current_session_token);

    ClientContext context;
    IsAliveResponse response;
    Status status = stub_->IsAlive(&context, request, &response);
    if (!status.ok()) {
      std::cout << "IsAlive fail...: " << status.error_code() << ": "
                << status.error_message() << std::endl;
    }
    /*
    return enemy motion or mark it on border here...
    */
    // std::cerr << response.first_figure_row() << " "
    //           << response.first_figure_column() << " - "
    //           << response.secon_figure_row() << " "
    //           << response.secon_figure_column() << std::endl;
    return {response.enemy_motion(),
            {{response.first_figure_row(), response.first_figure_column()},
             {response.secon_figure_row(), response.secon_figure_column()}}};
  }
  std::string current_session_token;

private:
  std::unique_ptr<Game::Stub> stub_;
};

#endif // CLIENT_H_