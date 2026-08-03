# Mimo - coeficientes aproximados de overall

Fonte: texto anexado pelo Fabiano, com imagem "Estimation of Konami's Regression Coefficient for Overall Rating".

## Cuidado de versão

O post é do contexto `r/pesmobile` e usa atributos como `Defensive Engagement`, que não existe com esse nome na base PES 2021 PC/PS4 usada pelo iMLM.

Portanto, esses coeficientes não devem ser tratados automaticamente como fórmula oficial do PES 2021 Season Update + Patch GOGOSZ.

## O que o post confirma

- Overall não é campo armazenado: é cálculo em tempo real.
- O cálculo é linear por posição.
- Altura entra na fórmula.
- Lado esquerdo/direito tende a compartilhar coeficientes equivalentes.
- Arredondamento ocorre em 0.5.
- O post não fornece interceptos no texto anexado, apenas a matriz de coeficientes da imagem.

## Uso no iMLM

Esses coeficientes servem como mais uma hipótese de fórmula para comparar contra os alvos reais do GOGOSZ.

A implementação direta só deve acontecer se:

1. bater melhor que a fórmula atual nos alvos reais;
2. generalizar em novas amostras;
3. a diferença por posição ficar no máximo 1 na maior parte dos jogadores.
