// Copyright 2008 Isis Innovation Limited
#include "ucsp.h"
#include "OpenGL.h"
//#include <GL/glut.h>
#include <cvd/convolution.h>
#include <fstream> //ifstream
#include <sstream> //stringstream
#include <iostream>
#include <vector>
#include <string>
#include <algorithm> //sort() 

using namespace CVD;
using namespace std;

extern struct model rabbit;

extern bool flagDragon;
extern bool flagWoola;
extern bool flagRabbit;
extern bool flagUCSP;

#define LETTER_U_FRONTAL 8
#define LETTER_U_SIDE 18

#define LETTER_C_FRONTAL 8
#define LETTER_C_SIDE 18

#define LETTER_S_FRONTAL 12
#define LETTER_S_SIDE 26

#define LETTER_P_FRONTAL 10
#define LETTER_P_SIDE 22

// Color
GLfloat c[8][3] = {{1,1,1},{1,1,1},{1,1,1},{1,1,1},{1,1,1},{1,1,1},{1,1,1},{1,1,1}}; //NUMVTX


/* First letter, also front and back */
GLfloat l_u[LETTER_U_SIDE][3] = { {-1.0,0.5,0.4},{-1.0,0.5,-0.4},{-0.9,0.5,0.4},{-0.9,0.5,-0.4},
                             {-0.9,0.1,0.4},{-0.9,0.1,-0.4},{-0.7,0.1,0.4},{-0.7,0.1,-0.4},
                             {-0.7,0.5,0.4},{-0.7,0.5,-0.4},{-0.6,0.5,0.4},{-0.6,0.5,-0.4},
                             {-0.6,0.0,0.4},{-0.6,0.0,-0.4},{-1.0,0.0,0.4},{-1.0,0.0,-0.4},
                             {-1.0,0.5,0.4},{-1.0,0.5,-0.4} };

GLfloat l_u_f[LETTER_U_FRONTAL][3] = { {-0.7,0.5,0.4},{-0.6,0.5,0.4},{-0.7,0.1,0.4},{-0.6,0.0,0.4},
                                {-0.9,0.1,0.4},{-1.0,0.0,0.4},{-0.9,0.5,0.4},{-1.0,0.5,0.4} };

GLfloat l_u_b[LETTER_U_FRONTAL][3] = { {-0.7,0.5,-0.4},{-0.6,0.5,-0.4},{-0.7,0.1,-0.4},{-0.6,0.0,-0.4},
                                {-0.9,0.1,-0.4},{-1.0,0.0,-0.4},{-0.9,0.5,-0.4},{-1.0,0.5,-0.4} };


/* Second letter, also front and back */
GLfloat l_c[LETTER_C_SIDE][3] = { {-0.5,0.5,0.4},{-0.5,0.5,-0.4},{-0.1,0.5,0.4},{-0.1,0.5,-0.4},
                                {-0.1,0.4,0.4},{-0.1,0.4,-0.4},{-0.4,0.4,0.4},{-0.4,0.4,-0.4},
                                {-0.4,0.1,0.4},{-0.4,0.1,-0.4},{-0.1,0.1,0.4},{-0.1,0.1,-0.4},
                                {-0.1,0,0.4},{-0.1,0,-0.4},{-0.5,0.0,0.4},{-0.5,0.0,-0.4},
                                {-0.5,0.5,0.4},{-0.5,0.5,-0.4} };

GLfloat l_c_f[LETTER_C_FRONTAL][3] = { {-0.1,0.5,0.4},{-0.1,0.4,0.4},{-0.5,0.5,0.4},{-0.4,0.4,0.4},
                                {-0.5,0,0.4},{-0.4,0.1,0.4},{-0.1,0,0.4},{-0.1,0.1,0.4}};

GLfloat l_c_b[LETTER_C_FRONTAL][3] = { {-0.1,0.5,-0.4},{-0.1,0.4,-0.4},{-0.5,0.5,-0.4},{-0.4,0.4,-0.4},
                                {-0.5,0,-0.4},{-0.4,0.1,-0.4},{-0.1,0.0,-0.4},{-0.1,0.1,-0.4} };



