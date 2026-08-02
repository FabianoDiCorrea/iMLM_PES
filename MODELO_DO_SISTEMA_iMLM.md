# MODELO DO SISTEMA iMLM (Master League Manager)

> **Documento de Referência Arquitetural e Estrutura Lógica**  
> **Versão:** 1.0  
> **Data:** Julho de 2026  
> **Finalidade:** Descrever a organização dos módulos, entidades, relacionamentos, fluxo de dados, árvore de dependências e limites de integração com o PES 2021 sem entrar no mérito de código-fonte.

---

## 1. VISÃO GERAL DA ARQUITETURA LÓGICA

O **iMLM PES** é estruturado sobre o padrão de arquitetura em camadas desacopladas (Clean Architecture / Domain-Driven Design). 

```text
+-----------------------------------------------------------------------+
|                         CAMADA DE APLICAÇÃO                           |
|                  (MLMEngine / Fachada do Universo)                    |
+-----------------------------------------------------------------------+
                                    │
                                    ▼
+-----------------------------------------------------------------------+
|                         CAMADA DE SERVIÇOS                            |
|    (Motores: Temporada, Evolução, Transferências, Finanças, IA)       |
+-----------------------------------------------------------------------+
                                    │
                                    ▼
+-----------------------------------------------------------------------+
|                         CAMADA DE DOMÍNIO                             |
|          (Entidades de Negócio, Modelos e Tipos Primitivos)           |
+-----------------------------------------------------------------------+
                   │                                │
                   ▼                                ▼
+------------------------------------+   +------------------------------+
|       CAMADA DE PERSISTÊNCIA       |   |   CAMADA DE INFRAESTRUTURA   |
| (SaveGame / Histórico Imutável)    |   |   (Adaptador PesEditLib)     |
+------------------------------------+   +------------------------------+
                                                        │
                                                        ▼
                                         +------------------------------+
                                         |         EDIT00000000         |
                                         |    (Binário PES 2021)        |
                                         +------------------------------+
```

---

## 2. ÁRVORE DE DEPENDÊNCIA DOS MÓDULOS

A ordem natural de construção e dependência lógica entre os módulos respeita o seguinte fluxo descendente:

```text
1. Módulo Core (Tipos primitivos e utilitários universais)
   └──► 2. Módulo Jogadores
         └──► 3. Módulo Clubes
               └──► 4. Módulo Treinadores (Carreira)
                     └──► 5. Módulo Seleções
                           └──► 6. Módulo Finanças & Mercado
                                 └──► 7. Módulo Categorias de Base & Regens
                                       └──► 8. Módulo Competições & Partidas
                                             └──► 9. Módulo Temporadas & Calendário
                                                   └──► 10. Módulo Notícias, Rankings & Hall da Fama
                                                         └──► 11. Módulo Sincronização, Exportação & Importação (PES 2021)
```

---

## 3. DETALHAMENTO DOS MÓDULOS DO SISTEMA

---

### 1. Módulo Carreira do Treinador
* **Objetivo:** Encapsular a trajetória profissional do treinador humano.
* **Responsabilidades:** Controlar licenças, reputação, acúmulo de prestígio, histórico de clubes/seleções comandados e estado contratual do protagonista.
* **Entidades Pertencentes:** `TreinadorHumano`, `PerfilTreinador`, `HistoricoCarreira`.
* **Dependências:** `Módulo Clubes`, `Módulo Seleções`.
* **Dados Persistidos:** Nível de reputação, histórico de títulos, salários acumulados, prêmios recebidos, registro de demissões/pedidos de demissão.
* **Integração:** Comunica-se com o `Módulo Mercado` para recebimento de propostas e com o `Módulo Finanças` para gestão salarial.

---

### 2. Módulo Treinadores (IA)
* **Objetivo:** Gerenciar o ecossistema de técnicos controlados por inteligência artificial.
* **Responsabilidades:** Assegurar que cada clube/seleção possua um comandante técnico com esquema tático definido e reputação própria.
* **Entidades Pertencentes:** `TreinadorIA`, `EstiloTatico`.
* **Dependências:** `Módulo Core`.
* **Dados Persistidos:** Nome, nacionalidade, reputação, formação tática preferida, clube/seleção atual, tempo de contrato.
* **Integração:** Fornece peso tático e de liderança para o `Módulo Competições` (simulação de partidas).

---

