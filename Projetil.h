//Projetil.h
#pragma once
#include "BibSistema.h"
#include "NaveInimiga.h"

class Projetil {
    private:
        GLfloat posicao[2];
        GLfloat velocidade;
        GLint direcao; // 0: direita, 1: cima, 2: esquerda, 3: baixo
        GLboolean donoProjetil; // Indica se o projétil é do jogador (true) ou inimigo (false)
        GLfloat cor[3];

    public:
        Projetil(GLfloat x, GLfloat y, GLint d, GLboolean dono);
        GLvoid corpoProjetil();
        GLvoid desenhaProjetil(GLvoid);
        GLboolean moveProjetil(GLvoid);
        GLboolean verificaColisao(GLfloat* posicaoInimigo, GLfloat* tamanhoInimigo);
        GLfloat* getPosicaoProjetil(GLvoid);
        GLboolean estaNaTela();
        GLboolean verificaColisao(NaveInimiga* naveInimiga);
        ~Projetil();
        
};