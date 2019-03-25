/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef KAGOME_MUXER_HPP
#define KAGOME_MUXER_HPP

#include "libp2p/connection/connection.hpp"

namespace libp2p::muxer {
  class Muxer {
   public:
    /**
     * Attach this muxer to a connection
     * @param conn, to which the muxer will be attached
     * @param isListener - true, if this node is on the server side of this
     * connection and is to listen to the messages; false if this node is to
     * send the messages
     * @return muxed connection
     */
    virtual std::unique_ptr<connection::Connection> attach(
        const connection::Connection &conn, bool isListener) = 0;
  };
}  // namespace libp2p::muxer

#endif  // KAGOME_MUXER_HPP