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
        case '+': tokens_.emplace_back(token_t::type_t::plus, token_begin_, ++cursor_, std::nullopt); break;
        case '-': tokens_.emplace_back(token_t::type_t::minus, token_begin_, ++cursor_, std::nullopt); break;
        case '*': tokens_.emplace_back(token_t::type_t::multiply, token_begin_, ++cursor_, std::nullopt); break;
        case ':': [[fallthrough]];
        case '/': tokens_.emplace_back(token_t::type_t::divide, token_begin_, ++cursor_, std::nullopt); break;
        case '=': tokens_.emplace_back(token_t::type_t::equals, token_begin_, ++cursor_, std::nullopt); break;
        case '(': tokens_.emplace_back(token_t::type_t::opened_parenthesis, token_begin_, ++cursor_, std::nullopt); break;
        case ')': tokens_.emplace_back(token_t::type_t::closed_parenthesis, token_begin_, ++cursor_, std::nullopt); break;
        case '[': tokens_.emplace_back(token_t::type_t::opened_bracket, token_begin_, ++cursor_, std::nullopt); break;
        case ']': tokens_.emplace_back(token_t::type_t::closed_bracket, token_begin_, ++cursor_, std::nullopt); break;
        case '{': tokens_.emplace_back(token_t::type_t::opened_brace, token_begin_, ++cursor_, std::nullopt); break;
        case '}': tokens_.emplace_back(token_t::type_t::closed_brace, token_begin_, ++cursor_, std::nullopt); break;
        default:
            if (std::isdigit(c))
            {
                cursor_ = scan_number(token_begin_, code_.cend());
                tokens_.emplace_back(token_t::type_t::number, token_begin_, cursor_, std::nullopt);
            }
            else if (std::isalpha(c))
            {
                cursor_ = scan_variable(token_begin_, code_.cend());
                tokens_.emplace_back(token_t::type_t::variable, token_begin_, cursor_, std::nullopt);
            }
            else
                tokens_.emplace_back(token_t::type_t::error, token_begin_, ++cursor_, token_begin_);
            break;
        }
    }

    return tokens_;
}
