#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include "Constants.hpp"
using namespace std;

class PersistantMap
{
private:
	string filepath;
	unordered_map<string, string> map;

public:
	void load(const string& filename);
	void save();

	string& operator[](const string& key);
	const string& assertValue(const string& key, const string& value);
};

extern PersistantMap Settings;

