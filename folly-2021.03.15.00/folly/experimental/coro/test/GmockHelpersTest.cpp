/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdexcept>
#include <string>
#include <vector>

#include <folly/Portability.h>

#include <gtest/gtest-death-test.h>

#include <folly/experimental/coro/GmockHelpers.h>

#include <folly/experimental/coro/BlockingWait.h>
#include <folly/experimental/coro/Task.h>

#include <folly/portability/GMock.h>
#include <folly/portability/GTest.h>

#if FOLLY_HAS_COROUTINES

using namespace ::testing;
using namespace folly::coro::gmock_helpers;

namespace {
class Foo {
 public:
  virtual ~Foo() = default;
  virtual folly::coro::Task<std::vector<std::string>> getValues() = 0;

  virtual folly::coro::Task<std::string> getString() = 0;

  virtual folly::coro::Task<void> getVoid() = 0;
};

class MockFoo : Foo {
 public:
  MOCK_METHOD0(getValues, folly::coro::Task<std::vector<std::string>>());

  MOCK_METHOD0(getString, folly::coro::Task<std::string>());
  MOCK_METHOD0(getVoid, folly::coro::Task<void>());
};

} // namespace

TEST(CoroGTestHelpers, CoInvokeAvoidsDanglingReferences) {
  MockFoo mock;

  const std::vector<std::string> values{"1", "2", "3"};
  EXPECT_CALL(mock, getValues())
      .WillRepeatedly(
          CoInvoke([&values]() -> folly::coro::Task<std::vector<std::string>> {
            co_return values;
          }));

  auto ret = folly::coro::blockingWait(mock.getValues());
  EXPECT_EQ(ret, values);

  auto ret2 = folly::coro::blockingWait(mock.getValues());
  EXPECT_EQ(ret2, values);
}

TEST(CoroGTestHelpers, CoReturnTest) {
  MockFoo mock;

  EXPECT_CALL(mock, getString()).WillRepeatedly(CoReturn(std::string("abc")));

  auto ret = folly::coro::blockingWait(mock.getString());
  EXPECT_EQ(ret, "abc");

  ret = folly::coro::blockingWait(mock.getString());
  EXPECT_EQ(ret, "abc");
}

TEST(CoroGTestHelpers, CoReturnWthImplicitConversionTest) {
  MockFoo mock;

  EXPECT_CALL(mock, getString()).WillRepeatedly(CoReturn("abc"));

  auto ret = folly::coro::blockingWait(mock.getString());
  EXPECT_EQ(ret, "abc");

  ret = folly::coro::blockingWait(mock.getString());
  EXPECT_EQ(ret, "abc");
}

TEST(CoroGTestHelpers, CoReturnByMoveTest) {
  MockFoo mock;

  EXPECT_CALL(mock, getString())
      .WillRepeatedly(CoReturnByMove(std::string("abc")));

  auto ret = folly::coro::blockingWait(mock.getString());
  EXPECT_EQ(ret, "abc");
}

TEST(CoroGTestHelpers, CoReturnByMoveWithImplicitConversionTest) {
  MockFoo mock;

  struct ImplicitToStringMoveOnly {
    ImplicitToStringMoveOnly(const ImplicitToStringMoveOnly&) = delete;
    ImplicitToStringMoveOnly& operator=(const ImplicitToStringMoveOnly&) =
        delete;

    ImplicitToStringMoveOnly(ImplicitToStringMoveOnly&&) = default;
    ImplicitToStringMoveOnly& operator=(ImplicitToStringMoveOnly&&) = default;

    operator std::string() { return "abc"; }
  };

  EXPECT_CALL(mock, getString())
      .WillRepeatedly(CoReturnByMove(ImplicitToStringMoveOnly{}));

  auto ret = folly::coro::blockingWait(mock.getString());
  EXPECT_EQ(ret, "abc");
}

TEST(CoroGTestHelpers, CoVoidReturnTypeTest) {
  MockFoo mock;

  EXPECT_CALL(mock, getVoid()).WillRepeatedly(CoReturn());

  EXPECT_NO_THROW(folly::coro::blockingWait(mock.getVoid()));
}

TEST(CoroLambdaGtest, CoThrowTest) {
  MockFoo mock;

  std::runtime_error ex("error");
  EXPECT_CALL(mock, getVoid())
      .WillOnce(CoThrow<void>(ex))
      .WillOnce(CoThrow<void>(std::out_of_range("range error")));

  EXPECT_THROW(folly::coro::blockingWait(mock.getVoid()), std::runtime_error);
  EXPECT_THROW(folly::coro::blockingWait(mock.getVoid()), std::out_of_range);

  EXPECT_CALL(mock, getString()).WillOnce(CoThrow<std::string>(ex));
  EXPECT_THROW(folly::coro::blockingWait(mock.getString()), std::runtime_error);
}

#endif // FOLLY_HAS_COROUTINES
