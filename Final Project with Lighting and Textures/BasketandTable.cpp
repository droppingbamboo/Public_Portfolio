/*Primary Source file
//Brionne Boggioni
//CS-330-x6373*/


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <iostream>

#include "cylinder.h"
#include "torus.h"


struct GLMesh
{
	GLuint vao;         // Handle for the vertex array object
	GLuint vbos[2];     // Handles for the vertex buffer objects
	GLuint nIndices;    // Number of indices of the mesh
};

GLMesh gMesh;
GLMesh pyramidBase;
GLMesh planeMesh;
GLMesh inkBox1;
GLMesh inkBox2;
GLMesh inkBox3;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void UCreateMesh(GLMesh& mesh);
void UCreateCubeMesh(GLMesh& mesh);
void UCreatePyramidBaseMesh(GLMesh& mesh);
void UCreatePlaneMesh(GLMesh& mesh);
void UCreateInkBox1Mesh(GLMesh& mesh);
void UCreateInkBox2Mesh(GLMesh& mesh);
void UCreateInkBox3Mesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
bool UCreateTexture(const char* filename, GLuint& textureId);
void UDestroyTexture(GLuint textureId);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 6.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Initialization of view values
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
GLuint viewType = 1;

//texture id's
GLuint topTextureId;
GLuint sideTextureId;
GLuint bottomTextureId;
GLuint tableTextureId; 
GLuint inkCylinderTopTextureId;
GLuint inkCylinderBaseTextureId;
GLuint inkCylinderTextureId;
GLuint penSidesTextureId;
GLuint penTopTextureId;
GLuint inkBox1TextureId;
GLuint inkBox2TextureId;
GLuint inkBox3TextureId;
GLuint yarnTextureId;

glm::vec2 gUVScale(1.0f, 1.0f);
glm::vec2 basketUVScale(5.0f, 5.0f);
GLint gTexWrapMode = GL_REPEAT;


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float cameraSpeed = 0.5f;
//Pyramid and light color

glm::vec3 gLightColor(1.0f, 1.0f, 1.0f);

//lamp positions and scale
glm::vec3 fillLampPosition(5.0f, 1.0f, 0.0f);
glm::vec3 fillLampScale(0.5f, 0.5f, 0.5f);
glm::vec3 fillLampColor(0.239f, 0.635f, 0.858f);

