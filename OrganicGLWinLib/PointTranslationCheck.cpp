#include "stdafx.h"
#include "PointTranslationCheck.h"

int PointTranslationCheck::performCheck(glm::vec3 in_pointToCheck)
{
	int returnValue = 0;
	if
		(
		(in_pointToCheck.x != 0.0f)
			||
			(in_pointToCheck.y != 0.0f)
			||
			(in_pointToCheck.z != 0.0f)
			)
	{
		returnValue = 1;
		requiresTranslation = 1;
		translationValue.x = (in_pointToCheck.x) * -1.0f;
		translationValue.y = (in_pointToCheck.y) * -1.0f;
		translationValue.z = (in_pointToCheck.z) * -1.0f;
	}
	else
	{
		// std::cout << "!!! Point is at 0, 0, 0...no translation required." << std::endl;
	}

	// apply blueprint values

	return returnValue;
}

int PointTranslationCheck::performCheck(glm::vec3 in_pointToCheck, int in_blueprintKeyX, int in_blueprintKeyY, int in_blueprintKeyZ)
{
	int returnValue = 0;
	if
		(
		(in_pointToCheck.x != 0.0f)
			||
			(in_pointToCheck.y != 0.0f)
			||
			(in_pointToCheck.z != 0.0f)
			)
	{
		returnValue = 1;
		requiresTranslation = 1;
		translationValue.x = (in_pointToCheck.x) * -1.0f;
		translationValue.y = (in_pointToCheck.y) * -1.0f;
		translationValue.z = (in_pointToCheck.z) * -1.0f;
	}
	else
	{
		// std::cout << "!!! Point is at 0, 0, 0...no translation required." << std::endl;
	}

	// apply blueprint values
	translationValue.x += ((in_blueprintKeyX * 32) * -1.0f);
	translationValue.y += ((in_blueprintKeyY * 32) * -1.0f);
	translationValue.z += ((in_blueprintKeyZ * 32) * -1.0f);

	return returnValue;
}

glm::vec3 PointTranslationCheck::getTranslationValue()
{
	//std::cout << "### Returning translation value: " << translationValue.x << ", " << translationValue.y << ", " << translationValue.z << std::endl;
	return translationValue;
}

glm::vec3 PointTranslationCheck::getReverseTranslationValue()
{
	glm::vec3 reverseTranslation;
	reverseTranslation.x = (translationValue.x * -1.0f);
	reverseTranslation.y = (translationValue.y * -1.0f);
	reverseTranslation.z = (translationValue.z * -1.0f);
	return reverseTranslation;
}