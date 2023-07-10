#include "hblk_crypto.h"
/*
 * ec_create-create a privet/public pair key
 *Return: EC_KEY if successful otherwise return NULL
 */
EC_KEY *ec_create(void)
{
	EC_KEY *key;

	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!EC_KEY_generate_key(key))
	{
		EC_KEY_free(key);
		return (NULL);
	}
	return (key);
}
