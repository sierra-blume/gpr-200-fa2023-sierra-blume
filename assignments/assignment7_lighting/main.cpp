#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

float prevTime;
ew::Vec3 bgColor = ew::Vec3(0.1f);

ew::Camera camera;
ew::CameraController cameraController;

struct Light {
	ew::Vec3 position; //World space
	ew::Vec3 color; //RGB
};

struct Material {
	float ambientK; //Ambient coefficient (0-1)
	float diffuseK; //Diffuse coefficient (0-1)
	float specular; //Specular coefficient (0-1)
	float shine; //Shininess
};

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/defaultLit.vert", "assets/defaultLit.frag");
	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg",GL_REPEAT,GL_LINEAR);

	ew::Shader unlit("assets/unlit.vert", "assets/unlit.frag");

	//Create cube
	ew::Mesh cubeMesh(ew::createCube(1.0f));
	ew::Mesh planeMesh(ew::createPlane(5.0f, 5.0f, 10));
	ew::Mesh sphereMesh(ew::createSphere(0.5f, 64));
	ew::Mesh cylinderMesh(ew::createCylinder(0.5f, 1.0f, 32));
	ew::Mesh lightMesh[4];
	for (int i = 0; i < 4; i++)
	{
		lightMesh[i] = (ew::createSphere(0.4f, 20));
	}

	//Initialize transforms
	ew::Transform cubeTransform;
	ew::Transform planeTransform;
	ew::Transform sphereTransform;
	ew::Transform cylinderTransform;
	ew::Transform lightTransform[4];
	planeTransform.position = ew::Vec3(0, -1.0, 0);
	sphereTransform.position = ew::Vec3(-1.5f, 0.0f, 0.0f);
	cylinderTransform.position = ew::Vec3(1.5f, 0.0f, 0.0f);

	Light light[4];
	for (int i = 0; i < 4; i++) {
		lightTransform[i].position.x = cos(((ew::PI * 2) / 4) * i) * 2;
		lightTransform[i].position.y = 2;
		lightTransform[i].position.z = sin(((ew::PI * 2) / 4) * i) * 2;
		light[i].position = lightTransform[i].position;
	}
	light[0].color = ew::Vec3(1.0);
	light[1].color = ew::Vec3(1.0);
	light[2].color = ew::Vec3(1.0);
	light[3].color = ew::Vec3(1.0);

	resetCamera(camera,cameraController);

	int numLights = 1;
	Material material;
	material.ambientK = 0.1;
	material.diffuseK = 0.5;
	material.specular = 1.0;
	material.shine = 50.0;
	float orbitRad = 2;
	bool orbit = false;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;
		
		if (orbit == true)
		{
			for (int i = 0; i < 4; i++)
			{
				lightTransform[i].position = ew::Vec3(cos(((ew::PI * 2) / numLights) * i + time) * orbitRad, 2, sin(((ew::PI * 2) / numLights) * (i) + time) * orbitRad);
				light[i].position = lightTransform[i].position;
			}
		}
		//Update camera
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);

		//RENDER
		glClearColor(bgColor.x, bgColor.y,bgColor.z,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		shader.setInt("_Texture", 0);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		shader.setVec3("cameraPos", camera.position);
		shader.setInt("numLights", numLights);
		shader.setFloat("vAmbient", material.ambientK);
		shader.setFloat("vDiffuse", material.diffuseK);
		shader.setFloat("vSpecular", material.specular);
		shader.setFloat("vShine", material.shine);

		for (int i = 0; i < numLights; i++)
		{
			shader.setVec3("_Lights[" + std::to_string(i) + "].position", light[i].position);
			shader.setVec3("_Lights[" + std::to_string(i) + "].color", light[i].color);
		}

		//Draw shapes
		shader.setMat4("_Model", cubeTransform.getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw();

		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw();

		shader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw();

		//TODO: Render point lights
		for (int i = 0; i < numLights; i++)
		{
			shader.setVec3("_Light.position", light[i].position);
			shader.setVec3("_Light.color", light[i].color);
			unlit.use();
			unlit.setMat4("_Model", lightTransform[i].getModelMatrix());
			unlit.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
			unlit.setVec3("_Color", light[i].color);
			lightMesh[i].draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Camera")) {
				if (ImGui::CollapsingHeader("Non Shading Stuff")) {
					ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
						ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
						ImGui::Checkbox("Orthographic", &camera.orthographic);
						if (camera.orthographic) {
							ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
						}
						else {
							ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
						}
					ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
						ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
					ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
					ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
					if (ImGui::Button("Reset")) {
						resetCamera(camera, cameraController);
					}
				}
				if (ImGui::CollapsingHeader("Shading")) {
					ImGui::ColorEdit3("BG color", &bgColor.x);
					ImGui::SliderInt("Num Lights", &numLights, 1, 4);
					ImGui::Checkbox("Orbit Lights", &orbit);
					ImGui::SliderFloat("Orbit Radius", &orbitRad, 2, 10);
				}
			}

			for (int i = 0; i < numLights; i++) {
				if (ImGui::CollapsingHeader(("Light " + std::to_string(i + 1)).c_str())) {
					ImGui::DragFloat3("Position", &light[i].position.x, 0, -10.0, 10.0);
					ImGui::ColorEdit3(("Light " + std::to_string(i + 1) + " Color").c_str(), &light[i].color.x);
				}
			}

			if (ImGui::CollapsingHeader("Material")) {
				ImGui::SliderFloat("AmbientK", &material.ambientK, 0, 1);
				ImGui::SliderFloat("DiffuseK", &material.diffuseK, 0, 1);
				ImGui::SliderFloat("SpecularK", &material.specular, 0, 1);
				ImGui::SliderFloat("Shininess", &material.shine, 0, 1000);
			}

			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0);
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}