########################
Hiveeyes e-Paper display
########################


*****
About
*****
This builds upon the fine `ESP32-e-Paper-Weather-Display`_ project by
`David Bird`_ in order to `Display Hiveeyes data on an e-Paper display`_.

Currently, we are using the Waveshare 4.2" model.

.. figure:: https://community.hiveeyes.org/uploads/default/original/2X/5/5c749f54f7ac438f24e093757bc481aa3d43c085.jpeg


********
Building
********
This project is using PlatformIO_ for building.

Just invoke::

    make

After successfully building it, you will find firmware images at

- .pio/build/esp32/firmware.bin
- .pio/build/esp32/firmware.elf


*********
Uploading
*********
::

	source .venv3/bin/activate
	pio run --target upload --upload-port COM3


***********
Development
***********
There are integrations for the most popular IDEs around, see
`PlatformIO for CLion`_ and `PlatformIO for VSCode`_.


.. _David Bird: http://g6ejd.dynu.com/
.. _ESP32-e-Paper-Weather-Display: https://github.com/G6EJD/ESP32-e-Paper-Weather-Display
.. _Display Hiveeyes data on an e-Paper display: https://community.hiveeyes.org/t/anzeige-der-daten-auf-einem-e-paper-display/3229
.. _PlatformIO: https://platformio.org/
.. _PlatformIO for CLion: https://docs.platformio.org/en/latest/integration/ide/clion.html
.. _PlatformIO for VSCode: https://docs.platformio.org/en/latest/integration/ide/vscode.html