### 3. Módulo Clubes
* **Objetivo:** Representar as instituições esportivas dentro do ecossistema.
* **Responsabilidades:** Manter elencos, dados de infraestrutura, prestígio institucional, diretoria e rivalidades.
* **Entidades Pertencentes:** `Clube`, `Estadio`, `Diretoria`, `Rivalidade`.
* **Dependências:** `Módulo Jogadores`.
* **Dados Persistidos:** ID do PES, nome, sigla, país, divisão, reputação (1-5 estrelas), capacidade do estádio, lista de rivais.
* **Integração:** Conecta-se ao `Módulo Finanças`, `Módulo Categorias de Base`, `Módulo Treinadores` e `Módulo Competições`.

---

### 4. Módulo Jogadores
* **Objetivo:** Modelar a biografia técnica, física e mercadológica dos atletas.
* **Responsabilidades:** Armazenar atributos técnicos/físicos, calcular Overall Rating, aplicar curva de evolução/involução por idade e gerenciar lesões/suspensões.
* **Entidades Pertencentes:** `Jogador`, `AtributosTecnicos`, `CurvaEvolucao`, `Lesao`, `Suspensao`.
* **Dependências:** `Módulo Core`.
* **Dados Persistidos:** ID PES, nome, nome de camisa, idade, data de nascimento, nacionalidade, posições, atributos bitwise, valor de mercado, moral, estado de lesão.
* **Integração:** Fornece a matéria-prima para o `Módulo Clubes`, `Módulo Seleções`, `Módulo Mercado` e `Módulo Sincronização`.

---

### 5. Módulo Competições
* **Objetivo:** Estruturar e resolver os torneios e partidas do universo.
* **Responsabilidades:** Organizar tabelas de pontos corridos, chaves de mata-mata, aplicar regras de saldo/gols e determinar campeões e rebaixados.
* **Entidades Pertencentes:** `Competicao`, `EdicaoCompeticao`, `Liga`, `Copa`, `TabelaClassificacao`, `Chaveamento`, `Partida`.
* **Dependências:** `Módulo Clubes`, `Módulo Treinadores`.
* **Dados Persistidos:** Histórico de edições, classificação rodada a rodada, placares de partidas, estatísticas individuais da edição.
* **Integração:** Recebe dados importados do PES via `Módulo Importação` (Modo Humano) ou aciona o algoritmo interno de simulação (Modo Simulado).

---

### 6. Módulo Mercado de Transferências
* **Objetivo:** Processar o fluxo de negociações entre clubes e atletas.
* **Responsabilidades:** Avaliar propostas de compra, venda, empréstimo, aceite de IA, rescisão e agentes livres.
* **Entidades Pertencentes:** `Transferencia`, `Proposta`, `JanelaTransferencias`.
* **Dependências:** `Módulo Jogadores`, `Módulo Clubes`, `Módulo Finanças`.
* **Dados Persistidos:** Histórico de transações, valores negociados, cláusulas de rescisão, status de propostas ativas.
* **Integração:** Altera a composição de elencos no `Módulo Clubes` e dispara requisições ao `Módulo Exportação` (sincronização no EDIT).

---

### 7. Módulo Finanças
* **Objetivo:** Gerir a contabilidade e a saúde econômica de todas as instituições.
* **Responsabilidades:** Controlar receitas, despesas, pagamentos de salários, bilheteria, patrocínios e limites orçamentários.
* **Entidades Pertencentes:** `BalancoFinanceiro`, `Contrato`, `Patrocinador`, `Orcamento`.
* **Dependências:** `Módulo Clubes`, `Módulo Jogadores`, `Módulo Treinadores`.
* **Dados Persistidos:** Saldo em caixa, orçamento de transferências, folha salarial total, contratos vigentes, histórico de receitas/despesas.
* **Integração:** Restringe as ações do `Módulo Mercado` e define o nível de satisfação da diretoria no `Módulo Clubes`.

---

### 8. Módulo Temporadas & Calendário
* **Objetivo:** Orquestrar o progresso temporal síncrono de todo o universo.
* **Responsabilidades:** Controlar a máquina de estados (Pré-temporada, Mercado, Competições, Pós-temporada) e avançar a idade/curva do mundo simultaneamente.
* **Entidades Pertencentes:** `Temporada`, `Calendario`, `FaseTemporada`.
* **Dependências:** `Módulo Competições`, `Módulo Mercado`.
* **Dados Persistidos:** Ano da temporada atual, fase ativa, dia do calendário, histórico de transições anuais.
* **Integração:** Dispara gatilhos universais em lote para o `Módulo Jogadores` (envelhecimento) e `Módulo Finanças` (fechamento fiscal).