glm::vec3 keyLampPosition(-5.0f, 3.0f, 0.0f);
glm::vec3 keyLampScale(0.5f, 0.5f, 0.5f);
glm::vec3 keyLampColor(1.0f, 1.0f, 1.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Final Tableau", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader objectShader("LightedVertexShader.vs", "LightedFragmentShader.fs");
	
	
	//Initialize Meshes for Pyramid, Pyramid Base and Table 

	UCreateMesh(gMesh);
	UCreatePyramidBaseMesh(pyramidBase);
	UCreatePlaneMesh(planeMesh);
	UCreateInkBox1Mesh(inkBox1);
	UCreateInkBox2Mesh(inkBox2);
	UCreateInkBox3Mesh(inkBox3);
	
    //Initialize textures

	const char* texFilename1 = "resizedBasketTop.jpg";
	if (!UCreateTexture(texFilename1, topTextureId))
	{
		std::cout << "Failed to load texture " << texFilename1 << std::endl;
		return EXIT_FAILURE;
	}

	const char* texFilename2 = "resizedBasketSides.jpg";
	if (!UCreateTexture(texFilename2, sideTextureId))
	{
		std::cout << "Failed to load texture " << texFilename2 << std::endl;
		return EXIT_FAILURE;
	}

	const char* texFilename3 = "resizedBasketBase.jpg";
	if (!UCreateTexture(texFilename3, bottomTextureId))
	{
		std::cout << "Failed to load texture " << texFilename3 << std::endl;
		return EXIT_FAILURE;
	}

	const char* texFilename4 = "resizedTable.jpg";
	if (!UCreateTexture(texFilename4, tableTextureId))
	{
		std::cout << "Failed to load texture " << texFilename4 << std::endl;
		return EXIT_FAILURE;
	}

	const char* texFilename5 = "resizedInkCylinderTop.jpg";
	if (!UCreateTexture(texFilename5, inkCylinderTopTextureId))
	{
		std::cout << "Failed to load texture " << texFilename5 << std::endl;
		return EXIT_FAILURE;
	}

	const char* texFilename6 = "resizedInkBase.jpg";
	if (!UCreateTexture(texFilename6, inkCylinderBaseTextureId))
	{
		std::cout << "Failed to load texture " << texFilename6 << std::endl;
		return EXIT_FAILURE;
	}

	const char* texFilename7 = "mergedInkSides.jpg";
	if (!UCreateTexture(texFilename7, inkCylinderTextureId))
	{
		std::cout << "Failed to load texture " << texFilename7 << std::endl;
		return EXIT_FAILURE;
	}

	const char* texFilename8 = "abaloneSheet.jpg";
	if (!UCreateTexture(texFilename8, penSidesTextureId))
	{
		std::cout << "Failed to load texture " << texFilename8 << std::endl;
		return EXIT_FAILURE;
	}

	const char* texFilename9 = "penBottom.jpg";
	if (!UCreateTexture(texFilename9, penTopTextureId))
	{
		std::cout << "Failed to load texture " << texFilename9 << std::endl;
		return EXIT_FAILURE;
	}

	const char* texFilename10 = "resizedInkBoxFront.jpg";
	if (!UCreateTexture(texFilename10, inkBox1TextureId))
	{
		std::cout << "Failed to load texture " << texFilename10 << std::endl;
		return EXIT_FAILURE;
	}

	const char* texFilename11 = "resizedInkBox2.jpg";
	if (!UCreateTexture(texFilename11, inkBox2TextureId))
	{
		std::cout << "Failed to load texture " << texFilename11 << std::endl;
		return EXIT_FAILURE;
	}

	const char* texFilename12 = "resizedInkBox3.jpg";
	if (!UCreateTexture(texFilename12, inkBox3TextureId))
	{
		std::cout << "Failed to load texture " << texFilename12 << std::endl;
		return EXIT_FAILURE;
	}

	const char* texFilename13 = "resizedYarn.jpg";
	if (!UCreateTexture(texFilename13, yarnTextureId))
	{
		std::cout << "Failed to load texture " << texFilename13 << std::endl;
		return EXIT_FAILURE;
	}

	objectShader.use();
	objectShader.setInt("topTextureId", 0);
	objectShader.setInt("sideTextureId", 1);
	objectShader.setInt("bottomTextureId", 2);
	objectShader.setInt("tableTextureId", 3);
	objectShader.setInt("inkCylinderTopTextureId", 3);
	objectShader.setInt("inkCylinderBaseTextureId", 4);
	objectShader.setInt("inkCylinderTextureId", 5);
	objectShader.setInt("penSidesTextureId", 6);
	objectShader.setInt("penTopTextureId", 7);
	objectShader.setInt("inkBox1TextureId", 8);
	objectShader.setInt("inkBox2TextureId", 9);
	objectShader.setInt("inkBox3TextureId", 10);
	objectShader.setInt("yarnTextureId", 11);

	
	
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);
		glm::mat4 model;
		float angle;
		// render
		// ------
		glClearColor(0.482f, 0.474f, 0.454f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		// pass projection matrix to shader (note that in this case it could change every frame)

		if (viewType == 1)
		{
			glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			objectShader.setMat4("projection", projection);

			
		}
		else if (viewType == 2)
		{
			glm::mat4 projection = glm::ortho(-8.0f, 8.0f, -8.0f, 8.0f, 0.1f, 100.0f);
			objectShader.setMat4("projection", projection);

			
		}

		// camera/view transformation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		objectShader.setMat4("view", view);

	

		//Send uniforms to shader for light sources
		
		
		GLint keyLightColorLoc = glGetUniformLocation(objectShader.ID, "keyLightColor");
		GLint keyLightPositionLoc = glGetUniformLocation(objectShader.ID, "keyLightPos");
		GLint fillLightColorLoc = glGetUniformLocation(objectShader.ID, "fillLightColor");
		GLint fillLightPositionLoc = glGetUniformLocation(objectShader.ID, "fillLightPos");
		GLint viewPositionLoc = glGetUniformLocation(objectShader.ID, "viewPosition");

		
		
		glUniform3f(keyLightColorLoc, keyLampColor.r, keyLampColor.g, keyLampColor.b);
		glUniform3f(keyLightPositionLoc, keyLampPosition.r, keyLampPosition.g, keyLampPosition.b);
		glUniform3f(fillLightColorLoc, fillLampColor.r, fillLampColor.g, fillLampColor.b);
		glUniform3f(fillLightPositionLoc, fillLampPosition.r, fillLampPosition.g, fillLampPosition.b);
		glUniform3f(viewPositionLoc, cameraPos.x, cameraPos.y, cameraPos.z);

		GLint UVScaleLoc = glGetUniformLocation(objectShader.ID, "uvScale");
		glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));
        /*Begin setting model information for objects in scene
		Pyramid - Basket handle
		Basket Cylinder - is the Basket
		Basket top and base is rendered separately for texture
		Ink Cylinder is the plastic container of inks
		Pen cylinder
		Ink box
		Yarn torus*/
		
		//***************************Pyramid************************************************

		//Render Pyramid
		// activate shader
		objectShader.use();

		//Render PyramidBase
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, topTextureId);		

		//model for Pyramid Base - top portion of handle
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 0.375f, 0.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		objectShader.setMat4("model", model);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(pyramidBase.vao);
		// Draws the triangles
		glDrawElements(GL_TRIANGLES, pyramidBase.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangles

		

		//Render Actual Pyramid
		objectShader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sideTextureId);

		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 0.375f, 0.0f));
		angle = 180.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		objectShader.setMat4("model", model);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(gMesh.vao);
		// Draws the triangles
		glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangles

		//****************************Pyramid*****************************************************
		
		//****************************Basket Top & Base******************************************************
		//Render Cylinder top with correct texture
		objectShader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bottomTextureId);

		//model for Cylinder top
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		objectShader.setMat4("model", model);

		static_meshes_3D::Cylinder basketCylinder(0.4, 30, 0.8, true, true, true);
		basketCylinder.renderTop();



		//Render Basket Cylinder base with correct texture
		objectShader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bottomTextureId);

		//model for Cylinder base
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		objectShader.setMat4("model", model);

		basketCylinder.renderBase();

		//**************************Basket Top & Base*********************************************

		//**************************Ink Container*************************************************
		//Render Ink Cylinder top;
		static_meshes_3D::Cylinder inkCylinder(0.2, 30, 0.4, true, true, true);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inkCylinderTopTextureId);

		//model for Ink Cylinder top
		model = glm::mat4(1.0f); //make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3(0.5f, -0.2f, 0.5f));
		objectShader.setMat4("model", model);

		inkCylinder.renderTop();

		//Render ink Cylinder bottom
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inkCylinderBaseTextureId);

		//model for Ink Cylinder top
		model = glm::mat4(1.0f); //make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3(0.5f, -0.2f, 0.5f));
		objectShader.setMat4("model", model);

		inkCylinder.renderBase();

		//Render ink Cylinder 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inkCylinderTextureId);

		//model for Ink Cylinder top
		model = glm::mat4(1.0f); //make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3(0.5f, -0.2f, 0.5f));
		objectShader.setMat4("model", model);

		inkCylinder.render();
        
		//***********************Ink Container***************************************************

		//***********************Abalone Pen*****************************************************
		//model for Pen top
		static_meshes_3D::Cylinder penCylinder(0.05, 30, 0.9, true, true, true);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, penTopTextureId);

		model = glm::mat4(1.0f); //make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3(-0.5f, -0.325, 0.5f));
		angle = -90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.25, 1.0f));
		objectShader.setMat4("model", model);

		penCylinder.renderTop();

		//Render pen bottom
		

		//model for pen bottom
		model = glm::mat4(1.0f); //make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3(-0.5f, -0.325f, 0.5f));
		angle = -90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.25f, 1.0f));
		objectShader.setMat4("model", model);

		penCylinder.renderBase();

		//Render pen
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, penSidesTextureId);

		//model for Pen
		model = glm::mat4(1.0f); //make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3(-0.5f, -0.325f, 0.5f));
		angle = -90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.25f, 1.0f));
		objectShader.setMat4("model", model);

		penCylinder.render();

		//**********************Abalone Pen**************************************************

		//**********************Table********************************************************
		//Render Table with correct texture

		objectShader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tableTextureId);

		//model for Table


		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-0.3f, -.40f, 0.45f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::scale(model, glm::vec3(1.5f, 0.2f, .8f));

		objectShader.setMat4("model", model);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(planeMesh.vao);
		// Draws the triangles
		glDrawElements(GL_TRIANGLES, planeMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangles
		
		//********************Table***********************************************************
		

		//********************Ink Box********************************************************
		objectShader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inkBox1TextureId);

		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 0.1f));
		angle = 25.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.125f));
		objectShader.setMat4("model", model);


		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(inkBox1.vao);
		// Draws the triangles
		glDrawElements(GL_TRIANGLES, inkBox1.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangles

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inkBox2TextureId);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 0.1f));
		angle = 25.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.125f));
		objectShader.setMat4("model", model);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(inkBox2.vao);
		// Draws the triangles
		glDrawElements(GL_TRIANGLES, inkBox2.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangles


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inkBox3TextureId);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 0.1f));
		angle = 25.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25, 0.5f, 0.125f));
		objectShader.setMat4("model", model);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(inkBox3.vao);
		// Draws the triangles
		glDrawElements(GL_TRIANGLES, inkBox3.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangles

		//***********************Ink Box******************************************************************

		//**********************Yarn Ball****************************************************************

		objectShader.use();

		static_meshes_3D::Torus yarnTorus(20, 20, 0.5f, 0.725f, true, true, true);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, yarnTextureId);

		model = glm::mat4(1.0f); //make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3(0.5f, 0.18f, 0.5f));
		angle = -90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		
		objectShader.setMat4("model", model);

		yarnTorus.render();

		//********************Yarn Ball******************************************************************

		//********************************Basket Cylinder Sides*******************************************
		//Rendered separately to use a different UVscale

		// Render Cylinder
	

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sideTextureId);

		GLint cylinderUVScaleLoc = glGetUniformLocation(objectShader.ID, "uvScale");
		glUniform2fv(cylinderUVScaleLoc, 1, glm::value_ptr(basketUVScale));

		//model for Cylinder
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		
		objectShader.setMat4("model", model);

		

		basketCylinder.render();

		

	

	    // Deactivate the Vertex Array Object
		glBindVertexArray(0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		if (viewType == 1)
		{
			viewType = 2;
		}
		else if (viewType == 2)
		{
			viewType = 1;
		}
	}
}
// Implements the UCreateMesh function
void UCreateMesh(GLMesh& mesh)
{
	//Calculate Normal Vector for each face (base normal vector will be (0.0f, -1.0f, 0.0f)
   // Faces are defined as: Apex-A-B, Apex-B-C, Apex-C-D, Apex-D-A
   // Vectors ApexA, ApexB, ApexC and ApexD need to be found first for relevant cross products
	glm::vec3 apex = glm::vec3(0.0f, 0.5f, 0.0f);

	glm::vec3 pointA = glm::vec3(0.0f, -0.75f, -0.75f);
	glm::vec3 pointB = glm::vec3(0.75f, -0.75f, 0.0f);
	glm::vec3 pointC = glm::vec3(0.0f, -0.75f, 0.75f);
	glm::vec3 pointD = glm::vec3(-0.75f, -0.75f, 0.0f);

	glm::vec3 vectorApexA = (apex - pointA);
	glm::vec3 vectorApexB = (apex - pointB);
	glm::vec3 vectorApexC = (apex - pointC);
	glm::vec3 vectorApexD = (apex - pointD);

	auto normalAB = glm::cross(vectorApexA, vectorApexB);
	auto normalBC = glm::cross(vectorApexB, vectorApexC);
	auto normalCD = glm::cross(vectorApexC, vectorApexD);
	auto normalDA = glm::cross(vectorApexD, vectorApexA);

	// Position and Color data
	GLfloat verts[] = {
		//For lighting purposes, all faces will need to be render independently and not share vertices

		// Vertex Positions    // Texture Coor.  //Normal Coordinates
		//FaceAB
		 0.0f,  0.5f,   0.0f,    0.5f,   1.0f, normalAB.x, normalAB.y, normalAB.z,// Vertex 0, ApexAB 

		 0.0f, -0.75f, -0.75f,   1.0f,   0.0f, normalAB.x, normalAB.y, normalAB.z,// Vertex 1, Base Point A.ab 
		 0.75f, -0.75f,  0.0f,   0.0f,   0.0f, normalAB.x, normalAB.y, normalAB.z, // Vertex 2, Base Point B.ab

		//FaceBC
		 0.0f,  0.5f,   0.0f,    0.5f,   1.0f, normalBC.x, normalBC.y, normalBC.z, //Vertex 3, ApexBC
		 0.75f, -0.75f,  0.0f,   1.0f,   0.0f, normalBC.x, normalBC.y, normalBC.z, // Vertex 4, Base Point B.bc
		 0.0f,  -0.75f,  0.75f,  0.0f,   0.0f, normalBC.x, normalBC.y, normalBC.z, // Vertex 5, Base Point C.bc

		 //FaceCD
		 0.0f,  0.5f,   0.0f,    0.5f,   1.0f, normalCD.x, normalCD.y, normalCD.z, //Vertex 6, Apex CD
		 0.0f, -0.75f,  0.75f,   1.0f,   0.0f, normalCD.x, normalCD.y, normalCD.z, //Vertex 7, Base Point C.cd
		-0.75f, -0.75f,  0.0f,   0.0f,   0.0f, normalCD.x, normalCD.y, normalCD.z, //Vertex 8, Base Point D.cd

		//FaceDA
		0.0f,  0.5f,    0.0f,   0.5f,    1.0f, normalDA.x, normalDA.y, normalDA.z, //Vertex 9, Apex DA
		-0.75f, -0.75f, 0.0f,   1.0f,    0.0f, normalDA.x, normalDA.y, normalDA.z, //Vertex 10, Base Point D.da
		 0.0f,  -0.75f, -0.75f, 0.0f,    0.0f, normalDA.x, normalDA.y, normalDA.z, //Vertex 11, Base Point A.da 

		//Vertexes for base   // Texture Coordinates
		 0.0f, -0.75f, -0.75f,   1.0f,   1.0f,  0.0f, -1.0f, 0.0f,                  //Vertex 12, Base Point A_T
		 0.75f, -0.75f, 0.0f,    1.0f,   0.0f,  0.0f, -1.0f, 0.0f,                  //Vertex 13 Base Point B_T
		 0.0f,  -0.75f, 0.75f,   0.0f,   0.0f,  0.0f, -1.0f, 0.0f,                  //Vertex 14, Base Point C_T
		 -0.75f, -0.75f, 0.0f,   0.0f,   1.0f,  0.0f, -1.0f, 0.0f                  //Vertex 15, Base Point D_T
	};

	// Index data to share position data
	GLushort indices[] = {
		//Draw Base First
		12, 13, 14,       //Triangle A_T - B_T - C_T
		12,15, 14,       //Triangle B_T - C_T - D_T

		0, 1, 2,         //Triangle Apex-A-B
		3, 4, 5,         //Triangle Apex-B-C
		6, 7, 8,         //Triangle Apex-C-D
		9, 10, 11        //Triangle Apex-D-A

	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerTexCoord = 2;
	const GLuint floatsPerNormal = 3;


	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(2, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	mesh.nIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates is 8 (x, y, z, s, t, n.x, n.y, n.z). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerTexCoord + floatsPerNormal);// The number of floats before each

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerTexCoord, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerTexCoord)));
	glEnableVertexAttribArray(2);


}
// Implements the UCreateMesh function
void UCreatePyramidBaseMesh(GLMesh& mesh)
{
	// Position and Color data
	GLfloat verts[] = {
		// Vertex Positions     //Texture Coords //Normals   
		 0.0f, 0.75f, -0.75f,   0.06f,  1.0f,    0.0f, 1.0f, 0.0f,// Vertex 1, Base Point A
		 0.75f, 0.75f,  0.0f,   0.83f,  1.0f,    0.0f, 1.0f, 0.0f, // Vertex 2, Base Point B
		 0.0f, 0.75f,  0.75f,   0.83f,  0.25f,   0.0f, 1.0f, 0.0f, // Vertex 3, Base Point C
		-0.75f, 0.75f,  0.0f,   0.06f,  0.25f,   0.0f, 1.0f, 0.0f// Vertex 4, Base Point D 
	};

	// Index data to share position data
	GLushort indices[] = {
		
		0, 1, 2,  // Triangle 6 - A - B - C 
		0, 2, 3 // Triangle 5 - A - C - D
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerTexCoord = 2;
	const GLuint floatsPerNormal = 3;

	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(2, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	mesh.nIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates is 8 (x, y, z, s, t, n.x, n.y, n.z). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerTexCoord + floatsPerNormal);// The number of floats before each

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerTexCoord, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerTexCoord)));
	glEnableVertexAttribArray(2);
}

