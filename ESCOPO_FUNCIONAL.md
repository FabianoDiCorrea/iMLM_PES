# ESCOPO FUNCIONAL DO iMLM PES (Master League Manager)

> **Documento de Referência Funcional Definitiva**  
> **Versão:** 1.0  
> **Data:** Julho de 2026  
> **Filosofia Central:** *"O iMLM PES busca oferecer a profundidade de gerenciamento esperada de um simulador moderno de carreira, enquanto delega ao PES 2021 a execução das competições escolhidas pelo usuário."*

---

## 1. VISÃO GERAL E PREMISSAS ARQUITETURAIS

O **iMLM PES (Master League Manager)** é um ecossistema completo e persistente de gerenciamento de carreira de futebol. Ele combina a inteligência administrativa profunda com o motor gráfico e de jogabilidade do **PES 2021**.

### 1.1 Princípio da Separação de Responsabilidades
* **iMLM PES (Cérebro do Universo):** Responsável por gerenciar o universo futebolístico em sua totalidade (clubes, jogadores, treinadores, seleções, finanças, mercado, estatísticas, convocações, notícias, categorias de base, evolução, regras e histórico).
* **PES 2021 (Motor de Partida):** Responsável exclusivamente pela execução física, tática e visual das partidas disputadas no **Modo Humano**. O PES não administra carreiras, não toma decisões de mercado e não guarda o estado oficial do universo.

### 1.2 Fonte Única da Verdade e Persistência
* O estado oficial e definitivo do universo reside 100% no **iMLM PES**.
* O arquivo binário do PES (`EDIT00000000`) atua como uma ponte temporária de I/O sincronizada via `PesEditLib`.
* O universo é **eterno e contínuo**: temporadas passam, pessoas envelhecem, títulos são acumulados e o passado afeta permanentemente o futuro ao longo de décadas.

---

## 2. ESTILO E MÓDULOS FUNCIONAIS DO SISTEMA

---

### MÓDULO 1: CARREIRA DO TREINADOR (PROTAGONISTA)

#### 1.1 Objetivo
Colocar o Treinador como o centro focal do iMLM. O progresso do jogo não pertence ao clube, mas sim à trajetória profissional do técnico humano.

#### 1.2 Descrição
O usuário constrói sua identidade profissional, acumula prestígio, recebe ofertas de emprego de clubes e seleções, cumpre metas da diretoria e constrói seu legado imortal no Hall da Fama.

#### 1.3 Regras Gerais
* **Criação da Carreira:** Definição de nome, nacionalidade, estilo tático de preferência, aparências conceituais e licença inicial de treinador.
* **Reputação Dinâmica:** Pontuação (0.0 a 100.0 ou 1 a 5 estrelas) impactada por desempenho em competições, cumprimento de metas financeiras/esportivas e relevância dos títulos.
* **Ciclo Contratual:** O treinador assina contratos com tempo determinado, salário base e metas estipuladas. 
* **Movimentação no Mercado:**
  * **Propostas:** Propostas recebidas de outros clubes/seleções com base na reputação atual.
  * **Demissão:** Mau desempenho sustentado ou quebra grave de orçamento resulta em demissão imediata, deixando o técnico livre no mercado.
  * **Pedido de Demissão:** O treinador pode rescindir seu contrato para assumir outro projeto ou aguardar propostas.
  * **Renovação Contratual:** Ao final de cada ciclo, a diretoria oferece novos termos baseados nos resultados.
* **Dupla Função (Clube & Seleção):** Treinadores de alta reputação podem comandar simultaneamente um clube e uma seleção nacional.
* **Aposentadoria e Legado:** Ao decidir se aposentar (ou atingir o limite de idade de carreira), a trajetória é encerrada e imortalizada no Hall da Fama com estatísticas consolidadas.

#### 1.4 Sugestões de Evolução Futura
* Licenças de treinador graduadas (UEFA Pro, Conmebol A, etc.) destravando clubes maiores.
* Árvore de habilidades e especializações do treinador (ex: *Desenvolvedor de Jovens*, *Tático Defensivo*, *Motivador Financial*).
* Relacionamento com a imprensa e coletivas de imprensa impactando o moral do elenco.

---

### MÓDULO 2: CLUBES

