//main.cpp
#include "BibSistema.h"
#include "NaveJogador.h"
#include "NaveInimiga.h"
#include "Projetil.h"

using namespace std;// usar o namespace std para evitar escrever std:: antes de cada função da biblioteca padrão




GLvoid criarNaveJogador(GLvoid); 
GLvoid criarNavesInimigo(GLvoid); 

/*----Funcoes para seleção de dificuldade----*/
GLvoid desenhaUISelecaoDificuldade(GLvoid);
GLvoid desenhaEcraSelecaoDificuldade(GLvoid);
GLvoid idleEcraSelecaoDificuldade(GLvoid);
GLvoid tecladoSelecaoDificuldade(unsigned char tecla, int x, int y);
GLvoid iniciarJogoComDificuldade(GLint dificuldade);

/*----Funcoes referentes ao estado de jogo----*/
GLvoid desenhaEcraJogo(GLvoid); 
GLvoid desenhaUIJogo(GLvoid); 
GLvoid idleJogo(GLvoid); 
GLvoid tecladoJogo(unsigned char tecla, int x, int y);


/*----funcoes para o estado inicial----*/
GLvoid desenhaUIEcraInicial(GLvoid); 
GLvoid desenhaEcraInicial(GLvoid); 
GLvoid idleEcraInicial(GLvoid); 
GLvoid tecladoEcraInicial (unsigned char tecla, int x, int y); 

/*----funcoes para o estado de ajuda-----*/
GLvoid desenhaUIEcraSuporte(GLvoid);
GLvoid desenhaEcraSuporte(GLvoid);
GLvoid idleEcraSuporte(GLvoid);
GLvoid tecladoSuporte(unsigned char tecla, int x, int y);



/*----funcoes para o estado de pausa -----*/
GLvoid desenhaUIEcraPausa(GLvoid);
GLvoid desenhaEcraPausa(GLvoid);
GLvoid idleEcraPausa(GLvoid);
GLvoid tecladoPausa(unsigned char tecla, int x, int y);


/*----funcoes para o estado de gameOver----*/
GLvoid desenhaUIGameOver(GLvoid);
GLvoid desenhaEcraGameOver(GLvoid);
GLvoid idleEcraGameOver(GLvoid);
GLvoid tecladoGameOver(unsigned char tecla,int x,int y);

/*----funcoes para o estado de GameWIn----*/
GLvoid desenhaUIGameWIn(GLvoid);
GLvoid desenhaEcraGameWIn(GLvoid);
GLvoid idleEcraGameWIn(GLvoid);
GLvoid tecladoGameWIn(unsigned char tecla, int x, int y);



/*----variaveis globais externas----*/

GLfloat tamanhoMundo = 20.0f;
GLfloat coordenadasMundo[4] =  {-tamanhoMundo, tamanhoMundo, -tamanhoMundo, tamanhoMundo}; //As coordenadas do mundo são: xmin, xmax, ymin, ymax. Ou seja, duma ponta a outra do mundo, no eixo x e no eixo y, tem 2 vezes o tamanhoMundo
GLfloat naveJogadorTamanho[2] = {6.0f,5.0f};
GLfloat velocidadeNaveAmiga = 1.0f;

GLfloat naveInimigaTamanho[2] = {3.0f,2.0f}; 
GLfloat velocidadeNaveInimiga = 1.0f;

GLfloat escala = 0.25f;

GLfloat razaoInimigoVertical = 0.2f; // grade de inimigos cobre 20% do 
GLfloat razaoInimigosHorizontal = 0.8f; //grade inimigos cobre 80%....

GLfloat tamanhoProjetil = (naveJogadorTamanho[0]*escala)/2;// Aumentado para metade do tamanho da nave inimiga

GLint larguraEcra = 720, alturaEcra = 720;

// Enumeração para os estados do jogo
enum EstadoJogo {
    INICIAL,
    PAUSADO,
    JOGANDO,
    AJUDAPAUSA,
    AJUDAINICIAL,
    SELECAO_DIFICULDADE,
    GAMEWIN,
    GAMEOVER
};

EstadoJogo estadoAtual = INICIAL;

GLint dificuldadeSelecionada;

NaveJogador* naveJogador; // ponteiro para a nave do jogador


vector <NaveInimiga*> navesInimigas; 
vector <Projetil*> projeteisJogador; 

// Variáveis para controle do timer de disparo
GLfloat ultimoDisparo = 0.0f;
GLfloat intervaloDisparo = 1.5f;

// Variáveis para controle do jogo
GLint vidasJogador = 3;
GLint inimigosEliminados = 0;
GLfloat tempoJogo = 0.0f;
GLfloat ultimoTempo = 0.0f;
GLfloat ultimoTempoAtualizacao = 0.0f;
GLint pontuacaoTotal = 0;

