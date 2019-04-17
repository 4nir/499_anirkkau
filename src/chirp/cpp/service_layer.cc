#include "service_layer.h"

ServiceLayer::ServiceLayer(KeyValueStoreClass* store_client) {
  store_client_ = store_client;
}

bool ServiceLayer::registeruser(const std::string& username) {
  if (username == "") {
    return false;
  }

  if (store_client_->KeyExists(username)) {
    return false;
  } else {
    store_client_->Put(username, "");
    return true;
  }
}

bool ServiceLayer::follow(const std::string& username,
                          const std::string& to_follow) {
  if (store_client_->KeyExists(username) &&
      store_client_->KeyExists(to_follow)) {
    store_client_->Put(username, to_follow);
    return true;
  } else {
    return false;
  }
}

bool ServiceLayer::chirp(const std::string& username, const std::string& text,
                         const std::string& parent_id) {
  // Generate Chirp
  Chirp chirp;
  chirp.set_username(username);
  chirp.set_text(text);
  std::string chirp_id = GenerateChirpID();
  chirp.set_id(chirp_id);
  chirp.set_parent_id(parent_id);

  // Serialize Chirp
  std::string chirp_str = chirp.SerializeAsString();

  if (parent_id == "0") {  // Root chirp

    // Store New Chirp
    store_client_->Put(chirp_id, chirp_str);
    // Push into chirp_log_. Every chirp gets logged in the Service Layer
    //(for Monitor)
    chirp_log_.push_back(chirp_str);
    return true;

  } else {  // Reply chirp

    if (store_client_->KeyExists(parent_id)) {  // Valid parent chirp ID
      // Store New Chirp
      store_client_->Put(chirp_id, chirp_str);

      // Add chirp ID to parent's reply vector
      store_client_->Put(parent_id, chirp_id);
      // Push into chirp_log_. Every chirp gets logged in the Service Layer
      //(for Monitor)
      chirp_log_.push_back(chirp_str);
      return true;
    } else {  // Invalid parent chirp ID
      return false;
    }
  }
}

std::vector<Chirp> ServiceLayer::read(const std::string& id) {
  // Grab current state of KeyValueStoreClass's internal map
  std::map<std::string, std::vector<std::string> > chirp_map =
      store_client_->ReturnStoreMap();

  // Recursive DFS search that returns vector of entire thread of serialized
  // chirps
  std::vector<std::string>* reply_thread_vec = new std::vector<std::string>();
  std::vector<std::string>* full_thread_vec;
  full_thread_vec = DFSReplyThread(chirp_map, reply_thread_vec, id);

  // full_thread_vec conatins serialized replies, so we will convert it to
  // a vector of chirp objects
  std::vector<Chirp> chirp_obj_thread;
  for (int i = 0; i < full_thread_vec->size(); i++) {
    std::string serialized_chirp = full_thread_vec->at(i);
    // Deserialize chirps
    Chirp chirp_catcher;
    chirp_catcher.ParseFromString(serialized_chirp);
    chirp_obj_thread.push_back(chirp_catcher);
  }

  // Now we have a vector of all replies in a vector of chirp objects
  // We then return this vector
  return chirp_obj_thread;
}

std::vector<Chirp> ServiceLayer::monitor(const std::string& username) {
  // Grab current state of KeyValueStoreClass's internal map
  std::map<std::string, std::vector<std::string> > chirp_map =
      store_client_->ReturnStoreMap();

  // Final vector of chirp objects to be returned
  std::vector<Chirp> chirp_obj_thread;

  if (!store_client_->KeyExists(username)) {  // Invalid Username
    return chirp_obj_thread;
  }

  std::vector<std::string> following_list = getFollowingList(username);
  if (following_list.size() == 0) {  // User doesn't follow anyone yet
    return chirp_obj_thread;
  }

  for (int i = 0; i < chirp_log_.size(); i++) {
    std::string current_chirp_bytes = chirp_log_.at(i);
    Chirp chirp_obj;
    chirp_obj.ParseFromString(current_chirp_bytes);
    std::string current_chirper = chirp_obj.username();

    // Check if user follows current_chirper
    for (std::string follow : following_list) {
      if (follow == current_chirper) {
        chirp_obj_thread.push_back(chirp_obj);
      }
    }
  }

  return chirp_obj_thread;
}

std::vector<Chirp> ServiceLayer::Stream(const std::string& username,
                                        const std::string& hashtag) {
  // TODO(tianhanl): add implementation
}

std::string ServiceLayer::GenerateChirpID() {
  chirp_count_++;
  std::string CurrentClientID = "cid//";
  std::string count_str = std::to_string(chirp_count_);
  CurrentClientID += count_str;

  return CurrentClientID;
}

std::vector<std::string>* ServiceLayer::DFSReplyThread(
    std::map<std::string, std::vector<std::string> > chirp_map,
    std::vector<std::string>* reply_thread_vec, std::string chirp_id) {
  auto it = chirp_map.find(chirp_id);
  if (it != chirp_map.end()) {
    std::vector<std::string> reply_vec = it->second;
    // Add chirp bytes to reply_thread_vec (Index 0)
    reply_thread_vec->push_back(reply_vec.at(0));

    if (reply_vec.size() > 1) {                     // Chirp has replies
      for (int i = 1; i < reply_vec.size(); i++) {  // Skip first index
        reply_thread_vec =
            DFSReplyThread(chirp_map, reply_thread_vec, reply_vec.at(i));
      }
      return reply_thread_vec;

    } else {  // End of sub-thread
      return reply_thread_vec;
    }
  } else {
    std::cout << "Error: chirp_id not found in chirp_map" << std::endl;
  }
}

std::vector<std::string> ServiceLayer::getFollowingList(
    const std::string& username) {
  // Grab current state of KeyValueStoreClass's internal map
  std::map<std::string, std::vector<std::string> > chirp_map =
      store_client_->ReturnStoreMap();
  auto it = chirp_map.find(username);
  std::vector<std::string> following_list = it->second;
  return following_list;
}