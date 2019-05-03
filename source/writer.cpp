#include"assembly_source/writer.hpp"
#include<sstream>
using namespace assembly_source;
using namespace parsing;
writer::writer(const std::string&filename):ofs(filename),indent(0),serial(0)
{
    write(".global main");
}
std::string writer::p(const std::string&str)
{
    return (str[0]=='('||str[0]=='.'?"":"%")+str;
}
std::string writer::label(const std::string&base)
{
    return '.'+base+std::to_string(serial++);
}
void writer::write(const std::string&str)
{
    ofs<<std::string(indent,' ')<<str<<std::endl;
}
void writer::write(const std::string&inst,const std::string&reg1,const std::string&reg2)
{
    write(inst+' '+p(reg1)+", "+p(reg2));
}
void writer::write(const std::string&inst,int arg,const std::string&reg)
{
    write(inst+" $"+std::to_string(arg)+", "+p(reg));
}
void writer::write(const std::string&inst,const std::string&reg)
{
    write(inst+' '+p(reg));
}
void writer::write(const std::string&inst,int arg)
{
    write(inst+" $"+std::to_string(arg));
} 
std::string writer::derefer(int dis,const std::string&base,const std::string&ofs,int scl)
{
    std::stringstream ss;
    if(dis!=0)ss<<dis;
    ss<<'(';
    if(base!="")ss<<'%'<<base;
    if(ofs!="")ss<<",%"<<ofs;
    if(scl!=1)ss<<','<<scl;
    ss<<')';
    return ss.str();
}
std::string writer::derefer(int dis,const std::string&base,int scl)
{
    return derefer(dis,base,"",scl);
}
std::string writer::derefer(const std::string&base,const std::string&ofs,int scl)
{
    return derefer(0,base,ofs,scl);
}
std::string writer::derefer(const std::string&base,int scl)
{
    return derefer(0,base,"",scl);
}
void writer::enumerate_var(parsing::node*const node)
{
    if(auto bop=dynamic_cast<parsing::biopr*>(node);bop!=nullptr){
	enumerate_var(bop->larg);
	enumerate_var(bop->rarg);
    }else if(auto uop=dynamic_cast<parsing::unopr*>(node);uop!=nullptr){
	enumerate_var(uop->arg);
    }else if(auto idp=dynamic_cast<parsing::ident*>(node);idp!=nullptr){
	if(!offset.count(idp->name)){
	    write("sub",8,"rsp");
	    offset[idp->name]=var_size+=8;
	}
    }
}
void writer::refer_var(parsing::node*const node)
{
    if(auto idp=dynamic_cast<parsing::ident*>(node);idp!=nullptr){
	write("mov","rbp","rax");
	write("sub",offset[idp->name],"rax");
	write("push","rax");
    }else{
	throw std::runtime_error("右辺値への代入はできません");
    }
}
void writer::RDP(parsing::node*const node)
{
    if(auto nup=dynamic_cast<parsing::numeric*>(node);nup!=nullptr){
	write("push",nup->value);
    }else if(auto idp=dynamic_cast<parsing::ident*>(node);idp!=nullptr){
	refer_var(idp);
	write("pop","rax");
	write("push",derefer("rax"));
    }else if(auto uop=dynamic_cast<parsing::unopr*>(node);uop!=nullptr){
	if(uop->type==ND::UPLUS){
	    RDP(uop->arg);
	}else if(uop->type==ND::UMINUS){
	    RDP(uop->arg);
	    write("pop","rax");
	    write("mov","rax","rdi");
	    write("mov",2,"rsi");write("mul","rsi");
	    write("sub","rax","rdi");
	    write("push","rdi");
	}else if(uop->type==ND::PREINC||uop->type==ND::PREDEC){
	    refer_var(uop->arg);
	    write("pop","rax");
	    switch(uop->type){
		case ND::PREINC:
		    write("add",1,derefer("rax"));
		    break;
		case ND::PREDEC:
		    write("sub",1,derefer("rax"));
		    break;
	    }
	    write("push",derefer("rax"));
	}
    }else if(auto bop=dynamic_cast<parsing::biopr*>(node);bop!=nullptr){
	if(ND::ASSIGN<=bop->type&&bop->type<=ND::RMASGN){
	    refer_var(bop->larg);
	    RDP(bop->rarg);
	    write("pop","rdi");
	    write("pop","rax");
	    switch(bop->type){
		case ND::ASSIGN:
		    write("mov","rdi",derefer("rax"));
		    break;
		case ND::PLASGN:
		    write("add","rdi",derefer("rax"));
		    break;
		case ND::MIASGN:
		    write("sub","rdi",derefer("rax"));
		    break;
		case ND::MUASGN:
		    write("mov","rax","rsi");
		    write("mov",derefer("rax"),"rax");
		    write("mul","rdi");
		    write("mov","rax",derefer("rsi"));
		    write("mov","rsi","rax");
		    break;
		case ND::DIASGN:
		    write("mov","rax","rsi");
		    write("mov",derefer("rax"),"rax");
		    write("mov",0,"rdx");
		    write("div","rdi");
		    write("mov","rax",derefer("rsi"));
		    write("mov","rsi","rax");
		    break;
		case ND::RMASGN:
		    write("mov","rax","rsi");
		    write("mov",derefer("rax"),"rax");
		    write("mov",0,"rdx");
		    write("div","rdi");
		    write("mov","rdx",derefer("rsi"));
		    write("mov","rsi","rax");
		    break;
	    }
	    write("push",derefer("rax"));
	}else{
	    RDP(bop->larg);
	    RDP(bop->rarg);
	    write("pop","rdi");
	    write("pop","rax");
	    switch(bop->type){
		case ND::PLUS:
		    write("add","rdi","rax");
		    break;
		case ND::MINUS:
		    write("sub","rdi","rax");
		    break;
		case ND::MULTI:
		    write("mul","rdi");
		    break;
		case ND::DIVIDE:
		    write("mov",0,"rdx");
		    write("div","rdi");
		    break;
		case ND::REMAIN:
		    write("mov",0,"rdx");
		    write("div","rdi");
		    write("mov","rdx","rax");
		    break;
		case ND::EQUAL:
		    write("cmp","rdi","rax");
		    write("sete","al");
		    write("movzb","al","rax");
		    break;
		case ND::NEQUAL:
		    write("cmp","rdi","rax");
		    write("setne","al");
		    write("movzb","al","rax");
		    break;
		case ND::LESS:
		    write("cmp","rdi","rax");
		    write("setl","al");
		    write("movzb","al","rax");
		    break;
		case ND::GREAT:
		    write("cmp","rdi","rax");
		    write("setg","al");
		    write("movzb","al","rax");
		    break;
		case ND::LEEQ:
		    write("cmp","rdi","rax");
		    write("setle","al");
		    write("movzb","al","rax");
		    break;
		case ND::GREQ:
		    write("cmp","rdi","rax");
		    write("setge","al");
		    write("movzb","al","rax");
		    break;
	    }
	    write("push","rax");
	}
    }
}
void writer::eval(parsing::statement*const st)
{
    if(auto sg=dynamic_cast<parsing::single*>(st);sg!=nullptr){
	eval(sg);
    }else if(auto com=dynamic_cast<parsing::compound*>(st);com!=nullptr){
	eval(com->stats);
    }
}
void writer::eval(parsing::single*const sg)
{
    if(!sg->is_nop()){
	enumerate_var(sg->stat);
	RDP(sg->stat);
	write("pop","rax");
    }
}
void writer::eval(const std::vector<parsing::statement*>&sv)
{
    for(int i=0;i<sv.size();++i){
	if(auto cif=dynamic_cast<parsing::_if_*>(sv[i]);cif!=nullptr){
	    if(auto cel=dynamic_cast<parsing::_else_*>(sv[i+1]);cel!=nullptr){
		std::string el=label("Lelse");
		std::string end=label("Lend");
		eval(cif->cond);
		write("cmp",0,"rax");
		write("je",el);
		eval(cif->st);
		write("jmp",end);
		write(el+':');
		eval(cel->st);
		write(end+':');
	    }else{
		std::string end=label("Lend");
		eval(cif->cond);
		write("cmp",0,"rax");
		write("je",end);
		eval(cif->st);
		write(end+':');
	    }
	}else if(auto cwh=dynamic_cast<parsing::_while_*>(sv[i]);cwh!=nullptr){
	    std::string beg=label("Lbegin");
	    std::string end=label("Lend");
	    write(beg+':');
	    eval(cwh->cond);
	    write("cmp",0,"rax");
	    write("je",end);
	    eval(cwh->st);
	    write("jmp",beg);
	    write(end+':');
	}else if(auto cfo=dynamic_cast<parsing::_for_*>(sv[i]);cfo!=nullptr){
	    std::string beg=label("Lbegin");
	    std::string end=label("Lend");
	    eval(cfo->init);
	    write(beg+':');
	    write("mov",1,"rax");
	    eval(cfo->cond);
	    write("cmp",0,"rax");
	    write("je",end);
	    eval(cfo->st);
	    eval(cfo->reinit);
	    write("jmp",beg);
	    write(end+':');
	}else{
	    eval(sv[i]);
	}
    }
}
void writer::enter(const std::string&func)
{
    write(func+':');
    ++indent;
    write("push","rbp");
    write("mov","rsp","rbp");
    var_size=0;
}
void writer::leave()
{
    write("mov","rbp","rsp");
    write("pop","rbp");
    write("retq");
    --indent;
    offset.clear();
}