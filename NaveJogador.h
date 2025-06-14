//NaveJogador.h
#pragma once
#include "BibSistema.h"



class NaveJogador{
    //variaveis privadas
    private:
        GLfloat posicao [2]; // Posição da nave no mundo
        GLfloat velocidade; 
        GLfloat anguloRotacao; //Angulo de rotacao da nave. pode ser +ou- 90 graus, a depender da orientacao


        GLvoid desenhaBaseNaveJogador(GLvoid); 
        GLvoid desenhaCanhaoNaveJogador(GLvoid); 
        GLvoid desenhaFoguetesNaveJogador(GLvoid);
        GLvoid desenhaCockpitNaveJogador(GLvoid);

    //variaveis publicas
    public:
        //construtor default
        NaveJogador(GLfloat v);

        
        GLvoid desenha(GLvoid); //metodo publico para desenhar a nave do jogador
        GLboolean moverNave(GLint); // metodo publico para mover a nave do jogador
        GLboolean rodarNave (GLint); //metodo para rodar a nave

        
        /*getters*/
        GLfloat* getPosicao();//ponteiro para o valor da posicao
        GLfloat getAnguloRotacao(); // Retorna o ângulo de rotação da nave

        /*setters*/
        
        
        /*destrutor*/ 
        ~NaveJogador();


};