// Variáveis para controle de tiro dos inimigos
vector<Projetil*> projeteisInimigos;
GLfloat ultimoTiroInimigo = 0.0f;
GLfloat intervaloDisparoInimigo = 2.0f; 

// Variáveis para controle do movimento das naves inimigas
GLfloat velocidadeHorizontalInimigos = 0.5f*escala;

GLfloat distanciaHorizontalInimigos = naveInimigaTamanho[0]*escala*3.5f; 
GLfloat distanciaVerticalInimigos = naveInimigaTamanho[1]*escala*3;

GLboolean movendoDireita = true;
GLint ciclosMovimento = 0;
GLint ciclosParaDescer = 4; 

GLboolean jogadorAtrasLinhaInimiga = false;

/*----Funcoes do ecra inicial----*/
GLvoid desenhaUIEcraInicial(){
    stringstream buffer;
    char caracter;

    // Título
    glColor3f(0.0f, 1.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "ALIEN INVASION";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f + 2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, caracter);
    }

    // Instruções
    glColor3f(0.0f, 1.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Press ENTER to play";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 2.0f, 
                  coordenadasMundo[3]/2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Press H for help";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 2.0f, 
                  coordenadasMundo[3]/2.0f - 1.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    glColor3f(1.0f, 0.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Press ESC to exit";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 2.0f, 
                  coordenadasMundo[3]/2.0f - 2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }
}

GLvoid desenhaEcraInicial(GLvoid){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Define a cor de fundo como preta
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();  
     
    gluOrtho2D(coordenadasMundo[0], coordenadasMundo[1], coordenadasMundo[2], coordenadasMundo[3]);

    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();

    desenhaUIEcraInicial(); 

    glutSwapBuffers();
}

GLvoid idleEcraInicial(GLvoid){
    
}

GLvoid tecladoEcraInicial(unsigned char tecla, int x,int y){
    switch (tecla) {
    

    case 13: // "ENTER"
        
        glutDisplayFunc(desenhaEcraSelecaoDificuldade);
        glutKeyboardFunc(tecladoSelecaoDificuldade);
        glutIdleFunc(idleEcraSelecaoDificuldade);
        estadoAtual = SELECAO_DIFICULDADE;
        glutPostRedisplay();
        break;

    case 'h': // "Help :)"
    case 'H':
        glutDisplayFunc(desenhaEcraSuporte);
        glutIdleFunc(idleEcraSuporte);
        glutKeyboardFunc(tecladoSuporte);
        glutPostRedisplay(); // Redesenha a cena
        estadoAtual = AJUDAINICIAL; 
        std::cout << "Indo para a tela de ajuda..." << std::endl;
        std::cout << estadoAtual << std::endl;

        break;

    case 27:
     // "ESC"
    exit(0); // Sair do jogo
    break;
    }
}

GLvoid desenhaUISelecaoDificuldade(GLvoid) {
    stringstream buffer;
    char caracter;

    // Desenha o título
    glColor3f(0.0f, 1.0f, 0.0f); // Verde
    buffer.str("");
    buffer.clear();
    buffer << "SELECT DIFFICULTY";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f + 2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, caracter);
    }

    // Desenha as opções de dificuldade
    glColor3f(1.0f, 1.0f, 1.0f); // Branco
    buffer.str("");
    buffer.clear();
    buffer << "1 - Deux Ex Machina";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 2.0f, 
                  coordenadasMundo[3]/2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    buffer.str("");
    buffer.clear();
    buffer << "2 - Medium";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 2.0f, 
                  coordenadasMundo[3]/2.0f - 1.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    buffer.str("");
    buffer.clear();
    buffer << "3 - Hard";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 2.0f, 
                  coordenadasMundo[3]/2.0f - 2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    // Instruções para voltar
    glColor3f(0.0f, 1.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Press ESC to go back";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f - 4.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }
}

GLvoid desenhaEcraSelecaoDificuldade(GLvoid) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();  
     
    gluOrtho2D(coordenadasMundo[0], coordenadasMundo[1], coordenadasMundo[2], coordenadasMundo[3]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    desenhaUISelecaoDificuldade();

    glutSwapBuffers();
}

GLvoid idleEcraSelecaoDificuldade(GLvoid) {

    glutPostRedisplay();
}

GLvoid tecladoSelecaoDificuldade(unsigned char tecla, int x, int y) {
    switch(tecla) {
        case '1':
            iniciarJogoComDificuldade(1);
            break;
        case '2':
            iniciarJogoComDificuldade(2);
            break;
        case '3':
            iniciarJogoComDificuldade(3);
            break;
        case 27: // ESC
            glutDisplayFunc(desenhaEcraInicial);
            glutKeyboardFunc(tecladoEcraInicial);
            glutIdleFunc(idleEcraInicial);
            estadoAtual = INICIAL;
            glutPostRedisplay();
            break;
    }
}

