/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 */

#include "gflags/gflags.h"

#include <glog/logging.h>

#include <fizz/crypto/Utils.h>
//#include <fizz/crypto/Sha256.h>
#include <folly/init/Init.h>
//#include <folly/logging/Init.h>
//#include <folly/logging/example/lib.h>
#include <folly/portability/GFlags.h>

#include <quic/samples/echo/EchoClient.h>
#include <quic/samples/echo/EchoServer.h>

DEFINE_string(host, "::1", "Echo server hostname/IP");
DEFINE_int32(port, 6666, "Echo server port");
DEFINE_string(mode, "server", "Mode to run in: 'client' or 'server'");

using namespace quic::samples;


//static example::ExampleObject staticInitialized("static");
//FOLLY_INIT_LOGGING_CONFIG(
//    ".=WARNING,folly=INFO; default:async=true,sync_level=WARNING");

//FOLLY_INIT_LOGGING_CONFIG(
//    ".=WARNING; default:async=false,sync_level=WARNING");

//FOLLY_INIT_LOGGING_CONFIG(".=INFO");


 //FOLLY_INIT_LOGGING_CONFIG(
 //   ".=INFO; default:async=false");

int main(int argc, char* argv[]) {
#if FOLLY_HAVE_LIBGFLAGS
  // Enable glog logging to stderr by default.
  gflags::SetCommandLineOptionWithMode(
      "logtostderr", "1", gflags::SET_FLAGS_DEFAULT);
#endif
  gflags::ParseCommandLineFlags(&argc, &argv, false);
  folly::Init init(&argc, &argv);
  fizz::CryptoUtils::init();
  folly::ByteRange test_blankHash = fizz::Sha256::BlankHash;  

  if (FLAGS_mode == "server") {
    EchoServer server(FLAGS_host, FLAGS_port);
    server.start();
  } else if (FLAGS_mode == "client") {
    if (FLAGS_host.empty() || FLAGS_port == 0) {
      LOG(ERROR) << "EchoClient expected --host and --port";
      return -2;
    }
    EchoClient client(FLAGS_host, FLAGS_port);
    client.start();
  } else {
    LOG(ERROR) << "Unknown mode specified: " << FLAGS_mode;
    return -1;
  }
  return 0;
}
