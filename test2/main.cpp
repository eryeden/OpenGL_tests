//#include <GL/glew.h>
#define GL_GLEXT_PROTOTYPES
//GLEWの拡張を使うには此れ↑が必要（glGenBuffers等）
//Linuxではこれだけで良い　おそらくGLEWのライブラリ不必要
//http://stackoverflow.com/questions/3032386/glgenbuffers-not-defined
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstring>

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
   const char* vertex_shader =
      "#version 330\n"
      "layout(location = 0) in vec3 vertex_position;"
      "layout(location = 1) in vec3 vertex_colour;"
      "out vec3 colour;"
      "void main () {"
      "  colour = vertex_colour;"
      "  gl_Position = vec4 (vertex_position, 1.0);"
      "}";
   //Fragment shader
   /*
     形状を描画した際の１ピクセルごとにfragment shaderが動く
     そのピクセルの色を決める
     出力はvec4(4d vector): R,G,B,alpha

     バーテックスシェーダと同様に並列に実行される
     こちらは２Dに写像されたピクセルごとに並列実行される
     
    */
   const char* fragment_shader =
      "#version 330\n"
      "in vec3 colour"
      "out vec4 frag_colour;"
      "void main () {"
      "  frag_colour = vec4 (colour, 1.0);"
      "}";

   //Load shader source code
   char s_vs[512];
   char s_fs[512];

   parse_file_into_str("test_vs.glsl", s_vs, 512);
   parse_file_into_str("test_fs.glsl", s_fs, 512);

   const GLchar* p1 = (const GLchar *)s_vs;
   const GLchar* p2 = (const GLchar *)s_fs;
   
   //Create vartex shader
   GLuint vs = glCreateShader (GL_VERTEX_SHADER);
   //glShaderSource (vs, 1, &vertex_shader, NULL); //Input shader's source
   glShaderSource (vs, 1, &p1, NULL); //Input shader's source
   glCompileShader (vs); 
   GLuint fs = glCreateShader (GL_FRAGMENT_SHADER); // Create fragment shader
