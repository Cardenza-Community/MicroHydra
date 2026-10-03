"""Check generated device code never constructs Cardenza's nonexistent ADC."""
from pathlib import Path
import sys
from types import ModuleType, SimpleNamespace

boards = Path(__file__).resolve().parents[1] / "MicroPython/ports/esp32/boards"
adc_calls = []

class ADC:
    ATTN_11DB = 3
    def __init__(self, pin):
        adc_calls.append(pin)
    def atten(self, value):
        pass

sys.modules["machine"] = SimpleNamespace(ADC=ADC, Timer=object)

def read_module(path):
    namespace = {"const": lambda value: value}
    exec(compile(path.read_text(encoding="utf-8"), str(path), "exec"), namespace)
    return namespace

battery = read_module(boards / "CARDENZA/lib/battlevel.py")["Battery"]
try:
    battery()
except NotImplementedError:
    pass
else:
    raise AssertionError("Cardenza battery must report unavailable")
assert not adc_calls, adc_calls

for name, contents in {
    "lib.display": {"Display": SimpleNamespace(overlay_callbacks=[])},
    "lib.hydra.config": {"Config": object},
    "lib.hydra.utils": {"get_instance": lambda cls: None},
}.items():
    module = ModuleType(name)
    module.__dict__.update(contents)
    sys.modules[name] = module
status = read_module(boards / "CARDENZA/lib/hydra/statusbar.py")["StatusBar"]
assert not status(enable_battery=True, register_overlay=False).enable_battery
assert not adc_calls, adc_calls

# The original board still reads its actual battery ADC.
read_module(boards / "CARDPUTER/lib/battlevel.py")["Battery"]()
assert adc_calls == [10], adc_calls
print("PASS: Cardenza ADC unavailable, battery UI disabled, original Cardputer preserved")
