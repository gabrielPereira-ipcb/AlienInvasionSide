//Projetil.cpp
#include "Projetil.h"

Projetil::Projetil(GLfloat x, GLfloat y, GLint d, GLboolean dono) {
    posicao[0] = x;
    posicao[1] = y;
    velocidade = 1.0f; // Velocidade do projétil
    direcao = d; // d = 0 para cima, 1 para direita, 2 para baixo, 3 para esquerda
    donoProjetil = dono; // Define se o projétil é do jogador ou inimigo
}

GLvoid Projetil::desenhaProjetil() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    
    glTranslatef(posicao[0], posicao[1], 0.0f);

    glScalef(escala, escala, 1.0f);

    glRotatef(direcao * 90.0f, 0.0f, 0.0f, 1.0f);
    
    corpoProjetil();
    glPopMatrix();
}

GLvoid Projetil::corpoProjetil() {
    if (donoProjetil) {
        glColor3f(0.0f, 0.0f, 1.0f); // azul para projéteis do jogador
    } else {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho para projéteis inimigos
    }
    

    glBegin(GL_QUADS);
        glVertex2f(-tamanhoProjetil, -tamanhoProjetil);
        glVertex2f(tamanhoProjetil, -tamanhoProjetil);
        glVertex2f(tamanhoProjetil, tamanhoProjetil);
        glVertex2f(-tamanhoProjetil, tamanhoProjetil);
    glEnd();
}

GLboolean Projetil::moveProjetil(GLvoid) {
    // Move o projétil baseado na direção
    switch (direcao) {
        case 0: // Direita
            posicao[0] += velocidade;
            break;
        case 1: // Cima
            posicao[1] += velocidade;
            break;
        case 2: // Esquerda
            posicao[0] -= velocidade;
            break;
        case 3: 
                // Baixo
            posicao[1] -= velocidade;
            break;
    }
    // Verifica se o projétil saiu da tela
    if (posicao[0] < coordenadasMundo[0] || posicao[0] > coordenadasMundo[1] ||
        posicao[1] < coordenadasMundo[2] || posicao[1] > coordenadasMundo[3]) {
        return true; // Projétil saiu da tela
    }
    return false; // Projétil ainda está na tela
}

GLfloat* Projetil::getPosicaoProjetil() {
    return posicao;
}

GLboolean Projetil::estaNaTela() {
    return posicao[1] <= coordenadasMundo[3];
}

GLboolean Projetil::verificaColisao(GLfloat* posicaoInimigo, GLfloat* tamanhoInimigo) {
    if (!posicaoInimigo || !tamanhoInimigo) 
        return false;
    
    // Verifica se o projétil está dentro da área do objeto
    if (posicao[0] >= posicaoInimigo[0] - *tamanhoInimigo &&
        posicao[0] <= posicaoInimigo[0] + *tamanhoInimigo &&
        posicao[1] >= posicaoInimigo[1] - *tamanhoInimigo &&
        posicao[1] <= posicaoInimigo[1] + *tamanhoInimigo) {
        return GL_TRUE;
    }
    
    return GL_FALSE;
}

Projetil::~Projetil() {
    
}