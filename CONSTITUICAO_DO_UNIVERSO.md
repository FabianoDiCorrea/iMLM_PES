# CONSTITUIÇÃO DO UNIVERSO — iMLM PES (Master League Manager)

> *"O iMLM PES não é um jogo de futebol. Também não é apenas um jogo de gerenciamento. É um ecossistema de carreira onde a inteligência administrativa vive no iMLM e a emoção das quatro linhas ganha vida no PES."*

---

## 1. O QUE É O iMLM PES

O **iMLM PES (Master League Manager)** é um simulador completo de carreira de treinador.

- Ele **NÃO** é um editor do PES.
- Ele **NÃO** é uma modificação da Master League.
- Ele **NÃO** pretende copiar qualquer jogo existente.

Ele é um projeto original que une dois conceitos:
1. Um simulador profundo de gerenciamento de carreira;
2. A possibilidade de disputar partidas reais utilizando o **PES 2021**.

O objetivo é oferecer ao jogador o melhor dos dois mundos: **Gerenciar. E jogar.**

---

## 2. A GRANDE IDEIA

Imagine um universo de futebol totalmente vivo. Nesse universo existem:
- Clubes, Jogadores, Treinadores, Seleções, Competições;
- Mercado de Transferências, Finanças, Patrocinadores, Categorias de Base;
- Aposentadorias, Novos Talentos, Notícias, Rankings, Recordes, Premiações e História Permanente.

Tudo isso acontece dentro do **iMLM PES**.

Quando chega a hora de jogar futebol:
1. O usuário abre o PES 2021;
2. Disputa a competição desejada (Modo Humano);
3. Fecha o PES 2021;
4. Importa os resultados para o iMLM;
5. O universo continua evoluindo normalmente.

**O PES nunca administra a carreira. O iMLM nunca joga futebol.** Cada sistema possui responsabilidades bem definidas.

---

## 3. SEPARAÇÃO DE RESPONSABILIDADES

### iMLM PES (Cérebro do Universo)
Controla: carreira do treinador, técnicos, clubes, jogadores, temporadas, competições, mercado, contratos, finanças, evolução, potencial, objetivos, diretoria, notícias, rankings, seleções, convocações, estatísticas, recordes, aposentadorias, categorias de base, regens e histórico imutável.

### PES 2021 (Motor das Partidas)
É exclusivamente o motor das partidas. Responsável apenas por: jogabilidade, física, animações, gráficos, inteligência artificial durante a partida e emoção do futebol. **Nada além disso.**

---

## 4. FONTE ÚNICA DA VERDADE

O estado oficial do universo pertence exclusivamente ao **iMLM PES**. 

O PES funciona apenas como um motor temporário para disputar competições em Modo Humano. Toda sincronização parte do iMLM para o PES. Após o término de uma competição, apenas seus resultados retornam ao iMLM. **O universo oficial nunca pertence ao PES.**

---

## 5. O UNIVERSO É PERSISTENTE

O universo nunca reinicia. Toda temporada deixa consequências:
- Clubes evoluem e regridem.
- Jogadores envelhecem.
- Treinadores mudam de clube.
- Ídolos permanecem na história.
- Novos talentos surgem.
- Recordes permanecem.
- O passado influencia permanentemente o futuro.

---

## 6. O PROTAGONISTA

O protagonista do iMLM **NÃO é um clube**. O **protagonista é o TREINADOR**.

Os clubes mudam, os jogadores envelhecem e as gerações passam, mas o **treinador constrói seu legado**. Seu histórico registra para sempre:
- Clubes e Seleções dirigidas;
- Títulos, Campanhas e Recordes;
- Prêmios, Estatísticas e Conquistas.

---

## 7. COMPETIÇÕES

Toda competição possui um modo de resolução:

### Modo Humano
O usuário disputa normalmente dentro do PES 2021. Ao término, importa os resultados para o iMLM.

### Modo Simulado
O iMLM resolve automaticamente a competição. O objetivo **NÃO é simular cada partida**, mas produzir um resultado coerente considerando: força do elenco, qualidade do treinador, reputação, momento e mando de campo.

