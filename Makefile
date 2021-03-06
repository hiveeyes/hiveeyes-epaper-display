$(eval venv      := .venv)
$(eval python    := $(venv)/bin/python)
$(eval pip       := $(venv)/bin/pip)
$(eval pio       := $(venv)/bin/pio)


build: setup-virtualenv
	$(pio) run

upload:
	$(pio) run --target upload --upload-port $(MCU_PORT)

setup-virtualenv:
	@test -e $(python) || python3 -m venv $(venv)
	$(pip) install platformio
