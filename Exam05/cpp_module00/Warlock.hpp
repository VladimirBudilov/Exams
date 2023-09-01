#include <iostream>

class Warlock
{
private:
	std::string name;
	std::string title;
	
	Warlock();
	Warlock(Warlock const &copy);
	Warlock &operator=(Warlock const &copy);

public:

    Warlock(const std::string &name, const std::string &title);
    ~Warlock();

    std::string const &getName(void) const;
	std::string const &getTitle(void) const;
	void introduce(void) const;

	void setTitle(std::string const &title);
};
