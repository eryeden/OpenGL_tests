//Widnows�̏ꍇ
//#ifdef _MSC_VER
//#pragma comment(lib, "glfw3.lib")
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "GlU32.Lib")
//#pragma comment(lib, "assimp-vc120-mt.lib")
//#pragma comment(lib, "glew32.lib")
//#include <gl/glew.h>
//#define _CRT_SECURE_NO_WARNINGS
//#endif
//
//
//#include <stdio.h>
//#include <string>
//#include <vector>
//#include <iostream>
//#include <fstream>
//#include <algorithm>
//using namespace std;
//
//#include <cstdlib>
//#include <cstring>
//
//#define GL_GLEXT_PROTOTYPES
////GLEW�̊g�����g���ɂ͍��ꁪ���K�v�iglGenBuffers���j
////Linux�ł͂��ꂾ���ŗǂ��@�����炭GLEW�̃��C�u�����s�K�v
////http://stackoverflow.com/questions/3032386/glgenbuffers-not-defined
//#include <GLFW/glfw3.h>
//
//#include <assimp/Importer.hpp>  // C++ importer interface
//#include <assimp/scene.h>       // Output data structure
//#include <assimp/postprocess.h> // Post processing flags
//
//#include "ogldev_util.h"
//#include "ogldev_math_3d.h"
//#include "ogldev_texture.h"

#include "Importer.hpp"
#define INVALID_MATERIAL 0xFFFFFFFF
#define INVALID_OGL_VALUE 0xFFFFFFFF 

using namespace glm;

MdlImporter::MeshEntry::MeshEntry(){

	VertexBuffer = INVALID_OGL_VALUE;
	NormalBuffer = INVALID_OGL_VALUE;
	ElementBuffer = INVALID_OGL_VALUE;
	ColorBuffer = INVALID_OGL_VALUE;

	NumIndices = 0;
	MaterialIndex = INVALID_MATERIAL;
}

MdlImporter::MeshEntry::~MeshEntry()
{

	if (VertexBuffer != INVALID_OGL_VALUE){
		glDeleteBuffers(1, &VertexBuffer);
	}

	if (NormalBuffer != INVALID_OGL_VALUE){
		glDeleteBuffers(1, &NormalBuffer);
	}

	if (ElementBuffer != INVALID_OGL_VALUE){
		glDeleteBuffers(1, &ElementBuffer);
	}

	if (ColorBuffer != INVALID_OGL_VALUE){
		glDeleteBuffers(1, &ColorBuffer);
	}
}

void MdlImporter::MeshEntry::Init(
	const std::vector<glm::vec3> &_Vertices
	, const std::vector<glm::vec3> &_Normals
	, const std::vector<glm::vec3> &_Colors
	, const std::vector<unsigned int> &_Indices
	){
	
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER
		, _Vertices.size() * sizeof(glm::vec3)
		, &_Vertices[0]
		, GL_STATIC_DRAW);

	glGenBuffers(1, &NormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glBufferData(GL_ARRAY_BUFFER
		, _Normals.size() * sizeof(glm::vec3)
		, &_Normals[0]
		, GL_STATIC_DRAW);
	
	glGenBuffers(1, &ColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER
		, _Colors.size() * sizeof(glm::vec3)
		, &_Colors[0]
		, GL_STATIC_DRAW);

	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ARRAY_BUFFER
		, _Indices.size() * sizeof(unsigned int)
		, &_Indices[0]
		, GL_STATIC_DRAW);

	NumIndices = _Indices.size();

}

void MdlImporter::MeshEntry::Init(
	const std::vector<glm::vec3> &_Vertices
	, const std::vector<glm::vec3> &_Normals
	, const std::vector<glm::vec2> &_UVs
	, const std::vector<unsigned int> &_Indices
	){

	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER
		, _Vertices.size() * sizeof(glm::vec3)
		, &_Vertices[0]
		, GL_STATIC_DRAW);

	glGenBuffers(1, &NormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glBufferData(GL_ARRAY_BUFFER
		, _Normals.size() * sizeof(glm::vec3)
		, &_Normals[0]
		, GL_STATIC_DRAW);

	glGenBuffers(1, &ColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER
		, _UVs.size() * sizeof(glm::vec2)
		, &_UVs[0]
		, GL_STATIC_DRAW);

	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ARRAY_BUFFER
		, _Indices.size() * sizeof(unsigned int)
		, &_Indices[0]
		, GL_STATIC_DRAW);

	NumIndices = _Indices.size();
}


MdlImporter::MdlImporter(const GLuint &_ShaderIDNonTexture, const GLuint &_ShaderIDTexture){
	UseTexture = false;
	ShaderIDNonTexture = _ShaderIDNonTexture;
	ShaderIDTexture = _ShaderIDTexture;

	//VAO
	//glGenVertexArrays(1, &VaoID);
	//glBindVertexArray(VaoID);
}

MdlImporter::MdlImporter() {
	UseTexture = false;
}


MdlImporter::~MdlImporter(){
	Clear();
}

