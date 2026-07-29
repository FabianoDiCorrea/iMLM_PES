# ARQUITETURA DE MÓDULOS E CAMADAS DO MASTER LEAGUE MANAGER (MLM)
**Estrutura do Núcleo e Regras de Comunicação entre Componentes**

---

## 1. VISÃO GERAL DA ESTRUTURA DE PASTAS

O sistema **MLM** foi projetado seguindo os princípios de **Clean Architecture** e **Domain-Driven Design (DDD)**, dividindo-se em módulos altamente desacoplados.

```text
MLM/
├── Core/             -> Tipos primitivos, utilitários globais, constantes e interfaces universais.
├── Domain/           -> As 20 Entidades de Negócio pura (sem dependência externa).
├── Services/         -> Serviços de Negócio e Casos de Uso (Transferências, Evolução, Finanças).
├── Persistence/      -> Contratos de repositórios e abstração de armazenamento.
├── Infrastructure/   -> Implementações concretas de I/O, adaptadores de integração e sistema.
├── Application/      -> Fachada principal do MLM, orquestração e pontos de entrada de API.
├── PesEditLib/       -> Biblioteca estritamente responsável pelo binário EDIT00000000.
└── Tests/            -> Suíte de testes unitários e de integração de cada módulo.
```

---

## 2. DETALHAMENTO DOS MÓDULOS, CLASSES E RESPONSABILIDADES

### 1. `MLM/Core/`
* **Responsabilidade:** Fornecer tipos universais, enumeradores, estruturas de resultado, geradores de ID e contratos base reutilizáveis por todo o sistema.
* **Classes / Arquivos:**
  * `Types.h` - Definições de enums (`Position`, `TransferStatus`, `InjurySeverity`, etc.).
  * `Result.h` - Template genérico de retorno de operações (`Result<T>`).
  * `DateTime.h` - Sistema de controle de tempo e calendário customizado.
  * `Guid.h` - Gerador de identificadores únicos para objetos de domínio.
* **Dependências:** Nenhuma (Módulo Base).
* **Permissão de Comunicação:** Pode ser importado por **todos** os outros módulos.

---

### 2. `MLM/Domain/`
* **Responsabilidade:** Encapsular os dados e as regras invariantes das 20 entidades do Master League Manager.
* **Classes / Arquivos:**
  * `Player.h / .cpp` - Entidade Jogador.
  * `Team.h / .cpp` - Entidade Clube.
  * `League.h / .cpp` - Entidade Liga.
  * `Cup.h / .cpp` - Entidade Copa.
  * `Competition.h / .cpp` - Entidade Competição.
  * `Season.h / .cpp` - Entidade Temporada.
  * `Contract.h / .cpp` - Entidade Contrato.
  * `Transfer.h / .cpp` - Entidade Transferência.
  * `Manager.h / .cpp` - Entidade Treinador.
  * `Nation.h / .cpp` - Entidade Nacionalidade/País.
  * `Stadium.h / .cpp` - Entidade Estádio.
  * `Award.h / .cpp` - Entidade Premiação.
  * `Finance.h / .cpp` - Entidade Finanças.
  * `YouthPlayer.h / .cpp` - Entidade Categoria de Base.
  * `Match.h / .cpp` - Entidade Partida.
  * `Calendar.h / .cpp` - Entidade Calendário.
  * `Injury.h / .cpp` - Entidade Lesão.
  * `Suspension.h / .cpp` - Entidade Suspensão.
  * `History.h / .cpp` - Entidade Log Histórico.
  * `Configuration.h / .cpp` - Entidade Configurações Globais.
* **Dependências:** Importa apenas `Core`.
* **Permissão de Comunicação:** É consumido por `Services`, `Persistence`, `Infrastructure` e `Application`. **NÃO** conhece `PesEditLib` nem banco de dados.

---

### 3. `MLM/Services/`
* **Responsabilidade:** Implementar a inteligência e os casos de uso do universo carreira (algoritmos de IA de mercado, cálculo de evolução, balanço financeiro, simulações).
* **Classes / Arquivos:**
  * `TransferService.h / .cpp` - Processamento de propostas, aceites de IA e negociações.
  * `DevelopmentService.h / .cpp` - Algoritmo de evolução/involução de atributos de jogadores.
  * `FinanceService.h / .cpp` - Gestão de caixa, pagamento de salários e receitas de bilheteria.
  * `MatchSimulationService.h / .cpp` - Processamento de resultados de partidas simuladas por IA.
  * `SeasonService.h / .cpp` - Transição anual de temporada e aplicação de virada de ano.
  * `YouthAcademyService.h / .cpp` - Geração de regens e promoção de promessas.
* **Dependências:** `Core` e `Domain`.
* **Permissão de Comunicação:** Consumido por `Application`. **NÃO** acessa arquivos ou binários diretamente.

---

### 4. `MLM/Persistence/`
* **Responsabilidade:** Definir os contratos das interfaces de Repositório (`IRepository`) para salvar e recuperar as entidades do MLM.
* **Classes / Arquivos:**
  * `IPlayerRepository.h` - Interface de persistência de Jogadores.
  * `ITeamRepository.h` - Interface de persistência de Clubes.
  * `ILeagueRepository.h` - Interface de persistência de Ligas/Competições.
  * `ISeasonRepository.h` - Interface de persistência de Temporadas.
  * `ISaveGameRepository.h` - Interface para salvar/carregar o Save completo do MLM.