/* Third letter, also front and back */
GLfloat l_s[LETTER_S_SIDE][3] = { {0.0,0.0,0.4},{0.0,0.0,-0.4},{0.0,0.1,0.4},{0.0,0.1,-0.4},
                                {0.3,0.1,0.4},{0.3,0.1,-0.4},{0.3,0.2,0.4},{0.3,0.2,-0.4},
                                {0.0,0.2,0.4},{0.0,0.2,-0.4},{0.0,0.5,0.4},{0.0,0.5,-0.4},
                                {0.4,0.5,0.4},{0.4,0.5,-0.4},{0.4,0.4,0.4},{0.4,0.4,-0.4},
                                {0.1,0.4,0.4},{0.1,0.4,-0.4},{0.1,0.3,0.4},{0.1,0.3,-0.4},
                                {0.4,0.3,0.4},{0.4,0.3,-0.4},{0.4,0.0,0.4},{0.4,0.0,-0.4},
                                {0.0,0.0,0.4},{0.0,0.0,-0.4}
                            };


GLfloat l_s_f[LETTER_S_FRONTAL][3] = { {0.0,0.0,0.4},{0.0,0.1,0.4},{0.4,0.0,0.4},{0.3,0.1,0.4},
                                {0.4,0.3,0.4},{0.3,0.2,0.4},{0.1,0.3,0.4},{0.0,0.2,0.4},
                                {0.1,0.4,0.4},{0.0,0.5,0.4},{0.4,0.4,0.4},{0.4,0.5,0.4} };

GLfloat l_s_b[LETTER_S_FRONTAL][3] = { {0.0,0.0,-0.4},{0.0,0.1,-0.4},{0.4,0.0,-0.4},{0.3,0.1,-0.4},
                                {0.4,0.3,-0.4},{0.3,0.2,-0.4},{0.1,0.3,-0.4},{0.0,0.2,-0.4},
                                {0.1,0.4,-0.4},{0.0,0.5,-0.4},{0.4,0.4,-0.4},{0.4,0.5,-0.4} };


/* Fourth letter, also front and back */
GLfloat l_p[LETTER_P_SIDE][3] = { {0.5,0.0,0.4},{0.5,0.0,-0.4},{0.5,0.5,0.4},{0.5,0.5,-0.4},
                            {0.9,0.5,0.4},{0.9,0.5,-0.4},{0.9,0.2,0.4},{0.9,0.2,-0.4},
                            {0.6,0.2,0.4},{0.6,0.2,-0.4},{0.6,0.3,0.4},{0.6,0.3,-0.4},
                            {0.8,0.3,0.4},{0.8,0.3,-0.4},{0.8,0.4,0.4},{0.8,0.4,-0.4},
                            {0.6,0.4,0.4},{0.6,0.4,-0.4},{0.6,0.0,0.4},{0.6,0.0,-0.4},
                            {0.5,0.0,0.4},{0.5,0.0,-0.4}
                            };

GLfloat l_p_f[LETTER_P_FRONTAL][3] = { {0.6,0.0,0.4},{0.5,0.0,0.4},{0.6,0.4,0.4},{0.5,0.5,0.4},
                                        {0.8,0.4,0.4},{0.9,0.5,0.4},{0.8,0.3,0.4},{0.9,0.2,0.4},
                                          {0.6,0.3,0.4},{0.6,0.2,0.4} };

GLfloat l_p_b[LETTER_P_FRONTAL][3] = { {0.6,0.0,-0.4},{0.5,0.0,-0.4},{0.6,0.4,0.4},{0.5,0.5,-0.4},
                                        {0.8,0.4,-0.4},{0.9,0.5,-0.4},{0.8,0.3,-0.4},{0.9,0.2,-0.4},
                                          {0.6,0.3,-0.4},{0.6,0.2,-0.4} };

ucsp::ucsp()
{
  mdEyeRadius = 0.1;
  mdShadowHalfSize = 2.5 * mdEyeRadius;
  mbInitialised = false;
}

