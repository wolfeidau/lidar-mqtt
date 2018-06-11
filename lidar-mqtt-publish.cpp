/*
Copyright 2018 Mark Wolfe

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <random>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <signal.h>
#include <memory>
#include <unistd.h>

#include <json/json.h>
#include "mqtt/async_client.h"

#include "CYdLidar.h"

using namespace std;
using namespace std::chrono;
using namespace ydlidar;

CYdLidar laser;

const string TOPIC { "data/lidar" };
const int    QOS = 1;

const auto PERIOD = milliseconds( 50 );

const int MAX_BUFFERED_MSGS = 120;  // 120 * 5sec => 10min off-line buffering

const string PERSIST_DIR { "data-persist" };

static bool running = true;

static void Stop( int signo )
{

    printf( "Received exit signal\n" );
    running = false;

}

/////////////////////////////////////////////////////////////////////////////

template <typename Iterable>
Json::Value iterable2json( Iterable const& cont )
{
    Json::Value v( Json::arrayValue );

    for( auto && element : cont ) {
        v.append( element );
    }

    return v;
}

int main( int argc, char* argv[] )
{

    bool showHelp  = argc > 1 && !strcmp( argv[1], "--help" );
    printf( " YDLIDAR C++ TEST\n" );

    if( argc < 4 || showHelp ) {

        printf( "Usage: %s <mqtt_address> <serial_port> <baudrate> <intensities>\n\n", argv[0] );
        printf( "Example:%s tcp://localhost:1883 /dev/ttyUSB0 115200 0\n\n", argv[0] );

        if( !showHelp ) {
            return -1;
        } else {
            return 0;
        }
    }

    const std::string address = string( argv[1] );
    const std::string port = string( argv[2] );
    const int baud =  atoi( argv[3] );
    const int intensities =  atoi( argv[4] );

    signal( SIGINT, Stop );
    signal( SIGTERM, Stop );

    mqtt::async_client cli( address, "", MAX_BUFFERED_MSGS, PERSIST_DIR );

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval( MAX_BUFFERED_MSGS * PERIOD );
    connOpts.set_clean_session( true );
    connOpts.set_automatic_reconnect( true );

    // Create a topic object. This is a convenience since we will
    // repeatedly publish messages with the same parameters.
    mqtt::topic top( cli, TOPIC, QOS, true );

    try {
        // Connect to the MQTT broker
        cout << "Connecting to server '" << address << "'..." << flush;
        cli.connect( connOpts )->wait();
        cout << "OK\n" << endl;

        // char tmbuf[32];
        // unsigned nsample = 0;

        // The time at which to reads the next sample, starting now
        auto tm = steady_clock::now();

        laser.setSerialPort( port );
        laser.setSerialBaudrate( baud );
        laser.setIntensities( intensities );

        Json::StreamWriterBuilder builder;
        builder.settings_["indentation"] = "";

        Json::Value fromScratch;

        while( running ) {
            this_thread::sleep_until( tm );

            bool hardError;
            LaserScan scan;

            if( laser.doProcessSimple( scan, hardError ) ) {
                fprintf( stderr, "received: %u ranges\n", ( unsigned int )scan.ranges.size() );

            }

            fromScratch["ranges"] = iterable2json( scan.ranges );
            fromScratch["intensities"] = iterable2json( scan.intensities );
            fromScratch["self_time_stamp"] = scan.self_time_stamp;
            fromScratch["system_time_stamp"] = scan.system_time_stamp ;

            // string payload = to_string( ( unsigned int )scan.ranges.size() );
            // cout << fromScratch << endl;

            // Publish to the topic
            top.publish( std::move( Json::writeString( builder, fromScratch ) ) );

            tm += PERIOD;
        }

        // Disconnect
        cout << "\nDisconnecting..." << flush;
        cli.disconnect()->wait();
        cout << "OK" << endl;
    } catch( const mqtt::exception& exc ) {
        cerr << exc.what() << endl;
        return 1;
    }

    return 0;
}