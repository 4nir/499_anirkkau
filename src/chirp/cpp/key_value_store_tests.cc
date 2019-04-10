#include <gtest/gtest.h>
#include "key_value_store.h"

// Tests default constructor for KeyValueStore
TEST(KeyValueStoreCreate, NewKeyValueStore) {
  KeyValueStoreClass store_client;
  ASSERT_NE(nullptr, &store_client);
}

// Tests Put method for KeyValueStore
// adds empty key to the KeyValueStore
TEST(PutGetTest, EmptyKey) {
  KeyValueStoreClass test_store;
  EXPECT_FALSE(test_store.Put("", "1testvalue"));
}

// Tests Get method for KeyValueStore
// Gets from invalid key should return empty vector
TEST(PutGetTest, InvalidKey) {
  KeyValueStoreClass test_store;
  std::vector<std::string> response = test_store.Get("made_up_key");
  EXPECT_EQ(0, response.size());
}

// Tests Put method for KeyValueStore
// adds a value to the KeyValueStore
TEST(PutGetTest, Simple) {
  KeyValueStoreClass test_store;
  test_store.Put("1testkey", "1testvalue");

  const std::vector<std::string>& values = test_store.Get("1testkey");
  ASSERT_EQ("1testvalue", values.at(0));
  ASSERT_EQ(1, values.size());
}

// Tests Put and Get method for KeyValueStore
// adds 3 values to the KeyValueStore
TEST(PutGetTest, ThreePutsThreeKeys) {
  KeyValueStoreClass test_store;
  test_store.Put("1testkey", "1testvalue");
  test_store.Put("2testkey", "2testvalue");
  test_store.Put("3testkey", "3testvalue");

  const std::vector<std::string>& values_1 = test_store.Get("1testkey");
  ASSERT_EQ("1testvalue", values_1.at(0));
  ASSERT_EQ(1, values_1.size());

  const std::vector<std::string>& values_2 = test_store.Get("2testkey");
  ASSERT_EQ("2testvalue", values_2.at(0));
  ASSERT_EQ(1, values_2.size());

  const std::vector<std::string>& values_3 = test_store.Get("3testkey");
  ASSERT_EQ("3testvalue", values_3.at(0));
  ASSERT_EQ(1, values_3.size());
}

// Tests Put and GET method for KeyValueStore
// adds 3 values to the same key in KeyValueStore
TEST(PutGetTest, ThreePutsOneKey) {
  KeyValueStoreClass test_store;
  test_store.Put("1testkey", "1testvalue");
  test_store.Put("1testkey", "2testvalue");
  test_store.Put("1testkey", "3testvalue");

  const std::vector<std::string>& values = test_store.Get("1testkey");
  ASSERT_EQ("1testvalue", values.at(0));
  ASSERT_EQ("2testvalue", values.at(1));
  ASSERT_EQ("3testvalue", values.at(2));
  ASSERT_EQ(3, values.size());
}

// Tests Delete method for KeyValueStore
// deletes key from KVS
TEST(DeleteTest, SimpleDelete) {
  KeyValueStoreClass test_store;
  test_store.Put("1testkey", "1testvalue");
  EXPECT_TRUE(test_store.DeleteKey("1testkey"));
}

// Tests Delete method for KeyValueStore
// deletes 3 keys from KVS
TEST(DeleteTest, ThreeKeyDelete) {
  KeyValueStoreClass test_store;
  test_store.Put("1testkey", "1testvalue");
  test_store.Put("2testkey", "2testvalue");
  test_store.Put("3testkey", "3testvalue");
  EXPECT_TRUE(test_store.DeleteKey("1testkey"));
  EXPECT_TRUE(test_store.DeleteKey("2testkey"));
  EXPECT_TRUE(test_store.DeleteKey("3testkey"));
}

// Tests Delete method for KeyValueStore
// deletes key from KVS
TEST(DeleteTest, InvalidDelete) {
  KeyValueStoreClass test_store;
  test_store.Put("1testkey", "1testvalue");
  EXPECT_FALSE(test_store.DeleteKey("invalid_key"));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}