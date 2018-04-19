#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Direct3D.h"
#include "Mesh.h"
#include "InputController.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>


using namespace std;

class GameObject
{
private:
	//game stuff
	/*
	m_name: name of the object
	m_attr: attribute of the object
	m_isAppeared: a boolean to determine the object is available or not(aka, visability)<==maybe delete and covered by floor as well
	m_position: position of the object(under the 3D stuff part)
	*/
	string m_name, m_attr;
	//bool isAppeared;
	int m_atk, m_def, m_speed, m_HP;
	int const M_MAX_PARA = 99;


	//3D positioning/rotating/scaling/rendering stuff
	Vector3 m_position;//positions for the object
	float m_rotX, m_rotY, m_rotZ;//rotation of the object(actually isn't do matter in this game...?)
	float m_scaleX, m_scaleY, m_scaleZ;//scale of the object (but the assignment doesn't matter at all
	float m_moveSpeed;//moving speed
	float m_rotateSpeed;//rotation speed
	Matrix m_world;//???
	Mesh* m_mesh;//polygon mesh of the object
	Texture* m_texture;//texture of the mesh
	Shader* m_shader;//shader of the mesh
	InputController* m_input;//user input

	

public:

	//static variable/const
	/*
		m_enemyNumbers : check the number of the enemy(if all enemy is unavailiable, the game stops)
		m_posBoarderX, m_posBoarderY : the constant value of the game world can go for
		m_wrapList: a list of vector3 for wraping
		m_lastWrap: the status of last wrapped location
	*/
	int static m_enemyNumbers;
	float static mapSize;
	vector<Vector3> static m_warpList;
	Vector3 static m_lastWarp;

	//constructor and destructor
	GameObject(Mesh* mesh, Shader* shader, InputController* input, Vector3 position);
	//constructor will be used for making the mesh object
	GameObject(Mesh* mesh, Shader* shader, InputController* input, Vector3 position, Texture* texture);
	//this version will be used for making the mesh for player, enemy and item
	GameObject(Mesh* mesh, Shader* shader, InputController* input, Vector3 position, Texture* texture,
		string name, string attr, int atk, int def, int speed, int HP);
	//this version will be used for making the mesh for the wall, floor, warp zone and HP bar.
	GameObject(Mesh * mesh, Shader * shader, InputController * input, Vector3 position, Texture * texture, string attr);

	//this line will be used for making the warp mesh object
		//direction: base on NULL, add name, attr, and target location
	~GameObject();

	//3D render and updating status
	void Update(float timestep);
	void Render(Direct3D* renderer, Camera* cam);

	// Accessors
	Vector3 GetPosition() { return m_position; }
	float GetXRotation() { return m_rotX; }
	float GetYRotation() { return m_rotY; }
	float GetZRotation() { return m_rotZ; }
	float GetXScale() { return m_scaleX; }
	float GetYScale() { return m_scaleY; }
	float GetZScale() { return m_scaleZ; }
	Mesh* GetMesh() { return m_mesh; }
	Texture* GetTexture() { return m_texture; }
	Shader* GetShader() { return m_shader; }

	string getAttr() { return m_attr; }
	string getName() { return m_name; }
	int getAtk() { return m_atk; }
	int getHP() { return m_HP; }
	int getDef() { return m_def; }
	int getSpeed() { return m_speed; }

	// Mutators
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetXRotation(float xRot) { m_rotX = xRot; }
	void SetYRotation(float yRot) { m_rotY = yRot; }
	void SetZRotation(float zRot) { m_rotZ = zRot; }
	void SetXScale(float xScale) { m_scaleX = xScale; }
	void SetYScale(float yScale) { m_scaleY = yScale; }
	void SetZScale(float zScale) { m_scaleZ = zScale; }
	void SetUniformScale(float scale) { m_scaleX = m_scaleY = m_scaleZ = scale; }
	void SetMesh(Mesh* mesh) { m_mesh = mesh; }
	void SetTexture(Texture* texture) { m_texture = texture; }
	void SetShader(Shader* shader) { m_shader = shader; }
	//change the attribute
	void SetAttr(string attr) { m_attr = attr; }

	//other functions, mainly for enemy/player.
		//output status in string
	string displayStatus();
	string displayStatus(bool withInstruct);
		//wrap to the random location
	void warpLocation();
		//gain the parameters of the item to the player
	void gainItem(GameObject* inObject);
		//check player/enemy is defeated or not
	bool isDefeated();
		//change the HP
	void changeHP(int inHP);
		//change the length of the HP bar
	void changeHPBar(int inHP);


};

#endif

