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
#include <server/thing.grpc.pb.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;

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
    // std::cerr << "user_token = " << user_token << std::endl;
    if (!ready_to_play.count(user_token)) {
      response->set_color(0);
      response->set_search_accept(true);
    } else {
      if (static_cast<int>(ready_to_play.size()) > 1) {
        std::string first = user_token;
        ready_to_play.erase(first);
        std::string second = *ready_to_play.begin();
        ready_to_play.erase(second);

        current_game[second] = first;
        response->set_color(1);
        response->set_search_accept(true);
      } else {
        response->set_color(1);
        response->set_search_accept(false);
      }
    }
    return Status::OK;
  }
  Status MakeTurn(ServerContext *context, const MakeTurnRequest *request,
                  MakeTurnResponse *response) override {
    std::cerr << request->first_figure_row() << ' '
              << request->first_figure_column() << " - "
              << request->secon_figure_row() << ' '
              << request->secon_figure_column() << std::endl;
    std::string token = request->token();
    if (current_game.count(token)) {
      std::string other_player_token = current_game[token];
      std::cerr << "MakeTurn from " << token << " to " << other_player_token
                << std::endl;
      response->set_correct_motion(true);
      current_game.erase(token);
      current_game[other_player_token] = other_player_token;
      last_motion[other_player_token] = {
          {request->first_figure_row(), request->first_figure_column()},
          {request->secon_figure_row(), request->secon_figure_column()}};
    }
    return Status::OK;
  }
  Status IsAlive(ServerContext *context, const IsAliveRequest *request,
                 IsAliveResponse *response) override {
    std::string token = request->token();
    std::cerr << "IsAlive for " << token << std::endl;
    if (last_motion.count(token)) {
      response->set_enemy_motion(true);
      std::pair<std::pair<int, int>, std::pair<int, int>> turn =
          last_motion[token];
      response->set_first_figure_row(turn.first.first);
      response->set_first_figure_column(turn.first.second);
      response->set_secon_figure_row(turn.second.first);
      response->set_secon_figure_column(turn.second.second);
      last_motion.erase(token);
    } else {
      response->set_enemy_motion(false);
      response->set_first_figure_row(-1);
      response->set_first_figure_column(-1);
      response->set_secon_figure_row(-1);
      response->set_secon_figure_column(-1);
    }
    return Status::OK;
  }

private:
  std::mutex mu_;
  std::set<std::string> users_tokens;
  std::set<std::string> ready_to_play; // tokens
  std::map<std::string, std::string> current_game;
  std::map<std::string, std::pair<std::pair<int, int>, std::pair<int, int>>>
      last_motion;
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