#ifndef SERVICE_LAYER_CLIENT_H
#define SERVICE_LAYER_CLIENT_H

#include <functional>
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

// Stream
using chirp::StreamReply;
using chirp::StreamRequest;

// gRPC client to communicate with ServiceLayerServiceImpl
class ServiceLayerClient {
 public:
  // Registers a new user with the given username
  // @username: username of new user
  // @return: string representing success or failure of registration
  std::string registeruser(const std::string& username);

  // Follows a new user with the given username
  // @username: username of new user
  // @to_follow: username of user to follow
  // @return: string representing success or failure of follow
  std::string follow(const std::string& username, const std::string& to_follow);

  // Posts new chirp
  // @username: username of chirper
  // @text: text of chirp
  // @parent_id: Parent ID of chirp; "0" if it's a root chirp
  // @return: string representing success or failure of chirp
  std::string chirp(const std::string& username, const std::string& text,
                    const std::string& parent_id);

  // Reads a thread from a given id
  // @id: the beginning of the chirp thread
  // @return: string representing success or failure of read
  std::string read(const std::string& id);

  // Monitors for a given username
  // @username: username
  // @return: string representing success or failure of read
  std::string monitor(const std::string& username);

  // Streams for chirps that contain `hashtag` for a given `username`
  // @username: username
  // @hashtag: hashtag
  // @return: the status of the request
  bool stream(const std::string& username, const std::string& hashtag,
              std::function<void(Chirp)> handle_response);

  ServiceLayerClient(std::shared_ptr<Channel> channel)
      : stub_(ServiceLayer::NewStub(channel)) {}

 private:
  // gRPC stub
  std::unique_ptr<ServiceLayer::Stub> stub_;
};

#endif  // SERVICE_LAYER_CLIENT_H