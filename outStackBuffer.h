typedef struct {
	int index;
	int stackLength;
	char * pBuffer;
} outStackBuffer, * poutStackBuffer;

bool initStack(poutStackBuffer pStack, int totalLength = 0)
{
	pStack->index = 0;
	if (totalLength > 1)
	{
		pStack->stackLength = totalLength;
	}
	else
	{
		pStack->stackLength = 1024;
	}
	pStack->pBuffer = new char[pStack->stackLength];

	if (pStack->pBuffer > 0)
	{
		return true;
	}
	pStack->pBuffer = NULL;
	return false;
}

void deleteStack(poutStackBuffer pStack)
{
	if (pStack->pBuffer)
	{
		delete[] pStack->pBuffer;
		pStack->pBuffer = NULL;
	}

	pStack->index = 0;
	pStack->stackLength = 0;
}

int pushStack(poutStackBuffer pStack, const char * pChar)
{
	if (pStack->index < pStack->stackLength)
	{
		*(pStack->pBuffer + pStack->index) = *pChar;
		pStack->index++;
		return pStack->index;
	}
	return -1;
}

int pushStack(poutStackBuffer pStack, const char * pChar, int length)
{
	if (!(pStack->index + length > pStack->stackLength))
	{
		CopyMemory(pStack->pBuffer + pStack->index, pChar, length);
		pStack->index += length;
		return pStack->index;
	}
	return -1;
}

void emptyStack(poutStackBuffer pStack)
{
	pStack->index = 0;
}