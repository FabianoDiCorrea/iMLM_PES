# Teste da fórmula parcial do print Reddit

A base usada é `sum(peso * (atributo - 25)) / soma_pesos + 25`, que foi a única normalização plausível com o print parcial.
Depois foi testado um ajuste linear por posição apenas para medir se o esqueleto de pesos está perto dos alvos GOGOSZ.

## CA

- Amostras: 4
- Base print: 0/4 OK, MAE 2.50
- Base print + ajuste linear local: 1/4 OK, MAE 1.00
- Ajuste local: `overall = 1.9020 * base + -67.3649`

| Jogador | PES | Base | Pred base | Pred ajustado | Erro base | Erro ajustado |
|---|---:|---:|---:|---:|---:|---:|
| Germán Cano | 79 | 76.07 | 76 | 77 | -3 | -2 |
| John Kennedy | 78 | 76.25 | 76 | 78 | -2 | 0 |
| Kaio Jorge | 81 | 78.62 | 79 | 82 | -2 | 1 |
| Ruan Índio | 70 | 72.67 | 73 | 71 | 3 | 1 |

## GOL

- Amostras: 5
- Base print: 0/5 OK, MAE 1.80
- Base print + ajuste linear local: 2/5 OK, MAE 0.80
- Ajuste local: `overall = 0.9566 * base + 1.5987`

| Jogador | PES | Base | Pred base | Pred ajustado | Erro base | Erro ajustado |
|---|---:|---:|---:|---:|---:|---:|
| Gabriel Brazao | 79 | 80.45 | 80 | 79 | 1 | 0 |
| Bento | 83 | 84.37 | 84 | 82 | 1 | -1 |
| Ederson | 86 | 87.04 | 87 | 85 | 1 | -1 |
| Agustín Rossi | 82 | 84.38 | 84 | 82 | 2 | 0 |
| Hugo Souza | 81 | 85.04 | 85 | 83 | 4 | 2 |

## LE

- Amostras: 2
- Base print: 0/2 OK, MAE 6.50
- Base print + ajuste linear local: 2/2 OK, MAE 0.00
- Ajuste local: `overall = 0.0000 * base + 83.0000`

| Jogador | PES | Base | Pred base | Pred ajustado | Erro base | Erro ajustado |
|---|---:|---:|---:|---:|---:|---:|
| Fran García | 83 | 76.14 | 76 | 83 | -7 | 0 |
| Ferland Mendy | 83 | 76.75 | 77 | 83 | -6 | 0 |

## MAT

- Amostras: 1
- Base print: 0/1 OK, MAE 5.00
- Base print + ajuste linear local: 1/1 OK, MAE 0.00
- Ajuste local: `overall = 1.0000 * base + 4.7772`

| Jogador | PES | Base | Pred base | Pred ajustado | Erro base | Erro ajustado |
|---|---:|---:|---:|---:|---:|---:|
| Neymar | 83 | 78.22 | 78 | 83 | -5 | 0 |

## MLG

- Amostras: 2
- Base print: 0/2 OK, MAE 5.50
- Base print + ajuste linear local: 2/2 OK, MAE 0.00
- Ajuste local: `overall = 0.0000 * base + 81.0000`

| Jogador | PES | Base | Pred base | Pred ajustado | Erro base | Erro ajustado |
|---|---:|---:|---:|---:|---:|---:|
| Cristian Medina | 81 | 75.17 | 75 | 81 | -6 | 0 |
| Gerson | 81 | 75.93 | 76 | 81 | -5 | 0 |

## PTD

- Amostras: 1
- Base print: 1/1 OK, MAE 0.00
- Base print + ajuste linear local: 1/1 OK, MAE 0.00
- Ajuste local: `overall = 1.0000 * base + -0.2383`

| Jogador | PES | Base | Pred base | Pred ajustado | Erro base | Erro ajustado |
|---|---:|---:|---:|---:|---:|---:|
| Robinho Junior | 73 | 73.24 | 73 | 73 | 0 | 0 |

## ZC

- Amostras: 5
- Base print: 0/5 OK, MAE 3.20
- Base print + ajuste linear local: 5/5 OK, MAE 0.00
- Ajuste local: `overall = 1.4905 * base + -34.5745`

| Jogador | PES | Base | Pred base | Pred ajustado | Erro base | Erro ajustado |
|---|---:|---:|---:|---:|---:|---:|
| Bremer | 85 | 80.02 | 80 | 85 | -5 | 0 |
| Danilo | 80 | 76.89 | 77 | 80 | -3 | 0 |
| Carlos Cuesta | 80 | 76.96 | 77 | 80 | -3 | 0 |
| Gabriel Mercado | 77 | 74.68 | 75 | 77 | -2 | 0 |
| Gustavo Gómez | 81 | 77.82 | 78 | 81 | -3 | 0 |
