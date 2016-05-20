#include "IOfunctions.h"
#include "MtlLib.h"

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

void readMtl(string filename, MtlLib* mlib)
{
	cout << "Reading " << filename << endl;

	string line;
	vector<string> splitLine;
	ifstream mtlFile(filename);
	string lastLine = "!temp!";
	Mtl newMtl;

	while (getline(mtlFile, line))
	{
		splitLine = split(line, ' ');
		if (splitLine.size() > 0)
		{
			if (splitLine[0] == "newmtl")
			{
				if (newMtl.name != "")
				{
					mlib->addMtl(newMtl);
					Mtl newMtl;
				}
				newMtl.name = splitLine[1];
			}
			else if (splitLine[0] == "Kd")
			{
				newMtl.Kd = Color(stod(splitLine[1]), stod(splitLine[2]), stod(splitLine[3]));
			}
			else if (splitLine[0] == "Ka")
			{
				newMtl.Ka = Color(stod(splitLine[1]), stod(splitLine[2]), stod(splitLine[3]));
			}
		}
		lastLine = splitLine[0];
	}

	mlib->addMtl(newMtl);

	cout << "Loaded " << mlib->numberOfMtls() << "materials." << endl;
}

Scene readObj(string filename)
{
	cout << "Reading " << filename << endl;
	Mesh* newMesh = new Mesh();
	Scene newScene;
	ifstream objFile(filename);
	string line;
	vector<string> splitLine;
	vector<Vec> verticies, normals;
	string lastLine = "temp";

	Mtl* currentMtl = new Mtl();
	MtlLib* mlib = new MtlLib();


	int totalTriCount = 0;
	int totalMeshCount = 0;
	//int mtlId = 0;
	//string currentMat = "";
	int i0, i1, i2;
	int lineId = 1;
	while (getline(objFile, line))
	{
		splitLine = split(line, ' ');
		lineId++;
		if (splitLine.size() > 0)
		{
			if (splitLine[0] == "v")
			{
				if ((lastLine == "f" || lastLine == "g") && verticies.size() > 0)
				{
					newMesh->generateBbox();

					newScene.addMesh(newMesh);
					totalMeshCount++;
					newMesh = new Mesh();
					//newMesh->clear();
					//verticies.clear();
					normals.clear();
					
				}
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
				Tri currentTri = { verticies[i0], verticies[i1], verticies[i2] };
				//currentTri.setNormals(normals[i0], normals[i1], normals[i2]);
				currentTri.mtl = currentMtl;

				newMesh->addTri(currentTri);
				totalTriCount++;
				//currentTri.printToConsole();
			}
			else if (splitLine[0] == "mtllib")
			{
				readMtl("D:/Users/Yegor/Desktop/raytracer/objects/" + splitLine[1], mlib);
				newScene.mlib = mlib;
			}
			else if (splitLine[0] == "usemtl")
			{
				currentMtl = newScene.mlib->findMtl(splitLine[1]);
				cout << currentMtl->name << endl;
			}
			lastLine = splitLine[0];
		}
	}
	newMesh->generateBbox();

	newScene.addMesh(newMesh);
	totalMeshCount++;
	objFile.close();

	cout << "Loaded " << totalTriCount << "tris among " << totalMeshCount << "meshes." << endl;

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
				string r = padInt((int)(clamp(currentPixel.color.r * a, 0, 1) * 255));
				string g = padInt((int)(clamp(currentPixel.color.g * a, 0, 1) * 255));
				string b = padInt((int)(clamp(currentPixel.color.b * a, 0, 1) * 255));
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