/*	FIT2049 - Example Code
*	Game.cpp
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*/

#include "Game.h"
#include "TexturedShader.h"




Game::Game()
{
	m_renderer = NULL;
	m_currentCam = NULL;
	m_input = NULL;
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_unlitTexturedShader = NULL;
	m_unlitVertexColouredShader = NULL;
}

Game::~Game() {}

bool Game::Initialise(Direct3D* renderer, InputController* input)
{
	m_renderer = renderer;	
	m_input = input;
	m_meshManager = new MeshManager();
	m_textureManager = new TextureManager();
	m_currentCam = new Camera();

	if (!InitShaders())
		return false;

	if (!LoadMeshes())
		return false;

	if (!LoadTextures())
		return false;

	InitGameWorld();

	m_gameSwitch = true;

	MessageBox(NULL, "Game Start!\n Reminder: please seperate the graphic and graphic interface first\nclick 'OK' to continue...", "initialize complete", MB_OK);

	return true;
}

bool Game::InitShaders()
{
	m_unlitVertexColouredShader = new Shader();
	if (!m_unlitVertexColouredShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/VertexColourPixelShader.ps"))
		return false;
	
	m_unlitTexturedShader = new TexturedShader();
	if (!m_unlitTexturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/TexturedPixelShader.ps"))
		return false;

	return true;
}

bool Game::LoadMeshes()
{
	// Don't load a mesh until you've completed its "Create" function in the Mesh class
	// If you get a "Could not create window" error, you're loading an incomlpete mesh

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/player_capsule.obj")) {
		return false;
	}

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/floor_tile.obj")) {
		return false;
	}

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/wall_tile.obj")) {
		return false;
	}

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/progress_cube.obj")) {
		return false;
	}
	
	return true;
}

bool Game::LoadTextures()
{
	// this will load the texture of the mesh(some of it can be shared with 1 mesh as well)

	if(!m_textureManager->Load(m_renderer, "Assets/Textures/checkerboard.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/floor.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/warp.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/enemy.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/item.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/enemyStrong.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/healthBar.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/wall.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/player.jpg")) {
		return false;
	}



	return true;
}

void Game::InitGameWorld()
{
	// Create some GameObjects so we can see our custom meshes
	// We need to load the meshes we intend to use.
	// This is in Game::LoadMeshes
	/*
	m_gameObjects.push_back(new GameObject(
		m_meshManager->GetMesh("Assets/Meshes/player_capsule.obj"),
		m_unlitTexturedShader, 
		m_input,
		Vector3(2.0f,0.0f,-1.0f),
		m_textureManager->GetTexture("Assets/Textures/checkerboard.jpg")));

		m_gameObjects.push_back(new GameObject(
		m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),
		m_unlitTexturedShader,
		m_input,
		Vector3(2.0f,0.0f,-1.0f),
		m_textureManager->GetTexture("Assets/Textures/checkerboard.jpg")));
	*/

	//initialize map property
		//maybe have the preset map?
		//or generate a self define map with certain uniform size?<== no thanks, might be preserved the space
	m_mapSize = 20;
	GameObject::mapSize = static_cast<float>(m_mapSize);

	//initialize player
		//ask the name of the player
	string m_playerName = "Player"; //might change into a lambda function
		//generate player object 
		// send the address to the gameObject
	m_player= new GameObject(
		m_meshManager->GetMesh("Assets/Meshes/player_capsule.obj"),//define mesh
		m_unlitTexturedShader,//define shader
		m_input,//define input value...actually no use for non player object
		Vector3(0.0f, 0.0f, 0.0f),//define initial position
		m_textureManager->GetTexture("Assets/Textures/player.jpg"),//define mesh texture
		m_playerName, "player", 10, 10, 10, 10 );//define parameters of the certain object
	m_gameObjects.push_back(m_player);

	//initialize arrays of non-player object
	//known bug: the random position generating function can't avoid choosing the 0,0 position when generating
	genWall();
		//initialize enemies
	genEnemy();
		//initialize items
	genItem();
		//initialize wrap zone (10 pcs)
	genWrap();
		//initialize floor
			//generate the remaining NULL array item into floors
	genFloor();

	//send the array of gameObject to the gameObjects
	for (int z = 0; z < m_mapSize; z++) {// z:row
		for (int x = 0; x < m_mapSize; x++) {//x:column
			m_gameObjects.push_back(m_mapBlocks[x][z]);
		}
	}
	
	m_HPBar = new GameObject(
		m_meshManager->GetMesh("Assets/Meshes/progress_cube.obj"),//define mesh
		m_unlitTexturedShader,//define shader
		m_input,//define input value...actually no use for non player object
		Vector3(0.0f, 0.0f, 0.0f),//define initial position
		m_textureManager->GetTexture("Assets/Textures/healthBar.jpg"),//define mesh texture
		"HPBar");//define parameters of the certain object
	m_gameObjects.push_back(m_HPBar); 

	cout << m_player->displayStatus(true);
	m_gameSwitch = true;
}