GLvoid iniciarJogoComDificuldade(GLint dificuldade) {
    // Limpa o estado anterior
    for (int i = 0; i < navesInimigas.size(); i++) {
        delete navesInimigas[i]; // Libera a memoria alocada 
    }
    navesInimigas.clear(); // Limpa o vetor de naves inimigas
    for (int i = 0; i < projeteisJogador.size(); i++) {
        delete projeteisJogador[i]; 
    }
    projeteisJogador.clear();
    for (int i = 0; i < projeteisInimigos.size(); i++) {
        delete projeteisInimigos[i];
    }
    projeteisInimigos.clear();

    // Configura a dificuldade
    switch (dificuldade) {
        case 1:
            dificuldadeSelecionada = 1;
            vidasJogador = 5;
            intervaloDisparo = 0.15f; // Tiro mais rápido
            intervaloDisparoInimigo = 3.0f; // Inimigos demoram mais para atirar
            razaoInimigoVertical = 0.2f;
            razaoInimigosHorizontal = 0.8f;
            velocidadeNaveAmiga = 2.0f;
            break;
        case 2:
            dificuldadeSelecionada = 2;
            vidasJogador = 3;
            intervaloDisparo = 0.5f;
            intervaloDisparoInimigo = 2.0f;
            razaoInimigoVertical = 0.2f;
            razaoInimigosHorizontal = 0.8f;
            velocidadeNaveAmiga = 1.0f;
            break;
        case 3:
            dificuldadeSelecionada = 3;
            vidasJogador = 2;
            intervaloDisparo = 1.5f;
            intervaloDisparoInimigo = 1.0f; // Inimigos atiram mais rápido
            razaoInimigoVertical = 0.3f; // Mais inimigos
            razaoInimigosHorizontal = 0.4f;
            velocidadeNaveAmiga = 1.0f;
            break;
    }

    // Reseta outras variáveis
    inimigosEliminados = 0;
    tempoJogo = 0.0f;
    pontuacaoTotal = 0;
    ultimoTiroInimigo = 0.0f;


    criarNaveJogador();
    criarNavesInimigo();

    // Muda para o estado de jogo
    estadoAtual = JOGANDO;
    glutDisplayFunc(desenhaEcraJogo);
    glutKeyboardFunc(tecladoJogo);
    glutIdleFunc(idleJogo);
    glutPostRedisplay();
}


