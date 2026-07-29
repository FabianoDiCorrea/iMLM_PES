# MOTOR DE MERCADO DE TRANSFERÊNCIAS (`MLM`)

**Documentação Técnica das Regras e Algoritmos de Negociação**

---

## 1. VISÃO GERAL DO MOTOR DE TRANSFERÊNCIAS

O **Motor de Transferências** (`MLM/Services/MotorTransferencias.h`) é o serviço desacoplado responsável por decidir se uma proposta de compra entre dois clubes é aceita ou recusada, avaliando separadamente a aprovação do **Clube Vendedor** e do **Jogador**.

O motor **não efetua alterações nos elencos**, apenas retorna a `RespostaTransferencia` detalhada.

---

## 2. ESTRUTURAS DE DADOS DA NEGOCIAÇÃO

- **`PropostaTransferencia`**: Contém `jogadorId`, `clubeVendedorId`, `clubeCompradorId`, `valorOferecido`, `salarioOferecido` e `anosContratoOferecidos`.
- **`RespostaTransferencia`**: Retorna `status` (`AceitaClubeEJogador`, `RecusadaPeloClube`, `RecusadaPeloJogador`, `SaldoInsuficienteComprador`), status booleano individual e justificativa textual (`motivoClube`, `motivoJogador`).

---

## 3. ALGORITMOS DE AVALIAÇÃO

### 1. Cálculo de Valor de Mercado Estimado
$$\text{ValorBase} = \left(\frac{\text{Overall}}{10}\right)^{3.8} \times 1000$$
- **Multiplicador de Idade:** $\le 21$ anos ($1.6\times$), $22-25$ anos ($1.3\times$), $26-29$ anos ($1.0\times$), $30-32$ anos ($0.7\times$), $33+$ anos ($0.3\times$).
- **Multiplicador de Potencial:** Adiciona $+5\%$ por cada ponto de diferencial entre Potencial e Overall.

### 2. Avaliação do Clube Vendedor
- Exige sobrepreço para **Ídolos** (Overall $\ge 85$: $+35\%$ sobre o valor de mercado).
- Exige sobrepreço para **Jovens Promessas** (Potencial $\ge 88$ e Idade $\le 21$: $+40\%$).
- Adiciona sobrepreço por **Reputação do Clube Vendedor** ($\ge 85$: $+15\%$).

### 3. Avaliação do Jogador
- Exige pretensão salarial de no mínimo $\sim 6\%$ a.a. do seu valor de mercado.
- **Clube com Reputação Inferior:** Se o clube comprador possuir reputação $> 15$ pontos abaixo do vendedor atual, o atleta exige no mínimo $+50\%$ de salário para aceitar o projeto esportivo.
