#ifndef SERVICE_LAYER_H
#define SERVICE_LAYER_H

#include "key_value_store.h"

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>

class ServiceLayer {
  public:
    // Default constructor
    //TODO: Change so it takes in pointer to KVS client
    ServiceLayer();

  private:
    KeyValueStoreClass store_client_;
};


#endif  // SERVICE_LAYER_H