//NaveInimiga.cpp
#include "NaveInimiga.h"
NaveInimiga::NaveInimiga(GLfloat v){
    posicao[0] = 0.0f;
    //posicao[1] = 0.0f;
    posicao[1] = coordenadasMundo[3]-2.0f; // Posiciona a nave inimiga no topo da tela

    velocidadeNaveInimiga = v;
}

GLvoid NaveInimiga::desenhaBaseNaveInimiga(GLvoid){
    glColor3f(1.0f, 1.0f, 1.0f); // cor branca para a nave inimiga

    // Desenha o corpo principal da nave (hexágono)
    glBegin(GL_QUADS);
    {
        glVertex2f(-naveInimigaTamanho[0]/2, -naveInimigaTamanho[1]/2); // Base esquerda
        glVertex2f(naveInimigaTamanho[0]/2, -naveInimigaTamanho[1]/2);  // Base direita
        glVertex2f(naveInimigaTamanho[0]/2, naveInimigaTamanho[1]/2);   // Topo direita
        glVertex2f(-naveInimigaTamanho[0]/2, naveInimigaTamanho[1]/2);  // Topo esquerda
    }
    glEnd();
}

GLvoid NaveInimiga::desenhaCanhoesNaveInimiga(GLvoid){
    glColor3f(1.0f, 0.0f, 0.0f); // cor branca para os canhões

    // Canhão esquerdo
    glBegin(GL_TRIANGLES);
    {
        glVertex2f(-naveInimigaTamanho[0]/2, -naveInimigaTamanho[1]/2);  // Base esquerda
        glVertex2f(0.0f, -naveInimigaTamanho[1]/2);   // Base direita
        glVertex2f(-naveInimigaTamanho[0]/4, naveInimigaTamanho[1]*0.8f);// Ponta
    }
    glEnd();

    //canhao direita
    glBegin(GL_TRIANGLES);
    {
        glVertex2f(0.0f, -naveInimigaTamanho[1]/2);   // Base esquerda
        glVertex2f(naveInimigaTamanho[0]/2, -naveInimigaTamanho[1]/2);  // Base direita
        glVertex2f(naveInimigaTamanho[0]/4, naveInimigaTamanho[1]*0.8f); // Ponta
    }
    glEnd();

    
}

/*----metodo para desenhar a nave inimiga----*/
GLvoid NaveInimiga::desenhaNaveInimiga(GLvoid){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(posicao[0], posicao[1], 0.0f); //transladar para a posicao
    glScalef(escala, escala, 1.0f); // Aplica a escala
    glRotatef(180, 0.0f, 0.0f, 1.0f); // Mantém a rotação para a nave apontar para baixo

    desenhaBaseNaveInimiga(); 
    desenhaCanhoesNaveInimiga();
    glPopMatrix();


}

GLvoid NaveInimiga::moverNaveInimiga(GLint direcao){
    switch (direcao) {
    case 0:
        posicao[0] = posicao[0] + velocidadeNaveInimiga;
        break;

    case 1:
        posicao[1] = posicao[1] - velocidadeNaveInimiga;
        break;

    case 2:
        posicao[0] = posicao[0] - velocidadeNaveInimiga;
        break;

    case 3:
        posicao[1] = posicao[1] + velocidadeNaveInimiga;
        break;
    }
}


GLfloat* NaveInimiga::getPosicaoNaveInimiga(){
    return posicao;
}
GLfloat NaveInimiga::getVelocidadeNaveInimiga(){
    return velocidadeNaveInimiga;
}

GLvoid NaveInimiga::setPosicaoNaveInimiga(GLfloat x, GLfloat y){
    posicao[0] = x;
    posicao[1] = y;
}

GLvoid NaveInimiga::setVelocidadeNaveInimiga(GLfloat v){
    velocidadeNaveInimiga = v;
}

NaveInimiga::~NaveInimiga(){

}