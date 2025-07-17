import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, sensor
from esphome.const import CONF_SENSOR_ID, CONF_PIN
from esphome.core import CORE

AUTO_LOAD = ["climate"]

samsung_ac_ns = cg.esphome_ns.namespace("samsung_ac")
SamsungClimate = samsung_ac_ns.class_("SamsungClimate", climate.Climate)

CONFIG_SCHEMA = climate.climate_schema(SamsungClimate).extend(
    {
        cv.Required(CONF_SENSOR_ID): cv.use_id(sensor.Sensor),
        cv.Required(CONF_PIN): cv.int_
    }
)

async def to_code(config):
    if CORE.is_esp8266 or CORE.is_esp32:
        cg.add_library("crankyoldgit/IRremoteESP8266", "2.8.6")

    var = cg.new_Pvariable(config[CONF_ID])
    await climate.register_climate(var, config)
    
    sens = await cg.get_variable(config[CONF_SENSOR_ID])
    cg.add(var.init(sens, config[CONF_PIN]))
