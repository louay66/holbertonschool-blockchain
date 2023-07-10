#include "hblk_crypto.h"

/**
 * sha256- encrypt a a string using SHA256 algorithm
 *
 * @s: the string
 * @len: the length of the string
 * @digest: the buffer for hash
 * Return: if the buffer digest is null rturn NULL otherwise hash string
 */

uint8_t *sha256(int8_t const *s, size_t len,
		uint8_t digest[SHA256_DIGEST_LENGTH])
{
	if (!digest)
		return (NULL);

	return (SHA256((const unsigned char *)s, len, digest));
}
