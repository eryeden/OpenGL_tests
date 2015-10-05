
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
#include "texture.hpp"

//Model Loader Library
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Importer.hpp"

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


	//必要なのか？
	//GLuint VertexArrayID;
	//glGenVertexArrays(1, &VertexArrayID);
	//glBindVertexArray(VertexArrayID);



	//GLuint shader_programme_id = LoadShaders("C:/Users/B4/Source/Repos/OpenGL_tests/test6/test_vs.glsl", "C:/Users/B4/Source/Repos/OpenGL_tests/test6/test_fs.glsl");
   //GLuint shader_nontex_id = LoadShaders("C:/Users/B4/Source/Repos/OpenGL_tests/test6/test_vs.glsl", "C:/Users/B4/Source/Repos/OpenGL_tests/test6/test_fs.glsl");
   //GLuint shader_usetex_id = LoadShaders("C:/Users/B4/Source/Repos/OpenGL_tests/test6/test_vs.glsl", "C:/Users/B4/Source/Repos/OpenGL_tests/test6/test_fs.glsl");


#ifdef KIKUTI_HOME
	GLuint shader_nontex_id = LoadShaders("C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/StandardShading_vs_non_tex.glsl"
		, "C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/StandardShading_fs_non_tex.glsl");
	GLuint shader_usetex_id = LoadShaders("C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/StandardShading_vs.glsl"
		, "C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/StandardShading_fs.glsl");
#else
	GLuint shader_nontex_id = LoadShaders("C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/StandardShading_vs_non_tex.glsl"
		, "C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/StandardShading_fs_non_tex.glsl");
	GLuint shader_usetex_id = LoadShaders("C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/StandardShading_vs.glsl"
		, "C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/StandardShading_fs.glsl");
#endif