void ucsp::DrawStuff(Vector<3> v3CameraPos)
{
  if(!mbInitialised)
    Init();

  mnFrameCounter ++;

  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  //glEnable(GL_LIGHTING);
  //glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  
  GLfloat af[4]; 
  af[0]=0.5; af[1]=0.5; af[2]=0.5; af[3]=1.0;
  glLightfv(GL_LIGHT0, GL_AMBIENT, af);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, af);
  af[0]=1.0; af[1]=0.0; af[2]=1.0; af[3]=0.0;
  glLightfv(GL_LIGHT0, GL_POSITION, af);
  af[0]=1.0; af[1]=1.0; af[2]=1.0; af[3]=1.0;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, af);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);
  
  glMatrixMode(GL_MODELVIEW);
  
  for(int i=0; i<1; i++) //4
    {/*
      if(mnFrameCounter < 100)
	LookAt(i, 500.0 * (Vector<3>) makeVector( (i<2?-1:1)*(mnFrameCounter < 50 ? -1 : 1) * -0.4 , -0.1, 1) , 0.05 );
      else
	LookAt(i, v3CameraPos, 0.02 );
      */
      glLoadIdentity();
      glMultMatrix(ase3WorldFromEye[i]);
      glScaled(mdEyeRadius, mdEyeRadius, mdEyeRadius);
      glCallList(mnEyeDisplayList);
    }
  
  glDisable(GL_LIGHTING);
  
  glLoadIdentity();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, mnShadowTex);
  glEnable(GL_BLEND);
  glColor4f(0,0,0,0.5);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex2d(-mdShadowHalfSize, -mdShadowHalfSize);
  glTexCoord2f(0,1);
  glVertex2d(-mdShadowHalfSize,  mdShadowHalfSize);
  glTexCoord2f(1,1);
  glVertex2d( mdShadowHalfSize,  mdShadowHalfSize);
  glTexCoord2f(1,0);
  glVertex2d( mdShadowHalfSize, -mdShadowHalfSize);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
};


void ucsp::Reset()
{
  for(int i=0; i<4; i++)
    ase3WorldFromEye[i] = SE3<>();

  ase3WorldFromEye[0].get_translation()[0] = -mdEyeRadius;
  ase3WorldFromEye[1].get_translation()[0] = mdEyeRadius;
  ase3WorldFromEye[2].get_translation()[0] = -mdEyeRadius;
  ase3WorldFromEye[3].get_translation()[0] = mdEyeRadius;

  ase3WorldFromEye[0].get_translation()[1] = -mdEyeRadius;
  ase3WorldFromEye[1].get_translation()[1] = -mdEyeRadius;
  ase3WorldFromEye[2].get_translation()[1] = mdEyeRadius;
  ase3WorldFromEye[3].get_translation()[1] = mdEyeRadius;

  ase3WorldFromEye[0].get_translation()[2] = mdEyeRadius;
  ase3WorldFromEye[1].get_translation()[2] = mdEyeRadius;
  ase3WorldFromEye[2].get_translation()[2] = mdEyeRadius;
  ase3WorldFromEye[3].get_translation()[2] = mdEyeRadius;
  mnFrameCounter = 0;
};