//   glShaderSource (fs, 1, &fragment_shader, NULL); // Load the string into a GL fragment shader
   glShaderSource (fs, 1, &p2, NULL); // Load the string into a GL fragment shader
   glCompileShader (fs);

   GLuint shader_programme = glCreateProgram (); // Create an empty program for attaching the shaders
   glAttachShader (shader_programme, fs);
   glAttachShader (shader_programme, vs);

   // glBindAttribLocation (shader_programme, 0, "vertex_position");
   // glBindAttribLocation (shader_programme, 1, "vertex_colour");
   
   glLinkProgram (shader_programme); // Link

   int width, height;

   // glEnable (GL_CULL_FACE); // cull face
   // glCullFace (GL_BACK); // cull back face
   // glFrontFace (GL_CW); // GL_CCW for counter clock-wise
   
   while (!glfwWindowShouldClose (window)) {
      _update_fps_counter(window);
      
      glfwGetWindowSize(window, &width, &height);
      //glViewport (0, 0, width, height);
      // wipe the drawing surface clear
      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram (shader_programme); // Set the shader programme that should be "in use"
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

      // glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      // glUseProgram (shader_programme);
      // glBindVertexArray (vao);
      // glDrawArrays (GL_TRIANGLES, 0, 3);
      // glfwPollEvents ();
      // glfwSwapBuffers (window);
      // if (glfwGetKey (window, GLFW_KEY_ESCAPE)) {
      //    glfwSetWindowShouldClose (window, 1);
      // }
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


bool parse_file_into_str (
	const char* file_name, char* shader_str, int max_len
) {
	shader_str[0] = '\0'; // reset string
	FILE* file = fopen (file_name , "r");
	if (!file) {
       //gl_log_err ("ERROR: opening file for reading: %s\n", file_name);
		return false;
	}
	int current_len = 0;
	char line[2048];
	strcpy (line, ""); // remember to clean up before using for first time!
	while (!feof (file)) {
		if (NULL != fgets (line, 2048, file)) {
			current_len += strlen (line); // +1 for \n at end
			if (current_len >= max_len) {
				// gl_log_err (
				// 	"ERROR: shader length is longer than string buffer length %i\n",
				// 	max_len
				// );
			}
			strcat (shader_str, line);
		}
	}
	if (EOF == fclose (file)) { // probably unnecesssary validation
       //gl_log_err ("ERROR: closing file from reading %s\n", file_name);
		return false;
	}
	return true;
}

// void print_shader_info_log (GLuint shader_index) {
// 	int max_length = 2048;
// 	int actual_length = 0;
// 	char log[2048];
// 	glGetShaderInfoLog (shader_index, max_length, &actual_length, log);
// 	printf ("shader info log for GL index %i:\n%s\n", shader_index, log);
// 	gl_log ("shader info log for GL index %i:\n%s\n", shader_index, log);
// }

// bool create_shader (const char* file_name, GLuint* shader, GLenum type) {
// 	gl_log ("creating shader from %s...\n", file_name);
// 	char shader_string[MAX_SHADER_LENGTH];
// 	assert (parse_file_into_str (file_name, shader_string, MAX_SHADER_LENGTH));
// 	*shader = glCreateShader (type);
// 	const GLchar* p = (const GLchar*)shader_string;
// 	glShaderSource (*shader, 1, &p, NULL);
// 	glCompileShader (*shader);
// 	// check for compile errors
// 	int params = -1;
// 	glGetShaderiv (*shader, GL_COMPILE_STATUS, &params);
// 	if (GL_TRUE != params) {
// 		gl_log_err ("ERROR: GL shader index %i did not compile\n", *shader);
// 		print_shader_info_log (*shader);
// 		return false; // or exit or something
// 	}
// 	gl_log ("shader compiled. index %i\n", *shader);
// 	return true;
// }

// void print_programme_info_log (GLuint sp) {
// 	int max_length = 2048;
// 	int actual_length = 0;
// 	char log[2048];
// 	glGetProgramInfoLog (sp, max_length, &actual_length, log);
// 	printf ("program info log for GL index %u:\n%s", sp, log);
// 	gl_log ("program info log for GL index %u:\n%s", sp, log);
// }

// bool is_programme_valid (GLuint sp) {
// 	glValidateProgram (sp);
// 	GLint params = -1;
// 	glGetProgramiv (sp, GL_VALIDATE_STATUS, &params);
// 	if (GL_TRUE != params) {
// 		gl_log_err ("program %i GL_VALIDATE_STATUS = GL_FALSE\n", sp);
// 		print_programme_info_log (sp);
// 		return false;
// 	}
// 	gl_log ("program %i GL_VALIDATE_STATUS = GL_TRUE\n", sp);
// 	return true;
// }

// bool create_programme (GLuint vert, GLuint frag, GLuint* programme) {
// 	*programme = glCreateProgram ();
// 	gl_log (
// 		"created programme %u. attaching shaders %u and %u...\n",
// 		*programme,
// 		vert,
// 		frag
// 	);
// 	glAttachShader (*programme, vert);
// 	glAttachShader (*programme, frag);
// 	// link the shader programme. if binding input attributes do that before link
// 	glLinkProgram (*programme);
// 	GLint params = -1;
// 	glGetProgramiv (*programme, GL_LINK_STATUS, &params);
// 	if (GL_TRUE != params) {
// 		gl_log_err (
// 			"ERROR: could not link shader programme GL index %u\n",
// 			*programme
// 		);
// 		print_programme_info_log (*programme);
// 		return false;
// 	}
// 	assert (is_programme_valid (*programme));
// 	// delete shaders here to free memory
// 	glDeleteShader (vert);
// 	glDeleteShader (frag);
// 	return true;
// }

// GLuint create_programme_from_files (
// 	const char* vert_file_name, const char* frag_file_name
// ) {
// 	GLuint vert, frag, programme;
// 	assert (create_shader (vert_file_name, &vert, GL_VERTEX_SHADER));
// 	assert (create_shader (frag_file_name, &frag, GL_FRAGMENT_SHADER));
// 	assert (create_programme (vert, frag, &programme));
// 	return programme;
// }
