#ifndef SERVICE_LAYER_CLIENT_H
#define SERVICE_LAYER_CLIENT_H

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include "backend_store.grpc.pb.h"
#include "service_layer.grpc.pb.h"

// Client Includes
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

// Server Includes
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

// Chirp Includes
using chirp::DeleteReply;
using chirp::DeleteRequest;
using chirp::GetReply;
using chirp::GetRequest;
using chirp::KeyValueStore;
using chirp::PutReply;
using chirp::PutRequest;

using chirp::Chirp;
using chirp::ChirpReply;
using chirp::ChirpRequest;
using chirp::FollowReply;
using chirp::FollowRequest;
using chirp::MonitorReply;
using chirp::MonitorRequest;
using chirp::ReadReply;
using chirp::ReadRequest;
using chirp::RegisterReply;
using chirp::RegisterRequest;
using chirp::ServiceLayer;
using chirp::Timestamp;

class ServiceLayerClient {
 public:
  ServiceLayerClient(const bool& testing);
  std::string registeruser(const std::string& username);
  std::string chirp(const std::string& username, const std::string& text,
                    const std::string& parent_id);
  std::string follow(const std::string& username, const std::string& to_follow);
  std::string read(const std::string& id);
  std::string monitor(const std::string& username);

  ServiceLayerClient(std::shared_ptr<Channel> channel)
      : stub_(ServiceLayer::NewStub(channel)) {}

 private:
  std::unique_ptr<ServiceLayer::Stub> stub_;
  // TODO: Add direct KVSStore member variable for testing to bypass GRPC
  bool testing_;
  // KeyValueStoreClass test_store_;
};

#endif  // SERVICE_LAYER_CLIENT_H