void UCreatePlaneMesh(GLMesh& mesh)
{
	// Position and Color data
	GLfloat verts[] = {
		// Vertex Positions    // texture coords.   //Normals 

		//Top
		 -1.0f,  0.0f, -1.0f,  0.0f, 1.0f,          0.0f, 1.0f, 0.0f, // Vertex 0, Top Surface A
		  1.0f,  0.0f, -1.0f,  1.0f, 1.0f,          0.0f, 1.0f, 0.0f, // Vertex 1, Top Surface B 
		  1.0f,  0.0f,  1.0f,  1.0f, 0.0f,          0.0f, 1.0f, 0.0f, // Vertex 2, Top Surface C
		 -1.0f,  0.0f,  1.0f,  0.0f, 0.0f,          0.0f, 1.0f, 0.0f, // Vertex 3, Top Surface D

        //Bottom
		 -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,          0.0f, -1.0f, 0.0f, // Vertex 4, Bottom Surface E
		  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,          0.0f, -1.0f, 0.0f, // Vertex 5, Bottom Surface F
		  1.0f, -1.0f,  1.0f,  0.0f, 1.0f,          0.0f, -1.0f, 0.0f, // Vertex 6, Bottom Surface G
		 -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,          0.0f, -1.0f, 0.0f, // Vertex 7, Bottom Surface H 

		 //Right Side
		 1.0f,   0.0f,  1.0f,  0.0f, 1.0f,          1.0f,  0.0f, 0.0f, // Vertex 8, Right Side Surface C
		 1.0f,   0.0f, -1.0f,  1.0f, 1.0f,          1.0f,  0.0f, 0.0f, // Vertex 9, Right Side Surface B
		 1.0f,  -1.0f, -1.0f,  0.0f, 1.0f,          1.0f,  0.0f, 0.0f, // Vertex 10, Right Side Surface F
		 1.0f,  -1.0f,  1.0f,  0.0f, 0.0f,          1.0f,  0.0f, 0.0f, // Vertex 11, Right Side Surface G

		 //Front 
        -1.0f,   0.0f,  1.0f,  0.0f, 1.0f,          0.0f,  0.0f, 1.0f, // Vertex 12, Front Surface D
		 1.0f,   0.0f,  1.0f,  1.0f, 1.0f,          0.0f,  0.0f, 1.0f, // Vertex 13, Front Surface C
         1.0f,  -1.0f,  1.0f,  0.0f, 1.0f,          0.0f,  0.0f, 1.0f, // Vertex 14, Front Surface G
        -1.0f,  -1.0f,  1.0f,  0.0f, 0.0f,          0.0f,  0.0f, 1.0f, // Vertex 15, Front Surface H

		//Left Side
	   -1.0f,   0.0f,  -1.0f,  0.0f, 1.0f,         -1.0f,  0.0f, 0.0f, // Vertex 16, Front Surface A
	   -1.0f,   0.0f,   1.0f,  0.0f, 0.0f,         -1.0f,  0.0f, 0.0f, // Vertex 17, Front Surface D
	   -1.0f,  -1.0f,   1.0f,  0.0f, 1.0f,         -1.0f,  0.0f, 0.0f, // Vertex 18, Front Surface H
	   -1.0f,  -1.0f,   1.0f,  0.0f, 0.0f,         -1.0f,  0.0f, 0.0f, // Vertex 19, Front Surface E

	   //Back
	   -1.0f,   0.0f,  -1.0f,  0.0f, 1.0f,          0.0f,  0.0f, -1.0f, // Vertex 20, Back Surface A
	    1.0f,   0.0f,  -1.0f,  1.0f, 1.0f,          0.0f,  0.0f, -1.0f, // Vertex 21, Back Surface B
		1.0f,  -1.0f,  -1.0f,  0.0f, 1.0f,          0.0f,  0.0f, -1.0f, // Vertex 22, Back Surface F
       -1.0f,  -1.0f,  -1.0f,  0.0f, 0.0f,          0.0f,  0.0f, -1.0f, // Vertex 23, Back Surface E 
	};

	// Index data to share position data
	GLushort indices[] = {
		//Sides
		8, 9, 10, 
		8, 10,11, 

		12, 13, 14,
		12, 15, 14,

		16, 17, 18,
		17, 16, 19,

		20, 21, 22, 
		20, 23, 22, 

		//Top
		0, 1, 2,
		0, 3, 2, 

		//Bottom
		4, 5, 6, 
		4, 7, 6
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerTexCoord = 2;
	const GLuint floatsPerNormal = 3;

	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(2, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	mesh.nIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates is 8 (x, y, z, s, t, n.x, n.y, n.z). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerTexCoord + floatsPerNormal);// The number of floats before each

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerTexCoord, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerTexCoord)));
	glEnableVertexAttribArray(2);
}
void UCreateInkBox1Mesh(GLMesh& mesh)
{
	// Position and Color data
	GLfloat verts[] = {
		// Vertex Positions    // texture coords.   //Normals 
        //Front 
		-1.0f,   0.0f,  1.0f,  0.0f, 1.0f,          0.0f,  0.0f, 1.0f, // Vertex 0
		 1.0f,   0.0f,  1.0f,  1.0f, 1.0f,          0.0f,  0.0f, 1.0f, // Vertex 1
		 1.0f,  -1.0f,  1.0f,  1.0f, 0.0f,          0.0f,  0.0f, 1.0f, // Vertex 2
		-1.0f,  -1.0f,  1.0f,  0.0f, 0.0f,          0.0f,  0.0f, 1.0f, // Vertex 3
			
	};

	// Index data to share position data
	GLushort indices[] = {
		//Front
		0, 1, 2,
		0, 2, 3,
			
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerTexCoord = 2;
	const GLuint floatsPerNormal = 3;

	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(2, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	mesh.nIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates is 8 (x, y, z, s, t, n.x, n.y, n.z). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerTexCoord + floatsPerNormal);// The number of floats before each

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerTexCoord, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerTexCoord)));
	glEnableVertexAttribArray(2);
}

