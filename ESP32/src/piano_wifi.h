#ifndef PIANO_WIFI_H_
#define PIANO_WIFI_H_

extern const char *ssid;
extern const char *password;

void wifi_init();
void wifi_loop();

#endif