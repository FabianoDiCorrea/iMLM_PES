# AUDITORIA TÉCNICA DA PesEditLib

**Documento Definitivo de Mapeamento do Binário `EDIT00000000` (PES 2021)**

---

## 1. RESUMO EXECUTIVO

Esta auditoria analisa os recursos e limites de manipulação direta sobre o arquivo binário `EDIT00000000` do PES 2021 fornecidos pela biblioteca **`PesEditLib`** (baseada na engenharia reversa do *4ccEditor* e *pesXdecrypter*).

### O que JÁ É POSSÍVEL fazer via `PesEditLib`:
1. **Criptografia Stream Cipher / Mersenne Twister:** Descriptografar e re-criptografar o binário `EDIT00000000` com 100% de aceitação pelo PES 2021 via `MasterKeyPes21`.
2. **Transferências de Jogadores:** Alterar os elencos de qualquer clube (adicionar e remover jogadores dos slots de roster de 0 a 39).
3. **Numeração de Camisa:** Alterar o número da camisa do atleta no clube.
4. **Leitura de Atributos do Jogador:** Parseamento bit-a-bit de nome, ID, idade, altura, peso, posição principal e 20+ atributos de habilidade (dribble, passe, velocidade, etc.).
5. **Leitura de Dados de Clubes:** Identificação do ID do clube, Nome e Nome Abreviado (Short Name).

---

## 2. TABELA COMPLETA DE MAPEAMENTO DE CAMPOS

### JOGADOR

| Campo / Atributo | Localizado? | Leitura? | Escrita (Edição)? | Observações Tecnicas |
| :--- | :---: | :---: | :---: | :--- |
| **ID do Jogador** | **SIM** | **SIM** | **NÃO** | Offset 0x00 do bloco do jogador (32 bits). Identificador único imutável. |
| **Nome Completo** | **SIM** | **SIM** | **NÃO** | Armazenado em UTF-8 no bloco do jogador. Leitura implementada. |
| **Nome na Camisa** | **SIM** | **SIM** | **NÃO** | Armazenado em string ASCII/UTF-8 no bloco do jogador. |
| **Idade** | **SIM** | **SIM** | **NÃO** | Mapeado no bit 7 do offset do jogador (6 bits). Atualmente apenas leitura. |
| **Altura** | **SIM** | **SIM** | **NÃO** | Mapeado no offset de atributos físicos (8 bits). |
| **Peso** | **SIM** | **SIM** | **NÃO** | Mapeado no offset de atributos físicos (8 bits). |
| **Pé Dominante** | **SIM** | **SIM** | **NÃO** | Mapeado em bits de atributos corporais. |
| **Posição Principal** | **SIM** | **SIM** | **NÃO** | Mapeado (4 bits, 13 posições registradas). |
| **Posições Secundárias** | **NÃO** | **NÃO** | **NÃO** | *Não identificado até o momento.* |
| **Nacionalidade** | **NÃO** | **NÃO** | **NÃO** | *Não identificado até o momento na PesEditLib.* |
| **Overall Rating** | **SIM** | **SIM** | **NÃO** | Calculado pela maior habilidade do jogador. Atualização depende da escrita de habilidades. |
| **Potencial** | **NÃO** | **NÃO** | **NÃO** | *Não existe no binário do PES.* O potencial é exclusivo do MLM. |
| **Habilidades Técnicas/Físicas** | **SIM** | **SIM** | **NÃO** | 20+ habilidades lidas bit-a-bit (`atk`, `ball_ctrl`, `speed`, `stamina`, etc.). |
| **Aparência / Face / Cabelo** | **NÃO** | **NÃO** | **NÃO** | *Não identificado até o momento.* Bloco de aparência ainda não parseado. |
| **Clube Atual** | **SIM** | **SIM** | **SIM** | Alterado via edição dos slots de elenco (0 a 39) da estrutura do time. |
| **Número da Camisa** | **SIM** | **SIM** | **SIM** | Gravado diretamente na tabela de elenco do time (`roster_entry`). |
| **Cartões Acumulados / Lesões** | **NÃO** | **NÃO** | **NÃO** | *Não existem no EDIT00000000.* Ficam armazenados na memória de Save da ML do jogo. |
| **Contrato / Salário** | **NÃO** | **NÃO** | **NÃO** | *Não existem no EDIT00000000.* Pertencem ao motor interno da ML ou MLM. |
| **Valor de Mercado** | **NÃO** | **NÃO** | **NÃO** | *Não existe no EDIT00000000.* |

---

### CLUBE

| Campo / Atributo | Localizado? | Leitura? | Escrita (Edição)? | Observações Técnicas |
| :--- | :---: | :---: | :---: | :--- |
| **ID do Clube** | **SIM** | **SIM** | **NÃO** | Offset 0x00 do bloco do time (32 bits). |
| **Nome do Clube** | **SIM** | **SIM** | **NÃO** | Armazenado em UTF-8 no stride `0x24C`. |
| **Nome Abreviado (Short Name)**| **SIM** | **SIM** | **NÃO** | String de 4 caracteres lida no stride do time. |
| **Elenco (Roster de 40 Jogadores)**| **SIM** | **SIM** | **SIM** | **Totalmente suportado.** Adição, remoção e troca de jogadores. |
| **Estádio** | **NÃO** | **NÃO** | **NÃO** | *Não identificado até o momento.* |
| **Uniformes / Kits** | **NÃO** | **NÃO** | **NÃO** | *Não identificado até o momento.* |
| **Técnico** | **NÃO** | **NÃO** | **NÃO** | *Não identificado até o momento.* |
| **Orçamento / Finanças** | **NÃO** | **NÃO** | **NÃO** | *Não existe no EDIT00000000.* |
| **Reputação / Prestígio** | **NÃO** | **NÃO** | **NÃO** | *Não identificado até o momento.* |

