# Teste dos coeficientes Mimo contra GOGOSZ

- Fórmula atual iMLM: 10/20 OK, MAE 1.40
- Coeficientes Mimo + intercepto por posição: 8/20 OK, MAE 0.85
Coeficientes transcritos da imagem. Como o post não inclui interceptos, foi estimado um intercepto por posição usando a média dos alvos disponíveis.
`Defensive Engagement` foi tratado como zero porque não existe no parser PES21 atual.

## CA

- Amostras: 4
- Intercepto estimado: -63.0530

| Jogador | PES | Atual | Mimo bruto | Mimo pred | Erro Mimo |
|---|---:|---:|---:|---:|---:|
| Germán Cano | 79 | 77 | 140.71 | 78 | -1 |
| John Kennedy | 78 | 75 | 140.34 | 77 | -1 |
| Kaio Jorge | 81 | 79 | 144.22 | 81 | 0 |
| Ruan Índio | 70 | 74 | 134.94 | 72 | 2 |

- Resultado Mimo + intercepto local: 1/4 OK, MAE 1.00

## GOL

- Amostras: 5
- Intercepto estimado: -82.3628

| Jogador | PES | Atual | Mimo bruto | Mimo pred | Erro Mimo |
|---|---:|---:|---:|---:|---:|
| Gabriel Brazao | 79 | 79 | 158.88 | 77 | -2 |
| Bento | 83 | 83 | 164.37 | 82 | -1 |
| Ederson | 86 | 86 | 169.25 | 87 | 1 |
| Agustín Rossi | 82 | 82 | 164.65 | 82 | 0 |
| Hugo Souza | 81 | 82 | 165.66 | 83 | 2 |

- Resultado Mimo + intercepto local: 1/5 OK, MAE 1.20

## LE

- Amostras: 2
- Intercepto estimado: -54.3645

| Jogador | PES | Atual | Mimo bruto | Mimo pred | Erro Mimo |
|---|---:|---:|---:|---:|---:|
| Fran García | 83 | 79 | 138.00 | 84 | 1 |
| Ferland Mendy | 83 | 79 | 136.72 | 82 | -1 |

- Resultado Mimo + intercepto local: 0/2 OK, MAE 1.00

## MAT

- Amostras: 1
- Intercepto estimado: -55.3470

| Jogador | PES | Atual | Mimo bruto | Mimo pred | Erro Mimo |
|---|---:|---:|---:|---:|---:|
| Neymar | 83 | 83 | 138.35 | 83 | 0 |

- Resultado Mimo + intercepto local: 1/1 OK, MAE 0.00

## MLG

- Amostras: 2
- Intercepto estimado: -50.4815

| Jogador | PES | Atual | Mimo bruto | Mimo pred | Erro Mimo |
|---|---:|---:|---:|---:|---:|
| Cristian Medina | 81 | 77 | 129.72 | 79 | -2 |
| Gerson | 81 | 79 | 133.24 | 83 | 2 |

- Resultado Mimo + intercepto local: 0/2 OK, MAE 2.00

## PTD

- Amostras: 1
- Intercepto estimado: -57.8250

| Jogador | PES | Atual | Mimo bruto | Mimo pred | Erro Mimo |
|---|---:|---:|---:|---:|---:|
| Robinho Junior | 73 | 73 | 130.83 | 73 | 0 |

- Resultado Mimo + intercepto local: 1/1 OK, MAE 0.00

## ZC

- Amostras: 5
- Intercepto estimado: -67.8398

| Jogador | PES | Atual | Mimo bruto | Mimo pred | Erro Mimo |
|---|---:|---:|---:|---:|---:|
| Bremer | 85 | 85 | 153.21 | 85 | 0 |
| Danilo | 80 | 80 | 147.90 | 80 | 0 |
| Carlos Cuesta | 80 | 80 | 147.84 | 80 | 0 |
| Gabriel Mercado | 77 | 79 | 143.99 | 76 | -1 |
| Gustavo Gómez | 81 | 81 | 149.26 | 81 | 0 |

- Resultado Mimo + intercepto local: 4/5 OK, MAE 0.20
