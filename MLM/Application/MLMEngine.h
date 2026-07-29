#ifndef MLM_ENGINE_H
#define MLM_ENGINE_H

#include <string>
#include "../Core/Result.h"

enum class SystemState {
    Uninitialized,
    Ready,
    CareerActive,
    Error,
    Shutdown
};

class MLMEngine {
public:
    MLMEngine();
    ~MLMEngine();

    // Ciclo de Vida do Sistema
    OperationResult Initialize(const std::string& configPath = "mlm_config.json");
    OperationResult Shutdown();

    // Gestão de Carreira
    OperationResult CreateNewCareer(const std::string& managerName, uint32_t startingTeamId);
    OperationResult LoadCareer(const std::string& saveFilePath);
    OperationResult SaveCareer(const std::string& saveFilePath);

    // Getters de Estado
    SystemState GetState() const { return m_state; }
    std::string GetVersion() const;
    bool IsCareerActive() const { return m_state == SystemState::CareerActive; }

private:
    SystemState m_state;
    std::string m_version;
    std::string m_currentSavePath;
    bool m_isInitialized;
};

#endif // MLM_ENGINE_H
