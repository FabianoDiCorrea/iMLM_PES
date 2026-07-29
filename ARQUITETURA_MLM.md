# ARQUITETURA DO MASTER LEAGUE MANAGER (MLM)
**Documentação Definitiva de Domínio & Modelagem de Dados**

---

## 1. VISÃO GERAL DA ARQUITETURA DO SISTEMA

O **Master League Manager (MLM)** é um sistema autônomo de gerenciamento de modo carreira que utiliza o PES 2021 estritamente como motor de simulação gráfica/de partidas. O arquivo binário `EDIT00000000` atua apenas como uma camada de **Exportação e Importação de Estado (I/O)**.

```
+-------------------------------------------------------------------+
|                   MASTER LEAGUE MANAGER (MLM)                     |
|           (Regras de Negócio, Histórico, Finanças, IA)            |
+-------------------------------------------------------------------+
                                 │
                                 ▼
+-------------------------------------------------------------------+
|                            PesEditLib                             |
|       (API de Leitura, Gravação e Validação Binária do EDIT)      |
+-------------------------------------------------------------------+
                                 │
                                 ▼
+-------------------------------------------------------------------+
|                           EDIT00000000                            |
|             (Binário do PES 2021 - Elencos & Atributos)           |
+-------------------------------------------------------------------+
                                 │
                                 ▼
+-------------------------------------------------------------------+
|                             PES 2021                              |
|                    (Motor Gráfico de Partida)                     |
+-------------------------------------------------------------------+
```

---

## 2. MATRIZ DE SEPARAÇÃO DE RESPONSABILIDADES (PES vs MLM)

| Informação / Atributo | Responsável Principal | Descrição / Raciocínio |
| :--- | :--- | :--- |
| **Atributos Técnicos, Físicos e Táticos** | **PES** | Leitura/Escrita via `PesEditLib`. Definem a performance no motor de jogo. |
| **ID de Aparência, Face e Cabelo** | **PES** | Estritamente binário. Gerenciado pelo PES. |
| **Overall Rating (Habilidade Geral)** | **PES** | Calculado pelas fórmulas de atributos binários do jogo. |
| **Idade / Data de Nascimento (Original)**| **PES / MLM** | Lido do PES, mas sincronizado e incrementado pelas Temporadas do MLM. |
| **Potencial e Curva de Desenvolvimento** | **MLM** | O MLM determina a evolução e aplica reajustes de atributos no PES via `PesEditLib`. |
| **Valor de Mercado & Salário** | **MLM** | Calculado dinamicamente por algoritmos econômicos do MLM. |
| **Contratos & Cláusulas de Rescisão** | **MLM** | Tempo de contrato, multas e exigências salariais pertencem ao MLM. |
| **Finanças do Clube (Orçamento/Folha)** | **MLM** | Receita de bilheteria, patrocinadores, prêmios e despesas operacionais. |
| **Histórico de Carreiras & Prêmios** | **MLM** | Acumulado de gols, assistências, títulos, Bolas de Ouro e estatísticas. |
| **Mercado de Transferências & IA** | **MLM** | Negociações, propostas entre clubes de IA e decisões de carreira de jogadores. |
| **Base de Jovens (Youth Academy)** | **MLM** | Geração e maturação de regens e jovens promessas antes de subirem ao PES. |
| **Calendário, Lesões & Suspensões** | **MLM** | Controle de fadiga de longa duração, tempo de departamento médico e cartões acumulados. |

---

## 3. MODELAGEM COMPLETA DAS ENTIDADES DE DOMÍNIO (20 ENTIDADES)

---

### 1. `Player` (Jogador)
* **Responsabilidade:** Representar a vida esportiva, física, econômica e o progresso de um atleta do início ao fim de sua carreira.
* **Atributos:**
  * `id` (int) - Identificador único do jogador (sincronizado com PES ID).
  * `name` / `shirtName` (string) - Nome completo e nome de camisa.
  * `birthDate` / `age` (Date/int) - Idade e data de nascimento no universo do MLM.
  * `nationalityId` (int) - Referência à entidade `Nation`.
  * `overall` (int) - Rating atual (lido/calculado do PES).
  * `potential` (int) - Rating máximo que o atleta pode atingir.
  * `currentTeamId` (int) - Referência à entidade `Team` (ou null se Agente Livre).
  * `shirtNumber` (int) - Número da camisa no clube atual.
  * `primaryPosition` / `secondaryPositions` - Posição registrada e posições jogáveis.
  * `dominantFoot` (enum) - Pé preferencial (Direito / Esquerdo).
  * `marketValue` (double) - Valor estimado de mercado no MLM.
  * `moral` (float) - Nível de satisfação (0.0 a 100.0).
  * `formState` (float) - Condição física / ritmo de jogo.
