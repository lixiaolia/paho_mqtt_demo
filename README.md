# paho_mqtt_demo

### 下载与安装

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

### 双向认证

制作测试用的CA：

```bash
mkdir myCA
wget https://github.com/owntracks/tools/raw/master/TLS/generate-CA.sh
chmod +x generate-CA.sh
# 生成服务端证书
./generate-CA.sh server
# 用生成的CA，生成客户端证书
openssl genrsa -out client.key 2048
openssl req -new -out client.csr -key client.key -subj "/CN=client/O=example.com"
openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAserial ./ca.srl -out client.crt -days 3650 -addtrust clientAuth
```

启动mosquitto：

```bash
mosquitto -c ./ca_test.conf
```

编译带SSL认证的Demo程序：

```bash
mkdir build && cd build
cmake -DWITH_SSL=ON ..
make
```

分别启动订阅者和发布者：

```bash
./mqtt_sub
./mqtt_pub
```

