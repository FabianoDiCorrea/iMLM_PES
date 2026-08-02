call "M:\VS code\VC\Auxiliary\Build\vcvars64.bat"
cl /EHsc /std:c++17 /IPesTransferTest PesTransferTest\mt19937ar.cpp PesTransferTest\pes_crypt.cpp test_inject_real_santos_team.cpp /Fe:gerar_santos_real.exe
gerar_santos_real.exe
