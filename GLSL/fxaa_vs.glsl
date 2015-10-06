#version 330 core

/*
�ǂ����|�X�g�v���Z�X�Ƃ������ă����_�����O���I�����t���[���o�b�t�@�Ɍ��ʂ��������܂ꂽ��ɌĂ΂�鏈���炵��
�Ȃ̂ŁA�����ł����e�N�X�`���Ƃ́A�t���[���o�b�t�@�̂��ƂȂ̂ł͂Ȃ���
�������܂ōl���Ă����V�F�[�_�[�Ƃ͕ʂ̂Ƃ���œ����Ă���炵��

�|�X�g�v���Z�V���O�̗���


*/

[Vertex_Shader]
varying vec4 posPos;
uniform float FXAA_SUBPIX_SHIFT = 1.0/4.0;
uniform float rt_w; //��ʁH�e�N�X�`���̕��H
// GeeXLab built-in
uniform float rt_h;//��ʁA�e�N�X�`���̍����H
// GeeXLab built-in
 
void main(void)
{
	
    gl_Position = ftransform(); 
		//�ǂ��Ȃ��Ă���̂��킩��Ȃ����A�����炭MVP�𒸓_���W�ɂ����Ă����Ԃ��֐��ł��낤
    
	gl_TexCoord[0] = gl_MultiTexCoord0; // gl_MuliTexCoord0�͒��_�̑Ή�����e�N�X�`�����W�炵��
	/*
		lg_MutiTexCoord0�̓e�N�X�`���j�b�g�O�Ɋ֘A�Â���ꂽ�e�N�X�`�����W�炵��
	*/

    vec2 rcpFrame = vec2(1.0/rt_w, 1.0/rt_h); //�t���[�����A�����̋t��

    posPos.xy = gl_MultiTexCoord0.xy; //UV���W�ł��낤

    posPos.zw = gl_MultiTexCoord0.xy - (rcpFrame * (0.5 + FXAA_SUBPIX_SHIFT));
	 //�e�N�X�`��UV���W����t���[���t���̔����������Ăǂ��Ȃ�̂��낤��
}

 
[Pixel_Shader]
#version 120
uniform sampler2D tex0; //Texture sampler
// 0
uniform float vx_offset; //?
uniform float rt_w; //�t���[����
// GeeXLab built-in
uniform float rt_h; //�t���[������
// GeeXLab built-in
uniform float FXAA_SPAN_MAX = 8.0;
uniform float FXAA_REDUCE_MUL = 1.0/8.0;
varying vec4 posPos;
#define FxaaInt2 ivec2
#define FxaaFloat2 vec2
#define FxaaTexLod0(t, p) texture2DLod(t, p, 0.0)
#define FxaaTexOff(t, p, o, r) texture2DLodOffset(t, p, 0.0, o)

/*
	texture2DLod(t, p, 0.0)�ɂ���
	t : sampler2D
	p : �T���v�������e�N�X�`���̍��W
	MEMO: Coordinate:���W

	�Δ���
	textureLod performs a texture lookup at coordinate P from the texture bound 
	to sampler with an explicit level-of-detail as specified in lod. 
	lod specifies ��base and sets the partial derivatives as follows:

	�uLOD�v�Ƃ́uLevel�@of�@Detail�v�̗��ł��āA���󂷂�Ɓu�ׂ����̓x�����v�Ƃ��������ł��傤���B



*/
 
