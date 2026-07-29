# Análise Técnica do 4ccEditor

## 1. Estrutura do Projeto

O **4ccEditor** é uma aplicação desktop nativa desenvolvida em C++ para Windows usando a API Win32 clássica e DLLs de descriptografia nativas da franquia Pro Evolution Soccer (PES 2015 a PES 2021).

### Árvore de Arquivos e Pastas

```
4ccEditor/
├── main.cpp                 # Entrypoint Win32 (_tWinMain), WndProc, inicialização de DLLs, data_handler e save_handler
├── editor.h                 # Declaração das estruturas principais (player_export, player_entry, team_entry, stripSet) e protótipos de funções por versão
├── window.h & window.cpp    # Layout da interface gráfica Win32 (criação de abas, formulários, controles da GUI)
├── crypt.h & crypt.cpp      # Declaração das estruturas de cabeçalho do arquivo EDIT (FileDescriptor15/Old/New) e ponteiros de função das DLLs de criptografia
├── data_util.cpp            # Utilitários de leitura/escrita bit a bit (read_data, write_data)
├── pes15.cpp                # Funções de leitura/escrita do formato de EDIT do PES 2015
├── pes16.cpp                # Funções de leitura/escrita do formato de EDIT do PES 2016
├── pes17.cpp                # Funções de leitura/escrita do formato de EDIT do PES 2017
├── pes18.cpp                # Funções de leitura/escrita do formato de EDIT do PES 2018
├── pes19.cpp                # Funções de leitura/escrita do formato de EDIT do PES 2019
├── pes20.cpp                # Funções de leitura/escrita do formato de EDIT do PES 2020 e PES 2021
├── aatf.h & aatf.cpp        # Verificador de regras de elenco e torneio 4chan Cup (Auto-AATF)
├── comparator.cpp           # Comparador entre dois arquivos de save EDIT
├── fpc.cpp                  # Utilitários FPC (First Player Created / ocultação de jogadores)
├── menu_lists.cpp           # Vetores de texto/arrays de posições e estilos de jogo
├── resource.h & resource.rc # Recursos da aplicação (IDs de menus, botões, diálogos da GUI)
├── 4ccEditor.vcxproj        # Arquivo de projeto do Visual Studio
└── lib/ (carregadas dinamicamente via LoadLibrary em runtime):
    ├── libpesXcrypter.dll   # DLL de descriptografia/criptografia (PES 2016 a 2021)
    └── libpes15crypter.dll  # DLL de descriptografia/criptografia (PES 2015)
```

---

## 2. Fluxo de Execução

1. **Inicialização (`_tWinMain`)**:
   - Registra a classe de janela Win32 `4cc_app`.
   - Executa `loadDLL()` que carrega via `LoadLibrary` as DLLs `libpesXcrypter.dll` e `libpes15crypter.dll` e mapeia os ponteiros de função para criação, destruição, descriptografia e criptografia de descritores de arquivo.
   - Cria a janela principal e inicializa os controles e abas (`setup_main`, `setup_tab1`, `setup_tab2`, `setup_tab3`).

2. **Abertura do Arquivo EDIT (`DoFileOpen`)**:
   - O usuário escolhe a versão do PES no menu (`ID_FILE_OPEN_15_EN` até `ID_FILE_OPEN_21_EN`).
   - Abre uma caixa de diálogo nativa de seleção de arquivo (`GetOpenFileName`).
   - Chama `data_handler(path, pesVersion)`.

