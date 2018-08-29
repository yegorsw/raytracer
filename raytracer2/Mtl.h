#pragma once
#include "Color.h"
#include "ShaderLambert.h"
#include "ShaderEmissive.h"
#include "ShaderReflection.h"
#include "ShaderRefraction.h"
#include <string>

using namespace std;

class Mtl
{
public:
	bool emitslight = false;
	bool scatterslight = false;

	vector<Shader*> shaderstack;

	string name = "default";

	Mtl()
	{
	}

	Mtl(string nameIn)
	{
		name = nameIn;
	}

	void addShader(Shader* s)
	{
		shaderstack.push_back(s);
		emitslight = emitslight || s->emitslight;
		scatterslight = scatterslight || s->scatterslight;
	}

	Shader* selectShaderStochastically()
	{
		Shader* shader = NULL;
		for (vector<Shader*>::iterator s = shaderstack.begin(); s != shaderstack.end(); s++)
		{
			if ((**s).scatterslight && ((**s).hitChance() > 0.9999 || ((**s).hitChance() > 0.0001 && randf() < (**s).hitChance())))
			{
				shader = &(**s);
			}
		}
		return shader;
	}

	Color getLightEmission()
	{

	}

	~Mtl()
	{
	}
};