vec3 FxaaPixelShader( 
  vec4 posPos, // Output of FxaaVertexShader interpolated across screen.
  sampler2D tex, // Input texture.
  vec2 rcpFrame) // Constant {1.0/frameWidth, 1.0/frameHeight}.
{
    /*---------------------------------------------------------*/
    #define FXAA_REDUCE_MIN   (1.0/128.0)
    //#define FXAA_REDUCE_MUL   (1.0/8.0)
    //#define FXAA_SPAN_MAX     8.0
	/*---------------------------------------------------------*/
	//�����炭�A���݂̃t���O�����g���W�̎��͂̐F�A�������͂��̔����l���擾���Ă���
	//4���_�̐F���A����ɉ�����̒����̐F�����擾���Ă���
    vec3 rgbNW = FxaaTexLod0(tex, posPos.zw).xyz;
    vec3 rgbNE = FxaaTexOff(tex, posPos.zw, FxaaInt2(1,0), rcpFrame.xy).xyz;
    vec3 rgbSW = FxaaTexOff(tex, posPos.zw, FxaaInt2(0,1), rcpFrame.xy).xyz;
    vec3 rgbSE = FxaaTexOff(tex, posPos.zw, FxaaInt2(1,1), rcpFrame.xy).xyz;
    vec3 rgbM  = FxaaTexLod0(tex, posPos.xy).xyz;
    /*---------------------------------------------------------*/
	//�^�b�v�H�擾������f�l��O�i�œ��邪�����RGB�̃x�N�g���Ȃ̂ŋP�x�l�œ��ω��Z�ɂ��ϊ�����B��܂�X�J���ɕϊ�����
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);
    /*---------------------------------------------------------*/
	//5�_�̋P�x�l�̂����ő�̂��̂ƍŏ��̂��̂𓾂�@�ő�l�ŏ��l�𓾂�
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    /*---------------------------------------------------------*/
	//�G�b�W�̕����𓾂Ă���@4��f�̔z���ɂ��p�x�𒲐����Ă���
    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
    /*---------------------------------------------------------*/
    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
        FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(FxaaFloat2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX), 
          max(FxaaFloat2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), 
          dir * rcpDirMin)) * rcpFrame.xy;
    /*--------------------------------------------------------*/
    vec3 rgbA = (1.0/2.0) * (
        FxaaTexLod0(tex, posPos.xy + dir * (1.0/3.0 - 0.5)).xyz +
        FxaaTexLod0(tex, posPos.xy + dir * (2.0/3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
        FxaaTexLod0(tex, posPos.xy + dir * (0.0/3.0 - 0.5)).xyz +
        FxaaTexLod0(tex, posPos.xy + dir * (3.0/3.0 - 0.5)).xyz);
    float lumaB = dot(rgbB, luma);
    if((lumaB < lumaMin) || (lumaB > lumaMax)) return rgbA;
    return rgbB;
}

 
vec4 PostFX(sampler2D tex, vec2 uv, float time)
{
    vec4 c = vec4(0.0);
    vec2 rcpFrame = vec2(1.0/rt_w, 1.0/rt_h);
    c.rgb = FxaaPixelShader(posPos, tex, rcpFrame);
    //c.rgb = 1.0 - texture2D(tex, posPos.xy).rgb;
  c.a = 1.0;
    return c;
}

    
void main() 
{
    vec2 uv = gl_TexCoord[0].st;
    gl_FragColor = PostFX(tex0, uv, 0.0);
}


//// Input vertex data, different for all executions of this shader.
//layout(location = 0) in vec3 vertexPosition_modelspace;
//layout(location = 1) in vec2 vertexUV;
//layout(location = 2) in vec3 vertexNormal_modelspace;

//// Output data ; will be interpolated for each fragment.
//out vec2 UV;
//out vec3 Position_worldspace;
//out vec3 Normal_cameraspace;
//out vec3 EyeDirection_cameraspace;
//out vec3 LightDirection_cameraspace;

//// Values that stay constant for the whole mesh.
//uniform mat4 MVP;
//uniform mat4 V;
//uniform mat4 M;
//uniform vec3 LightPosition_worldspace;

//void main(){

//	// Output position of the vertex, in clip space : MVP * position
//	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	
//	// Position of the vertex, in worldspace : M * position
//	Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;
	
//	// Vector that goes from the vertex to the camera, in camera space.
//	// In camera space, the camera is at the origin (0,0,0).
//	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
//	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

//	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
//	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
//	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
//	// Normal of the the vertex, in camera space
//	Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
//	// UV of the vertex. No special space for this one.
//	UV = vertexUV;
//}
