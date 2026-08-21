#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

typedef struct {
    int birth_month;
    int birth_day;
    char target_path[256]; // Hersteld naar veilige buffergrootte
} AppConfig;

void initConfig(AppConfig* config) {
    config->birth_month = PersonalData->birthMonth;
    config->birth_day   = PersonalData->birthDay;
    strcpy(config->target_path, "sd:/BOOT.nds");
}

int calculateDaysToBirthday(int b_month, int b_day) {
    time_t t = time(NULL);
    struct tm* now = localtime(&t);
    
    struct tm bday = *now;
    bday.tm_mon = b_month - 1;
    bday.tm_mday = b_day; 
    bday.tm_hour = 0;
    bday.tm_min = 0;
    bday.tm_sec = 0;
    
    time_t now_seconds = mktime(now);
    time_t bday_seconds = mktime(&bday);
    
    if (bday_seconds < now_seconds) {
        bday.tm_year += 1;
        bday_seconds = mktime(&bday);
    }
    
    double diff = difftime(bday_seconds, now_seconds);
    return (int)ceil(diff / (24 * 3600));
}

int main(void) {
    // 1. Initialiseer video-modi
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    // 2. VRAM toewijzen (Bank A boven, Bank C onder)
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);

    // 3. Initialiseer console op het BOVENSTE scherm
    PrintConsole topConsole;
    consoleInit(&topConsole, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
    consoleSelect(&topConsole);

    AppConfig config;
    initConfig(&config);

    // Variabelen voor de live-update lus
    time_t last_t = 0;

    while(1) {
        scanKeys();
        int keys = keysDown();
        
        if (keys & KEY_A) {
            // Cursor verplaatsen naar een schone regel onderaan voor de boot-tekst
            printf("\x1b[13;0H\nBooting %s...\n", config.target_path);
            break;
        }

        // Haal de huidige tijd op
        time_t current_t = time(NULL);

        // Update het scherm alleen als de seconde daadwerkelijk is veranderd
        if (current_t != last_t) {
            last_t = current_t;
            struct tm* now = localtime(&current_t);
            int days_left = calculateDaysToBirthday(config.birth_month, config.birth_day);

            // clear for clean shii
            printf("\x1b[2J\x1b[H"); 
            printf("================================\n");
            printf("     DSi Birthday Countdown     \n");
            printf("================================\n\n");
            
            printf(" Current Time:      %02d:%02d:%02d\n", now->tm_hour, now->tm_min, now->tm_sec);
            printf(" Firmware Birthday: %02d-%02d\n", config.birth_day, config.birth_month);
            printf(" Days until birthday: %d   \n\n", days_left); // Spaties voorkomen tekstresten
            
            printf("--------------------------------\n");
            printf(" Hold A to start: %s\n", config.target_path);
            printf("--------------------------------\n");
        }
                
        swiWaitForVBlank();
    }

    return 0;
}
