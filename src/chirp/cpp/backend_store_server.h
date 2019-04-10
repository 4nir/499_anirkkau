#ifndef BACKEND_SERVER_H
#define BACKEND_SERVER_H

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <iostream>
#include <string>
#include "backend_store.grpc.pb.h"
#include "key_value_store.h"
#include "service_layer_client.h"

// KeyValueStore server
class KeyValueStoreServiceImpl final : public KeyValueStore::Service {
 public:
  // Handles put command received from KeyValueStoreClient
  Status put(ServerContext* context, const PutRequest* request,
             PutReply* reply);

  // Handles get command received from KeyValueStoreClient
  Status get(ServerContext* context,
             ServerReaderWriter<GetReply, GetRequest>* stream);

  // Handles deletekey command received from KeyValueStoreClient
  Status deletekey(ServerContext* context, const DeleteRequest* request,
                   DeleteReply* reply);

  // Takes in the store map, the starting chirp ID to read from, and returns a
  // vector of chirp replies in byte form
  std::vector<std::string>* DFSReplyThread(
      std::map<std::string, std::vector<std::string> > chirp_map,
      std::vector<std::string>* reply_thread_vec, std::string chirp_id);

 private:
  // Stores map that tracks users and chirps
  // Users: The key holds the username (string), and the value holds the
  // usernames that the key follows (vector of strings) Chirps: The key holds the
  // chirp ID (string), and the value holds the following:
  // 1) The 0th index holds the serialized form of the chirp
  // 2) Indexes 1 to n-1 holds the chirp IDs of chirps in reply to the key
  KeyValueStoreClass store_;
};

#endif  // BACKEND_SERVER_H