#### 2.1 Objetivo
Representar cada instituição esportiva como uma entidade autônoma, viva e com identidade própria dentro do universo.

#### 2.2 Descrição
Os clubes possuem estruturas financeiras, objetivos institucionais, elenco de atletas, comissão técnica, estádio, divisões de base e torcida que evoluem ou regridem ao longo das décadas.

#### 2.3 Regras Gerais
* **Atributos Principais:** Nome, sigla, país, liga principal, divisão, reputação (1 a 5 estrelas), rivalidades históricas e cores/identidade.
* **Infraestrutura e Estádio:** Nome do estádio, capacidade de público, nível das instalações médicas e de treino, e categorias de base.
* **Finanças Institucionais:** Orçamento total, orçamento de transferências, folha salarial máxima e saúde financeira (excelente, estável, crítica).
* **Diretoria e Objetivos:** A diretoria estipula metas anuais obrigatórias (ex: "Ficar no Top 4", "Evitar rebaixamento", "Conquistar a Copa").
* **Patrocinadores:** Contratos de patrocínio master e secundário que injetam capital fixo e bônus por vitórias/títulos.
* **Estatísticas e Histórico:** Galeria de troféus, maiores goleadas, maior artilheiro da história do clube e registro de temporadas passadas.

#### 2.4 Sugestões de Evolução Futura
* Investimentos em expansão de estádio e modernização de CT para acelerar evolução da base.
* Troca de patrocinadores com base na reputação e exposição na mídia.
* Crises institucionais (Mudança de presidente, intervenções financeiras em clubes endividados).

---

### MÓDULO 3: JOGADORES

#### 3.1 Objetivo
Simular a biografia esportiva, técnica, física e econômica completa de cada atleta do universo.

#### 3.2 Descrição
Atletas progridem em curva de idade, sofrem lesões, valorizam-se no mercado, assinam contratos e abrem espaço para novas gerações ao se aposentarem.

#### 3.3 Regras Gerais
* **Identificação e Dados:** Nome, nome de camisa, idade, data de nascimento, nacionalidade, posição principal, posições secundárias e pé dominante.
* **Perfil Técnico e Físico:** Atributos compatíveis com a estrutura binária do PES 2021 (habilidades técnicas, velocidade, resistência, força) e Overall Rating calculado.
* **Motor de Evolução e Potencial:** 
  * Cada jogador possui uma curva de desenvolvimento definida por seu Potencial e idade (Fase de Crescimento, Ápice e Declínio).
  * O rendimento em partidas e minutos jogados influenciam na velocidade de evolução.
* **Economia Individual:** Valor de mercado calculado algoritmicamente, salário atual, multa rescisória e tempo de contrato restante.
* **Saúde e Disciplina:** Registros de lesões (tipo e tempo em dias de DM) e punições por cartões (suspensões acumuladas).
* **Fim de Carreira e Regens:** 
  * Atletas veteranos se aposentam ao final da temporada.
  * O sistema gera um **Regen** (novo jovem talento de 16-18 anos) mantendo o equilíbrio demográfico do universo.

#### 3.4 Sugestões de Evolução Futura
* Sistema de personalidade do atleta (ex: *Líder*, *Mercenário*, *Prata da Casa*, *Inconstante*).
* Adaptação tática para aprendizado de novas posições secundárias.
* Dinâmica de insatisfação por falta de tempo em campo exigindo titularidade.

---

### MÓDULO 4: TÉCNICOS (TREINADORES IA)

#### 4.1 Objetivo
Garantir que todos os clubes e seleções do mundo possuam um comandante técnico ativo e inteligente, operado pela IA do iMLM.

#### 4.2 Descrição
Treinadores de IA possuem características táticas, reputação própria, preferências de formação e movimentam o mercado técnico ao trocar de clube ou ser demitidos.

#### 4.3 Regras Gerais
* **Garantia de Comando:** Nenhum clube ou seleção pode disputar competições sem um treinador atribuído.
* **Estilo Tático:** Cada técnico possui esquema tático de preferência (ex: 4-3-3, 3-5-2) e mentalidade (Posse, Contra-ataque, Pressão alta).
* **Carreira de IA:** Treinadores de IA sobem ou descem de nível, recebem propostas de clubes maiores, são demitidos por maus resultados e podem se aposentar.
* **Sincronização com o PES:** Sempre que a estrutura binária permitir, o nome e características do treinador serão sincronizados no `EDIT00000000`.

