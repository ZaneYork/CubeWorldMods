#include "main.h"
#include "CWSDK/cwsdk.h"

#ifndef NO_DEBUG_LOG
using namespace LOGGER;
char GLOBAL_BUFFER[1024];
#endif

#define CUBE_SIZE 0x57DC00
#define ADD_BASE_OFFSET (BASE_OFFSET < 0 ? 0 : BASE_OFFSET)

int BASE_OFFSET = -1;

void* Offset(void* x1, uint64_t x2) {
    return (void*)((char*)x1 + x2);
}

std::vector<std::vector<std::string>> GetLocalizationProperties(){
    std::ifstream configFile("Mods\\LocalizationMod\\config.txt");
    if(!configFile.is_open()) {
#ifndef NO_DEBUG_LOG
        logger->TraceInfo("Config not found\n");
#endif // NO_DEBUG_LOG
        return std::vector<std::vector<std::string>>();
    }
    std::string line;

    if (getline(configFile, line)) {
#ifndef NO_DEBUG_LOG
        logger->TraceInfo("Config: %s\n", line.c_str());
#endif // NO_DEBUG_LOG
        CCSVParse *parse = new CCSVParse();
        return parse->parseCsvFile("Mods\\LocalizationMod\\" + line);
    }
    return std::vector<std::vector<std::string>>();
}

long long safeStoll(std::string str, long long defaultValue) {
    try {
        return std::stoll(str);
    } catch ( ... ) {
        return defaultValue;
    }
}

bool startWith(const std::string &str, const std::string &head) {
	return str.compare(0, head.size(), head) == 0;
}

size_t hexToDecimal(std::string hexstring, size_t defaultValue) {
    if(hexstring.empty()){
        return defaultValue;
    }
    size_t decimal;
    std::stringstream ss;
    ss.str(hexstring);
    ss>>std::hex>>decimal;
    return decimal;
}

void zeroFill(char* buf, size_t origin_len, size_t fill_len){
    for(buf += origin_len;origin_len < fill_len;origin_len++){
        *buf++ = '\0';
    }
}

void HexOutput(const char* buf, size_t len)
{
#ifndef NO_DEBUG_LOG
    char* ptr = GLOBAL_BUFFER;
    for(size_t i=0; i<len; ++i)
    {
        unsigned char c = buf[i]; // must use unsigned char to print >128 value
        if( c< 16)
        {
            sprintf(ptr, "0%x", c);
        }
        else
        {
            sprintf(ptr, "%x", c);
        }
        ptr+=2;
    }
    logger->TraceInfo("Hex: %s", GLOBAL_BUFFER);
#endif // NO_DEBUG_LOG
}

