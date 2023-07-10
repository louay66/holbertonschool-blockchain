#include "hblk_crypto.h"
/*
 *ec_from_pub-build a EC_key from publickey buffer
 *@pub:buffer of publickey
 *Return: if successful  return EC_key otherwise return NULL
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{

	EC_KEY *key;
	EC_POINT *point;
	const EC_GROUP *group;

	if (!pub)
	{
		return (NULL);
	}
	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
	{
		fprintf(stderr, "EC_KEY_new_by_curve_name() is failed\n");
		return (NULL);
	}
	group = EC_KEY_get0_group(key);
	if (!group)
	{
		fprintf(stderr, "EC_KEY_get0_group() is failed\n");
		return (NULL);
	}
	point = EC_POINT_new(group);
	if (!group)
	{
		fprintf(stderr, "EC_POINT_new() is failed\n");
		return (NULL);
	}

	if (!EC_POINT_oct2point(group, point, pub, EC_PUB_LEN, NULL) ||
		 !EC_KEY_set_public_key(key, point))
	{
		EC_POINT_free(point);
		EC_KEY_free(key);
		return (NULL);
	}
	EC_POINT_free(point);
	return (key);
}
