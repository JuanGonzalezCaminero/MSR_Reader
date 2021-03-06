#include "msr_reader.h"

#define BUFFER_SIZE 32


FILE* open_msr(int core)
{
	char filename[BUFFER_SIZE];
	snprintf(filename, BUFFER_SIZE, "/dev/cpu/%d/msr", core);
	FILE *file = fopen(filename, "rb");

	if(!file)
	{
		printf("Could not open %s\n", filename);
	}

	return file;
}

unsigned long long read_msr(FILE *file, unsigned int address)
{
	//According to the specification, a long long is at least 64 bits long
	unsigned long long data;

	pread(fileno(file), &data, 8, address);

	return data;
}


void read_msr_fields(int core, 
				const int msr_address, 
				const int msr_numfields, 
				const int *msr_offsets, 
				const int *msr_sizes, 
				unsigned long long *msr_values)
{
	unsigned long long field = 0;

	FILE *file = open_msr(core);
	unsigned long long data = read_msr(file, msr_address);

	//Parse the fields and store their values
	for(int i=0; i<msr_numfields; i++)
	{
		field = 0;
		field = data >> msr_offsets[i];
		field = field & get_mask(msr_sizes[i]);
		msr_values[i]=field;
	}

	fclose(file);
}

unsigned long long get_mask(int size)
{
	unsigned long long mask = 0;
	for(int i=0; i<size; i++)
	{
		mask = mask << 1;
		mask += 1;
	}
	return mask;
}