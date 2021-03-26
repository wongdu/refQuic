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

#include <folly/init/Init.h>
#include <folly/logging/Init.h>
#include <folly/logging/example/lib.h>
#include <folly/logging/xlog.h>

using namespace example;

//static constexpr folly::StringPiece csBlankHash{
//	  "\xe3\xb0\xc4\x42\x98\xfc\x1c\x14\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24\x27\xae\x41\xe4\x64\x9b\x93\x4c\xa4\x95\x99\x1b\x78\x52\xb8\x15" };

// Invoking code that uses XLOG() statements before main() is safe.
// This will use default log settings defined by folly::initializeLoggerDB().
static ExampleObject staticInitialized("static");

// Configure folly to enable INFO+ messages, and everything else to
// enable WARNING+.
//
// Set the default log handler to log asynchronously by default
// (except log messages with level WARNING and above synchronously)
FOLLY_INIT_LOGGING_CONFIG(
    ".=WARNING,folly=INFO; default:async=true,sync_level=WARNING");

//static /*constexpr*/ folly::StringPiece sBlankHash{
//	  "\xe3\xb0\xc4\x42\x98\xfc\x1c\x14\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24\x27\xae\x41\xe4\x64\x9b\x93\x4c\xa4\x95\x99\x1b\x78\x52\xb8\x55" };

int main(int argc, char* argv[]) {
  // Using log macros before calling folly::initLogging() will use the default
  // log settings defined by folly::initializeLoggerDB().  The default behavior
  // is to log INFO+ messages to stderr.
  XLOG(DBG) << "log messages less than INFO will be ignored before initLogging";
  XLOG(ERR) << "error messages before initLogging() will be logged to stderr";

  // folly::Init() will automatically initialize the logging settings based on
  // the FOLLY_INIT_LOGGING_CONFIG declaration above and the --logging command
  // line flag.

  ///*static*/ /*constexpr*/ folly::StringPiece BlankHash{
	 // "\xe3\xb0\xc4\x42\x98\xfc\x1c\x14\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24\x27\xae\x41\xe4\x64\x9b\x93\x4c\xa4\x95\x99\x1b\x78\x52\xb8\x55" };
  
  /*XLOG(INFO, "now construct");

  folly::ByteRange blankHash = BlankHash;
  folly::ByteRange sblankHash = sBlankHash;
  folly::ByteRange csblankHash = csBlankHash;
  BlankHash.clear();*/
  folly::Init init(&argc, &argv);

  /*folly::StringPiece BlankHash{
	  "\xe3\xb0\xc4\x42\x98\xfc\x1c\x14\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24\x27\xae\x41\xe4\x64\x9b\x93\x4c\xa4\x95\x99\x1b\x78\x52\xb8\x55" };*/

  // All XLOG() statements in this file will log to the category
  // folly.logging.example.main
  XLOG(INFO, "now the normal log settings have been applied");

  XLOG(DBG1, "log arguments are concatenated: ", 12345, ", ", 92.0);
  XLOGF(DBG1, "XLOGF supports {}-style formatting: {:.3f}", "python", 1.0 / 3);
  XLOG(DBG2) << "streaming syntax is also supported: " << 1234;
  XLOG(DBG2, "if you really want, ", "you can even")
      << " mix function-style and streaming syntax: " << 42;
  XLOGF(DBG3, "and {} can mix {} style", "you", "format") << " and streaming";

  ExampleObject("foo");
  XLOG(INFO) << "main returning";
  //XLOG(WARNING, "now returning");
  return 0;
}
