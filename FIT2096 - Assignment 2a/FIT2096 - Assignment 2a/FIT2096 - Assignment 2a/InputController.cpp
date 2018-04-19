/*	FIT2096 - Example Code
*	InputController.cpp
*	Created by Elliott Wilson - 2015 - Monash University
*	Implementation of InputController.h
*/

#include "InputController.h"

InputController::InputController(HWND windowHandle)
{
	m_windowHandle = windowHandle;						

	memset(m_PrevKeys, 0, sizeof(m_PrevKeys));
	memset(m_blankKeys, 0, sizeof(m_PrevKeys));

	inputInteger = NULL;
	inputStringReady = false;

}

void InputController::SetKeyDown(UINT keyCode)
{
	m_CurrentKeys[keyCode] = true;
}

void InputController::SetKeyUp(UINT keyCode)
{
	m_CurrentKeys[keyCode] = false;
}

bool InputController::GetKeyDown(UINT keyCode)
{
	return !m_PrevKeys[keyCode] && m_CurrentKeys[keyCode];
}

bool InputController::GetKeyHold(UINT keyCode)
{
	return m_CurrentKeys[keyCode];
}

bool InputController::GetKeyUp(UINT keyCode)
{
	return m_PrevKeys[keyCode] && !m_CurrentKeys[keyCode];
}

void InputController::BeginUpdate()
{
	//We'll use this in future weeks when we capture mouse input
}

void InputController::EndUpdate()
{
	//When we finish an update we copy the values of the current arrays into the last arrays
	memcpy(m_PrevKeys, m_CurrentKeys, sizeof(m_CurrentKeys));
}

void InputController::clearCurrentKey()
{
	memcpy(m_CurrentKeys, m_blankKeys, sizeof(m_CurrentKeys));

}

void InputController::clearInString()
{
	inString = "";
}

void InputController::concatInString(char inChar)
{
	inString.push_back(inChar);
}

string InputController::getInString()
{
	return inString;
}