void Game::genEnemy()
{
	//generate enemy
	/*
	preprare:
	import the name of monster and its status
	then generate a set of enemy location based on the number of them
	*/

	vector<string> objNames;
	vector<string> objAttrs;
	vector<int> objStat;
	fstream objFile;
	objFile.open("Assets/Database/monster.txt");
	string objLine;
	while (getline(objFile, objLine, '\n')) {
		string objData;
		vector<string> tempVector;
		istringstream tLine(objLine);
		while (getline(tLine, objData, '\t')) {
			tempVector.push_back(objData);
		}
		objNames.push_back(tempVector[0]);
		objAttrs.push_back(tempVector[1]);
		int tempArray[4] = { stoi(tempVector[2]), stoi(tempVector[3]), stoi(tempVector[4]), stoi(tempVector[5]) };
		objStat.insert( objStat.end(), tempArray, end(tempArray) );
	}
	vector<Vector3> locationList = genLocation(objNames.size());
	/*
	create:
	set those enemy to the array, then do the increment on its counter
		the powerful enemy(overall value are more than 100(?) will be used a dimmer color texture
		(be sure to notice in cmd screen)
		and the number of monsters for game loop control
	*/
	for (int i = 0; i != locationList.size(); i++) {
		int xPos = static_cast<int>(locationList[i].x);
		int zPos = static_cast<int>(locationList[i].z);
		int objIndex = i*4;
		//if enemy has much health, it will change it to a deeper color texture
		auto enemyTexture = [](int inHP) {
			if (inHP >= 50) {
				return "Assets/Textures/enemyStrong.jpg";
			}
			else {
				return "Assets/Textures/enemy.jpg";
			}
			return "";
		};

		m_mapBlocks[zPos][xPos] = new GameObject(
			m_meshManager->GetMesh("Assets/Meshes/floor_tile.obj"),//define mesh
			m_unlitTexturedShader,//define shader
			m_input,//define input value...actually no use for non player object
			Vector3(static_cast<float>(xPos) - 10.0f, 0.0f, static_cast<float>(zPos) - 10.0f),//define initial position
			m_textureManager->GetTexture(enemyTexture(objStat[objIndex + 3])),//define mesh texture
			objNames[i], objAttrs[i], objStat[objIndex], objStat[objIndex + 1], objStat[objIndex + 2], objStat[objIndex + 3]);
	}
	GameObject::m_enemyNumbers=objNames.size();


}

