#include "stdafx.h"
#include "BasicGrayscaleMG.h"

void BasicGrayscaleMG::initializeMGear()
{
	std::cout << "BasicGrayscaleMG: attempting instantiation of shader." << std::endl;

	// fetch the constructed program data, store it
	auto programData = OrganicShaderLoader::createBasicShaderProgram("graphics/mGearShaders/BasicGrayscaleMG.vertexshader", "graphics/mGearShaders/BasicGrayscaleMG.fragmentshader");
	mGearProgramID = programData.programId;
	grayscaleVertexShaderId = programData.vertexShaderId;
	grayscaleFragmentShaderId = programData.fragmentShaderId;

	std::cout << "BasicGrayscaleMG: checking current required variables: " << std::endl;
	auto screenWidth = mGearUniformRegistryRef->getInt("screen_pixel_width");
	auto screenHeight = mGearUniformRegistryRef->getInt("screen_pixel_height");
	auto computeGroupDim = mGearUniformRegistryRef->getInt("screen_compute_group_dim");
	auto computeColumns = mGearUniformRegistryRef->getInt("screen_compute_columns");
	auto computeRows = mGearUniformRegistryRef->getInt("screen_compute_rows");
	std::cout << "screenWidth: " << screenWidth << std::endl;
	std::cout << "screenHeight: " << screenHeight << std::endl;;
	std::cout << "computeGroupDim: " << computeGroupDim << std::endl;
	std::cout << "computeColumns: " << computeColumns << std::endl;
	std::cout << "computeRows: " << computeRows << std::endl;
}

std::vector<MAPIObjectMetadata> BasicGrayscaleMG::fetchRequiredMAPIObjects()
{
	std::vector<MAPIObjectMetadata> returnVector;
	returnVector.push_back(MAPIObjectMetadata(MAPIObjectType::BUFFER, "testRenderBuffer"));
	returnVector.push_back(MAPIObjectMetadata(MAPIObjectType::VAO, "commonVAO"));

	// Debug/testing only: Unused objects, but used to prompt some output to ensure that TEXTURE / FBO checks are done.
	returnVector.push_back(MAPIObjectMetadata(MAPIObjectType::TEXTURE, "testTexture"));
	returnVector.push_back(MAPIObjectMetadata(MAPIObjectType::FBO, "testFBO"));


	return returnVector;
}

std::vector<GLUniformRequest> BasicGrayscaleMG::fetchRequiredGLUniforms()
{
	std::vector<GLUniformRequest> requiredUniforms;

	GLUniformRequest basicMat4Req(GLDataType::MAT4, "basicMat4");
	requiredUniforms.push_back(basicMat4Req);

	return requiredUniforms;
}

void BasicGrayscaleMG::setObjectBindings()
{

}

void BasicGrayscaleMG::conductMAPIObjectScan(bool in_fullscanFlag, std::vector<MAPIObjectUpdate>* in_updateVectorRef)
{
	// If the full scan flag is set, we will need to check all MAPI objects to see if there's anything we want.
	if (in_fullscanFlag)
	{
		// NOTICE: Be sure to clear out the container(s) used by this MGearBase-derived class completely, 
		// before inserting based off a full scan (it is safer to start from scratch, as bindings for any deleted buffers
		// will still be lurking in the binding maps (before the first tick) )
		//
		// This needs to be fixed at a later date to somtehing cleaner (noted on 5/2/2025)

		std::cout << "BasicGrayscaleMG: conducting scan of usable objects (FULL_SCAN)" << std::endl;
		std::vector<MAPIObjectBinding> fetchedBindings = mGearObjectManagerRef->fetchAllBindings();
		for (auto& currentBinding : fetchedBindings)
		{
			// ...
			// Do whatever we want here, (ignore a buffer, accept a buffer, put it's value into a map...)
			// ...

			currentBinding.printBindingData();

			if
			(
				(currentBinding.getBindingRequestName() == "testRenderBuffer")
				&&
				(currentBinding.getBindingMapKeyType() == MAPIObjectMapKeyType::STRING_KEYTYPE)
				&&
				(currentBinding.getBindingRequestType() == MAPIObjectType::BUFFER)
			)
			{
				grayscaleMainBufferId = currentBinding.getBindingValue();
				std::cout << "!! Found testRenderBuffer to bind to for BasicGrayscaleMG to render simple triangles. Updating basicBufferID to be: " << grayscaleMainBufferId << std::endl;

				auto currentClearColor = mGearUniformRegistryRef->getVec3("background_clear_color");
				std::cout << "!! Current background color value: " << currentClearColor.x << "," << currentClearColor.y << "," << currentClearColor.z << std::endl;
			}
		}

		auto absoluteMVPCheck = mGearUniformRegistryRef->doesMat4Exist("absolute_MVP_matrix");
		if (absoluteMVPCheck)
		{
			std::cout << "!! BasicGrayscaleMG: found expected mat4 MVP value, named absolute_MVP_matrix! " << std::endl;
		}
		else
		{
			std::cout << "Did NOT find expected mat4 MVP value. " << std::endl;
		}
	}
	// ...otherwise, check whatever was added in the in_updateVectorRef, which contains updates for this tick, but gets reset
	// and cleared after the tick ends.
	else
	{
		for (auto& currentObjectUpdate : *in_updateVectorRef)
		{
			currentObjectUpdate.printUpdateData();
		}
	}
}

