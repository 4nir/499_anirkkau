#include "key_value_store.h"

bool KeyValueStoreClass::Put(const std::string &key, const std::string &value) {
  if (key != "") {
    std::lock_guard<std::mutex> lock(store_mutex_);
    chirp_map_[key].push_back(value);
    return true;
  } else {
    return false;
  }
}

std::vector<std::string> KeyValueStoreClass::Get(const std::string &key) {
  std::vector<std::string> values;
  if (KeyExists(key)) {
    std::lock_guard<std::mutex> lock(store_mutex_);
    values = chirp_map_[key];
  }
  return values;
}

bool KeyValueStoreClass::DeleteKey(const std::string &key) {
  if (KeyExists(key)) {
    std::lock_guard<std::mutex> lock(store_mutex_);
    chirp_map_.erase(key);
    return true;
  } else {
    return false;
  }
}

bool KeyValueStoreClass::KeyExists(const std::string &key) {
  std::lock_guard<std::mutex> lock(store_mutex_);
  auto it = chirp_map_.find(key);
  if (it != chirp_map_.end()) {
    return true;
  }
  return false;
}

std::map<std::string, std::vector<std::string> >
KeyValueStoreClass::ReturnStoreMap() {
  return chirp_map_;
}