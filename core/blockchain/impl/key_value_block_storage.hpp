/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef KAGOME_KEY_VALUE_BLOCK_STORAGE_HPP
#define KAGOME_KEY_VALUE_BLOCK_STORAGE_HPP

#include "blockchain/block_storage.hpp"
#include "blockchain/impl/common.hpp"
#include "common/logger.hpp"
#include "crypto/hasher.hpp"

namespace kagome::blockchain {

  class KeyValueBlockStorage : public BlockStorage {
   public:
    enum class Error { BLOCK_EXISTS = 1 };

    ~KeyValueBlockStorage() override = default;

    /**
     * Initialise block storage with a genesis block
     * @param genesis the genesis block
     * @param storage underlying storage (must be empty)
     * @param hasher a hasher instance
     */
    static outcome::result<std::shared_ptr<KeyValueBlockStorage>>
    createWithGenesis(const primitives::Block &genesis,
                      PersistentBufferMap &storage,
                      std::shared_ptr<crypto::Hasher> hasher);

    outcome::result<primitives::BlockHeader> getBlockHeader(
        const primitives::BlockId &id) const override;
    outcome::result<primitives::BlockBody> getBlockBody(
        const primitives::BlockId &id) const override;
    outcome::result<primitives::Justification> getJustification(
        const primitives::BlockId &block) const override;

    outcome::result<primitives::BlockHash> putBlock(
        const primitives::Block &block) override;

    outcome::result<void> putJustification(
        const primitives::Justification &j,
        const primitives::BlockHash &hash,
        const primitives::BlockNumber &number) override;

    outcome::result<void> removeBlock(
        const primitives::BlockHash &hash,
        const primitives::BlockNumber &number) override;

   private:
    KeyValueBlockStorage(PersistentBufferMap &storage,
                         std::shared_ptr<crypto::Hasher> hasher);

    PersistentBufferMap &storage_;
    std::shared_ptr<crypto::Hasher> hasher_;
    common::Logger logger_;
  };
}  // namespace kagome::blockchain

OUTCOME_HPP_DECLARE_ERROR(kagome::blockchain, KeyValueBlockStorage::Error);

#endif  // KAGOME_KEY_VALUE_BLOCK_STORAGE_HPP