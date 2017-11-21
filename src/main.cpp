// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Skybox.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>

// Properties
GLuint screenWidth = 1280, screenHeight = 720;
GLfloat theta = 0;
GLfloat alpha = 0;
bool enableKey = true;
bool enabledoor = false;
bool sirenSignal = true;
glm::vec3 lightPos(0.0f, 3.6f, 0.0f);

// GLfloat amb1 = 0.4f, amb2 = 1.0f, amb3 = 1.0f;
// GLfloat dif1 = 1.0f, dif2 = 1.0f, dif3 = 1.0f;
// GLfloat spec1 = 0.4f, spec2 = 0.5f, spec3 = 0.6f;
GLfloat amb1 = 0.4f, amb2 = 0.4f, amb3 = 0.5f;
GLfloat spec1 = 0.6f, spec2 = 0.6f, spec3 = 0.7f;
GLfloat dif1 = 0.4f, dif2 = 0.6f, dif3 = 0.8f;
GLfloat shi1 = 32.0f;

GLfloat colorOffsetR = 1.0f;
GLfloat colorOffsetG = 1.0f;
GLfloat colorOffsetB = 1.0f;
GLfloat siren = 0.5f;

glm::vec3 lightColor(colorOffsetR, colorOffsetG, colorOffsetB);

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
int Do_actions();

// Camera
Camera camera(glm::vec3(4.0f, -0.9f, 0.0f));
bool keys[1024];
bool firstMouse = true;
GLfloat lastX, lastY;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int KEY = -1;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
// Initializing Window
    // Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "inJail", nullptr, nullptr); // Windowed

    glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Skybox
float skyboxVertices[] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
};

// Shaders - - - -

	// Setup and compile our shaders
	Shader lightingShader("resources/shaders/lighting.vs", "resources/shaders/lighting.frag");
  Shader lampShader("resources/shaders/lamp.vs", "resources/shaders/lamp.frag");
	Shader cubeShader("resources/shaders/cube.vs", "resources/shaders/cube.frag");
	Shader skyboxShader("resources/shaders/skybox.vs", "resources/shaders/skybox.frag");

// skybox

// skybox VAO
unsigned int skyboxVAO, skyboxVBO;
glGenVertexArrays(1, &skyboxVAO);
glGenBuffers(1, &skyboxVBO);
glBindVertexArray(skyboxVAO);
glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

// load textures
// -------------
vector<std::string> faces
{
		"resources/skybox/FullMoonRight2048.png",
		"resources/skybox/FullMoonLeft2048.png",
		"resources/skybox/FullMoonUp2048.png",
		"resources/skybox/FullMoonDown2048.png",
		"resources/skybox/FullMoonBack2048.png",
		"resources/skybox/FullMoonFront2048.png"
};

unsigned int cubemapTexture = loadCubemap(faces);

// shader configuration

skyboxShader.Use();
skyboxShader.setInt("skybox", 0);

// Models - - - -


	while(KEY!=GLFW_KEY_ENTER)
	{
	    glfwSetWindowTitle(window, "Home");

        glfwPollEvents();
	    // Clear the colorbuffer
	    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
	}

    glfwSetWindowTitle(window, "Project Helios");

    // Load Models
    Model Map("resources/models/prisao.obj");
    Model Island("resources/models/Small_Tropical_Island.obj");

    Model Lamp("resources/models/Lamp.obj");
    Model Lamp2("resources/models/Lamp.obj");
    Model Lamp3("resources/models/Lamp.obj");
    Model Lamp4("resources/models/Lamp.obj");
    Model Lamp5("resources/models/Lamp.obj");
    Model Lamp6("resources/models/Lamp.obj");
    Model Lamp7("resources/models/Lamp.obj");

		Model Closeddoor("resources/models/closed-door.obj");
		Model Openeddoor("resources/models/opened-door.obj");
  	Model Battery("resources/models/Huge Battery.obj");

