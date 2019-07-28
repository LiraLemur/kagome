/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "libp2p/host/basic_host/basic_host.hpp"

namespace libp2p::host {

  BasicHost::BasicHost(std::shared_ptr<peer::IdentityManager> idmgr,
                       std::unique_ptr<network::Network> network,
                       std::unique_ptr<peer::PeerRepository> repo)
      : idmgr_(std::move(idmgr)),
        network_(std::move(network)),
        repo_(std::move(repo)) {
    BOOST_ASSERT(idmgr_ != nullptr);
    BOOST_ASSERT(network_ != nullptr);
    BOOST_ASSERT(repo_ != nullptr);
  }

  std::string_view BasicHost::getLibp2pVersion() const {
    return "0.0.0";
  }

  std::string_view BasicHost::getLibp2pClientVersion() const {
    return "kagome";
  }

  peer::PeerId BasicHost::getId() const {
    return idmgr_->getId();
  }

  peer::PeerInfo BasicHost::getPeerInfo() const {
    return {getId(), getObservedAddresses()};
  }

  std::vector<multi::Multiaddress> BasicHost::getAddresses() const {
    return network_->getListener().getListenAddresses();
  }

  std::vector<multi::Multiaddress> BasicHost::getAddressesInterfaces() const {
    return network_->getListener().getListenAddressesInterfaces();
  }

  std::vector<multi::Multiaddress> BasicHost::getObservedAddresses() const {
    auto r = repo_->getAddressRepository().getAddresses(getId());
    if (r) {
      return r.value();
    }

    // we don't know our addresses
    return {};
  }

  void BasicHost::setProtocolHandler(
      const peer::Protocol &proto,
      const std::function<connection::Stream::Handler> &handler) {
    network_->getListener().getRouter().setProtocolHandler(proto, handler);
  }

  void BasicHost::setProtocolHandler(
      const peer::Protocol &proto,
      const std::function<connection::Stream::Handler> &handler,
      const std::function<bool(const peer::Protocol &)> &predicate) {
    network_->getListener().getRouter().setProtocolHandler(proto, handler,
                                                           predicate);
  }

  void BasicHost::newStream(const peer::PeerInfo &p,
                            const peer::Protocol &protocol,
                            const Host::StreamResultHandler &handler) {
    network_->getDialer().newStream(p, protocol, handler);
  }

  outcome::result<void> BasicHost::listen(const multi::Multiaddress &ma) {
    return network_->getListener().listen(ma);
  }

  outcome::result<void> BasicHost::closeListener(
      const multi::Multiaddress &ma) {
    return network_->getListener().closeListener(ma);
  }

  outcome::result<void> BasicHost::removeListener(
      const multi::Multiaddress &ma) {
    return network_->getListener().removeListener(ma);
  }

  void BasicHost::start() {
    network_->getListener().stop();
  }

  void BasicHost::stop() {
    network_->getListener().start();
  }

  network::Network &BasicHost::getNetwork() {
    return *network_;
  }

  peer::PeerRepository &BasicHost::getPeerRepository() {
    return *repo_;
  }

  network::Router &BasicHost::getRouter() {
    return network_->getListener().getRouter();
  }

  event::Bus &BasicHost::getBus() {
    return bus_;
  }

  void BasicHost::connect(const peer::PeerInfo &p) {
    network_->getDialer().dial(p, [](auto && /* ignored */) {});
  }
}  // namespace libp2p::host