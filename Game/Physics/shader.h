#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"

bool CompileShader(GLuint shader, const char* file_name) {
	std::ifstream vertex_shader_file{ file_name };
	std::stringstream buf;
	buf << vertex_shader_file.rdbuf();

	std::string shader_source = buf.str();
	auto source_ptr = shader_source.c_str();

	glShaderSource(shader, 1, &source_ptr, 0);
	glCompileShader(shader);

	GLint is_success;
	GLchar error_msg[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &is_success);
	if (!is_success) {
		glGetShaderInfoLog(shader, 512, NULL, error_msg);
		std::cerr << "Failed compiling shader" << error_msg << std::endl;
	}

	return is_success;
}

bool InitShader(GLuint& shader_program) {
	auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	if (!CompileShader(vertex_shader, "vertex2.glsl"))
		return false;

	auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!CompileShader(fragment_shader, "fragment2.glsl"))
		return false;

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	GLint is_success;
	GLchar error_msg[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &is_success);
	if (!is_success) {
		glGetProgramInfoLog(shader_program, 512, 0, error_msg);
		std::cerr << "Failed linking fragment shader" << error_msg << std::endl;
		return false;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glUseProgram(shader_program);

	return true;
}