void Game::genItem()
{
	//generate items
	/*
	preprare:
	import the name of items and its status
	then generate a set location based on the number of them
	*/
	vector<string> objNames;
	vector<string> objAttrs;
	vector<int> objStat;
	fstream objFile;
	objFile.open("Assets/Database/item.txt");
	string objLine;
	while (getline(objFile, objLine, '\n')) {
		string objData;
		vector<string> tempVector;
		istringstream tLine(objLine);
		while (getline(tLine, objData, '\t')) {
			tempVector.push_back(objData);
		}
		objNames.push_back(tempVector[0]);
		objAttrs.push_back(tempVector[1]);
		int tempArray[4] = { stoi(tempVector[2]), stoi(tempVector[3]), stoi(tempVector[4]), stoi(tempVector[5]) };
		objStat.insert(objStat.end(), tempArray, end(tempArray));
	}

	/*
	create:
	set those item to the gameObject array
	the powerful item(overall value are more than 100(?) will be used a dimmer color texture
	(be sure to notice in cmd screen)
	*/
	vector<Vector3> locationList = genLocation(objNames.size());
	for (int i = 0; i != locationList.size(); i++) {
		int xPos = static_cast<int>(locationList[i].x);
		int zPos = static_cast<int>(locationList[i].z);
		int objIndex = i * 4;
		m_mapBlocks[zPos][xPos] = new GameObject(
			m_meshManager->GetMesh("Assets/Meshes/floor_tile.obj"),//define mesh
			m_unlitTexturedShader,//define shader
			m_input,//define input value...actually no use for non player object
			Vector3(static_cast<float>(xPos) - 10.0f, 0.0f, static_cast<float>(zPos) - 10.0f),//define initial position
			m_textureManager->GetTexture("Assets/Textures/item.jpg"),//define mesh texture
			objNames[i], objAttrs[i], objStat[objIndex], objStat[objIndex + 1], objStat[objIndex + 2], objStat[objIndex +3]);
	}
}

void Game::genWrap()
{
	//generate wrap zone
	/*
	preprare:
	generate a set of wrap location(in random)
	and then set those location into the static vector for wraping action
	*/
	vector<Vector3> locationList = genLocation(10);
	GameObject::m_warpList.clear();//clear all data of the vector first
	GameObject::m_warpList = locationList;
	
	/*
	create:
	set those warp zone to the gameObject array
	*/
	for (int i = 0; i != locationList.size(); i++) {
		int xPos = static_cast<int>(locationList[i].x);
		int zPos = static_cast<int>(locationList[i].z);
		m_mapBlocks[zPos][xPos] = new GameObject(
			m_meshManager->GetMesh("Assets/Meshes/floor_tile.obj"),//define mesh
			m_unlitTexturedShader,//define shader
			m_input,//define input value...actually no use for non player object
			Vector3(static_cast<float>(xPos)-10.0f, 0.0f, static_cast<float>(zPos)-10.0f),//define initial position
			m_textureManager->GetTexture("Assets/Textures/warp.jpg"),//define mesh texture
			"warp");
	}
}

void Game::genWall()
{
	//generate
	/*
	create border wall:
	generate a set of wall location and surrounding wall(do this first)
	*/
	for (int z = 0; z < m_mapSize; z++) {
		if (z == 0 || z == m_mapSize - 1) {
			for (int x = 0; x < m_mapSize; x++) {
				m_mapBlocks[z][x] = new GameObject(
					m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),//define mesh
					m_unlitTexturedShader,//define shader
					m_input,//define input value...actually no use for non player object
					Vector3(static_cast<float>(x) - 10.0f, 0.0f, static_cast<float>(z) - 10.0f),//define initial position
					m_textureManager->GetTexture("Assets/Textures/wall.jpg"),//define mesh texture
					"wall");

			}
		}
		else {
			m_mapBlocks[z][0] = new GameObject(
				m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),//define mesh
				m_unlitTexturedShader,//define shader
				m_input,//define input value...actually no use for non player object
				Vector3(-10.0f, 0.0f, static_cast<float>(z) - 10.0f),//define initial position
				m_textureManager->GetTexture("Assets/Textures/wall.jpg"),//define mesh texture
				"wall");
			m_mapBlocks[z][m_mapSize - 1] = new GameObject(
				m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),//define mesh
				m_unlitTexturedShader,//define shader
				m_input,//define input value...actually no use for non player object
				Vector3(static_cast<float>(m_mapSize - 1) - 10.0f, 0.0f, static_cast<float>(z) - 10.0f),//define initial position
				m_textureManager->GetTexture("Assets/Textures/wall.jpg"),//define mesh texture
				"wall");

		}
	}
	vector<Vector3> locationList = genLocation(50);
	/*
	create random wall:
	 generate the surrounding wall of the map.
	*/
	for (int i = 0; i != locationList.size(); i++) {
		int xPos = static_cast<int>(locationList[i].x);
		int zPos = static_cast<int>(locationList[i].z);
		m_mapBlocks[zPos][xPos] = new GameObject(
			m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),//define mesh
			m_unlitTexturedShader,//define shader
			m_input,//define input value...actually no use for non player object
			Vector3(static_cast<float>(xPos) - 10.0f, 0.0f, static_cast<float>(zPos) - 10.0f),//define initial position
			m_textureManager->GetTexture("Assets/Textures/wall.jpg"),//define mesh texture
			"wall");
	}

}

