// BibSistema.h
#pragma once

#include <iostream>   // Para entrada e saída padrão (como console)
#include <vector>     // Para usar vetores dinâmicos, por exemplo, para listas de inimigos ou balas 
#include <stdlib.h>   // Necessário para funções como exit()
#include <cmath>      // Para funções matemáticas comuns em computação gráfica (e.g., cálculos de distância, ângulos)
#include <sstream>    // Pode ser útil para formatação de texto, como exibir a pontuação ou vidas (embora não explicitamente mencionado para este arquivo, é uma biblioteca padrão útil)

// Detecção de sistema operacional para incluir a biblioteca gráfica correta


#ifdef __APPLE__
    #include <GLUT/glut.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include <GL/gl.h>
    #include <GL/glu.h>
#elif defined(__linux__)
    #include <GL/glut.h>
#endif

// cabeçalhos
// Declarar variáveis globais ou outras definições partilhadas aqui, se necessário.
extern GLfloat tamanhoDoMundo;
extern GLfloat coordenadasMundo[4];
extern GLfloat naveJogadorTamanho;



