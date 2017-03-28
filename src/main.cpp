#include <Arduino.h>

#include "tfwifi.h"
#include <WiFiUdp.h>

#include "microcoap/coap.h"
#include "coap_sender.h"

WiFiUDP	udp;

void setup() {
	Serial.begin(9600);
	setup_wifi();

	pinMode(A0,INPUT);
	randomSeed(analogRead(A0));
}

coap_sender_struct_t	sender;

/* intermedia buffer object to construct the uri */
const uint8_t	str_uri[] = "advanced";
coap_buffer_t	buf_uri = {
	.p = str_uri, .len = sizeof(str_uri)-1
};

/* intermedia buffer struct to construct content type */
coap_content_type_t 	int_cf = COAP_CONTENTTYPE_APPLICATION_OCTECT_STREAM;
coap_buffer_t	buf_cf = {
	.p = (const uint8_t*)&int_cf, .len = 1 
};

/* payload */
const uint8_t	str_payl[] = "Some POST payload...";

const char *p_server_ip = "172.20.10.6";


void loop() {
	delay(2000);

	/* if we're ready to send a packet .. */
	if ( udp.beginPacket(p_server_ip, 5683) == 1) {

		/* initialize coap sender structure */
		coap_sender_init(&sender,COAP_TYPE_NONCON, 4);

		/* form a request */
		//coap_sender_get_request(&sender);
		coap_sender_post_request(&sender, str_payl, sizeof(str_payl));
		/* add options */
		coap_sender_add_option(&sender, COAP_OPTION_URI_PATH, buf_uri);
		coap_sender_add_option(&sender, COAP_OPTION_CONTENT_FORMAT, buf_cf);

		/* build packet */
		if ( coap_sender_build(&sender) == 0 && sender.buflen > 0) {
			/* send using UDP */
			if ( udp.write(sender.buf, sender.buflen) != sender.buflen) {
				Serial.println("E udp.write");
			}
			udp.endPacket();
		} else {
			/* an error occured during construction of coap packet buffer */
			Serial.print("E coap_build, res=");
			Serial.println(sender.build_res);
		}
	} else {
		Serial.print("E udp.beginPacket");
	}
}