void Game::genFloor()
{
	//generate floor
	//loop all the dimension of the array
	for (int z = 0; z < 20; z++) {// z:row
		float zPos = static_cast<int>(z) - 10.0f;
		for (int x = 0; x < 20; x++) {//x:column
			float xPos = static_cast<int>(x) - 10.0f;
			if (m_mapBlocks[z][x] == NULL) {// if the array item is blank, create the floor.
				m_mapBlocks[z][x] = new GameObject(
					m_meshManager->GetMesh("Assets/Meshes/floor_tile.obj"),//define mesh
					m_unlitTexturedShader,//define shader
					m_input,//define input value...actually no use for non player object
					Vector3(xPos, 0.0f, zPos),//define initial position
					m_textureManager->GetTexture("Assets/Textures/floor.jpg"),//define mesh texture
					"floor");//define parameters of the certain object
			}
		}
	}
}

vector<Vector3> Game::genLocation(int genNumber)
{
	vector<Vector3> outLocation;
	Vector3 randLocation;
	//generate number of UNIQUE location depends on getNumber
	for (int i = 0; i < genNumber; i++) {
		bool isUnique = true;
		//keep generate till it's unique.
		do {
			bool isUnique = true;		//reset the boolean switch to true as a default situaton
			int randX = rand() % (m_mapSize-2) +1;//generate the random x location from 1 to max size-1
			int randZ = rand() % (m_mapSize-2) +1;//generate the random z location from 1 to max size-1
			randLocation = Vector3(static_cast<float>(randX), 0.0f, static_cast<float>(randZ));//set it as the Vector.

			// check the location has no occupant to the array and avoid the starting grid, otherwise do the random position again
			//bug: sometimes it will generated to 0,0 position. and I don't know why.
			if ( ((randZ== (m_mapSize / 2) ) && (randX == (m_mapSize / 2)) ) || m_mapBlocks[randZ][randX] != NULL ) { isUnique = false; }
			//if not occupied, make sure the randomized location is unique to all set
			else {
				for (int j = 0; j != outLocation.size(); j++) {
					if (outLocation[j] == randLocation) {
						isUnique = false;
						break;
					}
				}
			}
		} while (isUnique==false);
		outLocation.push_back(randLocation);// and then add it to the output vector.
	}

	return outLocation;
}

