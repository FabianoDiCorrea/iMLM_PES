# PROPOSTA DE ARQUITETURA E REFATORAÇÃO DA PesEditLib (iPESMLManager)

**Análise Estrutural e Recomendações para Evolução da API**

---

## 1. ANÁLISE DA ESTRUTURA ATUAL

Atualmente, a biblioteca `PesEditLib` está organizada da seguinte forma:

```text
PesEditLib/
├── EditDatabase.h / .cpp    -> Fachada de carregamento e salvamento binário.
├── PlayerManager.h / .cpp   -> Gerenciador e vetor de Jogadores.
├── TeamManager.h / .cpp     -> Gerenciador e vetor de Clubes e Elencos.
├── TransferManager.h / .cpp -> Orquestrador de transferências entre clubes.
├── BinaryIO.h / .cpp        -> Leitura e Escrita bit-a-bit de baixo nível.
├── pes_crypt.h / .cpp       -> Criptografia/Descriptografia do EDIT.
└── mt19937ar.h / .cpp       -> Mersenne Twister Stream Cipher.
```

---

## 2. PONTOS DE MELHORIA E OPORTUNIDADES DE REFATORAÇÃO

### 1. Padronização de Nomenclatura em Português (PT-BR)
* **Motivo:** O projeto Master League Manager adotou o Português como padrão oficial. A `PesEditLib` ainda utiliza mescla de Inglês (`PlayerManager`, `Get`, `Load`) e C-Style (`parse_player_entry`).
* **Sugestão:** Renomear as classes e métodos da API para Português (ex: `GerenciadorJogadores`, `Carregar`, `Salvar`, `AtualizarAtributos`).

### 2. Separação de Módulos e Namespaces Claros
* **Motivo:** Atualmente, funções globais e classes utilitárias como `read_data` e `write_data` estão expostas diretamente.
* **Sugestão:** Criar um namespace raiz `MLM::Pes` ou `MLM::Binario` para encapsular todas as operações de baixo nível de criptografia e I/O de bits.

### 3. Abstração do C-Style Pointer para Smart Pointers e Coleções Seguras
* **Motivo:** O retorno de métodos como `PlayerManager::Get` entrega ponteiros brutos `Player*`, o que pode causar *dangling pointers* se o vetor for realocado.
* **Sugestão:** Utilizar `std::shared_ptr<JogadorBinario>` ou referências constantes seguras.

### 4. Criação de uma Fachada Única da Biblioteca (`InterfacePesEdit`)
* **Motivo:** O usuário da biblioteca precisa acessar sub-gerenciadores `db.Players().UpdatePlayer(...)` e `db.Transfers().ExecuteTransfer(...)`.
* **Sugestão:** Criar uma classe fachada `EditorBinarioPes` que expõe métodos de alto nível em Português:
  - `EditorBinarioPes::CarregarArquivo(caminho)`
  - `EditorBinarioPes::TransferirJogador(jogadorId, clubeOrigemId, clubeDestinoId)`
  - `EditorBinarioPes::EvoluirJogador(jogadorId, novosAtributos)`
  - `EditorBinarioPes::SalvarArquivo(caminho)`

---

## 3. CONCLUSAO DA ANÁLISE

Essas refatorações deixarão a biblioteca 100% alinhada com o padrão DDD e a arquitetura em Português adotada no **Master League Manager**, transformando o iPESMLManager em uma API fluida, elegante e imune a erros de baixo nível.
