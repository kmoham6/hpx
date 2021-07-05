//  Copyright (c) 2020 ETH Zurich
//  Copyright (c) 2021 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <hpx/config.hpp>
#include <hpx/concepts/concepts.hpp>
#include <hpx/errors/try_catch_exception_ptr.hpp>
#include <hpx/execution/algorithms/detail/partial_algorithm.hpp>
#include <hpx/execution/algorithms/transform.hpp>
#include <hpx/execution_base/receiver.hpp>
#include <hpx/execution_base/sender.hpp>
#include <hpx/functional/invoke_result.hpp>
#include <hpx/functional/tag_fallback_dispatch.hpp>
#include <hpx/iterator_support/counting_shape.hpp>
#include <hpx/type_support/pack.hpp>

#include <exception>
#include <iterator>
#include <type_traits>
#include <utility>

namespace hpx { namespace execution { namespace experimental {

    ///////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename R, typename Shape, typename F>
        struct bulk_receiver
        {
            std::decay_t<R> r;
            std::decay_t<Shape> shape;
            std::decay_t<F> f;

            template <typename R_, typename Shape_, typename F_>
            bulk_receiver(R_&& r, Shape_&& shape, F_&& f)
              : r(std::forward<R_>(r))
              , shape(std::forward<Shape_>(shape))
              , f(std::forward<F_>(f))
            {
            }

            template <typename E>
            void set_error(E&& e) && noexcept
            {
                hpx::execution::experimental::set_error(
                    std::move(r), std::forward<E>(e));
            }

            void set_done() && noexcept
            {
                hpx::execution::experimental::set_done(std::move(r));
            }

            template <typename... Ts>
            void set_value(Ts&&... ts) noexcept
            {
                hpx::detail::try_catch_exception_ptr(
                    [&]() {
                        for (auto const& s : shape)
                        {
                            HPX_INVOKE(f, s, ts...);
                        }
                        hpx::execution::experimental::set_value(
                            std::move(r), std::forward<Ts>(ts)...);
                    },
                    [&](std::exception_ptr ep) {
                        hpx::execution::experimental::set_error(
                            std::move(r), std::move(ep));
                    });
            }
        };

        template <typename S, typename Shape, typename F>
        struct bulk_sender
        {
            std::decay_t<S> s;
            std::decay_t<Shape> shape;
            std::decay_t<F> f;

            template <template <typename...> class Tuple,
                template <typename...> class Variant>
            using value_types =
                typename hpx::execution::experimental::sender_traits<
                    S>::template value_types<Tuple, Variant>;

            template <template <typename...> class Variant>
            using error_types =
                hpx::util::detail::unique_t<hpx::util::detail::prepend_t<
                    typename hpx::execution::experimental::sender_traits<
                        S>::template error_types<Variant>,
                    std::exception_ptr>>;

            static constexpr bool sends_done = false;

            template <typename R>
            auto connect(R&& r) &&
            {
                return hpx::execution::experimental::connect(std::move(s),
                    bulk_receiver<R, Shape, F>(
                        std::forward<R>(r), std::move(shape), std::move(f)));
            }

            template <typename R>
            auto connect(R&& r) &
            {
                return hpx::execution::experimental::connect(s,
                    bulk_receiver<R, Shape, F>(std::forward<R>(r), shape, f));
            }
        };
    }    // namespace detail

    ///////////////////////////////////////////////////////////////////////////
    HPX_INLINE_CONSTEXPR_VARIABLE struct bulk_t final
      : hpx::functional::tag_fallback<bulk_t>
    {
    private:
        // clang-format off
        template <typename S, typename Shape, typename F,
            HPX_CONCEPT_REQUIRES_(
                is_sender_v<S> &&
                std::is_integral<Shape>::value
            )>
        // clang-format on
        friend constexpr HPX_FORCEINLINE auto tag_fallback_dispatch(
            bulk_t, S&& s, Shape const& shape, F&& f)
        {
            return detail::bulk_sender<S, hpx::util::counting_shape_type<Shape>,
                F>{std::forward<S>(s), hpx::util::make_counting_shape(shape),
                std::forward<F>(f)};
        }

        // clang-format off
        template <typename S, typename Shape, typename F,
            HPX_CONCEPT_REQUIRES_(
                is_sender_v<S> &&
                !std::is_integral<std::decay_t<Shape>>::value
            )>
        // clang-format on
        friend constexpr HPX_FORCEINLINE auto tag_fallback_dispatch(
            bulk_t, S&& s, Shape&& shape, F&& f)
        {
            return detail::bulk_sender<S, Shape, F>{std::forward<S>(s),
                std::forward<Shape>(shape), std::forward<F>(f)};
        }

        template <typename Shape, typename F>
        friend constexpr HPX_FORCEINLINE auto tag_fallback_dispatch(
            bulk_t, Shape&& shape, F&& f)
        {
            return detail::partial_algorithm<bulk_t, Shape, F>{
                std::forward<Shape>(shape), std::forward<F>(f)};
        }
    } bulk{};
}}}    // namespace hpx::execution::experimental
