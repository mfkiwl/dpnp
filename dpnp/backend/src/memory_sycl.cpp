//*****************************************************************************
// Copyright (c) 2016-2020, Intel Corporation
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************

#include <cstring>
#include <exception>
#include <iostream>

#include <dpnp_iface.hpp>
#include "queue_sycl.hpp"


static bool use_sycl_device_memory()
{
    // TODO need to move all getenv() into common dpnpc place
    const char* dpnpc_memtype_device = getenv("DPNPC_OUTPUT_DPARRAY_USE_MEMORY_DEVICE");
    if (dpnpc_memtype_device != nullptr)
    {
        return true;
    }

    return false;
}

// This variable is needed for the NumPy corner case
// if we have zero memory array (ex. shape=(0,10)) we must keep the pointer to somewhere
// memory of this variable must not be used
const char* numpy_stub = "...the NumPy Stub...";

char* dpnp_memory_alloc_c(size_t size_in_bytes)
{
    char* array = const_cast<char*>(numpy_stub);

    //std::cout << "dpnp_memory_alloc_c(size=" << size_in_bytes << std::flush;
    if (size_in_bytes > 0)
    {
        cl::sycl::usm::alloc memory_type = cl::sycl::usm::alloc::shared;
        if (use_sycl_device_memory())
        {
            memory_type = cl::sycl::usm::alloc::device;
        }
        array = reinterpret_cast<char*>(malloc(size_in_bytes, DPNP_QUEUE, memory_type));
        if (array == nullptr)
        {
            // TODO add information about number of allocated bytes
            throw std::runtime_error("DPNP Error: dpnp_memory_alloc_c() out of memory.");
        }

#if not defined(NDEBUG)
        for (size_t i = 0; i < size_in_bytes / sizeof(char); ++i)
        {
            array[i] = 0; // type dependant is better. set double(42.42) instead zero
        }
        // std::cout << ") -> ptr=" << (void*)array << std::endl;
#endif
    }

    return array;
}

void dpnp_memory_free_c(void* ptr)
{
    //std::cout << "dpnp_memory_free_c(ptr=" << (void*)ptr << ")" << std::endl;
    if (ptr != numpy_stub)
    {
        free(ptr, DPNP_QUEUE);
    }
}

void dpnp_memory_memcpy_c(void* dst, const void* src, size_t size_in_bytes)
{
    //std::cout << "dpnp_memory_memcpy_c(dst=" << dst << ", src=" << src << ")" << std::endl;

    memcpy(dst, src, size_in_bytes);
}
