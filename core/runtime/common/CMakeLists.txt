#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#


add_library(storage_wasm_provider
    storage_wasm_provider.cpp
    )
target_link_libraries(storage_wasm_provider
    buffer
    )

add_library(trie_storage_provider
    trie_storage_provider_impl.cpp
    )
target_link_libraries(trie_storage_provider
    trie_storage
    )
