#version 330 core

//�|�X�g�v���Z�V���O�p�t���O�����g�V�F�[�_�[
//�����ő����̃|�X�g�v���Z�X���s��
//���݂̓����_�����O���ꂽ���ʂ��i�[���Ă���e�N�X�`�������̂܂ܕ`�悵�Ă���
//uniform sampler2D fbo_texture;
//in vec2 f_texcoord;

//void main(void) {
//  gl_FragColor = texture2D(fbo_texture, f_texcoord);
//}


in vec2 UV;
out vec3 color;

uniform sampler2D renderedTexture;

void main(){
	color = texture( renderedTexture, UV).xyz ;
}


