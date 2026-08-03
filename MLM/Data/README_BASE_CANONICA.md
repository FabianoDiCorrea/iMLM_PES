# Base canônica iMLM PES

Este diretório guarda a base inicial do universo iMLM extraída do `EDIT00000000`.

## Arquivo principal

- `imlm_base_pes21.json`

Esta base representa o ponto zero do mundo:

- jogadores únicos;
- atributos técnicos completos;
- dados físicos;
- posições jogáveis;
- habilidades e estilos;
- clubes;
- elencos;
- overall registrado;
- overall por posição.

## Regra de arquitetura

A base canônica não é o save da carreira.

Ela serve para criar novos universos do iMLM, por exemplo:

- save solo;
- save com amigo;
- save de teste;
- save de temporada alternativa.

Cada save deve nascer como uma cópia lógica desta base, e depois evoluir de forma independente.

## Fluxo desejado

```text
EDIT original do PES
        ↓
ExportarBancoPes.exe
        ↓
imlm_base_pes21.json
        ↓
Novo save iMLM
        ↓
Transferências, contratos, evolução, idade, calendário
        ↓
EDIT temporário daquele save
        ↓
PES 2021 joga as partidas humanas
```

## Regra para múltiplos saves

O iMLM deve tratar cada save como um universo isolado.

Exemplo:

```text
MLM/Saves/
├── solo/
│   ├── universo.json
│   └── EDIT00000000
└── coop-amigo/
    ├── universo.json
    └── EDIT00000000
```

Ao carregar um save, o iMLM deve:

1. carregar o `universo.json` daquele save;
2. gerar ou restaurar o `EDIT00000000` correspondente;
3. substituir temporariamente o EDIT ativo do PES;
4. abrir o PES apenas como motor de partida;
5. importar resultados e voltar o controle ao iMLM.

## Sobre overall

`overallPesJogo` é o número usado pelo iMLM para mercado, simulação, elenco e evolução.

Ele não deve ser ajustado jogador por jogador.

O ajuste correto é calibrar a fórmula global do motor de overall contra amostras reais do PES. As amostras ficam em:

- `MLM/Tools/overall_targets.json`

E são verificadas por:

- `MLM/Tools/ValidarOverallsPes.ps1`
