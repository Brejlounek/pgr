#include "OglModel.h"

OglModel::OglModel()
{
	pos = glm::vec3(0, 0, 0);
	rot = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

void OglModel::fromFile(string fname, OglRenderer* renderer_, uint flags)
{
	renderer = renderer_;
	Importer Importer;

	bool flipz = flags & FLIPZ;
	bool calcNormals = flags & CALCNORMALS;

	const aiScene* pScene = Importer.ReadFile( fname.c_str( ),
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if ( !pScene )
	{
		cout << "Loading " << fname << " failed." << endl;
		return;
	}

	cout << fname << " loaded." << endl;

	for ( unsigned int i = 0; i < pScene->mNumMeshes; i++ ) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		OglMesh mesh;
		mesh.init( i, paiMesh, flipz );
		if ( calcNormals )
			mesh.calcNormals();

		uint matId = paiMesh->mMaterialIndex;
		meshes.push_back( MeshMat(mesh,matId) );
	}
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* paiMaterial = pScene->mMaterials[i];
		if (paiMaterial->GetTextureCount(aiTextureType_DIFFUSE)>0)
		{
			glm::vec3 color = glm::vec3(1, 1, 1);
			aiString path;
			paiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path);

			string imgPath = path.C_Str();
			stringstream imgPathSS;
			imgPathSS << "gfx/" << imgPath;
			imgPath = imgPathSS.str();

			OglMaterial material( color, imgPath );
			materials.push_back(material);
		}
		else
		{
			//material 3ds ma barvu zapsanou nejak hodne divne, nechci se s tim zdrzovat
			/*for ( uint i = 0; i < paiMaterial->mNumProperties; i++ )
			{
				cout << i << ": ";
				for ( uint q = 0; q < paiMaterial->mProperties[i]->mDataLength; q++ )
				{
					cout << (int)(unsigned char)paiMaterial->mProperties[i]->mData[q] << " ";
				}
				cout << endl;
			}*/
			glm::vec3 color = glm::vec3( 0 / 255.0, 150 / 255.0, 255 / 255.0 );
			OglMaterial material( color );
			materials.push_back( material );
		}
	}
}

void OglModel::toBuffs( Bufferism &buf )
{
	for ( uint i = 0; i < meshes.size(); i++ )
		meshes[i].mesh.toBuffs( &buf );
}

glm::mat4 OglModel::myTransform( glm::mat4 &myRot )
{
	glm::mat4 rotM = glm::mat4( 1 );
	rotM = glm::rotate( rotM, rot.x, glm::vec3( 1, 0, 0 ) );
	rotM = glm::rotate( rotM, rot.x, glm::vec3( 0, 1, 0 ) );
	rotM = glm::rotate( rotM, rot.x, glm::vec3( 0, 0, 1 ) );
	myRot = rotM;

	glm::mat4 mat = glm::mat4(1);
	mat = glm::translate(mat, pos);
	mat = glm::scale(mat, scale);
	mat = rotM*mat;
	return mat;
}

void OglModel::render(map<string, GLint> attrs)
{
	glm::mat4 myRot;
	renderer->setObj(myTransform(myRot));
	renderer->setRot( myRot );
	renderer->transformUpdate();

	for (uint i = 0; i < meshes.size(); i++)
	{
		if (meshes[i].matId != -1)
		{
			OglMaterial *mat = &materials[i];
			renderer->activeShader->importValue("hasTexture", mat->hasTexture ? 1 : 0);
			renderer->activeShader->importValue("mCol", mat->color);
			if (mat->hasTexture)
				renderer->activeShader->importValue("tex", &mat->texture);
		}
		meshes[i].mesh.render( attrs );
	}
}

void OglModel::skyboxRender(map<string, GLint> attrs)
{
	glm::mat4 myRot;
	//renderer->setRot(myRot);
	//renderer->transformUpdate();

	for (uint i = 0; i < meshes.size(); i++)
	{
		OglMaterial *mat = &materials[i];
		renderer->activeShader->importValue("tex", &mat->texture);
		meshes[i].mesh.render(attrs);
	}
}

glm::mat4 OglModel::alignObject( glm::vec3& position, const glm::vec3& front, const glm::vec3& up ) {

	glm::vec3 z = -glm::normalize( front );

	if ( !z.x && !z.y && !z.z )
		z = glm::vec3( 0.0, 0.0, 1.0 );

	glm::vec3 x = glm::normalize( glm::cross( up, z ) );

	if ( !x.x && !x.y && !x.z )
		x = glm::vec3( 1.0, 0.0, 0.0 );

	glm::vec3 y = glm::cross( z, x );
	//mat4 matrix = mat4(1.0f);
	glm::mat4 matrix = glm::mat4(
		x.x, x.y, x.z, 0.0,
		y.x, y.y, y.z, 0.0,
		z.x, z.y, z.z, 0.0,
		position.x, position.y, position.z, 1.0
		);

	return matrix;
}







OglMaterial::OglMaterial( glm::vec3 color, string texString )
{
	this->color = color;
	texture.init( texString );
	hasTexture = true;
}