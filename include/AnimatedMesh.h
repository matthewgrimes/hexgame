#ifndef ANIMATEDMESH_H
#define ANIMATEDMESH_H

#include "Mesh.h"
#include <map>

class AnimatedMesh : public Mesh {

public:
  AnimatedMesh();
  ~AnimatedMesh();
  bool LoadMesh(const std::string &Filename);
  bool InitFromScene(const aiScene *pScene, const std::string &Filename);

  virtual void Render(bool withOutline = 0);
  virtual bool RenderWithOutline(GLfloat red, GLfloat green, GLfloat blue,
                                 GLfloat thickness);

  unsigned int NumBones() const { return m_NumBones; }

  void BoneTransform(float TimeInSeconds, std::vector<Matrix4f> &Transforms);

  void setBoneTransform(unsigned int i, const Matrix4f &Transform);

private:
#define NUM_BONES_PER_VERTEX 5
  struct BoneInfo {
    Matrix4f BoneOffset;
    Matrix4f FinalTransformation;

    BoneInfo() {
      BoneOffset.SetZero();
      FinalTransformation.SetZero();
    }
  };

  struct VertexBoneData {
    unsigned int IDs[NUM_BONES_PER_VERTEX];
    float Weights[NUM_BONES_PER_VERTEX];

    VertexBoneData();

    void Reset();

    void AddBoneData(unsigned int BoneID, float Weight);
  };

  void InitMesh(unsigned int MeshIndex, const aiMesh *paiMesh,
                std::vector<Vector3f> &Positions,
                std::vector<Vector3f> &Normals, std::vector<Vector2f> &TexCoord,
                std::vector<unsigned int> &Indices,
                std::vector<Vector3f> &Tangents,
                std::vector<VertexBoneData> &Bones);

  void CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime,
                               const aiNodeAnim *pNodeAnim);
  void CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTime,
                                const aiNodeAnim *pNodeAnim);
  void CalcInterpolatedPosition(aiVector3D &Out, float AnimationTime,
                                const aiNodeAnim *pNodeAnim);
  unsigned int FindScaling(float AnimationTime, const aiNodeAnim *pNodeAnim);
  unsigned int FindRotation(float AnimationTime, const aiNodeAnim *pNodeAnim);
  unsigned int FindPosition(float AnimationTime, const aiNodeAnim *pNodeAnim);
  const aiNodeAnim *FindNodeAnim(const aiAnimation *pAnimation,
                                 const std::string NodeName);
  void ReadNodeHeirarchy(float AnimationTime, const aiNode *pNode,
                         const Matrix4f &ParentTransform);
  void LoadBones(unsigned int MeshIndex, const aiMesh *paiMesh,
                 std::vector<VertexBoneData> &Bones);

  std::map<std::string, unsigned int>
      m_BoneMapping; // maps a bone name to its index
  unsigned int m_NumBones;
  std::vector<BoneInfo> m_BoneInfo;
  Matrix4f m_GlobalInverseTransform;

  const aiScene *m_pScene;
  Assimp::Importer m_Importer;
};

#endif
