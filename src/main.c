#include <stdint.h>
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <GL/gl3w.h>

#include <stb_image.h>

#include "sys/cpu.h"

#include "core/simple_string.h"
#include "core/filesystem.h"
#include "gl/shader.h"

#include "math/mat4f.h"

uint32_t setupWindow(GLFWwindow**, uint32_t width, uint32_t height);

int main()
{
	if (!cpu_checkTSCSupport()) {
		printf("CPU lacks TSC support\n");
		return 1;
	}
	if (!cpu_checkTSCInvariant()) {
		printf("TSC is not invariant\n");
		return 1;
	}

	if (!glfwInit()) {
		printf("Failed to init GLFW\n");
		return 2;
	}

	GLFWwindow* window;
	uint32_t error = setupWindow(&window, 1280, 720);
	if (error) {
		return error;
	}

	string32_t vert_path, frag_path;
	ss_init32_c8(&vert_path, "res/shaders/block_vert.glsl");
	ss_init32_c8(&frag_path, "res/shaders/block_frag.glsl");

	string8_t vert_code, frag_code;
	ss_init8(&vert_code);
	ss_init8(&frag_code);

	fs_readFile8(&vert_code, &vert_path);
	fs_readFile8(&frag_code, &frag_path);

	shader_t shader;
	gls_initShader(&shader);
	gls_attachStage(&shader, vert_code.c_str, GL_VERTEX_SHADER);
	gls_attachStage(&shader, frag_code.c_str, GL_FRAGMENT_SHADER);
	gls_linkStages(&shader);

	int32_t width, height, channels;
	uint8_t* image = stbi_load("res/textures/test.png", &width, &height, &channels, 0);

	uint32_t texID;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image);

	float vertices[] = {
		 0.5f,  0.5f,  0.0f,  				// top right
		 0.5f, -0.5f,  0.0f,  				// bottom right
		-0.5f, -0.5f,  0.0f, 	 			// bottom left
		-0.5f,  0.5f,  0.0f,      			// top left
    };
	float texCoords[] = {
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};
	unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

	unsigned int VBO, TBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // vertices
	glGenBuffers(1, &TBO); // texCoords
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)) );
	glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 3 * sizeof(float) );
    glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(2 * sizeof(float)) );
	glVertexArrayVertexBuffer(VAO, 1, TBO, 0, 2 * sizeof(float) );
    glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

	glfwShowWindow(window);
	while (!glfwWindowShouldClose(window))
	{
		// update

		// nothing to see here

		// render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gls_use(&shader);
		{
			glBindTexture(GL_TEXTURE_2D, texID);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// cycle
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

static void glfw_error_callback(int32_t code, const char* description)
{
	printf("GLFW ERROR %i\n%s\n", code, description);
}

static uint32_t setupWindow(GLFWwindow** window, uint32_t width, uint32_t height)
{
	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	*window = glfwCreateWindow(width, height, "Vox", NULL, NULL);
	if (!*window) {
		glfwTerminate();
		return 2;
	}

	glfwMakeContextCurrent(*window);

	if (gl3wInit()) {
		printf("Error loading OpenGL\n");
		return 3;
	}

	return 0;
}