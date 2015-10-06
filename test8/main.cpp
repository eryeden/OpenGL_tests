
//Widnowsの場合
#if (_MSC_VER == 1900)	 //Visual Studio 2015
#pragma comment(lib, "glfw3-vc140.lib")
#pragma comment(lib, "assimp-vc140.lib")
#endif

#if (_MSC_VER == 1800)	 //Visual Studio 2013
#pragma comment(lib, "glfw3-vc120")  // glfw3-vc120.lib
#pragma comment(lib, "assimp-vc120.lib")  //assimp-vc120.lib
#endif

#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GlU32.Lib")
#pragma comment(lib, "glew32.lib")

#include <gl/glew.h>
#define _CRT_SECURE_NO_WARNINGS
#endif

#define GL_GLEXT_PROTOTYPES
//GLEWの拡張を使うには此れ↑が必要（glGenBuffers等）
//Linuxではこれだけで良い　おそらくGLEWのライブラリ不必要
//http://stackoverflow.com/questions/3032386/glgenbuffers-not-defined
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

//#include "gli/gli.hpp"
//#include "texture.hpp"

//Model Loader Library
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Importer.hpp"
#include "controls.hpp"

//家の場合
//#define KIKUTI_HOME

void gl_execute(GLFWwindow *window);
void _update_fps_counter(GLFWwindow * window);

bool parse_file_into_str(
	const char* file_name, char* shader_str, int max_len
	);



int main() {

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow * window = glfwCreateWindow(640, 480, "Hello Triangle",
		NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	//GLEWを使う場合
#ifdef _MSC_VER
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("Version: %s\n", version);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	gl_execute(window);

	glfwTerminate();
	return 0;
}


void gl_execute(GLFWwindow *window) {

#ifdef KIKUTI_HOME
	GLuint shader_nontex_id = LoadShaders("C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/StandardShading_vs_non_tex.glsl"
		, "C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/StandardShading_fs_non_tex.glsl");
	GLuint shader_usetex_id = LoadShaders("C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/StandardShading_vs.glsl"
		, "C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/StandardShading_fs.glsl");

	GLuint shader_postprocess = LoadShaders(
		"C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/pp_vs.glsl"
		, "C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/pp_fs.glsl"
		);

	GLuint shader_fxaa_id = LoadShaders(
		"C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/fxaa_vs.glsl"
		, "C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/fxaa_fs.glsl"
		);

#else
	GLuint shader_nontex_id = LoadShaders("C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/StandardShading_vs_non_tex.glsl"
		, "C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/StandardShading_fs_non_tex.glsl");
	GLuint shader_usetex_id = LoadShaders("C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/StandardShading_vs.glsl"
		, "C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/StandardShading_fs.glsl");

	//ポストプロセシング用シェーダー
	GLuint shader_postprocess_id = LoadShaders(
		"C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/pp_vs.glsl"
		, "C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/pp_fs.glsl"
		);
	GLuint shader_fxaa_id = LoadShaders(
		"C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/fxaa_vs.glsl"
		, "C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/fxaa_fs.glsl"
		);
#endif


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);



	mat4 Model = mat4(1.0f); // 単位行列
	Model = mat4(
		1.0f, 0.0f, 0.0f, 0.0f
		, 0.0f, 1.0f, 0.0f, 0.0f
		, 0.0f, 0.0f, 1.0f, 0.0f
		, 0.0f, 0.0f, 0.0f, 1.0f
		);
	mat4 View = lookAt(
		vec3(9, 9, 15),
		vec3(0, 0, 0),
		vec3(0, 1, 0)
		);
	//どこからどこを見るか　ワールド座標において
	mat4 Projection = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); //glPerspective

	mat4 MVP = Projection * View * Model; //これをGLSLに渡す

	//シェーダ中のMVPへのハンドルを取得
	GLuint MatrixID_MVP = glGetUniformLocation(shader_nontex_id, "MVP");
	//GLuint TextureID = glGetUniformLocation(shader_programme_id, "my_texture_sampler");
	GLuint MatrixID_V = glGetUniformLocation(shader_nontex_id, "V");
	GLuint MatrixID_M = glGetUniformLocation(shader_nontex_id, "M");
	GLuint Vector3ID_LightPosition = glGetUniformLocation(shader_nontex_id, "LightPosition_worldspace");
	vec3 lightPos = vec3(0, 40, 0);

	MdlImporter imptr(shader_nontex_id, shader_usetex_id);
	MdlImporter imptr1(shader_nontex_id, shader_usetex_id);

	/*
		黄色：山吹？ vec3(1, 0.85, 0)
			  FANUC? vec3(0.964, 0.714, 0)
	*/
#ifdef KIKUTI_HOME
	imptr.Load("C:/Users/ery/Documents/local_devel/OpenGL_tests/models/t1.stl", vec3(0.8, 0.2, 0.4)); //#cc00000
	imptr1.Load("C:/Users/ery/Documents/local_devel/OpenGL_tests/models/Asm.stl", vec3(0, 0.71, 0.101));
	//imptr.Load("C:/Users/ery/Documents/local_devel/OpenGL_tests/models/cube1.obj", vec3(1, 1, 1)); //#cc00000
