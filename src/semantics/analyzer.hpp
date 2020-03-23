#pragma once
#include<map>
#include<set>
#include<string>
#include<vector>
namespace semantics{
    /*
     * 検出すべき意味論的誤り
     * 未定義変数/関数の使用
     * 変数/関数の二重定義
     * 不適切な箇所でのbreak/continueの使用
     */
    class analyzer final{
        private:
            std::vector<std::set<std::string>>vars;
            std::map<std::string,int>funcs;
            int break_nest=0,continue_nest=0;
        public:
            void enter_scope();
            void leave_scope();
            void enter_break()noexcept;
            void leave_break()noexcept;
            void enter_continue()noexcept;
            void leave_continue()noexcept;
            void define_func(const std::string&name,int var_num);
            void define_var(const std::string&name);
            bool is_available_func(const std::string&name,int var_num);
            bool is_available_var(const std::string&name)const;
            bool is_definable_func(const std::string&nane)const;
            bool is_definable_var(const std::string&name)const;
            bool is_breakable()const noexcept;
            bool is_continuable()const noexcept;
            analyzer&operator=(const analyzer&)=delete;
            analyzer&operator=(analyzer&&)=delete;
    };
}
