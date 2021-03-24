/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include <fizz/crypto/aead/OpenSSLEVPCipher.h>
#include <functional>

namespace fizz {
namespace detail {

void encFunc(EVP_CIPHER_CTX*, const folly::IOBuf&, folly::IOBuf&);
void encFuncBlocks(EVP_CIPHER_CTX*, const folly::IOBuf&, folly::IOBuf&);

bool decFunc(
    EVP_CIPHER_CTX*,
    const folly::IOBuf&,
    folly::IOBuf&,
    folly::MutableByteRange);
bool decFuncBlocks(
    EVP_CIPHER_CTX*,
    const folly::IOBuf&,
    folly::IOBuf&,
    folly::MutableByteRange);

void encFuncBlocks(
    EVP_CIPHER_CTX* encryptCtx,
    const folly::IOBuf& plaintext,
    folly::IOBuf& output) {
  size_t totalWritten = 0;
  size_t totalInput = 0;
  int outLen = 0;
  auto outputCursor = transformBufferBlocks<16>(
      plaintext,
      output,
      [&](uint8_t* cipher, const uint8_t* plain, size_t len) {
        if (len > std::numeric_limits<int>::max()) {
          throw std::runtime_error("Encryption error: too much plain text");
        }
        if (len == 0) {
          return static_cast<size_t>(0);
        }
        if (EVP_EncryptUpdate(
                encryptCtx, cipher, &outLen, plain, static_cast<int>(len)) !=
                1 ||
            outLen < 0) {
          throw std::runtime_error("Encryption error");
        }
        totalWritten += outLen;
        totalInput += len;
        return static_cast<size_t>(outLen);
      });

  // We might end up needing to write more in the final encrypt stage
  auto numBuffered = totalInput - totalWritten;
  auto numLeftInOutput = outputCursor.length();
  if (numBuffered <= numLeftInOutput) {
    if (EVP_EncryptFinal_ex(encryptCtx, outputCursor.writableData(), &outLen) !=
        1) {
      throw std::runtime_error("Encryption error");
    }
  } else {
    // we need to copy nicely - this should be at most one block
    std::array<uint8_t, 16> block = {};
    if (EVP_EncryptFinal_ex(encryptCtx, block.data(), &outLen) != 1) {
      throw std::runtime_error("Encryption error");
    }
    outputCursor.push(block.data(), outLen);
  }
}

void encFunc(
    EVP_CIPHER_CTX* encryptCtx,
    const folly::IOBuf& plaintext,
    folly::IOBuf& output) {
  int numWritten = 0;
  int outLen = 0;
  transformBuffer(
      plaintext,
      output,
      [&](uint8_t* cipher, const uint8_t* plain, size_t len) {
        if (len > std::numeric_limits<int>::max()) {
          throw std::runtime_error("Encryption error: too much plain text");
        }
        if (len == 0) {
          return;
        }
        if (EVP_EncryptUpdate(
                encryptCtx, cipher, &outLen, plain, static_cast<int>(len)) !=
            1) {
          throw std::runtime_error("Encryption error");
        }
        numWritten += outLen;
      });
  // We don't expect any writes at the end
  if (EVP_EncryptFinal_ex(
          encryptCtx, output.writableData() + numWritten, &outLen) != 1) {
    throw std::runtime_error("Encryption error");
  }
}

bool decFuncBlocks(
    EVP_CIPHER_CTX* decryptCtx,
    const folly::IOBuf& ciphertext,
    folly::IOBuf& output,
    folly::MutableByteRange tagOut) {
  if (EVP_CIPHER_CTX_ctrl(
          decryptCtx,
          EVP_CTRL_GCM_SET_TAG,
          tagOut.size(),
          static_cast<void*>(tagOut.begin())) != 1) {
    throw std::runtime_error("Decryption error");
  }

  size_t totalWritten = 0;
  size_t totalInput = 0;
  int outLen = 0;
  auto outputCursor = transformBufferBlocks<16>(
      ciphertext,
      output,
      [&](uint8_t* plain, const uint8_t* cipher, size_t len) {
        if (len > std::numeric_limits<int>::max()) {
          throw std::runtime_error("Decryption error: too much cipher text");
        }
        if (EVP_DecryptUpdate(
                decryptCtx, plain, &outLen, cipher, static_cast<int>(len)) !=
            1) {
          throw std::runtime_error("Decryption error");
        }
        totalWritten += outLen;
        totalInput += len;
        return static_cast<size_t>(outLen);
      });

  // We might end up needing to write more in the final encrypt stage
  auto numBuffered = totalInput - totalWritten;
  auto numLeftInOutput = outputCursor.length();
  if (numBuffered <= numLeftInOutput) {
    auto res =
        EVP_DecryptFinal_ex(decryptCtx, outputCursor.writableData(), &outLen);
    return res == 1;
  } else {
    // we need to copy nicely - this should be at most one block
    std::array<uint8_t, 16> block = {};
    auto res = EVP_DecryptFinal_ex(decryptCtx, block.data(), &outLen);
    if (res != 1) {
      return false;
    }
    outputCursor.push(block.data(), outLen);
    return true;
  }
}

bool decFunc(
    EVP_CIPHER_CTX* decryptCtx,
    const folly::IOBuf& ciphertext,
    folly::IOBuf& output,
    folly::MutableByteRange tagOut) {
  int numWritten = 0;
  int outLen = 0;
  transformBuffer(
      ciphertext,
      output,
      [&](uint8_t* plain, const uint8_t* cipher, size_t len) {
        if (len > std::numeric_limits<int>::max()) {
          throw std::runtime_error("Decryption error: too much cipher text");
        }
        if (EVP_DecryptUpdate(
                decryptCtx, plain, &outLen, cipher, static_cast<int>(len)) !=
            1) {
          throw std::runtime_error("Decryption error");
        }
        numWritten += outLen;
      });

  auto tagLen = tagOut.size();
  if (EVP_CIPHER_CTX_ctrl(
          decryptCtx,
          EVP_CTRL_GCM_SET_TAG,
          tagLen,
          static_cast<void*>(tagOut.begin())) != 1) {
    throw std::runtime_error("Decryption error");
  }
  return EVP_DecryptFinal_ex(
             decryptCtx, output.writableData() + numWritten, &outLen) == 1;
}

std::unique_ptr<folly::IOBuf> evpEncrypt(
    std::unique_ptr<folly::IOBuf>&& plaintext,
    const folly::IOBuf* associatedData,
    folly::ByteRange iv,
    size_t tagLen,
    bool useBlockOps,
    size_t headroom,
    EVP_CIPHER_CTX* encryptCtx,
    bool forceInplace) {
  auto inputLength = plaintext->computeChainDataLength();
  // Setup input and output buffers.
  std::unique_ptr<folly::IOBuf> output;
  folly::IOBuf* input;

  if (!forceInplace && plaintext->isShared()) {
    // create enough to also fit the tag and headroom
    output = folly::IOBuf::create(headroom + inputLength + tagLen);
    output->advance(headroom);
    output->append(inputLength);
    input = plaintext.get();
  } else {
    output = std::move(plaintext);
    input = output.get();
  }

  if (EVP_EncryptInit_ex(encryptCtx, nullptr, nullptr, nullptr, iv.data()) !=
      1) {
    throw std::runtime_error("Encryption error");
  }

  if (associatedData) {
    for (auto current : *associatedData) {
      if (current.size() > std::numeric_limits<int>::max()) {
        throw std::runtime_error("too much associated data");
      }
      int len;
      if (EVP_EncryptUpdate(
              encryptCtx,
              nullptr,
              &len,
              current.data(),
              static_cast<int>(current.size())) != 1) {
        throw std::runtime_error("Encryption error");
      }
    }
  }

  if (useBlockOps) {
    encFuncBlocks(encryptCtx, *input, *output);
  } else {
    encFunc(encryptCtx, *input, *output);
  }

  // output is always something we can modify
  auto tailRoom = output->prev()->tailroom();
  if (tailRoom < tagLen) {
    if (forceInplace) {
      throw std::runtime_error("Cannot encrypt inplace.");
    }
    std::unique_ptr<folly::IOBuf> tag = folly::IOBuf::create(tagLen);
    tag->append(tagLen);
    if (EVP_CIPHER_CTX_ctrl(
            encryptCtx, EVP_CTRL_GCM_GET_TAG, tagLen, tag->writableData()) !=
        1) {
      throw std::runtime_error("Encryption error");
    }
    output->prependChain(std::move(tag));
  } else {
    auto lastBuf = output->prev();
    lastBuf->append(tagLen);
    // we can copy into output directly
    if (EVP_CIPHER_CTX_ctrl(
            encryptCtx,
            EVP_CTRL_GCM_GET_TAG,
            tagLen,
            lastBuf->writableTail() - tagLen) != 1) {
      throw std::runtime_error("Encryption error");
    }
  }
  return output;
}

folly::Optional<std::unique_ptr<folly::IOBuf>> evpDecrypt(
    std::unique_ptr<folly::IOBuf>&& ciphertext,
    const folly::IOBuf* associatedData,
    folly::ByteRange iv,
    folly::MutableByteRange tagOut,
    bool useBlockOps,
    EVP_CIPHER_CTX* decryptCtx) {
  auto tagLen = tagOut.size();
  auto inputLength = ciphertext->computeChainDataLength();
  if (inputLength < tagLen) {
    return folly::none;
  }
  inputLength -= tagLen;

  folly::IOBuf* input;
  std::unique_ptr<folly::IOBuf> output;
  trimBytes(*ciphertext, tagOut);
  if (ciphertext->isShared()) {
    // If in is shared, then we have to make a copy of it.
    output = folly::IOBuf::create(inputLength);
    output->append(inputLength);
    input = ciphertext.get();
  } else {
    // If in is not shared we can do decryption in-place.
    output = std::move(ciphertext);
    input = output.get();
  }

  if (EVP_DecryptInit_ex(decryptCtx, nullptr, nullptr, nullptr, iv.data()) !=
      1) {
    throw std::runtime_error("Decryption error");
  }

  if (associatedData) {
    for (auto current : *associatedData) {
      if (current.size() > std::numeric_limits<int>::max()) {
        throw std::runtime_error("too much associated data");
      }
      int len;
      if (EVP_DecryptUpdate(
              decryptCtx,
              nullptr,
              &len,
              current.data(),
              static_cast<int>(current.size())) != 1) {
        throw std::runtime_error("Decryption error");
      }
    }
  }

  auto decrypted = useBlockOps
      ? decFuncBlocks(decryptCtx, *input, *output, tagOut)
      : decFunc(decryptCtx, *input, *output, tagOut);
  if (!decrypted) {
    return folly::none;
  }
  return output;
}
} // namespace detail

OpenSSLEVPCipher::OpenSSLEVPCipher(
    size_t keyLength,
    size_t ivLength,
    size_t tagLength,
    const EVP_CIPHER* cipher,
    bool operatesInBlocks,
    bool requiresPresetTagLen)
    : keyLength_(keyLength),
      ivLength_(ivLength),
      tagLength_(tagLength),
      cipher_(cipher),
      operatesInBlocks_(operatesInBlocks),
      requiresPresetTagLen_(requiresPresetTagLen) {
  encryptCtx_.reset(EVP_CIPHER_CTX_new());
  if (encryptCtx_ == nullptr) {
    throw std::runtime_error("Unable to allocate an EVP_CIPHER_CTX object");
  }
  decryptCtx_.reset(EVP_CIPHER_CTX_new());
  if (decryptCtx_ == nullptr) {
    throw std::runtime_error("Unable to allocate an EVP_CIPHER_CTX object");
  }
  if (EVP_EncryptInit_ex(
          encryptCtx_.get(), cipher_, nullptr, nullptr, nullptr) != 1) {
    throw std::runtime_error("Init error");
  }
  if (EVP_CIPHER_CTX_ctrl(
          encryptCtx_.get(), EVP_CTRL_GCM_SET_IVLEN, ivLength_, nullptr) != 1) {
    throw std::runtime_error("Error setting iv length");
  }
  if (EVP_DecryptInit_ex(
          decryptCtx_.get(), cipher_, nullptr, nullptr, nullptr) != 1) {
    throw std::runtime_error("Init error");
  }
  if (EVP_CIPHER_CTX_ctrl(
          decryptCtx_.get(), EVP_CTRL_GCM_SET_IVLEN, ivLength_, nullptr) != 1) {
    throw std::runtime_error("Error setting iv length");
  }

  if (requiresPresetTagLen_) {
    if (EVP_CIPHER_CTX_ctrl(
            encryptCtx_.get(), EVP_CTRL_GCM_SET_TAG, tagLength_, nullptr) !=
        1) {
      throw std::runtime_error("Error setting enc tag length");
    }

    if (EVP_CIPHER_CTX_ctrl(
            decryptCtx_.get(), EVP_CTRL_GCM_SET_TAG, tagLength_, nullptr) !=
        1) {
      throw std::runtime_error("Error setting dec tag length");
    }
  }
}

void OpenSSLEVPCipher::setKey(TrafficKey trafficKey) {
  trafficKey.key->coalesce();
  trafficKey.iv->coalesce();
  if (trafficKey.key->length() != keyLength_) {
    throw std::runtime_error("Invalid key");
  }
  if (trafficKey.iv->length() != ivLength_) {
    throw std::runtime_error("Invalid IV");
  }
  trafficKey_ = std::move(trafficKey);
  // Cache the IV key. calling coalesce() is not free.
  trafficIvKey_ = trafficKey_.iv->coalesce();
  if (EVP_EncryptInit_ex(
          encryptCtx_.get(),
          nullptr,
          nullptr,
          trafficKey_.key->data(),
          nullptr) != 1) {
    throw std::runtime_error("Error setting encrypt key");
  }
  if (EVP_DecryptInit_ex(
          decryptCtx_.get(),
          nullptr,
          nullptr,
          trafficKey_.key->data(),
          nullptr) != 1) {
    throw std::runtime_error("Error setting decrypt key");
  }
}

std::unique_ptr<folly::IOBuf> OpenSSLEVPCipher::encrypt(
    std::unique_ptr<folly::IOBuf>&& plaintext,
    const folly::IOBuf* associatedData,
    uint64_t seqNum) const {
  auto iv = createIV(seqNum);
  return detail::evpEncrypt(
      std::move(plaintext),
      associatedData,
      folly::ByteRange(iv.data(), ivLength_),
      tagLength_,
      operatesInBlocks_,
      headroom_,
      encryptCtx_.get(),
      false /*forceInplace*/);
}

std::unique_ptr<folly::IOBuf> OpenSSLEVPCipher::inplaceEncrypt(
    std::unique_ptr<folly::IOBuf>&& plaintext,
    const folly::IOBuf* associatedData,
    uint64_t seqNum) const {
  auto iv = createIV(seqNum);
  return detail::evpEncrypt(
      std::move(plaintext),
      associatedData,
      folly::ByteRange(iv.data(), ivLength_),
      tagLength_,
      operatesInBlocks_,
      headroom_,
      encryptCtx_.get(),
      true /*forceInplace*/);
}

folly::Optional<std::unique_ptr<folly::IOBuf>> OpenSSLEVPCipher::tryDecrypt(
    std::unique_ptr<folly::IOBuf>&& ciphertext,
    const folly::IOBuf* associatedData,
    uint64_t seqNum) const {
  auto iv = createIV(seqNum);
  std::array<uint8_t, kMaxTagLength> tag;
  // buffer to copy the tag into when we decrypt
  folly::MutableByteRange tagOut{tag.data(), tagLength_};
  return detail::evpDecrypt(
      std::move(ciphertext),
      associatedData,
      folly::ByteRange(iv.data(), ivLength_),
      tagOut,
      operatesInBlocks_,
      decryptCtx_.get());
}

size_t OpenSSLEVPCipher::getCipherOverhead() const {
  return tagLength_;
}

std::array<uint8_t, OpenSSLEVPCipher::kMaxIVLength> OpenSSLEVPCipher::createIV(
    uint64_t seqNum) const {
  std::array<uint8_t, kMaxIVLength> iv;
  uint64_t bigEndianSeqNum = folly::Endian::big(seqNum);
  const size_t prefixLength = ivLength_ - sizeof(uint64_t);
  memset(iv.data(), 0, prefixLength);
  memcpy(iv.data() + prefixLength, &bigEndianSeqNum, 8);
  folly::MutableByteRange mutableIv{iv.data(), ivLength_};
  XOR(trafficIvKey_, mutableIv);
  return iv;
}
} // namespace fizz