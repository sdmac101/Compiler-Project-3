/* Ruj Haan
 * TypeChecking.cpp
 * 4/7/21
 * Dr. Zhijiang Dong
 */


#include <sstream>
#include "TypeChecking.h"
#include "Absyn.h"

namespace semantics
{
	//insert a variable into the var/function symbol table
	void TypeChecking::insertVar(string name, symbol::SymTabEntry entry)
	{
		string			msg;
		stringstream	ss(msg);

		if ( env.getVarEnv()->localContains(name) )
		{
			symbol::SymTabEntry		old = env.getVarEnv()->lookup(name);
			ss << "variable " << name << " is already defined at line " << old.node->getLineno();
			error(entry.node, ss.str());

		}
		else
			env.getVarEnv()->insert( name, entry );
	}

	//insert a function into the var/function symbol table
	void TypeChecking::insertFunc(string name, symbol::SymTabEntry entry)
	{
		string			msg;
		stringstream	ss(msg);

		if ( env.getVarEnv()->localContains(name) )
		{
			symbol::SymTabEntry		old = env.getVarEnv()->lookup(name);
			ss << "function " << name << " is already defined at line " << old.node->getLineno();
			error(entry.node, ss.str());

		}
		else
			env.getVarEnv()->insert( name, entry );
	}

	//insert a type into the type symbol table
	void TypeChecking::insertType(string name, symbol::SymTabEntry entry)
	{
		string			msg;
		stringstream	ss(msg);

		if ( env.getTypeEnv()->localContains(name) )
		{
			symbol::SymTabEntry		old = env.getTypeEnv()->lookup(name);
			ss << "variable " << name << " is already defined at line " << old.node->getLineno();
			error(entry.node, ss.str());

		}
		else
			env.getTypeEnv()->insert( name, entry );
	}

	const types::Type* TypeChecking::visit(const Absyn *v)
	{
		if ( dynamic_cast<const Exp *>(v) != NULL )
			return visit( dynamic_cast<const Exp *>(v) );
		else if ( dynamic_cast<const Var *>(v) != NULL )
			return visit( dynamic_cast<const Var *>(v) );
		else if ( dynamic_cast<const Dec *>(v) != NULL )
			return visit( dynamic_cast<const Dec *>(v) );
		else
			throw runtime_error("invalid node");
	}

	const types::Type* TypeChecking::visit(const Exp *e)
	{
		if (dynamic_cast<const OpExp*>(e) != NULL)			
			return visit((const OpExp*)e);
		else if (dynamic_cast<const VarExp*>(e) != NULL)	
			return visit((const VarExp*)e);
		else if (dynamic_cast<const NilExp*>(e) != NULL)	
			return visit((const NilExp*)e);
		else if (dynamic_cast<const IntExp*>(e) != NULL)	
			return visit((const IntExp*)e);
		else if (dynamic_cast<const StringExp*>(e) != NULL) 
			return visit((const StringExp*)e);
		else if (dynamic_cast<const CallExp*>(e) != NULL)	
			return visit((const CallExp*)e);
		else if (dynamic_cast<const SeqExp*>(e) != NULL)	
			return visit((const SeqExp*)e);
		else if (dynamic_cast<const AssignExp*>(e) != NULL) 
			return visit((const AssignExp*)e);
		else if (dynamic_cast<const IfExp*>(e) != NULL)		
			return visit((const IfExp*)e);
		else if (dynamic_cast<const WhileExp*>(e) != NULL)	
			return visit((const WhileExp*)e);
		else if (dynamic_cast<const ForExp*>(e) != NULL)	
			return visit((const ForExp*)e);
		else if (dynamic_cast<const BreakExp*>(e) != NULL)	
			return visit((const BreakExp*)e);
		else if (dynamic_cast<const LetExp*>(e) != NULL)	
			return visit((const LetExp*)e);
		else if (dynamic_cast<const ArrayExp*>(e) != NULL)	
			return visit((const ArrayExp*)e);
		else throw new runtime_error("TypeChecking.visit(Exp*)");
	}

	const types::Type* TypeChecking::visit(const Var *v)
	{
		if (dynamic_cast<const SimpleVar *>(v) != NULL)			
			return visit((const SimpleVar *) v);
		else if (dynamic_cast<const SubscriptVar *>(v) != NULL) 
			return visit((const SubscriptVar *)v);
		else throw new runtime_error("TypeChecking.visit(Var*)");
	}

