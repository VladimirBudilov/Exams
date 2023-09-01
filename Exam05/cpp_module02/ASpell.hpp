#pragma once

#include <iostream>
#include "ATarget.hpp"

class ATarget;

class ASpell
{
	protected:
		std::string name;
		std::string effects;
	
	public:
		ASpell();
		virtual ~ASpell();
		ASpell(ASpell const &copy);
		ASpell &operator=(ASpell const &copy);

		std::string getName(void) const;
		std::string getEffects(void) const;
		virtual ASpell *clone(void) const = 0;

		ASpell(std::string const& name, std::string const &effects);
		void launch(ATarget const &target) const;	
};
