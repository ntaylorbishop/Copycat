#include "Engine/Console/Console.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Console/Cursor.hpp"
#include "Engine/Renderer/Fonts/Font.hpp"
#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Engine/Renderer/Mesh/MeshRenderer.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Renderer/Material/Material.hpp"

STATIC Console* Console::s_theBeirusConsole = nullptr;
STATIC std::map<size_t, console_command_cb, std::less<size_t>, UntrackedAllocator<std::pair<size_t, console_command_cb>>> Console::s_commands;

//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC INIT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Console::Initialize(unsigned char keybind) {

	ASSERT_OR_DIE(s_theBeirusConsole == nullptr, "ERROR: Console already initialized.");
	s_theBeirusConsole = new Console(keybind);

	RegisterCommand("clear", "Clears the console window", ClearConsoleCommand);
	RegisterCommand("quit", "Quits the application", QuitApplication);
	RegisterCommand("help", "Show help", Help);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Console::Shutdown() {

	ASSERT_OR_DIE(s_theBeirusConsole != nullptr, "ERROR: Console not initialized.");
	delete s_theBeirusConsole;
	s_theBeirusConsole = nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Console::Update(float deltaSeconds) {

	if (InputSystem::GetKeyDownForConsole(s_theBeirusConsole->m_keybind)) {
		s_theBeirusConsole->Toggle();
	}

	if (s_theBeirusConsole->IsOpen()) {
		s_theBeirusConsole->UpdateConsole(deltaSeconds);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Console::Render() {

	if (s_theBeirusConsole->IsOpen()) {
		s_theBeirusConsole->RenderConsole();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC INTERFACE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void Console::ConsumeCharacterInput(KEYCODE keyPress) {

	if (s_theBeirusConsole->IsOpen())
		s_theBeirusConsole->RegisterCharacterInput(keyPress);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Console::RegisterCommand(String name, String tooltip, console_command_cb cb) {
	s_commands.insert(std::pair<size_t, console_command_cb>(std::hash<String>()(name), cb));

	String command = StringUtils::Stringf("%s: %s", name.c_str(), tooltip.c_str());
	if (name != "help")
		s_theBeirusConsole->m_commandList.push_back(command);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Console::RunCommand(String commandLine) {
	if (!commandLine.empty()) {
		Command com = Command(commandLine);

		console_command_cb out_command;
		std::map<size_t, console_command_cb>::iterator commandIterator = s_commands.find(std::hash<String>()(com.GetCommandName()));

		if (commandIterator != s_commands.end()) {
			out_command = commandIterator->second;
			out_command(com);
		}
		else {
			s_theBeirusConsole->PrintOutput("Invalid command. Type \"help\" for options", CONSOLE_WARNING);
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Console::PrintOutput(String output, eConsoleColor cColor) {
	RGBA textColor;
	if (cColor == CONSOLE_WARNING) {
		textColor = RGBA::RED;
	}
	else if (cColor == CONSOLE_NORMAL) {
		textColor = RGBA::WHITE;
	}
	else if (cColor == CONSOLE_VERIFY) {
		textColor = RGBA(0x00, 0xc6, 0xff, 0xff);
	}
	else if (cColor == CONSOLE_DEBUG) {
		textColor = RGBA::GREEN;
	}

	s_theBeirusConsole->m_prevInputs.push_back(std::pair<bool, std::pair<String, RGBA>>(true, std::pair<String, RGBA>(output, textColor)));
	TheLogger::LogPrintf(output.c_str(), "Console", LOG_DEFAULT);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Console::PrintDebugOutput(String output, eConsoleColor cColor) {
	RGBA textColor;
	if (cColor == CONSOLE_WARNING) {
		textColor = RGBA::RED;
	}
	else if (cColor == CONSOLE_NORMAL) {
		textColor = RGBA::WHITE;
	}
	else if (cColor == CONSOLE_VERIFY) {
		textColor = RGBA(0x00, 0xc6, 0xff, 0xff);
	}
	else if (cColor == CONSOLE_DEBUG) {
		textColor = RGBA::GREEN;
	}

	s_theBeirusConsole->m_prevInputs.push_back(std::pair<bool, std::pair<String, RGBA>>(true, std::pair<String, RGBA>(output, textColor)));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Console::ClearConsole() {

	s_theBeirusConsole->m_prevInputs.clear();
	s_theBeirusConsole->m_prevCommands.clear();
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Console::GetWidthOfStringOnConsole(const String& potentialOutput) {

	s_theBeirusConsole->m_newConsoleFont->GetWidthOfString(potentialOutput, 0.4f);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBERS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
Console::Console(KEYCODE keybind)
	: m_isActive(false)
	, m_cursor(nullptr)
	, m_consoleFont(nullptr)
	, m_currGlyphSelected(nullptr)
	, m_consoleScale(0.5f)
	, m_idxOfCurrGlyphSelected(-1)
	, m_numWindowLines(6)
	, m_idxOfLastLineShown(0)
	, m_prevInputStartIdx(0)
	, m_prevInputEndIdx(0)
	, m_currentLineSelected(0)
	, m_age(0.f)
	, m_keybind(keybind)
{
	m_cursor = new(MEMORY_GENERAL) Cursor(24.f, 3.f, 0.1f, Vector2(5.f, 3.f));
	m_consoleFont = BitmapFont::CreateOrGetFont("SquirrelFixedFont");
	m_newConsoleFont = Font::CreateOrGetFont("Tahoma");
}


//---------------------------------------------------------------------------------------------------------------------------
Console::~Console() {
	delete m_cursor;
	m_cursor = nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INTERNAL UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
void Console::UpdateConsole(float deltaSeconds) {
	m_cursor->Update(deltaSeconds);

	if (!m_isActive)
		return;

	m_age += deltaSeconds;

	if (m_prevCommands.size() >= MAX_PREV_INPUTS) {
		size_t numToErase = m_prevCommands.size() - MAX_PREV_INPUTS;
		m_prevCommands.erase(m_prevCommands.begin() + numToErase);
	}

	if (InputSystem::GetKeyDownForConsole(VK_RETURN)) {
		if (m_currentInput.size() > 0) {
			m_cursor->ResetCursorPosition();

			String currInput = "";
			for (unsigned int i = 0; i < m_currentInput.size(); i++) {
				char currChar = (char)m_currentInput[i];
				currInput.push_back(currChar);
			}

			m_prevInputs.push_back(std::pair<bool, std::pair<String, RGBA>>(true, std::pair<String, RGBA>(currInput, RGBA::WHITE)));
			m_prevCommands.push_back(currInput);
			TheLogger::LogPrintf(currInput.c_str(), "Console", LOG_DEFAULT);

			RunCommand(currInput);
			m_currentInput.clear();

			m_idxOfLastLineShown = 0;
			m_currentLineSelected = (int)m_prevCommands.size();
		}
		else {
			HideConsole();
		}
	}
	if (InputSystem::GetKeyDownForConsole(VK_ESCAPE)) {
		if (m_currentInput.size() == 0) {
			HideConsole();
		}
		else {
			m_currentInput.clear();
			m_cursor->ResetCursorPosition();
		}
	}

	if (InputSystem::GetMouseWheelDownForConsole()) {
		m_idxOfLastLineShown--;
	}
	else if (InputSystem::GetMouseWheelUpForConsole()) {
		m_idxOfLastLineShown++;
	}

	UpdatePrevInputIndices();

	if (InputSystem::GetKeyDownForConsole(VK_UP)) {
		ChangeCurrSelectedLine(-1);
	}
	else if (InputSystem::GetKeyDownForConsole(VK_DOWN)) {
		ChangeCurrSelectedLine(1);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void Console::UpdatePrevInputIndices() {
	if (m_idxOfLastLineShown < 0) {
		m_idxOfLastLineShown = 0;
	}
	else if (m_idxOfLastLineShown >(int)m_prevInputs.size() - m_numWindowLines) {
		m_idxOfLastLineShown = (int)m_prevInputs.size() - m_numWindowLines;
	}

	m_prevInputStartIdx = (int)m_prevInputs.size() - m_numWindowLines - m_idxOfLastLineShown;
	if (m_prevInputStartIdx < 0) {
		m_prevInputStartIdx = 0;
	}

	m_prevInputEndIdx = (int)m_prevInputs.size() - m_idxOfLastLineShown;
	if (m_prevInputEndIdx < 0) {
		m_prevInputEndIdx = 0;
		m_idxOfLastLineShown--;
	}
	else if (m_prevInputEndIdx >(int)m_prevInputs.size())
		m_prevInputEndIdx = (int)m_prevInputs.size();

	if ((int)m_prevInputs.size() >= m_numWindowLines && (m_prevInputEndIdx - m_prevInputStartIdx) < m_numWindowLines) {
		m_prevInputEndIdx = m_prevInputStartIdx + m_numWindowLines;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void Console::ChangeCurrSelectedLine(int whatToAddToIdx) {
	int lastPossibleLine = (int)m_prevCommands.size() - 1;

	if (m_currentLineSelected == lastPossibleLine) {
		m_currentInputHidden = m_currentInput;
	}

	m_currentLineSelected += whatToAddToIdx;

	//Clamp indices
	if (m_currentLineSelected < 0) {
		m_currentLineSelected = 0;
	}
	else if (m_currentLineSelected > lastPossibleLine) {
		m_currentLineSelected = lastPossibleLine;
	}
	else if (m_currentLineSelected == (int)m_prevCommands.size()) {
		m_currentInput = m_currentInputHidden;
	}

	//Set current input to one of the previous inputs
	m_currentInput.clear();
	for (unsigned int i = 0; i < m_prevCommands[m_currentLineSelected].size(); i++) {
		m_currentInput.push_back((unsigned char)m_prevCommands[m_currentLineSelected][i]);
	}

	m_cursor->ResetCursorPosition();
	m_cursor->MoveCursor(m_newConsoleFont->GetWidthOfString(m_prevCommands[m_currentLineSelected], m_consoleScale));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INTERNAL RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
void Console::RenderConsole() const {
	if (m_isActive) {
		//BeirusRenderer::DisableDepthTesting();

		RenderCommandLine();
		RenderCurrentInput();
		RenderPreviousInputs();
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void Console::RenderCommandLine() const {

	AABB2 quad = AABB2(Vector2(0.f, 0.f), Vector2(BeirusRenderer::GetScreenSize()->x, 30.f));
	BeirusRenderer::DrawAABB2(quad, RGBA(0.f, 0.f, 0.f, 0.5f));

	m_cursor->BlinkCursor();
}


//---------------------------------------------------------------------------------------------------------------------------
void Console::RenderCurrentInput() const {
	String currInput = "";
	for (unsigned int i = 0; i < m_currentInput.size(); i++) {
		char currChar = (char)m_currentInput[i];

		currInput.push_back(currChar);
	}

	m_newConsoleFont->DrawText2D(m_cursor->GetStartingPosition() + Vector2(3.f, 0.f), currInput, m_consoleScale, RGBA::WHITE);
}


//---------------------------------------------------------------------------------------------------------------------------
void Console::RenderPreviousInputs() const {

	const Vector2* screenSize = BeirusRenderer::GetScreenSize();

	AABB2 quad = AABB2(Vector2(10.f, 50.f), Vector2(screenSize->x - 10.f, 200.f));
	BeirusRenderer::DrawAABB2(quad, RGBA(0.f, 0.f, 0.f, 0.5f));

	if (m_prevInputs.empty())
		return;

	float startY = 200.f;
	float line = 1.f;
	for (int i = m_prevInputStartIdx; i < m_prevInputEndIdx; i++) {

		float prevInputScale = 0.4f;
		float dropDown = startY - m_newConsoleFont->GetLineHeight() * prevInputScale * line;
		m_newConsoleFont->DrawText2D(m_cursor->GetStartingPosition() + Vector2(13.f, dropDown), m_prevInputs[i].second.first, prevInputScale, m_prevInputs[i].second.second);
		line += 1.f;
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UTIL
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Console::Toggle() {
	if (!IsOpen()) {
		ShowConsole();
	}
	else {
		HideConsole();
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void Console::ShowConsole() {
	DebuggerPrintf("Console open\n");
	m_isActive = true;
	InputSystem::DisableInputToGame();
}


//---------------------------------------------------------------------------------------------------------------------------
void Console::HideConsole() {
	DebuggerPrintf("Console closed\n");
	m_isActive = false;
	InputSystem::EnableInputToGame();
	m_cursor->ResetCursorPosition();
	m_age = 0.f;
}


//---------------------------------------------------------------------------------------------------------------------------
bool Console::IsOpen() {
	return m_isActive;
}


//---------------------------------------------------------------------------------------------------------------------------
void Console::RegisterCharacterInput(unsigned char asKey) {
	if (asKey == VK_BACK) {
		if (m_currentInput.size() != 0) {
			m_cursor->MoveCursor(-m_newConsoleFont->GetXAdvanceOfGlyph(m_currentInput.back(), m_consoleScale));
			m_currentInput.pop_back();

			if (m_currentInput.size() > 0) {
				m_currGlyphSelected = m_newConsoleFont->GetGlyphForCharacter(m_currentInput.back());
			}
			else {
				m_currGlyphSelected = nullptr;
			}
		}
	}
	else {
		if (asKey != '\r' && asKey != '\n' && asKey != VK_ESCAPE) {
			m_currentInput.push_back(asKey);
			m_cursor->MoveCursor(m_newConsoleFont->GetXAdvanceOfGlyph(asKey, m_consoleScale));

			m_currGlyphSelected = m_newConsoleFont->GetGlyphForCharacter(m_currentInput.back());
		}
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//COMMANDS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ClearConsoleCommand(Command& args) {
	args;
	Console::ClearConsole();
}


//---------------------------------------------------------------------------------------------------------------------------
void QuitApplication(Command& args) {
	args;
	Console::PrintOutput("Quitting application...", CONSOLE_VERIFY);
	InputSystem::Quitting(true);
}


//---------------------------------------------------------------------------------------------------------------------------
void Help(Command& args) {
	
	UNREFERENCED_PARAMETER(args);

	std::vector<String>* commandList = Console::GetCommandList();

	for (unsigned int i = 0; i < commandList->size(); i++) {
		Console::PrintOutput(commandList->at(i), CONSOLE_VERIFY);
	}
}