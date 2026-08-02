call "M:\VS code\VC\Auxiliary\Build\vcvars64.bat"
cl /EHsc /std:c++17 /IPesTransferTest PesTransferTest\mt19937ar.cpp PesTransferTest\pes_crypt.cpp test_inspect_cup_teams.cpp /Fe:test_inspect_cup_teams.exe
test_inspect_cup_teams.exe