//#if (_MSC_VER == 1900)	 //Visual Studio 2015
//	GLuint shader_nontex_id = LoadShaders("C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/StandardShading_vs_non_tex.glsl"
//		, "C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/StandardShading_fs_non_tex.glsl");
//	GLuint shader_usetex_id = LoadShaders("C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/StandardShading_vs.glsl"
//		, "C:/Users/ery/Documents/local_devel/OpenGL_tests/GLSL/StandardShading_fs.glsl");
//#endif
//
//#if (_MSC_VER == 1800)	 //Visual Studio 2013
//	GLuint shader_nontex_id = LoadShaders("C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/StandardShading_vs_non_tex.glsl"
//		, "C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/StandardShading_fs_non_tex.glsl");
//	GLuint shader_usetex_id = LoadShaders("C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/StandardShading_vs.glsl"
//		, "C:/Users/B4/Source/Repos/OpenGL_tests/GLSL/StandardShading_fs.glsl");
//#endif



	//座標変換について
	/*
	  CPU側では変換行列を用意する
	  すべての変換を示す行列を用意出来たらこれをGPU側に転送する
	  シェーダはこの変換行列を頂点に掛け算して変換後座標を得る
	  つまり各座標に対しての変換はGPU側；シェーダ側で処理される
	  これらは並列に実行されるため高速な変換が可能になる
	 */
	 /*
	   モデル空間からワールド空間への変換を定める[Model Coordinates -> World Coordinates]
	   ワールド空間からカメラ空間への変換を定める[World Coordinates -> Camera Coordinates]
	   カメラ空間から同次空間への変換を定める[Camera Coordinates -> Homogeneous Coordinates]
	  */

	  /*
		#1: MVP(Model View Projection)行列を定める Projection * View * Model
		#2: GLSL(Shader side)に渡す
		#3: シェーダ側でMVPを用いて頂点の変換を行う

	   */

	mat4 Model = mat4(1.0f); // 単位行列
	Model = mat4(
		  1.0f, 0.0f, 0.0f, 0.0f
		, 0.0f, 1.0f, 0.0f, 0.0f
		, 0.0f, 0.0f, 1.0f, 0.0f
		, 0.0f, 0.0f, 0.0f, 1.0f
	);
	mat4 View = lookAt(
		vec3(5, 5, 5),
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
	vec3 lightPos = vec3(30, 30, 30);

	MdlImporter imptr(shader_nontex_id, shader_usetex_id);

#ifdef KIKUTI_HOME
	imptr.Load("C:/Users/ery/Documents/local_devel/OpenGL_tests/models/t1.stl", vec3(0.8, 0.2, 0.4)); //#cc00000
	 //imptr.Load("C:/Users/ery/Documents/local_devel/OpenGL_tests/models/cube1.obj", vec3(1, 1, 1)); //#cc00000
#else
	imptr.Load("C:/Users/B4/Source/Repos/OpenGL_tests/models/stoadmm.stl", vec3(1, 0.85, 0)); //#cc00000
	//imptr.Load("C:\Users\B4\Source\Repos\OpenGL_tests\models/cube1.obj", vec3(1, 1, 1)); //#cc00000

	//imptr.Load("C:/Users/B4/OneDrive/Documents/Models/PersonalComputer.stl", vec3(0, 0.8, 0.8)); //#cc00000

#endif


//#if (_MSC_VER == 1900)	 //Visual Studio 2015
//	imptr.Load("C:/Users/ery/Documents/local_devel/OpenGL_tests/models/t1.stl", vec3(0.8, 0.2, 0.4)); //#cc00000
//	//imptr.Load("C:/Users/ery/Documents/local_devel/OpenGL_tests/models/cube1.obj", vec3(1, 1, 1)); //#cc00000
//#endif
//
//#if (_MSC_VER == 1800)	 //Visual Studio 2013
//	imptr.Load("C:/Users/B4/Source/Repos/OpenGL_tests/models/stoadmm.stl", vec3(0.8, 0.2, 0.4)); //#cc00000
//	//imptr.Load("C:\Users\B4\Source\Repos\OpenGL_tests\models/cube1.obj", vec3(1, 1, 1)); //#cc00000
//#endif



	int width, height;

	// glEnable (GL_CULL_FACE); // cull face
	// glCullFace (GL_BACK); // cull back face
	// glFrontFace (GL_CW); // GL_CCW for counter clock-wise

	while (!glfwWindowShouldClose(window)) {
		_update_fps_counter(window);

		glfwGetWindowSize(window, &width, &height);
		glViewport (0, 0, width, height);
		// wipe the drawing surface clear

		// mat4 Model = mat4(1.0f); // 単位行列
		// mat4 View = lookAt(
		//    vec3(1, 1, 1),
		//    vec3(0, 0 ,0),
		//    vec3(0, 1, 0)
		//    );
		// //どこからどこを見るか　ワールド座標において
		// mat4 Projection = perspective(30.0f, (float)width / (float)height, 0.1f, 100.0f); //glPerspective

		// mat4 MVP = Projection * View * Model; //これをGLSLに渡す


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glUseProgram(shader_programme_id); // Set the shader programme that should be "in use"
		glUseProgram(shader_nontex_id);
		//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		//Bind iur texture in Texture Unit 0
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture);
		//Set our my_texture_sampler sampler to user Texture Unit 0
		//glUniform1i(TextureID, 0);

		//glBindVertexArray (vao);
		// // draw points 0-3 from the currently bound VAO with current in-use shader
		//glDrawArrays (GL_TRIANGLES, 0, 3);
		// // update other events like input handling

		// //Draw second VAO
		// glBindVertexArray(vao2);
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		Model = mat4(
			1.0f, 0.0f, 0.0f, 0.0f
			, 0.0f, 1.0f, 0.0f, 0.0f
			, 0.0f, 0.0f, 1.0f, 0.0f
			, 0.0f, 0.0f, 0.0f, 1.0f
			);
		MVP = Projection * View * Model; //これをGLSLに渡す

		glUniformMatrix4fv(MatrixID_MVP, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(MatrixID_M, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv(MatrixID_V, 1, GL_FALSE, &View[0][0]);

		lightPos = vec3(4, 4, 4);
		glUniform3f(Vector3ID_LightPosition, lightPos.x, lightPos.y, lightPos.z);

		//glBindVertexArray(vao_cube);
		//glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

		imptr.Render();

		//Model = mat4(
		//	1.0f, 0.0f, 0.0f, 0.0f
		//	, 0.0f, 1.0f, 0.0f, 0.0f
		//	, 0.0f, 0.0f, 1.0f, 0.0f
		//	, 3.0f, 3.0f, 0.0f, 1.0f
		//	);
		//MVP = Projection * View * Model; //これをGLSLに渡す
		//glUniformMatrix4fv(MatrixID_MVP, 1, GL_FALSE, &MVP[0][0]);
		//glUniformMatrix4fv(MatrixID_M, 1, GL_FALSE, &Model[0][0]);
		//glUniformMatrix4fv(MatrixID_V, 1, GL_FALSE, &View[0][0]);

		//lightPos = vec3(4, 4, 4);
		//glUniform3f(Vector3ID_LightPosition, lightPos.x, lightPos.y, lightPos.z);

		//imptr.Render();


		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}

	}
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