O usuário escolhe quais competições deseja disputar manualmente. Todas as demais continuam evoluindo automaticamente dentro do universo.

---

## 8. FASES DA TEMPORADA

O universo não utiliza calendário diário. O tempo avança através de **Fases da Temporada**:

```text
[Pré-temporada] ──► [Mercado] ──► [Competições] ──► [Mercado Intermediário] ──► [Encerramento] ──► [Nova Temporada]
```

Cada fase movimenta todo o universo simultaneamente.

---

## 9. INTELIGÊNCIA DO UNIVERSO

Todos os clubes, treinadores e seleções tomam decisões de forma autônoma. O usuário não controla o universo — ele faz parte dele. Cada entidade possui seus próprios interesses, objetivos e estratégias. O mundo deve parecer vivo, dinâmico e imprevisível.

---

## 10. PILARES DO PROJETO

1. O protagonista é o treinador.
2. O universo pertence ao iMLM.
3. O PES existe apenas para disputar partidas.
4. O usuário escolhe apenas as competições que deseja jogar.
5. As demais competições continuam evoluindo automaticamente.
6. Toda temporada deixa consequências permanentes.
7. O histórico nunca é apagado.
8. Toda funcionalidade deve fortalecer a carreira do treinador.
9. Toda funcionalidade deve respeitar a separação entre gerenciamento e partidas.
10. Em caso de dúvida arquitetural, a solução deverá favorecer um universo persistente, coerente e vivo.

---

## 11. ROADMAP

### FASE 1 — Fundação ✅
- [x] Domínio do Sistema (`Jogador`, `Clube`, `Temporada`, `Partida`, `Competicao`, `EdicaoCompeticao`).
- [x] Nomenclatura oficial em Português (PT-BR).
- [x] `iPESMLManager` (`PesEditLib`): Leitura e gravação bit-a-bit completa.
- [x] Motor de Evolução de Atletas (Curvas por idade/posição + Potencial).
- [x] Motor de Mercado de Transferências.
- [x] Motor de Temporada (Máquina de Estados).

### FASE 2 — Universo do Futebol
- [ ] Sistema de Técnicos (Carreira do treinador).
- [ ] Sistema de Competições & Edições.
- [ ] Motor de Evolução do Universo & Simulação de Torneios.
- [ ] Sistema de Seleções, Convocações, Rankings, Notícias, Premiações e Hall da Fama.

### FASE 3 — Economia
- [ ] Contratos, Finanças, Patrocínios, Objetivos da Diretoria, Base, Regens e Aposentadorias.

### FASE 4 — Integração
- [ ] Exportação para o PES, Importação de Competições Humanas e Sincronização Completa.

### FASE 5 — Interface
- [ ] Dashboard, Perfil do Treinador, Perfil dos Clubes, Perfil dos Jogadores, Central de Notícias e Timeline.

---

## 12. PRINCÍPIO FUNDAMENTAL

O iMLM nunca deverá implementar uma funcionalidade apenas porque ela existe em outro jogo. Toda funcionalidade deverá existir porque melhora a experiência proposta pelo iMLM. A inspiração em outros jogos serve apenas como referência. **O objetivo é construir uma identidade própria.**

---

## 13. REGRA DE ARQUITETURA

Antes de implementar qualquer funcionalidade, responda:
- *Ela fortalece a carreira do treinador?*
- *Ela fortalece o universo do futebol?*
- *Ela respeita a separação entre iMLM e PES?*
- *Ela contribui para um universo persistente ao longo de décadas?*

**Se qualquer resposta for NÃO, a implementação deverá ser revista.**

---

## 14. MISSÃO

Criar o simulador de carreira de treinador que sempre sonhamos jogar. Um universo vivo, uma carreira praticamente infinita. Um sistema onde toda a inteligência administrativa pertence ao iMLM PES e toda a emoção das quatro linhas pertence ao PES. **Toda decisão de desenvolvimento deverá respeitar esta Constituição.**
