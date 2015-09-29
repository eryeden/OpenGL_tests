//Widnowsの場合
#ifdef _MSC_VER
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GlU32.Lib")
#pragma comment(lib, "assimp-vc120-mt.lib")
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


void gl_execute(GLFWwindow *window);
void _update_fps_counter(GLFWwindow * window);

bool parse_file_into_str (
	const char* file_name, char* shader_str, int max_len
   );



int main(){

   if(!glfwInit()){
      fprintf(stderr, "ERROR: could not start GLFW3\n");
      return 1;
   }

//   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow * window = glfwCreateWindow(640, 480, "Hello Triangle",
                                          NULL, NULL);
   if(!window){
      fprintf(stderr, "ERROR: could not open window with GLFW3\n");
      glfwTerminate();
      return 1;
   }

   glfwMakeContextCurrent(window);

   glewExperimental = GL_TRUE;
   glewInit();

   const GLubyte* renderer = glGetString(GL_RENDERER);
   const GLubyte* version = glGetString(GL_VERSION);
   printf("Renderer: %s\n", renderer);
   printf("Version: %s\n", version);

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);

   gl_execute(window);
   
   glfwTerminate();
   return 0;
}


void gl_execute(GLFWwindow *window){
   float points[] = {
      0.0f, 0.4f, 0.0f,
      0.5f, -0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f
   };

   float points2[] = {
      0.25f, 0.75f, 0.0f,
      0.75f, -0.25f, 0.0f,
      -0.25f, -0.25f, 0.0f
   };

   float colours[] = {
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f
   };

   float cube_points[] = {
      -1.0f,-1.0f,-1.0f, // 三角形1:開始
      -1.0f,-1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f, // 三角形1:終了
      1.0f, 1.0f,-1.0f, // 三角形2:開始
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f, // 三角形2:終了
      1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
      1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,
      1.0f,-1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f,-1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f,
      1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,
      1.0f,-1.0f, 1.0f
   };

   float cube_colours[] = {
      0.583f,  0.771f,  0.014f,
      0.609f,  0.115f,  0.436f,
      0.327f,  0.483f,  0.844f,
      0.822f,  0.569f,  0.201f,
      0.435f,  0.602f,  0.223f,
      0.310f,  0.747f,  0.185f,
      0.597f,  0.770f,  0.761f,
      0.559f,  0.436f,  0.730f,
      0.359f,  0.583f,  0.152f,
      0.483f,  0.596f,  0.789f,
      0.559f,  0.861f,  0.639f,
      0.195f,  0.548f,  0.859f,
      0.014f,  0.184f,  0.576f,
      0.771f,  0.328f,  0.970f,
      0.406f,  0.615f,  0.116f,
      0.676f,  0.977f,  0.133f,
      0.971f,  0.572f,  0.833f,
      0.140f,  0.616f,  0.489f,
      0.997f,  0.513f,  0.064f,
      0.945f,  0.719f,  0.592f,
      0.543f,  0.021f,  0.978f,
      0.279f,  0.317f,  0.505f,
      0.167f,  0.620f,  0.077f,
      0.347f,  0.857f,  0.137f,
      0.055f,  0.953f,  0.042f,
      0.714f,  0.505f,  0.345f,
      0.783f,  0.290f,  0.734f,
      0.722f,  0.645f,  0.174f,
      0.302f,  0.455f,  0.848f,
      0.225f,  0.587f,  0.040f,
      0.517f,  0.713f,  0.338f,
      0.053f,  0.959f,  0.120f,
      0.393f,  0.621f,  0.362f,
      0.673f,  0.211f,  0.457f,
      0.820f,  0.883f,  0.371f,
      0.982f,  0.099f,  0.879f
   };

   float cube_uv[] = {
      0.000059f, 1.0f-0.000004f,
      0.000103f, 1.0f-0.336048f,
      0.335973f, 1.0f-0.335903f,
      1.000023f, 1.0f-0.000013f,
      0.667979f, 1.0f-0.335851f,
      0.999958f, 1.0f-0.336064f,
      0.667979f, 1.0f-0.335851f,
      0.336024f, 1.0f-0.671877f,
      0.667969f, 1.0f-0.671889f,
      1.000023f, 1.0f-0.000013f,
      0.668104f, 1.0f-0.000013f,
      0.667979f, 1.0f-0.335851f,
      0.000059f, 1.0f-0.000004f,
      0.335973f, 1.0f-0.335903f,
      0.336098f, 1.0f-0.000071f,
      0.667979f, 1.0f-0.335851f,
      0.335973f, 1.0f-0.335903f,
      0.336024f, 1.0f-0.671877f,
      1.000004f, 1.0f-0.671847f,
      0.999958f, 1.0f-0.336064f,
      0.667979f, 1.0f-0.335851f,
      0.668104f, 1.0f-0.000013f,
      0.335973f, 1.0f-0.335903f,
      0.667979f, 1.0f-0.335851f,
      0.335973f, 1.0f-0.335903f,
      0.668104f, 1.0f-0.000013f,
      0.336098f, 1.0f-0.000071f,
      0.000103f, 1.0f-0.336048f,
      0.000004f, 1.0f-0.671870f,
      0.336024f, 1.0f-0.671877f,
      0.000103f, 1.0f-0.336048f,
      0.336024f, 1.0f-0.671877f,
      0.335973f, 1.0f-0.335903f,
      0.667969f, 1.0f-0.671889f,
      1.000004f, 1.0f-0.671847f,
      0.667979f, 1.0f-0.335851f
   };

   //###################TEXTURE################################

   //Load texture iamge Texture Loader in GLFW is removed in GLFW3
   //結局テクスチャを読むのにはGLIといふライブラリを使うことになった
   //GLFW３ではテクスチャ読み込みはサポートされない
   //GLIはヘッダのみで構成されるのでライブラリのリンクは必要ない
   //しかし、C+11を用いて書かれるため -std=c++11 のオプションはコンパイル時に必要になる
   GLuint texture;
   texture = createTexture("C:/Users/B4/Source/Repos/OpenGL_tests/test6/uvtemplate.DDS");
   
   
   
   
   
   
   
   //###################TEXTURE################################


   
   GLuint vbo = 0;
   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo); 
   glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
          
   GLuint cvbo = 0;
   glGenBuffers(1, &cvbo);
   glBindBuffer(GL_ARRAY_BUFFER, cvbo);
   glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colours, GL_STATIC_DRAW);
   
   GLuint vao = 0;
   glGenVertexArrays(1, &vao); 
   glBindVertexArray(vao); 
   glBindBuffer(GL_ARRAY_BUFFER, vbo); 
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glBindBuffer(GL_ARRAY_BUFFER, cvbo);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);

   GLuint vbo2 = 0;
   glGenBuffers(1, &vbo2);
   glBindBuffer(GL_ARRAY_BUFFER, vbo2);
   glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points2, GL_STATIC_DRAW);

   GLuint vao2 = 0;
   glGenVertexArrays(1, &vao2);
   glBindVertexArray(vao2);
   //glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, vbo2);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

   glBindBuffer(GL_ARRAY_BUFFER, cvbo);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);


   //#################################CUBE#################################
   GLuint vbo_cube_vertex = 0;
   glGenBuffers(1, &vbo_cube_vertex);
   glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertex);
   glBufferData(GL_ARRAY_BUFFER, sizeof(cube_points), cube_points, GL_STATIC_DRAW);

   GLuint vbo_cube_colour = 0;
   glGenBuffers(1, &vbo_cube_colour);
   glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colour);
   glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colours), cube_colours, GL_STATIC_DRAW);

   GLuint vbo_cube_uv = 0;
   glGenBuffers(1, &vbo_cube_uv);
   glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_uv);
   glBufferData(GL_ARRAY_BUFFER, sizeof(cube_uv), cube_uv, GL_STATIC_DRAW);
   
   
   GLuint vao_cube = 0;
   glGenVertexArrays(1, &vao_cube);
   glBindVertexArray(vao_cube);
   glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertex);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   //glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colour);
   //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_uv);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
   
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   //#################################CUBE#################################

   


   
   //Vertex shader
   /*
     入力としてvec3
     出力として gl_Position　にvec4(4d vector)が入力される

     VAOの要素としてVBOがvec3より構成されるので
     ３頂点分としてこのシェーダが３つ同時に動くことになる
     これらシェーダーはGPUで並列に実行される
     ８頂点だったら８パラ
     
     vec4(vp, 1.0)の１．０は他の視点を計算しないことを示すらしい
    */
   //Fragment shader
   /*
     形状を描画した際の１ピクセルごとにfragment shaderが動く
     そのピクセルの色を決める
     出力はvec4(4d vector): R,G,B,alpha

     バーテックスシェーダと同様に並列に実行される
     こちらは２Dに写像されたピクセルごとに並列実行される
     
    */
   GLuint shader_programme_id = LoadShaders("C:/Users/B4/Source/Repos/OpenGL_tests/test6/test_vs.glsl", "C:/Users/B4/Source/Repos/OpenGL_tests/test6/test_fs.glsl");

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
   mat4 View = lookAt(
      vec3(4, 3, 3),
      vec3(0, 0 ,0),
      vec3(0, 1, 0)
      );
   //どこからどこを見るか　ワールド座標において
   mat4 Projection = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); //glPerspective

   mat4 MVP = Projection * View * Model; //これをGLSLに渡す


   mat4 Model2 = mat4(1.0f, 0, 0, 0,
                      0, 1.0f, 0, 0,
                      0, 0, 1.0f, 0,
                      1.0, -1.0 ,-2.3 ,1.0f);
   
   mat4 MVP2 = Projection * View * Model2;
   
   //シェーダ中のMVPへのハンドルを取得
   GLuint MatrixID = glGetUniformLocation(shader_programme_id, "MVP");
   GLuint TextureID = glGetUniformLocation(shader_programme_id, "my_texture_sampler");
   
   int width, height;

   // glEnable (GL_CULL_FACE); // cull face
   // glCullFace (GL_BACK); // cull back face
   // glFrontFace (GL_CW); // GL_CCW for counter clock-wise
   
   while (!glfwWindowShouldClose (window)) {
      _update_fps_counter(window);
      
      glfwGetWindowSize(window, &width, &height);
      //glViewport (0, 0, width, height);
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
      

      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram (shader_programme_id); // Set the shader programme that should be "in use"

      //glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

      //Bind iur texture in Texture Unit 0
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);
      //Set our my_texture_sampler sampler to user Texture Unit 0
      glUniform1i(TextureID, 0);
      
      //glBindVertexArray (vao);
      // // draw points 0-3 from the currently bound VAO with current in-use shader
      //glDrawArrays (GL_TRIANGLES, 0, 3);
      // // update other events like input handling

      // //Draw second VAO
      // glBindVertexArray(vao2);
      // glDrawArrays(GL_TRIANGLES, 0, 3);

      glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
      
      glBindVertexArray(vao_cube);
      glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

      
      glfwPollEvents ();
      // put the stuff we've been drawing onto the display
      glfwSwapBuffers (window);

      if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_ESCAPE)) {
         glfwSetWindowShouldClose (window, 1);
      }

   }
}


void _update_fps_counter(GLFWwindow * window){
   static double previous_seconds = glfwGetTime ();
   static int frame_count;
   double current_seconds = glfwGetTime ();
   double elapsed_seconds = current_seconds - previous_seconds;
   if (elapsed_seconds > 0.25) {
      previous_seconds = current_seconds;
      double fps = (double)frame_count / elapsed_seconds;
      char tmp[128];
      sprintf (tmp, "opengl @ fps: %.2f", fps);
      glfwSetWindowTitle (window, tmp);
      frame_count = 0;
   }
  frame_count++;
}

