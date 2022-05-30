#include <cassert>
#include <iostream>
#include <memory>
#include <mutex>
#include <set>

#include <grpc/grpc.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <thing.grpc.pb.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;

enum { WHITE, BLACK };

std::string get_token(const std::string &ip, const std::string &time) {
  return ip + ":" + time;
}

class GameImpl final : public Game::Service {
public:
  explicit GameImpl() {}
  Status Auth(ServerContext *context, const AuthRequest *request,
              AuthResponse *response) override { // save login-password
    std::string generate_token = get_token(context->peer(), request->time());
    response->set_token(generate_token);
    users_tokens.insert(generate_token);

    std::cerr << "new user with token = " << response->token() << std::endl;
    return Status::OK;
  }
  Status StartGame(ServerContext *context, const StartGameRequest *request,
                   StartGameResponse *response) override {
    std::string user_token = request->token();
    ready_to_play.insert(user_token);
    response->set_right_insert(true);
    return Status::OK;
  }
  Status SearchGame(ServerContext *context, const SearchGameRequest *request,
                    SearchGameResponse *response) override {
    std::string user_token = request->token();
    if (!ready_to_play.count(user_token)) {
      response->set_color(WHITE);
      response->set_search_accept(true);
    } else {
      if (static_cast<int>(ready_to_play.size()) > 1) {
        std::string first = user_token;
        ready_to_play.erase(first);
        std::string second = *ready_to_play.begin();
        ready_to_play.erase(second);

        current_game[second] = first;
        response->set_color(BLACK);
        response->set_search_accept(true);
      } else {
        response->set_color(BLACK);
        response->set_search_accept(false);
      }
    }
    return Status::OK;
  }
  Status MakeTurn(ServerContext *context, const MakeTurnRequest *request,
                  MakeTurnResponse *response) override {
    // for (auto &to : current_game) {
    //   std::cerr << to.first << ", " << to.second << std::endl;
    // }
    std::string token = request->token();
    if (current_game.count(token)) {
      std::string other_player_token = current_game[token];
      // std::cerr << "first token = " << token << "; second token = "
      //           << other_player_token << std::endl;
      if (1 == 1) { // correct motion
        response->set_correct_motion(true);
        current_game.erase(token);
        current_game[other_player_token] = token;
      } else {
        response->set_correct_motion(false);
      }
    }
    return Status::OK;
  }
  Status IsAlive(ServerContext *context, const IsAliveRequest *request,
                 IsAliveResponse *response) override {
    std::string token = request->token();
    if (current_game.count(token)) {
      response->set_enemy_motion(true);
    } else {
      response->set_enemy_motion(false);
    }
    return Status::OK;
  }

private:
  std::mutex mu_;
  std::set<std::string> users_tokens;
  std::set<std::string> ready_to_play; // tokens
  std::map<std::string, std::string> current_game;
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  GameImpl service;
  ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << "..." << std::endl;

  server->Wait();
}

int main(int argc, char **argv) {
  RunServer();
  return 0;
}