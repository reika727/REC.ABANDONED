#include"node.hpp"
#include"compilation_error.hpp"
using namespace syntax;
std::unique_ptr<const expression>expression::get(lexicon::token_array&ta,bool for_initialization)
{
    return for_initialization?get_order14(ta):get_order15(ta);
}
std::unique_ptr<const expression>expression::get_order15(lexicon::token_array&ta) // , left to right
{
    auto ret=get_order14(ta);
    while(auto tp=ta.consume(lexicon::TK::COMMA))
        ret=std::make_unique<const comma>(std::move(ret),get_order14(ta),tp->line,tp->col);
    return ret;
}
std::unique_ptr<const expression>expression::get_order14(lexicon::token_array&ta) // = += -= *= /= right to left
{
    auto ret=get_order13(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::EQUAL))
            ret=std::make_unique<const assign>(std::move(ret),get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::PLEQ))
            ret=std::make_unique<const plasgn>(std::move(ret),get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::MIEQ))
            ret=std::make_unique<const miasgn>(std::move(ret),get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::ASEQ))
            ret=std::make_unique<const muasgn>(std::move(ret),get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::SLEQ))
            ret=std::make_unique<const diasgn>(std::move(ret),get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::PEEQ))
            ret=std::make_unique<const rmasgn>(std::move(ret),get_order14(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order13(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order12(ta);
}
std::unique_ptr<const expression>expression::get_order12(lexicon::token_array&ta) // || left to right
{
    auto ret=get_order11(ta);
    while(auto tp=ta.consume(lexicon::TK::VBVB))
        ret=std::make_unique<const logor>(std::move(ret),get_order11(ta),tp->line,tp->col);
    return ret;
}
std::unique_ptr<const expression>expression::get_order11(lexicon::token_array&ta) // && left to right
{
    auto ret=get_order10(ta);
    while(auto tp=ta.consume(lexicon::TK::APAP))
        ret=std::make_unique<const logand>(std::move(ret),get_order10(ta),tp->line,tp->col);
    return ret;
}
std::unique_ptr<const expression>expression::get_order10(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order09(ta);
}
std::unique_ptr<const expression>expression::get_order09(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order08(ta);
}
std::unique_ptr<const expression>expression::get_order08(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order07(ta);
}
std::unique_ptr<const expression>expression::get_order07(lexicon::token_array&ta) // == != left to right
{
    auto ret=get_order06(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::EQEQ))
            ret=std::make_unique<const equal>(std::move(ret),get_order06(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::EXEQ))
            ret=std::make_unique<const nequal>(std::move(ret),get_order06(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order06(lexicon::token_array&ta) // < > <= >= left to right
{
    auto ret=get_order05(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::LESS))
            ret=std::make_unique<const less>(std::move(ret),get_order05(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::GREATER))
            ret=std::make_unique<const greater>(std::move(ret),get_order05(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::LEEQ))
            ret=std::make_unique<const leeq>(std::move(ret),get_order05(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::GREQ))
            ret=std::make_unique<const greq>(std::move(ret),get_order05(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order05(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order04(ta);
}
std::unique_ptr<const expression>expression::get_order04(lexicon::token_array&ta) // + - left to right
{
    auto ret=get_order03(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::PLUS))
            ret=std::make_unique<const bplus>(std::move(ret),get_order03(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::MINUS))
            ret=std::make_unique<const bminus>(std::move(ret),get_order03(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order03(lexicon::token_array&ta) // * / % left to right
{
    auto ret=get_order02(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::ASTER))
            ret=std::make_unique<const multiply>(std::move(ret),get_order02(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::SLASH))
            ret=std::make_unique<const divide>(std::move(ret),get_order02(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::PERCENT))
            ret=std::make_unique<const remain>(std::move(ret),get_order02(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order02(lexicon::token_array&ta) // + - ++ -- ! right to left
{
    if(auto tp=ta.consume(lexicon::TK::PLUS))
        return std::make_unique<const uplus>(get_order02(ta),tp->line,tp->col);
    else if(tp=ta.consume(lexicon::TK::MINUS))
        return std::make_unique<const uminus>(get_order02(ta),tp->line,tp->col);
    else if(tp=ta.consume(lexicon::TK::PLPL))
        return std::make_unique<const preinc>(get_order02(ta),tp->line,tp->col);
    else if(tp=ta.consume(lexicon::TK::MIMI))
        return std::make_unique<const predec>(get_order02(ta),tp->line,tp->col);
    else if(tp=ta.consume(lexicon::TK::EXCLAM))
        return std::make_unique<const lognot>(get_order02(ta),tp->line,tp->col);
    else
        return get_order01(ta);
}
std::unique_ptr<const expression>expression::get_order01(lexicon::token_array&ta) // () left to right
{
    auto ret=get_primary(ta);
    if(auto tp=ta.consume(lexicon::TK::PLPL)){
        return std::make_unique<const postinc>(std::move(ret),tp->line,tp->col);
    }else if(tp=ta.consume(lexicon::TK::MIMI)){
        return std::make_unique<const postdec>(std::move(ret),tp->line,tp->col);
    }else if(tp=ta.consume(lexicon::TK::OPARENT)){
        auto vars=std::vector<std::unique_ptr<const expression>>();
        if(!ta.consume(lexicon::TK::CPARENT)){
            while(true){
                vars.push_back(get_order14(ta));
                if(ta.consume(lexicon::TK::CPARENT))
                    break;
                else if(!ta.consume(lexicon::TK::COMMA))
                    throw exception::compilation_error("関数呼び出し演算子のコンマに問題があります",tp->line,tp->col);
            }
        }
        return std::make_unique<const fcall>(std::move(ret),vars,tp->line,tp->col);
    }else{
        return ret;
    }
}
std::unique_ptr<const expression>expression::get_primary(lexicon::token_array&ta) // literal, identifier, enclosed expression
{
    if(auto nump=std::dynamic_pointer_cast<const lexicon::numeric>(ta.consume(lexicon::TK::NUMERIC))){
        return std::make_unique<const numeric>(nump->value,nump->line,nump->col);
    }else if(auto idp=std::dynamic_pointer_cast<const lexicon::identifier>(ta.consume(lexicon::TK::IDENT))){
        return std::make_unique<const identifier>(idp->name,idp->line,idp->col);
    }else if(auto tp=ta.consume(lexicon::TK::OPARENT)){
        auto ret=get_order15(ta);
        if(!ta.consume(lexicon::TK::CPARENT))
            throw exception::compilation_error("括弧の対応が正しくありません",tp->line,tp->col);
        return ret;
    }
    throw exception::compilation_error("構文解析ができませんでした",ta.get_line(),ta.get_column());
}
std::unique_ptr<const statement>statement::get(lexicon::token_array&ta)
{
    if(ta.check(lexicon::TK::INT))return std::make_unique<const var_difinition>(ta);
    else if(ta.check(lexicon::TK::IF))return std::make_unique<const _if_else_>(ta);
    else if(ta.check(lexicon::TK::WHILE))return std::make_unique<const _while_>(ta);
    else if(ta.check(lexicon::TK::FOR))return std::make_unique<const _for_>(ta);
    else if(ta.check(lexicon::TK::BREAK))return std::make_unique<const _break_>(ta);
    else if(ta.check(lexicon::TK::CONTINUE))return std::make_unique<const _continue_>(ta);
    else if(ta.check(lexicon::TK::RETURN))return std::make_unique<const _return_>(ta);
    else if(ta.check(lexicon::TK::OBRACE))return std::make_unique<const compound>(ta);
    else if(ta.check(lexicon::TK::SCOLON))return std::make_unique<const null_statement>(ta);
    else return std::make_unique<const expression_statement>(ta);
}