/*----funcoes de suporte do jogo----*/
GLvoid desenhaUIEcraSuporte(GLvoid){
    stringstream buffer;
    char caracter;

    // Título
    glColor3f(0.0f, 1.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "GAME HELP";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 2.0f, 
                  coordenadasMundo[3]/2.0f + 2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, caracter);
    }

    // Instruções
    glColor3f(1.0f, 1.0f, 1.0f);
    buffer.str("");
    buffer.clear();
    buffer << "W, S, A, D - Move the ship in cardinal directions";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 5.0f, 
                  coordenadasMundo[3]/2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    buffer.str("");
    buffer.clear();
    buffer << "Q, E - Rotate the ship in 90 degree angles";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 5.0f, 
                  coordenadasMundo[3]/2.0f - 1.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    buffer.str("");
    buffer.clear();
    buffer << "Space - Shoot";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 5.0f, 
                  coordenadasMundo[3]/2.0f - 2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    // Voltar
    glColor3f(0.0f, 1.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Press ENTER to go back";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f - 4.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }
}
GLvoid desenhaEcraSuporte(GLvoid){
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(coordenadasMundo[0], coordenadasMundo[1], coordenadasMundo[2], coordenadasMundo[3]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    desenhaUIEcraSuporte();

    glutSwapBuffers();


}
GLvoid idleEcraSuporte(GLvoid){

}
GLvoid tecladoSuporte(unsigned char tecla, int x, int y){
    switch (tecla)
    {
    
    case 13:
        //depende qual era o ultimo estado. Se era o pausa, volta ao estado de pausa, se era o ecra inicial, volta ao ecra inicial
        
        
        if (estadoAtual == AJUDAPAUSA) 
        {
            glutDisplayFunc(desenhaEcraPausa);
            glutKeyboardFunc(tecladoPausa);
            glutIdleFunc(idleEcraPausa);
            estadoAtual = PAUSADO;
            glutPostRedisplay();
        }
        else if (estadoAtual == AJUDAINICIAL)
        {
            glutDisplayFunc(desenhaEcraInicial);
            glutKeyboardFunc(tecladoEcraInicial);
            glutIdleFunc(idleEcraInicial);
            estadoAtual = INICIAL;
            glutPostRedisplay();
        }
        break;
    
    default:
        break;
    }
}

/*----Funcoes de pausa----*/
GLvoid desenhaUIEcraPausa(GLvoid){
    stringstream buffer;
    char caracter;

    // Título
    glColor3f(0.0f, 1.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "GAME PAUSED";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 2.0f, 
                  coordenadasMundo[3]/2.0f + 2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, caracter);
    }

    // Instruções
    glColor3f(0.0f, 1.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Press ENTER to continue";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Press M to return to menu";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f - 1.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Press H for help";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f - 2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }
}

GLvoid desenhaEcraPausa(GLvoid){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();  
     
    gluOrtho2D(coordenadasMundo[0], coordenadasMundo[1], coordenadasMundo[2], coordenadasMundo[3]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    desenhaUIEcraPausa();

    glutSwapBuffers();
}

GLvoid idleEcraPausa(GLvoid){
    
}

GLvoid tecladoPausa(unsigned char tecla, int x, int y){
    switch (tecla) {
        case 13: //Enter
        
            // Volta para o jogo
            glutDisplayFunc(desenhaEcraJogo);
            glutKeyboardFunc(tecladoJogo);
            glutIdleFunc(idleJogo);
            estadoAtual = JOGANDO;
            glutPostRedisplay();
            break;

        case 'm':
        case 'M':
            // Volta para o menu inicial
            glutDisplayFunc(desenhaEcraInicial);
            glutKeyboardFunc(tecladoEcraInicial);
            glutIdleFunc(idleEcraInicial);
            estadoAtual = INICIAL;
            glutPostRedisplay();
            break;

        case 'h':
        case 'H':
            // Vai para a tela de ajuda
            glutDisplayFunc(desenhaEcraSuporte);
            glutIdleFunc(idleEcraSuporte);
            glutKeyboardFunc(tecladoSuporte);
            estadoAtual = AJUDAPAUSA;
            glutPostRedisplay();
            break;
    }
}

/*----Funcoes GameOver----*/
GLvoid desenhaUIGameOver(GLvoid) {
    stringstream buffer;
    char caracter;

    // Título
    glColor3f(1.0f, 0.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "GAME OVER";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 2.0f, 
                  coordenadasMundo[3]/2.0f + 2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, caracter);
    }

    // Estatísticas
    glColor3f(1.0f, 1.0f, 1.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Game Time: " << (int)tempoJogo << " seconds";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    buffer.str("");
    buffer.clear();
    buffer << "Enemies eliminated: " << inimigosEliminados;
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f - 1.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    // Voltar
    glColor3f(0.0f, 1.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Press ENTER to play again";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f - 3.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }
}

GLvoid desenhaEcraGameOver(GLvoid) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();  
     
    gluOrtho2D(coordenadasMundo[0], coordenadasMundo[1], coordenadasMundo[2], coordenadasMundo[3]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    desenhaUIGameOver();

    glutSwapBuffers();
}

GLvoid tecladoGameOver(unsigned char tecla, int x, int y) {
    switch(tecla) {
        case 13: //ENTER para voltar ao menu de início de jogo 

            glutDisplayFunc(desenhaEcraInicial);
            glutKeyboardFunc(tecladoEcraInicial);
            glutIdleFunc(idleEcraInicial);
            glutPostRedisplay();

            estadoAtual = INICIAL;
            break;
    }
}

GLvoid idleEcraGameOver(GLvoid) {
    
    //glutPostRedisplay();
}

/*----Funcoes GameWIn----*/
GLvoid desenhaUIGameWIn(GLvoid) {
    stringstream buffer;
    char caracter;

    // Título
    glColor3f(0.0f, 1.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "VICTORY!";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 2.0f, 
                  coordenadasMundo[3]/2.0f + 2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, caracter);
    }

    // Estatísticas
    glColor3f(1.0f, 1.0f, 1.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Score: " << (dificuldadeSelecionada == 1 ? 0 : pontuacaoTotal);
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f + 1.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    buffer.str("");
    buffer.clear();
    buffer << "Game Time: " << (int)tempoJogo << " seconds";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    buffer.str("");
    buffer.clear();
    buffer << "Enemies eliminated: " << inimigosEliminados;
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f - 1.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    // Voltar
    glColor3f(0.0f, 1.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Press ENTER to play again";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 3.0f, 
                  coordenadasMundo[3]/2.0f - 3.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }
}

GLvoid desenhaEcraGameWIn(GLvoid) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();  
     
    gluOrtho2D(coordenadasMundo[0], coordenadasMundo[1], coordenadasMundo[2], coordenadasMundo[3]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    desenhaUIGameWIn();

    glutSwapBuffers();
}

GLvoid idleEcraGameWIn(GLvoid) {
    // Não faz nada, apenas mantém a tela de vitória estática
    //glutPostRedisplay();
}

GLvoid tecladoGameWIn(unsigned char tecla, int x, int y) {
    switch(tecla) {
        case 13: //ENTER para voltar ao menu de início de jogo

            glutDisplayFunc(desenhaEcraInicial);
            glutKeyboardFunc(tecladoEcraInicial);
            glutIdleFunc(idleEcraInicial);
            glutPostRedisplay();

            estadoAtual = INICIAL;
            break;
    }
}

/*----funcao cria nave jogador----*/
GLvoid criarNaveJogador(){
    naveJogador = new NaveJogador(velocidadeNaveAmiga); 

}


/*----funcao cria inimigos----*/
GLvoid criarNavesInimigo(){
    

    // Grade de inimigos 
    GLfloat linhas, colunas;

    // Calcula o número de naves que cabem na tela
    linhas = (coordenadasMundo[3]-coordenadasMundo[2])/(naveInimigaTamanho[1]);
    colunas = (coordenadasMundo[1]-coordenadasMundo[0])/(naveInimigaTamanho[0]);
    
    // Aplica as razões para limitar o número de naves
    linhas = (GLint)(linhas * razaoInimigoVertical); //20% por defeito
    colunas = (GLint)(colunas * razaoInimigosHorizontal); //80% por defeito


    // Cria as naves inimigas em uma grade
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            // Calcula a posição inicial de cada nave inimiga
            GLfloat posX = coordenadasMundo[0] + distanciaHorizontalInimigos * (j + 1);
            GLfloat posY = coordenadasMundo[3] - distanciaVerticalInimigos * (i + 1);

            // Cria uma nova nave inimiga e define sua posição inicial
            NaveInimiga* novaNave = new NaveInimiga(velocidadeNaveInimiga);
            novaNave->setPosicaoNaveInimiga(posX, posY);
            
            // Adiciona a nave ao vetor de naves inimigas
            navesInimigas.push_back(novaNave);
        }
    }



}



/*----funcoes do jogo----*/

GLvoid moverNavesInimigas(GLvoid) {
    GLboolean mudarDirecao = false;
    GLfloat limiteEsquerda = coordenadasMundo[0] + (naveInimigaTamanho[0] * escala) / 2;
    GLfloat limiteDireita = coordenadasMundo[1] - (naveInimigaTamanho[0] * escala) / 2;

    // Verifica se alguma nave atingiu os limites
    for(NaveInimiga* nave : navesInimigas) {
        if(nave) {
            GLfloat* posicao = nave->getPosicaoNaveInimiga();
            if((movendoDireita && posicao[0] >= limiteDireita) || 
               (!movendoDireita && posicao[0] <= limiteEsquerda)) {
                mudarDirecao = true;
                break;
            }
        }
    }

    // Se atingiu o limite, muda a direção e desce
    if(mudarDirecao) {
        movendoDireita = !movendoDireita;
        ciclosMovimento++;
        
        if(ciclosMovimento >= ciclosParaDescer) {
            // Move todas as naves para baixo
            for(NaveInimiga* nave : navesInimigas) {
                if(nave) {
                    GLfloat* posicao = nave->getPosicaoNaveInimiga();
                    nave->setPosicaoNaveInimiga(posicao[0], posicao[1] - distanciaVerticalInimigos);
                    
                    // Verifica se alguma nave atingiu o limite inferior
                    if(posicao[1] - distanciaVerticalInimigos <= coordenadasMundo[2] + (naveInimigaTamanho[1] * escala)) {
                        // Game Over
                        estadoAtual = GAMEOVER;
                        glutDisplayFunc(desenhaEcraGameOver);
                        glutKeyboardFunc(tecladoGameOver);
                        glutIdleFunc(idleEcraGameOver);
                        glutPostRedisplay();
                        return;
                    }
                }
            }
            ciclosMovimento = 0;
        }
    }

    // Move todas as naves na direção atual
    for(NaveInimiga* nave : navesInimigas) {
        if(nave) {
            GLfloat* posicao = nave->getPosicaoNaveInimiga();
            GLfloat novaPosicao = posicao[0] + (movendoDireita ? velocidadeHorizontalInimigos : -velocidadeHorizontalInimigos);
            nave->setPosicaoNaveInimiga(novaPosicao, posicao[1]);
        }
    }
}

GLvoid verificaColisaoJogadorInimigo() {
    if (!naveJogador) return;
    
    GLfloat* posicaoJogador = naveJogador->getPosicao();
    GLfloat tamanhoJogador = naveJogadorTamanho[0]*escala; // Tamanho padrão da nave do jogador
    
    for (int i = 0; i < navesInimigas.size(); i++) {
        NaveInimiga* naveInimiga = navesInimigas[i];
        if (naveInimiga) {
            GLfloat* posicaoInimiga = naveInimiga->getPosicaoNaveInimiga();
            GLfloat tamanhoInimigo = naveInimigaTamanho[0]*escala;
            
            // Verifica colisão
            if (posicaoJogador[0] + tamanhoJogador >= posicaoInimiga[0] - tamanhoInimigo &&
                posicaoJogador[0] - tamanhoJogador <= posicaoInimiga[0] + tamanhoInimigo &&
                posicaoJogador[1] + tamanhoJogador >= posicaoInimiga[1] - tamanhoInimigo &&
                posicaoJogador[1] - tamanhoJogador <= posicaoInimiga[1] + tamanhoInimigo) {
                
                // Remove a nave inimiga
                delete naveInimiga;
                navesInimigas.erase(navesInimigas.begin() + i);
                
                // Reduz uma vida do jogador
                vidasJogador--;
                
                // Verifica se o jogo acabou
                if (vidasJogador <= 0) {
                    estadoAtual = GAMEOVER;
                    glutDisplayFunc(desenhaEcraGameOver);
                    glutKeyboardFunc(tecladoGameOver);
                    glutIdleFunc(idleEcraGameOver);
                    glutPostRedisplay();
                    return;
                }
                
                break;
            }
        }
    }
}

GLvoid verificaPosicaoJogador() {

    GLfloat* posicaoJogador = naveJogador->getPosicao();
    GLfloat posicaoYInimigoMaisBaixo = coordenadasMundo[3];

    for (NaveInimiga* nave : navesInimigas) {
        if (nave) {
            GLfloat* posicaoInimigo = nave->getPosicaoNaveInimiga();
            if (posicaoInimigo[1] < posicaoYInimigoMaisBaixo) {
                posicaoYInimigoMaisBaixo = posicaoInimigo[1];
            }
        }
    }

    jogadorAtrasLinhaInimiga = (posicaoJogador[1] > posicaoYInimigoMaisBaixo);
    
    if (jogadorAtrasLinhaInimiga) {
        pontuacaoTotal = pontuacaoTotal -5;
        vidasJogador = vidasJogador <= 1 ? 1: vidasJogador -1 ; 
    }
}

GLvoid idleJogo(GLvoid) {
    GLfloat tempoAtual = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    
    // Atualiza o tempo de jogo
    if (tempoAtual - ultimoTempoAtualizacao >= 1.0f) {
        tempoJogo += 1.0f;
        ultimoTempoAtualizacao = tempoAtual;
    }

    // Verifica se a nave do jogador esta atrás das linhas inimigas
    verificaPosicaoJogador();

    // Verifica colisão entre jogador e inimigos
    verificaColisaoJogadorInimigo();

    // Move as naves inimigas
    moverNavesInimigas();
    
    // Move os projéteis do jogador
    for (int i = 0; i < projeteisJogador.size(); i++) {
        Projetil* projetil = projeteisJogador[i];
        if (projetil) {
            if (projetil->moveProjetil()) {
                // Se o projétil saiu da tela, remove-o
                delete projetil;
                projeteisJogador.erase(projeteisJogador.begin() + i);
                i--; // Ajusta o índice pois removemos um elemento
                continue;
            }
            
            // Verifica colisão com naves inimigas
            for (int j = 0; j < navesInimigas.size(); j++) {
                NaveInimiga* naveInimiga = navesInimigas[j];
                if (naveInimiga) {
                    GLfloat* posicaoInimiga = naveInimiga->getPosicaoNaveInimiga();
                    GLfloat tamanhoInimigo = naveInimigaTamanho[0] * escala;
                    
                    if (projetil->verificaColisao(posicaoInimiga, &tamanhoInimigo)) {
                        // Remove o projétil e a nave inimiga
                        delete projetil;
                        projeteisJogador.erase(projeteisJogador.begin() + i);
                        i--;
                        
                        delete naveInimiga;
                        navesInimigas.erase(navesInimigas.begin() + j);
                        
                        // Atualiza pontuação
                        inimigosEliminados++;
                        pontuacaoTotal += 10; // 10 pontos por inimigo eliminado
                        break;
                    }
                }
            }
        }
    }
    
    // Lógica de tiro dos inimigos
    if (tempoAtual - ultimoTiroInimigo >= intervaloDisparoInimigo) {
        // Escolhe uma nave inimiga aleatória para atirar
        if (!navesInimigas.empty()) {
            int indiceAleatorio = rand() % navesInimigas.size();
            NaveInimiga* naveAtiradora = navesInimigas[indiceAleatorio];
            
            if (naveAtiradora) {
                GLfloat* posicaoNave = naveAtiradora->getPosicaoNaveInimiga();
                // Cria um projétil inimigo apontando para baixo (direção 3)
                Projetil* novoProjetil = new Projetil(posicaoNave[0], posicaoNave[1], 3, false);
                projeteisInimigos.push_back(novoProjetil);
                ultimoTiroInimigo = tempoAtual;
            }
        }
    }

    // Move os projéteis inimigos
    for (int i = 0; i < projeteisInimigos.size(); i++) {
        Projetil* projetil = projeteisInimigos[i];
        if (projetil) {
            projetil->moveProjetil();
            
            // Verifica se o projétil está fora da tela
            if (!projetil->estaNaTela()) {
                delete projetil;
                projeteisInimigos.erase(projeteisInimigos.begin() + i);
                i--; // Ajusta o índice pois removemos um elemento
                continue;
            }
            
            // Verifica colisão com o jogador
            if (naveJogador) {
                GLfloat* posicaoJogador = naveJogador->getPosicao();
                GLfloat tamanhoJogador = naveJogadorTamanho[0] * escala;
                
                if (projetil->verificaColisao(posicaoJogador, &tamanhoJogador)) {
                    // Remove o projétil
                    delete projetil;
                    projeteisInimigos.erase(projeteisInimigos.begin() + i);
                    i--;
                    
                    // Reduz uma vida do jogador
                    vidasJogador--;
                    
                    // Verifica se o jogo acabou
                    if (vidasJogador <= 0) {
                        estadoAtual = GAMEOVER;
                        glutDisplayFunc(desenhaEcraGameOver);
                        glutKeyboardFunc(tecladoGameOver);
                        glutIdleFunc(idleEcraGameOver);
                        glutPostRedisplay();
                        return;
                    }
                }
            }
        }
    }

    // Verifica se o jogador ganhou (eliminou todos os inimigos)
    if (navesInimigas.empty()) {
        // Calcula bônus de tempo
        GLint bonusTempo = (int)(1000 / tempoJogo); // Bônus inversamente proporcional ao tempo
        pontuacaoTotal += bonusTempo;
        
        // Calcula bônus de vidas
        GLint bonusVidas = vidasJogador * 500; // 500 pontos por vida restante
        pontuacaoTotal += bonusVidas;
        
        estadoAtual = GAMEWIN;
        glutDisplayFunc(desenhaEcraGameWIn);
        glutKeyboardFunc(tecladoGameWIn);
        glutIdleFunc(idleEcraGameWIn);
        glutPostRedisplay();
        return;
    }

    glutPostRedisplay();
}

GLvoid tecladoJogo(unsigned char tecla, int x, int y) {
    GLboolean move = false;
    GLboolean rotaciona = false;
    GLboolean projeteisAtivos = false;
    GLfloat tempoAtual = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    switch (tecla) {
    case 27: // "ESC"
        // Pausa o jogo
        glutDisplayFunc(desenhaEcraPausa);
        glutKeyboardFunc(tecladoPausa);
        glutIdleFunc(idleEcraPausa);
        estadoAtual = PAUSADO;
        glutPostRedisplay();
        break;

    case 32: // space
        if (tempoAtual - ultimoDisparo >= intervaloDisparo) {
            // jogador "dispara" um projetil
            if (naveJogador) {
                GLfloat* posicaoNave = naveJogador->getPosicao();
                GLfloat angulo = naveJogador->getAnguloRotacao();
            
                // Cria um novo projétil na posição do bico da nave
                Projetil* novoProjetil = new Projetil(posicaoNave[0], posicaoNave[1], (int)(angulo/90.0f), true);
                projeteisJogador.push_back(novoProjetil);
                projeteisAtivos = true;
                
                // Atualiza o tempo do último disparo
                ultimoDisparo = tempoAtual;
                
                glutPostRedisplay();
            }
        } 
        break;
    
    case 8: // "BACKSPACE"
        exit(0); // Sair do jogo
        break;

    case 'w':
    case 'W': //vai para cima
        move = naveJogador->moverNave(2); 
        std::cout << "X = " << naveJogador->getPosicao()[0] << ", Y = " << naveJogador->getPosicao()[1] << std::endl;
        break;

    case 'a':
    case 'A': //vai para esquerda
        move = naveJogador->moverNave(1);
        std::cout << "X = " << naveJogador->getPosicao()[0] << ", Y = " << naveJogador->getPosicao()[1] << std::endl;
        break;

    case 's':
    case 'S': //vai para baixo
        move = naveJogador->moverNave(3);
        std::cout << "X = " << naveJogador->getPosicao()[0] << ", Y = " << naveJogador->getPosicao()[1] << std::endl;
        break;

    case 'd':
    case 'D': //vai para direita
        move = naveJogador->moverNave(0);
        std::cout << "X = " << naveJogador->getPosicao()[0] << ", Y = " << naveJogador->getPosicao()[1] << std::endl;
        break;
    case 'q':
    case 'Q': //roda 90graus "esquerda" "anti-horario"
        rotaciona = naveJogador->rodarNave(1);
        std::cout <<" Angulo de rotação: " << naveJogador->getAnguloRotacao() << std::endl;

        break;
    case 'e':
    case 'E': //roda -90graus "direita" "horario"
        rotaciona = naveJogador->rodarNave(0);
        std::cout <<" Angulo de rotação: " << naveJogador->getAnguloRotacao() << std::endl;
        break;
    }
    

    //se a nave se mexeu ou rotacionou é preciso redesenhar
    if (move || rotaciona || projeteisAtivos) {
        glutPostRedisplay();
    }
}



GLvoid desenhaUIJogo(GLvoid) {
    stringstream buffer;
    char caracter;

    // Desenha as vidas
    glColor3f(1.0f, 0.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Lifes: " << vidasJogador;
    glRasterPos3f(coordenadasMundo[0] + 1.0f, coordenadasMundo[3] - 1.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    // Desenha o tempo
    glColor3f(1.0f, 1.0f, 1.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Time: " << (int)tempoJogo << "s";
    glRasterPos3f(coordenadasMundo[0] + ((coordenadasMundo[1] - coordenadasMundo[0])/2.0f) - 2.0f, coordenadasMundo[3] - 1.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    // Desenha o número de inimigos eliminados
    glColor3f(0.0f, 1.0f, 0.0f);
    buffer.str("");
    buffer.clear();
    buffer << "Enemies: " << inimigosEliminados;
    glRasterPos3f(coordenadasMundo[1] - 5.0f, coordenadasMundo[3] - 1.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }

    // Desenha o aviso de posição atrás da linha inimiga
    if (jogadorAtrasLinhaInimiga) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho para o aviso
        buffer.str("");
        buffer.clear();
        buffer << "BEHIND ENEMY LINES!";
        glRasterPos3f(coordenadasMundo[1] - 9.0f, coordenadasMundo[2] + 1.0f, 0.0f);
        while (buffer.get(caracter)) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
        }
    }

    // Desenha o status do disparo

    GLfloat tempoAtual = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    GLfloat tempoRestante = intervaloDisparo - (tempoAtual - ultimoDisparo);

    if (tempoRestante <= 0) {
        // Pode atirar
        glColor3f(0.0f, 1.0f, 0.0f); // Verde
        buffer.str("");
        buffer.clear();
        buffer << "FIRE!";
    } else {
        // Carregando
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
        buffer.str("");
        buffer.clear();
        buffer << "LOADING...";
    }

    glRasterPos3f(coordenadasMundo[0] + 1.0f, coordenadasMundo[2] + 1.0f, 0.0f);
    while (buffer.get(caracter)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, caracter);
    }
}

GLvoid desenhaEcraJogo(GLvoid) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();  
     
    gluOrtho2D(coordenadasMundo[0], coordenadasMundo[1], coordenadasMundo[2], coordenadasMundo[3]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Desenha a nave do jogador
    if (naveJogador) {
        naveJogador->desenha();
    }

    // Desenha todas as naves inimigas
    for(NaveInimiga* naveInimiga : navesInimigas) {
        if (naveInimiga) {
            naveInimiga->desenhaNaveInimiga();
        }
    }

    // Desenha todos os projéteis do jogador
    for(Projetil* projetil : projeteisJogador) {
        if (projetil) {
            projetil->desenhaProjetil();
        }
    }

    // Desenha todos os projéteis inimigos
    for(Projetil* projetil : projeteisInimigos) {
        if (projetil) {
            projetil->desenhaProjetil();
        }
    }

    // Desenha a UI do jogo
    desenhaUIJogo();

    glutSwapBuffers();
}



int main (int argc, char*argv[]){
    glutInit(&argc, argv); 

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutInitWindowPosition(larguraEcra, 0); //definir a posição inicial da janela de renderização no ecrã. (x,y) é o ponto de origem da janela. (0,0) é o canto superior esquerdo do ecrã, ou seja, a janela será aberta no canto superior esquerdo do ecrã.
    glutInitWindowSize(larguraEcra, alturaEcra);  //define o tamanho da janela 
    
    glutCreateWindow("Alien Invasion"); 
    


    glutDisplayFunc(desenhaEcraInicial);
    glutIdleFunc(idleEcraInicial);
    glutKeyboardFunc(tecladoEcraInicial); 

    glutMainLoop();//Inicia o ciclo de eventos que permite detetar eventos

    return 0;
}