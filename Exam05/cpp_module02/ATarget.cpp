#include "ATarget.hpp"

ATarget::ATarget(){}

ATarget::~ATarget(){}

ATarget::ATarget(ATarget const &copy)
{
	*this = copy;
}

ATarget &ATarget::operator=(ATarget const &copy)
{
	type = copy.type;
	return *this;
}
std::string ATarget::getType(void) const
{
	return type;
}

ATarget::ATarget(std::string const &type)
{
	this->type = type;
}

void ATarget::getHitBySpell(ASpell const &spell) const
{
	std::cout << type << " has been " << spell.getEffects() << "!" << std::endl;
}
