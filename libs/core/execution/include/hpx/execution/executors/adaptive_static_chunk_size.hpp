//  Copyright (c) 2007-2022 Hartmut Kaiser
//  Copyright (c) 2022 Karame M.Shokooh
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/executors/adaptive_static_chunk_size.hpp

#pragma once

#include <hpx/config.hpp>
#include <hpx/execution_base/traits/is_executor_parameters.hpp>
#include <hpx/serialization/serialize.hpp>
#include <hpx/timing/steady_clock.hpp>

#include <hpx/execution/executors/execution_parameters_fwd.hpp>

#include <chrono>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <type_traits>

namespace hpx::execution::experimental {

    ///////////////////////////////////////////////////////////////////////////
    /// Loop iterations are divided into pieces of size \a chunk_size and then
    /// assigned to threads. If \a chunk_size is not specified, the iterations
    /// are evenly (if possible) divided contiguously among the threads.
    ///
    /// \note This executor parameters type is equivalent to OpenMP's STATIC
    ///       scheduling directive.
    ///
    struct adaptive_static_chunk_size
    {
        /// Construct a \a adaptive_static_chunk_size executor parameters object
        ///
        /// \note By default the number of loop iterations is determined from
        ///       the number of available cores and the overall number of loop
        ///       iterations to schedule.
        ///
        constexpr adaptive_static_chunk_size() noexcept
          : chunk_size_(0)
        {
        }

        /// Construct a \a adaptive_static_chunk_size executor parameters object
        ///
        /// \param chunk_size   [in] The optional chunk size to use as the
        ///                     number of loop iterations to run on a single
        ///                     thread.
        ///
        constexpr explicit adaptive_static_chunk_size(
            std::size_t chunk_size) noexcept
          : chunk_size_(chunk_size)
        {
        }

        /// \cond NOINTERNAL
        template <typename Executor>
        std::size_t get_chunk_size(Executor& exec,
            hpx::chrono::steady_duration const&, std::size_t cores,
            std::size_t input_size)
        {
            // Make sure the internal round robin counter of the executor is
            // reset
            parallel::execution::reset_thread_distribution(*this, exec);

            // use the given chunk size if given
            if (chunk_size_ != 0)
            {
                return chunk_size_;
            }

            if (cores == 1)
            {
                return input_size;
            }

            // Return a chunk size that is a power of two; and that leads to at
            // least 2 chunks per core, and at most 4 chunks per core.
            std::size_t chunk_size = 1;
            std::size_t coeff = 1;

            if (input_size >= (1ull << 25))
            {
                coeff = 8;
            }
            else if (input_size >= (1ull << 19))
            {
                coeff = 4;
            }
            while (chunk_size * coeff * cores < input_size)
            {
                chunk_size *= 2;
            }


            // if (input_size >= (1ull << 25))
            // {
            //     coeff = 8;
            // }
            // if (input_size >= (1ull << 21))
            // {
            //     coeff = 4;
            // }
            //  else if (input_size >= (1ull << 19))
            // {
            //     coeff = 1;
            // }
            // while (chunk_size * coeff * cores < input_size)
            // {
            //     chunk_size *= 2;
            // }
            std::cout << "this is chunk size in adaptive_static_chunk: " << chunk_size <<std::endl;
            std::cout << "this is input size in adaptive_static_chunk: " << input_size <<std::endl;
            return chunk_size;
        }
        /// \endcond

    private:
        /// \cond NOINTERNAL
        friend class hpx::serialization::access;

        template <typename Archive>
        void serialize(Archive& ar, const unsigned int /* version */)
        {
            // clang-format off
            ar & chunk_size_;
            // clang-format on
        }
        /// \endcond

    private:
        /// \cond NOINTERNAL
        std::size_t chunk_size_;
        /// \endcond
    };
}    // namespace hpx::execution::experimental

namespace hpx::parallel::execution {

    /// \cond NOINTERNAL
    template <>
    struct is_executor_parameters<
        hpx::execution::experimental::adaptive_static_chunk_size>
      : std::true_type
    {
    };
    /// \endcond
}    // namespace hpx::parallel::execution

namespace hpx::execution {

    using adaptive_static_chunk_size HPX_DEPRECATED_V(1, 9,
        "hpx::execution::adaptive_static_chunk_size is deprecated, use "
        "hpx::execution::experimental::adaptive_static_chunk_size instead") =
        hpx::execution::experimental::adaptive_static_chunk_size;
}
