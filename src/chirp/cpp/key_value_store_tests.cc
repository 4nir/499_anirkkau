#include <gtest/gtest.h>

#include "key_value_store.h"

// adds a value to the KeyValueStore
TEST(PutGetTest, Simple) {
  KeyValueStore test_store;
  test_store.Put("1testkey", "1testvalue");

  const std::vector<std::string>& values = test_store.Get("1testkey");
  ASSERT_EQ("1testvalue", values.at(0));
  ASSERT_EQ(1, values.size());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}