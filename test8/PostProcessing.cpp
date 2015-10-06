#include "PostProcessing.hpp"

GLuint fbo;	// Frame Buffer Object
GLuint fbo_texture;	//FBO�Ƀo�C���h�����e�N�X�`��
GLuint rbo_depth;   //�����_�[�o�b�t�@�I�u�W�F�N�g�A�[�x�o�b�t�@���������܂��

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
	//�[���d�Ȃ�H�����������邽�߃t�B���^��K�p����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D
		, 0, GL_RGBA
		, s_width, s_height
		, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);  //�e�N�X�`���o�C���h����

	//Depth buffer	�����_�o�b�t�@�̐���
	glGenRenderbuffers(1, &rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, s_width, s_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//�e�N�X�`���A�����_�o�b�t�@��VBO�Ƀo�C���h����
	//�����炭����ɂ�背���_�����O���ʂ��e�N�X�`���o�b�t�@�A�����_�o�b�t�@�ɏ������܂��悤�ɂȂ�
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
		//�X�N���[���T�C�Y�ύX���Ƀe�N�X�`���T�C�Y�A�[�x�o�b�t�@�i�����_�o�b�t�@�j�T�C�Y�̕ύX���K�v
		// On reshape
		//�e�N�X�`���̃��T�C�Y
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s_width, s_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	//�����_�o�b�t�@�i�[�x�o�b�t�@�j�̃��T�C�Y
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, s_width, s_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//############################ON RESHAPE################################

	//############################FREE RESOURCES################################
	//���\�[�X�̊J��
	glDeleteRenderbuffers(1, &rbo_depth);
	glDeleteTextures(1, &fbo_texture);
	glDeleteFramebuffers(1, &fbo);
	//############################FREE RESOURCES################################


	/*
		�����_�����O���ʂ��i�[�����e�N�X�`����������邽�߂̒��_��p�ӂ���
		����͂QD�G�t�F�N�g���e�N�X�`���ɓK�p���邽�߂̂��̂ŁA���̂��߂̒��_���W��2�����ƂȂ�
	*/
	GLuint vbo_fbo_vertices; //�e�N�X�`���\���Ɏg�p����VBO
	//���_�̏�����
	GLfloat fbo_vertices[] = {
		  -1, -1
		,  1, -1
		, -1,  1
		,  1,  1
	};
	glGenBuffers(1, &vbo_fbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //�o�C���h����

	//VBO�̊J��
	glDeleteBuffers(1, &vbo_fbo_vertices);

	//�`��ɂ���
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