3. **Descriptografia e Parsing (`data_handler`)**:
   - Aloca o descritor correspondente (`FileDescriptor15`, `FileDescriptorOld` ou `FileDescriptorNew`).
   - Carrega a Chave Mestra (MasterKey) correspondente à versão do PES (ex: `MasterKeyPes21`).
   - Invoca a função da DLL (`decryptFile15`, `decryptWithKeyOld` ou `decryptWithKeyNew`). A DLL descriptografa os dados e popula o ponteiro `descriptor->data`.
   - Lê o número total de jogadores (`gnum_players`) e times (`gnum_teams`) a partir de offsets do cabeçalho de dados.
   - Instancia dinamicamente os arrays `gplayers = new player_entry[gnum_players]` e `gteams = new team_entry[gnum_teams]`.
   - Percorre o buffer binário descompactado invocando as funções específicas da versão (`fill_player_entryXX`, `fill_team_idsXX`, `fill_team_rostersXX`, `fill_team_tacticsXX`).
   - Associa cada jogador ao seu time correspondente preenchendo os campos `team_ind` e `team_lineup_ind`.
   - Popula a interface Win32 (comboboxes de times e listbox/listview de jogadores).

4. **Edição na Interface**:
   - O usuário altera atributos do jogador ou do clube na GUI. Os dados alterados marcam a flag `b_changed = true` no objeto da memória.

5. **Salvamento (`DoFileSave` e `save_handler`)**:
   - Abre a caixa de diálogo de salvamento (`GetSaveFileName`).
   - Cria automaticamente um backup (`.BACKUP`).
   - Chama as funções de extração (`extract_player_entryXX`, `extract_team_infoXX`, `extract_teamplayer_infoXX`, `extract_team_tacticsXX`) para reescrever os dados modificados no buffer `descriptor->data` usando manipulação de bits (`write_data`).
   - Invoca a função de criptografia da DLL (`encryptWithKeyOld`, `encryptWithKeyNew` ou `encryptFile15`).
   - Grava o buffer final criptografado no disco usando `writeFile()`.

---

## 3. Principais Classes e Estruturas

Embora o código utilize estilo procedural em C++, ele organiza os dados em estruturas centrais:

### `struct player_export` (editor.h)
Estrutura base com os atributos puros do jogador exportável/importável:
- Atributos físicos (altura, peso, idade, tom de pele, etc.).
- Stats de habilidade (ataque, defesa, passe curto, passe longo, finalização, curva, velocidade, resistência, força de chute, etc.).
- Estilos de jogo (`play_style`), movimentos/animações e habilidades especiais (`play_skill[41]`).
- Nome (`name[61]`) e Nome na Camisa (`shirt_name[21]`).

### `struct player_entry` (editor.h - herda de `player_export`)
Estrutura completa do jogador na memória do editor:
- `unsigned long id`: ID único do jogador no jogo PES.
- `unsigned long app_id`: ID de aparência.
- `int team_ind`: Índice do time ao qual o jogador pertence no array global `gteams`.
- `int team_lineup_ind`: Slot do jogador na escalação/elenco do clube (0 a 39).
- `bool b_changed`: Flag de alteração para evitar reescrita desnecessária.
- `bool b_show`: Flag de visibilidade na interface.

### `struct team_entry` (editor.h)
Estrutura do clube/time:
- `unsigned long id`: ID único do time.
- `unsigned long manager_id`: ID do treinador.
- `int stadium_id`: ID do estádio.
- `wchar_t name[0x46]`: Nome do time (Unicode).
- `char short_name[0x4]`: Sigla/Nome curto (ex: "BAR").
- `unsigned long players[40]`: **Array com os IDs dos 40 jogadores do elenco**.
- `uint16_t numbers[40]`: Números das camisas dos jogadores.
- `int starting11[11]`: Índices dos 11 titulares.
- `char captain_ind`: Índice do capitão do time.
- `stripSet stripBlock[10]`: Informações de uniformes/kits.

### Structures de Crypt (crypt.h)
- `FileHeaderNew` / `FileHeaderOld`: Estruturas dos cabeçalhos do arquivo EDIT descompactado.
- `FileDescriptorNew` / `FileDescriptorOld` / `FileDescriptor15`: Ponteiros para as seções do arquivo descompactado (`encryptionHeader`, `fileHeader`, `description`, `logo`, `data`, `serial`).

---

## 4. Principais Funções

