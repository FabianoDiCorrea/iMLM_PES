#include "MLMEngine.h"
#include <iostream>

MLMEngine::MLMEngine() 
    : m_state(SystemState::Uninitialized), 
      m_version("1.0.0-alpha"), 
      m_currentSavePath(""),
      m_isInitialized(false)
{
}

MLMEngine::~MLMEngine()
{
    if (m_state != SystemState::Shutdown && m_state != SystemState::Uninitialized) {
        Shutdown();
    }
}

OperationResult MLMEngine::Initialize(const std::string& configPath)
{
    if (m_isInitialized) {
        return OperationResult::Failure("MLMEngine ja esta inicializado.");
    }

    std::cout << "[MLM ENGINE] Inicializando nucleo do Master League Manager v" << m_version << "..." << std::endl;
    std::cout << "[MLM ENGINE] Carregando configuracoes de: " << configPath << std::endl;

    m_isInitialized = true;
    m_state = SystemState::Ready;

    std::cout << "[MLM ENGINE] Sistema inicializado e pronto para uso." << std::endl;
    return OperationResult::Success("MLMEngine inicializado com sucesso.");
}

OperationResult MLMEngine::CreateNewCareer(const std::string& managerName, uint32_t startingTeamId)
{
    if (!m_isInitialized) {
        return OperationResult::Failure("MLMEngine nao foi inicializado.");
    }

    std::cout << "[MLM ENGINE] Criando nova carreira..." << std::endl;
    std::cout << "[MLM ENGINE] Manager: " << managerName << " | Clube Inicial ID: " << startingTeamId << std::endl;

    m_state = SystemState::CareerActive;
    return OperationResult::Success("Nova carreira criada com sucesso.");
}

OperationResult MLMEngine::LoadCareer(const std::string& saveFilePath)
{
    if (!m_isInitialized) {
        return OperationResult::Failure("MLMEngine nao foi inicializado.");
    }

    std::cout << "[MLM ENGINE] Carregando carreira de: " << saveFilePath << "..." << std::endl;

    m_currentSavePath = saveFilePath;
    m_state = SystemState::CareerActive;
    return OperationResult::Success("Carreira carregada com sucesso.");
}

OperationResult MLMEngine::SaveCareer(const std::string& saveFilePath)
{
    if (!IsCareerActive()) {
        return OperationResult::Failure("Nenhuma carreira ativa para salvar.");
    }

    std::string targetPath = saveFilePath.empty() ? m_currentSavePath : saveFilePath;
    if (targetPath.empty()) {
        targetPath = "career_save.mlm";
    }

    std::cout << "[MLM ENGINE] Salvando carreira em: " << targetPath << "..." << std::endl;
    m_currentSavePath = targetPath;

    return OperationResult::Success("Carreira salva com sucesso.");
}

OperationResult MLMEngine::Shutdown()
{
    if (!m_isInitialized) {
        return OperationResult::Success("MLMEngine ja esta encerrado.");
    }

    std::cout << "[MLM ENGINE] Encerrando nucleo do Master League Manager..." << std::endl;

    m_isInitialized = false;
    m_state = SystemState::Shutdown;

    std::cout << "[MLM ENGINE] Nucleo encerrado limpadamente." << std::endl;
    return OperationResult::Success("MLMEngine encerrado com sucesso.");
}

std::string MLMEngine::GetVersion() const
{
    return m_version;
}
