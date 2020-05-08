#pragma once

#ifndef POINTTRANSLATIONCHECK_H
#define POINTTRANSLATIONCHECK_H

#include <glm/glm.hpp>
#include <iostream>

class PointTranslationCheck
{
public:
	int requiresTranslation = 0; // will there be any translation required?
	glm::vec3 translationValue;		// stores the translation (this may not be used)
	int performCheck(glm::vec3 in_pointToCheck);
	glm::vec3 getTranslationValue();
	glm::vec3 getReverseTranslationValue();
};

#endif
