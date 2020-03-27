#pragma once
#include"token.hpp"
#include<memory>
#include<string>
#include<vector>
namespace lexicon{
    class token_array final{
        private:
            std::vector<std::shared_ptr<const token>>tokens;
            std::vector<std::shared_ptr<const token>>::const_iterator itr;
        public:
            token_array(const std::string&s);
            bool is_all_read()const noexcept;
            bool check(TK type)const noexcept;
            int get_line()const noexcept;
            int get_column()const noexcept;
            std::shared_ptr<const token>consume(TK type)noexcept;
            token_array&operator=(const token_array&)=delete;
            token_array&operator=(token_array&&)=delete;
    };
}
