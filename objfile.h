#ifndef _OBJFILE_H_
#define _OBJFILE_H_

#include "globals.h"

/* GLMmaterial: Structure that defines a material in a model.
 */

#define HAS_VERTICES    (1)         /// Obviously this must always happen
#define HAS_TEXCOORDS   (1 << 1)
#define HAS_NORMALS     (1 << 2)

typedef struct _GLMmaterial
{
  char* name;				/* name of material */
  float diffuse[4];		/* diffuse component */
  float ambient[4];		/* ambient component */
  float specular[4];		/* specular component */
  float emmissive[4];	/* emmissive component */
  float shininess;		/* specular exponent */
  char *texture;        /// Texture file name
} GLMmaterial;

/* GLMtriangle: Structure that defines a triangle in a model.
 */
typedef struct _GLMtriangle {
  unsigned int vindices[3];		/* array of triangle vertex indices */
  unsigned int nindices[3];		/* array of triangle normal indices */
  unsigned int tindices[3];		/* array of triangle texcoord indices*/
  unsigned int findex;			   /* index of triangle facet normal */
} GLMtriangle;

/* GLMgroup: Structure that defines a group in a model.
 */
typedef struct _GLMgroup {
  char               *name;		   /* name of this group */
  char               properties;    /// HAS_TEXCOORDS | HAS_NORMALS ?
  unsigned int       numtriangles;	/* number of triangles in this group */
  unsigned int       *triangles;		/* array of triangle indices */
  unsigned int       material;      /* index to material for group */
  struct _GLMgroup   *next;		   /* pointer to next group in model */
} GLMgroup;

/* GLMmodel: Structure that defines a model.
 */
typedef struct _GLMmodel {
  char            *pathname;			/* path to this model */
  char            *mtllibname;		/* name of the material library */

  unsigned int    numvertices;		/* number of vertices in model */
  float           *vertices;			/* array of vertices  */

  unsigned int    numnormals;			/* number of normals in model */
  float           *normals;			/* array of normals */

  unsigned int    numtexcoords;		/* number of texcoords in model */
  float           *texcoords;			/* array of texture coordinates */

  unsigned int    numfacetnorms;		/* number of facetnorms in model */
  float           *facetnorms;		/* array of facetnorms */

  unsigned int    numtriangles;		/* number of triangles in model */
  GLMtriangle     *triangles;		   /* array of triangles */

  unsigned int    nummaterials;		/* number of materials in model */
  GLMmaterial     *materials;		   /* array of materials */

  unsigned int    numgroups;		   /* number of groups in model */
  GLMgroup        *groups;			   /* linked list of groups */

  float           position[3];		/* position of the model */

} GLMmodel;

#endif
