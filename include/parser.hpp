#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "prelude.hpp"
#include "string.hpp"

namespace efp
{
    namespace parser
    {
        template <typename In, typename T>
        bool start_with(const Seq<In> &in, const Seq<T> &t)
        {
            const auto in_length = length(in);
            const auto t_length = length(t);

            if (in_length >= t_length)
            {
                for (size_t i = 0; i < t_length; ++i)
                {
                    if (in[i] != t[i])
                        return false;
                }

                return true;
            }

            return false;
        }

        struct Ch
        {
            char t;

            template <typename In>
            Maybe<Pair<StringView, StringView>> operator()(const Seq<In> &in)
            {
                if (length(in) > 0)
                {
                    if (in[0] == t)
                        return tuple(drop(1, in), StringView{&t, 1});
                    else
                        return nothing;
                }
                return nothing;
            }
        };

        Ch ch(char t)
        {
            return Ch{t};
        }

        struct Tag
        {
            StringView t;

            template <typename In>
            Maybe<Pair<StringView, StringView>> operator()(const Seq<In> &in)
            {
                if (start_with(in, t))
                    return tuple(drop(length(t), in), t);
                else
                    return nothing;
            }
        };

        Tag tag(const char *str)
        {
            return Tag{StringView{str, strlen(str)}};
        }

        template <typename... Ps>
        struct Alt
        {
            Tuple<Ps...> ps;

            template <size_t n, typename In, typename = EnableIf<(n < sizeof...(Ps))>>
            auto impl(const Seq<In> &in) -> Maybe<Pair<StringView, StringView>>
            {
                const auto res = get<n>(ps)(in);

                if (res)
                    return res;
                else
                    return impl<n + 1>(in);
            }

            // Base case: when n equals the size of the tuple, stop recursion.
            template <size_t n, typename In, typename = EnableIf<(n >= sizeof...(Ps))>, typename = void>
            auto impl(const Seq<In> &in) -> Maybe<Pair<StringView, StringView>>
            {
                return nothing; // Or some representation of failure
            }

            template <typename In>
            auto operator()(const Seq<In> &in) -> Maybe<Pair<StringView, StringView>>
            {
                return impl<0>(in);
            }
        };

        template <typename... Ps>
        auto alt(const Ps &...ps)
            -> Alt<Ps...>
        {
            return Alt<Ps...>{tuple(ps...)};
        }

    };

};

#endif