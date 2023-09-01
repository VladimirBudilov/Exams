#pragma once

#include <iostream>
#include <map>
#include "ATarget.hpp"
#include "ASpell.hpp"
#include "SpellBook.hpp"

class Spellbook;
class ASpell;
class ATarget;

class Warlock
{
private:
	SpellBook book;
	std::string name;
	std::string title;
	
	Warlock();
	Warlock(Warlock const &copy);
	Warlock &operator=(Warlock const &copy);

public:
	std::string const &getName(void) const;
	std::string const &getTitle(void) const;
	
	Warlock(const std::string &name, const std::string &title);
	~Warlock();
	void introduce(void) const;
	void setTitle(std::string const &title);
	
	void learnSpell(ASpell *spell);
	void forgetSpell(std::string spell);
	void launchSpell(std::string spell, ATarget const &target);
};
