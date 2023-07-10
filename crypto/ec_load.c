#include "hblk_crypto.h"

/**
 *ec_save - load a public/privet pair key from  file
 *@folder:path to the folder that will contain the files
 *Return: if successful  return EC_KEY otherwise return NULL
 */
EC_KEY *ec_load(char const *folder)
{
	EC_KEY *key = NULL;
	FILE *pubKey, *privKey;
	char prikeyFile[strlen(folder) + strlen(PRI_FILENAME) + 2];
	char pubkeyFile[strlen(folder) + strlen(PUB_FILENAME) + 2];

	sprintf(prikeyFile, "%s/" PRI_FILENAME, folder);
	sprintf(pubkeyFile, "%s/" PUB_FILENAME, folder);
	pubKey = fopen(pubkeyFile, "r");
	privKey = fopen(prikeyFile, "r");
	if (!pubKey || !privKey)
	{
		fprintf(stderr, "fopen() failed\n");
		return (NULL);
	}
	if (!PEM_read_EC_PUBKEY(pubKey, &key, NULL, NULL))
	{
		EC_KEY_free(key);
		fclose(pubKey);
		fprintf(stderr, "PEM_read_EC_PUBKEY() failed\n");
		return (NULL);
	}
	fclose(pubKey);

	if (!PEM_read_ECPrivateKey(privKey, &key, NULL, NULL))
	{
		fprintf(stderr, "PEM_read_ECPrivateKey() failed\n");
		EC_KEY_free(key);
		fclose(privKey);
	}
	fclose(privKey);
	return (key);
}
