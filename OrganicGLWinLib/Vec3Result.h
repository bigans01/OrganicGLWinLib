#pragma once

#ifndef VEC3RESULT_H
#define VEC3RESULT_H


class Vec3Result
{
	public:
		Vec3Result() {};
		Vec3Result(bool in_isResultValid, glm::vec3 in_resultPoint) :
			isResultValid(in_isResultValid),
			resultPoint(in_resultPoint)
		{};
		bool isResultValid = true;
		glm::vec3 resultPoint;
};

#endif