* **Relacionamentos:**
  * Pertence a `Team` (ou Free Agent).
  * Possui 1 `Contract`.
  * Possui N registros de `Injury`, `Suspension`, `Award` e `History`.
  * Participa de N `Match`.

---

### 2. `Team` (Clube / Seleção)
* **Responsabilidade:** Representar uma entidade esportiva, seu elenco ativo, estrutura financeira e diretoria técnica.
* **Atributos:**
  * `id` (int) - Identificador único do clube (PES Team ID).
  * `name` / `shortName` (string) - Nome e sigla do clube.
  * `leagueId` (int) - Liga atual que disputa.
  * `stadiumId` (int) - Referência à entidade `Stadium`.
  * `reputation` (int) - Nível de prestígio no mercado mundial (1 a 5 estrelas).
  * `transferBudget` (double) - Saldo disponível para contratações.
  * `wageBudget` (double) - Saldo mensal/anual disponível para folha salarial.
  * `managerId` (int) - Referência à entidade `Manager`.
  * `tacticStyle` (enum) - Estilo tático predominante (ex: Posse de Bola, Contra-ataque).
* **Relacionamentos:**
  * Contém até 40 `Player` (Elenco ativo).
  * Disputa 1 `League` e N `Cup`.
  * Possui 1 `Manager`.
  * Possui N `YouthPlayer` na sua categoria de base.
  * Registra N `Transfer` (compras e vendas).
  * Possui 1 `Finance` (balanço de contas).

---

### 3. `League` (Liga Divisão / Campeonato de Pontos Corridos)
* **Responsabilidade:** Estruturar o campeonato de pontos corridos com tabela de classificação, vagas continentais e rebaixamento.
* **Atributos:**
  * `id` (int) - Identificador da liga.
  * `name` / `countryId` (string/int) - Nome da liga e país correspondente.
  * `tier` (int) - Nível da divisão (1ª Divisão, 2ª Divisão, etc.).
  * `maxTeams` (int) - Quantidade de participantes.
  * `promotionSlots` / `relegationSlots` (int) - Vagas de acesso e rebaixamento.
  * `continentalSlots` (int) - Vagas para torneios continentais.
* **Relacionamentos:**
  * Pertence a 1 `Competition`.
  * Contém N `Team`.
  * Gera N `Match` através do `Calendar`.

---

### 4. `Cup` (Copa / Torneio de Mata-Mata)
* **Responsabilidade:** Gerenciar competições de eliminação direta (nacionais ou continentais) com fases de grupos ou chaves.
* **Atributos:**
  * `id` (int) - Identificador da copa.
  * `name` (string) - Nome do torneio.
  * `type` (enum) - Nacional, Continental ou Recopa.
  * `format` (enum) - Mata-Mata Simples, Ida e Volta, Fase de Grupos + Mata-Mata.
  * `prizeMoney` (double) - Premiação financeira por fase e para o campeão.
* **Relacionamentos:**
  * Pertence a 1 `Competition`.
  * Associa N `Team` participantes.

---

### 5. `Competition` (Competição Genérica / Guarda-Chuva)
* **Responsabilidade:** Entidade abstrata que padroniza o histórico, regras de premiação e estatísticas de qualquer torneio (`League` ou `Cup`).
* **Atributos:**
  * `id` (int) - Identificador global da competição.
  * `name` (string) - Nome oficial.
  * `reputationWeight` (float) - Peso da competição para cálculo de prestígio de prêmios.
* **Relacionamentos:**
  * Agrupa instâncias de `League` e `Cup`.
  * Registra N `History` de campeões ao longo das temporadas.

---

### 6. `Season` (Temporada)
* **Responsabilidade:** Controlar a linha temporal do universo MLM, marcando o início, fim, janelas de transferências e transições anuais.
* **Atributos:**
  * `year` (int) - Ano de referência (ex: 2026).
  * `startDate` / `endDate` (Date) - Início e encerramento da temporada.
  * `transferWindowOpen` (bool) - Estado atual da janela de transferências.
  * `currentDay` (int) - Dia atual dentro do calendário da temporada.
* **Relacionamentos:**
  * Contém 1 `Calendar`.
  * Conecta N `Match`, N `Transfer` e N `Award`.

---

### 7. `Contract` (Contrato de Trabalho)
* **Responsabilidade:** Reger a relação financeira e de vínculo entre um `Player` ou `Manager` e um `Team`.
* **Atributos:**
  * `id` (int) - Identificador do contrato.
  * `salary` (double) - Salário periódico (semanal/mensal).
  * `releaseClause` (double) - Multa rescisória para transferência automática.
  * `startDate` / `expirationDate` (Date) - Período de vigência.
  * `appearanceBonus` (double) - Bônus por partida jogada.
  * `goalBonus` (double) - Bônus por gol marcado.