void ucsp::DrawEye()
{
  /*
  int nSegments = 45;
  int nSlices = 45;
  
  double dSliceAngle = M_PI / (double)(nSlices);
  double dSegAngle = 2.0 * M_PI / (double)(nSegments);
  
  glColor3f(0.0,0.0,0.0);
  {  // North pole:
    double Z = sin(M_PI/2.0 - dSliceAngle);
    double R = cos(M_PI/2.0 - dSliceAngle);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,1);
    glVertex3f(0,0,1);
    for(int i=0; i<nSegments;i++)
      {
	glNormal3f(R * sin((double)i * dSegAngle), R * cos((double)i * dSegAngle),  Z);
	glVertex3f(R * sin((double)i * dSegAngle), R * cos((double)i * dSegAngle),  Z);
      }
    glNormal3f(0,R,Z);
    glVertex3f(0,R,Z);
    glEnd();
  }
  
  int nBlueSlice = 3;
  int nWhiteSlice = 6;
  for(int j = 1; j<nSlices;j++)
    {
      if(j == nBlueSlice)
	glColor3f(0,0,1);
      if(j == nWhiteSlice)
	glColor4d(0.92, 0.9, 0.85,1);
      
      glBegin(GL_QUAD_STRIP); //GL_QUAD_STRIP //GL_POINTS
      double zTop = sin(M_PI/2.0 - dSliceAngle * (double)j);
      double zBot = sin(M_PI/2.0 - dSliceAngle * (double)(j+1));
      double rTop = cos(M_PI/2.0 - dSliceAngle * (double)j);
      double rBot = cos(M_PI/2.0 - dSliceAngle * (double)(j+1));
      for(int i=0; i<nSegments;i++) //nSegments
	{
	  glNormal3f(rTop*sin((double)i*dSegAngle), rTop*cos((double)i*dSegAngle), zTop);
	  glVertex3f(rTop*sin((double)i*dSegAngle), rTop*cos((double)i*dSegAngle), zTop);
	  glNormal3f(rBot*sin((double)i*dSegAngle), rBot*cos((double)i*dSegAngle), zBot);
	  glVertex3f(rBot*sin((double)i*dSegAngle), rBot*cos((double)i*dSegAngle), zBot);
	};
      glNormal3f(0,rTop, zTop);
      glVertex3f(0,rTop, zTop);
      glNormal3f(0,rBot, zBot);
      glVertex3f(0,rBot, zBot);
      glEnd();
    };

  {
    // South pole:
    double Z = sin(M_PI/2.0 - dSliceAngle);
    double R = cos(M_PI/2.0 - dSliceAngle);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,-1);
    glVertex3f(0,0,-1);
    for(int i=0; i<nSegments;i++)
      {
	glNormal3f(R * sin((double)i * -dSegAngle), R * cos((double)i * -dSegAngle),  -Z);
	glVertex3f(R * sin((double)i * -dSegAngle), R * cos((double)i * -dSegAngle),  -Z);
      }
    glNormal3f(0,R,-Z);
    glVertex3f(0,R,-Z);
    glEnd();
  };
  */
  {
    //axis
    //glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
      glColor3f(1.0,0.0,0.0);
      glVertex3f(0,0, 0);
      glVertex3f(10,0, 0);

      glColor3f(0.0,1.0,0.0);
      glVertex3f(0,0, 0);
      glVertex3f(0,10, 0);

      glColor3f(0.0,0.0,1.0);
      glVertex3f(0,0, 0);
      glVertex3f(0,0, 10);
    glEnd();
  }

  //UCSP
  if(flagUCSP){
    /*
    int NUMVTXZ=98;
    GLfloat vZ[NUMVTXZ][3]={{-3,1,0},{-2,1,0},
                        {-3,3,0},{-2,2,0},
                        {3,3,0},{1,2,0},
                        {3,2,0},{1,2,0},
                        {1,0,0},{1,2,0},
                        {1,0,0},{0,1,0}, //12
                        {1,0,0},{-1,1,0},
                        {1,0,0},{-0.5,0.5,0},
                        {1.5,-0.5,0},{-0.5,0.5,0},
                        {0.5,-0.5,0},{-0.5,0.5,0}, //20 Dx
                        {-1,-2,0},{-3,-2,0}, //22
                        {-1,-2,0},{-3,-3,0},
                        {2,-2,0},{3,-3,0},
                        {2,-1,0},{3,-1,0},
                        //espejo
                        {2,-1,-1},{3,-1,-1},
                        {2,-2,-1},{3,-3,-1},
                        {-1,-2,-1},{-3,-3,-1},
                        {-3,-2,-1},{-1,-2,-1}, //36
                        {-0.5,0.5,-1},{0.5,-0.5,-1},
                        {-0.5,0.5,-1},{1.5,-0.5,-1},
                        {-0.5,0.5,-1},{1,0,-1},
                        {-1,1,-1},{1,0,-1},
                        {0,1,-1},{1,0,-1}, //46
                        {1,2,-1},{1,0,-1},
                        {1,2,-1},{3,2,-1},
                        {1,2,-1},{3,3,-1},
                        {-2,2,-1},{-3,3,-1},
                        {-2,1,-1},{-3,1,-1}, //56
                        //
                        {-2,1,0},{-3,1,-1},
                        {-3,1,0},{-3,1,-1},
                        {-3,3,0},{-3,3,-1},
                        {3,3,0},{3,3,-1},
                        {3,2,0},{3,2,-1},
                        {1,0,0},{1,0,-1}, //68
                        {1.5,-0.5,0},{1.5,-0.5,-1},
                        {0.5,-0.5,0},{0.5,-0.5,-1},
                        {-1,-2,0},{-1,-2,-1},
                        {2,-2,0},{2,-2,-1},
                        {2,-1,0},{2,-1,-1}, //78
                        {3,-1,0},{3,-1,-1},
                        {3,-3,0},{3,-3,-1},
                        {-3,-3,0},{-3,-3,-1},
                        {-3,-2,0},{-3,-2,-1},
                        {-0.5,0.5,0},{-0.5,0.5,-1}, //88
                        {-1,1,0},{-1,1,-1},
                        {0,1,0},{0,1,-1},
                        {1,2,0},{1,2,-1},
                        {-2,2,0},{-2,2,-1},
                        {-2,1,0},{-2,1,-1}, //98
                      };
    GLfloat cZ[NUMVTXZ][3]={{0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0}, //12
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0}, //22
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        //espejo
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0}, //36
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0}, //46
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0}, //56
                        //
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0}, //68
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0}, //78
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0}, //88
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0},
                        {0,1,0},{0,1,0}, //98
                    };

    GLfloat nZ[NUMVTXZ][3]={{-1,-1,1},{1,-1,1}, //-1,-1,1},{1,-1,1
              {-1,1,1},{1,-1,1},
              {1,1,1},{-0.7071,-0,2929},
              {1.7071,0.7071,1},{-0.7071,-0,2929},
              {1.414,0,1},{-0.7071,-0,2929},
              {1.414,0,1},{-0.7071,1.7071,1},
              {1.414,0,1},{-0.7071,0.2929,1},
              {1.414,0,1},{-1.414,0,1},
              {0.7071,-0.2929,1},{-1.414,0,1},
              {0.7071,-1.7071,1},{-1.414,0,1}, //20
              {0.7071,0.2929,1},{-1.7071,0.7071,1},
              {0.7071,0.2929,1},{-1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,1,1},
              //espejo
              {-1,1,1},{1,-1,1}, //30
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1}, //40
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1}, //50
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1}, //60
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1}, //70
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1}, //80
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1}, //90
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1},
              {-1,1,1},{1,-1,1}, //98
            };

    glPushMatrix();
      glTranslatef(0.0f, 0.0f, 4.0f);
      glScalef(4.0,4.0,4.0);
      glBegin(GL_QUAD_STRIP);
      //glRotatef(90, 1.0f, 0.0f, 0.0f);
      for (int i=0; i<NUMVTXZ; i++) {
        glColor3fv(cZ[i]);
        glVertex3fv(vZ[i]);
        //glNormal3f(nZ[i]);
      }
      glEnd();
    glPopMatrix();
    */

    /* Draw my initials letters name */
    drawLetter(l_u, LETTER_U_SIDE);
    drawLetter(l_u_f, LETTER_U_FRONTAL);
    drawLetter(l_u_b, LETTER_U_FRONTAL);

    drawLetter(l_c, LETTER_C_SIDE);
    drawLetter(l_c_f, LETTER_C_FRONTAL);
    drawLetter(l_c_b, LETTER_C_FRONTAL);

    drawLetter(l_s, LETTER_S_SIDE);
    drawLetter(l_s_f, LETTER_S_FRONTAL);
    drawLetter(l_s_b, LETTER_S_FRONTAL);

    drawLetter(l_p, LETTER_P_SIDE);
    drawLetter(l_p_f, LETTER_P_FRONTAL);
    drawLetter(l_p_b, LETTER_P_FRONTAL);

  }
  //rabbit
  if(flagRabbit){
    /*
    glBegin(GL_TRIANGLES); //GL_TRIANGLES GL_POINTS
    for (int i=0; i<3*rabbit.numPolygons; i++) {
      glColor3fv(rabbit.model_colorsGLTriangles[i]);
      glVertex3fv(rabbit.model_pointsGLTriangles[i]);
      //glNormal3f(nZ[i]);
    }
    glEnd();
    */
    glPushMatrix();
      glTranslatef(0.0f, 0.0f, 0.6f);
      glRotatef(90, 1.0f, 0.0f, 0.0f);
      glScalef(4.0,4.0,4.0);
      //glColor3f(0, 1, 0);
      glBegin(GL_TRIANGLES);
      for (int i=0; i<3*rabbit.numPolygons; i++) {
          glColor3fv(rabbit.model_colorsGLTriangles[i]);
          glVertex3fv(rabbit.model_pointsGLTriangles[i]);
      }
      glEnd();
    glPopMatrix();
  }

  if(flagDragon){
    /*
    glBegin(GL_TRIANGLES); //GL_TRIANGLES GL_POINTS
    for (int i=0; i<3*rabbit.numPolygons; i++) {
      glColor3fv(rabbit.model_colorsGLTriangles[i]);
      glVertex3fv(rabbit.model_pointsGLTriangles[i]);
      //glNormal3f(nZ[i]);
    }
    glEnd();
    */
    glPushMatrix();
      
      //glRotatef(90, 1.0f, 0.0f, 0.0f);
      glScalef(0.08,0.08,0.08);
      glTranslatef(0.0f, 0.0f, 60.0f);
      //glColor3f(0, 1, 0);
      glBegin(GL_TRIANGLES);
      for (int i=0; i<3*rabbit.numPolygons; i++) {
          glColor3fv(rabbit.model_colorsGLTriangles[i]);
          glVertex3fv(rabbit.model_pointsGLTriangles[i]);
      }
      glEnd();
    glPopMatrix();
  }

  //glutKeyboardFunc(keyboard);


}

