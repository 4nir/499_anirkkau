#include <iostream>
#include <random>
#include <string>
#include <thread>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include "backend_store.grpc.pb.h"
#include "backend_store_client.h"
#include "service_layer.grpc.pb.h"
#include "service_layer_client.h"

// rpc registeruser
std::string ServiceLayerClient::registeruser(const std::string& username) {
  if (username == "") {
    return "Error: Empty username";
  }
  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // Data we are sending to the server.
  RegisterRequest request;
  request.set_username(username);

  // Container for the data we expect from the server.
  RegisterReply reply;

  Status status = stub_->registeruser(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return username + " has been successfully registered!";
  } else {
    return "Error: Username already exists.";
  }
}

std::string ServiceLayerClient::chirp(const std::string& username,
                                      const std::string& text,
                                      const std::string& parent_id) {
  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // Data we are sending to the server.
  ChirpRequest request;
  request.set_username(username);
  request.set_text(text);
  request.set_parent_id(parent_id);

  // Container for the data we expect from the server.
  ChirpReply reply;

  // The actual RPC.
  Status status = stub_->chirp(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return username + "\'s chirp was posted.";
  } else {
    return "Error: Invalid chirp ID.";
  }
}

std::string ServiceLayerClient::follow(const std::string& username,
                                       const std::string& to_follow) {
  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // Data we are sending to the server.
  FollowRequest request;
  request.set_username(username);
  request.set_to_follow(to_follow);

  // Container for the data we expect from the server.
  FollowReply reply;

  // The actual RPC.
  Status status = stub_->follow(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return username + " now follows " + to_follow;
  } else {
    return "Error: username or follower does not exist.";
  }
}

std::string ServiceLayerClient::read(const std::string& id) {
  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // Data we are sending to the server.
  ReadRequest request;
  request.set_chirp_id(id);

  // Container for the data we expect from the server.
  ReadReply reply;

  // The actual RPC.
  Status status = stub_->read(&context, request, &reply);

  std::string final_thread = reply.chirp_thread();

  // Act upon its status.
  if (status.ok()) {
    return final_thread;
  } else {
    return "Error: Invalid chirp ID.";
  }
}

std::string ServiceLayerClient::monitor(const std::string& username) {
  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // Data we are sending to the server.
  MonitorRequest request;
  request.set_username(username);

  // Container for the data we expect from the server.
  MonitorReply reply;

  std::unique_ptr<ClientReader<MonitorReply> > reader(
      stub_->monitor(&context, request));

  std::string chirp_text;
  while (reader->Read(&reply)) {
    std::string chirp_bytes = reply.chirp_bytes();
    if (chirp_bytes != "") {
      Chirp chirp_catcher;
      chirp_catcher.ParseFromString(chirp_bytes);
      chirp_text = chirp_catcher.text();
    } else {  // User isn't a follower
      chirp_text = "";
    }
  }
  Status status = reader->Finish();

  // Act upon its status.
  if (status.ok()) {
    return chirp_text;
  } else {
    return "Error: Invalid monitor request.";
  }
}

bool ServiceLayerClient::stream(const std::string& username,
                                const std::string& hashtag,
                                std::function<void(Chirp)> handle_response) {
  // TODO(tianhanl): add implementation of the function
  return true;
}
