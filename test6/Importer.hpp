#ifndef IMPORTER_HPP
#define IMPORTER_HPP
//Widnows�̏ꍇ
#ifdef _MSC_VER
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GlU32.Lib")
#pragma comment(lib, "assimp-vc120-mt.lib")
#pragma comment(lib, "glew32.lib")
#include <gl/glew.h>
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <cstdlib>
#include <cstring>

#define GL_GLEXT_PROTOTYPES
//GLEW�̊g�����g���ɂ͍��ꁪ���K�v�iglGenBuffers���j
//Linux�ł͂��ꂾ���ŗǂ��@�����炭GLEW�̃��C�u�����s�K�v
//http://stackoverflow.com/questions/3032386/glgenbuffers-not-defined
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ogldev_util.h"
#include "ogldev_math_3d.h"
#include "ogldev_texture.h"

struct Vertex
{

	Vector3f m_pos;
	Vector2f m_tex;
	Vector3f m_normal;

	Vertex() {}

	Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};
//
//struct MeshEntry {
//	MeshEntry();
//
//	~MeshEntry();
//
//	//���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�𐶐�����
//	//void Init(const std::vector<Vertex>& Vertices,
//	//	const std::vector<unsigned int>& Indices);
//	virtual void Init();
//
//	//GLuint VB; //�G���g���[�ЂƂ��̒��_�o�b�t�@
//	//GLuint IB; //�ʂ̒��_�C���f�b�N�X�o�b�t�@
//
//	GLuint VertexBuffer;
//	GLuint NormalBuffer;
//	GLuint ElementBuffer;
//
//	GLuint ColorBuffer; //UV�o�b�t�@�̃n���h����Color�o�b�t�@�̃n���h��������
//
//	unsigned int NumIndices;    //�ʂ̒��_�C���f�b�N�X��
//	unsigned int MaterialIndex; //aiScene���̃}�e���A���C���f�b�N�X
//
//	//���f�����Ă񂾒��_���W�AUV���W�A���_�ɑΉ�����F�A�@��������
//	std::vector<glm::vec3> Vertices;
//	std::vector<glm::vec3> Normals;
//	//�ʂ��\������C���f�b�N�X�񂪓���
//	std::vector<unsigned short> Indices;
//};
//
//struct MeshEntryColor : public MeshEntry{
//	MeshEntryColor();
//	~MeshEntryColor();
//
//	void Init(
//		const std::vector<glm::vec3> &_Vertices
//		, const std::vector<glm::vec3> &_Normals
//		, const std::vector<glm::vec3> &_Colors
//		, const std::vector<glm::vec3> &_Indices
//		);
//
//	std::vector<glm::vec3> Colors; //���_�ɑΉ�����F��񂪂͂���
//};
//struct MeshEntryTexture : public MeshEntry{
//	MeshEntryTexture();
//	~MeshEntryTexture();
//
//	void Init(
//		const std::vector<glm::vec3> &_Vertices
//		, const std::vector<glm::vec3> &_Normals
//		, const std::vector<glm::vec2> &_UVs
//		, const std::vector<glm::vec3> &_Indices
//		);
//
//	std::vector<glm::vec2> UVs; //�e�N�X�`��UV���W������
//};

class MdlImporter{
public:
	MdlImporter();
	~MdlImporter();

	//���f���ǂݍ���
	bool Load(const std::string & filename); //�e�N�X�`����K�p
	bool Load(const std::string & filename, const Vector3f & colour);
		//Vector3f�ɂ��F��K�p
	//���f���`��
	void Render();
	

private:

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

	void RenderTexture();
	void RenderColor();


	struct MeshEntry {
		MeshEntry();

		~MeshEntry();

		//���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�𐶐�����
		//void Init(const std::vector<Vertex>& Vertices,
		//	const std::vector<unsigned int>& Indices);
		virtual void Init() = 0;

		//GLuint VB; //�G���g���[�ЂƂ��̒��_�o�b�t�@
		//GLuint IB; //�ʂ̒��_�C���f�b�N�X�o�b�t�@

		GLuint VertexBuffer;
		GLuint NormalBuffer;
		GLuint ElementBuffer;
		
		GLuint ColorBuffer; //UV�o�b�t�@�̃n���h����Color�o�b�t�@�̃n���h��������

		unsigned int NumIndices;    //�ʂ̒��_�C���f�b�N�X��
		unsigned int MaterialIndex; //aiScene���̃}�e���A���C���f�b�N�X

		//���f�����Ă񂾒��_���W�AUV���W�A���_�ɑΉ�����F�A�@��������
		std::vector<glm::vec3> Vertices;
		std::vector<glm::vec3> Normals;
		//�ʂ��\������C���f�b�N�X�񂪓���
		std::vector<unsigned short> Indices;
	};

	struct MeshEntryColor : public MeshEntry{
		MeshEntryColor();
		~MeshEntryColor();

		void Init(
			const std::vector<glm::vec3> &_Vertices
			, const std::vector<glm::vec3> &_Normals
			, const std::vector<glm::vec3> &_Colors
			, const std::vector<unsigned short> &_Indices
			);

		void Init();

		std::vector<glm::vec3> Colors; //���_�ɑΉ�����F��񂪂͂���
	};
	struct MeshEntryTexture : public MeshEntry{
		MeshEntryTexture();
		~MeshEntryTexture();

		void Init(
			const std::vector<glm::vec3> &_Vertices
			, const std::vector<glm::vec3> &_Normals
			, const std::vector<glm::vec2> &_UVs
			, const std::vector<unsigned short> &_Indices
			);

		void Init();

		std::vector<glm::vec2> UVs; //�e�N�X�`��UV���W������
	};

	////���f�����Ă񂾒��_���W�AUV���W�A���_�ɑΉ�����F�A�@��������
	//std::vector<glm::vec3> vertices;
	//std::vector<glm::vec2> uvs;
	//std::vector<glm::vec3> colors;
	//std::vector<glm::vec3> normals;

	//���_���Ή�����C���f�b�N�X�z�񂪂����ɓ���
	//std::vector<unsigned short> indices;

	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;

};


#endif