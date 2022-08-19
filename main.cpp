#include <algorithm>
#include <csignal>
#include <execution>
#include <iostream>

#include "lexer.hpp"

namespace
{
    using namespace std::string_literals;

    auto signal_handler(int const signal)
    {
        std::cerr << "Force quit: SIGNAL " << signal << std::endl;
        std::cout << std::endl;
        exit(EXIT_SUCCESS);
    }

    auto to_lower(std::string const &s)
    {
        std::string result{ s };
        std::transform(std::execution::par_unseq, s.cbegin(), s.cend(), result.begin(), [](char const c){ return tolower(c); });
        return s;
    }

    auto prompt()
    {
        std::cout << "Welcome to the \u03C0 language prompt\n"
                  << "\u03A0 is a language for solving simple mathematical equations.\n\n"
                  << "Press 'CTRL+C' or type 'exit' to exit the program.'\n\n";
        for (std::string input_line;;)
        {
            std::cout << "> ";
            std::getline(std::cin, input_line);
            if (to_lower(input_line) == "exit"s)
                break;

            pi::lexer_t lexer{ input_line };
            auto tokens = lexer();

            std::clog << "Found " << tokens.size() << " tokens:\n";
            auto errors = 0;
            for (auto token : tokens)
            {
                if (token.type_ == pi::token_t::type_t::error)
                    ++errors;
                else
                    std::clog << std::string(token.begin_, token.end_) << std::endl;
            }
            if (errors > 0)
                std::clog << "Found " << errors << " tokens with errors." << std::endl;
        }
        std::cout << "\nBye!\n\n";
    }
}

int main()
{
    signal(SIGINT, &signal_handler);
    signal(SIGTERM, &signal_handler);

    prompt();

    return EXIT_SUCCESS;
}