// 		__  __ __    ___   ____   ____  ___ ___   ___       ____    ___       __    __  __ __  ____  _        ___
//    /  ]|  |  |  /  _] /    | /    ||   |   | /   \     |    \  /   \     |  |__|  ||  |  ||    || |      /  _]
//   /  / |  |  | /  [_ |   __||  o  || _   _ ||     |    |  _  ||     |    |  |  |  ||  |  | |  | | |     /  [_
//  /  /  |  _  ||    _]|  |  ||     ||  \_/  ||  O  |    |  |  ||  O  |    |  |  |  ||  _  | |  | | |___ |    _]
// /   \_ |  |  ||   [_ |  |_ ||  _  ||   |   ||     |    |  |  ||     |    |  `  '  ||  |  | |  | |     ||   [_
// \     ||  |  ||     ||     ||  |  ||   |   ||     |    |  |  ||     |     \      / |  |  | |  | |     ||     |
//  \____||__|__||_____||___,_||__|__||___|___| \___/     |__|__| \___/       \_/\_/  |__|__||____||_____||_____|

	// Game loop
	while(!glfwWindowShouldClose(window))
	{
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();


        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        lampShader.Use();

        // Transformation matrices
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"),  lightColor.x, lightColor.y, lightColor.z);


        lightingShader.Use();

        // Set lights properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.color"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.1f, 0.1f, 0.1f);

        // Transformation matrices
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

				Do_actions();
			// 	____   ___   ____  ______   ____
			//  |    \ /   \ |    \|      | /    |
			//  |  o  )     ||  D  )      ||  o  |
			//  |   _/|  O  ||    /|_|  |_||     |
			//  |  |  |     ||    \  |  |  |  _  |
			//  |  |  |     ||  .  \ |  |  |  |  |
			//  |__|   \___/ |__|\_| |__|  |__|__|

				if(!enabledoor)
				{
					glm::mat4 model_closeddoor;

					model_closeddoor = glm::translate(model_closeddoor, glm::vec3(0.0f, -2.5f, -1.24f));
					//model_closeddoor = glm::rotate(model_closeddoor, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

					lightingShader.Use();
					glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_closeddoor));
					glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), amb1, amb2, amb3);
					glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), dif1, dif2, dif3);
					glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), spec1, spec2, spec3);
					glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), shi1);
					Closeddoor.Draw(lightingShader);
				}
				else
				{


					lightColor = glm::vec3(siren, 0.2f, 0.2f);

					if(sirenSignal)
						if(siren < 1.0f)
							siren += 0.01f;
						else
							sirenSignal = false;
				 else
					 if(siren > 0.3f)
						siren -= 0.01f;
					 else
						sirenSignal = true;


					glm::mat4 model_openeddoor;

					model_openeddoor = glm::translate(model_openeddoor, glm::vec3(0.0f, -2.5f, -1.15f));
					//model_openeddoor = glm::rotate(model_openeddoor, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));


					lightingShader.Use();
					glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_openeddoor));
					glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), amb1, amb2, amb3);
					glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), dif1, dif2, dif3);
					glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), spec1, spec2, spec3);
					glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), shi1);
					Openeddoor.Draw(lightingShader);
				}

