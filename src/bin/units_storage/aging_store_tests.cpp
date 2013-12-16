// Copyright (c) 2012 Hasso-Plattner-Institut fuer Softwaresystemtechnik GmbH. All rights reserved.
#include "testing/test.h"

#include "storage/AgingStore.h"
#include "storage/TableGenerator.h"

namespace hyrise {
namespace storage {

class AgingStoreTests : public Test {};

TableGenerator tga(true);

TEST_F(AgingStoreTests, basic_instantiation) {
  AgingStore s;
}

/// Test that stores based on modifiable tables can be modified even in their main
TEST_F(AgingStoreTests, test_write_through) {
  auto table = tga.one_value_delta(1, 1, 0);
  AgingStore s(table);
  s.setValue<hyrise_int_t>(0, 0, 10);
}

/// Test that stores based on non-modifiable tables can't be modified,
/// when writing values out of order
TEST_F(AgingStoreTests, test_write_through_fail) {
  auto table = tga.one_value(1, 1, 0);
  AgingStore s(table);
  s.setValue<hyrise_int_t>(0, 0, 11); // 11 is larger than 0, so it'll
                                      // just add an extra item to dict
#ifdef EXPENSIVE_ASSERTIONS
  ASSERT_THROW({ s.setValue<hyrise_int_t>(0, 0, 10); }, std::runtime_error);
  // fails since the order_preserving dictionary has to be filled in-order
#endif
}

} } // namespace hyrise::storage

