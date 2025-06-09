# Game Design Document: Alien Invasion (Estilo Space Invaders)

## 1. Descrição do Jogo

Alien Invasion é um jogo do tipo Shoot 'Em Up clássico inspirado no lendário Space Invaders de 1978. O jogador controla uma nave defensora na parte inferior do ecrã, movendo-se horizontalmente e, um pouco diferente do jogo original, a nave tem mais liberdade de movimentação, podendo até mesmo se posicionar atrás das naves inimigas, para tal a nave devera ser possível fazer rotações de 90 graus. O objetivo é eliminar ondas de invasores alienígenas organizados em formação de grade antes que alcancem o solo ou destruam a nave do jogador.

### 1.1. Conceito e Inspirações

O jogo segue fielmente a fórmula clássica do Space Invaders original, mantendo a simplicidade e elegância que tornaram o jogo um ícone dos arcades. A experiência foca na tensão crescente conforme os inimigos aceleram e se aproximam, combinada com a estratégia de usar escudos destrutíveis para proteção.

**[ESPAÇO PARA FIGURA 1: Screenshot do layout clássico do jogo com formação de inimigos]**

### 1.2. Público-Alvo

- Faixa etária: 8+ anos
- Jogadores nostálgicos que apreciam clássicos dos arcades
- Novos jogadores interessados em experimentar um marco da história dos videogames
- Entusiastas de jogos simples mas desafiadores

### 1.3. Plataformas

Desenvolvido inicialmente para os sistemas operativos Windows, Linux e MacOS.

## 2. Elementos do Jogo

### 2.1. Nave do Jogador

A nave defensora possui as seguintes características:

- **Movimento**: horizontal (esquerda/direita)/vertical(cima/baixo)
- **Controles**: teclas A/D e W/S para movimento. e Q/E para rotação
- **Disparo**: Barra de espaço para atirar
- **Limitação de Tiro**: Apenas um projétil a cada segundo e meio
- **Vidas**: 3 vidas iniciais, representadas por pequenas naves no HUD
- **Velocidade**: Movimento responsivo mas não instantâneo

**[ESPAÇO PARA FIGURA 2: Sprite da nave do jogador e área de movimento]**

#### 2.1.1. Design da Nave

A nave do jogador segue o design clássico minimalista:
- Formato triangular ou retangular simples
- Cor verde para boa visibilidade contra o fundo escuro
- Tamanho pequeno para permitir manobras precisas entre projéteis inimigos
- Animação simples de explosão quando destruída

### 2.2. Invasores Alienígenas

Os inimigos são organizados em uma formação de grade fixa com três tipos distintos:

#### 2.2.1. Formação Inicial
- **Grade**: 8 colunas × 5 fileiras (40 inimigos total)
- **Fileira Superior (2 fileiras)**: Aliens pequenos - 10 pontos cada
- **Fileira Média (2 fileiras)**: Aliens médios - 20 pontos cada  
- **Fileira Inferior (1 fileira)**: Aliens grandes - 30 pontos cada

**[ESPAÇO PARA FIGURA 3: Formação completa dos invasores alienígenas]**

#### 2.2.2. Padrão de Movimento

Os invasores seguem o padrão clássico de movimento:

1. **Movimento Horizontal**: Toda a formação move-se lateralmente em sincronia
2. **Mudança de Direção**: Ao atingir a borda da tela, toda a formação desce uma linha e inverte a direção
3. **Aceleração**: Conforme inimigos são eliminados, os restantes aceleram
4. **Tiro Aleatório**: Inimigos da linha de frente disparam aleatoriamente

#### 2.2.3. Tipos de Alienígenas

**Tipo 1 - Explorador (Fileiras Superiores)**
- Menor valor em pontos (10)
- Sprite simples e pequeno
- Movimento padrão da formação

**Tipo 2 - Soldado (Fileiras Médias)**  
- Valor médio em pontos (20)
- Sprite ligeiramente maior
- Movimento padrão da formação

**Tipo 3 - Comandante (Fileira Inferior)**
- Maior valor em pontos (30)
- Sprite mais detalhado e maior
- Movimento padrão da formação
- Maior probabilidade de disparar


### 2.4. Sistema de Projéteis

#### 2.4.1. Projéteis do Jogador
- **Limitação**: Apenas um projétil a cada segundo e meio
- **Velocidade**: Rápida, viagem deve ser coincidente com a orientação da nave do jogador
- **Aparência**: Linha vertical simples ou pequeno retângulo
- **Comportamento**: Desaparece ao atingir inimigo ou topo da tela

#### 2.4.2. Projéteis dos Inimigos
- **Frequência**: Disparo aleatório dos inimigos da linha de frente
- **Velocidade**: Mais lenta que os projéteis do jogador
- **Aparência**: Linha vertical ou formato distinto dos projéteis do jogador
- **Comportamento**: eliminam o jogador ao contato

**[ESPAÇO PARA FIGURA 5: Comparação visual entre projéteis do jogador e inimigos]**

### 2.5. Interface de Usuário (HUD)

#### 2.5.1. Elementos do HUD
- **Pontuação**: Canto superior esquerdo, fonte monoespaçada
- **Vidas Restantes**: Canto inferior esquerdo, representadas por sprites de naves pequenas
- **Hi-Score**: Melhor pontuação registrada

**[ESPAÇO PARA FIGURA 6: Layout completo da interface do usuário]**

## 3. Gameplay

### 3.1. Objetivo Principal

