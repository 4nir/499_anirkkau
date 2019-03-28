#ifndef KEY_VALUE_STORE_H_
#define KEY_VALUE_STORE_H_

#include <string>
#include <map>
#include <mutex>

#include <grpcpp/grpcpp.h>
#include "backend_store.grpc.pb.h"

class KeyValueStoreClass {
  public:
  KeyValueStoreClass(){};
  ~KeyValueStoreClass(){};

  void Put(const std::string &key, const std::string &value);

  std::vector<std::string> Get(const std::string &key);

  void DeleteKey(const std::string &key);

  bool KeyExists(const std::string &key);

  std::map<std::string, std::vector<std::string> > ReturnStoreMap();

 private:
  std::map<std::string, std::vector<std::string> > chirp_map_;
  /*
    Mutex to safely lock threads/clients
  */
  std::mutex store_mutex_;
};


#endif