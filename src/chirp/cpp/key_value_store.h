#ifndef KEY_VALUE_STORE_H
#define KEY_VALUE_STORE_H

#include <map>
#include <mutex>
#include <string>

#include <grpcpp/grpcpp.h>
#include "backend_store.grpc.pb.h"

// Data structure that stores the state of usernames, followers, and chirps
class KeyValueStoreClass {
 public:
  KeyValueStoreClass(){};
  ~KeyValueStoreClass(){};

  // Adds key value pair to chirp_map_
  // @key: key under which new value will be added
  // @value: value to be added
  // @return: bool representing the success or failure of Put()
  bool Put(const std::string &key, const std::string &value);

  // Gets vector of relevent elements from chirp_map_
  // @key: key that will be used to query chirp_map_
  // @return: vector of requested elements
  std::vector<std::string> Get(const std::string &key);

  // Deletes key from chirp_map_
  // @key: key under which new value will be deleted
  // @return: bool representing the success or failure of DeleteKey()
  bool DeleteKey(const std::string &key);

  // Checks for key membership in chirp_map_
  // @key: key that will be checked
  // @return: bool representing the presence of key in chirp_map_
  bool KeyExists(const std::string &key);

  // Returns current state of internal store
  // @key: key that will be checked
  // @return: chirp_map_
  std::map<std::string, std::vector<std::string> > ReturnStoreMap();

 private:
  // Map that tracks users and chirps
  // Users: The key holds the username (string), and the value holds the
  // usernames that the key follows (vector of strings) Chirps: The key holds
  // the chirp ID (string), and the value holds the following: 1) The 0th index
  // holds the serialized form of the chirp 2) Indexes 1 to n-1 holds the chirp
  // IDs of chirps in reply to the key
  std::map<std::string, std::vector<std::string> > chirp_map_;

  // Mutex to safely lock threads/clients
  std::mutex store_mutex_;
};

#endif  // KEY_VALUE_STORE_H