#### 4.4 Sugestões de Evolução Futura
* Árvore de rivalidade entre treinadores (histórico de confrontos diretos).
* Filosofia de recrutamento do técnico (ex: treinadores que priorizam contratações de jovens vs. experientes).

---

### MÓDULO 5: COMPETIÇÕES E MODOS DE RESOLUÇÃO

#### 5.1 Objetivo
Estruturar o calendário esportivo global composto por Ligas (pontos corridos), Copas (mata-mata) e Torneios Continentais/Internacionais.

#### 5.2 Descrição
Cada competição pode ser disputada no **Modo Humano** (via PES 2021) ou resolvida automaticamente no **Modo Simulado** (via iMLM).

#### 5.3 Regras Gerais
* **Modo Humano (Modo PES):**
  * O iMLM prepara e sincroniza os elencos no arquivo `EDIT00000000`.
  * O usuário executa as partidas dentro do PES 2021.
  * Ao final da rodada/competição, os dados e placares são importados de volta para o iMLM.
* **Modo Simulado (Modo iMLM):**
  * O iMLM calcula os resultados de forma instantânea ou por rodada.
  * **Não há simulação visual 3D:** o motor matemático utiliza a força dos elencos, táticas, reputação, mando de campo e momento para definir o placar.
* **Estrutura Organizacional:** 
  * Ligas com sistema de acesso e rebaixamento (Divisão 1, Divisão 2).
  * Copas com jogos de ida e volta ou eliminação simples.
  * Vagas continentais distribuídas dinamicamente via classificação da liga.

#### 5.4 Sugestões de Evolução Futura
* Recalculo de coeficientes continentais por país para alterar a quantidade de vagas da Champions/Libertadores.
* Criação de supercopas e torneios de pré-temporada amigáveis.

---

### MÓDULO 6: MERCADO DE TRANSFERÊNCIAS

#### 6.1 Objetivo
Simular o mercado global de compra, venda e empréstimo de jogadores com comportamento realista de IA.

#### 6.2 Descrição
Clubes geridos por IA analisam suas carências no elenco, consultam seus orçamentos e enviam propostas formais por atletas, negociando salários e contratos.

#### 6.3 Regras Gerais
* **Janelas de Transferências:** O mercado se abre apenas nas fases estipuladas da temporada (Janela Principal de Início de Ano e Janela Intermediária).
* **Modalidades de Negociação:** Compra em definitivo, proposta de empréstimo (com ou sem opção de compra) e contratação de Agentes Livres.
* **Propostas e Recusas:**
  * Valores aceitos dependem do valor de mercado, reputação do clube comprador, tempo restante de contrato e importância do atleta para o time de origem.
  * O jogador pode recusar a transferência se o salário ou a reputação do novo clube forem insatisfatórios.
* **Multa Rescisória:** Pagamento integral da multa rescisória obriga o clube detentor a liberar as negociações diretas com o atleta.
* **Restrição de Elenco:** Respeito rigoroso aos limites de elenco (máximo de 40 jogadores por clube).

#### 6.4 Sugestões de Evolução Futura
* Troca direta de jogadores (Atleta + Compensação financeira).
* Cláusulas de porcentagem de venda futura e bônus por performance.
* Lista de transferências e jogadores encostados pedindo para sair.

---

### MÓDULO 7: FINANÇAS E ECONOMIA

#### 7.1 Objetivo
Garantir que a gestão financeira seja um fator crítico no sucesso ou fracasso de clubes e treinadores.

#### 7.2 Descrição
Todas as transações de dinheiro são contabilizadas no fluxo de caixa anual dos clubes, impactando o orçamento de contratações e a avaliação da diretoria.

#### 7.3 Regras Gerais
* **Receitas:** Premiações por fases de competições, venda de ingressos (bilheteria baseada na capacidade do estádio e apelo do jogo), patrocínios e venda de atletas.
* **Despesas:** Folha salarial dos jogadores e comissão técnica, custo de manutenção de instalações, contratações efetuadas e multas rescisórias.
* **Orçamento de Transferências e Salários:** Divisão clara entre o caixa disponível para compras imediatas e a margem mensal/anual para vencimentos salariais.
* **Fair Play Financeiro e Balanço:** Clubes que operam no vermelho sofrem punições da diretoria (redução de orçamento ou impossibilidade de efetuar compras).