| Função | Arquivo | Responsabilidade |
| :--- | :--- | :--- |
| `_tWinMain` | `main.cpp` | Ponto de entrada do aplicativo Win32, cria a janela e executa o loop de mensagens. |
| `loadDLL` | `main.cpp` | Carrega dinamicamente via Win32 API (`LoadLibrary`/`GetProcAddress`) as DLLs de criptografia. |
| `DoFileOpen` | `main.cpp` | Abre a janela de seleção de arquivo e coordena a chamada ao `data_handler`. |
| `data_handler` | `main.cpp` | Gerencia o ciclo de descriptografia, alocação de memória e carga dos dados nas estruturas. |
| `DoFileSave` | `main.cpp` | Gerencia o salvamento e criação de backup do arquivo EDIT. |
| `save_handler` | `main.cpp` | Grava os objetos alterados de volta no buffer e invoca a criptografia da DLL antes de gravar em disco. |
| `read_data` / `read_dataOld` | `data_util.cpp` | Lê $N$ bits de uma posição específica no buffer binário deslocando ponteiros de byte e aplicando máscaras. |
| `write_data` / `write_dataOld` | `data_util.cpp` | Escreve $N$ bits no buffer binário aplicando operações bitwise AND/OR. |
| `fill_player_entryXX` | `pesXX.cpp` | Converte os bits do buffer binário descompactado nos campos da estrutura `player_entry`. |
| `fill_team_idsXX` | `pesXX.cpp` | Converte os bits do buffer nos campos básicos da estrutura `team_entry` (IDs, nomes, cores). |
| `fill_team_rostersXX` | `pesXX.cpp` | Lê os 40 IDs de jogadores e 40 números de camisa de cada time. |
| `extract_player_entryXX` | `pesXX.cpp` | Empacota os campos da estrutura `player_entry` de volta para os bits do buffer binário. |
| `extract_team_infoXX` / `extract_teamplayer_infoXX` | `pesXX.cpp` | Empacota os dados do clube e elenco de volta para o buffer binário. |

---

## 5. Como o Arquivo EDIT é Processado

### Como é Aberto e Descriptografado
1. O arquivo `EDIT00000000` é lido do disco para a memória através da função `readFile()` em `crypt.cpp` que executa `_tfopen` em modo binário (`"rb"`), descobre o tamanho com `_tstat` e faz a leitura para um ponteiro de bytes `uint8_t*`.
2. Em seguida, a função `data_handler` instancia um descritor de arquivo (`FileDescriptorNew*` para PES 18-21, `FileDescriptorOld*` para PES 16-17 ou `FileDescriptor15*` para PES 15).
3. A chave de descriptografia de 32 bytes (ex: `MasterKeyPes21`) é obtida da DLL `libpesXcrypter.dll` via `GetProcAddress`.
4. É executada a função `decryptWithKeyNew` (da DLL externa), que desfaz a criptografia do PES e separa o arquivo em blocos (`encryptionHeader`, `fileHeader`, `data`, etc.). O buffer descriptografado útil fica acessível através do ponteiro `pDescriptor->data`.

### Como os Dados são Carregados
1. No início do buffer `data`, nos offsets `0x60` / `0x61` (ou equivalentes dependendo da versão), o 4ccEditor lê a quantia total de jogadores (`gnum_players`) e times (`gnum_teams`).
2. O editor executa o loop nos blocos de jogadores (em PES 20/21 a tabela de jogadores começa no offset `0x7C`):
   - Cada jogador ocupa um tamanho fixo de bytes.
   - Como o PES armazena atributos em larguras de bits não alinhadas (ex: estatísticas usam 7 bits, estilos usam 5 bits, flags usam 1 bit), a função `read_data()` é chamada sequencialmente extraindo bit a bit os valores para a struct `player_entry`.
3. Em seguida, lê a tabela de dados dos times (`fill_team_idsXX`) e a **tabela de elencos** (`fill_team_rostersXX`):
   - A tabela de elencos lê 40 IDs de jogadores de 32 bits (4 bytes cada) para cada clube.
4. Por fim, o editor faz o cruzamento de dados: percorre todos os jogadores lidos e procura se o seu `id` está em algum dos elencos (`gteams[j].players[k]`). Se encontrar, grava `player.team_ind = j` e `player.team_lineup_ind = k`.

