#include "AnimatedMesh.h"
#include "ResourceManager.h"

#ifdef WIN32
#include <assert.h>
#endif

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2
#define TANGENT_LOCATION 3
#define BONE_ID_LOCATION 4
#define BONE_WEIGHT_LOCATION 5

AnimatedMesh::VertexBoneData::VertexBoneData() { Reset(); }
void AnimatedMesh::VertexBoneData::Reset() {
  memset(IDs, 0, sizeof(IDs));
  memset(Weights, 0.f, sizeof(Weights));
}

void AnimatedMesh::VertexBoneData::AddBoneData(unsigned int BoneID,
                                               float Weight) {
  for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++) {
    if (Weights[i] == 0.0f) {
      IDs[i] = BoneID;
      Weights[i] = Weight;
      return;
    }
  }
  assert(0);
}

AnimatedMesh::AnimatedMesh() {
  m_pScene = NULL;
  m_NumBones = 0;
}

AnimatedMesh::~AnimatedMesh() {}
bool AnimatedMesh::LoadMesh(const std::string &Filename) {
  // Release the previously loaded mesh (if it exists)
  Clear();

  bool Ret = false;

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glGenBuffers(NUM_VBs, m_Buffers);

  m_pScene = m_Importer.ReadFile(
      Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                            aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

  if (m_pScene) {
    m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
    m_GlobalInverseTransform.Inverse();
    Ret = InitFromScene(m_pScene, Filename);
  } else {
    printf("Error parsing '%s': '%s'\n", Filename.c_str(),
           m_Importer.GetErrorString());
  }

  glBindVertexArray(0);
  return Ret;
}

void AnimatedMesh::Render(bool withOutline) {
  glUniform1i(glGetUniformLocation(ShaderManager::getInstance()->getProgram(),
                                   "skeletalAnimation"),
              true);
  Mesh::Render(withOutline);
  glUniform1i(glGetUniformLocation(ShaderManager::getInstance()->getProgram(),
                                   "skeletalAnimation"),
              false);
}
bool AnimatedMesh::RenderWithOutline(GLfloat red, GLfloat green, GLfloat blue,
                                     GLfloat thickness) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  Render();
  glEnable(GL_CULL_FACE);
  glCullFace(GL_CCW);
  glDepthMask(GL_TRUE);
  glDisable(GL_LIGHTING);
  Render(1);
  glPopAttrib();

  return true;
}

bool AnimatedMesh::InitFromScene(const aiScene *pScene,
                                 const std::string &Filename) {
  m_Entries.resize(pScene->mNumMeshes);
  m_Textures.resize(pScene->mNumMaterials);

  std::vector<Vector3f> Positions;
  std::vector<Vector3f> Normals;
  std::vector<Vector3f> Tangents;
  std::vector<Vector2f> TexCoords;
  std::vector<unsigned int> Indices;
  std::vector<VertexBoneData> Bones;
  unsigned int NumVertices = 0;
  unsigned int NumIndices = 0;

  for (unsigned int i = 0; i < m_Entries.size(); i++) {
    m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
    m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
    m_Entries[i].baseVertex = NumVertices;
    m_Entries[i].baseIndex = NumIndices;

    NumVertices += pScene->mMeshes[i]->mNumVertices;
    NumIndices += m_Entries[i].NumIndices;
  }

  Positions.reserve(NumVertices);
  Normals.reserve(NumVertices);
  TexCoords.reserve(NumVertices);
  Tangents.reserve(NumVertices);
  Indices.reserve(NumVertices);
  Bones.resize(NumVertices);

  // Initialize the meshes in the scene one by one
  for (unsigned int i = 0; i < m_Entries.size(); i++) {
    const aiMesh *paiMesh = pScene->mMeshes[i];
    InitMesh(i, paiMesh, Positions, Normals, TexCoords, Indices, Tangents,
             Bones);
  }
  if (!InitMaterials(pScene, Filename)) {
    return false;
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(),
               &Positions[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(POSITION_LOCATION);
  glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(),
               &TexCoords[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(TEX_COORD_LOCATION);
  glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(),
               &Normals[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(NORMAL_LOCATION);
  glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TANGENT_VB]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Tangents[0]) * Tangents.size(),
               &Tangents[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(TANGENT_LOCATION);
  glVertexAttribPointer(TANGENT_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0],
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(BONE_ID_LOCATION);
  glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData),
                         (const GLvoid *)0);
  glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
  glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE,
                        sizeof(VertexBoneData),
                        (const GLvoid *)(4 * NUM_BONES_PER_VERTEX));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(),
               &Indices[0], GL_STATIC_DRAW);

  return true;
}