void Game::checkPos()
{

	//that should be check the value of the  position, if higher/lower, it will change it back to the boarder value....?
	//set it to the game

	//collect the array and its coordination(and integer) of the current position(in integer)
	float posX = m_player->GetPosition().x;
	float posZ = m_player->GetPosition().z;
	float correctedPosX = posX - 0.5f;
	float correctedPosZ = posZ - 0.5f;
	int gridX = 0;
	int	gridZ = 0;

	//super dirty way: elimate double zeros after turning to integer with round down
	if (correctedPosX > 0) { gridX = static_cast<int>(correctedPosX) + 1; }
	else { gridX = static_cast<int>(correctedPosX); }
	if (correctedPosZ > 0) { gridZ = static_cast<int>(correctedPosZ) + 1; }
	else { gridZ = static_cast<int>(correctedPosZ); }

	int arrX = gridX + 10;
	int arrZ = gridZ + 10;

	//those are for checking if player touching the wall or not
	{
		//check the boarder of the map is over or not, otherwise no moving
		//check the player is touching the wall or not, by checking its direction
		//(although it could be a little buggy on the detection, at least it works first, 
		//but it's best to find a matrix way but that's way too hard for me in certain time limit)
		//known bug: when going to the corner of some wall pattern, it can sneak inside easily, if it's over the bound, it'll crash the program.

		//if goes up
		if(posZ < (static_cast<float>(m_mapSize) / 2 -0.75f))
		{	
			int predictUp = static_cast<int>(correctedPosZ + 0.25f);
			//de-double zero (super dirty way, really need someone how to use a proper way to get the colision mechanism)
			if ((correctedPosZ + 0.25f) > 0) { predictUp += 11; }
			else { predictUp += 10; }
			//trace the predicted the array index is wall or not
			if (m_mapBlocks[predictUp][arrX]->getAttr() == "wall") {
				// if yes, adjust it back to the normal position
				float adjustZ = static_cast<float>(gridZ) + 0.25f;
				m_player->SetPosition(Vector3(posX, 0.0f, adjustZ));
			}
		}
		else { m_player->SetPosition(Vector3(posX, 0.0f, static_cast<float>(m_mapSize) / 2 - 0.75f));}


		//if goes down
		if (posZ > -(static_cast<float>(m_mapSize) / 2 + 0.25f))
		{
			int predictDown = static_cast<int>(correctedPosZ - 0.25f);
			//de-double zero (super dirty way, really need someone how to use a proper way to get the colision mechanism)
			if ((correctedPosZ - 0.25f) > 0) { predictDown += 11; }
			else { predictDown += 10; }
			//trace the predicted array index is wall or not
			if (m_mapBlocks[predictDown][arrX]->getAttr() == "wall") {
				// if yes, adjust it back to the normal position
				float adjustZ = static_cast<float>(gridZ) - 0.25f;
				m_player->SetPosition(Vector3(posX, 0.0f, adjustZ));
			}
		}
		else { m_player->SetPosition(Vector3(posX, 0.0f, -(static_cast<float>(m_mapSize) / 2 + 0.25f))); }


		//if goes left
		if (posX > -(static_cast<float>(m_mapSize) / 2 + 0.25f))
		{
			int predictLeft = static_cast<int>(correctedPosX - 0.25f);
			//de-double zero (super dirty way, really need someone how to use a proper way to get the colision mechanism)
			if ((correctedPosX - 0.25f) > 0) { predictLeft += 11; }
			else { predictLeft += 10; }
			//trace the predicted array index is wall or not
			if (m_mapBlocks[arrZ][predictLeft]->getAttr() == "wall") {
				// if yes, adjust it back to the normal position
				float adjustX = static_cast<float>(gridX) - 0.25f;
				m_player->SetPosition(Vector3(adjustX, 0.0f, posZ));
			}
		}
		else { m_player->SetPosition(Vector3(-(static_cast<float>(m_mapSize) / 2 + 0.25f), 0.0f, posZ)); }


		//if goes right
		if (posX < (static_cast<float>(m_mapSize) / 2 - 0.75f))
		{
			int predictRight = static_cast<int>(correctedPosX + 0.25f);
			//de-double zero (super dirty way, really need someone how to use a proper way to get the colision mechanism)
			if ((correctedPosX + 0.25f) > 0) { predictRight += 11; }
			else { predictRight += 10; }
			//trace the predicted array index is wall or not
			if (m_mapBlocks[arrZ][predictRight]->getAttr() == "wall") {
				// if yes, adjust it back to the normal position
				float adjustX = static_cast<float>(gridX) + 0.25f;
				m_player->SetPosition(Vector3(adjustX, 0.0f, posZ));
			}
		}
		else { m_player->SetPosition(Vector3((static_cast<float>(m_mapSize) / 2 -0.75f), 0.0f, posZ)); }


	}

	//cout << gridX << " " << gridZ << "\n";




	if (m_mapBlocks[arrZ][arrX]->getAttr() != "floor") {
		//check the current position of player is what what kind of stuff in the array
		//if position is an enemy: enter to battle
		// if player is defeated: shutdown the operation. 
		// if enemy is defeated: delete the enemy object and change to null object
		//and then deduct the counter, if counter is 0, the game congratz the player and shutdown
		//how to shutdown: m_gameSwitch=false

		//notify player
		if (m_mapBlocks[arrZ][arrX]->getAttr() == "enemy") {
			//clear the arrow key buffer
			m_input->clearCurrentKey();
			MessageBox(NULL,"You've found an enemy!\nIt's time to battle!!!\nClick 'OK' to continue...", "Enemy found!", MB_OK);
			if (Battle(m_player, m_mapBlocks[arrZ][arrX])) {
				Sleep(500);
				cout << "You won the duel!\n";
				Sleep(500);
				system("pause");
				//and then change the attribute into floor with changing the texture (although it's dirty, it works)
				m_mapBlocks[arrZ][arrX]->SetTexture(m_textureManager->GetTexture("Assets/Textures/floor.jpg"));
				m_mapBlocks[arrZ][arrX]->SetAttr("floor");
				GameObject::m_enemyNumbers--;
				//update the prompt screen
				if (GameObject:: m_enemyNumbers == 0) {
					//inform the player
					Sleep(500);
					cout << "Congratulations! You've defeated all monsters!\n";
					Sleep(500);
					system("pause");
					MessageBox(NULL, "Game Clear!\nClick 'OK' to close the application...", "", MB_OK);
					//stop the game
					m_gameSwitch = false;
				}
				system("cls");
				cout << m_player->displayStatus(true);
				//if enemy number is zero
				//congrate the player and then stop the game. 
			}
			else {
				//inform the player
				Sleep(500);
				cout << "Unfortunately, you've defeated by "<< m_mapBlocks[arrZ][arrX]->getName()<<"!\n";
				Sleep(500);
				system("pause");
				MessageBox(NULL, "Game Over!\nClick 'OK' to close the application...", "", MB_OK);
				//stop the game
				m_gameSwitch = false;
			}
		}
		//if position is an item: acquire the item status and then delete itself and change to null object 
		else if (m_mapBlocks[arrZ][arrX]->getAttr() == "item") {
			//cancel the arrow key buffer
			m_input->clearCurrentKey();
			//gain the value of the item
			m_player->gainItem(m_mapBlocks[arrZ][arrX]);
			//notice player
			stringstream itemMsg;
			itemMsg << "you got a " << m_mapBlocks[arrZ][arrX]->getName()<<"!\n";
			itemMsg << "You gain: " << m_mapBlocks[arrZ][arrX]->getAtk() << "Attack, "; 
			itemMsg << m_mapBlocks[arrZ][arrX]->getDef() << "Defence, ";
			itemMsg << m_mapBlocks[arrZ][arrX]->getSpeed() << "Speed, ";
			itemMsg << m_mapBlocks[arrZ][arrX]->getHP() << "HP\n";
			itemMsg << "Click 'OK' to continue...";
			MessageBox(NULL, itemMsg.str().c_str() , "Item found!",MB_OK);
			//and then change the attribute into floor with changing the texture (although it's dirty, it works)
			m_mapBlocks[arrZ][arrX]->SetTexture(m_textureManager->GetTexture("Assets/Textures/floor.jpg"));
			m_mapBlocks[arrZ][arrX]->SetAttr("floor");
			//update the prompt screen
			system("cls");
			cout << m_player->displayStatus(true);
		}
		//if position is a wrap zone: randomly choose a place to and take it to the that coordination
		else if (m_mapBlocks[arrZ][arrX]->getAttr() == "warp") {
			//check the location isn't the same array then the current array
			float lastWarpX = GameObject::m_lastWarp.x;
			float lastWarpZ = GameObject::m_lastWarp.z;
			int arrWarpX = static_cast<int>(GameObject::m_lastWarp.x - 0.5f)+10;
			int arrWarpZ = static_cast<int>(GameObject::m_lastWarp.z - 0.5f)+10;
			if (lastWarpX - 0.5f > 0) { arrWarpX += 1; }
			if (lastWarpZ - 0.5f > 0) { arrWarpZ += 1; }
			if ((arrWarpX != arrX) || (arrWarpZ != arrZ)) {
				m_player->warpLocation();
			}
		}
	}


	//if it's a null object, just keep update it.
}


