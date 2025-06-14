//NaveInimiga.h
#pragma once
#include "BibSistema.h"



class NaveInimiga{
    
    private:
        GLfloat posicao [2]; // Posição da nave no mundo
        GLfloat velocidadeNaveInimiga; 
        
        GLvoid desenhaBaseNaveInimiga(GLvoid); 
        GLvoid desenhaCanhoesNaveInimiga(GLvoid); 
        
    public:
        
        NaveInimiga(GLfloat);


        GLvoid desenhaNaveInimiga(GLvoid); 
        GLvoid moverNaveInimiga(GLint); 


        
        /*----getters----*/
        GLfloat* getPosicaoNaveInimiga();//ponteiro para o valor da posicao
        GLfloat getVelocidadeNaveInimiga();
        
        /*----setters----*/
        GLvoid setPosicaoNaveInimiga(GLfloat x, GLfloat y);
        GLvoid setVelocidadeNaveInimiga(GLfloat v);
    
        
        
        /*destrutor*/ 
        ~NaveInimiga();


};