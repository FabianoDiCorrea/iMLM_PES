#include <iostream>
#include <vector>
#include <string>
#include <clocale>
#include "../PesEditLib/EditDatabase.h"
#include "../PesEditLib/PlayerManager.h"
#include "../PesEditLib/TeamManager.h"
#include "../PesEditLib/TransferManager.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    std::cout << "==========================================" << std::endl;
    std::cout << "  PesEditLib Demo - Business API Test    " << std::endl;
    std::cout << "==========================================" << std::endl;

    EditDatabase db;
    if (!db.Load("EDIT00000000")) {
        std::cerr << "[ERROR] Failed to load EDIT00000000" << std::endl;
        return 1;
    }

    std::cout << "[LOG] Database loaded successfully." << std::endl;
    std::cout << "[LOG] Total Players: " << db.GetNumPlayers() << std::endl;
    std::cout << "[LOG] Total Teams: " << db.GetNumTeams() << std::endl;

    // Test 1: High Level Player Query
    uint32_t messi_id = 7511;
    Player *messi = db.Players().Get(messi_id);
    if (messi) {
        std::wcout << L"[PLAYER] Found: " << messi->name 
                  << L" | Overall: " << messi->overall 
                  << L" | Current Team Index: " << messi->team_index << std::endl;
    }

    // Test 2: Transfer via Clean API
    uint32_t roma_id = 125;
    uint32_t inter_miami_id = 1264;

    std::cout << std::endl << "--> Executing Transfer: Messi (7511) -> Inter Miami (1264)" << std::endl;
    TransferResult res = db.Transfers().TransferPlayer(messi_id, inter_miami_id);

    std::cout << "Transfer Success: " << (res.success ? "YES" : "NO") << std::endl;
    std::cout << "Message: " << res.message << std::endl;
    std::cout << "Old Slot: " << res.old_slot << " | New Slot: " << res.new_slot << std::endl;

    // Test 3: Validation & Save
    std::vector<std::string> errors;
    if (db.Validate(errors)) {
        std::cout << "[VALIDATION] Database is consistent. Saving EDIT00000000..." << std::endl;
        if (db.Save("EDIT00000000")) {
            std::cout << "[SUCCESS] File saved cleanly via PesEditLib API!" << std::endl;
        }
    } else {
        std::cerr << "[VALIDATION ERROR] Integrity check failed." << std::endl;
    }

    return 0;
}
