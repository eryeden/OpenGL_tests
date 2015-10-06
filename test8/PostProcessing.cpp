#include "PostProcessing.hpp"
#include "shader.hpp"
PostProcessing::PostProcessing(const GLuint & _shader_fxaa_id, const GLuint & _shader_passthrough_id) {

	shader_fxaa = _shader_fxaa_id;
	shader_passthrough = _shader_passthrough_id;

	Initialize();
}

PostProcessing::PostProcessing(
	const std::string & _path_to_vertexshader_fxaa
	, const std::string & _path_to_fragmentshader_fxaa
	, const std::string & _path_to_vertexshader_passthrough
	, const std::string & _path_to_fragmentshader_passthrough
	) {

	shader_fxaa = LoadShaders(_path_to_vertexshader_fxaa.c_str(), _path_to_fragmentshader_fxaa.c_str());
	shader_passthrough = LoadShaders(_path_to_vertexshader_passthrough.c_str(), _path_to_fragmentshader_passthrough.c_str());

	Initialize();
}

PostProcessing::~PostProcessing() {

}

void PostProcessing::Reshape(
	GLsizei width
	, GLsizei height) {
	//スクリーンサイズ変更時にテクスチャサイズ、深度バッファ（レンダバッファ）サイズの変更が必要
	// On reshape
	//テクスチャのリサイズ
	glBindTexture(GL_TEXTURE_2D, texturebuffer_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	//レンダバッファ（深度バッファ）のリサイズ
	glBindRenderbuffer(GL_RENDERBUFFER, render_depthbuffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

}

void PostProcessing::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object_id);
	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

void PostProcessing::DeBind() {

}

void PostProcessing::Render() {

}


void PostProcessing::Initialize() {
	GLfloat g_quad_vertex_buffer_data[]
		= {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint framebuffer_object_id = 0;
	glGenFramebuffers(1, &framebuffer_object_id);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object_id);

	// The texture we're going to render to
	GLuint texturebuffer_id;
	glGenTextures(1, &texturebuffer_id);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texturebuffer_id);

	// Give an empty image to OpenGL ( the last "0" means "empty" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The depth buffer
	GLuint render_depthbuffer_id;
	glGenRenderbuffers(1, &render_depthbuffer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, render_depthbuffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_depthbuffer_id);

	//// Alternative : Depth texture. Slower, but you can sample it later in your shader
	//GLuint depthTexture;
	//glGenTextures(1, &depthTexture);
	//glBindTexture(GL_TEXTURE_2D, depthTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, 1024, 768, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texturebuffer_id, 0);

	//// Depth texture alternative : 
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);


	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

								   // Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("FB failed\n");
	//return false;

	GLuint quad_vertexbuffer_id;
	glGenBuffers(1, &quad_vertexbuffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);


	handler_texture_passthrough = glGetUniformLocation(shader_passthrough, "renderedTexture");

	handler_texture_faxx = glGetUniformLocation(shader_fxaa, "renderedTexture");
	hundler_width_screen = glGetUniformLocation(shader_fxaa, "rt_w");
	hundler_height_screen = glGetUniformLocation(shader_fxaa, "rt_h");

}


//#define USE_POSTPROCESSING

int PostProcessing::execute() {

#ifdef USE_POSTPROCESSING

	//#####################INITIALIZE####################################
	/* Texture */
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &fbo_texture);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	//端が重なる？これを回避するためフィルタを適用する
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D
		, 0, GL_RGBA
		, s_width, s_height
		, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);  //テクスチャバインド解除

	//Depth buffer	レンダバッファの生成
	glGenRenderbuffers(1, &rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, s_width, s_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//テクスチャ、レンダバッファをVBOにバインドする
	//おそらくこれによりレンダリング結果がテクスチャバッファ、レンダバッファに書きこまれるようになる
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "glCheckFramebufferStatus: error %p", status);
		return 0;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//#####################INITIALIZE####################################


	//############################ON RESHAPE################################
		//スクリーンサイズ変更時にテクスチャサイズ、深度バッファ（レンダバッファ）サイズの変更が必要
		// On reshape
		//テクスチャのリサイズ
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s_width, s_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	//レンダバッファ（深度バッファ）のリサイズ
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, s_width, s_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//############################ON RESHAPE################################

	//############################FREE RESOURCES################################
	//リソースの開放
	glDeleteRenderbuffers(1, &rbo_depth);
	glDeleteTextures(1, &fbo_texture);
	glDeleteFramebuffers(1, &fbo);
	//############################FREE RESOURCES################################


	/*
		レンダリング結果を格納したテクスチャを乗っけるための頂点を用意する
		これは２Dエフェクトをテクスチャに適用するためのもので、そのための頂点座標も2次元となる
	*/
	GLuint vbo_fbo_vertices; //テクスチャ表示に使用するVBO
	//頂点の初期化
	GLfloat fbo_vertices[] = {
		  -1, -1
		,  1, -1
		, -1,  1
		,  1,  1
	};
	glGenBuffers(1, &vbo_fbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //バインド解除

	//VBOの開放
	glDeleteBuffers(1, &vbo_fbo_vertices);

	//描画について
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program_postproc);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glUniform1i(uniform_fbo_texture, /*GL_TEXTURE*/0);
	glEnableVertexAttribArray(attribute_v_coord_postproc);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
	glVertexAttribPointer(
		attribute_v_coord_postproc,  // attribute
		2,                  // number of elements per vertex, here (x,y)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(attribute_v_coord_postproc);

	glfwSwapBuffers();

#endif //USE_POSTPROCESSING

	return 0;
}












