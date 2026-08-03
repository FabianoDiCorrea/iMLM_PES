# Relatório de calibração de overall PES 2021 / GOGOSZ

## Resumo

- Amostras reais: 104
- Fórmula atual: 22/104 OK, MAE 1.91
- Modelo calibrado em treino: 83/104 OK, MAE 0.20

> Observação: modelo calibrado em treino não é prova de fórmula oficial. A prova vem do erro em novas amostras e validação leave-one-out por posição.

## ATACANTE

- Amostras: 7
- Treino: 7/7 OK, MAE 0.00, max 0
- Leave-one-out: 1/7 OK, MAE 1.43, max 4
- Principais sinais encontrados:
  - + `phys_cont` (0.63)
  - + `tight_pos` (0.57)
  - + `kick_pwr` (0.52)
  - + `lowpass` (0.49)
  - + `atk` (0.48)
  - + `ball_ctrl` (0.47)
  - + `finish` (0.47)
  - + `drib` (0.43)

| Jogador | PES | Atual | Calibrado | Erro atual | Erro calibrado |
|---|---:|---:|---:|---:|---:|
| Kenan Yildiz | 80 | 76 | 80 | -4 | 0 |
| Daniel Maldini | 77 | 75 | 77 | -2 | 0 |
| Cauã Soares | 71 | 70 | 71 | -1 | 0 |
| Bernard | 76 | 75 | 76 | -1 | 0 |
| Memphis Depay | 81 | 77 | 81 | -4 | 0 |
| Fabinho | 75 | 73 | 75 | -2 | 0 |
| Bruno Rodrigues | 75 | 72 | 75 | -3 | 0 |

## CA

- Amostras: 18
- Treino: 9/18 OK, MAE 0.50, max 1
- Leave-one-out: 6/18 OK, MAE 1.11, max 3
- Principais sinais encontrados:
  - + `finish` (0.84)
  - + `kick_pwr` (0.81)
  - + `atk` (0.69)
  - + `ball_ctrl` (0.65)
  - + `lowpass` (0.60)
  - + `weak_acc` (0.54)
  - + `drib` (0.47)
  - + `jump` (0.47)

| Jogador | PES | Atual | Calibrado | Erro atual | Erro calibrado |
|---|---:|---:|---:|---:|---:|
| Germán Cano | 79 | 77 | 79 | -2 | 0 |
| John Kennedy | 78 | 75 | 78 | -3 | 0 |
| Kaio Jorge | 81 | 79 | 82 | -2 | 1 |
| Ruan Índio | 70 | 74 | 70 | 4 | 0 |
| Lautaro Martínez | 89 | 83 | 89 | -6 | 0 |
| Pio Esposito | 77 | 78 | 76 | 1 | -1 |
| Andrej Kostic | 77 | 77 | 78 | 0 | 1 |
| Niclas Füllkrug | 81 | 79 | 80 | -2 | -1 |
| Santiago Giménez | 84 | 81 | 83 | -3 | -1 |
| Robinio Vaz | 72 | 76 | 73 | 4 | 1 |
| Artem Dovbyk | 83 | 80 | 82 | -3 | -1 |
| Robert Lewandowski | 89 | 86 | 89 | -3 | 0 |
| Hamza Abdelkarim | 75 | 76 | 76 | 1 | 1 |
| Enzo Vágner | 70 | 70 | 70 | 0 | 0 |
| Brayan Gravelo | 71 | 74 | 71 | 3 | 0 |
| Neyser Villarreal | 75 | 78 | 75 | 3 | 0 |
| Pedro | 82 | 80 | 82 | -2 | 0 |
| Martin Braithwaite | 79 | 77 | 78 | -2 | -1 |

## GOL

- Amostras: 11
- Treino: 11/11 OK, MAE 0.00, max 0
- Leave-one-out: 7/11 OK, MAE 0.36, max 1
- Principais sinais encontrados:
  - + `clearing` (1.17)
  - + `jump` (0.74)
  - + `cover` (0.73)
  - + `catching` (0.67)
  - + `reflex` (0.67)
  - + `gk` (0.53)
  - + `phys_cont` (0.27)
  - - `height` (-0.18)

| Jogador | PES | Atual | Calibrado | Erro atual | Erro calibrado |
|---|---:|---:|---:|---:|---:|
| Gabriel Brazao | 79 | 79 | 79 | 0 | 0 |
| Bento | 83 | 83 | 83 | 0 | 0 |
| Ederson | 86 | 86 | 86 | 0 | 0 |
| Agustín Rossi | 82 | 82 | 82 | 0 | 0 |
| Hugo Souza | 81 | 82 | 81 | 1 | 0 |
| Thiago Beltrame | 72 | 72 | 72 | 0 | 0 |
| Gabriel Grando | 77 | 78 | 77 | 1 | 0 |
| Weverton | 82 | 83 | 82 | 1 | 0 |
| Gabriel Menegon | 70 | 72 | 70 | 2 | 0 |
| Sergio Rochet | 78 | 79 | 78 | 1 | 0 |
| Anthoni Spier | 75 | 77 | 75 | 2 | 0 |

