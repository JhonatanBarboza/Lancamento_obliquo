// Caminho: simulacao_lancamento_viscoso.cpp
#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Constantes físicas
const float GRAVIDADE = 9.8f; // Aceleração gravitacional (m/s²)
float coeficienteResistencia = 0.1f; // Coeficiente de resistência viscosa
float massaObjeto = 1.0f;        // Massa do objeto (kg)


// Variáveis globais para o lançamento
float velocidadeInicial = 20.0f; // Velocidade inicial (m/s)
float anguloLancamento = 45.0f;  // Ângulo de lançamento (graus)
float tempoSimulado = 0.0f;      // Tempo decorrido na simulação (s)
float intervaloTempo = 0.016f;   // Intervalo entre quadros (~60 FPS)

// Variáveis de estado
float velocidadeX, velocidadeY;  // Componentes de velocidade
float posicaoX = 0.0f, posicaoY = 0.0f; // Posição atual do objeto

// Dimensões e controle da simulação
float raioObjeto = 0.5f;         // Raio do objeto
float nivelChao = 0.0f;          // Altura do chão
float larguraTela = 40.0f;       // Largura da tela (unidades)
float alturaTela = 20.0f;        // Altura da tela (unidades)
bool simulacaoPausada = true;    // Simulação inicia pausada
bool objetoVisivel = true;       // Controle de visibilidade do objeto

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

// Função de exibição
void exibir() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha o chão
    glColor3f(0.0f, 0.0f, 0.0f); // Preto
    glBegin(GL_LINES);
    glVertex2f(0.0f, nivelChao);
    glVertex2f(larguraTela, nivelChao);
    glEnd();

    // Desenha o objeto apenas se ele for visível
    if (objetoVisivel && posicaoY > nivelChao && posicaoX < larguraTela) {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul
        desenharCirculo(posicaoX, posicaoY, raioObjeto);
    }

    // Mensagem de pausa
    if (simulacaoPausada) {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul
        glRasterPos2f(15.0f, 15.0f);
        std::string mensagem = "Pressione ESPACO para lancar o objeto";
        for (char c : mensagem) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    // Informações na tela
    glColor3f(0.0f, 0.0f, 1.0f); // Azul
    glRasterPos2f(1.0f, alturaTela - 2.0f);
    std::string info = "Velocidade: " + std::to_string(velocidadeInicial) + " m/s, Angulo: " + std::to_string(anguloLancamento) + " graus";
    for (char c : info) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glutSwapBuffers();
}

// Atualização da simulação
void atualizar(int valor) {
    if (!simulacaoPausada) {
        // Atualiza o tempo
        tempoSimulado += intervaloTempo;

        // Calcula as forças viscosas
        float forcaViscosaX = -coeficienteResistencia * velocidadeX;
        float forcaViscosaY = -coeficienteResistencia * velocidadeY;

        // Atualiza as velocidades considerando a resistência viscosa
        velocidadeX += (forcaViscosaX / massaObjeto) * intervaloTempo;
        velocidadeY += ((forcaViscosaY - GRAVIDADE * massaObjeto) / massaObjeto) * intervaloTempo;

        // Atualiza as posições
        posicaoX += velocidadeX * intervaloTempo;
        posicaoY += velocidadeY * intervaloTempo;

        // Verifica se o objeto saiu da tela ou atingiu o chão
        if (posicaoY <= nivelChao || posicaoX > larguraTela) {
            objetoVisivel = false; // Torna o objeto invisível
            simulacaoPausada = true; // Pausa a simulação
        }
    }

    glutPostRedisplay();              // Redesenha a tela
    glutTimerFunc(16, atualizar, 0);  // Chama a função novamente após ~16ms
}

// Função para capturar teclas normais
void teclado(unsigned char tecla, int x, int y) {
    if (tecla == ' ') { // Espaço: relança o objeto
        if (simulacaoPausada) {
            simulacaoPausada = false; // Retoma a simulação
            tempoSimulado = 0.0f;     // Reinicia o tempo
            objetoVisivel = true;     // Torna o objeto visível novamente
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
            if (velocidadeInicial < 100.0f) velocidadeInicial += 1.0f;
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

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1600, 800);
    glutCreateWindow("Simulação de Lançamento com Resistência do Ar");

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
