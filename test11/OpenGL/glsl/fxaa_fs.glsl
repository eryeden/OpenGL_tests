#version 400 core

//uniform sampler2D tex0; //Texture sampler
uniform sampler2D renderedTexture;
uniform float vx_offset = 1.0;
uniform float rt_w;//�t���[����
uniform float rt_h;//�t���[������
uniform float FXAA_SPAN_MAX = 8.0;
uniform float FXAA_REDUCE_MUL = 0.0;//1.0/8.0;

in vec2 UV;
//in vec4 posPos;
out vec3 color;

#define FxaaInt2 ivec2
#define FxaaFloat2 vec2
#define FxaaTexLod0(t, p) textureLod(t, p, 0.0)
#define FxaaTexOff(t, p, o, r) textureLodOffset(t, p, 0.0, o)


///*
//	texture2DLod(t, p, 0.0)�ɂ���
//	t : sampler2D
//	p : �T���v�������e�N�X�`���̍��W
//	MEMO: Coordinate:���W

//	�Δ���
//	textureLod performs a texture lookup at coordinate P from the texture bound 
//	to sampler with an explicit level-of-detail as specified in lod. 
//	lod specifies ��base and sets the partial derivatives as follows:

//	�uLOD�v�Ƃ́uLevel�@of�@Detail�v�̗��ł��āA���󂷂�Ɓu�ׂ����̓x�����v�Ƃ��������ł��傤���B



//*/



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


void main(){
    color =  PostFX(renderedTexture, gl_FragCoord.st, 0.0).rgb;
}



