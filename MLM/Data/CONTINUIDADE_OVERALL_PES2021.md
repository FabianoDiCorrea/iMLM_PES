# Pesquisa do Overall do PES 2021 - Continuidade

## Objetivo

Fazer o iMLM calcular exatamente o mesmo overall exibido pelo PES 2021 para todos os jogadores, usando os atributos lidos do arquivo `EDIT`, sem valores manuais por jogador e sem depender do PESDB.

O PESDB não é a fonte correta para este projeto porque o patch GOGOSZ altera jogadores, elencos e atributos. A fonte de verdade precisa ser o `EDIT` ativo do usuário e a fórmula nativa do executável usado pelo patch.

## Estado da pesquisa

A fórmula nativa foi localizada no executável e reproduzida. Os coeficientes não são mais uma regressão ou estimativa: foram extraídos das tabelas usadas pelo próprio PES 2021.

### Funções localizadas no executável

- Wrapper de cálculo do jogador: `PES2021.exe + 0x14E7A50`.
- Núcleo exato do cálculo: `PES2021.exe + 0x1E640A0`.
- Rotina observada durante a cópia/leitura do jogador: `PES2021.exe + 0xC6ED40`.

### Tabelas estáticas localizadas

- Códigos de posição: RVA `0x2B53BB0`.
- Índices dos atributos: RVA `0x2B53BF0`.
- Pesos por posição: RVA `0x2B53C50`.
- Bônus da posição principal: RVA `0x2B54100`.
- Bônus de familiaridade completa: RVA `0x2B54138`.
- Bônus de familiaridade baixa: RVA `0x2B54170`.
- Lista usada na média: RVA `0x2B541B0`.
- Curva não linear da média: RVA `0x2B54210`.

Esses RVAs pertencem ao executável testado. Antes de usar em outra versão do jogo ou patch, validar assinatura/hash e não assumir que os endereços continuam iguais.

## Ordem das posições

Ordem interna da fórmula:

```text
[GK, CB, RB, LB, DMF, CMF, RMF, LMF, AMF, RWF, LWF, SS, CF]
```

Códigos internos encontrados:

```text
[12, 9, 11, 10, 5, 6, 8, 7, 4, 3, 2, 1, 0]
```

Enum atualmente usado no repositório:

```text
GK=0, CB=1, LB=2, RB=3, DMF=4, CMF=5, LMF=6,
RMF=7, AMF=8, LWF=9, RWF=10, SS=11, CF=12
```

É obrigatório converter a enumeração do projeto para a ordem da fórmula antes de selecionar uma coluna de pesos.

## Vetor de entrada da fórmula

O cálculo usa 23 entradas, nesta ordem:

1. Offensive Awareness
2. Ball Control
3. Dribbling
4. Tight Possession
5. Low Pass
6. Lofted Pass
7. Finishing
8. Set Piece Taking
9. Curl
10. Heading
11. Defensive Awareness
12. Tackling / Ball Winning
13. Aggression
14. Kicking Power
15. Speed
16. Acceleration
17. Physical Contact
18. Balance
19. Jump
20. Média do par de atributos de goleiro
21. Média do trio de atributos de goleiro
22. Stamina
23. Weak Foot Usage

Agregações dos atributos de goleiro:

```text
gkPair   = floor((GK Awareness + GK Reach) / 2)
gkTriple = floor((GK Catching + GK Clearing + GK Reflexes) / 3)
```

O campo de pé fraco usado pela fórmula é a frequência de uso (`weak_use`), e não a precisão do pé fraco.

## Pesos inteiros exatos

Cada linha abaixo segue a ordem das 23 entradas descritas anteriormente.

```text
GK  [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,11,50,48,0,0]
CB  [0,0,0,0,0,0,0,0,0,22,25,17,8,0,10,0,19,0,19,0,0,9,23]
RB  [6,9,9,5,0,14,0,0,0,0,14,8,5,0,16,14,8,4,11,0,0,14,56]
LB  [6,9,9,5,0,14,0,0,0,0,14,8,5,0,16,14,8,4,11,0,0,14,56]
DMF [7,18,10,5,18,19,0,0,12,0,7,3,1,0,3,3,9,4,5,0,0,14,23]
CMF [5,23,16,8,23,21,0,0,0,0,3,0,0,0,4,6,3,2,0,0,0,17,23]
RMF [7,15,17,8,7,12,0,0,4,0,0,0,0,0,24,21,0,0,0,0,0,13,56]
LMF [7,15,17,8,7,12,0,0,4,0,0,0,0,0,24,21,0,0,0,0,0,13,56]
AMF [14,23,17,7,21,14,17,0,0,0,0,0,0,0,5,7,2,3,0,0,0,3,37]
RWF [17,19,15,7,5,9,11,0,0,0,0,0,0,5,15,15,2,4,0,0,0,6,47]
LWF [17,19,15,7,5,9,11,0,0,0,0,0,0,5,15,15,2,4,0,0,0,6,47]
SS  [15,19,13,6,9,9,14,0,0,0,0,0,0,6,9,21,2,5,0,0,0,4,37]
CF  [31,23,9,5,0,0,36,0,0,3,0,0,0,3,5,5,8,2,3,0,0,0,38]
```

## Bônus por posição/familiaridade

Na ordem interna `[GK, CB, RB, LB, DMF, CMF, RMF, LMF, AMF, RWF, LWF, SS, CF]`:

```text
principal:          [8,8,9,9,9,8,8,8,8,10,10,9,8]
secundária completa:[5,5,4,4,4,5,5,5,5,4,4,5,5]
secundária baixa:   [3,3,2,2,2,3,3,3,3,2,2,3,3]
```

## Algoritmo reconstruído

