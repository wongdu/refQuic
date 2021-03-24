/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 */

#pragma once

#include <quic/api/QuicStreamAsyncTransport.h>

namespace quic {

class QuicServerAsyncTransport : public QuicStreamAsyncTransport,
                                 public QuicSocket::ConnectionCallback {
 public:
  using UniquePtr = std::unique_ptr<
      QuicServerAsyncTransport,
      folly::DelayedDestruction::Destructor>;
  QuicServerAsyncTransport() = default;
  void setServerSocket(std::shared_ptr<quic::QuicSocket> sock);

 protected:
  ~QuicServerAsyncTransport() override = default;

  //
  // QuicSocket::ConnectionCallback
  //
  void onNewBidirectionalStream(StreamId id) noexcept override;
  void onNewUnidirectionalStream(StreamId id) noexcept override;
  void onStopSending(StreamId id, ApplicationErrorCode error) noexcept override;
  void onConnectionEnd() noexcept override;
  void onConnectionError(
      std::pair<QuicErrorCode, std::string> code) noexcept override;
  void onTransportReady() noexcept override;
};
} // namespace quic
