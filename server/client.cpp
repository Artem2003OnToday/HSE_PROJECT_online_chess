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
#include <thing.grpc.pb.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::Status;

using namespace std::chrono;
std::string token;

class GameClient {
public:
  GameClient(std::shared_ptr<Channel> channel)
      : stub_(Game::NewStub(channel)) {}
  std::string Auth(const std::string &login, const std::string &password) {
    AuthRequest request;
    request.set_login(login);
    request.set_password(password);
    // request.set_ip("11.1.1.1");
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
    request.set_token(token);

    ClientContext context;
    StartGameResponse response;
    Status status = stub_->StartGame(&context, request, &response);
    if (!status.ok()) {
      std::cout << "StartGame fail...: " << status.error_code() << ": "
                << status.error_message() << std::endl;
    }
    return response.right_insert();
  }
  bool SearchGame() {
    SearchGameRequest request;
    request.set_token(token);

    ClientContext context;
    SearchGameResponse response;
    Status status = stub_->SearchGame(&context, request, &response);
    if (!status.ok()) {
      std::cout << "StartGame fail...: " << status.error_code() << ": "
                << status.error_message() << std::endl;
    }
    return response.search_accept();
  }

private:
  std::unique_ptr<Game::Stub> stub_;
  std::mutex mu_;
};

int main(int argc, char **argv) {
  GameClient guide(grpc::CreateChannel("localhost:50051",
                                       grpc::InsecureChannelCredentials()));
  token = guide.Auth("login", "password");
  std::cout << token << std::endl;
  guide.StartGame();
  while (!guide.SearchGame()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  return 0;
}