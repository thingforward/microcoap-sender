#include "microcoap/coap.h"

typedef struct coap_sender_struct_s {
	coap_packet_t	coap_packet;		/* coap packet to be constructed */
	int	build_res = -1;			/* return value of call to coap_build. 0=no error */

	char 	buf[256];			/* output buffer */
	size_t	buflen = 0;			/* length/size of buf, will be number of bytes for UDP packet */
		
	char 	buf_token[8];			/* buffer for token construction */

	int	option_idx = 0;

} coap_sender_struct_t;

/** clean the coap_sender_struct_t, set default values. Do this before re-using */
void coap_sender_clean(coap_sender_struct_t *s);

/** initialize the coap_sender_struct_t, with a default of Confirmable message and no token */
void coap_sender_init(coap_sender_struct_t *s, coap_msgtype_t type = COAP_TYPE_CON, int with_token_length = 0);

/** construct a get request. Set options (i.e. uri) later */
void coap_sender_get_request(coap_sender_struct_t *s);

/** construct a post request with given payload */
void coap_sender_post_request(coap_sender_struct_t *s, const uint8_t *payload, size_t sz_payload);

/** add an option */
void coap_sender_add_option(coap_sender_struct_t *s, coap_option_num_t num, coap_buffer_t o);

/** serialize the coap request into internal buffer */
int coap_sender_build(coap_sender_struct_t *s);