	const types::Type* TypeChecking::visit(const Dec *d)
	{
		if (dynamic_cast<const TypeDec *>(d) != NULL)			
			return visit((const TypeDec *) d);
		else if (dynamic_cast<const VarDec *>(d) != NULL)		
			return visit((const VarDec *) d);
		else throw new runtime_error("TypeChecking.visit(Dec*)");
	}

	const types::Type* TypeChecking::visit(const Ty *t)
	{
		if (dynamic_cast<const NameTy *>(t) != NULL)			
			return visit((const NameTy *) t);
		else if (dynamic_cast<const ArrayTy *>(t) != NULL)		
			return visit((const ArrayTy *) t);
		else throw new runtime_error("TypeChecking.visit(Ty*)");
	}

	//functions checking semantic error of different type of Var nodes
	const types::Type* TypeChecking::visit(const SimpleVar *v)
	{
		/* check if the variable is defined by looking up the symbol table*/
		string info;
		stringstream stream(info);
		string myName = v->getName();

		if (env.getVarEnv()->contains(myName)) {
			stream << "undefined variable";
			error(v, stream.str());
		}
		return NULL;
	}

	const types::Type* TypeChecking::visit(const SubscriptVar *v)
	{
		/* check both the variable and index */
		const Var* var = v->getVar();

		if (var != NULL)
			visit(var);

		const Exp* index = v->getIndex();

		if (index != NULL)
			visit(index);
		return NULL;
	}


	//functions checking semantic error of different type of Exp nodes
	const types::Type* TypeChecking::visit(const OpExp *e)
	{
		/* check both operands */
		const Exp* l = e->getLeft();
		const Exp* r = e->getRight();

		if (l != NULL)
			visit(l);
		if (r != NULL)
			visit(r);
		return NULL;
	}

	const types::Type* TypeChecking::visit(const VarExp *e)
	{
		/* check the variable */
		const Var* variable = e->getVar();

		if (variable != NULL)
			visit(variable);
		return NULL;
	}

	const types::Type* TypeChecking::visit(const NilExp *e)
	{
		//don't need to do anything
		return NULL;
	}

	const types::Type* TypeChecking::visit(const IntExp *e)
	{
		//don't need to do anything
		return NULL;
	}

	const types::Type* TypeChecking::visit(const StringExp *e)
	{
		//don't need to do anything
		return NULL;
	}

	const types::Type* TypeChecking::visit(const CallExp *e)
	{
		/*
			step 1: check the function name
			step 2: check every argument expression
		*/
		string myName = e->getFunc();
		string x;
		stringstream stream(x);

		if (!(env.getVarEnv()->contains(myName))) {
			stream << "undefined function name";
			error(e, stream.str());
		}

		const ExpList* args = e->getArgs();

		while (args != NULL) {
			const Exp* head = args->getHead();
			if (head != NULL)
				visit(head);
			args = args->getRest();
		}
		return NULL;
	}

	const types::Type* TypeChecking::visit(const SeqExp *e)
	{
		/*	check every expression in the sequence */
		const ExpList* list = e->getList();

		while (list != NULL) {
			const Exp* head = list->getHead();
			if (head != NULL)
				visit(head);
			list = list->getRest();
		}
		return NULL;
	}

	const types::Type* TypeChecking::visit(const AssignExp *e)
	{
		/* check both variable and expression */
		const Var* var = e->getVar();

		if (var != NULL)
			visit(var);

		const Exp* express = e->getExp();

		if (express != NULL)
			visit(express);
		return NULL;
	}

	const types::Type* TypeChecking::visit(const IfExp *e)
	{
		/* check test condition, then-clause, and else-clause (if exists) */
		const Exp* test = e->getTest();

		if (test != NULL)
			visit(test);

		const Exp* thenC = e->getThenClause();

		if (thenC != NULL)
			visit(thenC);

		const Exp* elseC = e->getElseClause();

		if (elseC != NULL)
			visit(elseC);
		
		return NULL;
	}