void MdlImporter::Clear(){
	//�e�N�X�`����j��
	
}


//���f����ǂ�
bool MdlImporter::Load(const std::string& Filename)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	UseTexture = true;

	bool Ret = false;
	Assimp::Importer Importer;

	//Importer���aiScene�C���X�^���X�̐���
	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);


	if (pScene) {
		//aiScene�C���X�^���X���MeshEntry�̃R���e�i�𐶐�����
		//���_�f�[�^�A�}�e���A���f�[�^(�e�N�X�`���f�[�^)��ǂ�VBO�𐶐�
		Ret = InitFromScene(pScene, Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}


	return Ret;
}

bool MdlImporter::Load(const std::string & Filename, const glm::vec3 & _Color){
	// Release the previously loaded mesh (if it exists)
	Clear();

	UseTexture = false;

	bool Ret = false;
	Assimp::Importer Importer;

	//Importer���aiScene�C���X�^���X�̐���
	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);


	if (pScene) {
		//aiScene�C���X�^���X���MeshEntry�̃R���e�i�𐶐�����
		//���_�f�[�^�A�}�e���A���f�[�^(�e�N�X�`���f�[�^)��ǂ�VBO�𐶐�
		Ret = InitFromScene(pScene, Filename, _Color);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}

	return Ret;
}

bool MdlImporter::InitFromScene(const aiScene* pScene, const std::string& Filename)
{

	//�܂܂��aiScene�G���g�����ɂ��R���e�i�����X�V
	m_Entries.resize(pScene->mNumMeshes);
	//�e�N�X�`���R���e�i�̐����X�V
	//m_Textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		//���b�V���ւ̃|�C���^��������̂ŁA�����胁�b�V���G���g����i�Ԗڂɒ��_�A���_�z��,
		//UV���W����ۑ�����A�e�N�X�`���R���e�i�̍X�V���s��
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	//����InitMaterials�ɂ����t�@�C���p�X���g���ĂȂ�
	//�����Ń}�e���A���i�e�N�X�`���f�[�^�j�̐������s��
	return InitMaterials(pScene, Filename);
}


//�e�N�X�`�����g�p���Ȃ��ꍇ
//�e�N�X�`���̑����VEC3�̐F��񂪗^������̂ŁA����ŐF�Â�����
//��ɐ^���Ԃ����
bool MdlImporter::InitFromScene(const aiScene* pScene, const std::string& Filename, const glm::vec3 & _Color)
{

	//�܂܂��aiScene�G���g�����ɂ��R���e�i�����X�V
	m_Entries.resize(pScene->mNumMeshes);
	//�e�N�X�`���R���e�i�̐����X�V
	//�e�N�X�`���͎g��Ȃ�
	//m_Textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		//���b�V���ւ̃|�C���^��������̂ŁA�����胁�b�V���G���g����i�Ԗڂɒ��_�A���_�z��,
		//�e���_�ɃJ���[����t������
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh, _Color);

		printf("Model loaded: %d / %d\n", i+1, m_Entries.size());
	}
	
	return true;
}


//VBO�𐶐�
void MdlImporter::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	//std::vector<Vertex> Vertices;
	//std::vector<unsigned int> Indices;

	//���_���W
	std::vector<glm::vec3> Vertices;
	//�@��
	std::vector<glm::vec3> Normals;
	//�ʂ��\������C���f�b�N�X�񂪓���
	std::vector<unsigned int> Indices;
	//�e�N�X�`��UV���W
	std::vector<glm::vec2> UVs;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]); //���_
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]); //�@��
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		//�����Œ��_���W�A�@���AUV���W��ǉ�����
		Vertices.push_back(vec3(pPos->x, pPos->y, pPos->z)); //���_���W
		Normals.push_back(vec3(pNormal->x, pNormal->y, pNormal->z)); //�@��
		UVs.push_back(vec2(pTexCoord->x, pTexCoord->y)); //UV���W

	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
		//�ʂ��\�����钸�_�C���f�b�N�X�𒸓_�R���e�i�Ɋi�[
	}

	//������VBO�𐶐�
	//glBindVertexArray(VaoID); //VAO���o�C���h
	m_Entries[Index].Init(Vertices, Normals, UVs, Indices);
	//���b�V���G���g���N���X�ɒ��_�A
	//���_�C���f�b�N�X�R���e�i���킽���A
	//�����炭VBO�̏����������s�킹��
}

