#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "mesh.h"
#include "plyreader/ply.h"

char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup) strcpy(dup, str);
    return dup;
}

void calculateGroupNormals(meshGroup_t *group)
{
   mesh_t *mesh = group->meshes;

   while(mesh) {
      calculateNormals(mesh);
      mesh = mesh->next;
   }
}

void calculateNormals(mesh_t *mesh)
{
   if(mesh->normals)
      free(mesh->normals);

   mesh->normals = (vertex3f_t *) malloc(mesh->nVertices * sizeof(vertex3f_t));
   assert(mesh->normals);
   memset((void *)mesh->normals, 0, mesh->nVertices* sizeof(vertex3f_t));

   char *connections = (char *)malloc(mesh->nVertices * sizeof(char));
   memset(connections, 0, mesh->nVertices * sizeof(char));

   vertex_t tempNorm;

   int i;
   uint16_t *indices = mesh->indices;
   vertex_t *vertices = mesh->vertices;
   vertex3f_t *normals = mesh->normals;
      for(i = 0; i < mesh->nFaces; i++) {
         crossProduct(&vertices[indices[3 * i    ]],
                      &vertices[indices[3 * i + 1]],
                      &vertices[indices[3 * i + 2]],
                      &tempNorm);

         normals[indices[3 * i    ]].x += tempNorm.x;
         normals[indices[3 * i    ]].y += tempNorm.y;
         normals[indices[3 * i    ]].z += tempNorm.z;

         normals[indices[3 * i + 1]].x += tempNorm.x;
         normals[indices[3 * i + 1]].y += tempNorm.y;
         normals[indices[3 * i + 1]].z += tempNorm.z;

         normals[indices[3 * i + 2]].x += tempNorm.x;
         normals[indices[3 * i + 2]].y += tempNorm.y;
         normals[indices[3 * i + 2]].z += tempNorm.z;
      }

  	for(i = 0; i < mesh->nVertices; i++) {
		if (connections[i]) {
			normals[i].x /= connections[i];
			normals[i].y /= connections[i];
			normals[i].z /= connections[i];
	}

      normalize3f(&normals[i]);
	}

	free(connections);
}

void normalize3f(vertex3f_t *vec)
{
   float len = magnitude3f(vec);

   vec->x /= len;
   vec->y /= len;
   vec->z /= len;
}

void crossProduct(const vertex_t *v1, const vertex_t *v2, const vertex_t *v3, vertex_t *cross)
{
	cross->x = (v1->y - v2->y) * (v2->z - v3->z) - (v2->y - v3->y) * (v1->z - v2->z);
	cross->y = (v2->x - v3->x) * (v1->z - v2->z) - (v1->x - v2->x) * (v2->z - v3->z);
	cross->z = (v1->x - v2->x) * (v2->y - v3->y) - (v2->x - v3->x) * (v1->y - v2->y);
}

void normalize(vertex_t *vec)
{
   float len = magnitude(vec);

   vec->x /= len;
   vec->y /= len;
   vec->z /= len;
}

void Normalize(float *x, float *y, float *z)
{
   vertex_t vec;
   vec.x = *x;
   vec.y = *y;
   vec.z = *z;

   float len = magnitude(&vec);

   *x /= len;
   *y /= len;
   *z /= len;
}


