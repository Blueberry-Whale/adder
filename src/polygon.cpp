#include "polygon.h"

namespace adder {

Polygon::Polygon(std::vector<glm::vec2> vertices, glm::vec2 pos) :
	_verts{vertices, 0.0f, 1.0f} {
	set_position(pos, 0.0f, 1.0f);
}

Polygon::Polygon(std::vector<glm::vec4> vertices, glm::vec4 pos) :
	_verts{vertices} {
	set_position(pos);
}

Polygon::~Polygon() {
}

void Polygon::rotate(float ang, const glm::vec4 &axis) {
	if(ang == 0) return;

	glm::vec4 negated_axis = -axis;
	negated_axis[3] = 1;
	translate(negated_axis);

	glm::mat4 rotmat = glm::rotate(glm::mat4(1.0), (float)ang * 180 / PI, glm::vec3(0, 0, 1)); // Always rotating around z-axis
	for(int i = 0; i < _verts.size(); ++i)
		_verts[i] = rotmat*_verts[i];
	translate(axis);
}

void Polygon::set_position(const glm::vec4 &pos) {
	if(pos != _pos) {
		translate(pos - _pos);
	}
}

void Polygon::translate(const glm::vec4 &xy) {
	_pos += xy;
	_pos[3] = 1;
	for(glm::vec4 &pt : _verts) {
		pt += xy;
		pt[3] = 1;
	}
}

glm::vec4 Polygon::position() {
	return _pos;
}

std::vector<glm::vec4> Polygon::vertices() {
	return _verts;
}

bool Polygon::is_convex() {
	if(!_cache_status.is_convex) {
		glm::vec3 a{_verts.front()[0] - _verts.back()[0], _verts.front()[1] - _verts.back()[1], 0.f};
		glm::vec3 b{_verts[1][0] - _verts.front()[0], _verts[1][1] - _verts.front()[1], 0.f};

		float z = glm::cross(a, b)[2];
		int sign = ((z > 0) ? 1 : -1);

		_is_convex = true;

		for(int i = 0; i < _verts.size() - 2; ++i) {
			glm::vec3 a{_verts[i + 1][0] - _verts[i][0], _verts[i + 1][1] - _verts[i][1], 0.f};
			glm::vec3 b{_verts[i + 2][0] - _verts[i + 1][0], _verts[i + 2][1] - _verts[i + 1][1], 0.f};
			float z = glm::cross(a, b)[2];
			int this_sign = ((z > 0) ? 1 : -1);
			if(sign != this_sign) {
				_is_convex = false;
				break;
			}
		}
		_cache_status.is_convex = true;
	}

	return _is_convex;
}

glm::vec4 Polygon::get_pos() {
	return _pos;
}

} // adder

std::string to_string(const std::vector<glm::vec4> &vertices) {
	std::string str = "";
	for(const glm::vec4 &vertex : vertices)
		str += "(" + std::to_string(vertex[0]) + ", " + std::to_string(vertex[1]) + ") ";
	return str;
}