void Game::Update(float timestep)
{
	m_input->BeginUpdate();
	
	//update all gameobjects (only the non-player objects)
	for (unsigned int i = 1; i < m_gameObjects.size();i++) {
		m_gameObjects[i]->Update(timestep);
	}
	//update the player object
	/*because player need to check the surroundings has touched the wall(s) or not, so it will do the seperate action.*/
	m_player->Update(timestep);


	//and then check the position has what kind of stuff around player
	checkPos();

	//check again player's HP, because it might have some HP changes when walking on the map.
	if (m_player->getHP() <= 0) {
		MessageBox(NULL, "Game Over!\nClick 'OK' to close the application...", "", MB_OK);
		m_gameSwitch = false;
	}

	//and then move the HP bar or change its length for showing the health
	m_HPBar->SetPosition(m_player->GetPosition());
	m_HPBar->changeHPBar(m_player->getHP());

	//and the camera
	//add a update variation to adapting the position between camera and player object
	//follow the player object
	m_currentCam->SetPosition(Vector3(m_player->GetPosition().x, 7.5f, m_player->GetPosition().z - 5.0f));
	//and look at the player object
	m_currentCam->SetLookAt(m_player->GetPosition());

	m_currentCam->Update(timestep);


	m_input->EndUpdate();

}

