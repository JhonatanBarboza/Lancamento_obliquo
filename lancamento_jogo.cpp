// Caminho: simulacao_lancamento_game.cpp
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <sstream>

// Constantes físicas
float GRAVIDADE = 9.8f; // Aceleração gravitacional (m/s²)

// Variaveis para jogar
float viscosidade = 0.1f; // Coeficiente de resistência viscosa
float massaObjeto = 1.0f;        // Massa do objeto (kg)

// Alvo
float alvoX = 36.0f;             // Posição X do retângulo-alvo
float alvoY = 5.0f;              // Posição Y do retângulo-alvo
float alvoLargura = 2.0f;        // Largura do retângulo-alvo
float alvoAltura = 2.0f;         // Altura do retângulo-alvo
bool jogadorGanhou = false;      // Estado de vitória do jogador
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
float raioObjeto = 0.5f;         // Raio do objeto
float nivelChao = 0.0f;          // Altura do chão
float larguraTela = 40.0f;       // Largura da tela (unidades)
float alturaTela = 20.0f;        // Altura da tela (unidades)
bool simulacaoPausada = true;    // Simulação inicia pausada
bool objetoVisivel = true;       // Controle de visibilidade do objeto

void jogo() {
    // Valores aleatórios iniciais
    GRAVIDADE = 2.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 7.99f));   // Entre 2 e 8000
    viscosidade = 0.05f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.95f)); // Entre 0.05 e 3.0
    massaObjeto = 0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 4.9f));   // Entre 0.1 e 5.0
    alvoX = 20.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 18.0f));      // Entre 20 e 38
    alvoY = 5.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 10.0f));       // Entre 5 e 15
    alvoLargura = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));  // Entre 1 e 4
    alvoAltura = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));   // Entre 1 e 4
 
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

// Verifica colisão com o retângulo-alvo
bool verificarColisao() {
    return (posicaoX + raioObjeto >= alvoX &&
            posicaoX - raioObjeto <= alvoX + alvoLargura &&
            posicaoY + raioObjeto >= alvoY &&
            posicaoY - raioObjeto <= alvoY + alvoAltura);
}

#include <iomanip> // Para std::setprecision e std::fixed

#include <iomanip> // Para std::setprecision e std::fixed

// Função de exibição
void exibir() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha o chão
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, nivelChao);
    glVertex2f(larguraTela, nivelChao);
    glEnd();

    // Desenha o objeto apenas se ele for visível
    if (objetoVisivel && posicaoY > nivelChao && posicaoX < larguraTela) {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul
        desenharCirculo(posicaoX, posicaoY, raioObjeto);
    }

    // Desenha o retângulo-alvo
    glColor3f(0.0f, 0.0f, 0.0f);
    desenharRetangulo(alvoX, alvoY, alvoLargura, alvoAltura);

    // Mensagem de pausa
    if (simulacaoPausada) {
        glColor3f(0.0f, 0.0f, 1.0f);
        glRasterPos2f(15.0f, 15.0f);

        if (jogadorGanhou) {
            // Exibe a mensagem de vitória com o número de tentativas
            std::ostringstream mensagem;
            mensagem << "Parabens! Você ganhou com " << tentativas << " tentativas.";
            for (char c : mensagem.str()) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }

            // Mensagem de reinício
            glRasterPos2f(15.0f, 13.0f); // Ajustar a posição para a próxima linha
            std::string reiniciar = "PRESSIONE ESPACO PARA JOGAR NOVAMENTE.";
            for (char c : reiniciar) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        } else {
            // Exibe a mensagem para continuar
            std::string mensagem = "PRESSIONE ESPACO PARA LANCAR.";
            for (char c : mensagem) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        }
    }

    // Exibe informações formatadas com duas casas decimais
    glColor3f(0.0f, 0.0f, 1.0f);

    // Velocidade e Ângulo
    glRasterPos2f(1.0f, alturaTela - 2.0f);
    std::ostringstream info;
    info << std::fixed << std::setprecision(2);
    info << "Velocidade: " << velocidadeInicial << " m/s, Ângulo: " << anguloLancamento << " graus";
    std::string infoStr = info.str();
    for (char c : infoStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Viscosidade, Massa e Gravidade
    glRasterPos2f(1.0f, alturaTela - 4.0f); // Ajusta a posição vertical
    std::ostringstream parametros;
    parametros << std::fixed << std::setprecision(2);
    parametros << "Viscosidade: " << viscosidade 
               << ", Massa: " << massaObjeto << " kg"
               << ", Gravidade: " << GRAVIDADE << " m/s²";
    std::string parametrosStr = parametros.str();
    for (char c : parametrosStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Troca os buffers
    glutSwapBuffers();
}



// Atualização da simulação
void atualizar(int valor) {
    if (!simulacaoPausada) {
        // Atualiza o tempo
        tempoSimulado += intervaloTempo;

        // Calcula as forças viscosas
        float forcaViscosaX = -viscosidade * velocidadeX;
        float forcaViscosaY = -viscosidade * velocidadeY;

        // Atualiza as velocidades considerando a resistência viscosa
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
    // Inicializa o gerador de números aleatórios
    srand(static_cast<unsigned int>(time(0)));

    void jogo();

    // Configuração OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1600, 800);
    glutCreateWindow("Simulacao de Lançamento Gamificada");

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

