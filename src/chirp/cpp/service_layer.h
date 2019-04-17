#ifndef SERVICE_LAYER_H
#define SERVICE_LAYER_H

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>
#include "backend_store.grpc.pb.h"
#include "key_value_store.h"
#include "service_layer.grpc.pb.h"
using chirp::Chirp;
using chirp::Timestamp;

class ServiceLayer {
 public:
  // Default constructor
  ServiceLayer(KeyValueStoreClass* store_client);

  // Registers a new user with the given username
  // @username: username of new user
  // @return: bool representing success or failure of registration
  bool registeruser(const std::string& username);

  // Follows a new user with the given username
  // @username: username of new user
  // @to_follow: username of user to follow
  // @return: bool representing success or failure of follow
  bool follow(const std::string& username, const std::string& to_follow);

  // Posts new chirp
  // @username: username of chirper
  // @text: text of chirp
  // @parent_id: Parent ID of chirp; "0" if it's a root chirp
  // @return: bool representing success or failure of chirp
  bool chirp(const std::string& username, const std::string& text,
             const std::string& parent_id);

  // Reads a thread from a given id
  // @id: the beginning of the chirp thread
  // @return: vector of chirp replies in object form
  std::vector<Chirp> read(const std::string& id);

  // Monitors for a given username
  // @username: monitoring user
  // @return: vector of relevent chirps in object form
  std::vector<Chirp> monitor(const std::string& username);

  // Streams for a given username and a hashtag
  // @username: username to stream for
  // @hashtag: hashtag to stream for
  // @return: vector of relevent chiprs
  std::vector<Chirp> Stream(const std::string& username,
                            const std::string& hashtag);

  // Generates chirp ID to be stored in KVS
  // @return: returns unique chirp ID in string form
  std::string GenerateChirpID();

  // Takes in the store map, the starting chirp ID to read from, and returns a
  // vector of chirp replies in byte form
  // @chirp_map: current state of chirp_map in KVS
  //@reply_thread_vec: empty array that will be populated as it's passed through
  // recursive calls
  // @return: vector of relevent chirps in the thread after the recursive DFS
  // search
  std::vector<std::string>* DFSReplyThread(
      std::map<std::string, std::vector<std::string> > chirp_map,
      std::vector<std::string>* reply_thread_vec, std::string chirp_id);

  // Returns the following list of a user
  // @username: username of user
  // @return: returns a vector of usernames that user is following
  std::vector<std::string> getFollowingList(const std::string& username);

 private:
  // Store client for testing
  KeyValueStoreClass* store_client_;

  // Keeps track of every chirp (for Monitor)
  std::vector<std::string> chirp_log_;

  // Count of chirps
  int chirp_count_ = 0;
};

#endif  // SERVICE_LAYER_H