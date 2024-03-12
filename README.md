# ESPHome Samsung AC IR remote control component

This implementation of the ESPHome component to control HVAC with IR channel.

Base implementation of the protocol can be found here: [https://github.com/crankyoldgit/IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266)

## Supported AC

* Samsung Inverter

## How to use

1. Add sensor configuration. It can be a connected temperature sensor or home assistant provided sensor. For example:

```
sensor:
  # External temperature sensor from Home Assistant 
  - platform: homeassistant
    id: current_temperature
    entity_id: ${temperature_sensor}
```

2. Add climate component, set platform as `samsung_ac`, set `sensor_id` and `pin` number for IR LED. 

```
climate:
  - platform: samsung_ac
    sensor_id: current_temperature # Sensor ID with the current temperature
    pin: 3 # Pin with IR led
    name: "AC Samsung"
```