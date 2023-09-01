#pragma once

#include <iostream>
#include "ASpell.hpp"

class ASpell;

class ATarget
{
	protected:
		std::string type;
	public:
		ATarget();
		ATarget(ATarget const & copy);
		~ATarget();
		ATarget &operator=(ATarget const & copy);

		std::string getType(void) const;
		virtual ATarget *clone(void) const = 0;
		ATarget(std::string const &type);
		void getHitBySpell(ASpell const &spell) const;

};