void ucsp::Init()
{
  if(mbInitialised) return;
  mbInitialised = true;
  // Set up the display list for the eyeball.
  mnEyeDisplayList = glGenLists(1);
  glNewList(mnEyeDisplayList,GL_COMPILE);
  DrawEye();
  glEndList();
  //MakeShadowTex();
};


void ucsp::LookAt(int nEye, Vector<3> v3, double dRotLimit)
{
  Vector<3> v3E = ase3WorldFromEye[nEye].inverse() * v3;
  if(v3E * v3E == 0.0)
    return;
  
  normalize(v3E);
  Matrix<3> m3Rot = Identity;
  m3Rot[2] = v3E;
  m3Rot[0] -= m3Rot[2]*(m3Rot[0]*m3Rot[2]); 
  normalize(m3Rot[0]);
  m3Rot[1] = m3Rot[2] ^ m3Rot[0];
  
  SO3<> so3Rotator = m3Rot;
  Vector<3> v3Log = so3Rotator.ln();
  v3Log[2] = 0.0;
  double dMagn = sqrt(v3Log * v3Log);
  if(dMagn > dRotLimit)
    {
      v3Log = v3Log * ( dRotLimit / dMagn);
    }
  ase3WorldFromEye[nEye].get_rotation() = ase3WorldFromEye[nEye].get_rotation() * SO3<>::exp(-v3Log);
};

