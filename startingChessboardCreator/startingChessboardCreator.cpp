// chessBoardCreator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "chessBoardCreatorFunctions.h"

int main()
{
	std::string save;
	std::cout << "Podaj miejsce zapisu";
	std::cin >> save;
	chessmansStandard(save);


	return 0;
}

