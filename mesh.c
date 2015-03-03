#include <stdlib.h>
#include <string.h>

#include "mesh.h"
#include "globals.h"

mesh_t *addMesh(meshGroup_t *meshGroup)
{
   mesh_t *newMesh = (mesh_t *)calloc(1, sizeof(mesh_t));
   newMesh->next = meshGroup->meshes;
   meshGroup->meshes = newMesh;
   meshGroup->nMeshes++;

   return newMesh;
}

void deleteGroup(meshGroup_t *group)
{
   mesh_t *mesh = group->meshes;
   while(mesh) {
      if(mesh->colors)     free(mesh->colors);
      if(mesh->vertices)   free(mesh->vertices);
      if(mesh->textCoords) free(mesh->textCoords);
      if(mesh->normals)    free(mesh->normals);
      if(mesh->indices)    free(mesh->indices);
      if(mesh->texture) {
         free(mesh->texture->imageData);
         glDeleteTextures(1, &mesh->texture->texID);
         free(mesh->texture);
      }

      mesh_t *oldMesh = mesh;
      mesh = mesh->next;
      free(oldMesh);
   }

   group->nMeshes = 0;
   group->meshes = NULL;
}

void drawGroup(const meshGroup_t *group)
{
   vertex_t *mesh_transformedVerts, *tmpVerts;

   mesh_t *mesh = group->meshes;
   while(mesh) {
      /// If mesh has texture enable it
      if(mesh->texture && mesh->textCoords) {
         glEnable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D, mesh->texture->texID);
      } else {
         glDisable(GL_TEXTURE_2D);
      }

      drawMesh(mesh);
      mesh = mesh->next;
   }

   glFlush();
}

void drawMesh(const mesh_t *mesh)
{
   if(mesh->vertices) {
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(4, GL_FLOAT, 0, mesh->vertices);
   }

   if(mesh->colors) {
      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(4, GL_FLOAT, 0, mesh->colors);
   }

   if(mesh->textCoords) {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, 0, mesh->textCoords);
   }

   if(mesh->normals) {
      glEnableClientState(GL_NORMAL_ARRAY);
      glNormalPointer(GL_FLOAT, 0, mesh->normals);
   }

   if(!mesh->indices) {
      glDrawArrays(GL_TRIANGLES, 0, mesh->nVertices);
   } else {
      glDrawElements(GL_TRIANGLES, 3 * mesh->nFaces, GL_UNSIGNED_SHORT, mesh->indices);
   }

   if(mesh->vertices) {
      glDisableClientState(GL_VERTEX_ARRAY);
   }

   if(mesh->colors) {
      glDisableClientState(GL_COLOR_ARRAY);
   }

   if(mesh->textCoords) {
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   }

   if(mesh->normals) {
      glDisableClientState(GL_NORMAL_ARRAY);
   }
}