#### 7.4 Sugestões de Evolução Futura
* Empréstimos bancários e quitação de dívidas a longo prazo.
* Investimento customizável no orçamento da base para melhorar o rating dos regens.

---

### MÓDULO 8: CATEGORIAS DE BASE E REGENS

#### 8.1 Objetivo
Garantir o rejuvenescimento contínuo do universo do futebol sem perda de qualidade técnica global.

#### 8.2 Descrição
A base gera promessas inéditas e processa o retorno de lendas e jogadores veteranos recém-aposentados através do sistema de Regens.

#### 8.3 Regras Gerais
* **Geração de Novos Talentos:** Jogadores de 15 a 18 anos gerados periodicamente na academia de base de cada clube.
* **Regens (Reencarnação Esportiva):** Jogadores aposentados retornam ao universo com 16 anos, mesmo país e posição, porém com nome renovado e desenvolvimento zerado.
* **Promotores de Base:** O treinador pode promover atletas da base ao elenco principal a qualquer momento se houver vaga no elenco profissional.
* **Maturação e Avaliação:** Promessas possuem faixa de potencial estimado (ex: Potencial 80-88) que se confirma conforme jogam no time principal.

#### 8.4 Sugestões de Evolução Futura
* Rede de Olheiros (Scouting) enviada para diferentes continentes em busca de jovens promessas.
* Torneio de base simulado (Copa São Paulo / Youth League).

---

### MÓDULO 9: SELEÇÕES NACIONAIS E CONVOCAÇÕES

#### 9.1 Objetivo
Integrar o futebol internacional de seleções ao calendário dos clubes e à carreira do treinador.

#### 9.2 Descrição
Seleções nacionais convocam os melhores atletas elegíveis por nacionalidade para disputar Eliminatórias, Copas Continentais e Copa do Mundo.

#### 9.3 Regras Gerais
* **Elegibilidade:** Atletas são elegíveis exclusivamente para a seleção da sua nacionalidade primária.
* **Períodos de Convocação:** Ocorrem em datas específicas do calendário. Atletas convocados ficam indisponíveis temporariamente para amistosos de clubes.
* **Comando Técnico:** Seleções possuem ranking de prestígio e contratam treinadores de alta reputação (humanos ou IA).
* **Torneios Internacionais:** Simulação ou disputa de Copas Continentais (Eurocopa, Copa América) e Copa do Mundo a cada 4 anos.

#### 9.4 Sugestões de Evolução Futura
* Dupla nacionalidade e processo de escolha da seleção principal pelo jovem atleta.
* Lesões em "Data FIFA" gerando compensação financeira ao clube.

---

### MÓDULO 10: ESTATÍSTICAS, RECORDES E HALL DA FAMA

#### 10.1 Objetivo
Registrar cada feito relevante do universo, permitindo consultas históricas imutáveis a qualquer momento do futuro.

#### 10.2 Descrição
O sistema guarda recordes de gols, assistências, prêmios individuais, títulos coletivos e marcas históricas alcançadas por treinadores e atletas.

#### 10.3 Regras Gerais
* **Estatísticas de Temporada:** Artilharia, assistências, partidas jogadas, cartões, notas médias de atuação e melhor goleiro por competição.
* **Premiações Anuais:** Bola de Ouro (Melhor do Mundo), Seleção da Temporada, Melhor Treinador do Ano e Golden Boy.
* **Recordes Históricos Globais:** Maior artilheiro da história de uma liga/clube, transferência mais cara de todos os tempos, maior sequência de vitórias.
* **Hall da Fama:** Galeria imutável com os maiores treinadores e jogadores da história do simulador após a aposentadoria.

#### 10.4 Sugestões de Evolução Futura
* Comparador de lendas (comparar carreiras de dois ex-jogadores do universo).
* Quadro de honra no perfil do clube com os maiores ídolos da história da instituição.

---

### MÓDULO 11: NOTÍCIAS E JORNALISMO DO UNIVERSO

#### 11.1 Objetivo
Aumentar a imersão do usuário, informando tudo o que acontece no universo do futebol de forma dinâmica e narrativa.

