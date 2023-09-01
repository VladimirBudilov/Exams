#include "TargetGenerator.hpp"

TargetGenerator::TargetGenerator(){}

TargetGenerator::~TargetGenerator(){}

TargetGenerator::TargetGenerator(TargetGenerator const &copy)
{
	*this = copy;
}

TargetGenerator &TargetGenerator::operator=(TargetGenerator const &copy)
{
	generator = copy.generator;
	return *this;
}

void TargetGenerator::learnTargetType(ATarget* target)
{
	if(target)
		generator[target->getType()] = target;
}

void TargetGenerator::forgetTargetType(std::string const & target)
{
	if(generator.find(target) != generator.end())
		generator.erase(generator.find(target));
}

ATarget* TargetGenerator::createTarget(std::string const &target)
{
	ATarget * temp = NULL;
	if(generator.find(target) != generator.end())
		temp = generator[target];
	return temp;
}