void UCreateInkBox2Mesh(GLMesh& mesh)
{
	// Position and Color data
	GLfloat verts[] = {
		// Vertex Positions    // texture coords.   //Normals 

		//Left Side
	   -1.0f,   0.0f,  -1.0f,  0.0f, 1.0f,         -1.0f,  0.0f, 0.0f, // Vertex 0, 
	   -1.0f,   0.0f,   1.0f,  1.0f, 1.0f,         -1.0f,  0.0f, 0.0f, // Vertex 1,
	   -1.0f,  -1.0f,   1.0f,  1.0f, 0.0f,         -1.0f,  0.0f, 0.0f, // Vertex 2, 
	   -1.0f,  -1.0f,  -1.0f,  0.0f, 0.0f,         -1.0f,  0.0f, 0.0f, // Vertex 3,

	   //Back
	   -1.0f,   0.0f,  -1.0f,  1.0f, 1.0f,          0.0f,  0.0f, -1.0f, // Vertex 4, 
		1.0f,   0.0f,  -1.0f,  0.0f, 1.0f,          0.0f,  0.0f, -1.0f, // Vertex 5, 
		1.0f,  -1.0f,  -1.0f,  0.0f, 0.0f,          0.0f,  0.0f, -1.0f, // Vertex 6, 
	   -1.0f,  -1.0f,  -1.0f,  1.0f, 0.0f,          0.0f,  0.0f, -1.0f, // Vertex 7,  
	};

	// Index data to share position data
	GLushort indices[] = {
	
		//Left Side
		0, 1, 2,
		0, 3, 2,

		//Back
		4, 5, 6,
		4, 7, 6
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerTexCoord = 2;
	const GLuint floatsPerNormal = 3;

	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(2, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	mesh.nIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates is 8 (x, y, z, s, t, n.x, n.y, n.z). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerTexCoord + floatsPerNormal);// The number of floats before each

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerTexCoord, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerTexCoord)));
	glEnableVertexAttribArray(2);
}

