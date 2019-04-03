#ifndef SERVICE_SERVER_H
#define SERVICE_SERVER_H

#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "backend_store.grpc.pb.h"
#include "service_layer.grpc.pb.h"
#include "service_layer_client.h"

// Logic and data behind the server's behaviour - add implementation here.
class ServiceLayerServiceImpl final : public ServiceLayer::Service {
 public:
  Status registeruser(ServerContext* context, const RegisterRequest* request,
                      RegisterReply* reply);
  Status chirp(ServerContext* context, const ChirpRequest* request,
               ChirpReply* reply);
  Status follow(ServerContext* context, const FollowRequest* request,
                FollowReply* reply);
  Status read(ServerContext* context, const ReadRequest* request,
              ReadReply* reply);

  Status monitor(ServerContext* context, const MonitorRequest* request,
                 ServerWriter<MonitorReply>* writer);
  std::string GenerateChirpID();

 private:
  std::vector<std::string> chirp_log_;
  int chirp_count_ = 0;
};

#endif  // SERVICE_SERVER_H
