# MANIFESTO E ROADMAP OFICIAL DO PROJETO — iMLM PES (Master League Manager)

> *"O iMLM PES não é um jogo de futebol. Também não é apenas um jogo de gerenciamento. É um ecossistema de carreira onde a inteligência administrativa vive no iMLM e a emoção das quatro linhas ganha vida no PES."*

---

## 1. O QUE É O iMLM PES

O **iMLM PES (Master League Manager)** é um simulador completo de carreira de treinador.

- Ele **NÃO** é um editor do PES.
- Ele **NÃO** é uma modificação da Master League.
- Ele **NÃO** pretende copiar nenhum jogo existente.

Ele é um projeto original que une dois conceitos:
1. Um simulador profundo de gerenciamento de carreira;
2. A possibilidade de disputar partidas reais utilizando o **PES 2021**.

O objetivo é oferecer ao jogador o melhor dos dois mundos: **Gerenciar. E jogar.**

---

## 2. A GRANDE IDEIA

Imagine um universo de futebol totalmente vivo, contendo:
- Clubes, Jogadores, Técnicos, Seleções, Competições;
- Mercado de Transferências, Finanças, Patrocinadores, Categorias de Base;
- Aposentadorias, Regens, Notícias, Rankings, Recordes, Premiações e História Imutável.

Tudo isso acontece dentro do **iMLM PES**.

Quando chega a hora de jogar futebol:
1. O usuário abre o PES 2021;
2. Joga a competição desejada (Modo Humano);
3. Fecha o PES 2021;
4. Importa o resultado para o iMLM PES;
5. O universo continua evoluindo normalmente.

**O PES nunca administra a carreira. O iMLM nunca joga futebol.** Cada sistema possui sua responsabilidade estrita.

---

## 3. SEPARAÇÃO DE PAPÉIS DA ARQUITETURA

```text
┌──────────────────────────────────────────────────────────┐
│                    iMLM PES (Cérebro)                    │
│  - Carreira do Treinador (Protagonista) & Legado         │
│  - Contratos, Mercado de Transferências & Finanças       │
│  - Motor de Evolução e Potencial (Exclusivo MLM)         │
│  - Máquina de Estados & Fases da Temporada               │
│  - Domínio de Competições (Modo Humano e Simulado)       │
└───────────────────────────┬──────────────────────────────┘
                            │
                            │ Sincronização via iPESMLManager (EDIT00000000)
                            ▼
┌──────────────────────────────────────────────────────────┐
│                   PES 2021 (Motor de Partidas)           │
│  - Leitura/Escrita de Elencos e Atributos de Campo       │
│  - Física de Jogo, Animações e Gráficos                  │
│  - Inteligência Artificial e Emoção das Quatro Linhas    │
└──────────────────────────────────────────────────────────┘
```

---

## 4. PROTAGONISTA DO JOGO

O protagonista do jogo **NÃO é um clube**. O **protagonista é o TREINADOR**.

Os clubes mudam, os jogadores envelhecem e as gerações passam, mas o **treinador constrói seu legado**. Toda a carreira pertence ao treinador e seu histórico imutável registra:
- Clubes e Seleções dirigidas;
- Títulos, Prêmios e Campanhas;
- Recordes, Estatísticas e Conquistas ao longo de décadas.

---

## 5. OS 10 PILARES DO PROJETO

1. O protagonista é o treinador.
2. O universo pertence ao iMLM.
3. O PES existe apenas para jogar futebol.
4. O usuário escolhe quais competições deseja jogar.
5. As demais competições são resolvidas automaticamente.
6. Toda temporada deixa consequências permanentes.
7. O histórico nunca é apagado.
8. Toda funcionalidade deve fortalecer a carreira do treinador.
9. Toda funcionalidade deve respeitar a separação entre gerenciamento e partidas.
10. Sempre que existir dúvida de arquitetura, a solução deverá favorecer um universo de futebol vivo, persistente e coerente.

---

## 6. ROADMAP DE DESENVOLVIMENTO

### FASE 1 — Fundação (100% Concluída ✔)
- [x] **Domínio do Sistema:** `Jogador`, `Clube`, `Temporada`, `Partida`, `Competicao`, `EdicaoCompeticao`.
- [x] **Nomenclatura Oficial:** Integralmente em Português (PT-BR).
- [x] **iPESMLManager (`PesEditLib`):** Leitura, alteração e gravação bit-a-bit de atributos (`age`, `height`, `weight`, `skills`, `roster`).
- [x] **Motor de Evolução:** Curvas por idade e posição + Potencial exclusivo MLM.
- [x] **Motor de Mercado:** Valuation dinâmico, decisão do clube vendedor e aceite por reputação.
- [x] **Motor de Temporada:** Máquina de Estados e orquestração de fases da temporada.

### FASE 2 — Universo do Futebol (Próxima Fase ⬜)
- [ ] **Sistema de Técnicos:** Carreira do treinador, demissões, propostas, reputação e ascensão.
- [ ] **Motor de Resolução de Competições:** Simulação rápida e coerente.
- [ ] **Sistema de Seleções & Convocações:** Torneios internacionais.
- [ ] **Rankings Mundiais, Notícias, Premiações & Hall da Fama.**

### FASE 3 — Economia (⬜)
- [ ] Contratos, Finanças e Patrocínios.
- [ ] Objetivos da Diretoria, Categorias de Base, Regens e Aposentadorias.

### FASE 4 — Integração com o PES (⬜)
- [ ] Exportação para o `EDIT00000000`.
- [ ] Importação de Competições Humanas e Sincronização Total.

### FASE 5 — Interface Gráfica (UI/UX) (⬜)
- [ ] Dashboard, Perfil do Técnico, Perfil de Clubes e Jogadores.
- [ ] Central de Notícias e Timeline da Carreira.

---

## 7. A REGRA MAIS IMPORTANTE

Antes de implementar qualquer funcionalidade, responda às seguintes perguntas:
1. *Esta funcionalidade melhora a experiência de gerenciamento da carreira?*
2. *Ela fortalece o universo do futebol?*
3. *Ela respeita a separação entre iMLM e PES?*
4. *Ela contribui para que o treinador construa um legado ao longo de décadas?*

**Se qualquer resposta for NÃO, a implementação deverá ser repensada.**
