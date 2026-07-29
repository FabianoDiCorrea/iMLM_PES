#ifndef TRANSFER_MANAGER_H
#define TRANSFER_MANAGER_H

#include "PesTypes.h"

class EditDatabase;

class TransferManager {
public:
    explicit TransferManager(EditDatabase &db);

    TransferResult TransferPlayer(uint32_t player_id, uint32_t dest_team_id);

private:
    EditDatabase &m_db;
};

#endif // TRANSFER_MANAGER_H
