#pragma once
#include "iostream"
#include <map>

#include "ATarget.hpp"

class ATarget;

class TargetGenerator
{
	private:
		TargetGenerator(TargetGenerator const &copy);
		TargetGenerator &operator=(TargetGenerator const &copy);

		std::map<std::string, ATarget * > generator;
	public:
		TargetGenerator();
		~TargetGenerator();

		void learnTargetType(ATarget* target);
		void forgetTargetType(std::string const & target);
		ATarget* createTarget(std::string const &target);
};