void ucsp::MakeShadowTex()
{
  const int nTexSize = 256;
  Image<byte> imShadow(ImageRef(nTexSize, nTexSize));
  double dFrac = 1.0 - mdEyeRadius / mdShadowHalfSize;
  double dCenterPos = dFrac * nTexSize / 2 - 0.5;
  ImageRef irCenter; irCenter.x = irCenter.y = (int) dCenterPos;
  int nRadius = int ((nTexSize / 2 - irCenter.x) * 1.05);
  unsigned int nRadiusSquared = nRadius*nRadius;
  ImageRef ir;
  for(ir.y = 0; 2 * ir.y < nTexSize; ir.y++)
    for(ir.x = 0; 2 * ir.x < nTexSize; ir.x++)
      {
	byte val = 0;
	if((ir - irCenter).mag_squared() < nRadiusSquared)
	  val = 255;
	imShadow[ir] = val;
	imShadow[ImageRef(nTexSize - 1 - ir.x, ir.y)] = val;
	imShadow[ImageRef(nTexSize - 1 - ir.x, nTexSize - 1 - ir.y)] = val;
	imShadow[ImageRef(ir.x, nTexSize - 1 - ir.y)] = val;
      }
  
  convolveGaussian(imShadow, 4.0);
  glGenTextures(1, &mnShadowTex);
  glBindTexture(GL_TEXTURE_2D,mnShadowTex);
  glTexImage2D(GL_TEXTURE_2D, 0, 
	       GL_ALPHA, nTexSize, nTexSize, 0, 
	       GL_ALPHA, GL_UNSIGNED_BYTE, imShadow.data()); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
};

