#ifndef GAMEBATTLE_H
#define GAMEBATTLE_H

#include "GameObject.h"

#include <sstream>

//battle class: for doing a battle loop
class GameBattle
{
private:
	/*
	variables used in the battle loop:

	battleRotate: the queue of the battle candidates(player and enemies, but listed depends on speed, in decending order)
	BATTLE_COMMAND: the enum value describe the action command
	battleCommand: a list of battle candidates' action command <--could be change to enum for better vision

	inInt: battle command inputer cache
	inCmd: battle command inputer cache

	battleTurn: counting the turns battled between player and enemies
	currentAtker: the index of current attacker

	attackerPt: the attack value of the attacker
	attackerSpeed: the speed of the attacker, for stopping defender evades
	defenderPt: the defence value of the defender, to reduce damage from attacker
	defenderSpeed:the speed of the attacker, for avoiding attacker attacks

	*/
	GameObject* b_player;
	GameObject* b_enemy;
	GameObject* battleRotate[2];
	typedef enum {
		standby = 0,
		attack = 1,
		defence = 2,
		aware = 3,
		escape = 4
	} BATTLE_COMMAND;
	BATTLE_COMMAND battleCommand[2]= { standby,standby };
	bool battleLoop; //the boolean for breaking the loop
	BATTLE_COMMAND inCmd;

	int battleCounter;
	int currentAtker;
	int inInt;
	int attackerPt;
	int attackerSpeed;
	int defenderPt;
	int defenderSpeed;


	//void playerInput();

public:
	GameBattle();
	GameBattle(GameObject* player, GameObject* enemy);
	~GameBattle();

	void setAction(int inChoice);
	string getActionMsg();

	string askPlayer();
	string getBattleStatus();
	string getCurrentAtkrAttr();//output the current attacker's attribute
	string getCurrentAtkrMsg();//output the current attacker introduction message(eg: is XXX's turn!)
	int getTurn();
	//get the result of that battle 0:no change 1:one of the candidate is defeated 2:escaped
	//or change it to battleflow to do this result?
	int checkResult();
	int takeBattle();
};

#endif // !GAMEBATTLE_H





