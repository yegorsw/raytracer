#include "IOfunctions.h"
#include "MtlLib.h"
#include "GeoContainer.h"

using namespace std;

double clamp(double n, double minValue, double maxValue)
{
	if (n < minValue)
		return minValue;
	if (n > maxValue)
		return maxValue;
	return n;
}

//convert int to string and pad (python zfill()) - '4' -> "0004"
string padInt(int nIn)
{
	string n = to_string(nIn);
	string outString = string(4 - n.length(), ' ') + n;
	return outString;
}

//split string into vector of substrings
vector<string> split(string &inputString, char splitChar)
{
	string stringBuffer = "";
	vector<string> outputString;
	for (unsigned int i = 0; i < inputString.length(); i++)
	{
		if (inputString[i] == splitChar)
		{
			outputString.push_back(stringBuffer);
			stringBuffer = "";
		}
		else
		{
			stringBuffer += inputString[i];
		}
	}
	if (stringBuffer.length() > 0)
	{
		outputString.push_back(stringBuffer);
	}
	return outputString;
}

void loadMtlFile(string filename, MtlLib* mlib)
{
	cout << "Loading materials from " << filename << endl;

	string line;
	vector<string> splitLine;
	ifstream mtlFile(filename);
	//string lastLine = "!temp!";
	Mtl newMtl;

	while (getline(mtlFile, line))
	{
		splitLine = split(line, ' ');
		cout << splitLine[1] << endl;

		Color col;
		Shader* newShader;

		if (splitLine.size() > 0)
		{
			if (splitLine[0] == "newmtl")
			{
				if (newMtl.name != "")
				{
					mlib->addMtl(newMtl);
					newMtl = Mtl();
				}
				newMtl.name = splitLine[1];
			}
			else if (splitLine[0] == "Kd")
			{
				col = Color(stod(splitLine[1]), stod(splitLine[2]), stod(splitLine[3]));
				if (col > 0.0)
				{
					newMtl.addShader(new ShaderLambert(col));
				}
			}
			else if (splitLine[0] == "Ks")
			{
				col = Color(stod(splitLine[1]), stod(splitLine[2]), stod(splitLine[3]));
				if (col > 0.0)
				{
					newMtl.addShader(new ShaderReflection(col));
				}
			}
			else if (splitLine[0] == "Ka")
			{
				col = Color(stod(splitLine[1]), stod(splitLine[2]), stod(splitLine[3]));
				if (col > 0.0)
				{
					newMtl.addShader(new ShaderEmissive(col));
				}
			}
		}
		//lastLine = splitLine[0];
	}

	mlib->addMtl(newMtl);
	cout << "Loaded material: " << newMtl.name << endl;

	cout << "Loaded " << mlib->numberOfMtls() << " materials." << endl;
}


Scene readObj(string filename)
{
	cout << "Reading " << filename << endl;
	Scene newScene;
	ifstream objFile(filename);
	string line;
	vector<string> splitLine;
	vector<Vec> verticies, normals;
	string lastLine = "temp";

	Mtl* currentMtl = new Mtl();
	Tri* currentTri;
	MtlLib* mlib = new MtlLib();


	int totalTriCount = 0;
	int totalMeshCount = 0;

	int i0, i1, i2, n0, n1, n2;
	int lineId = 1;
	while (getline(objFile, line))
	{
		splitLine = split(line, ' ');
		lineId++;
		if (splitLine.size() > 0)
		{
			if (splitLine[0] == "v")
			{
				verticies.push_back(Vec(stod(splitLine[1]), stod(splitLine[2]), stod(splitLine[3])));
			}
			else if (splitLine[0] == "vn")
			{
				normals.push_back(Vec(stod(splitLine[1]), stod(splitLine[2]), stod(splitLine[3])));;
			}
			else if (splitLine[0] == "f")
			{
				i0 = stoi(split(splitLine[1], '/')[0]) - 1;
				i1 = stoi(split(splitLine[2], '/')[0]) - 1;
				i2 = stoi(split(splitLine[3], '/')[0]) - 1;
				n0 = stoi(split(splitLine[1], '/')[2]) - 1;
				n1 = stoi(split(splitLine[2], '/')[2]) - 1;
				n2 = stoi(split(splitLine[3], '/')[2]) - 1;
				currentTri = new Tri;
				currentTri->setPoints(verticies[i0], verticies[i1], verticies[i2]);
				currentTri->setNormals(normals[n0], normals[n1], normals[n2]);
				currentTri->mtl = currentMtl;
				currentTri->precompute();
				newScene.addTri(currentTri);
				totalTriCount++;
				//currentTri.printToConsole();
			}
			else if (splitLine[0] == "mtllib")
			{
				loadMtlFile(filename.substr(0, filename.size() - 3) + "mtl", mlib);
				newScene.mlib = mlib;
			}
			else if (splitLine[0] == "usemtl")
			{
				currentMtl = newScene.mlib->findMtl(splitLine[1]);
				cout << "Assigning material: " << currentMtl->name << endl;
			}
			lastLine = splitLine[0];
		}
	}
	totalMeshCount++;
	objFile.close();

	newScene.generateBbox();

	cout << "Loaded " << totalTriCount << " triangles among " << totalMeshCount << " meshes." << endl;

	return newScene;
}

void writePPM(Screen &screenIn, string filename, string mode){

	cout << "Writing " + filename << endl;

	ofstream outputFile;
	outputFile.open(filename);

	//header
	outputFile << "P3\n" << screenIn.xres << " " << screenIn.yres << " 256\n";
	Pixel currentPixel;

	if (mode == "rgba")
	{
		for (int y = 0; y < screenIn.yres; y++){
			for (int x = 0; x < screenIn.xres; x++){
				currentPixel = screenIn.getPixel(x, y);
				double a = currentPixel.a;
				string r = padInt((int)(clamp(currentPixel.color.r, 0, 1) * 255));
				string g = padInt((int)(clamp(currentPixel.color.g, 0, 1) * 255));
				string b = padInt((int)(clamp(currentPixel.color.b, 0, 1) * 255));
				outputFile << r << g << b;
			}
			outputFile << '\n';
		}
	}
	else if (mode == "samples")
	{
		for (int y = 0; y < screenIn.yres; y++){
			for (int x = 0; x < screenIn.xres; x++){
				currentPixel = screenIn.getPixel(x, y);
				double a = currentPixel.a;
				string r = padInt((int)(clamp(currentPixel.samples.r * a, 0, 1) * 255));
				string g = padInt((int)(clamp(currentPixel.samples.g * a, 0, 1) * 255));
				string b = padInt((int)(clamp(currentPixel.samples.b * a, 0, 1) * 255));
				outputFile << r << g << b;
			}
			outputFile << '\n';
		}
	}

	outputFile.close();
}