#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct {
    int birth_month;
    int birth_day;
    char target_path[256];
} AppConfig;

void initConfig(AppConfig* config) {
    // Read the user's birthday directly from the DSi internal firmware settings
    config->birth_month = PersonalData->birthMonth;
    config->birth_day   = PersonalData->birthDay;
    
    // Default boot target path
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
    
    // If the birthday already happened this year, calculate for next year
    if (bday_seconds < now_seconds) {
        bday.tm_year += 1;
        bday_seconds = mktime(&bday);
    }
    
    double diff = difftime(bday_seconds, now_seconds);
    return (int)(diff / (24 * 3600));
}

int main(void) {
    consoleDemoInit();

    AppConfig config;
    initConfig(&config);
    
    int days_left = calculateDaysToBirthday(config.birth_month, config.birth_day);

    printf("================================\n");
    printf("     DSi Birthday Countdown     \n");
    printf("================================\n\n");
    
    printf(" Firmware Birthday: %02d-%02d\n", config.birth_day, config.birth_month);
    printf(" Days until your birthday: %d\n\n", days_left);
    
    printf("--------------------------------\n");
    printf(" Hold A to start: %s\n", config.target_path);
    printf("--------------------------------\n");

    while(1) {
        scanKeys();
        int keys = keysDown();
        
        if (keys & KEY_A) {
            printf("\nBooting %s...\n", config.target_path);
            break;
        }
                
        swiWaitForVBlank();
    }

    return 0;
}
