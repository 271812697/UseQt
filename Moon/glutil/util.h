#pragma once
#include <string>
#include <map>
namespace MOON
{

	bool create_shader_program(
		const std::string& geom_source,
		const std::string& vert_source,
		const std::string& frag_source,
		const std::map<std::string, unsigned int>& attrib,
		unsigned int& id);
	bool create_shader_program(
		const std::string& vert_source,
		const std::string& frag_source,
		const std::map<std::string, unsigned int>& attrib,
		unsigned int& id);

	unsigned int create_shader_program(
		const std::string& geom_source,
		const std::string& vert_source,
		const std::string& frag_source,
		const std::map<std::string, unsigned int>& attrib);

	unsigned int create_shader_program(
		const std::string& vert_source,
		const std::string& frag_source,
		const std::map<std::string, unsigned int>& attrib);
}

