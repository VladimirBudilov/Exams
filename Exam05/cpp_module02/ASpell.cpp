#include "ASpell.hpp"

ASpell::ASpell(){}

ASpell::~ASpell(){}

ASpell::ASpell(ASpell const &copy)
{
	*this=copy;
}

ASpell &ASpell::operator=(ASpell const  &copy)
{
	name = copy.name;
	effects = copy.effects;
	return *this;
}

std::string ASpell::getName(void) const
{
	return name;
}

std::string ASpell::getEffects(void) const
{
	return effects;
}

ASpell::ASpell(std::string const &name, std::string const &effects)
{
	this->name = name;
	this->effects = effects;
}

void ASpell::launch(ATarget const &target) const
{
	target.getHitBySpell(*this);
}
