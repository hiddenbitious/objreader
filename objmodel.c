#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <GL/glut.h>
#include "mesh.h"
#include "objfile.h"

//#define DRAW_BUNNY   1

mesh_t bunny;
meshGroup_t bunnyGroup;

void display(void)
{
   float angle = 0.0f;
   int frames = 0;
   struct timeval tim;
   double t1, t2;
   float fps = 0.0f;
   t1 = t2 = 0.0;

   gettimeofday(&tim, NULL);
   t1 = tim.tv_sec + (tim.tv_usec / 1000000.0);

   while(1) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glLoadIdentity();
      glTranslatef(0.0f, -0.0f, -0.5f);
      glRotatef(angle, 0.1f, 1.0f, 0.0f);

      angle += 15.0f;
      if(angle >= 360.0f)
         angle = 0.0f;

#     ifdef DRAW_BUNNY
         drawMesh(&bunny);
#     else
         drawGroup(&bunnyGroup);
#     endif

      glutSwapBuffers();

      ++frames;
      if(frames == 20) {
         gettimeofday(&tim, NULL);
         t2 = tim.tv_sec + (tim.tv_usec / 1000000.0);

         fps = (float)frames / (t2 - t1);
         printf("fps: %f\n", fps);

         t1 = t2;
         frames = 0;
      }
   }
}

void reshape(int w, int h)
{
	if (h==0) {
		h=1;
	}

	glViewport(0,0,w,h);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
}

void init(void)
{
#  ifdef DRAW_BUNNY
      plyReader("models/bun_zipper_res3.ply", &bunny, 1.0f, 0);
      calculateNormals(&bunny);
#  else
      glmReadOBJ("models/teapot.obj", &bunnyGroup, 0.02f);
//      glmReadOBJ("models/cube.obj", &bunnyGroup, 0.2f);
#  endif

	glShadeModel(GL_SMOOTH);							      // Enable Smooth Shading
	glClearColor(0.20f, 0.48f, 0.95f, 1.0f);				// Black Background

   GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };

   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

//   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDisable(GL_CULL_FACE);							// Enables Depth Testing
	glDepthMask(GL_TRUE);
}

int main(int argc, char** argv)
{
   glutInit (&argc, argv);
   glutInitWindowSize (800, 600);
   glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutCreateWindow ("lol bunny");
   // Initialize OpenGL graphics state
   init();
   // Register callbacks:
   glutDisplayFunc (display);
   glutReshapeFunc (reshape);

   glutMainLoop();

   return 0;
}

