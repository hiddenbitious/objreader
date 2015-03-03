#ifndef _MESH_H_
#define _MESH_H_

#include <stdint.h>
#include "tgaLoader/texture.h"

typedef struct {
   float x, y;
} vertex2f_t;

typedef struct {
   float x, y, z;
} vertex3f_t;

typedef struct {
   float x, y, z, w;
} vertex_t;

typedef struct mesh_t_ {
   vertex_t       *vertices;     /// vertices
   vertex_t       *colors;       /// colors
   vertex2f_t     *textCoords;   /// texture coordinates
   vertex3f_t     *normals;      /// normals
   uint16_t       *indices;      /// Vertex indices
   struct mesh_t_ *next;         /// Pointer to next mesh in meshGroup
   Texture        *texture;      /// Pointer to texture struct

   int            nVertices;     /// Number of vertices in mesh
   int            nFaces;        /// Number of triangles in mesh
} mesh_t;

typedef struct meshGroup_t_ {
   mesh_t         *meshes;       /// Linked list of meshes in group
   int            nMeshes;       /// Number of meshes in group
   vertex_t       position;      /// Group's position
} meshGroup_t;

mesh_t *addMesh(meshGroup_t *meshGroup);
void deleteGroup(meshGroup_t *group);
void drawGroup(const meshGroup_t *meshGroup);
void drawMesh(const mesh_t *mesh);

#endif
