##################################
Hiveeyes e-Paper display CHANGELOG
##################################


***********
In progress
***********


****************
2021-01-15 0.1.1
****************

- Add missing ``#include <Adafruit_I2CDevice.h>``. Thanks, Sascha!


****************
2020-06-15 0.1.0
****************

- Add Hiveeyes HTTP data export module for Arduino,
  see also `Using the Hiveeyes HTTP export interface from Arduino`_.
- Add "make upload" target to Makefile.
- Add instructions about using PlatformIO on Windows natively.
- Add access to `Apicast HTTP API`_.
- Add Grafana panel images for displaying weight trends, see also
  `Anzeige von PNG-Bitmaps aus Grafana auf einem e-Paper Display`_.
- Add light sleep.
- Add touch control to wake up device by touch.


****************
2020-05-28 0.0.0
****************

- Build upon ``Waveshare_4_2.ino`` example from `ESP32-e-Paper-Weather-Display`_.


.. _ESP32-e-Paper-Weather-Display: https://github.com/G6EJD/ESP32-e-Paper-Weather-Display
.. _Using the Hiveeyes HTTP export interface from Arduino: https://community.hiveeyes.org/t/erschliessung-der-http-datenexportschnittstelle-via-arduino/3254
.. _Apicast HTTP API: https://community.hiveeyes.org/t/dwd-prognose-bienenflug/787/15
.. _Anzeige von PNG-Bitmaps aus Grafana auf einem e-Paper Display: https://community.hiveeyes.org/t/anzeige-von-png-bitmaps-aus-grafana-auf-einem-e-paper-display/3286
