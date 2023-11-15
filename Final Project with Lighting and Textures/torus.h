#pragma once
#include "staticMeshIndexed3D.h"

namespace static_meshes_3D {

	/**
	* Torus static mesh with given radius, number of slices and height.
	*/
	class Torus : public StaticMeshIndexed3D
	{
	public:
		Torus(int mainSegments, int tubeSegments, float mainRadius, float tubeRadius,
			bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

		void render() const override;
		void renderSpecial(int segments) const;
		
		/**
		 * Gets torus main segments.
		 */
		float getMainSegments() const;

		/**
		 * Gets number of tube segments.
		 */
		int getTubeSegments() const;

		/**
		 * Gets main torus radius.
		 */
		float getMainRadius() const;

		/**
		 * Gets tube torus radius.
		 */
		float getTubeRadius() const;

	private:
		int _mainSegments; // number of main segments in Torus
		int _tubeSegments; // Number of tube segments in Torus
		float _mainRadius; // main Radius of Torus
		float _tubeRadius; // tube Radius of Torus

		int _numIndices = 0; // Indices to render Torus
		int _primitiveRestartIndex = 0; // How many vertices to render top / bottom of the cylinder
		

		void initializeData() override;
	};

} // namespace static_meshes_3D#pragma once#pragma once