void Game::Render()
{
	m_renderer->BeginScene(0.2f, 0.2f, 0.2f, 1.0f);

	// TODO render all gameobjects
	for (unsigned int i = 0; i < m_gameObjects.size();i++) {
		m_gameObjects[i]->Render(m_renderer,m_currentCam);
	}

	m_renderer->EndScene();
}

void Game::Shutdown()
{
	// TODO delete all gameobjects

	if (m_currentCam)
	{
		delete m_currentCam;
		m_currentCam = NULL;
	}

	if (m_unlitVertexColouredShader)
	{
		m_unlitVertexColouredShader->Release();
		delete m_unlitVertexColouredShader;
		m_unlitVertexColouredShader = NULL;
	}

	if (m_meshManager)
	{
		m_meshManager->Release();
		delete m_meshManager;
		m_meshManager = NULL;
	}

	if (m_textureManager)
	{
		m_textureManager->Release();
		delete m_textureManager;
		m_textureManager = NULL;
	}

	for (int i = 0; i != m_gameObjects.size(); i++) {
		delete m_gameObjects[i];
		m_gameObjects[i] = NULL;
	}
	
	for (int i = 0; i < m_mapSize; i++) {
		for (int j = 0; j < m_mapSize; j++) {
			if(m_mapBlocks[i][j]){ m_mapBlocks[i][j] = NULL; }
		}
	}
	
	if (m_player) { m_player = NULL; }

	if (m_HPBar) { m_HPBar = NULL; }
}

bool Game::isGameOver()
{
	return !m_gameSwitch;
}

