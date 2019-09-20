/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "libp2p/security/plaintext/plaintext_connection.hpp"

namespace libp2p::connection {

  PlaintextConnection::PlaintextConnection(
      std::shared_ptr<RawConnection> raw_connection,
      crypto::PublicKey localPubkey, crypto::PublicKey remotePubkey)
      : raw_connection_{std::move(raw_connection)},
        local_(std::move(localPubkey)),
        remote_(std::move(remotePubkey)) {}

  outcome::result<peer::PeerId> PlaintextConnection::localPeer() const {
    return peer::PeerId::fromPublicKey(local_.data);
  }

  outcome::result<peer::PeerId> PlaintextConnection::remotePeer() const {
    return peer::PeerId::fromPublicKey(remote_.data);
  }

  outcome::result<crypto::PublicKey> PlaintextConnection::remotePublicKey()
      const {
    return remote_;
  }

  bool PlaintextConnection::isInitiator() const noexcept {
    return raw_connection_->isInitiator();
  }

  outcome::result<multi::Multiaddress> PlaintextConnection::localMultiaddr() {
    return raw_connection_->localMultiaddr();
  }

  outcome::result<multi::Multiaddress> PlaintextConnection::remoteMultiaddr() {
    return raw_connection_->remoteMultiaddr();
  }

  void PlaintextConnection::read(gsl::span<uint8_t> in, size_t bytes,
                                 Reader::ReadCallbackFunc f) {
    return raw_connection_->read(in, bytes, std::move(f));
  };

  void PlaintextConnection::readSome(gsl::span<uint8_t> in, size_t bytes,
                                     Reader::ReadCallbackFunc f) {
    return raw_connection_->readSome(in, bytes, std::move(f));
  };

  void PlaintextConnection::write(gsl::span<const uint8_t> in, size_t bytes,
                                  Writer::WriteCallbackFunc f) {
    return raw_connection_->write(in, bytes, std::move(f));
  }

  void PlaintextConnection::writeSome(gsl::span<const uint8_t> in, size_t bytes,
                                      Writer::WriteCallbackFunc f) {
    return raw_connection_->writeSome(in, bytes, std::move(f));
  }

  bool PlaintextConnection::isClosed() const {
    return raw_connection_->isClosed();
  }

  outcome::result<void> PlaintextConnection::close() {
    return raw_connection_->close();
  }
}  // namespace libp2p::connection
