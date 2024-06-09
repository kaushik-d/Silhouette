#pragma once
#include <string>
#include <map>


struct optProf {
	bool required;
	bool valueRequired;
	std::string help;
};

class CmdLine
{
	//
	// This is a singlenton class
	//
public:
	static CmdLine& Instance()
	{
		static CmdLine instance;
		return instance;
	}

	bool Initialize(int argc, char** argv);

	bool isDefined(const std::string& opt) const;
	std::string getString(const std::string& opt) const;
	int getInt(const std::string& opt) const;
	double getDouble(const std::string& opt) const;

	void displayHelp();

private:
	CmdLine() {}

public:
	CmdLine(CmdLine const&) = delete;
	void operator=(CmdLine const&) = delete;

	std::map<std::string, std::string> m_opts;
	std::map<std::string, optProf> m_validOptions;

};

