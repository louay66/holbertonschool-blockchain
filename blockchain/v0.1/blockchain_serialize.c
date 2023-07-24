#include "blockchain.h"
#include "provided/endianness.h"

#define MAGIC_HBLK "HBLK"
#define VERSION_HBLK "0.1"
/**
 * handel_error_file_IO-handell error of fwrite
 * @ptr: pointer to write
 * @size: ype of byte going to write
 * @nmemb: number of bytes
 * @stream: pointer to file
 * Return: if successful return 0 otherwise return -1
 */
int handel_error_file_IO(const void *ptr, size_t size, size_t nmemb,
								 FILE *stream)
{
	if (fwrite(ptr, size, nmemb, stream) != nmemb)
	{
		fclose(stream);
		return (-1);
	}
	return (0);
}
/**
 * blockchain_serialize-serialize a bloackchain to file format
 * @blockchain: pointer of bloockchain
 * @path: path of file to going save into it serializtion
 * Return: if successful return hash otherwise return NULL
 */

int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	int size, i;
	FILE *fptr;
	uint8_t endianess;
	block_t *block;

	if (!blockchain || !blockchain->chain || !path)
		return (-1);
	size = llist_size(blockchain->chain);
	endianess = _get_endianness();
	fptr = fopen(path, "wb");
	if (!fptr)
		return (-1);
	if (handel_error_file_IO(MAGIC_HBLK, 1, strlen(MAGIC_HBLK), fptr) == -1)
		return (-1);
	if (handel_error_file_IO(VERSION_HBLK, 1, strlen(VERSION_HBLK), fptr) == -1)
		return (-1);
	if (handel_error_file_IO(&endianess, 1, 1, fptr) == -1)
		return (-1);
	if (handel_error_file_IO(&size, 1, 4, fptr) == -1)
		return (-1);
	for (i = 0; i < size; i++)
	{
		block = llist_get_node_at(blockchain->chain, i);
		if (!block)
		{
			fclose(fptr);
			return (-1);
		}
		if (handel_error_file_IO(&block->info, sizeof(block->info), 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&block->data.len, 4, 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&block->data.buffer,
										 block->data.len + 1, 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&block->hash, SHA256_DIGEST_LENGTH, 1, fptr) == -1)
			return (-1);
	}
	fclose(fptr);
	return (0);
}
