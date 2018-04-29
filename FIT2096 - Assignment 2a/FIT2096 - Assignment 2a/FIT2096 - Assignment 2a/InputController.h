/*	FIT2096 - Example Code
*	InputController.h
*	Created by Elliott Wilson - 2015 - Monash University
*	This class keeps track of the current state of the keyboard and mouse
*	It receives data from the Window Proc about the current keys and mouse buttons
*	and uses the Windows API for information about the Mouse position
*/

#ifndef INPUTCONTROLLER_H_
#define INPUTCONTROLLER_H_

#include <Windows.h>
#include <string>

#define LEFT_MOUSE 0						//A value to represent the Left Mouse Button
#define RIGHT_MOUSE 1						//A value to represent the Right Mouse Button
#define MIDDLE_MOUSE 2						//A value to represent the Middle Mouse Button
#define NUMBER_OF_KEYS 256					//The number of entries in the keys arrays

using namespace std;

class InputController
{
private:
	HWND m_windowHandle;

	bool m_CurrentKeys[NUMBER_OF_KEYS];		//An array of key values for the current frame
	bool m_PrevKeys[NUMBER_OF_KEYS];		//An array of key values for the previous frame
	bool m_blankKeys[NUMBER_OF_KEYS];	//for clearing the current values of the key

	bool m_currentMouse[3];					//An array of mouse button values for the current frame
	bool m_prevMouse[3];					//An array of mouse button values for the previous frame
	int m_mouseX;							//The current X value of the mouse cursor
	int m_mouseY;							//The current Y value of the mouse cursor
	int m_mouseDeltaX;
	int m_mouseDeltaY;

	void InitMouse();


	int inputInteger;
	string inString;
public:
	InputController(HWND windowHandle);		//Constructor

	void SetKeyDown(UINT keyCode);			//These set methods are used to update the correct values in
	void SetKeyUp(UINT keyCode);			//the current keys/mouse button arrays
	void SetMouseDown(int mouseButton);
	void SetMouseUp(int mouseButton);
	void SetMouseDeltaX(long deltaX);
	void SetMouseDeltaY(long deltaY);

	bool GetKeyDown(UINT keyCode);			//Returns true the frame the key became down
	bool GetKeyHold(UINT keyCode);			//Returns true while a key is held down
	bool GetKeyUp(UINT keyCode);			//Returns true if the selected key was down last frame and up this frame

	bool GetMouseDown(int mouseButton);		//Returns true if the selected mouse button is down
	bool GetMouseUp(int mouseButton);		//Returns true if the selected mouse button was down last frame and up this frame
	int GetMouseX();						//Returns the current mouse X
	int GetMouseY();						//Returns the current mouse Y
	int GetMouseDeltaX();
	int GetMouseDeltaY();


	void BeginUpdate();						//Begins updating the input, this reads the current value of the mouse cursor
	void EndUpdate();						//Ends updating the input, this copys the values of current key/mouse into previous keys/mouse

	void clearCurrentKey();
	void clearInString();
	
	void concatInString(char inChar);
	string getInString();

	bool inputStringReady;
};

#endif