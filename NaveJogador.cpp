//naveJogador.cpp
#include "NaveJogador.h"


/*----Nave padrao do jogador----*/

/*----construtor default----*/
NaveJogador::NaveJogador(GLfloat v){
    posicao[0] = 0.0f;
    posicao[1] = coordenadasMundo[2]+2.0f;//defina a posicao y inicial um pouco acima do limite inferior do mundo
    anguloRotacao = 0.0f; //90 é orientado para cima. Segue a convecao trigonometrica Leste=0, Norte =90, Oeste = 180 e Sul =270. Entretanto, como a nave já é criado com a ponta para o norte, o seu angulo inicial é 0.
    velocidade = v;
}



/*----metodo para desenhar a base da nave do jogador----*/
GLvoid NaveJogador::desenhaBaseNaveJogador(GLvoid){
    glColor3f(0.0f, 1.0f, 0.0f); //cor verde para a cor da nave amiga
    
    // Desenha o hexágono centralizado na origem
    glBegin(GL_POLYGON);
    {
        // Base inferior
        glVertex2f(-naveJogadorTamanho[0]/2, -naveJogadorTamanho[1]/2); // Esquerda inferior
        glVertex2f(naveJogadorTamanho[0]/2, -naveJogadorTamanho[1]/2);  // Direita inferior
        
        // Lado direito
        glVertex2f(naveJogadorTamanho[0]/2, 0.0f);  // Direita meio
        
        // Topo
        glVertex2f(naveJogadorTamanho[0]/4, naveJogadorTamanho[1]/4);   // Topo direito
        glVertex2f(-naveJogadorTamanho[0]/4, naveJogadorTamanho[1]/4);  // Topo esquerdo
        
        // Lado esquerdo
        glVertex2f(-naveJogadorTamanho[0]/2, 0.0f); // Esquerda meio inferior
    }
    glEnd();
}

GLvoid NaveJogador::desenhaCockpitNaveJogador(GLvoid){
    glColor3f(0.2f, 0.5f, 1.0f); // Azul claro para a cabine
    // Desenha o triângulo invertido (cabine) 
    glBegin(GL_TRIANGLES);
    {
        glVertex2f(-naveJogadorTamanho[0]/4, naveJogadorTamanho[1]/4);  
        glVertex2f(naveJogadorTamanho[0]/4, naveJogadorTamanho[1]/4);   
        glVertex2f(0.0f, -naveJogadorTamanho[1]/2);                     
    }
    glEnd();
}


/*----metodo para desenhar o cockpit (triangulo) da nave----*/
GLvoid NaveJogador::desenhaCanhaoNaveJogador(GLvoid){
    glColor3f(0.0f, 1.0f, 0.0f); 
    // Desenha o triângulo (ponta da nave) 
    glBegin(GL_TRIANGLES);
    {
        glVertex2f(-naveJogadorTamanho[0]/2, -naveJogadorTamanho[1]/2);  // ponto esquerda
        glVertex2f(naveJogadorTamanho[0]/2, -naveJogadorTamanho[1]/2);   // ponto direita
        glVertex2f(0.0f, (naveJogadorTamanho[1]/2)*1.5f);  // Ponta
    }
    glEnd();
}

/*----metodo para desenhar os foguetes da nave----*/
GLvoid NaveJogador::desenhaFoguetesNaveJogador(GLvoid){
    glColor3f(1.0f, 0.5f, 0.0f); // Cor laranja para os foguetes
    
    // Foguete esquerdo
    glBegin(GL_TRIANGLES);
    {
        glVertex2f(-naveJogadorTamanho[0]/3, -naveJogadorTamanho[1]/2); // Base esquerda
        glVertex2f(-naveJogadorTamanho[0]/6, -naveJogadorTamanho[1]/2); // Base direita
        glVertex2f(-naveJogadorTamanho[0]/4, -naveJogadorTamanho[1]);   // Ponta
    }
    glEnd();
    

    // Foguete direito
    glBegin(GL_TRIANGLES);
    {
        glVertex2f(naveJogadorTamanho[0]/6, -naveJogadorTamanho[1]/2);  // Base esquerda
        glVertex2f(naveJogadorTamanho[0]/3, -naveJogadorTamanho[1]/2);  // Base direita
        glVertex2f(naveJogadorTamanho[0]/4, -naveJogadorTamanho[1]);    // Ponta
    }
    glEnd();
}

/*----metodo para desenhar a nave do jogador----*/
GLvoid NaveJogador::desenha(GLvoid){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    // Primeiro translada para a posição
    glTranslatef(posicao[0], posicao[1], 0.0f);
    // Aplica a escala
    glScalef(escala, escala, 1.0f);
    // Depois rotaciona em torno do ponto de origem da nave
    glRotatef(anguloRotacao, 0.0f, 0.0f, 1.0f);
    
    desenhaCanhaoNaveJogador();
    desenhaBaseNaveJogador();
    desenhaFoguetesNaveJogador();
    desenhaCockpitNaveJogador();
    glPopMatrix();
}

/*----metodo para mover a nave do jogador----*/
GLboolean NaveJogador::moverNave(GLint direcao){
    GLboolean move = false;


    // Calcula os limites considerando a escala uma única vez
    GLfloat limiteDireita = coordenadasMundo[1] - (naveJogadorTamanho[0] * escala);
    GLfloat limiteEsquerda = coordenadasMundo[0] + (naveJogadorTamanho[0] * escala);
    GLfloat limiteCima = coordenadasMundo[3] - (naveJogadorTamanho[1] * escala);
    GLfloat limiteBaixo = coordenadasMundo[2] + (naveJogadorTamanho[1] * escala);

    switch (direcao) {
        case 0: //direita
            if (posicao[0] < limiteDireita) {
                posicao[0] += velocidade;
                move = true;
            }
            break;
        case 1: //esquerda
            if (posicao[0] > limiteEsquerda) {
                posicao[0] -= velocidade;
                move = true;
            }
            break;
        case 2: //cima
            if (posicao[1] < limiteCima) {
                posicao[1] += velocidade;
                move = true;
            }
            break;
        case 3: //baixo
            if (posicao[1] > limiteBaixo) {
                posicao[1] -= velocidade;
                move = true;
            }
            break;
        default:
            std::cerr << "Direção inválida!" << std::endl;
            break;
    }
    return move;
}

/*----Metodo para rodar a nave----*/
GLboolean NaveJogador::rodarNave(GLint direcao){
    GLboolean rotaciona = false;

    /*Nao ha limite de rotacao. */
    switch (direcao){
        case 1: //"esquerda" somar 90graus
            if (anguloRotacao >=270.0f){
                anguloRotacao = 0.0f;
                rotaciona = true;
                break;
            }
            else{
                anguloRotacao += 90.0f;
                rotaciona = true;
                break;
            }    
            
            
        case 0: //"direita" subtrarir 90graus

            if (anguloRotacao <= 0.0f){
                anguloRotacao = 270.0f;
                rotaciona = true;
                break;
            }
            else {
                anguloRotacao -= 90.0f;
                rotaciona = true;
                break;
            }
            
        default:
            std::cerr<<"Rotação inválida"<<std::endl;
            break;
    }
    return rotaciona;
}

GLfloat* NaveJogador::getPosicao(){
    return posicao;
}

GLfloat NaveJogador::getAnguloRotacao(){

    
    return (anguloRotacao + 90.0f) == 360.0f ? 0.0f : (anguloRotacao + 90.0f); 
}

NaveJogador::~NaveJogador() { //destrutor da nave do Jogador
    // Destroi a nave do jogador
    std::cout << "Destruindo a nave do jogador!" << std::endl;
}