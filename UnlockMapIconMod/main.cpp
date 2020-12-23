#include "main.h"
#include "CWSDK/cwsdk.h"

#define MAP_ICON_DISPLAY_JUMP_OFFSET 0x5F6AE
#define MAP_TAG_DISPLAY_JUMP_OFFSET 0x283CE8

#define OP_JMP 0xEB
#define OP_JNZ 0x75
// 0F 84
#define OP_JZ 0x84
// 0F 80
#define OP_JO 0x80

void ModMessage(const wchar_t* message) {
    cube::Game* game = cube::GetGame();
    game->PrintMessage(L"[");
    game->PrintMessage(L"UnlockMapIconMod", 255, 140, 0);
    game->PrintMessage(L"] ");
    game->PrintMessage(message);
}

void* Offset(void* x1, uint64_t x2) {
    return (void*)((char*)x1 + x2);
}

void MapIconSwtich(int i) {
    if(!i) {
        WriteByte(Offset(CWBase(), MAP_ICON_DISPLAY_JUMP_OFFSET), OP_JNZ);
    }
    else {
        WriteByte(Offset(CWBase(), MAP_ICON_DISPLAY_JUMP_OFFSET), OP_JMP);
    }
}

void MapTagSwtich(int i){
    if(!i) {
        WriteByte(Offset(CWBase(), MAP_TAG_DISPLAY_JUMP_OFFSET), OP_JZ);
    }
    else {
        WriteByte(Offset(CWBase(), MAP_TAG_DISPLAY_JUMP_OFFSET), OP_JO);
    }
}

int HandleChat(wchar_t* msg) {
    wchar_t response[256];
    int state;

    if ( swscanf(msg, L"/map ico %d", &state) == 1) {
        MapIconSwtich(state);
        if(state == 0) {
            ModMessage(L"Map icon always display: off.\n");
            return 1;
        }
        ModMessage(L"Map icon always display: on.\n");
        return 1;

    } else if ( swscanf(msg, L"/map tag %d", &state) == 1) {
        MapTagSwtich(state);
        if(state == 0) {
            ModMessage(L"Map tag always display: off.\n");
            return 1;
        }
        ModMessage(L"Map tag always display: on.\n");
        return 1;

    } else if ( swscanf(msg, L"/map all %d", &state) == 1) {
        MapIconSwtich(state);
        MapTagSwtich(state);
        if(state == 0) {
            ModMessage(L"Map mark always display: off.\n");
            return 1;
        }
        ModMessage(L"Map mark always display: on.\n");
        return 1;

    }
    return 0;
}


class Mod : GenericMod {
    virtual void Initialize() override {
        MapIconSwtich(1);
        MapTagSwtich(1);
    }
    virtual int OnChat(std::wstring* message) override {
        wchar_t* msg = (wchar_t*)message->c_str();
        return HandleChat(msg);
    }
};

EXPORT Mod* MakeMod() {
    return new Mod();
}