### Como os Dados são Gravados Novamente
1. Durante a edição, os campos editados pelo usuário são atribuídos à struct do jogador/time e a flag `b_changed` é ativada.
2. Na chamada de salvamento (`save_handler`), se `b_changed` for `true`, as funções de extração (ex: `extract_player_entry20`) percorrem os campos da struct e invocam `write_data()`, que grava os valores diretamente no buffer `pDescriptor->data` respeitando o mapa exato de bits de cada versão.
3. O buffer modificado `pDescriptor` é passado para a função `encryptWithKeyNew` (ou equivalente) da DLL externa.
4. A DLL retorna um buffer final criptografado `output` e o seu tamanho `outputSize`.
5. A função `writeFile()` em `crypt.cpp` grava os bytes no arquivo de destino usando `fwrite` em modo binário (`"wb"`).

---

## 6. Relação entre Jogador e Clube (Jogador -> Clube)

### Estrutura Identificada

A relação **Jogador -> Clube** é mantida no 4ccEditor através de duas estruturas que se conectam por IDs:

1. **Na Estrutura do Clube (`team_entry` em `editor.h`)**:
   ```cpp
   struct team_entry
   {
       unsigned long id; // ID do Time
       ...
       unsigned long players[40]; // Array fixo de 40 posições contendo os IDs dos Jogadores
       uint16_t numbers[40];      // Números das camisas correspondentes
       ...
   };
   ```

2. **Na Estrutura do Jogador (`player_entry` em `editor.h`)**:
   ```cpp
   struct player_entry : player_export
   {
       unsigned long id; // ID único do Jogador
       ...
       int team_ind;         // Índice do time no array global 'gteams' (-1 se sem clube)
       int team_lineup_ind;  // Posição no elenco (0 a 39 no array 'players')
   };
   ```

### Como a Relação Funciona no Arquivo Binário do EDIT

No arquivo binário do PES (bloco de rosters/elencos):
- O bloco de rosters possui uma entrada para cada time.
- Cada entrada do time no bloco de roster começa com o `team_id` (4 bytes).
- Logo após o `team_id`, seguem **40 inteiros de 32 bits (4 bytes cada)** contendo os IDs dos jogadores alocados naquele clube (`players[40]`). Se o slot estiver vazio, o ID é `0`.
- No carregamento, o 4ccEditor lê essa tabela de 40 IDs para cada time (`fill_team_rostersXX`) e faz o mapeamento inverso gravando em cada jogador o índice do seu time (`gplayers[i].team_ind = index_do_time`).

---

## 7. Módulos / Arquivos Necessários para Portar para C#

Para criar uma versão equivalente ou biblioteca em **C#**, os seguintes componentes e arquivos precisavam ser portados/recriados:

1. **Lógica de Leitura/Escrita Binária Bitwise (`data_util.cpp`)**:
   - Criar uma classe utilitária em C# (ex: `BitReader` / `BitWriter` ou usar `BinaryReader`/`BinaryWriter` estendido para manipular offsets arbitrários de bits).

2. **Estruturas de Dados (`editor.h`)**:
   - Portar as structs/classes `PlayerExport`, `PlayerEntry` e `TeamEntry` para C# (POCOs/Classes com propriedades auto-implementadas).

3. **Mapeamento/Parsing de Versões (`pes15.cpp` a `pes20.cpp`)**:
   - Portar a lógica das funções `fill_*` e `extract_*` de cada versão do PES para classes de Parser/Serializer em C# (ex: `Pes21Parser.cs`, `Pes18Parser.cs`, etc.).

4. **Integração com as DLLs de Criptografia (`crypt.h`, `crypt.cpp`)**:
   - Como a criptografia original é realizada pelas DLLs `libpesXcrypter.dll` e `libpes15crypter.dll`, em C# será necessário fazer P/Invoke (`[DllImport]`) para chamar as funções expostas por essas DLLs nativas ou reimplementar os algoritmos de cifra/hash em C# gerenciado caso a chave/algoritmo seja conhecido.
