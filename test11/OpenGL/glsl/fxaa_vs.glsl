#version 400 core

/*
�ǂ����|�X�g�v���Z�X�Ƃ������ă����_�����O���I�����t���[���o�b�t�@�Ɍ��ʂ��������܂ꂽ��ɌĂ΂�鏈���炵��
�Ȃ̂ŁA�����ł����e�N�X�`���Ƃ́A�t���[���o�b�t�@�̂��ƂȂ̂ł͂Ȃ���
�������܂ōl���Ă����V�F�[�_�[�Ƃ͕ʂ̂Ƃ���œ����Ă���炵��

�|�X�g�v���Z�V���O�̗���

�o�[�e�b�N�X�o�b�t�@�A�t���O�����g�o�b�t�@�͗^����ꂽ���_�ɂ����钸�_���A�F��񂵂��m�邱�Ƃ͂ł��Ȃ����A
�e�N�X�`���Ƀ����_�����O���邱�ƂŃV�F�[�_�[�������_�����O���ʂ̑S��ɃA�N�Z�X���邱�Ƃ��ł���悤�ɂȂ�B

���̂��߁A�܂��t���[���o�b�t�@�I�u�W�F�N�g��p���ă����_�����O�񏥂��e�N�X�`���Ƃ��āA���̃e�N�X�`���ɑ΂���
�|�X�g�t�B���^�v���Z�X��K�p����B

����
���P
	�t���[���o�b�t�@�I�u�W�F�N�g�𐶐����A�����ʂ��Đ[�x�o�b�t�@�������_�o�b�t�@�ɕۑ��A
	�J���[�o�b�t�@���e�N�X�`���ɕۑ�����
	�RD�V�[���������_�����O����Ȃ�ΐ[�x�o�b�t�@�͕K�v

	GL_CLAMP_TO_EDGE���w��AGL_REPEAT�ɂ��{�[�_�[���b�s���O���ʂ�h������



*/

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
//out vec4 posPos;


uniform float rt_w; //�X�N���[����
uniform float rt_h;//�X�N���[������
uniform float FXAA_SUBPIX_SHIFT = 0.0;//1.0/4.0;


void main(){
	gl_Position =  vec4(vertexPosition_modelspace,1);
	//UV = (vertexPosition_modelspace.xy+vec2(1,1))/2.0;

	vec2 rcpFrame = vec2(1.0/rt_w, 1.0/rt_h);

//	posPos.xy = gl_MultiTexCoord0; //UV���W�ł��낤
//    posPos.zw = gl_MultiTexCoord0 - (rcpFrame * (0.5 + FXAA_SUBPIX_SHIFT));
}


