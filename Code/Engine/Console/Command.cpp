#include "Engine/Console/Command.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/Console/Console.hpp"

//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
Command::Command(std::string commandString) 
	: m_currIdx(1)
{
	StringUtils::TokenizeBuffer(commandString, ' ', m_stringTokens);

	if (m_stringTokens.size() == 1) {
		m_currIdx = -1;
	}
}
Command::Command(std::vector<std::string> commandString)
	: m_currIdx(1)
	, m_stringTokens(commandString)
{ }
Command::~Command() { }

//INTERFACE
//---------------------------------------------------------------------------------------------------------------------------
std::string Command::GetCommandName() const {
	return m_stringTokens[0];
}
bool Command::GetNextInt(int& out) {
	if (m_currIdx == -1) {
		return false;
	}
	else if (m_currIdx == (int)m_stringTokens.size()) {
		return false;
	}
	else {
		try {
			out = stoi(m_stringTokens[m_currIdx]);
		}
		catch (std::invalid_argument& e) {
			UNUSED(e);
			Console::PrintOutput("ERROR: Could not parse integer. Please use the console command correctly. Type help for help and a list of console commands.", CONSOLE_VERIFY);
		}
		m_currIdx++;
		return true;
	}
}
bool Command::GetNextString(std::string& out) {
	if (m_currIdx == -1) {
		return false;
	}
	else if (m_currIdx == (int)m_stringTokens.size()) {
		return false;
	}
	else {
		out = m_stringTokens[m_currIdx];
		m_currIdx++;
		return true;
	}
}
bool Command::GetNextColor(RGBA& out) {
	if (m_currIdx == -1) {
		return false;
	}
	else if (m_currIdx == (int)m_stringTokens.size()) {
		return false;
	}
	else {
		std::string arg = m_stringTokens[m_currIdx];

		if (arg == "red") {
			out = RGBA::RED;
		}
		else if (arg == "green") {
			out = RGBA::GREEN;
		}
		else if (arg == "blue") {
			out = RGBA::BLUE;
		}
		else if (arg == "yellow") {
			out = RGBA::YELLOW;
		}
		else if (arg == "cyan") {
			out = RGBA::CYAN;
		}
		else if (arg == "magenta") {
			out = RGBA::MAGENTA;
		}
		else if (arg == "white") {
			out = RGBA::WHITE;
		}

		m_currIdx++;
		return true;
	}
}
bool Command::GetNextFloat(float& out) {
	if (m_currIdx == -1) {
		return false;
	}
	else if (m_currIdx == (int)m_stringTokens.size()) {
		return false;
	}
	else {
		out = stof(m_stringTokens[m_currIdx]);
		m_currIdx++;
		return true;
	}
}