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

StartInfo MakeStartInfo(int id, bool yourMotion) {
  StartInfo i;
  i.set_id(id);
  i.set_yourmotion(yourMotion);
  return i;
}

class RouteGuideImpl final : public RouteGuide::Service {
public:
  explicit RouteGuideImpl() {}
  Status GetIn(ServerContext *context,
               ServerReaderWriter<StartInfo, StartMessage> *stream) override {
    std::cerr << "server sending..." << std::endl;
    [&]() {
      std::unique_lock<std::mutex> lock(mu_);
      stream->Write(MakeStartInfo(1, false));
    }();

    std::cerr << "server read" << std::endl;
    StartMessage note;
    while (stream->Read(&note)) {
      std::unique_lock<std::mutex> lock(mu_);
      // stream->Write(MakeStartInfo(1, false));
      std::cerr << note.name() << std::endl;
    }

    return Status::OK;
  }

private:
  std::mutex mu_;
  std::set<std::string> users;
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  RouteGuideImpl service;
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