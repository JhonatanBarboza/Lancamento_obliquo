#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <cstdlib> // Para rand() e srand()
#include <ctime>   // Para time()
#include <iomanip> // Para std::setprecision e std::fixed


// Constantes físicas
float GRAVIDADE = 9.8f; // Aceleração gravitacional (m/s²)

// Variáveis para jogar
float viscosidade = 0.1f; // Coeficiente de resistência viscosa
float massaObjeto = 1.0f; // Massa do objeto (kg)

// Alvo
float alvoX = 36.0f;      // Posição X do retângulo-alvo
float alvoY = 5.0f;       // Posição Y do retângulo-alvo
float alvoRaio = 2.0f;    // Raio do retângulo-alvo
bool jogadorGanhou = false; // Estado de vitória do jogador
int tentativas = 1;

// Variáveis globais para o lançamento
float velocidadeInicial = 20.0f; // Velocidade inicial (m/s)
float anguloLancamento = 45.0f;  // Ângulo de lançamento (graus)
float tempoSimulado = 0.0f;      // Tempo decorrido na simulação (s)
float intervaloTempo = 0.016f;   // Intervalo entre quadros (~60 FPS)

// Variáveis de estado
float velocidadeX, velocidadeY;  // Componentes de velocidade
float posicaoX = 0.0f, posicaoY = 0.0f; // Posição atual do objeto

// Dimensões e controle da simulação
float raioObjeto = 0.4f;         // Raio do objeto
float nivelChao = 0.0f;          // Altura do chão
float larguraTela = 40.0f;       // Largura da tela (unidades)
float alturaTela = 20.0f;        // Altura da tela (unidades)
bool simulacaoPausada = true;    // Simulação inicia pausada
bool objetoVisivel = true;       // Controle de visibilidade do objeto

// Variáveis para o fundo dinâmico
float corCeuR = 0.53f, corCeuG = 0.81f, corCeuB = 0.92f; // Cor do céu (azul claro)
float tempoCeu = 0.0f; // Tempo para mudança de cor do céu

// Função para gerar valores aleatórios iniciais
void jogo() {
    GRAVIDADE = 2.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 7.99f));   // Entre 2 e 8000
    viscosidade = 0.05f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.95f)); // Entre 0.05 e 3.0
    massaObjeto = 0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 4.9f));   // Entre 0.1 e 5.0
    alvoX = 20.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 18.0f));      // Entre 20 e 38
    alvoY = 5.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 10.0f));       // Entre 5 e 15
    alvoRaio = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f)); // Entre 1 e 4
}

// Função para desenhar um círculo
void desenharCirculo(float x, float y, float raio) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i++) {
        float angulo = i * M_PI / 180.0f;
        glVertex2f(x + raio * cos(angulo), y + raio * sin(angulo));
    }
    glEnd();
}

// Função para desenhar um retângulo
void desenharRetangulo(float x, float y, float largura, float altura) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + largura, y);
    glVertex2f(x + largura, y + altura);
    glVertex2f(x, y + altura);
    glEnd();
}

// Função para desenhar o canhão
void desenharCanhao(float x, float y, float angulo) {
    glPushMatrix(); // Salva a matriz atual
    glTranslatef(x, y, 0.0f); // Translada para a posição do canhão
    glRotatef(angulo, 0.0f, 0.0f, 1.0f); // Rotaciona o canhão

    // Cano do canhão (retângulo)
    glColor3f(0.5f, 0.5f, 0.5f); // Cor cinza
    glBegin(GL_QUADS);
    glVertex2f(0.0f, -0.4f);
    glVertex2f(2.0f, -0.4f);
    glVertex2f(2.0f, 0.4f);
    glVertex2f(0.0f, 0.4f);
    glEnd();

    // Base do canhão (círculo)
    glColor3f(0.3f, 0.3f, 0.3f); // Cor cinza escuro
    desenharCirculo(0.0f, 0.0f, 0.6f);

    glPopMatrix(); // Restaura a matriz anterior
}

