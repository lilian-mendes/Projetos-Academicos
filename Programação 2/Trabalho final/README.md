# **Sailor Mars**

## **Autor**
- **Nome:** Lilian Miranda Mendes 
- **GRR:** 20222549

---

## **Descrição**
**Sailor Mars** é um jogo de plataforma 2D desenvolvido em **C** utilizando a biblioteca **Allegro 5**. O jogo apresenta uma campanha com progressão de fases, inimigos com comportamentos distintos e chefes, cada um com mecânicas únicas. O jogador controla a personagem principal através de níveis.

### **Principais Características**
✅ **Sistema de Estados** (Menu, Jogo, Game Over, Vitória)  
✅ **Mecânicas de Jogabilidade** (Movimento, Pulo, Agachamento, Tiros)  
✅ **Sistema de Projéteis** (Tiros normais que consomem stamina e tiros especiais com cooldow)  
✅ **Inimigos e Chefes** (Com comportamentos distintos por fase)  
✅ **Progressão e Sistemas de Save** (Modo Campanha com salvamento automático e Fase Única)  
✅ **Interface de usuario**  (Barras de Vida e Stamina, ícones e telas de feedback)
✅ **Sistema de itens**   (Coletáveis de cura que aparecem periodicamente)

---

## **Estrutura do Projeto**

### **Arquivos Principais**
| Arquivo                     | Descrição                                                                   |
|-----------------------------|-----------------------------------------------------------------------------|
| `main.c`                    | Ponto de entrada do jogo, gerencia os estados e transições.                 |
| `Game.c` / `Game.h`         | Contém a lógica principal do jogo, incluindo inicialização e loop.          |
| `Square.c` / `Square.h`     | Define o jogador, movimentação, estados e mecânicas de tiro.                |
| `Enemy.c` / `Enemy.h`       | Implementa inimigos básicos com IA de perseguição e ataque.                 |
| `Boss.c` / `Boss.h`         | Chefes com comportamentos distintos (Fase 1: Movimento, Fase 2: Teleporte). |
| `Pistol.c` / `Pistol.h`     | Sistema de armas e projéteis.                                               |
| `Bullet.c` / `Bullet.h`     | Lógica de colisão e movimentação de projéteis.                              |
| `Joystick.c` / `Joystick.h` | Controles do jogador (teclado).                                             |
| `Sprite.c` / `Sprite.h`     | Carregamento e renderização de sprites.                                     |
| `Makefile`                  | Compilação do projeto.                                                      |

---

## **Mecânicas do Jogo**

### **1. Controles do Jogador**
| Tecla              | Ação                                  |
|--------------------|---------------------------------------|
| **← / → /A / D**   | Movimentação horizontal               |
| **↑ / W**          | Pular                                 |
| **↓ / S**          | Agachar                               |
| **Espaço / C**     | Atirar (gasta stamina)                |
| **F**              | Tiro Especial (cooldown)              |
| **P**              | Pausar                                |
| **Esq**            | Sair/ Voltar                          |

### **2. Sistema de Vida e Stamina**
- **Vida (HP):**  
  - Representada por uma barra vermelha no canto superior.  
  - O jogador começa com 10 HP.
  - Perdida ao ser atingido por projéteis inimigos.  
  - Pode ser recuperada com itens de cura, que restaura 3 HP.

- **Stamina:**  
  - Necessária para atirar.  
  - Regenera automaticamente ao longo do tempo.  
  - **Tiro Normal** consome 1 unidade.  
  - **Tiro Especial** consome 5 unidades.  

### **3. Inimigos e Chefes**
- **Inimigos Comuns:**  
  - Seguem o jogador.  
  - Atiram quando próximos.  
  - Possuem 5 HP.

- **Chefes:**  
  - **Fase 1:** 
    - Persegue o jogador se ele estiver fora do alcance de tiro. 
    - Para e atira em intervalos regulares quando o jogador está próximo.
    - Possui 15 HP.
    
  - **Fase 2:** 
    - Utiliza uma mecânica de teleporte: desaparece e reaparece em uma posição aleatória próxima ao jogador.
    - Permanece visível por um curto período (TELEPORT_COOLDOWN de 3s) para atacar.
    - Possui 25 HP.

### **4. Progressão**
- **Modo Campanha:**  
  - Derrote todos os inimigos para enfrentar o chefe.  
  - Avance para a Fase 2 após vencer o chefe da Fase 1.  

- **Modo Fase Única:**  
  - Selecione a fase desejada no menu.  

---

## **Fluxo do Jogo**

### **1. Menu Principal**
- **Opções:**  
  - **Jogar (Campanha)** → Inicia na Fase 1.  

  **Continuar** → Carrega o progresso a partir da última fase salva.

  - **Selecionar Fase** → Escolha entre Fase 1 ou 2. 

  - **Sair** → Encerra o jogo.  

### **2. Durante o Jogo**
1. **Derrote Inimigos** para avançar.  
2. **Enfrente o Chefe** ao eliminar todos os inimigos.  
3. **Colete Itens de Cura** para recuperar HP.  

### **3. Estados de Fim de Jogo**
- **Game Over:**  
  - Reinicie a fase ou volte ao menu.  

- **Vitória:**  
  - No modo campanha, o jogo avança para a próxima fase ou para a tela de vitória final.
  - No modo fase única, o jogador pode reiniciar a fase ou voltar ao menu.

---

## **Compilação e Execução**

### **Dependências**
- **Allegro 5** (Bibliotecas: `allegro`, `allegro_image`, `allegro_font`, `allegro_ttf`, `allegro_primitives`)  
- **GCC** (Compilador C)  

### **Compilação**
```bash
make
```

### **Execução**
```bash
./sailor_mars
```

### **Limpar Arquivos de Compilação**
```bash
make clean
```

---

## **Decisões de Projeto**

### **1. Sistema de Estados**
- **Por quê?**  
  - Facilita a transição entre menus, jogo e telas de fim.  
  - Evita código repetitivo.  

### **2. Comportamento dos Chefes por Fase**
- **Fase 1 (Movimento):**  
  - Mais simples, ideal para introdução.  

- **Fase 2 (Teleporte):**  
  - Aumenta a dificuldade e varia a jogabilidade.  