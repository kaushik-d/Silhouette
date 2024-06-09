#include <iostream>
#include "CmdLine.h"
//#include "Exceptions.h"

bool CmdLine::Initialize(int argc, char** argv) {

	if (argc < 0 || argv == nullptr) {
		return false;
	}
		 
	m_validOptions = {
	// Option, { required?, value required?, Help }
		{"h", {false, false, "Display help."} },
		{"s", {true,  true, "stl file."} },
		{"nx", {true,  true, "shadow plane normal x."} },
		{"ny", {true,  true, "shadow plane normal y."} },
		{"nz", {true,  true, "shadow plane normal z."} },
		{"ox", {true,  true, "shadow plane origin x."} },
		{"oy", {true,  true, "shadow plane origin y."} },
		{"oz", {true,  true, "shadow plane origin z."} }
	};

	bool CmlLineError(false);

	for (int i = 1; i < argc; i++) {
		
		if (argv[i] == nullptr) {
			return false;
		}

		if (argv[i][0] == '-' && argv[i][1] == '-') {

			const char* sOpt = &(argv[i][2]);

			if (sOpt == "" || m_validOptions.find(sOpt) == m_validOptions.end()) {
				std::cout << "\nInvalid command line option " << sOpt << std::endl;
				CmlLineError = true;
			}

			if (i != argc - 1 && argv[i + 1][0] != '--') {

				const char* sVal = argv[i + 1];
				m_opts.insert(std::make_pair(sOpt, sVal));
				i++;
			}
			else {

				std::string val("");
				m_opts.insert(std::make_pair(sOpt, val));
			}
		}
	}

	// Make sure all required options are present on cmdline

	for (auto iter : m_validOptions) {
		if (iter.second.required) {
			if (!isDefined(iter.first)) {
				std::cout << "\nCommand line option --" << iter.first << " is required." << std::endl;
				CmlLineError = true;
			}
		}
		if (isDefined(iter.first)) {
			if (iter.second.valueRequired) {
				if (getString(iter.first).empty()) {
					std::cout << "\nCommand line option --" << iter.first << " must have a value." << std::endl;
					CmlLineError = true;
				}
			}
		}
	}

	if (CmlLineError) {
		displayHelp();
		return false;
	}

	return true;
}

bool CmdLine::isDefined(const std::string& opt) const
{
	return m_opts.find(opt) != m_opts.end();
}

std::string CmdLine::getString(const std::string& opt) const
{
	auto iter = m_opts.find(opt);
	if (iter == m_opts.end()) {
		throw std::runtime_error("Invalid command line option " + opt);
	}
	else {
		return iter->second.c_str();
	}
}

int CmdLine::getInt(const std::string& opt) const
{
	auto iter = m_opts.find(opt);
	if (iter == m_opts.end()) {
		return 0;
	}
	else {
		return std::atoi(iter->second.c_str());
	}
}

double CmdLine::getDouble(const std::string& opt) const
{
	auto iter = m_opts.find(opt);
	if (iter == m_opts.end()) {
		return 0;
	}
	else {
		return std::atof(iter->second.c_str());
	}
}

void CmdLine::displayHelp()
{
	std::cout << "\n CLI file reader." << std::endl;
	for (auto& opts : m_validOptions) {
		std::cout <<"\t--" << opts.first << " : " << (opts.second.required ? " (required) " : " (optional) ") << " : " << opts.second.help << std::endl;
	}
	std::cout << std::endl;
}