void readObj(){
  printf("xD\n");
  rabbit.numVertices=100;
  printf("%d\n",rabbit.numVertices);
  //----------------------------------------------------------------------------
  float **modelo;
  int numT;
  string linea;
  ifstream archivo_entrada;
  char OBJETO[] ="models/Dragon_1.obj";  //"models/Rabbit.obj"
  archivo_entrada.open(OBJETO);
  if (archivo_entrada.fail()) { cout<<"el archivo no se abrio correctamente (1era lectura)"<<endl; }

  int countV=0;
  int countF=0;
  
  do{
    string id;
    string vertex="v";
    string face="f";
    getline(archivo_entrada, linea);
    stringstream ss;
    ss<<linea;
    ss>>id;
    //cout<<"id: "<<id<<endl;
    if(id.compare(vertex) == 0){ countV++; }
    if(id.compare(face) == 0){ countF++; }
    //stringstream ss;
    //ss<<linea;
    //ss>>numT;
    //cout<<numT<<endl;
      
  } while(archivo_entrada.eof()==0);  //stringstream ss;
  cout<<"countV: "<<countV<<endl;
  cout<<"countF: "<<countF<<endl;
  archivo_entrada.close();

  ////////////////////reserva de memoria para los vertices//////////////////////
  rabbit.numVertices=countV;
  rabbit.model_points=(GLfloat**) malloc(rabbit.numVertices*sizeof(float*));
  for(int i=0;i<rabbit.numVertices;i++){
    rabbit.model_points[i]=(GLfloat*) malloc(3*sizeof(float));
  }
  ////////////////////reserva de memoria para los vertices de los poligonos//////////////////////
  rabbit.numPolygons=countF;
  rabbit.model_pointsGLTriangles=(GLfloat**) malloc(3*rabbit.numPolygons*sizeof(float*));
  for(int i=0;i<3*rabbit.numPolygons;i++){
    rabbit.model_pointsGLTriangles[i]=(GLfloat*) malloc(3*sizeof(float));
  }
  ////////////////////reserva de memoria para los colores de los vertices de los poligonos//////////////////////
  rabbit.model_colorsGLTriangles=(GLfloat**) malloc(3*rabbit.numPolygons*sizeof(float*));
  for(int i=0;i<3*rabbit.numPolygons;i++){
    rabbit.model_colorsGLTriangles[i]=(GLfloat*) malloc(3*sizeof(float));
  }

  //almacenamiento de vertices
  archivo_entrada.open(OBJETO);
  if (archivo_entrada.fail()) { cout<<"el archivo no se abrio correctamente (1era lectura)"<<endl; }
  
  int k=0;
  cout<<"lectura de Vertices"<<endl;
  do{
    string id;
    string vertex="v";
    getline(archivo_entrada, linea);
    stringstream ss;
    ss<<linea;
    ss>>id;
    //cout<<"id2: "<<id<<endl;
    if(id.compare(vertex) == 0){ 
      //CNode node;

      for(unsigned i=0;i<3;i++){
        string xyzString;
        ss>>xyzString;
        //cout<<xyzString<<endl;
        rabbit.model_points[k][i]=strtof(&xyzString[0],0); //scala
        //cout<<"model_points[k][i]: "<<model_points[k][i]<<endl;
        //cout<<"k: "<<k<<endl;
        
        //if(i==0){ node.id=k; node.x=rabbit.model_points[k][i];}
        //else if(i==1){node.id=k; node.y=rabbit.model_points[k][i];}
        //else if(i==2){node.id=k; node.z=rabbit.model_points[k][i];}
      }
      //GRabbit.Nodes.push_back(node);
      k++;
    }
  
  } while(archivo_entrada.eof()==0);  //stringstream ss;
  archivo_entrada.close();
  /*
  for(unsigned i=0;i<numVertices;i++){
    cout<<i<<": "<<model_points[i][0]<<model_points[i][1]<<model_points[i][2]<<endl;
  } cout<<endl;
  */

  //cout<<":?"<<endl;
  //almacenamiento de vertices de poligonos listo para ser dibujados con la primitiva GL_TRIANGLES
  archivo_entrada.open(OBJETO);
  if (archivo_entrada.fail()) { cout<<"el archivo no se abrio correctamente (1era lectura)"<<endl; }
  int kk=0;
  //int numFacesD=0;

  cout<<"lectura de faces"<<endl;
  do{
    string id;
    string face="f";
    getline(archivo_entrada, linea);
    stringstream ss;
    ss<<linea;
    ss>>id;
    //cout<<"id2: "<<id<<endl;

    if(id.compare(face) == 0){
      //CEdge edge1;
      //CEdge edge2;
      //CEdge edge3;

      for(unsigned i=0;i<3;i++){
        string xyzString;
        ss>>xyzString;
        //cout<<xyzString<<endl;
        //vector<char> v;
        string v;
        int indice;
        if(flagDragon || flagWoola){ 
          indice=stoi(xyzString);
          if(kk%30000==0){cout<<"numF:"<<kk/3<<endl;}
        }
        else{
          int ii=0;
          while(xyzString[ii] != '/'){
            //v.push_back(xyzString[ii]);
            v=v + xyzString[ii];
            //cout<<v<<endl;
            ii++;
          }
          //cout<<v<<endl;
          indice=stoi(v);
        }
        
        //cout<<indice<<endl;
        rabbit.model_pointsGLTriangles[kk]=rabbit.model_points[indice-1]; //scala
        //model_points[k][i]=strtof(&xyzString[0],0);
        //cout<<"model_pointsGLTriangles[kk]: "<<model_pointsGLTriangles[kk][0]<<" "<<model_pointsGLTriangles[kk][1]<<" "<<model_pointsGLTriangles[kk][2]<<endl;
        //cout<<"kk: "<<kk<<endl;

        kk++;
      }
      //cout<<endl;
      //if(flagDragon){cout<<"numFacesD: "<<numFacesD++<<endl;}
    } 
  } while(archivo_entrada.eof()==0);  //stringstream ss;
  archivo_entrada.close();

  cout<<"asignacion de color rabbit.model_colorsGLTriangles :"<<endl;
  for(unsigned i=0;i<3*rabbit.numPolygons;i++){ 
    rabbit.model_colorsGLTriangles[i][0]=1.0;
    rabbit.model_colorsGLTriangles[i][1]=1.0;
    rabbit.model_colorsGLTriangles[i][2]=1.0;
  }

  for(int i=0;i<rabbit.numVertices;i++){
    //cout<<i<<": "<<rabbit.model_points[i][0]<<" "<<rabbit.model_points[i][1]<<" "<<rabbit.model_points[i][2]<<endl;
  }
  //for(int i=0;i<3*rabbit.numPolygons;i++){
    
  //}
}


void drawLetter(GLfloat arr[][3], int n) {
  glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.6f);
    //glRotatef(90, 1.0f, 0.0f, 0.0f);
    glScalef(4.0,4.0,4.0);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < n; i++) {
        glColor3f(1.0,1.0,1.0); //c[i]
        glVertex3fv(arr[i]);
    }
    glEnd();
  glPopMatrix();
}


/*
void keyboard(unsigned char key, int x, int y){
  switch(key) {
    case 033: // Escape Key
    case 'r': case 'R':
      flagRabbit=true;
      flagUCSP=false;
      break;
    case 'u': case 'U':
      flagRabbit=false;
      flagUCSP=true;
      break;
  } 
  glutPostRedisplay();
}
*/