1. Montar o vetor de 23 entradas.
2. Para as primeiras 22 entradas, transformar o atributo com:

```text
base = atributo >= 25 ? atributo - 25 : 1
```

3. Multiplicar cada `base` pelo peso da posição e somar.
4. A frequência do pé fraco entra com seu valor bruto, sem subtrair 25.
5. Calcular a nota ponderada:

```text
weighted = floor((sum + 50) / 100)
positionScore = weighted + bonus
```

6. Quando a posição avaliada é a posição registrada e o jogador não é goleiro, calcular também a média dos primeiros 22 valores brutos:

```text
mean = floor((sumRaw22 + 11) / 22)
curveValue = averageCurve[mean]
overall = floor((positionScore * 60 + curveValue * 40 + 50) / 100)
```

7. Fora dessa condição, usar `positionScore` diretamente.
8. Limitar o resultado ao intervalo de 40 a 109.

### Curva da média

- Entradas de 0 a 39 retornam 0.
- Entradas de 40 a 61 retornam valores de 31 a 52, sequencialmente.
- Demais valores extraídos:

```text
62:56  63:59  64:62  65:65  66:67  67:70  68:73  69:75
70:77  71:79  72:82  73:83  74:85  75:87  76:89  77:90
78:92  79:93  80:94  81:95  82:96  83:97  84:98  85:99
86:100 87:101 88:102 89:103 90:104 91:105 92:106 93:107
94:108 95:109 96:110 97:111 98:112 99:113
```

## Validação realizada

- Zakaria Labyad, AMF: PES retornou 78 e o emulador retornou 78.
- Caso RWF observado: soma ponderada 7053; `71 + bônus 10 = 81`; média bruta 1484, curva 70; mistura final 77; PES retornou 77.
- Foram feitas chamadas diretas à função nativa para 104 vetores construídos: a implementação reconstruída coincidiu exatamente com a função do PES em todos esses casos.
- Comparação com os números anotados por screenshots: 84 de 104 coincidiram exatamente.
- Nos 20 restantes, a função nativa e o emulador ficaram exatamente `+1` em relação ao screenshot.

Jogadores com diferença residual de `+1` na amostra de screenshots:

```text
Fran García, Mateus Mané, Daniel Maldini, Robinio Vaz, Robert Lewandowski,
João Cancelo, Jules Koundé, Eric García, Rubén Sánchez, Omar El Hilali,
Adama Boiro, Marc Aguado, Jimmy Cabot, Florian Tardieu, Angelo Lucena,
Gustavo Scarpa, Dudu, Ademir, Erick Pulga e Martin Braithwaite.
```

Essa diferença não indica erro nos pesos: a chamada direta ao núcleo do PES confirmou a fórmula para os dados fornecidos. A causa mais provável é uma entrada decodificada com diferença de 1, um estado/familiaridade não representado no vetor, ou screenshots pertencentes a outro estado do `EDIT`.

## Arquivos de pesquisa no repositório

- `MLM/Tools/InspecionarOverallNoEdit.cpp`: inspeção do overall e dados no `EDIT`.
- `MLM/Tools/ValidarOverallsPes.ps1`: apoio à validação.
- `MLM/Tools/overall_targets.json`: alvos coletados durante os testes.
- `MLM/Tools/testar_formula_reddit_completa.py`: reprodução da fórmula encontrada.
- `MLM/Tools/testar_formula_print.py`: testes com a fórmula parcial dos prints.
- `MLM/Tools/testar_coeficientes_mimo.py`: testes com coeficientes de regressão.
- `MLM/Tools/calibrar_overall_pes.py`: calibração com amostras do PES.
- `MLM/Tools/calibrar_overall_pes2021_csv.py`: calibração usando CSV.
- `MLM/Tools/calibrar_overall_playersdb.py`: comparação com base externa.
- `MLM/Tools/coletar_pesdb_overalls.py`: coleta experimental do PESDB.
- `MLM/Tools/coletar_playersdb_pes21.py`: coleta experimental do PlayersDB.
- `MLM/Tools/analisar_formula_print_reddit.py`: análise dos coeficientes publicados.

As bases externas foram úteis apenas como referência. Elas não devem substituir o `EDIT` ativo como fonte de verdade do iMLM.

## Próximos passos recomendados

1. Consolidar a fórmula exata em uma única função de produção e remover o cálculo aproximado antigo.
2. Criar testes unitários por posição usando vetores e resultados já confirmados pela função nativa.
3. Auditar o mapeamento binário de cada atributo do `EDIT`, principalmente os casos residuais de `+1`.
4. Confirmar o campo de posição registrada e os níveis de familiaridade usados pelo wrapper.
5. Gerar `pes_database.json` novamente com a fórmula final para todos os jogadores.
6. Comparar uma amostra estratificada por posição e faixa de overall diretamente no PES.
7. Só depois congelar essa extração como banco-base de novos saves do iMLM.
8. Guardar, por save, uma cópia/versionamento do `EDIT` para transferências e evolução, sem modificar a base original.

## Critério de conclusão

O problema só deve ser considerado resolvido quando:

- os atributos do `EDIT` forem decodificados integralmente;
- a posição/familiaridade utilizada pelo PES estiver correta;
- a fórmula produzir o mesmo overall do jogo em amostra ampla de todas as posições;
- a geração do banco completo for repetível a partir de qualquer `EDIT` compatível;
- nenhuma correção manual por jogador for necessária.

## Observação importante para continuidade

Não voltar a ajustar overall jogador por jogador. Os jogadores mostrados nos prints servem apenas como vetores de validação. O objetivo é resolver globalmente a leitura dos campos e o cálculo por posição para que transferências, evolução, múltiplos saves e sincronização automática do `EDIT` sejam confiáveis.
