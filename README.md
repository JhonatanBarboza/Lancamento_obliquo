# Jogo de Lançamento Obliquo

Este repositório contém um jogo interativo que simula o lançamento oblíquo de um objeto, explorando a influência da gravidade e da força viscosa no movimento do projétil. O objetivo do jogo é ajustar o ângulo e a velocidade inicial do lançamento para acertar um retângulo alvo na tela.

![Image](https://github.com/user-attachments/assets/1769a548-6fc9-4519-be71-c45bc1235370)

## Funcionamento

O jogo permite ao jogador controlar o lançamento do objeto através das seguintes interações:

- **Teclas para cima e para baixo**: Ajustam o ângulo de lançamento.
- **Teclas para esquerda e para direita**: Controlam a velocidade inicial do lançamento.
- **Barra de espaço**: Realiza o lançamento do projétil.

O objetivo é acertar o retângulo alvo na tela. Quando o alvo é atingido, os valores da gravidade e da viscosidade são alterados aleatoriamente, reiniciando o desafio com novas condições físicas.

## Características do Jogo

- **Simulação Física**: O jogo utiliza equações de movimento para simular o efeito da gravidade e da resistência viscosa no projétil.
- **Interatividade**: O jogador pode ajustar o ângulo e a velocidade do lançamento para atingir o alvo.
- **Desafio Dinâmico**: A cada acerto, os parâmetros físicos do jogo são alterados, proporcionando um novo desafio a cada rodada.
- **Interface Gráfica**: Utiliza OpenGL para renderizar o cenário, o projétil e o alvo.

## Como Jogar

1. Ajuste o ângulo de lançamento usando as teclas para cima e para baixo.
2. Defina a velocidade inicial com as teclas para esquerda e para direita.
3. Pressione a barra de espaço para lançar o projétil.
4. Tente acertar o retângulo alvo. Se conseguir, o jogo reiniciará com novos valores de gravidade e viscosidade.

## Requisitos

- OpenGL
- GLUT (OpenGL Utility Toolkit)

## Compilação e Execução

Para compilar e executar o jogo, siga os passos abaixo:

1. Clone o repositório:
   ```bash
   git clone https://github.com/seu-usuario/jogo-lancamento-oblicuo.git
   cd jogo-lancamento-oblicuo
   ```

2. Compile o código-fonte:
   ```bash
   g++ -o lancamento_jogo lancamento_jogo.cpp -lGL -lGLU -lglut
   ```

3. Execute o jogo:
   ```bash
   ./lancamento_jogo
   ```

## Contribuições

Contribuições são bem-vindas! Sinta-se à vontade para abrir issues ou enviar pull requests para melhorar o jogo.

## Licença

Este projeto está licenciado sob a licença MIT. Consulte o arquivo [LICENSE](LICENSE) para mais detalhes.

---

Divirta-se jogando e explorando a física do lançamento oblíquo! 🚀
