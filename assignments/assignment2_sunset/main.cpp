#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <slib/shader.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

struct Vertex {
	float x, y, z;
	float u, v;
};

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

Vertex vertices[4] = {
	//x     y    z    u    v
	{-1.0, -1.0, 0.0, 0.0, 0.0}, //Bottom left
	{1.0, -1.0, 0.0, 1.0, 0.0}, //Bottom right
	{1.0, 1.0, 0.0, 1.0, 1.0},  //Top right
	{-1.0, 1.0, 0.0, 0.0, 1.0}  //Top left
};

unsigned int indices[6] = {
	0, 1, 2, // Triangle 1
	2, 3, 0 // Triangle 2
};

//Setting base variables for fragment shader
float bgColorDay1[3] = {1.0,.7,.34};
float bgColorDay2[3] = {.75,.54,.85};
float bgColorNight1[3] = {.71,.52,.84};
float bgColorNight2[3] = {.08,.04,.38};
float sunColor[3] = {.99,1.0,.1};
float hillColor[3] = {0.1, 0.1, 0.1};
float sunSpeed = 1.0f;
float sunRadius = 0.4f;

bool showImGUIDemoWindow = true;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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

	//Build the shader object from personal library and vao
	slib::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	unsigned int vao = createVAO(vertices, 4, indices, 6);

	//Start the shader program
	shader.use();
	//Bind the vao
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		shader.setFloat("iTime", (float)glfwGetTime());
		shader.setVec2("iResolution", SCREEN_WIDTH, SCREEN_HEIGHT);
		shader.setVec3("bgColorDay1", bgColorDay1[0], bgColorDay1[1], bgColorDay1[2]);
		shader.setVec3("bgColorDay2", bgColorDay2[0], bgColorDay2[1], bgColorDay2[2]);
		shader.setVec3("bgColorNight1", bgColorNight1[0], bgColorNight1[1], bgColorNight1[2]);
		shader.setVec3("bgColorNight2", bgColorNight2[0], bgColorNight2[1], bgColorNight2[2]);
		shader.setVec3("sunColor", sunColor[0], sunColor[1], sunColor[2]);
		shader.setVec3("hillColor", hillColor[0], hillColor[1], hillColor[2]);
		shader.setFloat("sunSpeed", sunSpeed);
		shader.setFloat("sunRadius", sunRadius);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();


			//All sliders for adjustable values
			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("bgColorDay1", bgColorDay1);
			ImGui::ColorEdit3("bgColorDay2", bgColorDay2);
			ImGui::ColorEdit3("bgColorNight1", bgColorNight1);
			ImGui::ColorEdit3("bgColorNight2", bgColorNight2);
			ImGui::ColorEdit3("sunColor", sunColor);
			ImGui::ColorEdit3("hillColor", hillColor);
			ImGui::SliderFloat("Sun Speed", &sunSpeed, 0.0f, 10.0f);
			ImGui::SliderFloat("Sun Radius", &sunRadius, 0.0f, 2.0f);

			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices)
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 5, vertexData, GL_STATIC_DRAW);
	
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vbo;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}