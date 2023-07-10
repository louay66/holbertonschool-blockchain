#include "hblk_crypto.h"

/**
 *ec_sign - signs a given set of bytes, using a given EC_KEY private key
 *@key:EC key to get the private key
 *@msg:message to sign
 *@msglen: lentgth of the message
 *@sig: signature strctur
 *Return: if successful  return pointer to the signature buffer
 * otherwise return NULL
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg,
					  size_t msglen, sig_t *sig)
{
	unsigned int len;

	if (!key || !msg || !sig)
		return (NULL);
	len = sig->len;

	if (!ECDSA_sign(0, msg, msglen, sig->sig, &len, (EC_KEY *)key))
	{
		fprintf(stderr, "ECDSA_sign() failed\n");
		return (NULL);
	}
	sig->len = len;

	return (sig->sig);
}