void AnimatedMesh::InitMesh(unsigned int MeshIndex, const aiMesh *paiMesh,
                            std::vector<Vector3f> &Positions,
                            std::vector<Vector3f> &Normals,
                            std::vector<Vector2f> &TexCoord,
                            std::vector<unsigned int> &Indices,
                            std::vector<Vector3f> &Tangents,
                            std::vector<VertexBoneData> &Bones) {
  Mesh::InitMesh(paiMesh, Positions, Normals, TexCoord, Indices, Tangents);
  LoadBones(MeshIndex, paiMesh, Bones);
}

void AnimatedMesh::LoadBones(unsigned int MeshIndex, const aiMesh *pMesh,
                             std::vector<VertexBoneData> &Bones) {
  for (unsigned int i = 0; i < pMesh->mNumBones; i++) {
    unsigned int BoneIndex = 0;
    std::string BoneName(pMesh->mBones[i]->mName.data);

    if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
      BoneIndex = m_NumBones;
      m_NumBones++;
      BoneInfo bi;
      m_BoneInfo.push_back(bi);
      m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;
      m_BoneMapping[BoneName] = BoneIndex;
    } else {
      BoneIndex = m_BoneMapping[BoneName];
    }

    for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
      unsigned int VertexID = m_Entries[MeshIndex].baseVertex +
                              pMesh->mBones[i]->mWeights[j].mVertexId;
      float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
      Bones[VertexID].AddBoneData(BoneIndex, Weight);
    }
  }
}

unsigned int AnimatedMesh::FindPosition(float AnimationTime,
                                        const aiNodeAnim *pNodeAnim) {
  for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
    if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
      return i;
    }
  }
  assert(0);
  return 0;
}

unsigned int AnimatedMesh::FindRotation(float AnimationTime,
                                        const aiNodeAnim *pNodeAnim) {
  for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
    if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
      return i;
    }
  }
  assert(0);
  return 0;
}

unsigned int AnimatedMesh::FindScaling(float AnimationTime,
                                       const aiNodeAnim *pNodeAnim) {
  for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
    if (AnimationTime <= (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
      return i;
    }
  }
  assert(0);
  return 0;
}

void AnimatedMesh::CalcInterpolatedPosition(aiVector3D &Out,
                                            float AnimationTime,
                                            const aiNodeAnim *pNodeAnim) {
  if (pNodeAnim->mNumPositionKeys == 1) {
    Out = pNodeAnim->mPositionKeys[0].mValue;
    return;
  }

  unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
  unsigned int NextPositionIndex = (PositionIndex + 1);

  float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime -
                            pNodeAnim->mPositionKeys[PositionIndex].mTime);

  float Factor =
      (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) /
      DeltaTime;

  const aiVector3D &Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
  const aiVector3D &End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
  aiVector3D Delta = End - Start;

  Out = Start + Factor * Delta;
}