* **Relacionamentos:**
  * Pertence a 1 `Player` ou 1 `Manager`.
  * Vinculado a 1 `Team`.

---

### 8. `Transfer` (Transação de Transferência)
* **Responsabilidade:** Registrar o histórico e o estado de negociações entre dois clubes por um atleta.
* **Atributos:**
  * `id` (int) - Identificador da negociação.
  * `playerId` (int) - Jogador envolvido.
  * `sellerTeamId` (int) - Clube vendedor.
  * `buyerTeamId` (int) - Clube comprador.
  * `transferFee` (double) - Valor acordado da venda.
  * `status` (enum) - Proposta, Negociando, Aceito, Recusado, Concluído.
  * `transferDate` (Date) - Data da efetivação da transferência.
* **Relacionamentos:**
  * Envolve 1 `Player`, 1 `Team` vendedor e 1 `Team` comprador.
  * Sincronizado via `PesEditLib` para atualização do `EDIT00000000`.

---

### 9. `Manager` (Treinador / Manager)
* **Responsabilidade:** Representar o comandante técnico de uma equipe (seja o usuário ou um treinador controlado por IA).
* **Atributos:**
  * `id` (int) - Identificador único.
  * `name` (string) - Nome do treinador.
  * `nationalityId` (int) - Nacionalidade.
  * `reputation` (int) - Nível de prestígio (afeta atratividade do clube).
  * `preferredFormation` (string) - Esquema tático favorito (ex: 4-3-3).
  * `isHuman` (bool) - Indica se é o usuário do MLM ou IA.
* **Relacionamentos:**
  * Comanda 1 `Team`.
  * Possui 1 `Contract`.
  * Possui N registros em `History`.

---

### 10. `Nation` (País / Nacionalidade)
* **Responsabilidade:** Catalogar os países do mundo do jogo para critérios de elegibilidade de seleção, naturalização e filtros do mercado.
* **Atributos:**
  * `id` (int) - Código do país.
  * `name` (string) - Nome do país.
  * `flagCode` (string) - Código da bandeira / ícone.
  * `region` (enum) - Europa, América do Sul, Ásia, etc.
* **Relacionamentos:**
  * Origem de N `Player` e N `Manager`.
  * Vinculada a 1 `Team` (Seleção Nacional).

---

### 11. `Stadium` (Estádio)
* **Responsabilidade:** Modelar o local de jogo, sua capacidade de público e receitas operacionais por partida.
* **Atributos:**
  * `id` (int) - ID do estádio.
  * `name` (string) - Nome do estádio.
  * `capacity` (int) - Lotação máxima de torcedores.
  * `ticketPriceBase` (double) - Preço médio de ingresso.
* **Relacionamentos:**
  * Pertence a 1 `Team` (Sede principal).
  * Palco de N `Match`.

---

### 12. `Award` (Premiação Individual / Coletiva)
* **Responsabilidade:** Registrar honrarias da temporada (Bola de Ouro, Artilheiro da Liga, Seleção do Ano, Melhor Treinador).
* **Atributos:**
  * `id` (int) - Identificador do prêmio.
  * `name` (string) - Título do prêmio (ex: "Melhor Jogador do Mundo").
  * `seasonYear` (int) - Ano de concessão.
  * `category` (enum) - Jogador, Artilheiro, Treinador, Clube.
* **Relacionamentos:**
  * Concedido a 1 `Player`, `Manager` ou `Team`.
  * Vinculado a 1 `Season`.

---

### 13. `Finance` (Balanço Financeiro)
* **Responsabilidade:** Controlar o fluxo de caixa detalhado de um clube ao longo do ano fiscal.
* **Atributos:**
  * `teamId` (int) - Clube proprietário da conta.
  * `totalRevenue` (double) - Receita acumulada (venda de ingressos, patrocínios, vendas de jogadores).
  * `totalExpenses` (double) - Despesas acumuladas (salários, compras de jogadores, manutenção do estádio).
  * `netProfit` (double) - Lucro ou prejuízo líquido.
* **Relacionamentos:**
  * Pertence a 1 `Team`.

---

### 14. `YouthPlayer` (Jogador da Categoria de Base)
* **Responsabilidade:** Representar promessas em formação na academia de base antes da promoção ao profissional.
* **Atributos:**
  * `id` (int) - ID único na base.
  * `name` (string) - Nome da promessa.
  * `age` (int) - Idade (geralmente 15 a 18 anos).
  * `position` (enum) - Posição de origem.
  * `currentRating` (int) - Nível atual.
  * `potentialMin` / `potentialMax` (int) - Faixa de potencial previsto.
  * `monthsInAcademy` (int) - Tempo de maturação na base.