	const types::Type* TypeChecking::visit(const WhileExp *e)
	{
		/* check both the test condition and loop-body expression */
		const Exp* test = e->getTest();

		if (test != NULL)
			visit(test);

		const Exp* body = e->getBody();

		if (body != NULL)
			visit(body);
		return NULL;
	}

	const types::Type* TypeChecking::visit(const ForExp *e)
	{
		/*
			step 1: begin a new scope for variable
			step 2: check var declaration, upper bound expression, and loop-body
			step 3: end the scope
		*/
		env.getVarEnv()->beginScope();
		const VarDec* var = e->getVar();

		if (var != NULL)
			visit(var);

		const Exp* hi = e->getHi();

		if (hi != NULL)
			visit(hi);

		const Exp* body = e->getBody();

		if (body != NULL)
			visit(body);
		env.getVarEnv()->endScope();
		return NULL;
	}

	const types::Type* TypeChecking::visit(const BreakExp *e)
	{
		//don't need to do anything
		return NULL;
	}

	const types::Type* TypeChecking::visit(const LetExp *e)
	{
		/*
			step 1: begin a new scope for type and variable/function symbol tables
			step 2: check every declaraion in the decl list
			step 3: check the body expression
			step 4: end the scope for both symbol tables
		*/
		env.getVarEnv()->beginScope();
		env.getTypeEnv()->beginScope();
		const DecList* decs = e->getDecs();

		while (decs != NULL) {
			const Dec* head = decs->getHead();
			if (head != NULL)
				visit(head);
			decs = decs->getRest();
		}

		const Exp* body = e->getBody();

		if (body != NULL)
			visit(body);
		env.getVarEnv()->endScope();
		env.getTypeEnv()->endScope();
		
		return NULL;
	}

	const types::Type* TypeChecking::visit(const ArrayExp *e)
	{
		/*
			step 1: check the type of the array by looking up the type symbol table
			step 2: check the size expression
			step 3: check the initial expression
		*/
		string x;
		stringstream stream(x);
		string type = e->getType();

		if (!env.getTypeEnv()->contains(type)) {
			stream << "undefined array type";
			error(e, stream.str());
		}

		const Exp* size = e->getSize();

		if (size != NULL)
			visit(size);

		const Exp* init = e->getInit();

		if (init != NULL)
			visit(init);
		return NULL;
	}

	const types::Type* TypeChecking::visit(const VarDec *d)
	{
		/*
			step 1: insert the variable to the var/function symbol table
			step 2: if the type information is provided, check the type
			step 3: check the initial expression
		*/
		symbol::SymTabEntry* entry = new symbol::SymTabEntry;
		entry->node = d;
		entry->level = env.getVarEnv()->getLevel();
		entry->info = NULL;

		insertVar(d->getName(), *entry);

		const NameTy* type = d->getType();

		if (type != NULL)
			visit(type);

		const Exp* init = d->getInit();

		if (init != NULL)
			visit(init);
		return NULL;
	}

	const types::Type* TypeChecking::visit(const TypeDec *d)
	{
		/*
			step 1: insert the name of the new type to the type symbol table
			step 2: check the type information of the new type
		*/
		const TypeDec* ptr = d;

		while (ptr != NULL) {
			symbol::SymTabEntry* entry = new symbol::SymTabEntry;
			entry->node = d;
			entry->level = env.getTypeEnv()->getLevel();
			entry->info = NULL;

			insertType(d->getName(), *entry);

			const Ty* type = d->getTy();
			if (type != NULL)
				visit(type);
			ptr = ptr->getNext();
		}
		return NULL;
	}

	//functions checking semantic error of different type of Ty nodes
	const types::Type* TypeChecking::visit(const NameTy *t)
	{
		/*
			step 1: check if the type name is defined by looking up the type symbol table
		*/
		string name;
		stringstream stream(name);
		string myName = t->getName();

		if (!env.getTypeEnv()->contains(myName)) {
			stream << "undefined type name";
			error(t, stream.str());
		}
		return NULL;
	}


	const types::Type* TypeChecking::visit(const ArrayTy *t)
	{
		/*
			step 1: Check the name of the array type by looking up the type symbol table
		*/
		string name;
		stringstream stream(name);
		string myName = t->getName();

		if (!env.getTypeEnv()->contains(myName)) {
			stream << "undefined array type name";
			error(t, stream.str());
		}
		return NULL;
	}


} // end of namespace semantics
