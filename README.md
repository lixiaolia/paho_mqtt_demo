# paho_mqtt_demo

下载与安装 Paho C&C++ Library：

安装C++库之前需要先安装C库，以下步骤完全参考官方README：

```bash
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
git checkout v1.3.8
cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON
sudo cmake --build build/ --target install
sudo ldconfig
```

```bash
git clone https://github.com/eclipse/paho.mqtt.cpp.git
cd paho.mqtt.cpp
cmake -Bbuild -H. -DPAHO_BUILD_SHARED=ON -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE -DPAHO_WITH_SSL=ON
sudo cmake --build build/ --target install
sudo ldconfig
```

下载与安装 Mosquitto，官网：http://mosquitto.org/

```bash
tar -zvxf mosquitto-2.0.7.tar.gz
cd mosquitto-2.0.7
make
sudo make install
```

安装可选的依赖库cJSON：

```bash
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
make all
sudo make install
```

