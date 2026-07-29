# MOTOR DE FLUXO DA TEMPORADA (`MLM`)

**Documentação Arquitetural do Ciclo Anual do Master League Manager**

---

## 1. VISÃO GERAL DO MOTOR DE TEMPORADA

O **Motor de Temporada** (`MLM/Services/MotorTemporada.h`) é o serviço orquestrador central do Master League Manager. 

Ele é responsável por gerenciar a **Máquina de Estados da Temporada** e invocar os demais serviços de domínio na ordem correta, sem implementar regras de negócio diretamente.

---

## 2. MÁQUINA DE ESTADOS DA TEMPORADA

O ciclo de vida de uma temporada é controlado estritamente pelas seguintes fases:

```text
[1. PreTemporada] ───► [2. JanelaTransferencias] ───► [3. EmCompeticao]
                                                              │
[5. Encerrada / NovaTemporada] ◄─── [4. PosTemporada] ◄───────┘
```

- **Validação de Fase:** Métodos como `ExecutarCompeticoes()` ou `ProcessarJanelaTransferencias()` retornam `false` se invocados fora da fase apropriada.

---

## 3. FLUXO SEQUENCIAL DE EXECUÇÃO ANUAL

### Phase 1: Pré-Temporada (`PreTemporada`)
1. Incrementa a idade de todos os jogadores ativos (`FazerAniversario()`).
2. Invoca o `MotorEvolucao` para calcular os ajustes de Overall e Habilidades baseados no desempenho da temporada anterior.
3. Abre a janela de transferências inicial.

### Phase 2: Janela de Transferências (`JanelaTransferencias`)
1. Recebe a lista de `PropostaTransferencia`.
2. Invoca o `MotorTransferencias` para avaliar o aceite do clube vendedor e do atleta.

### Phase 3: Em Competição (`EmCompeticao`)
- Suporta dois modos de resolução (`ModoResolucaoCompeticao`):
  - **`ModoHumano`:** Aguarda a partida ser jogada no PES 2021 e seus dados/estatísticas importados.
  - **`ModoSimulado`:** Dispara a resolução automática por algoritmos MLM.

### Phase 4: Pós-Temporada (`PosTemporada`)
- Concede premiações, atualiza rankings continentais, finanças e determina promovidos e rebaixados.

### Phase 5: Encerramento & Virada (`Encerrada`)
- Fecha a temporada atual e invoca `m_temporada.CriarProximaTemporada()`, gerando a instância do ano subsequente.