void offReader(const char *filename, mesh_t *mesh, float scale, const char reverse)
{
   int nverts = 0, nfaces = 0, ntris = 0, tmp, i, j, i1, i2, i3, count;
   float f1, f2, f3;
   char c, head[4];

   printf("Reading file: %s\n", filename);

   FILE *f;
   f = fopen(filename, "r");
   if (!f) {
      printf("Cannot open mesh file '%s'!\n", filename);
      abort();
   }

   /// Discard first line
   fscanf(f, "%s", head);
   fscanf(f, "%c", &c);

   /// Read number of vertices and number of faces
   fscanf(f, "%d %d %d", &nverts, &nfaces, &tmp);
   fscanf(f, "%c", &c);

   printf("nverts = %d - nfaces = %d\n", nverts, nfaces);

   /// Run a first pass through the file, bypass vertices and count the number of triangles
   for(i = 0; i < nverts; ++i) {
      fscanf(f, "%f %f %f", &f1, &f2, &f3);
      fscanf(f, "%c", &c);
   }

   for(i = 0; i < nfaces; ++i) {
      fscanf(f, "%d", &tmp);
      assert(tmp >= 3);
      ntris += tmp - 2;

      for(j = 0; j < tmp; ++j)
         fscanf(f, "%f", &f1);

      fscanf(f, "%c", &c);
   }

   /// Init mesh struct
   memset(mesh, 0, sizeof(mesh_t));

   /// Allocate memory
   mesh->nVertices = nverts;
   mesh->nFaces = ntris;
   mesh->vertices = (vertex_t *)malloc(nverts * sizeof(vertex_t));
   if(!mesh->vertices) {
      printf("Out of memory...\n");
      abort();
   }

   mesh->indices = (uint16_t *)malloc(ntris * 3 * sizeof(uint16_t));
   if(!mesh->indices) {
      printf("Out of memory...\n");
      abort();
   }

   /// Rewind file and read data
   fseek(f, 0L, SEEK_SET);

   fscanf(f, "%s", head);
   fscanf(f, "%c", &c);

   /// Number of vertices and faces now is known. Discard
   fscanf(f, "%d %d %d", &i1, &i2, &i3);
   fscanf(f, "%c", &c);

   vertex_t *vp = mesh->vertices;
   for(i = 0; i < nverts; ++i) {
      fscanf(f, "%f %f %f", &f1, &f2, &f3);
      fscanf(f, "%c", &c);
      //      printf("%f %f %f\n", f1, f2, f3);

      vp[i].x = f1 /* * scale */;
      vp[i].y = f2 /* * scale */;
      vp[i].z = f3 /* * scale */;
      vp[i].w = 1.0f;

//      printf("%f %f %f\n", vp[3 * i    ], vp[3 * i + 1], vp[3 * i + 2]);
   }

   uint16_t *ip = mesh->indices;
   for(i = 0, count = 0; i < nfaces; ++i) {
      fscanf(f, "%d %d %d %d", &tmp, &i1, &i2, &i3);

      ip[3 * count    ] = i1;
      ip[3 * count + 1] = reverse ? i3 : i2;
      ip[3 * count + 2] = reverse ? i2 : i3;
      ++count;

      for(j = 3; j < tmp; ++j) {
         i2 = i3;
         fscanf(f, "%d", &i3);

         ip[3 * count    ] = i1;
         ip[3 * count + 1] = reverse ? i3 : i2;
         ip[3 * count + 2] = reverse ? i2 : i3;
         ++count;
      }
      fscanf(f, "%c", &c);
   }

   assert(count == ntris);
   fclose(f);
}

int LoadShaderSource(const char* filename, char **shaderSource, unsigned int *length)
{
	FILE *file;
	file = fopen(filename, "r");
	if(!file) {
      printf("Error opening file %s\n", filename);
		return 0;
	}

	fseek (file, 0, SEEK_END);
	unsigned int len = ftell(file);
	fseek (file, 0, SEEK_SET);

	if(!len) {
		return 0;
	}

	*length = len;
	*shaderSource = (char *) malloc((len + 1) * sizeof(char));
	assert(*shaderSource);
	char *src = *shaderSource;

	// len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value...
	fread(src, len, 1, file);
	src[len] = '\0';

   fclose(file);

	return 1;
}

//void transformMesh(const ESMatrix *matrix, const vertex_t *vertices, vertex_t *transformedVerts, int nVertices)
//{
//   for (int i = 0; i < nVertices; i++)
//      transformedVerts[i] = transformPoint(matrix, &vertices[i]);
//}

typedef struct Vertex {
  float x,y,z;
  float nx,ny,nz;
  float r,g,b,a;
  float s, t;
} ply_vertex_t;

typedef struct Face {
  unsigned char nverts;    /* number of vertex indices in list */
  int *verts;              /* vertex index list */
  void *other_props;       /* other properties */
} ply_face_t;

