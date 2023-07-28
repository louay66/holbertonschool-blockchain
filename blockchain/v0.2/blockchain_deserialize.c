#include "blockchain.h"
#include "provided/endianness.h"

#define MAGIC_HBLK "HBLK"
#define VERSION_HBLK "0.1"
#define CHECK_ENDIAN(x) (endianess ? SWAPENDIAN(x) : (void)0)

llist_t *deserialize_blocks(FILE *file, int size, uint8_t endianess);

/**
 * blockchain_deserialize-deserialize a  file format to bloackchain
 * @path: path of file to get from it file  of blockchain serializted
 * Return: if successful return hash otherwise return NULL
 */
blockchain_t *blockchain_deserialize(char const *path)
{

	blockchain_t *blockchain;
	FILE *file;
	uint8_t endianess;
	int size;
	char buff[20];

	if (!path)
		return (NULL);

	file = fopen(path, "r");
	if (!file)
		return (NULL);
	fread(&buff, 7, 1, file);
	if (memcmp(buff, "HBLK0.1", 7) != 0)
	{
		fclose(file);
		return (NULL);
	}
	fread(&endianess, 1, 1, file);
	endianess = endianess != _get_endianness();
	fread(&size, 4, 1, file);
	CHECK_ENDIAN(size);
	blockchain = calloc(1, sizeof(*blockchain));
	blockchain->chain = deserialize_blocks(file, size, endianess);
	if (!blockchain->chain)
	{
		free(blockchain);
		fclose(file);
		return (NULL);
	}
	fclose(file);

	return (blockchain);
}

/**
 * deserialize_blocks-deserialize a  file format to blocks
 * @file: file to deserialize blocks from it
 * @size: size of  blocks in blockchain
 * @endianess: if 1 is big endian need to swap byte  else lettle endien
 * Return: if successful return hash otherwise return NULL
 */
llist_t *deserialize_blocks(FILE *file, int size, uint8_t endianess)
{
	llist_t *chain;
	block_t *block;
	int i;

	chain = llist_create(MT_SUPPORT_TRUE);
	if (!chain)
		return (NULL);

	for (i = 0; i < size; i++)
	{
		block = calloc(1, sizeof(*block));
		if (fread(&block->info, sizeof(block->info), 1, file) != 1)
		{
			free(block);
			llist_destroy(chain, 1, NULL);
			return (NULL);
		}
		CHECK_ENDIAN(block->info.index);
		CHECK_ENDIAN(block->info.difficulty);
		CHECK_ENDIAN(block->info.timestamp);
		CHECK_ENDIAN(block->info.nonce);
		if (fread(&block->data.len, 4, 1, file) != 1)
		{
			free(block);
			llist_destroy(chain, 1, NULL);
			return (NULL);
		}
		CHECK_ENDIAN(block->data.len);

		if (fread(&block->data.buffer, block->data.len, 1, file) != 1)
		{
			free(block);
			llist_destroy(chain, 1, NULL);
			return (NULL);
		}

		if (fread(&block->hash, SHA256_DIGEST_LENGTH, 1, file) != 1)
		{
			free(block);
			llist_destroy(chain, 1, NULL);
			return (NULL);
		}
		if (llist_add_node(chain, block, ADD_NODE_REAR))
		{
			free(block);
			llist_destroy(chain, 1, NULL);
			return (NULL);
		}
	}
	return (chain);
}