//VBO�𐶐�
void MdlImporter::InitMesh(unsigned int Index, const aiMesh* paiMesh, const glm::vec3 & _Color)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	//std::vector<Vertex> Vertices;
	//std::vector<unsigned int> Indices;

	//���_���W
	std::vector<glm::vec3> Vertices;
	//�@��
	std::vector<glm::vec3> Normals;
	//�ʂ��\������C���f�b�N�X�񂪓���
	std::vector<unsigned int> Indices;
	//�J���[�p
	std::vector<glm::vec3> Colors;

	Vertices.resize(paiMesh->mNumVertices);
	Normals.resize(paiMesh->mNumVertices);
	Colors.resize(paiMesh->mNumVertices);


	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		//�e�N�X�`�����͈���Ȃ�

		const aiVector3D* pPos = &(paiMesh->mVertices[i]); //���_
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]); //�@��
		
		//�����Œ��_���W�A�@���A�F����ǉ�����
		//Vertices.push_back(vec3(pPos->x, pPos->y, pPos->z)); //���_���W
		//Normals.push_back(vec3(pNormal->x, pNormal->y, pNormal->z)); //�@��
		//Colors.push_back(_Color); //�F���

		//���W���ύX�Ȃ�
		//Vertices[i] = (vec3(pPos->x, pPos->y, pPos->z)); //���_���W
		//Normals[i] = (vec3(pNormal->x, pNormal->y, pNormal->z)); //�@��
		//Colors[i] = (_Color); //�F���

		/*
		���ϊ�
		X -> X
		Y -> Z
		Z -> -Y
		*/

		////���W���ύX����
		//Vertices[i] = (vec3(pPos->x, pPos->z, -pPos->y)); //���_���W
		//Normals[i] = (vec3(pNormal->x, pNormal->z, -pNormal->y)); //�@��
		//Colors[i] = (_Color); //�F���

							  //���W���ύX����
		Vertices[i] = (vec3(pPos->x, pPos->y, pPos->z)); //���_���W
		Normals[i] = (vec3(pNormal->x, pNormal->y, pNormal->z)); //�@��
		Colors[i] = (_Color); //�F���

		//printf("%3.2f%\n", ((double)(i + 1)/((double)paiMesh->mNumVertices) * 100.0));
	}

	//�ʂ��\�����钸�_�C���f�b�N�X�𒸓_�R���e�i�Ɋi�[
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);	
	}

	//������VBO�𐶐�
	m_Entries[Index].Init(Vertices, Normals, Colors, Indices);

	

}

//���b�V���G���g���̏������iVBO�̐����j�̌�Ă΂�� �v�t�@�C���p�X
//�e�N�X�`���f�[�^�̐����@ASSIMPS�ł̓}�e���A���ƌĂ΂�Ă���炵��
bool MdlImporter::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	//�}�e���A���f�[�^�̐��������[�v���܂킷
	//���b�V���f�[�^�̐��ƃ}�e���A���f�[�^�̐��͈�v���Ă��Ȃ��炵��
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		//aiScene���}�e���A�����o�@�e�N�X�`���̃f�[�^���܂܂�Ă���炵��
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		//m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data; //�e�N�X�`���f�[�^�ւ̃t���p�X
				//m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());
				//�e�N�X�`���R���e�i��i�ԖڂɃe�N�X�`���C���X�^���X��ǉ�

				//if (!m_Textures[i]->Load()) {
				//	//�e�N�X�`���t�@�C�����ǂ߂Ȃ���Ώ���
				//	printf("Error loading texture '%s'\n", FullPath.c_str());
				//	delete m_Textures[i];
				//	m_Textures[i] = NULL;
				//	Ret = false;
				//}
				//else {
				//	printf("Loaded texture '%s'\n", FullPath.c_str());
				//}
			}
		}

		// Load a white texture in case the model does not include its own texture
		//�e�N�X�`���f�[�^���ǂ߂Ȃ������甒���f�[�^��ǂ�
		/*if (!m_Textures[i]) {
			m_Textures[i] = new Texture(GL_TEXTURE_2D, "../Content/white.png");

			Ret = m_Textures[i]->Load();
		}*/
	}

	return Ret;
}


void MdlImporter::RenderTexture(){

	//�e�N�X�`���`�悷��V�F�[�_�[���g�p
	//glUseProgram(ShaderIDTexture);

	//glBindVertexArray(VaoID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		//glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 12);
		//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 20);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VertexBuffer);
		glVertexAttribPointer(
			0
			, 3
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void *)0
			);
		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].ColorBuffer);
		glVertexAttribPointer(
			1
			, 2
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void *)0
			);
		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].NormalBuffer);
		glVertexAttribPointer(
			2
			, 3
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void *)0
			);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].ElementBuffer);

		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		/*if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
		}*/

		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


//Use NON-TEXTURED shader
void MdlImporter::RenderColor(){

	//�P�ɐF�����邾���̃V�F�[�_�[���g�p
	//glUseProgram(ShaderIDNonTexture);

	//glBindVertexArray(VaoID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {

		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VertexBuffer);
		glVertexAttribPointer(
				0
				, 3
				, GL_FLOAT
				, GL_FALSE
				, 0
				, (void *)0
			);

		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].ColorBuffer);
		glVertexAttribPointer(
			1
			, 3
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void *)0
			);

		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].NormalBuffer);
		glVertexAttribPointer(
			2
			, 3
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void *)0
			);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].ElementBuffer);

		glDrawElements(GL_TRIANGLES
			, m_Entries[i].NumIndices
			, GL_UNSIGNED_INT
			, (void *)0
			);


	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


//�`��
void MdlImporter::Render()
{
	if (UseTexture) {
		RenderTexture();
	}
	else {
		RenderColor();
	}
}


































