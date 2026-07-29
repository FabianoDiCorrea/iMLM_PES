# Análise do Fluxo de Transferência (Jogador -> Clube) no 4ccEditor

## 1. Qual estrutura representa um jogador?

O jogador é representado por duas estruturas encadeadas em `editor.h`:

### `struct player_export` (editor.h)
Contém os atributos legíveis e exportáveis do jogador:
- Dados pessoais (nome `name[61]`, nome de camisa `shirt_name[21]`, nacionalidade, idade, altura, peso).
- Habilidades e atributos técnicos (ataque, defesa, curva, passe, etc.).
- Aparência, movimentos, chuteira e luva.

### `struct player_entry` (editor.h - herda de `player_export`)
Adiciona metadados de runtime e indexação:
- `unsigned long id`: ID único do jogador no banco de dados do PES.
- `unsigned long app_id`: ID de aparência.
- `int team_ind`: **Índice do clube atual** no array global de times `gteams`.
- `int team_lineup_ind`: **Slot (posição)** do jogador dentro do elenco de 40 jogadores do clube.
- `bool b_changed`: Indicação de alteração pendente de salvamento.
- `bool b_show`: Indicação de visibilidade do jogador na interface.

---

## 2. Onde está o Team ID?

O **Team ID** (ID único de 32 bits que identifica o clube) não é gravado dentro do cabeçalho individual do jogador no arquivo binário. Em vez disso, a relação é armazenada na **estrutura do clube (`team_entry`)**:

```cpp
struct team_entry
{
    unsigned long id;             // <-- AQUI FICA O TEAM ID (32 bits / 4 bytes)
    unsigned long manager_id;
    int stadium_id;
    wchar_t name[0x46];
    char short_name[0x4];
    unsigned long players[40];    // <-- ARRAY COM OS IDs DOS 40 JOGADORES DO ELENCO
    uint16_t numbers[40];         // Números das camisas
    int starting11[11];           // Posições dos 11 titulares
    char captain_ind;             // Slot do capitão
    ...
};
```

---

## 3. Onde o editor altera o Team ID / Vínculo de Clube?

No 4ccEditor, o vínculo do jogador com o clube pode ser alterado de três formas principais no código:

### A. Troca de Posição/Elenco na Interface (Drag & Drop ou Move Button)
Em `main.cpp` (linhas 542–648 - caso `UDN_DELTAPOS`):
- Quando um jogador é movido na lista da interface gráfica (subir/descer na ordem do elenco):
- O código realiza a troca dos dados de atributos (`player_export`) mantendo os IDs fixos dos slots do clube, trocando efetivamente a identidade do jogador entre o slot $A$ e o slot $B$ (`gplayers[slotA].PlayerImport(exportB)`).

### B. Importação de Elenco Completo (`import_squad`)
Em `main.cpp` (função `import_squad`):
- Permite carregar um arquivo binário `.bin` com dados de um clube e sobrescrever os atributos e escalação dos 40 jogadores vinculados àquele `team_entry`.

### C. Mapeamento de Vínculo durante Carga e Exibição (`data_handler` e `show_player_info`)
Em `main.cpp` (funções `data_handler` nas linhas 1898–1924 e `show_player_info` nas linhas 2512–2576):
- Durante o carregamento do save, o 4ccEditor varre todos os times (`gteams[ii]`) e verifica se o ID do jogador (`gplayers[p_ind].id`) coincide com algum dos 40 IDs contidos em `gteams[ii].players[jj]`.
- Quando encontra a correspondência, atualiza em tempo de execução:
  - `gplayers[p_ind].team_ind = ii` (posição do time no array global `gteams`).
  - `gplayers[p_ind].team_lineup_ind = jj` (slot de 0 a 39 no elenco do clube).

---

## 4. Como essa alteração é salva no EDIT00000000?

Quando o salvamento é disparado (`DoFileSave` -> `save_handler` em `main.cpp`), a gravação da transferência/vínculo ocorre no **bloco de elencos (Rosters)** do arquivo `EDIT00000000`:

### Fluxo de Gravação Binária (`pes20.cpp` / `extract_teamplayer_info20`):

```cpp
void extract_teamplayer_info20(team_entry team, int &current_byte, void* ghdescriptor)
{
    FileDescriptorNew* pDescriptorNew = (FileDescriptorNew*)ghdescriptor;

    if(!team.b_changed)
    {
        current_byte += 0x11C; // Avança o ponteiro se não houver alterações
        return;
    }
    
    // 1. Escreve o ID do Clube (4 bytes / 32 bits)
    write_data(team.id, 0, 4*8, current_byte, pDescriptorNew);

    // 2. Escreve a lista dos 40 IDs de Jogadores do Elenco (40 x 4 bytes = 160 bytes)
    for(int ii = 0; ii < 40; ii++)
    {
        write_data(team.players[ii], 0, 4*8, current_byte, pDescriptorNew);
    }

    // 3. Escreve os números das camisas (40 x 2 bytes = 80 bytes)
    for(int ii = 0; ii < 40; ii++)
    {
        write_data(team.numbers[ii], 0, 2*8, current_byte, pDescriptorNew);
    }

    current_byte += 40; // Dados suplementares do bloco
}
```

### Passos finais do salvamento binário:
1. `extract_teamplayer_info20` grava a matriz `team.players[40]` atualizada com os IDs dos jogadores no buffer da memória `pDescriptorNew->data` nos offsets específicos de Rosters (ex: `0x9D4648` no PES 2021).
2. O buffer completo é enviado para a DLL `libpesXcrypter.dll` através da função `encryptWithKeyNew(...)`.
3. A DLL aplica a cifra e gera a assinatura de integridade Hash SHA-256 no cabeçalho `FileHeaderNew`.
4. O buffer criptografado final é salvo no disco via `writeFile()` sobrescrevendo o arquivo `EDIT00000000`.
