#include "main.h"
#include "CWSDK/cwsdk.h"

void ModMessage(const wchar_t* message) {
    cube::Game* game = cube::GetGame();
    game->PrintMessage(L"[");
    game->PrintMessage(L"ChangeSpecializationMod", 255, 140, 0);
    game->PrintMessage(L"] ");
    game->PrintMessage(message);
}

bool CheckArmor(cube::Creature* player) {
    return (player->entity_data.equipment.weapon_left.category != 0
        ||player->entity_data.equipment.weapon_right.category != 0
        ||player->entity_data.equipment.chest.category != 0
        ||player->entity_data.equipment.hands.category != 0
        ||player->entity_data.equipment.shoulder.category != 0
        ||player->entity_data.equipment.feet.category != 0);
}

void SwtichClass(unsigned __int8 classType) {
    cube::Game* game = cube::GetGame();
    cube::Creature* player = game->GetPlayer();
    if(player->entity_data.classType != classType){
        switch(classType){
        case 1:            //战士
        case 2:            //游侠
        case 3:            //法师
        case 4:            //盗贼
            if(CheckArmor(player)){
                ModMessage(L"Take off armors and weapon first.\n");
                return;
            }
            player->entity_data.classType = classType;
            ModMessage(L"Class Type has transferred.\n");
            return;
        default:
            ModMessage(L"Illegal Class.\n");
            return;
        }
    }
    ModMessage(L"Nothing changed.\n");
}
void SwtichSpecialization(char specialization) {
    cube::Game* game = cube::GetGame();
    cube::Creature* player = game->GetPlayer();
    if(specialization == 0 || specialization == 1){
        player->entity_data.specialization = specialization;
        ModMessage(L"Specialization had transferred.\n");
        return;
    }
    ModMessage(L"Illegal Specialization.\n");
}

EXPORT int HandleChat(wchar_t* msg) {
    wchar_t response[256];
    unsigned __int8 classType;
    char specialization;
    int race;

    if ( swscanf(msg, L"/trans class %d", &classType) == 1) {
        SwtichClass(classType);
        return 1;

    }
    else if ( swscanf(msg, L"/trans spec %d", &specialization) == 1) {
        SwtichSpecialization(specialization);
        return 1;

    }
    return 0;
}

class Mod : GenericMod {
    virtual int OnChat(std::wstring* message) override {
        wchar_t* msg = (wchar_t*)message->c_str();
        return HandleChat(msg);
    }
};

EXPORT Mod* MakeMod() {
    return new Mod();
}
