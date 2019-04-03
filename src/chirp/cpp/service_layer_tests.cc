#include <gtest/gtest.h>

#include "service_layer.h"
#include "key_value_store.h"

// adds a value to the KeyValueStore
//TODO: Replace with Service Layer test
TEST(PutGetTest, Simple) {
  KeyValueStoreClass test_store;
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}