#else
	imptr.Load("C:/Users/B4/Source/Repos/OpenGL_tests/models/stoadmm.stl", vec3(0.964, 0.714, 0)); //#cc00000
	imptr1.Load("C:/Users/B4/Source/Repos/OpenGL_tests/models/Asm.stl", vec3(0, 0.71, 0.101));
	//imptr.Load("C:\Users\B4\Source\Repos\OpenGL_tests\models/cube1.obj", vec3(1, 1, 1)); //#cc00000
	//imptr.Load("C:/Users/B4/OneDrive/Documents/Models/PersonalComputer.stl", vec3(0, 0.8, 0.8)); //#cc00000

#endif
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	//#######################INITIALIZE POSTPROCESS##############################
	/*
		テクスチャ、レンダバッファ、FBOの初期化を行う
	*/
	// ---------------------------------------------
	// Render to Texture - specific code begins here
	// ---------------------------------------------

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The texture we're going to render to
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" means "empty" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

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
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	//// Depth texture alternative : 
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);


	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

								   // Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("FB failed\n");
		//return false;


	// The fullscreen quad's FBO
	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	GLuint quad_vertexbuffer;
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	// Create and compile our GLSL program from the shaders
	//GLuint quad_programID = LoadShaders("pp_vs.glsl", "pp_fs.glsl");
	GLuint texID = glGetUniformLocation(shader_postprocess_id, "renderedTexture");

	GLuint texID_fxaa = glGetUniformLocation(shader_fxaa_id, "renderedTexture");
	GLuint uniform_w = glGetUniformLocation(shader_fxaa_id, "rt_w");
	GLuint uniform_h = glGetUniformLocation(shader_fxaa_id, "rt_h");







	//#######################INITIALIZE POSTPROCESS##############################





	while (!glfwWindowShouldClose(window)) {
		_update_fps_counter(window);

		glfwGetWindowSize(window, &width, &height);


		//############################ON RESHAPE POSTPROCESS################################
		//スクリーンサイズ変更時にテクスチャサイズ、深度バッファ（レンダバッファ）サイズの変更が必要
		// On reshape
		//テクスチャのリサイズ
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		//レンダバッファ（深度バッファ）のリサイズ
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//############################ON RESHAPE POSTPROCESS################################


		//レンダリング開始時にFBOをバインドする。
		//そうするとレンダバッファではなくて、テクスチャに色フレームが描画されるようになる
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_nontex_id);

		Projection = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f); //glPerspective

		Model = mat4(
			1.0f, 0.0f, 0.0f, 0.0f
			, 0.0f, 1.0f, 0.0f, 0.0f
			, 0.0f, 0.0f, 1.0f, 0.0f
			, 0.0f, 0.0f, 0.0f, 1.0f
			);
		//MVP = Projection * View * Model; //これをGLSLに渡す

		computeMatricesFromInputs(window);
		Projection = getProjectionMatrix();
		View = getViewMatrix();
		MVP = Projection * View * Model;

		glUniformMatrix4fv(MatrixID_MVP, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(MatrixID_M, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv(MatrixID_V, 1, GL_FALSE, &View[0][0]);

		lightPos = vec3(4, 4, 4);
		glUniform3f(Vector3ID_LightPosition, lightPos.x, lightPos.y, lightPos.z);

		imptr.Render();

		Model = mat4(
			1.0f, 0.0f, 0.0f, 0.0f
			, 0.0f, 1.0f, 0.0f, 0.0f
			, 0.0f, 0.0f, 1.0f, 0.0f
			, 0.0f, 0.0f, 3.0f, 1.0f
			);
		MVP = Projection * View * Model; //これをGLSLに渡す
		glUniformMatrix4fv(MatrixID_MVP, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(MatrixID_M, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv(MatrixID_V, 1, GL_FALSE, &View[0][0]);

		lightPos = vec3(5, 5, 5);
		glUniform3f(Vector3ID_LightPosition, lightPos.x, lightPos.y, lightPos.z);

		imptr1.Render();



		//レンダリング終了時にFBOのバインドを解除する
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, width, height);

		//ここからポストプロセシング
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//ポストプロセスに使用するシェーダーを起動
		glUseProgram(shader_postprocess_id);
		//glUseProgram(shader_fxaa_id);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		
		glUniform1i(texID, 0);

		//glUniform1i(texID_fxaa, 0);
		//glUniform1f(uniform_w, (float)width);
		//glUniform1f(uniform_h, (float)height);

		//頂点バッファ
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glVertexAttribPointer(
			0
			, 3
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void *)0
			);
		 //描画
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);

		
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}

	}

	//リソースの開放
	glDeleteFramebuffers(1, &FramebufferName);
	glDeleteTextures(1, &renderedTexture);
	glDeleteRenderbuffers(1, &depthrenderbuffer);
	glDeleteBuffers(1, &quad_vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
}


void _update_fps_counter(GLFWwindow * window) {
	static double previous_seconds = glfwGetTime();
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;
	if (elapsed_seconds > 0.25) {
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		sprintf(tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}
	frame_count++;
}

