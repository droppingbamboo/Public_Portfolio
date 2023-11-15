// STL
#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Project
#include "torus.h"
#include <iostream>


namespace static_meshes_3D {

	Torus::Torus(int mainSegments, int tubeSegments, float mainRadius, float tubeRadius,
			bool withPositions,  bool withTextureCoordinates, bool withNormals):
			StaticMeshIndexed3D(withPositions, withTextureCoordinates, withNormals)
			,_mainSegments(mainSegments)
			,_tubeSegments(tubeSegments)
			,_mainRadius(mainRadius)
			,_tubeRadius(tubeRadius)
		    
	{
		initializeData();
	}

	float Torus::getMainSegments() const
	{
		return _mainSegments;
	}

	int Torus::getTubeSegments() const
	{
		return _tubeSegments;
	}

	float Torus::getMainRadius() const
	{
		return _mainRadius;
	}

	float Torus::getTubeRadius() const
	{
		return _tubeRadius;
	}

	void Torus::initializeData()
	{
		if (_isInitialized) {
			return;
		}

		// Calculate and cache numbers of vertices and indices
		const auto numVertices = (_mainSegments + 1) * (_tubeSegments + 1);
		_primitiveRestartIndex = numVertices;
		_numIndices = (_mainSegments * 2 * (_tubeSegments + 1)) + _mainSegments - 1;

		// Generate VAO and VBO for vertex attributes
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
		_vbo.createVBO(getVertexByteSize() * numVertices);
		_indicesVBO.createVBO(sizeof(GLuint) * _numIndices);

		
		auto mainSegmentAngleStep = glm::radians(360.0f / float(_mainSegments));
		auto tubeSegmentAngleStep = glm::radians(360.0f / float(_tubeSegments));
		
		if (hasPositions())
		{
			auto currentMainSegmentAngle = 0.0f;
			for(auto i = 0; i<=_mainSegments; i++)
			{
				//Calculate sine and cosine of main segment angle
				auto sinMainSegment = sin(currentMainSegmentAngle);
				auto cosMainSegment = cos(currentMainSegmentAngle);
				auto currentTubeSegmentAngle = 0.0f;
				for (auto j = 0; j <= _tubeSegments; j++)
				{
					//Calculate sine and cosine of tube segment angle
					auto sinTubeSegment = sin(currentTubeSegmentAngle);
					auto cosTubeSegment = cos(currentTubeSegmentAngle);
					
					//Calculate vertex position on the surface of the torus
					auto surfacePosition = glm::vec3(
						(_mainRadius + _tubeRadius * cosTubeSegment) * cosMainSegment,
						(_mainRadius + _tubeRadius * cosTubeSegment) * sinMainSegment,
						_tubeRadius * sinTubeSegment);

					
					_vbo.addRawData(&surfacePosition, sizeof(glm::vec3));

					//Update current tube angle
					currentTubeSegmentAngle += tubeSegmentAngleStep;
				}

				//Update main segment angle
				currentMainSegmentAngle += mainSegmentAngleStep;
			}
			
		}

		if (hasTextureCoordinates())
		{
			auto mainSegmentTextureStep = 2.0f / float(_mainSegments);
			auto tubeSegmentTextureStep = 1.0f / float(_tubeSegments);

			auto currentMainSegmentTexCoordV = 0.0f;
			for (auto i = 0; i <= _mainSegments; i++)
			{
				auto currentTubeSegmentTexCoordU = 0.0f;
				for (auto j = 0; j <= _tubeSegments; j++)
				{
					auto textureCoordinate = glm::vec2(currentTubeSegmentTexCoordU, currentMainSegmentTexCoordV);
					_vbo.addRawData(&textureCoordinate, sizeof(glm::vec2));
					currentTubeSegmentTexCoordU += tubeSegmentTextureStep;
				}
			//update texture coordinate of main segment
			currentMainSegmentTexCoordV += mainSegmentTextureStep;
			}
		}

		if (hasNormals())
		{
			auto currentMainSegmentAngle = 0.0f;
			for (auto i = 0; i <= _mainSegments; i++)
			{
				//Calculate sine and cosine of main segment angle
				auto sinMainSegment = sin(currentMainSegmentAngle);
				auto cosMainSegment = cos(currentMainSegmentAngle);
				auto currentTubeSegmentAngle = 0.0f;
				for (auto j = 0; j <= _tubeSegments; j++)
				{
					//Calculate sine and cosine of tube segment angle
					auto sinTubeSegment = sin(currentTubeSegmentAngle);
					auto cosTubeSegment = cos(currentTubeSegmentAngle);

					auto normal = glm::vec3(
						cosMainSegment * cosTubeSegment,
						sinMainSegment * cosTubeSegment,
						sinTubeSegment
					);
					_vbo.addRawData(&normal, sizeof(glm::vec3));

					//Update current tube angle
					currentTubeSegmentAngle += tubeSegmentAngleStep;
				}

				//Update main segment angle
				currentMainSegmentAngle += mainSegmentAngleStep;
			}
		}
				
		GLuint currentVertexOffset = 0;
		for (auto i = 0; i < _mainSegments; i++)
		{
			for (auto j = 0; j <= _tubeSegments; j++)
			{
				GLuint vertexIndexA = currentVertexOffset;
				_indicesVBO.addRawData(&vertexIndexA, sizeof(GLuint));
				GLuint vertexIndexB = currentVertexOffset + _tubeSegments + 1;
				_indicesVBO.addRawData(&vertexIndexB, sizeof(GLuint));
				currentVertexOffset++;
			}

			//Don't restart primitive, if it's last segment, rendering ends here
			if (i != _mainSegments - 1)
			{
				_indicesVBO.addRawData(&_primitiveRestartIndex, sizeof(GLuint));
			}
		}

		// Finally upload data to the GPU
		_vbo.bindVBO();
		_vbo.uploadDataToGPU(GL_STATIC_DRAW);

		setVertexAttributesPointers(numVertices);

		_indicesVBO.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
		_indicesVBO.uploadDataToGPU(GL_STATIC_DRAW);

		_isInitialized = true;
	}

	void Torus::render() const
	{
		if (!_isInitialized) {
			return;
		}
		
		glBindVertexArray(_vao);

		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(_primitiveRestartIndex);
		glDrawElements(GL_TRIANGLE_STRIP, _numIndices, GL_UNSIGNED_INT,0);
		glDisable(GL_PRIMITIVE_RESTART);
	}

	

} // namespace static_meshes_3D