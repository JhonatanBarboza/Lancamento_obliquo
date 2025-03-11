// Caminho: simulacao_lancamento_obliquo.cpp
#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Constantes físicas
const float GRAVIDADE = 9.8f; // Aceleração gravitacional (m/s²)

// Variáveis globais para o lançamento
float velocidadeInicial = 20.0f; // Velocidade inicial (m/s)
float anguloLancamento = 45.0f;  // Ângulo de lançamento (graus)
float tempoSimulado = 0.0f;      // Tempo decorrido na simulação (s)
float raioObjeto = 0.5f;         // Raio do objeto
float intervaloTempo = 0.016f;   // Intervalo entre quadros (~60 FPS)

// Limites da simulação
float nivelChao = 0.0f;          // Altura do chão
float larguraTela = 40.0f;       // Largura da tela (unidades)
float alturaTela = 20.0f;        // Altura da tela (unidades)

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

    // Calcula a posição do objeto
    float anguloRad = anguloLancamento * M_PI / 180.0f;
    float x = velocidadeInicial * cos(anguloRad) * tempoSimulado;
    float y = velocidadeInicial * sin(anguloRad) * tempoSimulado - 0.5f * GRAVIDADE * tempoSimulado * tempoSimulado;

    // Desenha o chão
    glColor3f(0.0f, 0.0f, 0.0f); // Preto
    glBegin(GL_LINES);
    glVertex2f(0.0f, nivelChao);
    glVertex2f(larguraTela, nivelChao);
    glEnd();

    // Desenha o objeto
    if (y > nivelChao) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
        desenharCirculo(x, y, raioObjeto);
    }

    glutSwapBuffers();
}

// Atualização da simulação
void atualizar(int valor) {
    tempoSimulado += intervaloTempo;

    // Para a simulação quando o objeto atinge o solo
    float anguloRad = anguloLancamento * M_PI / 180.0f;
    float y = velocidadeInicial * sin(anguloRad) * tempoSimulado - 0.5f * GRAVIDADE * tempoSimulado * tempoSimulado;
    if (y <= nivelChao) {
        tempoSimulado = 0.0f; // Reinicia a simulação
    }

    glutPostRedisplay();              // Redesenha a tela
    glutTimerFunc(16, atualizar, 0);  // Chama a função novamente após ~16ms
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
    std::cout << "Digite a velocidade inicial (m/s): ";
    std::cin >> velocidadeInicial;
    std::cout << "Digite o ângulo de lançamento (graus): ";
    std::cin >> anguloLancamento;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1600, 800);
    glutCreateWindow("Simulação de Lançamento Oblíquo");

    inicializarOpenGL();
    glutDisplayFunc(exibir);
    glutTimerFunc(16, atualizar, 0);

    glutMainLoop();
    return 0;
}