void BasicGrayscaleMG::render()
{
	// switch to this program, then
	// use MAPIObjectManager::fetchBinding to bind to appropriate resources (i.e, glBindBuffer(mGearObjectManagerRef->fetchBinding(...)); )
	switchToMGearProgram();

	// Before rendering the very first time, setup the VAO; we assume the VAO exists.
	if (!vaoSetup)
	{
		setupVAO();
	}

	// Clear out the default FBO 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Checks/fetches from uniform registry can be done now, that we have switched to the program. This code assumes all the uniforms
	// that the MGearBase-derivative class requires have been defined already (see functions BasicGrayscaleMG::fetchRequiredMAPIObjects and BasicGrayscaleMG::fetchRequiredGLUniforms)
	// ...
	// ... 
	glBindVertexArray(vaoId);
	auto fetchedMVP = mGearUniformRegistryRef->getMat4("absolute_MVP_matrix");
	GLuint mvpUniform = glGetUniformLocation(mGearProgramID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &fetchedMVP[0][0]);		// set the uniform

	// Hard-coded triangle setup (temporary only)
	// Logic: 1 triangle, 9 floats per triangle, times 2 (vec3 for point, vec3 for color)
	int totalHighlightFloats = 1 * 9 * 2;
	testPointArray.reset(new GLfloat[totalHighlightFloats]);

	glm::vec3 point0(0.0, 0.0, 0.0);
	glm::vec3 color0(1.0, 0.0, 0.0);
	glm::vec3 point1(1.0, 0.0, 0.0);
	glm::vec3 color1(0.0, 1.0, 0.0);
	glm::vec3 point2(1.0, 0.0, 1.0);
	glm::vec3 color2(0.0, 0.0, 1.0);

	std::vector<glm::vec3> orderedVector;
	orderedVector.push_back(point0);
	orderedVector.push_back(color0);
	orderedVector.push_back(point1);
	orderedVector.push_back(color1);
	orderedVector.push_back(point2);
	orderedVector.push_back(color2);

	auto orderedBegin = orderedVector.begin();
	int arrayIndex = 0;
	for (int x = 0; x < 6; x++)
	{
		testPointArray[arrayIndex] = orderedBegin->x;
		testPointArray[arrayIndex + 1] = orderedBegin->y;
		testPointArray[arrayIndex + 2] = orderedBegin->z;

		orderedBegin++;
		arrayIndex += 3;
	}

	// vertices = number of triangles x 3; in this hardcoded test, it's just 1 triangle.
	int numberOfVertices = 1 * 3;

	glBindBuffer(GL_ARRAY_BUFFER, grayscaleMainBufferId);
	glBufferData(GL_ARRAY_BUFFER, totalHighlightFloats * 4, testPointArray.get(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);

}

void BasicGrayscaleMG::setupVAO()
{
	vaoId = mGearObjectManagerRef->fetchBinding(MAPIObjectMetadata(MAPIObjectType::VAO, "commonVAO"));
	grayscaleMainBufferId = mGearObjectManagerRef->fetchBinding(MAPIObjectMetadata(MAPIObjectType::BUFFER, "testRenderBuffer"));
	
	std::cout << "!! BasicGrayscaleMG::setupVAO: found vaoId: " << vaoId << std::endl;
	std::cout << "!! BasicGrayscaleMG::setupVAO: found grayscaleMainBufferId: " << grayscaleMainBufferId << std::endl;
	std::cout << "!! BasicGrayscaleMG::setupVAO: program ID is: " << mGearProgramID << std::endl;

	//glVertexAttribPointer(
	//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	//	3,                  // size
	//	GL_FLOAT,           // type
	//	GL_FALSE,           // normalized?
	//	0,                  // stride = 0 (tightly packed); bytes offset between consecutive generic vertex attributes is 0.
	//	(void*)0            /* array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
	//						For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
	//						*/
	//);

	glBindBuffer(GL_ARRAY_BUFFER, grayscaleMainBufferId);
	glBindVertexArray(vaoId);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);		// First attribute: a vec3 representing the point data, before it is translated by MVP.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)12);  

	glEnableVertexAttribArray(0);

	vaoSetup = true;
}

void BasicGrayscaleMG::cleanupMGear()
{
	// delete shaders, and then the program itself
	std::cout << "BasicGrayscaleMG: cleaning up resources..." << std::endl;
	glDeleteShader(grayscaleVertexShaderId);
	glDeleteShader(grayscaleFragmentShaderId);
	glDeleteProgram(mGearProgramID);
}