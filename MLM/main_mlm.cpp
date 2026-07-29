#include <iostream>
#include "Application/MLMEngine.h"

int main()
{
    std::cout << "==================================================" << std::endl;
    std::cout << "  MASTER LEAGUE MANAGER (MLM) - LIFECYCLE DEMO    " << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    // 1. Instanciação do Núcleo
    MLMEngine engine;
    std::cout << "Versao do Sistema: " << engine.GetVersion() << std::endl;

    // 2. Inicialização do Ciclo de Vida
    OperationResult initRes = engine.Initialize("mlm_config.json");
    if (!initRes.success) {
        std::cerr << "[ERRO] " << initRes.message << std::endl;
        return 1;
    }

    // 3. Criação ou Carregamento de Carreira
    OperationResult careerRes = engine.CreateNewCareer("Fabiano", 125); // Roma ID: 125
    if (careerRes.success) {
        std::cout << "[OK] Status do Carreira: Ativo (" << (engine.IsCareerActive() ? "SIM" : "NAO") << ")" << std::endl;
    }

    // 4. Execução de Salvamento
    OperationResult saveRes = engine.SaveCareer("saves/minha_carreira.mlm");
    if (saveRes.success) {
        std::cout << "[OK] " << saveRes.message << std::endl;
    }

    // 5. Encerramento do Ciclo de Vida
    OperationResult shutdownRes = engine.Shutdown();
    if (shutdownRes.success) {
        std::cout << "[OK] Ciclo de Vida concluido com sucesso." << std::endl;
    }

    return 0;
}
