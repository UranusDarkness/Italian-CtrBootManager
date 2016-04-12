#include <3ds.h>
#include <stdio.h>
#include "gfx.h"
#include "netloader.h"
#include "utility.h"
#include "loader.h"
#include "menu.h"

int menu_netloader() {

    if (netloader_activate() != 0) {
        debug("Err: netloader_activate");
        return -1;
    }

    char msg[256];
    u32 ip = (u32) gethostid();
    sprintf(msg,
            "NetLoader Attivato - attendendo per 3dslink\n\n"
                    "IP: %lu.%lu.%lu.%lu, Porta: %d\n\n"
                    "Premi B per annullare\n",
            ip & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, (ip >> 24) & 0xFF, NETLOADER_PORT);

    drawBg();
    gfxDrawTextf(GFX_TOP, GFX_LEFT, &fontDefault, MENU_MIN_X + 16, MENU_MIN_Y + 16, msg);
    gfxSwap();

    while (aptMainLoop()) {

        hidScanInput();
        u32 kDown = hidKeysDown();

        if (kDown & KEY_B) {
            netloader_deactivate();
            return -1;
        }

        int rc = netloader_loop();
        if (rc > 0) {
            netloader_boot = true;
            return load_3dsx(netloadedPath);
        } else if (rc < 0) {
            netloader_draw_error();
            break;
        }
    }
    return -1;
}
