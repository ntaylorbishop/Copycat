#pragma once

#include <map>
#include <vector>
#include <string>
#include <functional>
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Console/Command.hpp"

class BitmapFont;
class Glyph;
class Font;
class Cursor;

typedef void(*console_command_cb)(Command&);

enum eConsoleColor {
	CONSOLE_WARNING = 0,
	CONSOLE_VERIFY,
	CONSOLE_DEBUG,
	CONSOLE_NORMAL
};

const uint MAX_PREV_INPUTS = 100;


class Console {
public:
	//INIT
	static void Initialize(unsigned char keybind);
	static void Shutdown();

	//UPDATE RENDER
	static void Update(float deltaSeconds);
	static void Render();

	//INTERFACE
	static void ConsumeCharacterInput(KEYCODE keyPress);
	static void RegisterCommand(String name, String tooltip, console_command_cb);
	static void RunCommand(String commandLine);

	static void PrintOutput(String output, eConsoleColor cColor);
	static void PrintDebugOutput(String output, eConsoleColor cColor);

	static void ClearConsole();
	static void GetWidthOfStringOnConsole(const String& potentialOutput);
	static bool IsConsoleOpen()						{ return s_theBeirusConsole->IsOpen();			}
	static std::vector<String>* GetCommandList()	{ return &s_theBeirusConsole->m_commandList;	}

	Font* m_newConsoleFont = nullptr;

private:
	//STRUCTORS
	Console(KEYCODE keybind);
	~Console();

	//INTERNAL UPDATE
	void UpdateConsole(float deltaSeconds);
	void UpdatePrevInputIndices();
	void ChangeCurrSelectedLine(int whatToAddToIdx);

	//INTERNAL RENDER
	void RenderConsole() const;
	void RenderCommandLine() const;
	void RenderCurrentInput() const;
	void RenderPreviousInputs() const;

	//UTIL
	void Toggle();
	void ShowConsole();
	void HideConsole();
	bool IsOpen();
	void RegisterCharacterInput(unsigned char asKey);

	
	std::vector<std::pair<bool, std::pair<String, RGBA>>>	m_prevInputs;
	std::vector<unsigned char>								m_currentInput;
	std::vector<unsigned char>								m_currentInputHidden;
	std::vector<String>										m_prevCommands;
	std::vector<String>										m_commandList;
	bool		m_isActive					= false;
	Cursor*		m_cursor					= nullptr;
	BitmapFont* m_consoleFont				= nullptr;
	float		m_consoleScale				= 0.f;
	Glyph*		m_currGlyphSelected			= nullptr;
	int			m_idxOfCurrGlyphSelected	= 0;
	int			m_numWindowLines			= 0;
	int			m_prevInputStartIdx			= 0;
	int			m_prevInputEndIdx			= 0;
	int			m_idxOfLastLineShown		= 0;
	int			m_currentLineSelected		= 0;
	float		m_age						= 0.f;
	KEYCODE		m_keybind					= 0;
	
	static std::map<size_t, console_command_cb, std::less<size_t>, UntrackedAllocator<std::pair<size_t, console_command_cb>>> s_commands;
	static Console* s_theBeirusConsole;

	friend class BeirusEngine;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TYPEDEFS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
typedef std::pair<String, String> StringPair;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DEFAULT COMMANDS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ClearConsoleCommand(Command& args);
void QuitApplication(Command& args);
void Help(Command& args);