## LAT

- Amostras: 14
- Treino: 8/14 OK, MAE 0.43, max 1
- Leave-one-out: 3/14 OK, MAE 1.29, max 3
- Principais sinais encontrados:
  - + `ball_ctrl` (0.95)
  - + `phys_cont` (0.92)
  - + `stamina` (0.80)
  - + `lowpass` (0.75)
  - + `ball_win` (0.74)
  - + `def` (0.70)
  - + `speed` (0.62)
  - + `aggres` (0.58)

| Jogador | PES | Atual | Calibrado | Erro atual | Erro calibrado |
|---|---:|---:|---:|---:|---:|
| Fran García | 83 | 79 | 83 | -4 | 0 |
| Ferland Mendy | 83 | 79 | 83 | -4 | 0 |
| Dodô | 80 | 78 | 80 | -2 | 0 |
| Tariq Lamptey | 76 | 78 | 77 | 2 | 1 |
| João Cancelo | 85 | 79 | 85 | -6 | 0 |
| Jules Koundé | 86 | 82 | 85 | -4 | -1 |
| Eric García | 82 | 78 | 82 | -4 | 0 |
| Rubén Sánchez | 75 | 76 | 75 | 1 | 0 |
| Omar El Hilali | 68 | 69 | 69 | 1 | 1 |
| Carlos Romero | 68 | 69 | 69 | 1 | 1 |
| Javi Hernández | 78 | 77 | 78 | -1 | 0 |
| José Salinas | 72 | 71 | 71 | -1 | -1 |
| Adama Boiro | 76 | 75 | 77 | -1 | 1 |
| Yuri Berchiche | 81 | 77 | 81 | -4 | 0 |

## MEIA_OF

- Amostras: 10
- Treino: 10/10 OK, MAE 0.00, max 0
- Leave-one-out: 4/10 OK, MAE 0.90, max 3
- Principais sinais encontrados:
  - + `body_ctrl` (0.79)
  - + `speed` (0.60)
  - + `ball_ctrl` (0.56)
  - - `form` (-0.55)
  - + `lowpass` (0.47)
  - + `exp_pwr` (0.46)
  - + `loftpass` (0.41)
  - + `drib` (0.38)

| Jogador | PES | Atual | Calibrado | Erro atual | Erro calibrado |
|---|---:|---:|---:|---:|---:|
| Neymar | 83 | 83 | 83 | 0 | 0 |
| Morgan Gibbs-White | 80 | 76 | 80 | -4 | 0 |
| Elliot Anderson | 74 | 71 | 74 | -3 | 0 |
| Mateus Mané | 74 | 73 | 74 | -1 | 0 |
| Tawanda Chirewa | 74 | 73 | 74 | -1 | 0 |
| Reinier | 78 | 77 | 78 | -1 | 0 |
| Gustavo Scarpa | 79 | 79 | 79 | 0 | 0 |
| Igor Gomes | 78 | 77 | 78 | -1 | 0 |
| Mateus Iseppe | 71 | 73 | 71 | 2 | 0 |
| Mamady Cissé | 72 | 72 | 72 | 0 | 0 |

## MEIO

- Amostras: 10
- Treino: 10/10 OK, MAE 0.00, max 0
- Leave-one-out: 2/10 OK, MAE 1.60, max 4
- Principais sinais encontrados:
  - + `exp_pwr` (0.84)
  - + `body_ctrl` (0.66)
  - - `stamina` (-0.62)
  - + `lowpass` (0.60)
  - + `drib` (0.60)
  - + `loftpass` (0.52)
  - + `atk` (0.51)
  - + `injury` (0.50)

| Jogador | PES | Atual | Calibrado | Erro atual | Erro calibrado |
|---|---:|---:|---:|---:|---:|
| Cristian Medina | 81 | 77 | 81 | -4 | 0 |
| Gerson | 81 | 79 | 81 | -2 | 0 |
| Alan Franco | 78 | 77 | 78 | -1 | 0 |
| Maycon | 80 | 72 | 80 | -8 | 0 |
| Allan | 80 | 80 | 80 | 0 | 0 |
| Danilo | 80 | 78 | 80 | -2 | 0 |
| Wallace Davi | 70 | 72 | 70 | 2 | 0 |
| Arthur Novaes | 71 | 72 | 71 | 1 | 0 |
| Higor Meritão | 76 | 77 | 76 | 1 | 0 |
| Vinicius Balieiro | 74 | 74 | 74 | 0 | 0 |

