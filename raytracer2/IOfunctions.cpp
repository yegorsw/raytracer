#include "IOfunctions.h"

using namespace std;

double clamp(double n, double minValue, double maxValue){
	if (n < minValue)
		return minValue;
	if (n > maxValue)
		return maxValue;
	return n;
}

//convert int to string and pad (python zfill()) - '4' -> "0004"
string padInt(int nIn){
	string n = to_string(nIn);
	string outString = string(4 - n.length(), ' ') + n;
	return outString;
}

//split string into vector of substrings
vector<string> split(string &inputString, char splitChar){
	string stringBuffer = "";
	vector<string> outputString;
	for (unsigned int i = 0; i < inputString.length(); i++){
		if (inputString[i] == splitChar){
			outputString.push_back(stringBuffer);
			stringBuffer = "";
		}
		else{
			stringBuffer += inputString[i];
		}
	}
	if (stringBuffer.length() > 0){
		outputString.push_back(stringBuffer);
	}
	return outputString;
}


Scene readObj(string filename){
	cout << "Reading " << filename << endl;
	Mesh newMesh;
	Scene newScene;
	ifstream objFile(filename);
	string line;
	vector<string> splitLine;
	vector<Vec> verticies, normals;
	string lastLine = "temp";
	//int mtlId = 0;
	//string currentMat = "";
	int i0, i1, i2;
	int lineId = 1;
	while (getline(objFile, line)){
		splitLine = split(line, ' ');
		lineId++;
		if (splitLine.size() > 0)
		{
			if (splitLine[0] == "v")
			{
				if (lastLine == "f" || lastLine == "g")
				{
					newMesh.generateBbox();

					newScene.addMesh(newMesh);
					newMesh.clear();
					//verticies.clear();
					normals.clear();
					
				}
				verticies.push_back(Vec(stod(splitLine[1]), stod(splitLine[2]), stod(splitLine[3])));
			}
			else if (splitLine[0] == "vn")
			{
				normals.push_back(Vec(stod(splitLine[1]), stod(splitLine[2]), stod(splitLine[3])));;
			}
			else if (splitLine[0] == "f"){
				i0 = stoi(split(splitLine[1], '/')[0]) - 1;
				i1 = stoi(split(splitLine[2], '/')[0]) - 1;
				i2 = stoi(split(splitLine[3], '/')[0]) - 1;
				Tri currentTri = { verticies[i0], verticies[i1], verticies[i2] };
				//currentTri.setNormals(normals[i0], normals[i1], normals[i2]);
				newMesh.addTri(currentTri);
				//currentTri.printToConsole();
			}
			lastLine = splitLine[0];
		}
	}
	newMesh.generateBbox();

	newScene.addMesh(newMesh);
	objFile.close();

	cout << "Finished Reading.";

	return newScene;
}

void writePPM(Screen &screenIn, string filename){

	cout << "Writing " + filename << endl;

	ofstream outputFile;
	outputFile.open(filename);

	//header
	outputFile << "P3\n" << screenIn.xres << " " << screenIn.yres << " 256\n";
	Pixel currentPixel;

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

	outputFile.close();
}