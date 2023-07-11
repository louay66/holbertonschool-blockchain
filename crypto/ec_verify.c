#include "hblk_crypto.h"

/**
 *ec_sign - verifies the signature of a given set of bytes,
 *using a given EC_KEY public key
 *@key:EC key to get the private key
 *@msg:message to sign
 *@msglen: lentgth of the message
 *@sig: signature strctur
 *Return: if successful  return 1
 * otherwise return 0
 */

int ec_verify(EC_KEY const *key, uint8_t const *msg,
				  size_t msglen, sig_t const *sig)
{
	if (!key || !msg || !sig)
		return (0);
	if (ECDSA_verify(0, msg, msglen, sig->sig, sig->len, (EC_KEY *)key) == 0)
	{
		return (0);
	}
	return (1);
}