// Função para desenhar o fundo dinâmico
void desenharFundo() {
    // Céu gradiente
    glBegin(GL_QUADS);
    glColor3f(corCeuR, corCeuG, corCeuB); // Cor do céu
    glVertex2f(0.0f, alturaTela);
    glVertex2f(larguraTela, alturaTela);
    glColor3f(corCeuR * 0.8f, corCeuG * 0.8f, corCeuB * 0.8f); // Cor mais escura no horizonte
    glVertex2f(larguraTela, nivelChao);
    glVertex2f(0.0f, nivelChao);
    glEnd();

    // Montanhas
    glColor3f(0.0f, 0.4f, 0.0f); // Cor das montanhas
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, nivelChao);
    glVertex2f(10.0f, nivelChao + 8.0f);
    glVertex2f(20.0f, nivelChao);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(15.0f, nivelChao);
    glVertex2f(25.0f, nivelChao + 6.0f);
    glVertex2f(35.0f, nivelChao);
    glEnd();

    // Nuvens
    glColor3f(1.0f, 1.0f, 1.0f); // Cor das nuvens
    desenharCirculo(5.0f, 15.0f, 2.0f);
    desenharCirculo(8.0f, 15.0f, 2.0f);
    desenharCirculo(6.5f, 17.0f, 2.0f);

    desenharCirculo(20.0f, 18.0f, 2.5f);
    desenharCirculo(23.0f, 18.0f, 2.5f);
    desenharCirculo(21.5f, 20.0f, 2.5f);
}

