#include "PostProcessing.hpp"

GLuint fbo;	// Frame Buffer Object
GLuint fbo_texture;	//FBOにバインドされるテクスチャ
GLuint rbo_depth;   //レンダーバッファオブジェクト、深度バッファが書き込まれる

GLsizei s_width;
GLsizei s_height;

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

}












