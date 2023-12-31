#include "hblk_crypto.h"

#define privetkey_size (strlen(folder) + strlen(PRI_FILENAME) + 2)
#define pubkey_size (strlen(folder) + strlen(PRI_FILENAME) + 2)
/**
 *ec_save - save a public/privet pair key each one in file
 *@key:EC_KEY structure
 *@folder:path to the folder that will contain the files
 *Return: if successful  return 1 otherwise return 0
 */
int ec_save(EC_KEY *key, char const *folder)
{
	FILE *pubKey, *privKey;
	char *prikeyFile, *pubkeyFile;

	prikeyFile = malloc(sizeof(privetkey_size));
	pubkeyFile = malloc(sizeof(pubkey_size));
	if (!prikeyFile || !pubkeyFile)
		return (0);

	if (!key)
		return (0);
	mkdir(folder, 0777);
	sprintf(prikeyFile, "%s/" PRI_FILENAME, folder);
	sprintf(pubkeyFile, "%s/" PUB_FILENAME, folder);

	privKey = fopen(prikeyFile, "w");
	pubKey = fopen(pubkeyFile, "w");
	if (!privKey || !pubKey)
	{
		fprintf(stderr, "fopen() failed\n");
		return (0);
	}
	else
	{
		if (!PEM_write_EC_PUBKEY(pubKey, key))
		{
			fclose(pubKey);
			return (0);
		}
		if (!PEM_write_ECPrivateKey(privKey, key, NULL, NULL, 0, NULL, NULL))
		{
			fclose(privKey);
			return (0);
		}
	}
	fclose(pubKey);
	fclose(privKey);
	free(prikeyFile);
	free(pubkeyFile);
	return (1);
}
