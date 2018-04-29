/*	FIT2049 - Example Code
*	Game.h
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*	This class is the heart of our game and is also where our game logic will reside
*	It contains the overall Update and Render method for the whole game
*	You should eventually split some game logic into other objects which Game will manage
*	Watch the size of this class - it can grow out of control very fast!
*/

#ifndef GAME_H
#define GAME_H

#include "Direct3D.h"
#include "Camera.h"
#include "InputController.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "GameBattle.h"
#include "FPCamera.h"

#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <fstream>
#include <sstream>
#include<limits>
#undef max
#define MAX_MAP_SIZE 30 
#define MAX_HEALTH 99

using namespace std;

class Game
{
private:
	//game stuffs
	/*
	m_player: object of player

	m_mapObjects: the array of the removable objects(eg:enemy,item,warp)
	m_mapBlocks: the array of unremovable objects(eg:floor)(wall should be here, but due to the bug of the position generator, I can't able to set it here but mapObjects)
	m_mapSize: the size of the whole map
	m_gameSwitch: switch for breaking the game loop

	GAME_MODE: enum of describing the game mode
	m_gameModeSwitch: mode switch of the game
	(initial for initialize the game, map for map movement, battling for in battle)

	 m_battle: battle for player and enemy
	 m_battleEnemy: the 2D array position for the enemy to be battled
	 
	 m_lastPosition:the position player reached last, for limiting player's walking path.
	 isInputing: the switch for detecting player is inputed a valid command or not when allowing to input in battle.

	 m_lastPosition: the last position player reached
	 isInputing: boolean that check player has inputed the value or not.
	*/
	GameObject* m_player;

	GameObject* m_mapObjects[MAX_MAP_SIZE][MAX_MAP_SIZE];
	GameObject* m_mapBlocks[MAX_MAP_SIZE][MAX_MAP_SIZE];

	GameObject* m_HPBar;

	//change that to array of int after established
	int m_battleEnemy[2];
	GameBattle* m_battle;

	int m_mapSize;
	bool m_gameSwitch;

	typedef enum { initial = 0, map = 1, battling = 2 }GAME_MODE;
	GAME_MODE m_gameModeSwitch;
	

	Vector3 m_lastPosition;
	bool isInputing;


	//3D stuffs
	Camera* m_currentCam;
	Direct3D* m_renderer;
	InputController* m_input;
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;

	Shader* m_unlitVertexColouredShader;
	Shader* m_unlitTexturedShader;

	vector<GameObject*> m_gameObjects;

	

	// Splitting initialisation up into several steps
	bool InitShaders();
	bool LoadMeshes();
	bool LoadTextures();
	void InitGameWorld();

	//creating enemy, item, wrap zone and walls
	void genEnemy();
	void genItem();
	void genWrap();
	void genWall();
	void genFloor();

	//functions for generate locations
	vector<Vector3> genLocation(int genNumber);

	//identify the position and take actions
	void checkPos();

	//take battle, the output integer will describe the condition
		//0: no result is occured
		//1: player wins (cond: player remains health and enemy not)
		//2: player lose (cond: player has no health and enemy has)
		//3: player escaped(cond: player selected the escape option)
	int battle(GameObject* player, GameObject* enemy);

	//sprites
	void InitUI();
	void DrawUI();
	void RefreshUI();

	// Sprites / Fonts
	SpriteBatch* m_spriteBatch;
	SpriteFont* m_arialFont12;
	SpriteFont* m_arialFont18;

	void LoadFonts();

	// Wide strings use more than 8 bits per character so they can capture more symbols
	// Windows loves them and as such so does Direct3D and its text helpers
	wstring m_currentHPText;
	wstring m_currentEnemyText;
	wstring m_battleInterfaceReminder;
	Texture* m_currentHPSprite;
	vector<Texture*> m_currentHPSprites;


public:
	Game();	
	~Game();

	//core methods
	bool Initialise(Direct3D* renderer, InputController* input); //The initialise method will load all of the content for the game (meshes, textures, etc.)

	void Update(float timestep);	//The overall Update method for the game. All gameplay logic will be done somewhere within this method
	void Render();					//The overall Render method for the game. Here all of the meshes that need to be drawn will be drawn

	void Shutdown(); //Cleanup everything we initialised

	//other methods
	bool isGameOver();//trace the game is finished or not. true: the game is over, false: can still keep going
};

#endif