void plyReader(const char *filename, mesh_t *mesh, float scale, char reverse)
{
   int i, j, elemCount;
   char *elemName;
   char hasNormal = 0, hasColor = 0, hasTexCoord = 0;
   ply_vertex_t vert;
   ply_face_t face;

   memset((void *)mesh, 0, sizeof(mesh_t));

   PlyProperty vert_props[] = { /* list of property information for a vertex */
                              {"x", Float32, Float32, offsetof(ply_vertex_t, x), 0, 0, 0, 0},
                              {"y", Float32, Float32, offsetof(ply_vertex_t, y), 0, 0, 0, 0},
                              {"z", Float32, Float32, offsetof(ply_vertex_t, z), 0, 0, 0, 0},
                              {"nx", Float32, Float32, offsetof(ply_vertex_t, nx), 0, 0, 0, 0},
                              {"ny", Float32, Float32, offsetof(ply_vertex_t, ny), 0, 0, 0, 0},
                              {"nz", Float32, Float32, offsetof(ply_vertex_t, nz), 0, 0, 0, 0}};

   PlyProperty face_props[] = { /* list of property information for a face */
                              {"vertex_index", Int32, Int32, offsetof(ply_face_t,verts), 1, Uint8, Uint8, offsetof(ply_face_t, nverts)}};

   FILE *fp = fopen(filename, "r");
   PlyFile *plymesh = read_ply(fp);

   for(i = 0; i < plymesh->num_elem_types; i++) {
      elemName = setup_element_read_ply(plymesh, i, &elemCount);

      /// Read mesh vertices
      if (equal_strings ("vertex", elemName)) {
         printf("Number of vertices: %d\n", elemCount);

         setup_property_ply (plymesh, &vert_props[0]);
         setup_property_ply (plymesh, &vert_props[1]);
         setup_property_ply (plymesh, &vert_props[2]);

         for (j = 0; j < plymesh->elems[i]->nprops; j++) {
            PlyProperty *prop;
            prop = plymesh->elems[i]->props[j];
            if (equal_strings ("nx", prop->name)) {
               setup_property_ply (plymesh, &vert_props[3]);
               hasNormal = 1;
            }

            if (equal_strings ("ny", prop->name)) {
               setup_property_ply (plymesh, &vert_props[4]);
               hasNormal = 1;
            }

            if (equal_strings ("nz", prop->name)) {
               setup_property_ply (plymesh, &vert_props[5]);
               hasNormal = 1;
            }
         }

         /// Allocate memory
         mesh->vertices = (vertex_t *)malloc(elemCount * sizeof(vertex_t));
         mesh->nVertices = elemCount;

         /// If ply contains normals
//         mesh->normals = (vertex3f_t *)(hasNormal ? malloc(elemCount * sizeof(vertex3f_t)) : NULL);
         /// If ply contains colors
//         mesh->colors = (vertex_t *)(hasColor ? malloc(elemCount * sizeof(vertex_t)) : NULL);
         /// If ply contains texture coordinates
//         mesh->textCoords = (vertex2f_t *)(hasTexCoord ? malloc(elemCount * sizeof(vertex2f_t)): NULL);

         for(j = 0; j < elemCount; j++) {
            get_element_ply (plymesh, (void *) &vert);

//            printf("\t%f %f %f\n", vert.x, vert.y, vert.z);

            mesh->vertices[j].x = vert.x;
            mesh->vertices[j].y = vert.y;
            mesh->vertices[j].z = vert.z;
            mesh->vertices[j].w = 1.0f;
         }
      } else if (equal_strings("face", elemName)) {
         /// Read mesh faces
         printf("Number of faces: %d\n", elemCount);
         mesh->nFaces = elemCount;
         mesh->indices = (uint16_t *)malloc(3 * elemCount * sizeof(uint16_t));

         setup_property_ply (plymesh, &face_props[0]);

         for (j = 0; j < elemCount; j++) {
            get_element_ply (plymesh, (void *) &face);

            mesh->indices[j * 3    ] = face.verts[0];
            mesh->indices[j * 3 + 1] = face.verts[1];
            mesh->indices[j * 3 + 2] = face.verts[2];

//            printf("%d %d %d\n", face.verts[0], face.verts[1], face.verts[2]);
         }
      } else {
         assert(0);
      }
   }
}
