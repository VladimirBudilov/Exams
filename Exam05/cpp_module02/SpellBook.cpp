#include "SpellBook.hpp"

SpellBook::SpellBook(){}

SpellBook::~SpellBook(){}

SpellBook::SpellBook(SpellBook const & copy)
{
	*this = copy;
}

SpellBook &SpellBook::operator=(SpellBook const &copy)
{
	book = copy.book;
	return *this;
}

void SpellBook::learnSpell(ASpell * spell)
{
	if(spell)
		book[spell->getName()] = spell;
}

void SpellBook::forgetSpell(std::string const & spell)
{
	if(book.find(spell) != book.end())
		book.erase(book.find(spell));
}

ASpell* SpellBook::createSpell(std::string const &spell)
{
	ASpell *tmp = NULL;
	if(book.find(spell) != book.end())
		tmp = book[spell];
	return tmp;
}

