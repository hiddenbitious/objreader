#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <GL/gl.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>
#include "mesh.h"
#include "tgaLoader/texture.h"

typedef struct {
    float   m[4][4];
} ESMatrix;

//extern ESMatrix globalModelviewMatrix, globalProjectionMatrix, globalMVP;

char *strdup(const char *str);
void calculateNormals(mesh_t *mesh);
void calculateGroupNormals(meshGroup_t *group);
void offReader(const char *filename, mesh_t *mesh, float scale, const char reverse);
void plyReader(const char *filename, mesh_t *mesh, float scale, char reverse);
void glmReadOBJ(const char* filename, meshGroup_t *meshgroup, float scale);
int LoadShaderSource(const char* filename, char **shaderSource, unsigned int *length);
void transformMesh(const ESMatrix *matrix, const vertex_t *vertices, vertex_t *transformedVerts, int nVertices);
void crossProduct(const vertex_t *v1, const vertex_t *v2, const vertex_t *v3, vertex_t *cross);
void Normalize(float *x, float *y, float *z);
void normalize3f(vertex3f_t *vec);
void normalize(vertex_t *vec);

static inline float dotProduct(const vertex_t* vec1, const vertex_t* vec2)
{
	return vec1->x * vec2->x + vec1->y * vec2->y + vec1->z * vec2->z;
}

static inline float dotProduct3f(const vertex3f_t* vec1, const vertex3f_t* vec2)
{
	return vec1->x * vec2->x + vec1->y * vec2->y + vec1->z * vec2->z;
}

static inline float magnitude(const vertex_t *vec)
{
	return (sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z));
}

static inline float magnitude3f(const vertex3f_t *vec)
{
	return (sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z));
}

//void loadGLTexture(const char *filename, Texture **texture);

#endif
