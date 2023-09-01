#include "Warlock.hpp"

Warlock::Warlock() {
    std::cout << name << ": This looks like another boring day." << std::endl;
};

Warlock::~Warlock() {
    std::cout << name << ": My job here is done!" << std::endl;
};

Warlock::Warlock(Warlock const &copy) {
    *this = copy;
    std::cout << name << ": This looks like another boring day." << std::endl;
};

Warlock &Warlock::operator=(Warlock const &copy) {
    name = copy.name;
    title = copy.title;
    return *this;
}

std::string const &Warlock::getName(void) const {
    return name;
}

std::string const &Warlock::getTitle(void) const {
    return title;
}

Warlock::Warlock(const std::string &name, const std::string &title) : name(name), title(title) {
    std::cout << name << ": This looks like another boring day." << std::endl;
}

void Warlock::introduce(void) const {
    std::cout << name << ": I am " << name << ", " << title << "!" << std::endl;
}

void Warlock::setTitle(std::string const &title) {
    this->title = title;
}

void Warlock::learnSpell(ASpell *spell) {
    if (spell)
        book[spell->getName()] = spell;
}

void Warlock::forgetSpell(std::string spell) {
    if (book.find(spell) != book.end())
        book.erase(book.find(spell));

}

void Warlock::launchSpell(std::string spell, ATarget const &target) {
    if (book.find(spell) != book.end())
        book[spell]->launch(target);
}

