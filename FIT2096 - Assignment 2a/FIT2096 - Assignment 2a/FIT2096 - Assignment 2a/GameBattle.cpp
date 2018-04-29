#include "GameBattle.h"



int GameBattle::takeBattle(){

	//return values:
	//0: no result (and then increse the counter to switch candidate)
	//1: player defeated
	//2: enemy defeated
	//3: player escaped (see above if inCmd==4 operation )

	//setup output string
	//set up values
	attackerPt = battleRotate[battleCounter%2]->getAtk();
	defenderPt = battleRotate[(battleCounter+ 1)%2]->getDef();
	attackerSpeed = battleRotate[battleCounter%2]->getSpeed();
	defenderSpeed = battleRotate[(battleCounter+ 1) % 2]->getSpeed() / 4 + 1;

	Sleep(500);

	//do action.
	//if the candidate attacks
	if (inCmd == attack) {
		//check the opponent's action to determine the damage calculation

		//if opponent choose defence
		if (battleCommand[(battleCounter + 1) % 2] == 2) {
			//boost its defence 50%
			defenderPt = defenderPt * 3 / 2;
		}
		//choose standby
		else if (battleCommand[(battleCounter + 1) % 2] == 3) {
			//boost its speed 50%
			defenderSpeed = defenderSpeed * 3 / 2;
		}

		//check if opponent can evade or not and take damages
		//can't evade
		if (rand() % attackerSpeed + 1 > rand() % defenderSpeed + 1) {
			//start damage calculation
			int damage = rand() % attackerPt - rand() % defenderPt;
			if (damage < 0) { damage = 0; }
			else {
				battleRotate[(battleCounter + 1) % 2]->changeHP(-damage);
			}
			cout << battleRotate[(battleCounter + 1) % 2]->getName() << " takes " << damage << " damage!\n";

			//and then check the HP for the both candidates, if one of it is zero, return specfic value
			//to stop the loop operation.
			if (battleRotate[(battleCounter + 1) % 2]->getHP() <= 0) {
				//battleLoop = false;
				if (battleRotate[(battleCounter + 1) % 2]->getAttr() == "player") {
					return 2;
				}
				else {
					return 1;
				}
			}
		}
		//can evade
		else {
			cout << battleRotate[(battleCounter + 1) % 2]->getName() << " evades the attack!\n";
		}



	}
	//defence
	else if (inCmd == defence) {
	}
	//aware
	else if (inCmd == aware) {
	}
	//escape
	else if (inCmd == escape) {
		//50% of chance to escape
		//0: can't escape
		//1: can escape
		if ((rand() % 2) == 1) {
			return 3;
		}
		else {
			cout << "but failed!\n";
		}
	}

	Sleep(500);

	//if no one is defeated, increase the counter and return 0 for next iteration;
	battleCounter++;
	return 0;

	//




}

GameBattle::GameBattle()
{
}

GameBattle::GameBattle(GameObject * player, GameObject * enemy)
{
	//store player and enemy address to the memory
	b_player = player;
	b_enemy = enemy;
	//initialize the counters, input value, array for managing rotation, some calculating var.
	battleCounter = 0;
	battleLoop = true; //the boolean for breaking the loop
	inInt=0 ;
	attackerPt=0;
	attackerSpeed=0;
	defenderPt=0;
	defenderSpeed=0;

	//check player and the enemy's speed to determine who goes first
	if (player->getSpeed() >= enemy->getSpeed()) {
		battleRotate[0] = player;
		battleRotate[1] = enemy;
	}
	else {
		battleRotate[0] = enemy;
		battleRotate[1] = player;
	}

	srand(time(NULL));

}


GameBattle::~GameBattle()
{
}

void GameBattle::setAction(int inChoice)
{
	//set the action to the command variable
	inCmd = static_cast<BATTLE_COMMAND>(inChoice);
	//and then save the command for next turn
	battleCommand[currentAtker] = inCmd;
}

string GameBattle::getActionMsg()
{
	//output the inputed command in a sentence
	stringstream outString;

	outString << battleRotate[battleCounter%2]->getName();
	if (inCmd == attack) {
			outString<< " chooses to attack "<< battleRotate[(battleCounter + 1) % 2]->getName() << "!\n";
		}
	else if (inCmd == defence) {
		outString << " chooses to defend himself!\n";
	}
	else if (inCmd == aware) {
		outString << " chooses to aware himself!\n";
	}
	else if (inCmd == escape) {
		outString << " chooses to escape!\n";
	}

	return outString.str();
}

string GameBattle::askPlayer()
{
	//return the string we designated to ask for
	return "what would you like to do ? \n1 : attack(attack to enemy)\n2 : defence(not attack but boost your defence 150 % in 1 turn)\n3 : standby(not attack but boost your evade speed 150 % in 1 turn)\n4 : escape from enemy(50% of chance)\nPlease choose your choice by entering the number directly\n";
}

string GameBattle::getBattleStatus()
{
	stringstream outString;

	outString << b_enemy->displayStatus() << "\n\n\t\t\tVS\n\n" << b_player->displayStatus() << endl;
	return outString.str();
}

string GameBattle::getCurrentAtkrAttr()
{
	return battleRotate[battleCounter%2]->getAttr();
}

string GameBattle::getCurrentAtkrMsg()
{
	stringstream outString;
	outString << "It's " << battleRotate[battleCounter%2]->getName()<<"'s turn!\n";
	return outString.str();
}

int GameBattle::getTurn()
{
	return battleCounter;
}

int GameBattle::checkResult()
{
	//check the HP for the both candidates, if one of it is zero, call the main program to
	//stop the loop operation.
	//1: have result
	//0: no result
	//2: player escaped
	if (battleRotate[(battleCounter + 1) % 2]->getHP() <= 0) {
		//battleLoop = false;
		return 1;
	}
	battleCounter++;
	Sleep(1000);
	return 0;

	/*
		//check the HP for the both candidates, if one of it is zero, call the main program to
		//stop the loop operation.
		//1: have result
		//0: no result (and then increse the counter to switch candidate)
		//2: player escaped (see above if inCmd==4 operation )
		if (battleRotate[(battleCounter + 1) % 2]->getHP() <= 0) {
			//battleLoop = false;
			//return 1;
		}
		battleCounter++;
		//return 0;

	*/
}


