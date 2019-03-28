#include "key_value_store.h"

void KeyValueStore::Put(const std::string &key, const std::string &value){
  std::lock_guard<std::mutex> lock(store_mutex_);
  chirp_map_[key].push_back(value);
}

std::vector<std::string> KeyValueStore::Get(const std::string &key){
  std::lock_guard<std::mutex> lock(store_mutex_);
  std::vector<std::string> values = chirp_map_[key];
  return values;
}

void KeyValueStore::DeleteKey(const std::string &key){
  std::lock_guard<std::mutex> lock(store_mutex_);
  chirp_map_.erase(key);
}

bool KeyValueStore::KeyExists(const std::string &key){
  std::lock_guard<std::mutex> lock(store_mutex_);
  auto it = chirp_map_.find(key);
  if (it != chirp_map_.end()) {
    return true;
  }
  return false;
}