// 	___      ___  _____   ___  ____   __ __   ____  ____   ___     ___
// |   \    /  _]/ ___/  /  _]|    \ |  |  | /    ||    \ |   \   /   \
// |    \  /  [_(   \_  /  [_ |  _  ||  |  ||  o  ||  _  ||    \ |     |
// |  D  ||    _]\__  ||    _]|  |  ||  _  ||     ||  |  ||  D  ||  O  |
// |     ||   [_ /  \ ||   [_ |  |  ||  |  ||  _  ||  |  ||     ||     |
// |     ||     |\    ||     ||  |  ||  |  ||  |  ||  |  ||     ||     |
// |_____||_____| \___||_____||__|__||__|__||__|__||__|__||_____| \___/


        // Draw the loaded model
        glm::mat4 model_map;
        glm::mat4 model_island;
        glm::mat4 model_lamp;
        glm::mat4 model_lamp2;
        glm::mat4 model_lamp3;
        glm::mat4 model_lamp4;
        glm::mat4 model_lamp5;
        glm::mat4 model_lamp6;
        glm::mat4 model_lamp7;
				glm::mat4 model_battery;
			//
			// 	___   ____   ___  ____    ____    __   ___     ___  _____
			//  /   \ |    \ /  _]|    \  /    |  /  ] /   \   /  _]/ ___/
			// |     ||  o  )  [_ |  D  )|  o  | /  / |     | /  [_(   \_
			// |  O  ||   _/    _]|    / |     |/  /  |  O  ||    _]\__  |
			// |     ||  | |   [_ |    \ |  _  /   \_ |     ||   [_ /  \ |
			// |     ||  | |     ||  .  \|  |  \     ||     ||     |\    |
			//  \___/ |__| |_____||__|\_||__|__|\____| \___/ |_____| \___|
			//

				model_island = glm::translate(model_island, glm::vec3(26.0f, -5.0f, -15.0f));
				model_island = glm::scale(model_island, glm::vec3(0.10f, 0.10f, 0.10f));

				model_map = glm::translate(model_map, glm::vec3(0.0f, -2.5f, 0.0f));
				model_map = glm::rotate(model_map, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				model_lamp = glm::translate(model_lamp, glm::vec3(-5.0f, 0.0f, -2.0f));
				model_lamp = glm::rotate(model_lamp, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

				model_lamp2 = glm::translate(model_lamp2, glm::vec3(-16.0f, 0.0f, -5.2f));
				model_lamp2 = glm::rotate(model_lamp2, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

				model_lamp3 = glm::translate(model_lamp3, glm::vec3(-25.0f, 0.0f, -26.0f));
				model_lamp3 = glm::rotate(model_lamp3, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

				model_lamp4 = glm::translate(model_lamp4, glm::vec3(-30.0f, -0.3f, -7.6f));
				model_lamp4 = glm::rotate(model_lamp4, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

				model_lamp5 = glm::translate(model_lamp5, glm::vec3(-33.0f, -0.3f, -3.2f));
				model_lamp5 = glm::rotate(model_lamp5, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

				model_lamp6 = glm::translate(model_lamp6, glm::vec3(0.05f, -0.2f, -1.0f));
				model_lamp6 = glm::rotate(model_lamp6, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				model_lamp6 = glm::scale(model_lamp6, glm::vec3(0.3f, 0.3f, 0.3f));


				model_lamp7 = glm::translate(model_lamp7, glm::vec3(0.05f, -0.2f, -3.0f));
				model_lamp7 = glm::rotate(model_lamp7, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				model_lamp7 = glm::scale(model_lamp7, glm::vec3(0.3f, 0.3f, 0.3f));

				model_battery = glm::translate(model_battery, glm::vec3(-34.0f, -2.5f, -26.0f));
				model_battery = glm::rotate(model_battery, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp));
				Lamp.Draw(lampShader);

				lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp2));
				Lamp2.Draw(lampShader);

				lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp3));
				Lamp3.Draw(lampShader);

				lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp4));
				Lamp4.Draw(lampShader);

				lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp5));
				Lamp5.Draw(lampShader);

				lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp6));
				Lamp6.Draw(lampShader);

				lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp7));
				Lamp7.Draw(lampShader);

        lightingShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_map));
				glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), amb1, amb2, amb3);
				glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), dif1, dif2, dif3);
				glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), spec1, spec2, spec3);
				glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), shi1);
        Map.Draw(lightingShader);

        lightingShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_island));
				glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), amb1, amb2, amb3);
				glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), dif1, dif2, dif3);
				glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), spec1, spec2, spec3);
				glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), shi1);
        Island.Draw(lightingShader);

				lightingShader.Use();
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_battery));
				glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), amb1, amb2, amb3);
				glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), dif1, dif2, dif3);
				glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), spec1, spec2, spec3);
				glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), shi1);
				Battery.Draw(lightingShader);

        colorOffsetR = 0.5 * sin(glm::radians(alpha)) + 0.5;
        colorOffsetG = 0.5 * sin(glm::radians(2*alpha)) + 0.5;
        colorOffsetB = 0.5 * sin(glm::radians(4*alpha)) + 0.5;
        //lightColor = glm::vec3(colorOffsetR, colorOffsetG, colorOffsetB);

        if(alpha > 360)
            alpha = 0;
        else
            alpha += 0.05;

    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    skyboxShader.Use();
    view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix

    skyboxShader.setMat4("view", view);
    skyboxShader.setMat4("projection", projection);
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// _____  ____  ___ ___      ___     ___       __    __  __ __  ____  _        ___
// |     ||    ||   |   |    |   \   /   \     |  |__|  ||  |  ||    || |      /  _]
// |   __| |  | | _   _ |    |    \ |     |    |  |  |  ||  |  | |  | | |     /  [_
// |  |_   |  | |  \_/  |    |  D  ||  O  |    |  |  |  ||  _  | |  | | |___ |    _]
// |   _]  |  | |   |   |    |     ||     |    |  `  '  ||  |  | |  | |     ||   [_
// |  |    |  | |   |   |    |     ||     |     \      / |  |  | |  | |     ||     |
// |__|   |____||___|___|    |_____| \___/       \_/\_/  |__|__||____||_____||_____|
//

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    //GLfloat offsetX = 0.0;//01 * cos(glm::radians(theta));
    GLfloat offsetY = 0.0003 * sin(glm::radians(theta));

    if((keys[GLFW_KEY_W])||(keys[GLFW_KEY_S])||(keys[GLFW_KEY_A])||(keys[GLFW_KEY_D])){

        // Camera controls
        if(keys[GLFW_KEY_W])
            camera.ProcessKeyboard(FORWARD, deltaTime, offsetY);
        if(keys[GLFW_KEY_S])
            camera.ProcessKeyboard(BACKWARD, deltaTime, offsetY);
        if(keys[GLFW_KEY_A])
            camera.ProcessKeyboard(LEFT, deltaTime, offsetY);
        if(keys[GLFW_KEY_D])
            camera.ProcessKeyboard(RIGHT, deltaTime, offsetY);

        if(theta > 360)
            theta = 0;
        else
            theta += 0.8;
    }
}

int Do_actions()
{
	if( (camera.Position.x <= -32 && camera.Position.x >= -34) && (camera.Position.z <= -25 && camera.Position.z >= -27) && keys[GLFW_KEY_F])
	{
		std::cout << "ACTIVATE" << '\n';
		enabledoor = true;
		return 1;
	}

	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    KEY = key;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if((camera.Position.x > -7.5)&&(camera.Position.x < -5)&&(camera.Position.z > -5)&&(camera.Position.z < -3)&&(keys[GLFW_KEY_E])&&(!enableKey))
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
