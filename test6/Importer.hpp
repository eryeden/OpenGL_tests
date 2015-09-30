#ifndef IMPORTER_HPP
#define IMPORTER_HPP
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


#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <cstdlib>
#include <cstring>

#define GL_GLEXT_PROTOTYPES
//GLEWの拡張を使うには此れ↑が必要（glGenBuffers等）
//Linuxではこれだけで良い　おそらくGLEWのライブラリ不必要
//http://stackoverflow.com/questions/3032386/glgenbuffers-not-defined
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

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

class MdlImporter{
public:
	MdlImporter();
	~MdlImporter();

	//モデル読み込み
	bool Load(const std::string & filename); //テクスチャを適用
	bool Load(const std::string & filename, const Vector3f & colour);
		//Vector3fによる色を適用
	//モデル描画
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

		//頂点バッファ、インデックスバッファオブジェクトを生成する
		void Init(const std::vector<Vertex>& Vertices,
			const std::vector<unsigned int>& Indices);

		GLuint VB; //エントリーひとつ分の頂点バッファ
		GLuint IB; //面の頂点インデックスバッファ
		unsigned int NumIndices;    //面の頂点インデックス数
		unsigned int MaterialIndex; //aiScene中のマテリアルインデックス
	};

	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;

};


#endif