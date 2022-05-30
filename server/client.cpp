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
std::string current_session_token;
int color;

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
  bool SearchGame() {
    SearchGameRequest request;
    request.set_token(current_session_token);

    ClientContext context;
    SearchGameResponse response;
    Status status = stub_->SearchGame(&context, request, &response);
    if (!status.ok()) {
      std::cout << "StartGame fail...: " << status.error_code() << ": "
                << status.error_message() << std::endl;
    }
    return response.search_accept();
  }
  bool MakeTurn() {
    MakeTurnRequest request;
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
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
  bool IsAlive() {
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
    return response.enemy_motion();
  }

private:
  std::unique_ptr<Game::Stub> stub_;
};

int main(int argc, char **argv) {
  GameClient guide(grpc::CreateChannel("localhost:50051",
                                       grpc::InsecureChannelCredentials()));
  current_session_token = guide.Auth("login", "password");
  std::cout << current_session_token << std::endl;
  guide.StartGame();
  while (!guide.SearchGame()) { // if game search is closed ...
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  std::cerr << guide.MakeTurn() << std::endl;

  while (1) {                  // if game end +-
    guide.MakeTurn();          // fix this
    while (!guide.IsAlive()) { // waiting enemy motion: fix youself desk ...
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "wow" << std::endl;
  }
  return 0;
}