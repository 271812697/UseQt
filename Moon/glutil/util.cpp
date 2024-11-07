#include "util.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdio>

static unsigned int load_shader(
	const std::string& src, const GLenum type)
{
	if (src.empty())
	{
		return 0;
	}

	unsigned int s = glCreateShader(type);
	if (s == 0)
	{
		fprintf(stderr, "Error: load_shader() failed to create shader.\n");
		return 0;
	}

	const char* c = src.c_str();
	glShaderSource(s, 1, &c, NULL);
	glCompileShader(s);
	return s;
}
bool MOON::create_shader_program(
	const std::string& geom_source,
	const std::string& vert_source,
	const std::string& frag_source,
	const std::map<std::string, unsigned int>& attrib,
	unsigned int& id)
{
	using namespace std;
	if (vert_source == "" && frag_source == "")
	{
		cerr <<
			"create_shader_program() could not create shader program,"
			" both .vert and .frag source given were empty" << endl;
		return false;
	}

	// create program
	id = glCreateProgram();
	if (id == 0)
	{
		cerr << "create_shader_program() could not create shader program." << endl;
		return false;
	}
	GLuint g = 0, f = 0, v = 0;

	if (geom_source != "")
	{
		// load vertex shader
		g = load_shader(geom_source.c_str(), GL_GEOMETRY_SHADER);
		if (g == 0)
		{
			cerr << "geometry shader failed to compile." << endl;
			return false;
		}
		glAttachShader(id, g);
	}

	if (vert_source != "")
	{
		// load vertex shader
		v = load_shader(vert_source.c_str(), GL_VERTEX_SHADER);
		if (v == 0)
		{
			cerr << "vertex shader failed to compile." << endl;
			return false;
		}
		glAttachShader(id, v);
	}

	if (frag_source != "")
	{
		// load fragment shader
		f = load_shader(frag_source.c_str(), GL_FRAGMENT_SHADER);
		if (f == 0)
		{
			cerr << "fragment shader failed to compile." << endl;
			return false;
		}
		glAttachShader(id, f);
	}

	// loop over attributes
	for (
		std::map<std::string, unsigned int>::const_iterator ait = attrib.begin();
		ait != attrib.end();
		ait++)
	{
		glBindAttribLocation(
			id,
			(*ait).second,
			(*ait).first.c_str());
	}
	// Link program
	glLinkProgram(id);
	const auto& detach = [&id](const unsigned int shader)
		{
			if (shader)
			{
				glDetachShader(id, shader);
				glDeleteShader(shader);
			}
		};
	detach(g);
	detach(f);
	detach(v);

	// print log if any
	//MOON::print_program_info_log(id);

	return true;
}

bool MOON::create_shader_program(
	const std::string& vert_source,
	const std::string& frag_source,
	const std::map<std::string, unsigned int>& attrib,
	GLuint& prog_id)
{
	return create_shader_program("", vert_source, frag_source, attrib, prog_id);
}
GLuint MOON::create_shader_program(
	const std::string& geom_source,
	const std::string& vert_source,
	const std::string& frag_source,
	const std::map<std::string, unsigned int>& attrib)
{
	GLuint prog_id = 0;
	create_shader_program(geom_source, vert_source, frag_source, attrib, prog_id);
	return prog_id;
}

GLuint MOON::create_shader_program(
	const std::string& vert_source,
	const std::string& frag_source,
	const std::map<std::string, unsigned int>& attrib)
{
	GLuint prog_id = 0;
	create_shader_program(vert_source, frag_source, attrib, prog_id);
	return prog_id;
}