#### 11.2 Descrição
Um motor gerador de feed de notícias cria manchetes e textos sobre transferências bombásticas, demissões, resultados decisivos e lesões graves.

#### 11.3 Regras Gerais
* **Feed Dinâmico:** Atualizado a cada mudança de fase de temporada ou grande evento.
* **Categorias de Notícias:** 
  * Mercado (ex: *"Real Madrid acerta a compra de atacante por 100M"*);
  * Crise/Diretoria (ex: *"Após derrota humilhante, técnico é demitido"*);
  * Lesões (ex: *"Estrela da equipe desfalca o time por 6 meses"*);
  * Conquistas (ex: *"Clube X sagra-se campeão após 20 anos"*).
* **Filtros por Relevância:** Notícias divididas entre Mundo, Liga Atual e Meu Clube.

#### 11.4 Sugestões de Evolução Futura
* Notícias com rumores e especulações de mercado nem sempre confirmadas (Fogo de Palha).
* Reação de torcedores fictícios nas redes sociais virtuais do jogo.

---

### MÓDULO 12: MOTOR DE TEMPORADA E LINHA DO TEMPO

#### 12.1 Objetivo
Orquestrar a transição do tempo e a evolução síncrona de todo o universo através de uma máquina de estados rígida.

#### 12.2 Descrição
O tempo no iMLM não depende de dias corridos em tempo real, mas sim do avanço por **Fases Temporais**, onde todas as entidades do mundo processam suas ações em lote.

#### 12.3 Regras Gerais
* **Fases Oficiais da Temporada:**
  ```text
  [1. Pré-temporada] ──► [2. Mercado Principal] ──► [3. Competições] ──► [4. Mercado Intermediário] ──► [5. Pós-temporada]
  ```
* **Ações em Cada Fase:**
  1. **Pré-temporada:** Atualização das idades dos atletas (+1 ano), aplicação da curva de evolução/involução física, orçamento anual liberado pelas diretorias e definição dos objetivos.
  2. **Mercado Principal:** Abertura da janela de transferências global, negociações intensas de IA e usuário.
  3. **Competições:** Disputa das partidas em Modo Humano (PES) ou Resolução Simulado (iMLM), acúmulo de estatísticas, cartões e lesões.
  4. **Mercado Intermediário:** Janela secundária de ajustes de elenco.
  5. **Pós-temporada:** Apuração dos campeões, rebaixamentos, concessão de prêmios (Bola de Ouro), aposentadorias de atletas/técnicos, regens e fechamento de balanços financeiros.
* **Consequências Permanentes:** O universo avança sem opção de "resetar" temporadas passadas.

#### 12.4 Sugestões de Evolução Futura
* Opção de salvar pontos de checagem ao final de cada década para análise de estatísticas históricas.

---

### MÓDULO 13: SINCRONIZAÇÃO E INTEGRAÇÃO COM O PES 2021

#### 13.1 Objetivo
Manter a perfeita paridade entre o banco de dados do iMLM e o arquivo de opção do jogo (`EDIT00000000`) antes e depois das competições humanas.

#### 13.2 Descrição
A biblioteca `PesEditLib` é utilizada exclusivamente na camada de Infraestrutura do iMLM para traduzir o estado do universo para o binário do PES 2021 e vice-versa.

#### 13.3 Regras Gerais
* **Exportação (iMLM ➔ PES):**
  * Ao iniciar um ciclo de partidas no Modo Humano, o iMLM grava no `EDIT00000000`:
    * Elencos atualizados de todos os clubes (transferências efetivadas);
    * Idades, posições e atributos técnicos recalculados pela evolução do iMLM;
    * Nomes de jogadores e numerações de camisa;
    * Nomes e atribuições de treinadores (quando tecnicamente suportado pela estrutura do binário).
* **Importação (PES ➔ iMLM):**
  * Após a conclusão de torneios ou partidas humanas no PES, o iMLM lê os relatórios/arquivos de resultados:
    * Placar de jogos e classificados;
    * Autores de gols, assistências e cartões acumulados;
    * Estatísticas individuais para incorporação definitiva ao histórico do iMLM.