* **Relacionamentos:**
  * Pertence à base de 1 `Team`.
  * Quando promovido, transforma-se em um `Player` ativo e é gravado no PES via `PesEditLib`.

---

### 15. `Match` (Partida)
* **Responsabilidade:** Registrar o confronto entre dois clubes, incluindo o resultado retornado pelo PES, gols, cartões e estatísticas detalhadas.
* **Atributos:**
  * `id` (int) - ID da partida.
  * `homeTeamId` / `awayTeamId` (int) - Clubes mandante e visitante.
  * `homeGoals` / `awayGoals` (int) - Placar final.
  * `matchDate` (Date) - Data no calendário MLM.
  * `competitionId` (int) - Torneio a que pertence a partida.
  * `isPlayed` (bool) - Status de conclusão da partida.
* **Relacionamentos:**
  * Vinculada a 1 `Competition` e 1 `Season`.
  * Ocorre em 1 `Stadium`.
  * Gera N entradas de estatísticas individuais para `Player`.

---

### 16. `Calendar` (Calendário de Jogos)
* **Responsabilidade:** Organizar a agenda cronológica de partidas, eventos e janelas de uma temporada.
* **Atributos:**
  * `seasonYear` (int) - Ano de referência.
  * `totalDays` (int) - Quantidade de dias da temporada.
* **Relacionamentos:**
  * Contém N `Match` ordenadas por data.
  * Pertence a 1 `Season`.

---

### 17. `Injury` (Lesão / Departamento Médico)
* **Responsabilidade:** Controlar o histórico e o tempo de inatividade de um jogador por questões médicas.
* **Atributos:**
  * `id` (int) - ID do registro de lesão.
  * `playerId` (int) - Atleta lesionado.
  * `type` (string) - Tipo de lesão (ex: Estiramento, Fratura).
  * `daysRemaining` (int) - Dias para alta médica.
  * `severity` (enum) - Leve, Moderada, Grave.
* **Relacionamentos:**
  * Pertence a 1 `Player`.

---

### 18. `Suspension` (Suspensão)
* **Responsabilidade:** Controlar punições disciplinares (cartões vermelhos ou acúmulo de amarelos) que impedem a escalação de um atleta.
* **Atributos:**
  * `id` (int) - ID da suspensão.
  * `playerId` (int) - Atleta suspenso.
  * `competitionId` (int) - Competição na qual cumpre a pena.
  * `matchesRemaining` (int) - Quantidade de partidas de gancho.
  * `reason` (enum) - Acúmulo de Amarelos, Cartão Vermelho Direto, Punição Disciplinar.
* **Relacionamentos:**
  * Pertence a 1 `Player`.
  * Vinculada a 1 `Competition`.

---

### 19. `History` (Histórico de Carreira / Log Histórico)
* **Responsabilidade:** Armazenar dados históricos e estáticos de temporadas passadas (campeões antigos, artilheiros por ano, histórico de transferência do clube).
* **Atributos:**
  * `id` (int) - ID do log histórico.
  * `seasonYear` (int) - Ano da temporada arquivada.
  * `entityType` (enum) - Player, Team, Competition.
  * `summaryDataJson` (string) - Dados sumarizados do desempenho no ano.
* **Relacionamentos:**
  * Associa-se a `Player`, `Team` ou `Competition`.

---

### 20. `Configuration` (Configurações do MLM)
* **Responsabilidade:** Definir parâmetros globais do motor do MLM (dificuldade financeira, taxa de evolução de jovens, velocidade de simulação).
* **Atributos:**
  * `currencySymbol` (string) - Moeda utilizada (€, $, R$).
  * `financialDifficulty` (enum) - Fácil, Normal, Realista, Difícil.
  * `growthRate` (float) - Multiplicador de desenvolvimento de jogadores.
  * `pesEditFilePath` (string) - Caminho do arquivo `EDIT00000000`.

---

## 4. CONCLUSÃO & PRÓXIMOS PASSOS

Esta arquitetura garante que o **Master League Manager (MLM)** seja um sistema 100% modular, desacoplado das limitações binárias do PES 2021. 

A comunicação com o binário ocorrerá exclusivamente nas seguintes pontas:
1. **Sincronização Inicial (Load):** O MLM lê os jogadores e elencos do PES via `PesEditLib`.
2. **Efetivação de Transferências (Write):** Quando uma compra é fechada no MLM, a `PesEditLib` é invocada para alterar o roster no `EDIT00000000`.
3. **Evolução de Atributos (Write):** Ao fim de cada período, a evolução calculada pelo MLM atualiza os atributos no `EDIT00000000` via `PesEditLib`.

Com o modelo de domínio totalmente mapeado, o projeto está preparado para a criação da camada de persistência e implementação progressiva das regras do MLM.