* **Dependências:** `Core` e `Domain`.
* **Permissão de Comunicação:** Implementado por `Infrastructure` e consumido por `Services` e `Application`.

---

### 5. `MLM/Infrastructure/`
* **Responsabilidade:** Fornecer os adaptadores concretos de infraestrutura, incluindo a integração com a `PesEditLib`, I/O de disco e logging.
* **Classes / Arquivos:**
  * `PesEditAdapter.h / .cpp` - Adaptador que traduz ações do MLM (`TransferService`) para chamadas da `PesEditLib`.
  * `SaveGameAdapter.h / .cpp` - Implementação de gravação de arquivos de Save do MLM.
  * `Logger.h / .cpp` - Sistema centralizado de logs de execução.
* **Dependências:** `Core`, `Domain`, `Persistence` e `PesEditLib`.
* **Permissão de Comunicação:** Responsável por conectar o mundo exterior/binário ao MLM.

---

### 6. `MLM/Application/`
* **Responsabilidade:** Fachada única e ponto de entrada da aplicação (API de alto nível para a Interface do Usuário ou CLI).
* **Classes / Arquivos:**
  * `MLMEngine.h / .cpp` - Motor principal da aplicação (Init, LoadGame, SaveGame, AdvanceDay).
  * `CareerFacade.h / .cpp` - Fachada de acesso simplificado para as visões do usuário.
* **Dependências:** Todos os módulos (`Core`, `Domain`, `Services`, `Persistence`, `Infrastructure`).
* **Permissão de Comunicação:** É a única camada com quem a UI do sistema conversará.

---

### 7. `MLM/PesEditLib/`
* **Responsabilidade:** Biblioteca binária de baixo nível responsável estritamente por abrir, descriptografar, ler, modificar e gravar o binário `EDIT00000000` do PES 2021.
* **Classes / Arquivos:**
  * `EditDatabase.h / .cpp` - Facade de manipulação do binário.
  * `PlayerManager.h / .cpp` - Leitor binário de jogadores.
  * `TeamManager.h / .cpp` - Leitor binário de times.
  * `TransferManager.h / .cpp` - Escritor binário de transferências.
  * `BinaryIO.h / .cpp` - Leitura e escrita bitwise (`read_data`, `write_data`).
  * `pes_crypt.h / .cpp` - Criptografia do PES 2021 (`MasterKeyPes21`).
* **Dependências:** Nenhuma do MLM (Biblioteca isolada).
* **Permissão de Comunicação:** Conversa **apenas** com o módulo `Infrastructure/PesEditAdapter`.

---

### 8. `MLM/Tests/`
* **Responsabilidade:** Suíte de testes automatizados unitários e de integração para validar todas as regras do MLM e a integridade da `PesEditLib`.

---

## 3. REGRAS DE DEPENDÊNCIA E RESTRICÕES DE COMUNICAÇÃO

1. **A regra de ouro da Clean Architecture:** As camadas internas (`Domain`, `Core`) nunca dependem das camadas externas (`Infrastructure`, `PesEditLib`).
2. **Isolamento do PES:** Nenhuma classe das camadas `Domain`, `Services` ou `Application` possui permissão para incluir `#include "pes_crypt.h"` ou conhecer offsets binários (`0x138`, `0x24C`).
3. **Ponto Único de Contato Binário:** Apenas a classe `Infrastructure/PesEditAdapter` pode invocar a API da `PesEditLib`.

---

## 4. DIAGRAMA DE COMUNICAÇÃO ENTRE CAMADAS

```text
                                +---------------------------+
                                |      USER INTERFACE /     |
                                |       APPLICATION CLI     |
                                +---------------------------+
                                              │
                                              ▼
                                +---------------------------+
                                |    Application Layer      |
                                |      (MLMEngine)          |
                                +---------------------------+
                                     │                 │
            ┌────────────────────────┘                 └────────────────────────┐
            ▼                                                                   ▼
+-----------------------+                                           +-----------------------+
|    Services Layer     |                                           |  Persistence Layer    |
| (Regras do Carreira)  |                                           | (Interfaces Repo)     |
+-----------------------+                                           +-----------------------+
            │                                                                   │
            ▼                                                                   ▼
+-------------------------------------------------------------------------------------------+
|                                    Domain Layer                                           |
|                     (Player, Team, League, Season, Contract, etc.)                        |
+-------------------------------------------------------------------------------------------+
                                            ▲
                                            │
                                +---------------------------+
                                |     Infrastructure        |
                                |    (PesEditAdapter)       |
                                +---------------------------+
                                              │
                                              ▼
                                +---------------------------+
                                |        PesEditLib         |
                                | (API de Acesso ao EDIT)   |
                                +---------------------------+
                                              │
                                              ▼
                                +---------------------------+
                                |       EDIT00000000        |
                                |     (Binário do PES)      |
                                +---------------------------+
```
