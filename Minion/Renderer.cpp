#include "Renderer.h"

Renderer* renderer = NULL;


Renderer::Renderer()
{
	isMinimized = false;

	// Frame buffer objects
	screenOutputFBO = 0;
	screenOutputTexture = 0;
	guiFBO = 0;
	guiTexture = 0;
	
	glGenFramebuffers(1, &screenOutputFBO);
	glGenFramebuffers(1, &guiFBO);


	// Rendering settings
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	ExitOnGLError("ERROR: Could not set OpenGL depth testing options");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	ExitOnGLError("ERROR: Could not set OpenGL culling options");


	// The fullscreen quad's FBO
	GLuint quad_VertexArrayID;
	glGenVertexArrays(1, &quad_VertexArrayID);
	glBindVertexArray(quad_VertexArrayID);

	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	
	quad_vertexbuffer = 0;
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	
	// Build Shader Programs
	unique_ptr<Shader> _ppShader(new Shader(SHADER_POSTPROCESSING_VERT_FILE, SHADER_POSTPROCESSING_FRAG_FILE));
	ppShader = move(_ppShader);
	unique_ptr<Shader> _bcShader(new Shader(SHADER_BUFFER_COMBINE_VERT_FILE, SHADER_BUFFER_COMBINE_FRAG_FILE));
	bcShader = move(_bcShader);
	unique_ptr<Shader> _guiShader(new Shader(SHADER_GUI_VERT_FILE, SHADER_GUI_FRAG_FILE));
	guiShader = move(_guiShader);
}


Renderer::~Renderer(void)
{
	// Release loaded assets from memory	
	glDeleteTextures(1, &screenOutputTexture);

	glDeleteFramebuffers(1, &screenOutputFBO);
}


void Renderer::renderScene()
{
	if (!isMinimized) {
		// Clear the back buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		// Render scene here
		renderGUI();

		renderToScreen();
	}
}


void Renderer::renderGUI() 
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glViewport(0, 0, width, height);
	glDisable(GL_DEPTH_TEST);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, guiFBO);
	glUseProgram(guiShader->programId);

	for (auto comp : guiComponents) {

		auto texture = comp->getTexture();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getID());
		glUniform1i(glGetUniformLocation(guiShader->programId, SU_GUI_TEXTURE), 0);

		comp->render(&(*guiShader));
	}

	glDisable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);

	glEnable(GL_DEPTH_TEST);
	ExitOnGLError("ERROR: failed to render gui.");
}


void Renderer::renderToScreen() 
{
	// Run buffer combining shader to generate final image
	glBindFramebuffer(GL_FRAMEBUFFER, screenOutputFBO);
	glUseProgram(bcShader->programId);

	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	glUniform1i(glGetUniformLocation(bcShader->programId, SU_BC_DIFFUSE_TEXTURE), 0);
	//drawScreenQuad();

	// Post Processing + Render to the screen
	glUseProgram(ppShader->programId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenOutputTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, guiTexture);
	glUniform1i(glGetUniformLocation(ppShader->programId, SU_PP_SOURCE_TEXTURE), 0);
	glUniform1i(glGetUniformLocation(ppShader->programId, SU_PP_GUI_TEXTURE), 1);
	glUniform2f(glGetUniformLocation(ppShader->programId, SU_PP_FRAME_SIZE), (GLfloat)width, (GLfloat)height);
	ExitOnGLError("ERROR: failed to render post processing step.");

	// Final draw to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	drawScreenQuad();
	ExitOnGLError("ERROR: failed to render final screen draw.");

	glUseProgram(0);
}


void Renderer::resizeWindow(int windowWidth, int windowHeight)
{
	ExitOnGLError("ERROR: Broke somewhere before resize.");
	if (windowWidth == 0)
		return;

	width = windowWidth;
	height = windowHeight;

	/*
	camera->Projection = glm::perspective(
		Settings.FOVY,
		((float)Settings.ResolutionX) / Settings.ResolutionY,
		0.1f,	// Distance to Near Plane
		Settings.DrawDistance
		);
	*/

	// Rebuild frame buffers
	//------------------------------------------------------------------------	
	glDeleteTextures(1, &screenOutputTexture);
	glDeleteTextures(1, &guiTexture);
	ExitOnGLError("ERROR: Could not clear framebuffer texture objects.");

	GLenum buffers[3];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;
	buffers[2] = GL_COLOR_ATTACHMENT2;

	// Generate our scene textures ... 
	generateScreenTexture(screenOutputTexture);
	generateScreenTexture(guiTexture);

	// And now attach them to our FBOs 	
	glBindFramebuffer(GL_FRAMEBUFFER, screenOutputFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenOutputTexture, 0);
	glDrawBuffers(1, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ExitOnError("screenOutputFBO incomplete");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, guiFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, guiTexture, 0);
	glDrawBuffers(1, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ExitOnError("guiFBO incomplete");
	}

	ExitOnGLError("ERROR: Framebuffers broke on resize.");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::drawScreenQuad()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glViewport(0, 0, width, height);
	glDisable(GL_DEPTH_TEST);

	// Draw quad
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
		0,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
}


void Renderer::generateScreenTexture(GLuint & into, bool depth, int width, int height)
{
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		depth ? GL_DEPTH_COMPONENT : GL_RGBA16,
		(width) ? width : this->width,
		(height) ? height : this->height,
		0,
		depth ? GL_DEPTH_COMPONENT : GL_RGBA,
		GL_UNSIGNED_BYTE,
		NULL
		);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void Renderer::add(GuiComponent* component) 
{
	guiComponents.push_back(component);
}