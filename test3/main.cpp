//#include <GL/glew.h>
#define GL_GLEXT_PROTOTYPES
//GLEWの拡張を使うには此れ↑が必要（glGenBuffers等）
//Linuxではこれだけで良い　おそらくGLEWのライブラリ不必要
//http://stackoverflow.com/questions/3032386/glgenbuffers-not-defined
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstring>

#include "shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;



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

   //glewExperimental = GL_TRUE;
   //glewInit();

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
      0.0f, 0.5f, 0.0f,
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
   
   GLuint vbo = 0;
   glGenBuffers(1, &vbo); // Create vertex buffer object
   glBindBuffer(GL_ARRAY_BUFFER, vbo); // Set this vb object as the current buffer
   glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
          // Copy the points into currently bound buffer

   GLuint cvbo = 0;
   glGenBuffers(1, &cvbo);
   glBindBuffer(GL_ARRAY_BUFFER, cvbo);
   glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colours, GL_STATIC_DRAW);
   
   GLuint vao = 0;
   glGenVertexArrays(1, &vao);  // Create vertex attribute(array ?) object
   glBindVertexArray(vao); // Set this VA object as the current VAO
   //glEnableVertexAttribArray(0); // Enable the first attribute; 0
   
   glBindBuffer(GL_ARRAY_BUFFER, vbo); // Bind VBO
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
         // Define the layout of the first vertex buffer as 0
         // Define the layout for attribute number 0
         // 3 means that the variables are vec3 made from every 3 float in the buffer

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
   GLuint shader_programme_id = LoadShaders("test_vs.glsl", "test_fs.glsl");

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

      GLuint MatrixID = glGetUniformLocation(shader_programme_id, "MVP");
      glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
      
      glBindVertexArray (vao);
      // draw points 0-3 from the currently bound VAO with current in-use shader
      glDrawArrays (GL_TRIANGLES, 0, 3);
      // update other events like input handling

      //Draw second VAO
      glBindVertexArray(vao2);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      
      
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

