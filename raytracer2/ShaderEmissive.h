#pragma once
#include "Shader.h"
#include "Color.h"
class ShaderEmissive :
	public Shader
{
public:
	ShaderEmissive()
	{
		Shader::emitslight = true;
	}

	ShaderEmissive(Color c)
	{
		Shader::color = c;
		Shader::emitslight = true;
	}

	~ShaderEmissive()
	{
	}
};