## MEIO_LADO

- Amostras: 14
- Treino: 8/14 OK, MAE 0.43, max 1
- Leave-one-out: 3/14 OK, MAE 1.36, max 3
- Principais sinais encontrados:
  - + `speed` (1.27)
  - + `ball_win` (1.15)
  - + `atk` (1.15)
  - + `def` (1.02)
  - + `exp_pwr` (0.87)
  - + `drib` (0.82)
  - + `tight_pos` (0.79)
  - + `stamina` (0.69)

| Jogador | PES | Atual | Calibrado | Erro atual | Erro calibrado |
|---|---:|---:|---:|---:|---:|
| Denzel Dumfries | 88 | 81 | 87 | -7 | -1 |
| Federico Dimarco | 87 | 81 | 87 | -6 | 0 |
| Marc Aguado | 67 | 70 | 68 | 3 | 1 |
| Ovie Ejaria | 73 | 71 | 72 | -2 | -1 |
| Tajon Buchanan | 79 | 79 | 79 | 0 | 0 |
| Romain Faivre | 74 | 72 | 74 | -2 | 0 |
| Elies Mahmoud | 66 | 69 | 66 | 3 | 0 |
| Jimmy Cabot | 75 | 75 | 76 | 0 | 1 |
| Florian Tardieu | 69 | 70 | 70 | 1 | 1 |
| T. Vargas | 67 | 68 | 68 | 1 | 1 |
| Yonser Parra | 69 | 69 | 69 | 0 | 0 |
| Joiser Arias | 69 | 70 | 69 | 1 | 0 |
| Angelo Lucena | 70 | 70 | 70 | 0 | 0 |
| Joantony Carmona | 71 | 72 | 71 | 1 | 0 |

## PONTA

- Amostras: 10
- Treino: 10/10 OK, MAE 0.00, max 0
- Leave-one-out: 6/10 OK, MAE 0.40, max 1
- Principais sinais encontrados:
  - + `speed` (0.78)
  - + `body_ctrl` (0.71)
  - - `form` (-0.65)
  - + `exp_pwr` (0.57)
  - + `loftpass` (0.56)
  - + `finish` (0.53)
  - + `atk` (0.48)
  - + `swerve` (0.33)

| Jogador | PES | Atual | Calibrado | Erro atual | Erro calibrado |
|---|---:|---:|---:|---:|---:|
| Robinho Junior | 73 | 73 | 73 | 0 | 0 |
| Gabriel Veneno | 70 | 71 | 70 | 1 | 0 |
| Junior Santos | 77 | 75 | 77 | -2 | 0 |
| Dudu | 78 | 75 | 78 | -3 | 0 |
| Tomás Cuello | 78 | 74 | 78 | -4 | 0 |
| Ruan Pablo | 70 | 69 | 70 | -1 | 0 |
| Lyan | 69 | 69 | 69 | 0 | 0 |
| Ademir | 78 | 74 | 78 | -4 | 0 |
| Mateo Sanabria | 78 | 75 | 78 | -3 | 0 |
| Erick Pulga | 79 | 76 | 79 | -3 | 0 |

## ZC

- Amostras: 10
- Treino: 10/10 OK, MAE 0.00, max 0
- Leave-one-out: 5/10 OK, MAE 0.50, max 1
- Principais sinais encontrados:
  - + `def` (0.62)
  - + `lowpass` (0.58)
  - + `ball_ctrl` (0.56)
  - + `header` (0.53)
  - + `aggres` (0.53)
  - + `ball_win` (0.51)
  - + `body_ctrl` (0.42)
  - + `speed` (0.42)

| Jogador | PES | Atual | Calibrado | Erro atual | Erro calibrado |
|---|---:|---:|---:|---:|---:|
| Bremer | 85 | 85 | 85 | 0 | 0 |
| Danilo | 80 | 80 | 80 | 0 | 0 |
| Carlos Cuesta | 80 | 80 | 80 | 0 | 0 |
| Gabriel Mercado | 77 | 79 | 77 | 2 | 0 |
| Gustavo Gómez | 81 | 81 | 81 | 0 | 0 |
| Clayton Sampaio | 72 | 75 | 72 | 3 | 0 |
| Pedro Kauã | 71 | 73 | 71 | 2 | 0 |
| João Dalla Corte | 72 | 74 | 72 | 2 | 0 |
| Victor Gabriel | 76 | 75 | 76 | -1 | 0 |
| Juninho | 76 | 78 | 76 | 2 | 0 |
