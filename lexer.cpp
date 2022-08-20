#include <utility>

#include "lexer.hpp"

namespace
{
    pi::cursor_t scan_number(pi::cursor_t cursor, pi::cursor_t end)
    {
        for (; cursor != end && std::isdigit(*cursor); ++cursor);
        return cursor;
    }

    pi::cursor_t scan_variable(pi::cursor_t cursor, pi::cursor_t end)
    {
        for (; cursor != end && (std::isalnum(*cursor) || *cursor == '_'); ++cursor);
        return cursor;
    }
}

pi::token_t::token_t(token_t::type_t const type, cursor_t begin, cursor_t end, std::optional<cursor_t> error_location)
: type_{ type }
, begin_{ begin }
, end_{ end }
, error_location_{ error_location }
{
}

pi::lexer_t::lexer_t(std::string code)
    : code_{ std::move(code) }
    , cursor_{ code_.cbegin() }
{
}

pi::tokens_t &pi::lexer_t::operator ()()
{
    if (!tokens_.empty())
        return tokens_;

    auto token_type = token_t::type_t::error;
    std::optional<pi::cursor_t> error_location = std::nullopt;
    while (cursor_  != code_.cend())
    {
        if (std::isspace(*cursor_))
        {
            ++cursor_;
            continue;
        }

        token_begin_ = cursor_;
        switch (auto c = *cursor_; c)
        {
        case '+': ++cursor_, token_type = token_t::type_t::plus; break;
        case '-': ++cursor_, token_type = token_t::type_t::minus; break;
        case '*': ++cursor_, token_type = token_t::type_t::multiply; break;
        case ':': [[fallthrough]];
        case '/': ++cursor_, token_type = token_t::type_t::divide; break;
        case '=': ++cursor_, token_type = token_t::type_t::equals; break;
        case '(': ++cursor_, token_type = token_t::type_t::opened_parenthesis; break;
        case ')': ++cursor_, token_type = token_t::type_t::closed_parenthesis; break;
        case '[': ++cursor_, token_type = token_t::type_t::opened_bracket; break;
        case ']': ++cursor_, token_type = token_t::type_t::closed_bracket; break;
        case '{': ++cursor_, token_type = token_t::type_t::opened_brace; break;
        case '}': ++cursor_, token_type = token_t::type_t::closed_brace; break;
        default:
            if (std::isdigit(c))
            {
                cursor_ = scan_number(token_begin_, code_.cend());
                token_type = token_t::type_t::number;
            }
            else if (std::isalpha(c))
            {
                cursor_ = scan_variable(token_begin_, code_.cend());
                token_type = token_t::type_t::variable;
            }
            else
                ++cursor_, token_type = token_t::type_t::error, error_location = token_begin_;
            break;
        }
        tokens_.emplace_back(token_type, token_begin_, cursor_, error_location);
    }

    return tokens_;
}
