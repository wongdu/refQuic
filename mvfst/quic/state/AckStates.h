/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 */

#pragma once

#include <quic/QuicConstants.h>
#include <quic/codec/Types.h>
#include <quic/common/IntervalSet.h>

#include <folly/Random.h>

namespace quic {

// Ack and PacketNumber states. This is per-packet number space.
struct AckState {
  // Largest ack that has been written to a packet
  folly::Optional<PacketNum> largestAckScheduled;
  // Count of outstanding packets received with only non-retransmittable data.
  uint64_t numNonRxPacketsRecvd{0};
  // The receive time of the largest ack packet
  folly::Optional<TimePoint> largestRecvdPacketTime;
  // Latest packet number acked by peer
  folly::Optional<PacketNum> largestAckedByPeer;
  // Largest received packet numbers on the connection.
  folly::Optional<PacketNum> largestReceivedPacketNum;
  // Largest received packet number at the time we sent our last close message.
  folly::Optional<PacketNum> largestReceivedAtLastCloseSent;
  // Next PacketNum we will send for packet in this packet number space
  PacketNum nextPacketNum{0};
  AckBlocks acks;
  bool ignoreReorder{false};
  folly::Optional<uint64_t> tolerance;
  folly::Optional<uint64_t> ackFrequencySequenceNumber;
  // Flag indicating that if we need to send ack immediately. This will be set
  // to true if we got packets with retransmittable data and haven't sent the
  // ack for the first time.
  // TODO: upgrade this to be bundled like non-retransmittable packets once we
  // figured out how to do timeout.
  bool needsToSendAckImmediately{false};
  // Count of oustanding packets received with retransmittable data.
  uint8_t numRxPacketsRecvd{0};
};

struct AckStates {
  AckStates() {
    PacketNum randomizedPacketNum =
        folly::Random::secureRand32(kMaxInitialPacketNum);
    initialAckState.nextPacketNum = randomizedPacketNum;
    handshakeAckState.nextPacketNum = randomizedPacketNum;
    appDataAckState.nextPacketNum = randomizedPacketNum;
  }
  // AckState for acks to peer packets in Initial packet number space.
  AckState initialAckState;
  // AckState for acks to peer packets in Handshake packet number space.
  AckState handshakeAckState;
  // AckState for acks to peer packets in AppData packet number space.
  AckState appDataAckState;
  std::chrono::microseconds maxAckDelay{kMaxAckTimeout};
};

} // namespace quic
