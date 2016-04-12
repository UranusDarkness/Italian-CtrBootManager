#ifdef ARM9

#include "arm9/source/common.h"
#include "arm9/source/hid.h"

#else
#include <3ds.h>
#include <string.h>
#include "menu_netloaderarm9.h"
#endif
#include "gfx.h"
#include "utility.h"
#include "loader.h"
#include "menu.h"
#include "picker.h"

#ifdef ARM9
#define MENU_COUNT 4
static char menu_item[4][64] = {"File browser", "Opzioni", "Riavvia", "Spegni"};
#else
#define MENU_COUNT 6
static char menu_item[6][64] = {"File browser", "Netload 3dsx",
                                 "Netload arm9", "Opzioni", "Riavvia", "Spegni"};
#endif

static int menu_index = 0;
static void draw();

int menu_choose() {

    file_s picked;
    memset(&picked, 0, sizeof(file_s));
    pick_file(&picked, "/");

    if (strlen(picked.path) > 0) {
        return load(picked.path, 0);
    }

    return -1;
}

int menu_more() {

    menu_index = 0;

    while (aptMainLoop()) {

        hidScanInput();
        u32 kDown = hidKeysDown();

        if (kDown & KEY_DOWN) {
            menu_index++;
            if (menu_index >= MENU_COUNT)
                menu_index = 0;
        }
        else if (kDown & KEY_UP) {
            menu_index--;
            if (menu_index < 0)
                menu_index = MENU_COUNT - 1;
        }
        else if (kDown & KEY_A) {
            if (menu_index == 0 && menu_choose() == 0) {
                return 0;
#ifdef ARM9
            } else if (menu_index == 1) {
                menu_config();
            } else if (menu_index == 2) {
                reboot();
            } else if (menu_index == 3) {
                poweroff();
            }
#else
            } else if (menu_index == 1 && menu_netloader() == 0) {
                return 0;
            } else if (menu_index == 2 && menu_netloaderarm9() == 0) {
                return 0;
            } else if (menu_index == 3) {
                menu_config();
            } else if (menu_index == 4) {
                reboot();
            } else if (menu_index == 5) {
                poweroff();
            }
#endif
        }
        else if (kDown & KEY_B) {
            return -1;
        }

        draw();
    }
    return -1;
}

static void draw() {

    int i = 0;

    drawBg();
    drawTitle("*** Seleziona un'opzione ***");

    for (i = 0; i < MENU_COUNT; i++) {
        drawItem(i == menu_index, 16 * i, menu_item[i]);
    }

    // draw "help"
    switch (menu_index) {
#ifdef ARM9
        case 0:
            drawInfo("Cerca un file da avviare");
            break;
        case 1:
            drawInfo("Modifica opzioni di avvio");
            break;
        case 2:
            drawInfo("Riavvia il 3ds");
            break;
        case 3:
            drawInfo("Spegni il 3ds");
            break;
#else
        case 0:
            drawInfo("Cerca un file da avviare o da aggiungere come voce");
            break;
        case 1:
            drawInfo("Netload un file (3dsx) dal computer tramite 3dslink");
            break;
        case 2:
            drawInfo("Netload un file (arm9) dal computer tramite nc");
            break;
        case 3:
            drawInfo("Modifica impostazioni di avvio");
            break;
        case 4:
            drawInfo("Riavvia il 3ds");
            break;
        case 5:
            drawInfo("Spegni il 3ds");
            break;
#endif
        default:
            break;
    }

    gfxSwap();
}