* **Proteção de Dados:** O universo oficial reside sempre no iMLM; se o arquivo do PES for corrompido ou substituído, ele pode ser regravado a qualquer momento a partir do Save do iMLM.

#### 13.4 Sugestões de Evolução Futura
* Ferramenta de autocorreção e checagem de integridade binária antes da gravação do `EDIT00000000`.

---

### MÓDULO 14: SISTEMA DE EXPECTATIVAS

#### 14.1 Objetivo
Definir e recalcular dinamicamente as exigências e metas institucionais de cada clube do universo, eliminando objetivos estáticos ou inflexíveis.

#### 14.2 Descrição
O **Sistema de Expectativas** utiliza algoritmos de IA para analisar continuamente o contexto histórico, a tradição e o momento esportivo/financeiro do clube, estabelecendo objetivos coerentes para cada temporada.

#### 14.3 Regras Gerais
* **Geração Dinâmica de Expectativas:** As metas de um clube são recalculadas no início de cada temporada considerando simultaneamente:
  * **Tradição do Clube:** História de longo prazo, relevância nacional/continental e peso da camisa;
  * **Reputação Atual:** Nível de prestígio no mercado (1 a 5 estrelas);
  * **Desempenho Recente:** Resultados e campanhas acumulados nas últimas 3 a 5 temporadas;
  * **Divisão e Liga:** Nível competitivo do campeonato disputado (Tier 1, Tier 2, etc.);
  * **Qualidade do Elenco:** Overall médio dos atletas titulares e reservas;
  * **Situação Financeira:** Saúde do caixa e capacidade de investimento em salários/transferências;
  * **Competições Disputadas:** Participação paralela em torneios continentais (Champions/Libertadores) ou copas nacionais;
  * **Momento Esportivo:** Sequência de evolução (ascensão de divisão) ou declínio (rebaixamentos recentes).
* **Impacto Global do Sistema:** As expectativas geradas dinamicamente influenciam diretamente:
  * **Diretoria:** Avaliação da pressão sobre o treinador e risco de demissão em caso de não cumprimento de metas;
  * **Torcida:** Taxa de ocupação do estádio e paciência em partidas dentro de casa;
  * **Imprensa:** Repercussão das notícias, nível de cobrança e manchetes jornalísticas;
  * **Mercado:** Atratividade do clube para atrair reforços de peso e aceitação de propostas por atletas;
  * **Permanência do Treinador:** Facilidade para renovação contratual ou propostas de extensão;
  * **Comportamento Futuro da IA:** Postura dos clubes controlados por IA na janela de transferências (ex: busca por reforços imediatos vs. reestruturação financeira).

#### 14.4 Sugestões de Evolução Futura
* Reuniões de alinhamento com a diretoria na metade da temporada para ajuste de expectativas em caso de lesões em massa ou vendas inesperadas.

---

## 3. RESUMO DA MATRIZ DE RESPONSABILIDADES

| Módulo / Funcionalidade | Responsável no iMLM | Sincronizado para o PES 2021? |
| :--- | :--- | :--- |
| **Carreira & Histórico do Treinador** | iMLM PES | Não (Exclusivo do iMLM) |
| **Evolução & Potencial de Atletas** | iMLM PES | **Sim** (Atributos regravados no EDIT) |
| **Mercado & Transferências** | iMLM PES | **Sim** (Elencos regravados no EDIT) |
| **Finanças, Patrocínios & Orçamento**| iMLM PES | Não (Exclusivo do iMLM) |
| **Categorias de Base & Regens** | iMLM PES | **Sim** (Promovidos entram no EDIT) |
| **Execução das Partidas (Humano)** | PES 2021 | **Sim** (Motor gráfico/jogabilidade) |
| **Simulação de Partidas (IA)** | iMLM PES | Não (Resultados gerados via algoritmo) |
| **Notícias & Jornalismo** | iMLM PES | Não (Exclusivo do iMLM) |
| **Estatísticas & Hall da Fama** | iMLM PES | Não (Exclusivo do iMLM) |

---

## 4. CONCLUSÃO

O presente **Escopo Funcional** consolida todas as exigências do projeto **iMLM PES**, atuando como o guia definitivo para o desenvolvimento do software. Ele garante a expansão contínua do simulador em um ambiente estável, sustentável e fiel à **Constituição do Universo**.
