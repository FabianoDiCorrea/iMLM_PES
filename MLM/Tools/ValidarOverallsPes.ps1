param(
    [string]$DatabasePath = "..\UI\pes_database.json",
    [string]$TargetsPath = ".\overall_targets.json"
)

$database = Get-Content -LiteralPath $DatabasePath -Raw | ConvertFrom-Json
$targets = Get-Content -LiteralPath $TargetsPath -Raw | ConvertFrom-Json

$rows = @()

foreach ($target in $targets) {
    $club = $database.clubes | Where-Object { $_.id -eq $target.clubId } | Select-Object -First 1
    $player = $null
    if ($club) {
        $player = $club.jogadores | Where-Object { $_.id -eq $target.playerId } | Select-Object -First 1
    }

    if (-not $player) {
        $rows += [pscustomobject]@{
            Status = "NAO_ENCONTRADO"
            ClubeId = $target.clubId
            PlayerId = $target.playerId
            Nome = $target.nome
            Posicao = $target.posicaoPes
            Pes = $target.overallPesJogo
            Imlm = $null
            Diferenca = $null
        }
        continue
    }

    $diff = [int]$player.overall - [int]$target.overallPesJogo
    $rows += [pscustomobject]@{
        Status = if ($diff -eq 0) { "OK" } else { "DIVERGENTE" }
        ClubeId = $target.clubId
        PlayerId = $target.playerId
        Nome = $player.nome
        Posicao = $target.posicaoPes
        Pes = [int]$target.overallPesJogo
        Imlm = [int]$player.overall
        Diferenca = $diff
    }
}

$rows | Format-Table -AutoSize

$total = $rows.Count
$ok = @($rows | Where-Object { $_.Status -eq "OK" }).Count
$divergentes = @($rows | Where-Object { $_.Status -ne "OK" }).Count

Write-Host ""
Write-Host "Resumo: $ok/$total overalls batendo com os alvos PES; $divergentes divergentes."

if ($divergentes -gt 0) {
    exit 1
}