void FindAndReplaceString(std::vector<std::vector<std::string>> entrys){
    char output[1024];
#ifndef NO_DEBUG_LOG
    logger->TraceInfo("Read %d lines.", entrys.size());
#endif // NO_DEBUG_LOG
    for(std::vector<std::string> item : entrys) {
        if(item.size() < 3) {
            continue;
        }
        std::string origin = item[0];
        std::string target = item[1];
        std::string encoding = item[2];
        size_t range_begin = 0x0;
        size_t range_end = CUBE_SIZE;
        if(item.size() > 3) {
            range_begin = hexToDecimal(item[3], range_begin);
            if(item.size() > 4) {
                range_end = hexToDecimal(item[4], range_end);
            }
        }
        if(!encoding.compare("Unicode")){
            bool ok;
            std::u16string u16str = utf8_to_utf16be(origin, false, &ok);
            size_t str_len = utf16_to_char(u16str, output);
#ifndef NO_DEBUG_LOG
            HexOutput(output, str_len);
#endif
            int targetOffset = nut::kmp_search((const char*)CWBase() + ADD_BASE_OFFSET + range_begin, range_end - range_begin, output, str_len);
            if(targetOffset >= 0) {
                if(BASE_OFFSET < 0) {
                    BASE_OFFSET = targetOffset;
                    targetOffset = 0x0;
                }
#ifndef NO_DEBUG_LOG
                if(targetOffset != 0x0){
                    logger->TraceInfo("Patching: %s", origin.c_str());
                    logger->TraceInfo("Search:0x%x-0x%x", range_begin, range_end);
                    logger->TraceInfo("Patched: %s", target.c_str());
                    logger->TraceInfo("Offset: 0x%x", targetOffset);
                }
#endif // NO_DEBUG_LOG
                std::u16string u16target = utf8_to_utf16be(target, false, &ok);
                size_t target_len = utf16_to_char(u16target, output);
                zeroFill(output, target_len, str_len);
                HexOutput(output, target_len);
                MemoryHelper::PatchMemory(Offset(CWBase(), BASE_OFFSET + range_begin + targetOffset), (void*)output, str_len);
            }
#ifndef NO_DEBUG_LOG
            else {
                logger->TraceInfo("Patching: %s", origin.c_str());
                logger->TraceInfo("Search:0x%x-0x%x", range_begin, range_end);
                logger->TraceError("Not found: %s", origin.c_str());
            }
#endif // NO_DEBUG_LOG
        }
        else if(!encoding.compare("ASCII")){
            size_t str_len = utf8_to_char(origin, output);
#ifndef NO_DEBUG_LOG
            HexOutput(output, str_len);
#endif // NO_DEBUG_LOG
            int targetOffset = nut::kmp_search((const char*)CWBase() + range_begin, range_end - range_begin, output, str_len);
            if(targetOffset >= 0) {
                if(BASE_OFFSET < 0) {
                    BASE_OFFSET = targetOffset;
                    targetOffset = 0x0;
                }
#ifndef NO_DEBUG_LOG
                if(targetOffset != 0x0){
                    logger->TraceInfo("Patching: %s", origin.c_str());
                    logger->TraceInfo("Patched: %s", target.c_str());
                    logger->TraceInfo("Offset: 0x%x", targetOffset);
                }
#endif // NO_DEBUG_LOG
                size_t target_len = utf8_to_char(target, output);
                zeroFill(output, target_len, str_len);
#ifndef NO_DEBUG_LOG
                HexOutput(output, str_len);
#endif // NO_DEBUG_LOG
                MemoryHelper::PatchMemory(Offset(CWBase(), BASE_OFFSET + range_begin + targetOffset), (void*)output, str_len);
            }
#ifndef NO_DEBUG_LOG
            else {
                logger->TraceInfo("Patching: %s", origin.c_str());
                logger->TraceInfo("Search:0x%x-0x%x", range_begin, range_end);
                logger->TraceError("Not found: %s", origin.c_str());
            }
#endif // NO_DEBUG_LOG
        }
        else if(!encoding.compare("ASCII_HEX")){
            size_t str_len = utf8_to_char(origin.c_str(), output);
#ifndef NO_DEBUG_LOG
            HexOutput(output, str_len);
#endif // NO_DEBUG_LOG
            int targetOffset = nut::kmp_search((const char*)CWBase() + ADD_BASE_OFFSET + range_begin, range_end - range_begin, output, str_len);
            if(targetOffset >= 0) {
                if(BASE_OFFSET < 0) {
                    BASE_OFFSET = targetOffset;
                    targetOffset = 0x0;
                }
#ifndef NO_DEBUG_LOG
                if(targetOffset != 0x0){
                    logger->TraceInfo("Patching: %s", origin.c_str());
                    logger->TraceInfo("Patched: %s", target.c_str());
                    logger->TraceInfo("Offset: 0x%x", targetOffset);
                }
#endif // NO_DEBUG_LOG
                size_t target_len = hex_to_char(target.c_str(), output);
                zeroFill(output, target_len, str_len);
#ifndef NO_DEBUG_LOG
                HexOutput(output, str_len);
#endif // NO_DEBUG_LOG
                MemoryHelper::PatchMemory(Offset(CWBase(), BASE_OFFSET + range_begin + targetOffset), (void*)output, str_len);
            }
#ifndef NO_DEBUG_LOG
            else {
                logger->TraceInfo("Patching: %s", origin.c_str());
                logger->TraceInfo("Search:0x%x-0x%x", range_begin, range_end);
                logger->TraceError("Not found: %s", origin.c_str());
            }
#endif // NO_DEBUG_LOG
        }
    }
}


class Mod : GenericMod {
    virtual void Initialize() override {
        if (BASE_OFFSET != -1) {
            return;
        }
        BASE_OFFSET = -2;
#ifndef NO_DEBUG_LOG
        logger = new CLogger(LogLevel_Info, CLogger::GetAppPathA() + "Mods\\LocalizationMod\\");
        logger->ChangeLogLevel(LOGGER::LogLevel_Info);
#endif // NO_DEBUG_LOG
        FindAndReplaceString(GetLocalizationProperties());
    }
};

EXPORT Mod* MakeMod() {
    return new Mod();
}
