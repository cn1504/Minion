#include "Settings.h"

PersistantMap Settings;

void PersistantMap::load(const string& filename)
{
	filepath = filename;
	ifstream file(filename);
	if (file.is_open()) {
		while (!file.eof()) {
			string key, value;
			file >> key;
			file >> value;

			if (!file.eof()) {
				map[key] = value;
			}
		}
		file.close();
	}
}

void PersistantMap::save()
{
	ofstream file(filepath.c_str(), ios_base::out);
	if (file.is_open()) {
		for (auto& pair : map) {
			file << pair.first << " " << pair.second << endl;
		}
		file.close();
	}
}

string& PersistantMap::operator[](const string& key) 
{
	return map[key];
}

const string& PersistantMap::assertValue(const string& key, const string& value) {
	if (map[key].length() == 0) {
		map[key] = value;

		ofstream file(filepath.c_str(), ios_base::app);
		if (file.is_open()) {
			file << key << " " << map[key] << endl;
			file.close();
		}
	}
	
	return map[key];
}