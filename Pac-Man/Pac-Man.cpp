#include <stdio.h>
#include "Minigin.h"

void LoadPacMan()
{

}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(LoadPacMan);

	return 0;
}