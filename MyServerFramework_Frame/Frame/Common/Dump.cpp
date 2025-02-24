#include "Dump.h"

int Dump::max_lib = 0;
library_maps_t Dump::lib[Dump::MAX_LIB_NUM];

void Dump::init()
{
#ifdef LINUX
	signal(SIGPIPE, SIG_IGN);
	signal(SIGBUS, signal_exit);
	signal(SIGSEGV, signal_exit);
	dump_library_maps();
#endif
}

#ifdef LINUX
string Dump::output_addrline(bt_t* bt)
{
	char cmd[256] = { 0 };
	char line[2][256] = { 0 };
	int idx = 0;
	
	char* offset = bt->offset;

	if (bt == nullptr || bt->exe == nullptr)
	{
		printf("[%s][%d]Error bt:%p,exe:%p\n", __FUNCTION__, __LINE__, bt, bt->exe);
		return "";
	}

	if (offset < lib[idx].offset_start || offset > lib[idx].offset_end)
	{
		++idx;
		while (idx < max_lib && (offset < lib[idx].offset_start || offset > lib[idx].offset_end))
		{
			++idx;
		}
		if (idx == max_lib)
		{
			return "";
		}
		offset = (char*)(offset - lib[idx].offset_start);
	}

	snprintf(cmd, sizeof(cmd), "addr2line -Cif -e %s %p ", bt->exe, offset);
	FILE* file = popen(cmd, "r");
	if (file == nullptr)
	{
		return "";
	}

	int i = 0;
	while (i < 2 && fgets(line[i], 256, file) != nullptr)
	{
		++i;
	}

	pclose(file);
	line[0][strlength(line[0]) - 1] = 0;
	return string(line[0]) + ":" + line[1];
}

/***********************************************************
return:
	 0:success
	-1:false
************************************************************/
bool Dump::dump_library_maps()
{
	char cmd[64] = { 0 };
	char maps_line[512];
	int num_exe = 0;
	library_maps_t* temp = &lib[0];

	memset(&lib, 0, sizeof(lib));
	memset(maps_line, 0, sizeof(maps_line));

	// 1 get maps info to file
	snprintf(cmd, sizeof(cmd), "/proc/%d/maps", getpid());
	FILE* fd_maps = fopen(cmd, "r");
	if (fd_maps == nullptr)
	{
		printf("ERROR\n");
		return false;
	}

	// 2 save maps info to lib
	while (fgets(maps_line, sizeof(maps_line), fd_maps) != nullptr)
	{
		int maps_column_num = sscanf(maps_line, "%p-%p\t%s\t%s\t%s\t%s\t%s"
			, &temp->offset_start
			, &temp->offset_end
			, temp->not_care[0]
			, temp->not_care[1]
			, temp->not_care[2]
			, temp->not_care[3]
			, temp->library_path);
		//printf("%p-%p\t%s\t%s\t%s\t%s\t%s\n", temp->offset_start, temp->offset_end, temp->not_care[0],
		//	temp->not_care[1], temp->not_care[2], temp->not_care[3], temp->library_path);
		if (maps_column_num == 7 && ((num_exe == 0 && strcmp("r-xp", temp->not_care[0]) == 0) || strcmp(temp->library_path, temp[-1].library_path)))
		{
			if (num_exe == MAX_LIB_NUM)
			{
				printf("Error MAX_LIB_NUM is %d!!!\n", MAX_LIB_NUM);
				break;
			}
			++temp;
			++num_exe;
		}
		else
		{
			// so文件 是记录[min_offset, r-xp:offset_end]
			if (strcmp("r-xp", temp->not_care[0]) == 0 && strcmp(temp->library_path, temp[-1].library_path) == 0)
			{
				temp[-1].offset_end = temp->offset_end;
			}
			memset(temp, 0, sizeof(*temp));
		}
		memset(maps_line, 0, sizeof(maps_line));
	}

	fclose(fd_maps);
	max_lib = num_exe;
	//printf("[%s][%d]debug_backtrace_init success,num_exe:%d\n", __FUNCTION__, __LINE__, num_exe);
	num_exe = 0;
	while (num_exe < max_lib)
	{
		//printf("%p-%p\t%s\t%s\n", lib[num_exe].offset_start, lib[num_exe].offset_end, temp->not_care[0], lib[num_exe].library_path);
		++num_exe;
	}
	return true;
}

void Dump::signal_exit(int dunno)
{
	if (dunno == SIGBUS || dunno == SIGSEGV)
	{
		void* array[32] = { 0 };

		size_t size = backtrace(array, 32);
		char** strings = backtrace_symbols(array, size);
		if (strings == nullptr)
		{
			printf("[%s][%d]Error at backtrace_symbols()\n", __FUNCTION__, __LINE__);
			return;
		}

		string str;
		bt_t bt;
		for (size_t i = 1; i < size - 2; ++i)
		{
			sscanf(strings[i], "%[^(]%*[^ ] [%p]", bt.exe, &bt.offset);
			str += output_addrline(&bt) + "\n";
		}
		free(strings);

		writeFile("./crash-" + getTime() + ".txt", str);
	}
	exit(0);
}
#endif