// Função de exibição
void exibir() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha o fundo dinâmico
    desenharFundo();

    // Desenha o chão
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, nivelChao);
    glVertex2f(larguraTela, nivelChao);
    glEnd();

    // Desenha o objeto apenas se ele for visível
    if (objetoVisivel && posicaoY > nivelChao && posicaoX < larguraTela) {
        glColor3f(0.05f, 0.05f, 0.05f); // Azul
        desenharCirculo(posicaoX, posicaoY, raioObjeto);
    }

    // Desenha o retângulo-alvo
    desenharCirculo(alvoX, alvoY, alvoRaio);
    glColor3f(1.0f, 1.0f, 1.0f);
    desenharCirculo(alvoX, alvoY, alvoRaio/1.5);
    glColor3f(0.0f, 0.0f, 0.0f);
    desenharCirculo(alvoX, alvoY, alvoRaio/2.3);


    // Desenha o canhão
    desenharCanhao(0.0f, nivelChao, anguloLancamento);


    // Mensagem de pausa
    if (simulacaoPausada) {

        glColor3f(0.0f, 0.0f, 0.0f);
        glRasterPos2f(15.0f, 15.0f);

        if (jogadorGanhou) {

            glColor3f(1.0f, 1.0f, 1.0f);
            desenharRetangulo(14.5f, 14.7f, 9.5f, 1.f);

            std::ostringstream mensagem;
            mensagem << "Parabens! Voce ganhou com " << tentativas << " tentativas.";
            for (char c : mensagem.str()) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }

            glColor3f(1.0f, 1.0f, 1.0f);
            desenharRetangulo(14.5f, 12.7f, 11.8f, 1.f);

            glColor3f(0.0f, 0.0f, 0.0f);
            glRasterPos2f(15.0f, 13.0f);
            std::string reiniciar = "PRESSIONE ESPACO PARA JOGAR NOVAMENTE.";
            for (char c : reiniciar) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        } else {
            glColor3f(1.0f, 1.0f, 1.0f);
            desenharRetangulo(14.5f, 14.7f, 9.0f, 1.f);

            std::string mensagem = "PRESSIONE ESPACO PARA LANCAR.";
            for (char c : mensagem) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        }
    }

    // Exibe informações formatadas
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(1.0f, alturaTela - 2.0f);
    std::ostringstream info;
    info << std::fixed << std::setprecision(2);
    info << "Velocidade: " << velocidadeInicial << " m/s, Angulo: " << anguloLancamento << " graus";
    std::string infoStr = info.str();
    for (char c : infoStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2f(1.0f, alturaTela - 4.0f);
    std::ostringstream parametros;
    parametros << std::fixed << std::setprecision(2);
    parametros << "Viscosidade: " << viscosidade
               << ", Massa: " << massaObjeto << " kg"
               << ", Gravidade: " << GRAVIDADE << " m/s²";
    std::string parametrosStr = parametros.str();
    for (char c : parametrosStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glutSwapBuffers();
}


// Verifica colisão com o retângulo-alvo
bool verificarColisao() {
    return (posicaoX + raioObjeto >= alvoX &&
            posicaoX - raioObjeto <= alvoX + alvoRaio &&
            posicaoY + raioObjeto >= alvoY &&
            posicaoY - raioObjeto <= alvoY + alvoRaio);
}

// Atualização da simulação
void atualizar(int valor) {
    if (!simulacaoPausada) {
        // Atualiza o tempo
        tempoSimulado += intervaloTempo;

        // Atualiza a cor do céu (transição suave)
        tempoCeu += intervaloTempo;
        corCeuR = 0.53f + 0.1f * sin(tempoCeu * 0.25f);
        corCeuG = 0.81f + 0.1f * sin(tempoCeu * 0.35f);
        corCeuB = 0.92f + 0.1f * sin(tempoCeu * 0.45f);

        // Calcula as forças viscosas
        float forcaViscosaX = -viscosidade * velocidadeX;
        float forcaViscosaY = -viscosidade * velocidadeY;

        // Atualiza as velocidades
        velocidadeX += (forcaViscosaX / massaObjeto) * intervaloTempo;
        velocidadeY += ((forcaViscosaY - GRAVIDADE * massaObjeto) / massaObjeto) * intervaloTempo;

        // Atualiza as posições
        posicaoX += velocidadeX * intervaloTempo;
        posicaoY += velocidadeY * intervaloTempo;

        // Verifica colisão com o retângulo-alvo
        if (verificarColisao()) {
            jogadorGanhou = true;
            simulacaoPausada = true;
        }

        // Verifica se o objeto saiu da tela ou atingiu o chão
        if (posicaoY <= nivelChao || posicaoX > larguraTela) {
            tentativas++;
            objetoVisivel = false;
            simulacaoPausada = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, atualizar, 0);
}

// Função para capturar teclas normais
void teclado(unsigned char tecla, int x, int y) {
    if (tecla == ' ') { // Espaço: relança o objeto
        if (simulacaoPausada) {
            simulacaoPausada = false; // Retoma a simulação
            tempoSimulado = 0.0f;     // Reinicia o tempo
            objetoVisivel = true;     // Torna o objeto visível novamente
            if (jogadorGanhou) jogo();
            jogadorGanhou = false;    // Reseta o estado de vitória
            posicaoX = 0.0f;          // Reseta a posição inicial
            posicaoY = 0.0f;
            float anguloRad = anguloLancamento * M_PI / 180.0f;
            velocidadeX = velocidadeInicial * cos(anguloRad);
            velocidadeY = velocidadeInicial * sin(anguloRad);
        }
    }
}

// Função para capturar teclas especiais
void tecladoEspecial(int tecla, int x, int y) {
    if (simulacaoPausada) {
        switch (tecla) {
        case GLUT_KEY_UP: // Aumenta o ângulo
            if (anguloLancamento < 90.0f) anguloLancamento += 1.0f;
            break;
        case GLUT_KEY_DOWN: // Diminui o ângulo
            if (anguloLancamento > 0.0f) anguloLancamento -= 1.0f;
            break;
        case GLUT_KEY_RIGHT: // Aumenta a velocidade
            if (velocidadeInicial < 200.0f) velocidadeInicial += 1.0f;
            break;
        case GLUT_KEY_LEFT: // Diminui a velocidade
            if (velocidadeInicial > 1.0f) velocidadeInicial -= 1.0f;
            break;
        }
    }
    glutPostRedisplay(); // Atualiza a tela
}

// Inicialização do OpenGL
void inicializarOpenGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Cor de fundo: branco
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f, larguraTela, 0.0f, alturaTela);
}

// Configuração inicial
int main(int argc, char** argv) {
    // Inicializa o gerador de números aleatórios
    srand(static_cast<unsigned int>(time(0)));

    void jogo();

    // Configuração OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1600, 800);
    glutCreateWindow("Simulacao de Lancamento Gamificada");

    inicializarOpenGL();
    glutDisplayFunc(exibir);
    glutSpecialFunc(tecladoEspecial); // Captura teclas especiais
    glutKeyboardFunc(teclado);        // Captura teclas normais
    glutTimerFunc(16, atualizar, 0);

    // Configuração inicial da velocidade
    float anguloRad = anguloLancamento * M_PI / 180.0f;
    velocidadeX = velocidadeInicial * cos(anguloRad);
    velocidadeY = velocidadeInicial * sin(anguloRad);

    glutMainLoop();
    return 0;
}

