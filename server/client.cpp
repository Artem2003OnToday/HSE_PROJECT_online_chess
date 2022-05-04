#include <cassert>
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

StartMessage MakeStartMessage(const std::string &name) {
  StartMessage m;
  m.set_name(name);
  return m;
}

class RouteGuideClient {
public:
  RouteGuideClient(std::shared_ptr<Channel> channel)
      : stub_(RouteGuide::NewStub(channel)) {}
  void GetIn() {
    ClientContext context;
    std::shared_ptr<ClientReaderWriter<StartMessage, StartInfo>> stream(
        stub_->GetIn(&context));

    std::thread writer([stream]() {
      std::vector<StartMessage> messages{MakeStartMessage("Artemka")};
      for (const StartMessage &start : messages) {
        std::cout << "Sending message " << start.name() << std::endl;
        stream->Write(start);
      }
      stream->WritesDone();
    });
    StartInfo server_info;
    while (stream->Read(&server_info)) {
      std::unique_lock<std::mutex> lock(mu_);
      std::cout << "Got message " << server_info.id() << ", "
                << server_info.yourmotion() << std::endl;
    }

    std::cerr << "why not??" << std::endl;
    // [&]() {
    //   std::unique_lock<std::mutex> lock(mu_);
    //   std::cerr << "send to server <<Artemka>>" << std::endl;
    //   stream->Write(MakeStartMessage("Artemka"));
    //   stream->WritesDone();
    // }();
    writer.join();
    Status status = stream->Finish();
    if (!status.ok()) {
      std::cout << "GetIn rpc failed." << std::endl;
    }
  }

private:
  std::unique_ptr<RouteGuide::Stub> stub_;
  std::mutex mu_;
};

int main(int argc, char **argv) {
  RouteGuideClient guide(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  guide.GetIn();
  return 0;
}