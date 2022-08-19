#ifndef PIL_LEXER_HPP
#define PIL_LEXER_HPP

#include <optional>
#include <string>
#include <vector>

namespace pi
{
    using cursor_t = std::string::const_iterator;

    struct token_t
    {
        enum class type_t
        {
            number
          , variable
          , plus
          , minus
          , multiply
          , divide
          , equals
          , error
        };

        token_t(type_t, cursor_t begin, cursor_t end, std::optional<cursor_t> error_location);

        type_t type_{ type_t::error };
        cursor_t begin_{};
        cursor_t end_{};
        std::optional<cursor_t> error_location_{};
    };
    using tokens_t = std::vector<token_t>;

    class lexer_t
    {
    public:
        explicit lexer_t(std::string code);

        [[nodiscard]] tokens_t &operator ()();

    private:
        std::string code_{};
        cursor_t token_begin_{};
        cursor_t cursor_{};
        tokens_t tokens_{};
    };
}

#endif //PIL_LEXER_HPP
