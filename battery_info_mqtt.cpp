#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#else
#include <signal.h>
#endif

#include "rec/robotino/api2/all.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <mqtt/async_client.h>
#include <rec/robotino/api2/Com.h>

using namespace rec::robotino::api2;

//robotino 

const std::string MQTT_SERVER_ADDRESS = "192.168.0.1"; 
const std::string MQTT_CLIENT_ID = "robotino_client";
const std::string MQTT_TOPIC = "robotino/battery";

class MQTTCallback : public virtual mqtt::callback
{
    void connection_lost(const std::string& cause) override {}
    void delivery_complete(mqtt::delivery_token_ptr token) override {}

    void message_arrived(mqtt::const_message_ptr msg) override
    {
        // Not handling incoming messages in this example
    }
};

int main(int argc, char** argv)
{
    std::string hostname = "127.0.1.1";
    if (argc > 1)
    {
        hostname = argv[1];
    }

    rec::robotino::api2::Com com;
    rec::robotino::api2::OmniDrive omniDrive;
    rec::robotino::api2::PowerManagement pm;

    // Connect to Robotino
    com.setAddress(hostname.c_str());
    com.connectToServer(true);

    if (!com.isConnected())
    {
        std::cerr << "Could not connect to " << com.address() << std::endl;
        return 1;
    }
    // Create the MQTT client
    mqtt::async_client client(MQTT_SERVER_ADDRESS, MQTT_CLIENT_ID);

    // Set the MQTT callback
    MQTTCallback callback;
    client.set_callback(callback);

    // Connect to the MQTT broker
    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);
    mqtt::token_ptr conntok = client.connect(connOpts);
    conntok->wait();
    

    // Get voltage value and battery type
    float voltage = pm.voltage();
    std::string batteryType = pm.batteryType();
    
    // Print the values
    std::cout << "Voltage: " << voltage << std::endl;
    std::cout << "Battery Type: " << batteryType << std::endl;

    // Format data in MQTT payload
    std::string payload = "{\"voltage\": " + std::to_string(voltage) + ", \"battery_type\": \"" + batteryType + "\"}";

    // Publish data to MQTT topic
    mqtt::message_ptr pubmsg = mqtt::make_message(MQTT_TOPIC, payload);
    client.publish(pubmsg)->wait();

    // Disconnect from Robotino and MQTT broker
    com.disconnectFromServer();
    client.disconnect()->wait();

    return 0;
}