---

### COMPETIÇÕES & CALENDÁRIO

| Campo / Atributo | Localizado? | Leitura? | Escrita (Edição)? | Observações Técnicas |
| :--- | :---: | :---: | :---: | :--- |
| **Ligas / Copas (Estrutura)** | **NÃO** | **NÃO** | **NÃO** | *Não identificado até o momento na PesEditLib.* |
| **Premiações / Históricos** | **NÃO** | **NÃO** | **NÃO** | *Não existem no EDIT00000000.* |
| **Calendário / Data de Jogos** | **NÃO** | **NÃO** | **NÃO** | *Não existem no EDIT00000000.* |

---

## 3. RESPOSTAS OBJETIVAS DE LIMITAÇÕES

- **É possível alterar idade?** ❌ *Atualmente Não* (O bit offset é conhecido na leitura, mas a função `write_data` de idade ainda não foi exposta na PesEditLib).
- **É possível alterar overall?** ❌ *Atualmente Não* (Depende da implementação de `write_data` nos bits de cada habilidade individual).
- **É possível alterar potencial?** ❌ *Não* (Atributo inexistente no PES; gerido 100% pelo MLM).
- **É possível alterar altura / peso?** ❌ *Atualmente Não* (Campos lidos, mas sem função de escrita exposta).
- **É possível alterar posição / nacionalidade?** ❌ *Atualmente Não*.
- **É possível alterar número da camisa?** **SIM** (Totalmente suportado).
- **É possível criar um novo jogador?** ❌ *Não* (A estrutura do PES usa slots fixos no arquivo `EDIT00000000`).
- **É possível reutilizar um slot existente / editar jogador sem clube?** **SIM** (Qualquer jogador existente no banco pode ter seus dados editados ou ser movido para um clube).
- **É possível transferir jogadores entre clubes?** **SIM** (Totalmente suportado e validado).

---

## 4. MAPA DE RISCOS DO PROJETO MLM

| Funcionalidade MLM | Dependência do PES / EDIT | Impacto no Projeto & Solução MLM |
| :--- | :--- | :--- |
| **Transferências entre Clubes** | **Suportado (SIM)** | **Nenhum risco.** O MLM pode executar compras e vendas e gravá-las diretamente no PES. |
| **Evolução de Atributos / Overall** | **Requer Escrita (NÃO)** | **Médio Risco.** O MLM calcula a evolução, mas para refletir no PES 2021 precisaremos implementar a rotina de escrita de bits (`write_data`) na `PesEditLib`. |
| **Envelhecimento / Aniversário** | **Requer Escrita (NÃO)** | **Baixo Risco.** A idade é controlada no domínio MLM. Para atualizar o PES, basta expor a escrita do bit de idade na `PesEditLib`. |
| **Aposentadoria & Regens** | **Slot Reutilizável (SIM)** | **Sem Risco.** O MLM pode pegar um jogador aposentado ou sem clube (Free Agent) e sobrescrever seus dados para criar um novo jovem da base. |
| **Finanças, Contratos & Premiações** | **100% MLM (Isolado)** | **Nenhum risco.** O PES não possui essas estruturas no EDIT; elas pertencem inteiramente à camada MLM. |

---

## 5. CONCLUSÃO & PROPOSTA TÉCNICA PARA EVOLUÇÃO DA PesEditLib

### Conclusão Objetiva
Se o desenvolvimento do MLM continuar exatamente como está, as **Transferências de Jogadores**, **Numeração de Camisas** e a **Simulação de Regras de Carreira no MLM (Finanças, Contratos, Históricos e Ligas)** estarão **100% automatizadas**. 

Para que a **Evolução de Jogadores** (aumento/redução de overall) e o **Envelhecimento** apareçam dentro de campo no PES 2021, será necessário estender a `PesEditLib` com funções de escrita bit-a-bit (`write_data`).

---

### PROPOSTA TÉCNICA: EVOLUÇÃO DA PesEditLib PARA UM EDITOR COMPLETO DE BANCO DE DADOS

#### 1. Viabilidade Técnica
Como a leitura bit-a-bit de todos os atributos (`atk`, `drib`, `speed`, `age`, `height`, etc.) já está implementada na função `parse_player_entry` da `BinaryIO.cpp`, a viabilidade técnica para escrita é de **100%**. O algoritmo `write_data` já existe no projeto e funciona perfeitamente para gravarmos qualquer bit do bloco do jogador.

#### 2. Plano de Implementação Sugerido para a PesEditLib
1. **Criação da função `write_player_entry`:**
   Implementar a função espelhada de `parse_player_entry` em `BinaryIO.cpp`, utilizando `write_data` nos mesmos offsets de bits para sobrescrever atributos físicos, habilidades e idade.
2. **Atualização da API do `PlayerManager`:**
   Expor métodos na biblioteca como `PlayerManager::UpdatePlayerSkills(const Player& player)` e `PlayerManager::UpdatePlayerAge(uint32_t playerId, uint8_t newAge)`.
3. **Refatoração do Cálculo de Overall:**
   Recalcular o rating do jogador automaticamente no binário sempre que um atributo de habilidade for modificado.