---

### 9. Módulo Seleções Nacionais
* **Objetivo:** Gerenciar as seleções e os torneios internacionais.
* **Responsabilidades:** Processar convocações por nacionalidade, rankings FIFA/iMLM e torneios continentais/mundiais.
* **Entidades Pertencentes:** `SelecaoNacional`, `Convocacao`, `RankingInternacional`.
* **Dependências:** `Módulo Jogadores`, `Módulo Treinadores`.
* **Dados Persistidos:** Lista de atletas convocados por data FIFA, histórico de títulos internacionais, posição no ranking.
* **Integração:** Consome atletas do `Módulo Jogadores` sem desvinculá-los dos seus respectivos clubes.

---

### 10. Módulo Categorias de Base & Regens
* **Objetivo:** Assegurar a renovação demográfica contínua dos atletas.
* **Responsabilidades:** Gerar novos talentos juvenis e reciclagem de atletas aposentados (Regens).
* **Entidades Pertencentes:** `CategoriaBase`, `AtletaBase`, `GeradorRegen`.
* **Dependências:** `Módulo Jogadores`, `Módulo Clubes`.
* **Dados Persistidos:** Atletas em formação nas academias, faixas de potencial previsto, cadastro de regens pendentes.
* **Integração:** Quando um atleta da base é promovido, ele é injetado no `Módulo Jogadores` e sincronizado via `Módulo Exportação`.

---

### 11. Módulo Notícias & Imprensa
* **Objetivo:** Gerar narrativas dinâmicas baseadas nos acontecimentos do universo.
* **Responsabilidades:** Processar eventos relevantes e transformá-los em feeds noticiosos categorizados.
* **Entidades Pertencentes:** `Noticia`, `FeedJornalistico`.
* **Dependências:** `Módulo Mercado`, `Módulo Competições`, `Módulo Carreira`.
* **Dados Persistidos:** Histórico de manchetes por temporada, nível de impacto do evento.
* **Integração:** Consome eventos disparados por todos os outros módulos de serviço.

---

### 12. Módulo Rankings, Estatísticas & Hall da Fama
* **Objetivo:** Manter a memória histórica e imutável de todas as décadas de jogo.
* **Responsabilidades:** Calcular artilharias, eleger prêmios individuais (Bola de Ouro) e imortalizar ídolos.
* **Entidades Pertencentes:** `EstatisticaCarreira`, `Premiação`, `HallDaFama`, `RecordeHistorico`.
* **Dependências:** `Módulo Competições`, `Módulo Jogadores`, `Módulo Treinadores`.
* **Dados Persistidos:** Acumulado de gols/assistências de por vida, vencedores de prêmios por ano, galeria de lendas.
* **Integração:** Leitura permanente dos relatórios de encerramento do `Módulo Temporadas`.

---

### 13. Módulo Sincronização, Exportação & Importação (PES 2021)
* **Objetivo:** Realizar a ponte de comunicação bit-a-bit entre o iMLM e o arquivo `EDIT00000000`.
* **Responsabilidades:** Traduzir os dados do iMLM para a estrutura binária do PES (Exportação) e ler resultados de partidas (Importação).
* **Entidades Pertencentes:** `AdaptadorPesEdit`, `LeitorBinario`, `EscritorBinario`, `LeitorResultados`.
* **Dependências:** `Módulo Jogadores`, `Módulo Clubes`, `Módulo Sincronização (PesEditLib)`.
* **Dados Persistidos:** Mapeamento de offsets e IDs binários do PES 2021.
* **Integração:** Garante que o PES 2021 espelhe perfeitamente a realidade do iMLM antes de partidas humanas.

---

## 4. MAPA DE RELACIONAMENTOS ENTRE ENTIDADES (DIAGRAMA DE ENTIDADES)