void UCreateInkBox3Mesh(GLMesh& mesh)
{
	// Position and Color data
	GLfloat verts[] = {
		// Vertex Positions    // texture coords.   //Normals 

		//Top
		 -1.0f,  0.0f, -1.0f,  0.0f, 1.0f,          0.0f, 1.0f, 0.0f, // Vertex 0, Top Surface A
		  1.0f,  0.0f, -1.0f,  1.0f, 1.0f,          0.0f, 1.0f, 0.0f, // Vertex 1, Top Surface B 
		  1.0f,  0.0f,  1.0f,  1.0f, 0.0f,          0.0f, 1.0f, 0.0f, // Vertex 2, Top Surface C
		 -1.0f,  0.0f,  1.0f,  0.0f, 0.0f,          0.0f, 1.0f, 0.0f, // Vertex 3, Top Surface D

		//Bottom
		 -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,          0.0f, -1.0f, 0.0f, // Vertex 4, Bottom Surface E
		  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,          0.0f, -1.0f, 0.0f, // Vertex 5, Bottom Surface F
		  1.0f, -1.0f,  1.0f,  0.0f, 1.0f,          0.0f, -1.0f, 0.0f, // Vertex 6, Bottom Surface G
		 -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,          0.0f, -1.0f, 0.0f, // Vertex 7, Bottom Surface H 

		 //Right Side
		 1.0f,   0.0f,  1.0f,  0.0f, 1.0f,          1.0f,  0.0f, 0.0f, // Vertex 8, Right Side Surface C
		 1.0f,   0.0f, -1.0f,  1.0f, 1.0f,          1.0f,  0.0f, 0.0f, // Vertex 9, Right Side Surface B
		 1.0f,  -1.0f, -1.0f,  0.0f, 1.0f,          1.0f,  0.0f, 0.0f, // Vertex 10, Right Side Surface F
		 1.0f,  -1.0f,  1.0f,  0.0f, 0.0f,          1.0f,  0.0f, 0.0f, // Vertex 11, Right Side Surface G
		 		
	};

	// Index data to share position data
	GLushort indices[] = {
		//Right Side
		8, 9, 10,
		8, 10,11,

		//Top
		0, 1, 2,
		0, 3, 2,

		//Bottom
		4, 5, 6,
		4, 7, 6
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerTexCoord = 2;
	const GLuint floatsPerNormal = 3;

	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(2, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	mesh.nIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates is 8 (x, y, z, s, t, n.x, n.y, n.z). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerTexCoord + floatsPerNormal);// The number of floats before each

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerTexCoord, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerTexCoord)));
	glEnableVertexAttribArray(2);
}
void UCreateCubeMesh(GLMesh& mesh)
{
	// Position data
	GLfloat verts[] = {
		// Vertex Positions   
		 -1.0f,  0.0f, -1.0f,  // Vertex 0, Top Surface A
		  1.0f,  0.0f, -1.0f,  // Vertex 1, Top Surface B 
		  1.0f,  0.0f,  1.0f,  // Vertex 2, Top Surface C
		 -1.0f,  0.0f,  1.0f,  // Vertex 3, Top Surface D

		 -1.0f, -1.0f, -1.0f,  // Vertex 4, Bottom Surface E
		  1.0f, -1.0f, -1.0f,  // Vertex 5, Bottom Surface F
		  1.0f, -1.0f,  1.0f,  // Vertex 6, Bottom Surface G
		 -1.0f, -1.0f,  1.0f,  // Vertex 7, Bottom Surface H 
	};

	// Index data to share position data
	GLushort indices[] = {
		//Sides
		0, 1, 5,  // Triangle 1 - ABF
		0, 4, 5,  // Triangle 2 - AEF
		1, 6, 5,  // Triangle 3 - BGF
		1, 2, 5,  // Triangle 4 - BCF
		2, 6, 7,  // Triangle 5 - CGH
		2, 3, 7,  // Triangle 6 - CDH
		3, 0, 7,  // Triangle 7 - DAH
		0, 4, 7,  // Triangle 8 - AEH
		//Top
		0, 1, 2,  //Triangle 9 - ABC
		0, 2, 3,  //Triangle 10 - ACD
		//Bottom
		4, 5, 6,  //Triangle 11 - EFH
		4, 6, 7   //Triangle 12 - EGH
	};

	const GLuint floatsPerVertex = 3;

	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(2, mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	mesh.nIndices = sizeof(indices) / sizeof(indices[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, 3 * (sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);


}

void UDestroyMesh(GLMesh& mesh)
{
	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(2, mesh.vbos);
}
/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint& textureId)
{
	int width, height, channels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
	if (image)
	{


		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << channels << " channels" << std::endl;
			return false;
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		return true;
	}

	// Error loading the image
	return false;
}
void UDestroyTexture(GLuint textureId)
{
	glGenTextures(1, &textureId);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameraSpeed += yoffset;
	std::cout << "CameraSpeed: " << cameraSpeed << std::endl;
}