bool Game::Battle(GameObject * player, GameObject * enemy)
{
	//initialize the counters, input value, array for managing rotation, some calculating var.
	int battleCounter = 0;
	GameObject* battleRotate[2];
	int battleCommand[2] = { 0,0 };
	bool battleLoop = true; //the boolean for breaking the loop
	int inInt;
	int attackerPt;
	int attackerSpeed;
	int defenderPt;
	int defenderSpeed;

	//check player and the enemy's speed to determine who goes first
	if (player->getSpeed() >= enemy->getSpeed()) {
		battleRotate[0] = player;
		battleRotate[1] = enemy;
	}
	else {
		battleRotate[0] = enemy;
		battleRotate[1] = player;  
	}

	//start loop till one of it has no more HP
	while (battleLoop) {
		//set up values
		battleCounter = battleCounter % 2;
		attackerPt = battleRotate[battleCounter]->getAtk();
		defenderPt = battleRotate[(battleCounter + 1) % 2]->getDef();
		attackerSpeed = battleRotate[battleCounter]->getSpeed();
		defenderSpeed = battleRotate[(battleCounter + 1) % 2]->getSpeed() / 4;
		srand(time(NULL));

		//clear screen and display the current status of player and enemy
		system("cls");
		cout << enemy->displayStatus() << "\n\n\t\t\tVS\n\n" << player->displayStatus() << "\nAuto Battle:please wait until someone is defeated\n";
		Sleep(500);
		cout << "it's " << battleRotate[battleCounter]->getName() << "'s turn!\n";
		//if the index is a player, ask the player for the command
		//no one can tell me how to do a non-real time read line changed to random.
		if (battleRotate[battleCounter]->getAttr() == "player") {
			//object can attack(1), random attack - random defence
			//player can also do defence(2), defence doubled in 1 turn
			//or standby(3), speed doubled in 1 turn
			//ask player for command
			// "what would you like to do?\n1:attack(attack to enemy)\n2:defence(not attack but boost your defence 150% in 1 turn)\n3:standby(not attack but boost your evade speed 150% in 1 turn)\nPlease choose your choice by entering the number then press enter/return: 
			//cout << "256";
			//cin >> inInt;
			//cout << inInt;
			//while (
			//	cout <<"123" &&
			//	(!(cin>>inInt) || (inInt < 1) || (inInt > 3))) {
			//	cin.clear();
			//	cin.ignore(numeric_limits<streamsize>::max(),'\n');
			//	cout << "\nThe input is invalid, please try again and make sure those number are around 1-3.\n\n";
			//	system("pause");
			//}
			/*			//prepare for getting the value
			int inInt=0;
			m_input->clearCurrentKey();
			m_input->inputStringReady = false;
			m_input->clearInString();
			//ask player for the value
			cout << "what would you like to do?\n1:attack(attack to enemy)\n2:defence(not attack but boost your defence 150% in 1 turn)\n3:standby(not attack but boost your evade speed 150% in 1 turn)\nPlease choose your choice by entering the number then press enter/return: ";
			//don't stop infinite loop till it got a valid input
			while (true) {
				m_input->BeginUpdate();
				//if the string is ready(by typing enter), start check the string is valid or not
				if (m_input->inputStringReady) {
					//display the input value
					cout << m_input->getInString();
					//check it
					if (m_input->getInString() == "1" || m_input->getInString() == "2" || m_input->getInString() == "3") {
						break;
					}
					//if not the result, inform player then clear the whole input and try again.
					else {
						cout << "\nThe input is invalid, please try again and make sure type number are around 1-3 and press enter.\n\n";
						m_input->inputStringReady = false;
						m_input->clearInString();
						cout << "what would you like to do?\n1:attack(attack to enemy)\n2:defence(not attack but boost your defence 150% in 1 turn)\n3:standby(not attack but boost your evade speed 150% in 1 turn)\nPlease choose your choice by entering the number then press enter/return: ";
					}
				}
			}
			inInt = stoi(m_input->getInString());
 */
			inInt = rand() % 3 + 1;
		}
		//if it is not a player, just random choose the command
		else {

			inInt = rand() % 3 + 1;
		}
		Sleep(500);

		//save the command for next turn
		battleCommand[battleCounter] = inInt;

		//if the candidate attacks
		if (inInt == 1) {
			//display the action
			cout << battleRotate[battleCounter]->getName() << " attacks " << battleRotate[(battleCounter + 1) % 2]->getName() << "!\n";
			Sleep(500);
			//then check the opponent's action to determine the damage calculation

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
			if (rand() % attackerSpeed > rand() % defenderSpeed) {
				//start damage calculation
				int damage = rand() % attackerPt - rand() % defenderPt;
				if (damage < 0) { damage = 0; }
				else{
					battleRotate[(battleCounter + 1) % 2]->changeHP(-damage);
				}
				cout << battleRotate[(battleCounter + 1) % 2]->getName() << " takes " << damage << " damage!\n";
			}
			//can evade
			else {
				cout << battleRotate[(battleCounter + 1) % 2]->getName() << " evades the attack!\n";
			}



		}
		//defence
		else if (inInt == 2) {
			cout << battleRotate[battleCounter]->getName() << " chooses to defend himself!\n";
			Sleep(500);
		}
		//standby
		else if (inInt == 3) {
			cout << battleRotate[battleCounter]->getName() << " chooses to aware himself!\n";
			Sleep(500);
		}

		//check the HP for the opponent, if it's zero, halt the loop.
		if (battleRotate[(battleCounter + 1) % 2]->getHP() <= 0) {
			battleLoop = false;
		}
		battleCounter++;
		Sleep(1000);
	}

	//check who is defeated
	if (enemy->getHP() <= 0) {
		return true;
	}
	else if (player->getHP() <= 0) {
		return false;
	}

	//return true;
}

