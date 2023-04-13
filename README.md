# Fisher-Price Controller WiFi Integration

PlatformIO project for the software side of a project to integrate an ESP8266 into a "Fisher-Price Laugh & Learn Game & Learn Controller".

The button pushes are sent to an MQTT broker which can report to client software to control all sorts of stuff.

For a extensive write up on the hardware build process, see <https://www.robopenguins.com/toy-controller/>

The `python` folder has the software for the two demos:

[python/controller_demo.py](python/controller_demo.py) is simple PyGame demo:

[![PyGame video](https://img.youtube.com/vi/W4YEOwaM-SE/0.jpg)](https://www.youtube.com/shorts/W4YEOwaM-SE)

[python/wled_demo.py](python/wled_demo.py) is integrates the controller with a [WLED](https://kno.wled.ge/) project I did awhile back:

[![WLED video](https://img.youtube.com/vi/oS_-zvyJSHY/0.jpg)](https://youtu.be/oS_-zvyJSHY)
