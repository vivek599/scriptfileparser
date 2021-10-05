#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Vec3
{
	float x, y, z;
};

struct ModelData
{
	string tagName;
	string filePath;
	string texturePath;
	string normalPath;
	bool bMovable;
	Vec3 Position;
	Vec3 Rotation;
	Vec3 Scale;
};

vector<ModelData> g_modelData;

int main()
{
	ifstream infile("modeldata3.txt", ios::in);
	if (!infile.good())
	{
		cout << "Error opening file!" << endl;
	}

	while (!infile.eof())
	{
		string line;
		line.resize(1024);
		infile.getline((char*)line.data(), line.size(), '{');
		line.erase(remove(line.begin(), line.end(), '\n'), line.end());
		line.erase(remove(line.begin(), line.end(), '\t'), line.end());
		line.erase(remove(line.begin(), line.end(), ' '), line.end());
		line.erase(remove(line.begin(), line.end(), '"'), line.end());

		size_t colonPos = line.find_first_of(':');

		string tagID, tagName;
		tagID = line.substr(0, colonPos);
		tagName = line.substr(colonPos + 1);

		if (tagID == "MODEL")
		{
			ModelData md;
			md.tagName = tagName;

			string block;
			block.resize(16384);
			infile.getline((char*)block.data(), block.size(), '}');
			block.erase(remove(block.begin(), block.end(), '\n'), block.end());
			block.erase(remove(block.begin(), block.end(), '\t'), block.end());
			block.erase(remove(block.begin(), block.end(), ' '), block.end());
			block.erase(remove(block.begin(), block.end(), '"'), block.end());

			stringstream ss(block);
			string context;
			context.resize(1024);

			while (ss.getline((char*)context.data(), context.size(), ';'))
			{
				string tempID, tempValue;
				size_t equalPos = context.find_first_of('=');
				tempID = context.substr(0, equalPos);
				tempValue = context.substr(equalPos + 1);


				if (tempID == "FILEPATH")
				{
					md.filePath = tempValue;
				}

				if (tempID == "TEXTUREPATH")
				{
					md.texturePath = tempValue;
				}

				if (tempID == "NORMALPATH")
				{
					md.normalPath = tempValue;
				}

				if (tempID == "TRANSFORM")
				{
					size_t squarebrktopen = tempValue.find_first_of('[');
					size_t squarebrktclose = tempValue.find_first_of(']');

					tempValue = tempValue.substr(squarebrktopen + 1, squarebrktclose);

					stringstream transformdatastr(tempValue);
					string str;
					str.resize(1024);

					while (transformdatastr.getline((char*)str.data(), str.size(), '|'))
					{
						string _tID, _tValue;
						size_t _equalPos = str.find_first_of('=');
						_tID = str.substr(0, _equalPos);
						_tValue = str.substr(_equalPos + 1);

						if (_tID == "MOVABLE")
						{
							md.bMovable = !strcmp(_tValue.c_str(), "true");
						}

						if (_tID == "POSITION")
						{
							string valstr = _tValue;
							size_t commaPos = valstr.find_first_of(',');
							md.Position.x = stoi(valstr.substr(0, commaPos));

							valstr = valstr.substr(commaPos + 1);
							commaPos = valstr.find_first_of(',');
							md.Position.y = stoi(valstr.substr(0, commaPos));

							valstr = valstr.substr(commaPos + 1);
							commaPos = valstr.find_first_of(',');
							md.Position.z = stoi(valstr.substr(0, commaPos));
						}

						if (_tID == "ROTATION")
						{
							string valstr = _tValue;
							size_t commaPos = valstr.find_first_of(',');
							md.Rotation.x = stoi(valstr.substr(0, commaPos));

							valstr = valstr.substr(commaPos + 1);
							commaPos = valstr.find_first_of(',');
							md.Rotation.y = stoi(valstr.substr(0, commaPos));

							valstr = valstr.substr(commaPos + 1);
							commaPos = valstr.find_first_of(',');
							md.Rotation.z = stoi(valstr.substr(0, commaPos));
						}

						if (_tID == "SCALE")
						{
							string valstr = _tValue;
							size_t commaPos = valstr.find_first_of(',');
							md.Scale.x = stoi(valstr.substr(0, commaPos));

							valstr = valstr.substr(commaPos + 1);
							commaPos = valstr.find_first_of(',');
							md.Scale.y = stoi(valstr.substr(0, commaPos));

							valstr = valstr.substr(commaPos + 1);
							commaPos = valstr.find_first_of(',');
							md.Scale.z = stoi(valstr.substr(0, commaPos));

						}
					}
				}
			}

			g_modelData.push_back(md);
		}
	}



	infile.close();
	system("pause");
	return 0;
}







