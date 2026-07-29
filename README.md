# README — iMLM PES (Master League Manager)

> *"O iMLM PES não é um jogo de futebol. Também não é apenas um jogo de gerenciamento. É um ecossistema de carreira onde a inteligência administrativa vive no iMLM e a emoção das quatro linhas ganha vida no PES."*

---

## ⚽ Sobre o Projeto

O **iMLM PES (Master League Manager)** é um simulador completo de carreira de treinador de futebol. Ele une a profundidade de um gerenciador de carreiras com a possibilidade de disputar partidas reais dentro do **PES 2021**.

### 🏛️ Separação de Responsabilidades
- **iMLM PES (Cérebro):** Gerencia 100% da inteligência da carreira (contratos, finanças, evolução, potencial, mercado de transferências, seleções e históricos imutáveis).
- **PES 2021 (Motor de Partidas):** Fornece exclusivamente a experiência e emoção das partidas jogadas pelo usuário (Modo Humano).
- **iPESMLManager (`PesEditLib`):** Biblioteca de I/O em C++ para leitura, alteração e sincronização bit-a-bit do arquivo `EDIT00000000`.

---

## 🛠️ Arquitetura do Sistema

```text
MLM/
├── Domain/              -> Entidades (Jogador, Clube, Temporada, Partida, Competicao, EdicaoCompeticao)
├── Services/            -> Motores (MotorEvolucao, MotorTransferencias, MotorTemporada)
└── Tests/               -> Suítes de testes unitários automatizados em C++17

PesEditLib/              -> Biblioteca I/O binária para EDIT00000000 do PES 2021
```

---

## 🚀 Status da Versão `v0.0.1` (Fase 1 — Fundação Concluída)

- [x] Domínio do MLM implementado 100% em Português (PT-BR).
- [x] `PesEditLib` com capacidade de leitura e gravação bit-a-bit de atributos (`age`, `height`, `weight`, `skills`, `roster`).
- [x] Motor de Evolução de Atletas (Curvas por idade e posição + Potencial exclusivo MLM).
- [x] Motor de Mercado de Transferências (Valuation dinâmico e reputação).
- [x] Motor de Temporada (Máquina de Estados de fases anuais).
- [x] Documentação e Constituição do Universo (`CONSTITUICAO_DO_UNIVERSO.md`).

---

## 📜 Licença

Projeto desenvolvido por **DevDuo / Fabiano Di Correa**. Todos os direitos reservados.
