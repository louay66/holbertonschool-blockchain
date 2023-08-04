#include "blockchain.h"

/**
 * hash_matches_difficulty-checks whether a
 * given hash matches a given difficulty
 * @hash: is the hash to check
 * @difficulty: is the minimum difficulty the hash should match
 * Return: if successful return 1 otherwise return 0
 */

int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
									 uint32_t difficulty)
{

	size_t i;

	if (!difficulty)
		return (1);
	for (i = 0; difficulty >= 8; i++)
	{
		difficulty -= 8;
		if (hash[i] != 0)
			return (0);
	}
	if (hash[i] >> (8 - difficulty) == 0)
		return (1);
	return (0);
}