void AnimatedMesh::CalcInterpolatedRotation(aiQuaternion &Out,
                                            float AnimationTime,
                                            const aiNodeAnim *pNodeAnim) {
  // we need at least two values to interpolate...
  if (pNodeAnim->mNumRotationKeys == 1) {
    Out = pNodeAnim->mRotationKeys[0].mValue;
    return;
  }

  unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
  unsigned int NextRotationIndex = (RotationIndex + 1);
  assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
  float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime -
                            pNodeAnim->mRotationKeys[RotationIndex].mTime);
  float Factor =
      (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) /
      DeltaTime;
  assert(Factor >= 0.0f && Factor <= 1.0f);
  const aiQuaternion &StartRotationQ =
      pNodeAnim->mRotationKeys[RotationIndex].mValue;
  const aiQuaternion &EndRotationQ =
      pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
  aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
  Out = Out.Normalize();
}

void AnimatedMesh::CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime,
                                           const aiNodeAnim *pNodeAnim) {
  if (pNodeAnim->mNumScalingKeys == 1) {
    Out = pNodeAnim->mScalingKeys[0].mValue;
    return;
  }

  unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
  unsigned int NextScalingIndex = (ScalingIndex + 1);
  assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
  float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime -
                            pNodeAnim->mScalingKeys[ScalingIndex].mTime);
  float Factor =
      (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) /
      DeltaTime;
  assert(Factor >= 0.0f && Factor <= 1.0f);
  const aiVector3D &Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
  const aiVector3D &End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
  aiVector3D Delta = End - Start;
  Out = Start + Factor * Delta;
}

void AnimatedMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode *pNode,
                                     const Matrix4f &ParentTransform) {
  std::string NodeName(pNode->mName.data);

  const aiAnimation *pAnimation = m_pScene->mAnimations[0];

  Matrix4f NodeTransformation(pNode->mTransformation);

  const aiNodeAnim *pNodeAnim = FindNodeAnim(pAnimation, NodeName);

  if (pNodeAnim) {
    // Interpolate scaling and generate scaling transformation matrix
    aiVector3D Scaling;
    CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
    Matrix4f ScalingM;
    ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

    // Interpolate rotation and generate rotation transformation matrix
    aiQuaternion RotationQ;
    CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
    Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

    // Interpolate translation and generate translation transformation matrix
    aiVector3D Translation;
    CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
    Matrix4f TranslationM;
    TranslationM.InitTranslationTransform(Translation.x, Translation.y,
                                          Translation.z);

    // Combine the above transformations
    NodeTransformation = TranslationM * RotationM * ScalingM;
  }

  Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;

  if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
    unsigned int BoneIndex = m_BoneMapping[NodeName];
    m_BoneInfo[BoneIndex].FinalTransformation =
        m_GlobalInverseTransform * GlobalTransformation *
        m_BoneInfo[BoneIndex].BoneOffset;
  }

  for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
    ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
  }
}

void AnimatedMesh::BoneTransform(float TimeInSeconds,
                                 std::vector<Matrix4f> &Transforms) {
  Matrix4f Identity;
  Identity.InitIdentity();

  float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0
                                     ? m_pScene->mAnimations[0]->mTicksPerSecond
                                     : 25.0f);
  float TimeInTicks = TimeInSeconds * TicksPerSecond;
  float AnimationTime =
      fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

  ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

  Transforms.resize(m_NumBones);

  for (unsigned int i = 0; i < m_NumBones; i++) {
    Transforms[i] = m_BoneInfo[i].FinalTransformation;
  }
}

const aiNodeAnim *AnimatedMesh::FindNodeAnim(const aiAnimation *pAnimation,
                                             const std::string NodeName) {
  for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
    const aiNodeAnim *pNodeAnim = pAnimation->mChannels[i];

    if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
      return pNodeAnim;
    }
  }

  return NULL;
}

void AnimatedMesh::setBoneTransform(unsigned int i, const Matrix4f &Transform) {
  char Name[128];
  memset(Name, 0, sizeof(Name));
  SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);

  glUniformMatrix4fv(
      glGetUniformLocation(ShaderManager::getInstance()->getProgram(), Name), 1,
      GL_TRUE, (const GLfloat *)Transform.m);
}
