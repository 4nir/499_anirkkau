#ifndef SERVICE_LAYER_H
#define SERVICE_LAYER_H

//TODO: Clean up includes

#include <grpcpp/grpcpp.h>
#include "backend_store.grpc.pb.h"
#include "service_layer.grpc.pb.h"
#include "key_value_store.h"
using chirp::Chirp;

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>

class ServiceLayer {
 public:
  // Default constructor
  // TODO: Change so it takes in pointer to KVS client
  ServiceLayer(KeyValueStoreClass* store_client);

  bool registeruser(const std::string& username);
  bool follow(const std::string& username, const std::string& to_follow);
  bool chirp(const std::string& username, const std::string& text,
                    const std::string& parent_id);
  std::vector<Chirp> read(const std::string& id);
  bool monitor(const std::string& username);
  std::string GenerateChirpID();
  //Takes in the store map, the starting chirp ID to read from, and returns a
  //vector of chirp replies in byte form
  std::vector<std::string>* DFSReplyThread(
    std::map<std::string, std::vector<std::string> > chirpMap,
    std::vector<std::string>* reply_thread_vec, std::string chirp_id);

 private:
  KeyValueStoreClass* store_client_;
  int chirp_count_ = 0;
};

#endif  // SERVICE_LAYER_H