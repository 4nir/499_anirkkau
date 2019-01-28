#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "backend_store.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using chirp::KeyValueStore;
using chirp::PutRequest;
using chirp::PutReply;
using chirp::GetRequest;
using chirp::GetReply;
// using chirp::DeleteRequest;
// using chirp::DeleteReply;

class KeyValueStoreClient {
  public:
    KeyValueStoreClient(std::shared_ptr<Channel> channel)
      : stub_(KeyValueStore::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.

    // rpc put
    std::string put(const std::string& key, const std::string& value) {
      // Data we are sending to the server.
      PutRequest request;
      request.set_key(key);
      request.set_value(value);

      // Container for the data we expect from the server.
      PutReply reply;

      // Context for the client. It could be used to convey extra information to
      // the server and/or tweak certain RPC behaviors.
      ClientContext context;

      // The actual RPC.
      Status status = stub_->put(&context, request, &reply);

      // Act upon its status.
      if (status.ok()) {
        return "RPC succeeded";
      } else {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return "RPC failed";
      }
    }
  
      // rpc get
      std::string get(const std::string& key) {
      // Data we are sending to the server.
      PutRequest request;
      request.set_key(key);

      // Container for the data we expect from the server.
      PutReply reply;

      // Context for the client. It could be used to convey extra information to
      // the server and/or tweak certain RPC behaviors.
      ClientContext context;

      // The actual RPC.
      Status status = stub_->put(&context, request, &reply);

      // Act upon its status.
      if (status.ok()) {
        return "RPC succeeded";
      } else {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return "RPC failed";
      }
    }

      // rpc deletekey (TODO)

  

  private:
    std::unique_ptr<KeyValueStore::Stub> stub_;
};

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  KeyValueStoreClient backend_store(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  std::cout << "Successfully connected!" << std::endl;
  std::string user("key");
  std::string reply = backend_store.get(user);
  std::cout << "KeyValueStore replied: " << reply << std::endl;

  return 0;
}
