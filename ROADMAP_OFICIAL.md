# ROADMAP OFICIAL DO PROJETO — iMLM PES (Master League Manager)

**Documento de Filosofia, Arquitetura e Direcionamento de Longo Prazo**

---

## 1. VISÃO E FILOSOFIA DO PROJETO

O **iMLM PES** (Master League Manager) **NÃO é um editor de PES** nem uma modificação da Master League original. 

Seu objetivo é criar um **novo e completo simulador de carreira de treinador** inspirado nos maiores jogos de gestão do futebol mundial, utilizando o **PES 2021 exclusivamente como motor de partidas**.

### A Filosofia "Fora do Jogo / Dentro de Campo"
- **Fora do Jogo (iMLM PES):** Toda a administração de carreira, transferências, finanças, evolução de atletas, envelhecimento, movimentação de técnicos, contratações, convocações e históricos são calculados e gerenciados 100% no iMLM PES.
- **Dentro de Campo (PES 2021):** O PES 2021 é utilizado apenas quando o usuário decide disputar uma competição no modo humano. Ao terminar a competição, seus resultados são importados de volta para o iMLM PES.

---

## 2. SEPARAÇÃO DE PAPÉIS DA ARQUITETURA

```text
┌─────────────────────────────────────────────────────────┐
│                    iMLM PES (Cérebro)                   │
│  - Domínio de Carreiras, Técnicos e Contratos           │
│  - Motor de Evolução e Potencial (Exclusivo MLM)        │
│  - Motor de Mercado de Transferências                   │
│  - Orquestrador da Temporada & Fases da Carreira        │
│  - Sistema de Competições (Modo Humano e Simulado)      │
└──────────────────────────┬──────────────────────────────┘
                           │
                           │ Sincronização via iPESMLManager (Binary I/O)
                           ▼
┌─────────────────────────────────────────────────────────┐
│                   PES 2021 (Motor de Partidas)          │
│  - EDIT00000000 (Elencos, Idades, Atributos de Campo)   │
│  - Física de Jogo, Gráficos e Inteligência Artificial   │
└─────────────────────────────────────────────────────────┘
```

---

## 3. ROADMAP GERAL E STATUS DE DESENVOLVIMENTO

### FASE 1 — Fundação (100% Concluída ✔)
- [x] Domínio do Sistema (`Jogador`, `Clube`, `Temporada`, `Partida`, `Competicao`, `EdicaoCompeticao`).
- [x] Nomenclatura oficial integralmente em Português (PT-BR).
- [x] `iPESMLManager` (`PesEditLib`): Leitura, alteração e gravação bit-a-bit de atributos (`age`, `height`, `weight`, `skills`, `roster`).
- [x] Motor de Evolução de Atletas (Curvas por idade e posição + Potencial exclusivo MLM).
- [x] Motor de Mercado de Transferências (Valuation dinâmico, decisão do clube vendedor e do jogador por reputação).
- [x] Motor de Temporada (Máquina de Estados de Fases da Temporada desacoplada de relógio real).

---

### FASE 2 — Universo do Futebol (Próxima Fase ⬜)
- [ ] **Sistema de Técnicos (Treinador protagonizado pelo jogador):** Demissões, propostas, reputação e ascensão.
- [ ] **Motor de Resolução de Competições:** Simulação rápida e coerente para ligas e copas secundárias.
- [ ] **Sistema de Seleções & Convocações:** Torneios internacionais (Copa do Mundo, Eurocopa, Copa América).
- [ ] **Rankings Mundiais, Premiações & Hall da Fama.**

---

### FASE 3 — Economia & Categorias de Base (⬜)
- [ ] **Gestão Contratual & Salarial Avançada.**
- [ ] **Finanças do Clube & Patrocínios.**
- [ ] **Categorias de Base, Aposentadorias e Sistema de Regens.**

---

### FASE 4 — Integração & Sincronização Total (⬜)
- [ ] Importação automatizada de resultados de competições jogadas no PES 2021.
- [ ] Exportação e sincronização completa do estado do MLM para o arquivo `EDIT00000000`.

---

### FASE 5 — Interface Gráfica (UI/UX) (⬜)
- [ ] Dashboard principal da carreira.
- [ ] Central de Notícias, Perfil do Técnico, Perfil dos Clubes e Jogadores.
- [ ] Visualização de Estatísticas e Histórico Imutável.

---

## 4. MISSÃO FINAL

Construir uma experiência de carreira infinita, rica e viva, onde o usuário desfruta do melhor dos dois mundos: **a profundidade de gerenciamento no iMLM PES e a emoção do futebol real no PES 2021.**
