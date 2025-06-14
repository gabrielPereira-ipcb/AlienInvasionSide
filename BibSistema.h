// BibSistema.h
#pragma once

#include <iostream>   
#include <vector>     
#include <stdlib.h>   
#include <cmath>      
#include <sstream>    

// Detecção de sistema operacional para incluir a biblioteca gráfica correta
#if defined(_WIN32) || defined(_WIN64)
    #include <GL\freeglut.h>
#elif defined(__APPLE_) || defined(__MACH__) || defined (TAREGT_OS_MAC)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#elif defined(__linux__)
    #include <GL/glut.h>
#endif



//variáveis globais ou outras definições partilhadas 

extern GLfloat tamanhoMundo;
extern GLfloat coordenadasMundo[4];
extern GLfloat naveJogadorTamanho[2];
extern GLfloat naveInimigaTamanho[2];
extern GLfloat tamanhoProjetil;
extern GLfloat escala;


