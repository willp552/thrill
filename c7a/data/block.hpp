/*******************************************************************************
 * c7a/data/block.hpp
 *
 * Part of Project c7a.
 *
 * Copyright (C) 2015 Timo Bingmann <tb@panthema.net>
 *
 * This file has no license. Only Chuck Norris can compile it.
 ******************************************************************************/

#pragma once
#ifndef C7A_DATA_BLOCK_HEADER
#define C7A_DATA_BLOCK_HEADER

#include <memory>
#include <cassert>
#include <string>

namespace c7a {
namespace data {

//! \addtogroup data Data Subsystem
//! \{

//! default block size of files.
static const size_t default_block_size = 2 * 1024 * 1024;

template <size_t BlockSize>
class Block
{
public:
    //! type of underlying memory area
    using Byte = unsigned char;

    //! constant size of memory block
    static const size_t block_size = BlockSize;

protected:
    //! constant size of memory block
    static const size_t size_ = BlockSize;

    //! the memory block itself
    Byte data_[size_];

public:
    //! mutable data accessor to memory block
    Byte * data() { return data_; }
    //! const data accessor to memory block
    const Byte * data() const { return data_; }

    //! mutable data accessor to beginning of memory block
    Byte * begin() { return data_; }
    //! const data accessor to beginning of memory block
    const Byte * begin() const { return data_; }

    //! mutable data accessor beyond end of memory block
    Byte * end() { return data_ + size_; }
    //! const data accessor beyond end of memory block
    const Byte * end() const { return data_ + size_; }

    //! the block size
    size_t size() const { return size_; }
};

/**
 * VirtualBlocks combine a reference to a \ref Block and book-keeping
 * information.
 *
 * Multiple virtual blocks can point to the same block but have different
 * book-keeping information!
 */
template <size_t BlockSize = default_block_size>
struct VirtualBlock
{
    using Block = data::Block<BlockSize>;
    using BlockCPtr = std::shared_ptr<const Block>;

    VirtualBlock() { }

    VirtualBlock(const BlockCPtr& block,
                 size_t bytes_used, size_t nitems, size_t first)
        : block(block),
          bytes_used(bytes_used),
          nitems(nitems),
          first(first) { }

    //! referenced block
    BlockCPtr   block;

    //! number of valid bytes in the block (can be used to virtually shorten
    //! a block)
    size_t      bytes_used = 0;

    //! number of valid items in this block (includes cut-off element at the end)
    size_t      nitems = 0;

    //! offset of first element in the block
    size_t      first = 0;

    //! Releases the reference to the block and resets book-keeping info
    void        Release() {
        block = BlockCPtr();
        bytes_used = 0;
        nitems = 0;
        first = 0;
    }

    bool        IsEndBlock() const {
        return block == nullptr;
    }

    //Return virtual block as std::string (for debugging)
    //inclutes eventually cut off elements form the beginning included
    std::string AsString() const {
        return std::string(reinterpret_cast<const char*>(block->data()), bytes_used + first);
    }
};

//! \}

} // namespace data
} // namespace c7a

#endif // !C7A_DATA_BLOCK_HEADER

/******************************************************************************/
