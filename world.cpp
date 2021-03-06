#include <generator.h>

using namespace generator;
using namespace std;

std::map<std::string *, Module *>::iterator World::modulesIterBegin()
{
	return this->Modules->begin();
}

std::map<std::string *, Module *>::iterator World::modulesIterEnd()
{
	return this->Modules->end();
}

bool World::moduleAdd(std::string *name, Module *m)
{
	this->Modules->insert(std::pair<std::string *, Module *>(name, m));
	return true;
}

std::list<std::string *>::iterator World::filesIterBegin()
{
	return this->Files->begin();
}

std::list<std::string *>::iterator World::filesIterEnd()
{
	return this->Files->end();
}

bool World::print_logic_header(std::ofstream &logic)
{
	std::list<std::string *>::iterator fCurr = filesIterBegin();
	std::list<std::string *>::iterator fEnd = filesIterEnd();
			
	logic << "/* This file was automatically generated by generator from (one of): *" << std::endl;
	for(; fCurr != fEnd; fCurr++)
	{
		logic << " * " << *fCurr << std::endl;
	}
	logic  << " */" << std::endl;
	return true;
}

bool World::generate(const char *output_dir)
{
	std::map<std::string *, Module *>::iterator curr = modulesIterBegin();
	std::map<std::string *, Module *>::iterator end = modulesIterEnd();

	// create the directories we need to make
	int res = mkdir(output_dir, 0755);
	if(res != 0 && errno != EEXIST)
		gen_error(strerror(errno));
	
	char *lb_path = NULL;
	asprintf(&lb_path, "%s/lib", output_dir);
	res = mkdir(output_dir, 0755);
	free(lb_path);
	if(res != 0 && errno != EEXIST)
		gen_error(strerror(errno));

	for( ; curr != end ; ++curr)
	{
		if(!curr->second->generate(curr->first, output_dir))
		{
			std::cerr << "Error generating module: " << curr->first << std::endl;
			return false;
		}
	}

	return true;
}