Eliminar todos os invasores alienígenas da formação antes que:
- Alcancem a parte inferior da tela
- Destruam a nave do jogador com seus projéteis

### 3.2. Controles

| Ação | Tecla Principal | Tecla Alternativa |
|------|----------------|-------------------|
| Mover Esquerda | ← | A |
| Mover Direita | → | D |
| Mover Cima | ↑ | W |
| Mover Baixo | ↓ | S |
| Rotação Esquerda | Q | - |
| Rotação Direita | E | - |
| Disparar | Espaço | - |
| Pausar | P | ESC |
| Menu | Enter | - |

### 3.3. Mecânicas de Jogo


#### 3.3.2. Sistema de Vidas
- **Vidas Iniciais**: 3 vidas
- **Perda de Vida**: Contato com projétil inimigo ou invasor
- **Vida Extra**: A cada 5.000 pontos (configurável)
- **Game Over**: Quando todas as vidas são perdidas

#### 3.3.3. Condições de Vitória/Derrota

**Vitória da Onda**:
- Todos os invasores eliminados
- Progressão para próxima onda com nova formação

**Derrota**:
- Invasores alcançam a linha de defesa (parte inferior)
- Jogador perde todas as vidas

### 3.4. Estados do Jogo (FSM)

- **MENU**: Tela inicial com opções
- **JOGANDO**: Gameplay ativo
- **PAUSA**: Jogo pausado, estado preservado
- **GAME_OVER**: Fim de jogo, exibição de pontuação
- **ENTRE_ONDAS**: Transição entre ondas com breve pausa

**[ESPAÇO PARA FIGURA 7: Diagrama de estados do jogo]**

## 4. Sistema de Pontuação

### 4.1. Valores Base

| Inimigo | Pontos |
|---------|--------|
| Explorador (pequeno) | 10 |
| Soldado (médio) | 20 |
| Comandante (grande) | 30 |


**[ESPAÇO PARA FIGURA 8: Tabela visual do sistema de pontuação]**

## 5. Progressão de Dificuldade

### 5.1. Escalada de Dificuldade


#### 5.1.2. Frequência de Tiro
- **Onda 1**: 1 tiro a cada 3-5 segundos. Redução gradual do intervalo



**[ESPAÇO PARA FIGURA 9: Gráfico de progressão de dificuldade]**

## 6. Arte e Estilo Visual

### 6.1. Estética Geral

- **Paleta de Cores**: Limitada, com foco em verde, branco, azul e vermelho sobre fundo preto
- **Resolução**: Baixa resolução simulada para autenticidade
- **Efeitos**: Mínimos, focando na clareza visual

### 6.2. Sprites e Animações


#### 6.2.2. Efeitos Visuais

- **Projéteis**: Linhas simples ou pequenos retângulos


**[ESPAÇO PARA FIGURA 10: Folha de sprites completa do jogo]**


## 8. Implementação Técnica

### 8.1. Arquitetura do Jogo

#### 8.1.1. Estrutura de Classes Principais
- **Game**: Classe principal que gerencia estados
- **Player**: Nave do jogador e controles
- **InvaderFormation**: Gerencia a grade de invasores
- **Invader**: Classe individual de cada invasor
- **Projectile**: Projéteis do jogador e inimigos
- **GameState**: Gerenciamento de estados do jogo

#### 8.1.2. Sistemas Principais
- **Input System**: Captura e processamento de entrada
- **Collision System**: Detecção de colisões
- **Render System**: Renderização de sprites
- **Audio System**: Reprodução de sons e música
- **Score System**: Gerenciamento de pontuação

### 8.2. Requisitos Técnicos

#### 8.2.1. Requisitos Mínimos
- **SO**: Windows 10/11, Linux e MacOS


#### 8.2.2. Ferramentas de Desenvolvimento
- **Linguagem**: C++
- **Controle de Versão**: Git

**[ESPAÇO PARA FIGURA 12: Diagrama da arquitetura técnica]**

## 9. Cronograma de Desenvolvimento

### 9.1. Fases de Desenvolvimento

#### Fase 1: Protótipo Básico (Semana 1-2)
- Movimento da nave do jogador
- Tiro básico
- Invasor simples
- Colisões básicas

#### Fase 2: Mecânicas Core (Semana 3-4)
- Formação completa de invasores
- Padrão de movimento clássico
- Sistema de escudos
- Tiros dos inimigos

#### Fase 3: Sistemas de Jogo (Semana 5-6)
- Sistema de pontuação
- Múltiplas ondas
- Progressão de dificuldade
- Estados do jogo

#### Fase 4: Polish e Finalização (Semana 7-8)
- Arte final
- Efeitos sonoros
- Balanceamento
- Testes e correções

**[ESPAÇO PARA FIGURA 13: Cronograma visual de desenvolvimento]**

## 10. Expansões Futuras

### 10.1. Melhorias Possíveis

- **Modo Dois Jogadores**: Cooperativo ou alternado
- **Invasores Especiais**: Tipos com comportamentos únicos
- **Power-ups**: Melhorias temporárias para a nave
- **Níveis Temáticos**: Diferentes ambientações espaciais
- **Modo Sobrevivência**: Ondas infinitas com dificuldade crescente

### 10.2. Versões de Plataforma


**[ESPAÇO PARA FIGURA 14: Mockups de expansões futuras]**

---

*Este documento serve como guia completo para o desenvolvimento de Alien Invasion, inspirado no clássico Space Invaders enquanto oferece uma experiência polida e moderna.*

