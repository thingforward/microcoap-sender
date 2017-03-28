#include <Arduino.h>

#include "microcoap/coap.h"
#include "coap_sender.h"


void coap_sender_clean(coap_sender_struct_t *s) {
	memset(s,0,sizeof(coap_sender_struct_t));
	s->option_idx = s->buflen = 0;
	s->build_res = -1;
}

void coap_sender_init(coap_sender_struct_t *s, coap_msgtype_t type, int with_token_length) {
	coap_sender_clean(s);

	coap_packet_t *pkt = &(s->coap_packet);

	/* set basic header data, choose random msg id */
	pkt->hdr.ver = 0x01;
	pkt->hdr.t = type;
	pkt->hdr.code = MAKE_RSPCODE(0,0);
	pkt->hdr.id[0] = random(1,255);
	pkt->hdr.id[1] = random(1,255);

	/* set token to random bytes, given a with_token_length > 0 */
	pkt->hdr.tkl = 0;
	if ( with_token_length > 0) {
		for( int i = 0; i < with_token_length; i++) {
			s->buf_token[i] = random(65,127);
		}
		pkt->hdr.tkl = strlen(s->buf_token);
	}
	pkt->tok.len = pkt->hdr.tkl;
	pkt->tok.p = (const uint8_t*)s->buf_token;

	pkt->payload.len = 0;
}

void coap_sender_get_request(coap_sender_struct_t *s) {
	coap_packet_t *pkt = &(s->coap_packet);
	pkt->hdr.code = MAKE_RSPCODE(0,1);
    	pkt->payload.len = 0;
}

void coap_sender_post_request(coap_sender_struct_t *s, const uint8_t *payload, size_t sz_payload) {
	coap_packet_t *pkt = &(s->coap_packet);
	pkt->hdr.code = MAKE_RSPCODE(0,2);
    	pkt->payload.len = sz_payload;
	pkt->payload.p = payload;
}

/*void coap_sender_add_option(coap_sender_struct_t *s, coap_option_t o) {
	coap_packet_t *pkt = &(s->coap_packet);
	if (s->option_idx >= MAXOPT) {
		return;
	}
	pkt->numopts = 1+(s->option_idx);
	pkt->opts[s->option_idx] = o;

	s->option_idx++;
}*/

void coap_sender_add_option(coap_sender_struct_t *s, coap_option_num_t num, coap_buffer_t o) {
	coap_packet_t *pkt = &(s->coap_packet);
	if (s->option_idx >= MAXOPT) {
		return;
	}
	pkt->numopts = 1+(s->option_idx);
	pkt->opts[s->option_idx].num = num;
	pkt->opts[s->option_idx].buf.len = o.len;
	pkt->opts[s->option_idx].buf.p = o.p;

	s->option_idx++;
}

int coap_sender_build(coap_sender_struct_t *s) {
	coap_packet_t *pkt = &(s->coap_packet);
	s->buflen = sizeof(s->buf);
	s->build_res = coap_build((uint8_t*)s->buf, &s->buflen, pkt);

	return s->build_res;
}