```text
[Treinador] ────── (1:1) ──────► [Contrato] ────── (N:1) ──────► [Clube]
    │                                                               │
    │ (Comanda)                                                     │ (Possui)
    ▼                                                               ▼
[Seleção] ◄────── (N:1) ─────── [Jogador] ◄────── (1:N) ─────── [Elenco]
    │                              │                                │
    │                              │ (Possui)                       │ (Possui)
    │                              ▼                                ▼
    │                         [Contrato]                   [CategoriaBase]
    │                              │                                │
    │                              ▼                                ▼
    │                     [BalancoFinanceiro] ◄──────────── [AtletaBase]
    │                              │
    ▼                              ▼
[Competicao] ◄─── (1:N) ──── [Partida] ◄─── (N:1) ────────── [Estadio]
    │                              │
    │                              ▼
    └─────────── (1:N) ────► [Estatistica] ───► [Historico] ───► [HallDaFama]
```

### Relacionamentos Principais:
1. **Treinador ── (Possui) ──► Contrato:** Um treinador possui um contrato de trabalho ativo vinculado a um Clube ou Seleção.
2. **Clube ── (Possui) ──► Elenco:** Um clube possui um elenco composto por até 40 Jogadores.
3. **Jogador ── (Pertence) ──► Clube:** Um jogador possui contrato vigente com exatamente um Clube (ou é Agente Livre).
4. **Jogador ── (Convocado) ──► Seleção:** Um jogador pode ser convocado para a Seleção Nacional correspondente à sua nacionalidade.
5. **Treinador ── (Dirige) ──► Clube / Seleção:** Um treinador pode comandar um Clube, uma Seleção ou ambos simultaneamente.
6. **Clube ── (Possui) ──► CategoriaBase:** Um clube possui uma academia contendo N AtletasBase.
7. **Temporada ── (Contém) ──► Calendario:** Uma temporada contém um calendário cronológico com N Competições e N Partidas.
8. **Competição ── (Agrupa) ──► Participantes:** Uma competição agrupa N Clubes ou N Seleções participantes.
9. **Partida ── (Ocorre em) ──► Estádio:** Uma partida é realizada no Estádio do clube mandante.

---

## 5. SEPARAÇÃO DE DADOS: iMLM vs. PES 2021

Para garantir o respeito à Constituição do Universo, as informações do sistema são rigorosamente divididas entre dados sincronizados com o arquivo `EDIT00000000` e dados exclusivos da inteligência do iMLM.

### 5.1 Dados Sincronizados com o PES 2021
Estes dados são lidos/gravados no arquivo binário do PES via `PesEditLib` para refletir o estado exato nas partidas humanas:

* **Jogadores:** PES Player ID, Nome completo, Nome na camisa, Idade, Posição principal, Posições secundárias, Pé dominante, Atributos técnicos/físicos/táticos bitwise.
* **Clubes:** PES Team ID, Nome do clube, Sigla, ID da Liga participante, ID do Estádio associado.
* **Elencos (Rosters):** Composição exata do grupo de atletas de cada time (efetivação de transferências e promoções da base).
* **Treinadores:** Nome e aparência do técnico (quando a estrutura do bloco no EDIT permitir compatibilidade).

---

### 5.2 Dados Exclusivos do iMLM (NUNCA EXISTEM NO PES)
Estes dados pertencem 100% à camada administrativa do iMLM e jamais são gravados no binário do PES 2021:

* **Carreira & Treinadores:** Reputação do treinador, licenças, histórico profissional completo, metas da diretoria, insatisfação e propostas de contratação.
* **Economia & Finanças:** Orçamento de transferências, folha salarial, saldo em caixa, patrocinadores master/secundários, valor de mercado dos atletas e multas rescisórias.
* **Contratos:** Duração de contratos, salários semanais/mensais, bônus por gols/partidas.
* **Categorias de Base & Regens:** Promessas em formação, cálculo de potencial futuro (Min/Max) e cadastro de aposentados para reciclagem.
* **Histórico & Registros:** Registros imutáveis de campeões de 50 anos atrás, Bola de Ouro, artilheiros históricos, Hall da Fama e livro de recordes.
* **Notícias & Mídia:** Gerador de notícias, artigos jornalísticos e rumores de mercado.
* **Saúde & Disciplina a Longo Prazo:** Lesões de longa duração (departamento médico por dias/meses) e suspensões acumuladas por torneio.
* **Rankings:** Ranking global de clubes e ranking mundial de seleções.

---

## 6. CONCLUSÃO

O **Modelo do Sistema iMLM** estabelece uma arquitetura clara, robusta e modular. A separação estrita de responsabilidades garante que o iMLM funcione como um ecossistema autônomo, persistente e completo, utilizando o PES 2021 apenas como um motor de execução gráfica e de partidas.
