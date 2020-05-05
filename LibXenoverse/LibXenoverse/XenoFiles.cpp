#include <stdio.h>  /* defines FILENAME_MAX */
//#ifdef WINDOWS
#ifdef WIN32
	#include <direct.h>
	#define GetCurrentDir _getcwd
#else
	#include <unistd.h>
	#define GetCurrentDir getcwd
#endif


#include <sstream>
#include <iomanip>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>



namespace LibXenoverse
{
	wstring_convert<codecvt<char16_t, char, mbstate_t>, char16_t> convert16;

	FILE *global_debugging_log = NULL;
	
	bool HAVE_WARNINGS = false;
	bool HAVE_ERRORS = false;
	ApplicationEndWait applicationEndWait = AEW_OnError;



	void setWarningErrorsFalse() { HAVE_WARNINGS = false; HAVE_ERRORS = false; }
	void notifyWarning() { HAVE_WARNINGS = true; }
	void notifyError() { HAVE_ERRORS = true; }
	void setApplicationEndWait(ApplicationEndWait aew) { applicationEndWait = aew; }
	bool haveToWait()
	{
		if (applicationEndWait == AEW_Always)
			return true;
		else if (applicationEndWait == AEW_None)
			return false;
		else if (applicationEndWait == AEW_OnError)
			return HAVE_ERRORS;
		else if (applicationEndWait == AEW_OnWarning)
			return HAVE_WARNINGS;
		return false;
	}
	void waitOnEnd() { if (haveToWait()) { printf("press Enter to continue ...\n");  getchar(); } }


	string fillStringNumberLeft(string value, size_t numberCharacter)
	{
		for (size_t i = value.length(); i < numberCharacter; i++)
			value = "0" + value;
		return value;
	}


	std::vector<string> initApplication(int argc, char** argv, ApplicationEndWait applicationEndWait)
	{
		LibXenoverse::setWarningErrorsFalse();
		LibXenoverse::setApplicationEndWait(applicationEndWait);
		LibXenoverse::initializeDebuggingLog();

		std::vector<string> arguments;
		string str_tmp = "";
		string str2_tmp = "";
		//for (int i = ((argc > 1) ? 1 : 0); i < argc; i++)									//detection of options, remove then from arguments list, plus take care of folder with spaces inside (use " ")
		for (int i = 1; i < argc; i++)
		{
			str_tmp = ToString(argv[i]);
			if ((str2_tmp.length() == 0) && (str_tmp.length() == 0))
				continue;

			if (str_tmp == "-NoWait")
			{
				LibXenoverse::setApplicationEndWait(LibXenoverse::AEW_None);
				continue;
			}else if (str_tmp == "-AlwaysWait") {
				LibXenoverse::setApplicationEndWait(LibXenoverse::AEW_Always);
				continue;
			}else if (str_tmp == "-WaitOnError") {
				LibXenoverse::setApplicationEndWait(LibXenoverse::AEW_OnError);
				continue;
			}else if (str_tmp == "-WaitOnWarning") {
				LibXenoverse::setApplicationEndWait(LibXenoverse::AEW_OnWarning);
				continue;
			}

			if ((str2_tmp.length() == 0) && (str_tmp[0] == '"'))
			{
				if (str_tmp[str_tmp.length() - 1] != '"')
				{
					str2_tmp = str_tmp.substr(1);
				}
				else {
					str2_tmp = str_tmp.substr(1, str_tmp.length() - 2);
					arguments.push_back(str2_tmp);
					str2_tmp = "";
				}
				continue;

			}else if (str2_tmp.length() != 0) {

				str2_tmp += " "+ str_tmp;

				if (str_tmp[str_tmp.length() - 1] == '"')
				{
					arguments.push_back(str2_tmp.substr(0, str2_tmp.length() - 1));
					str2_tmp = "";
				}
				continue;
			}

			arguments.push_back(str_tmp);
		}
		if (str2_tmp.length() != 0)					//put the last argument if they miss a " at the end
			arguments.push_back(str2_tmp);

		return arguments;
	}




	void initializeDebuggingLog()
	{
		#ifdef LIBXENOVERSE_DEBUGGING_LOG
		

		char cCurrentPath[FILENAME_MAX];
		if (GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
		{
			cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
		}else{
			cCurrentPath[0] = '\0';
		}
		string basePath(cCurrentPath);

		OutputDebugStringA( ("WorkingDir: " + basePath +"\n").c_str());

		global_debugging_log = fopen( (basePath + "\\libxenoverse.log").c_str(), "wt");
		#endif
	}

	void flushDebuggingLog() {
		if (!global_debugging_log) return;
		#ifdef LIBXENOVERSE_DEBUGGING_LOG
		fflush(global_debugging_log);
		#endif
	}

	void closeDebuggingLog() {
		if (!global_debugging_log) return;
		#ifdef LIBXENOVERSE_DEBUGGING_LOG
		fclose(global_debugging_log);
		#endif
	}

	void endianSwap(uint64_t& x)
	{
		//00   00     00    00		 00   00    00   00
		// A	B	   C	D		  E	   F	 G   H
		// =>
		// H    G	   F    E         D    C     B	 A

		x = (x >> 56) |
		((x >> 40) & 0x00FF000000000000) |
		((x >> 24) & 0x0000FF0000000000) |
		((x << 8)  & 0x000000FF00000000) |
		((x >> 8)  & 0x00000000FF000000) |
		((x << 24) & 0x0000000000FF0000) |
		((x << 40) & 0x000000000000FF00) |
		(x << 56);
	}

	void endianSwap(unsigned int& x) {
		x = (x>>24) |
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
	}

	void endianSwap(int& x) {
	    x = (x>>24) |
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
	}

	void endianSwap(unsigned short& x) {
	    x = (x>>8) | (x<<8);
	}

	File::File(string filename, string mode) {
		file_ptr=fopen(filename.c_str(), mode.c_str());

		path=filename;
		global_offset = 0;

		if (!file_ptr) {
			return;
		}
	}

	int File::endOfFile() {
		if (file_ptr) {
			return feof(file_ptr);
		}
		else {
			return true;
		}
	}

	bool File::valid() {
		if (file_ptr) return true;
		else return false;
	}

	void File::close() {
		if (file_ptr) {
			fclose(file_ptr);
		}
		else {
		}
	}

	bool File::readSafeCheck(void *dest) {
		if (!file_ptr) {
			return false;
		}
		if (!dest) {
			return false;
		}

		return true;
	}

	void File::read(void *dest, size_t sz) {
		if (!readSafeCheck(dest)) return;
		fread(dest, sz, 1, file_ptr);
	}

	void File::readInt16(unsigned short *dest) {
		if (!readSafeCheck(dest)) return;
		fread(dest, sizeof(short), 1, file_ptr);
	}
	
	void File::readInt16BE(unsigned short *dest) {
		if (!readSafeCheck(dest)) return;
		fread(dest, sizeof(short), 1, file_ptr);
		endianSwap(*dest);
	}

	void File::readInt32(unsigned int *dest) {
		if (!readSafeCheck(dest)) return;
		fread(dest, sizeof(int), 1, file_ptr);
	}
	void File::readInt64(uint64_t*dest) {
		if (!readSafeCheck(dest)) return;
		fread(dest, sizeof(uint64_t), 1, file_ptr);
	}

	void File::readInt32BE(unsigned int *dest) {
		if (!readSafeCheck(dest)) return;
		fread(dest, sizeof(int), 1, file_ptr);
		endianSwap(*dest);
	}
	void File::readInt64BE(uint64_t*dest) {
		if (!readSafeCheck(dest)) return;
		fread(dest, sizeof(uint64_t), 1, file_ptr);
		endianSwap(*dest);
	}

	void File::readFloat8(float *dest) {
		if (!readSafeCheck(dest)) return;
		unsigned char v=0;
		fread(&v, sizeof(char), 1, file_ptr);
		*dest = v / 256.0f;
	}

	void File::readFloat16(float *dest) {
		if (!readSafeCheck(dest)) return;
		
		unsigned short v=0;
		readInt16(&v);
		unsigned int f=half_to_float(v);
		*dest = *(float*)&(f);
	}

	void File::readFloat16BE(float *dest) {
		if (!readSafeCheck(dest)) return;
		
		unsigned short v=0;
		readInt16BE(&v);
		unsigned int f=half_to_float(v);
		*dest = *(float*)&(f);
	}

	void File::readFloat32(float *dest) {
		if (!readSafeCheck(dest)) return;
		fread(dest, sizeof(int), 1, file_ptr);
	}

	void File::readFloat32BE(float *dest) {
		if (!readSafeCheck(dest)) return;
		fread(dest, sizeof(int), 1, file_ptr);
		endianSwap(*((unsigned int *) dest));
	}

	void File::readUChar(unsigned char *dest) {
		if (!readSafeCheck(dest)) return;
		fread(dest, sizeof(char), 1, file_ptr);
	}

	void File::readString(string *dest) {
		if (!readSafeCheck(dest)) return;
		char c=1;
		*dest = "";
		while (c && !endOfFile()) {
			fread(&c, sizeof(char), 1, file_ptr);
			if (c) *dest += c;
		}
	}


	void File::readString16(u16string *dest) {
		if (!readSafeCheck(dest)) return;
		char16_t c = 1;
		dest->clear();
		while (c && !endOfFile()) {
			fread(&c, sizeof(char16_t), 1, file_ptr);
			if (c) *dest += c;
		}
	}

	bool File::readLine(string *dest) {
		char string_buffer[LIBXENOVERSE_FILE_STRING_BUFFER];
		if (fgets(string_buffer, LIBXENOVERSE_FILE_STRING_BUFFER, file_ptr)) {
			size_t ln = strlen(string_buffer) - 1;
			if (string_buffer[ln] == '\n') {
				string_buffer[ln] = '\0';
			}

			*dest = ToString(string_buffer);
			return true;
		}

		return false;
	}

	void File::readString(string *dest, size_t n) {
		if (!readSafeCheck(dest)) return;
		char c=1;
		*dest = "";
		for (size_t i=0; i<n; i++) {
			fread(&c, sizeof(char), 1, file_ptr);
			if (c) *dest += c;
		}
	}


	void File::readInt16E(unsigned short *dest) {
		if (big_endian) readInt16BE(dest);
		else readInt16(dest);
	}

	void File::readInt32E(unsigned int *dest) {
		if (big_endian) readInt32BE(dest);
		else readInt32(dest);
	}
	void File::readInt64E(uint64_t*dest) {
		if (big_endian) readInt64BE(dest);
		else readInt64(dest);
	}

	void File::readFloat16E(float *dest) {
		if (big_endian) readFloat16BE(dest);
		else readFloat16(dest);
	}

	void File::readFloat32E(float *dest) {
		if (big_endian) readFloat32BE(dest);
		else readFloat32(dest);
	}


	void File::writeInt16E(unsigned short *dest) {
		if (big_endian) writeInt16BE(dest);
		else writeInt16(dest);
	}

	void File::writeInt32E(unsigned int *dest) {
		if (big_endian) writeInt32BE(dest);
		else writeInt32(dest);
	}
	void File::writeInt64E(uint64_t *dest) {
		if (big_endian) writeInt64BE(dest);
		else writeInt64(dest);
	}

	/*
	void File::writeFloat8(float *dest) {
		if (!readSafeCheck(dest)) return;

		unsigned char v = half_from_float(*((uint32_t *)dest));		//Todo half_from_float => quart_from_float
		writeUChar(&v);
	}
	*/

	void File::writeFloat16E(float *dest) {
		if (big_endian) writeFloat16BE(dest);
		else writeFloat16(dest);
	}

	void File::writeFloat32E(float *dest) {
		if (big_endian) writeFloat32BE(dest);
		else writeFloat32(dest);
	}

	void File::write(void *dest, size_t sz) {
		if (!readSafeCheck(dest)) return;
		fwrite(dest, sz, 1, file_ptr);
	}
	
	void File::writeString(const char *dest) {
		if (!readSafeCheck((void *) dest)) return;

		fwrite(dest, sizeof(char), strlen(dest), file_ptr);
	}

	void File::writeString(string *dest) {
		if (!readSafeCheck(dest)) return;

		if (!dest->size()) writeNull(1);
		else fwrite(dest->c_str(), sizeof(char), dest->size()+1, file_ptr);
	}

	void File::writeUChar(unsigned char *dest) {
		if (!readSafeCheck(dest)) return;

		fwrite(dest, sizeof(char), 1, file_ptr);
	}

	void File::writeInt16(unsigned short *dest) {
		if (!readSafeCheck(dest)) return;

		unsigned short target=*dest;
		fwrite(&target, sizeof(short), 1, file_ptr);
	}

	void File::writeInt16BE(unsigned short *dest) {
		if (!readSafeCheck(dest)) return;

		unsigned short target=*dest;
		endianSwap(target);
		fwrite(&target, sizeof(short), 1, file_ptr);
	}

	void File::writeInt32(unsigned int *dest) {
		if (!readSafeCheck(dest)) return;

		fwrite(dest, sizeof(int), 1, file_ptr);
	}
	void File::writeInt64(uint64_t *dest) {
		if (!readSafeCheck(dest)) return;

		fwrite(dest, sizeof(uint64_t), 1, file_ptr);
	}

	
	void File::writeInt32BE(unsigned int *dest) {
		if (!readSafeCheck(dest)) return;

		unsigned int target=*dest;
		endianSwap(target);
		fwrite(&target, sizeof(int), 1, file_ptr);
	}
	void File::writeInt64BE(uint64_t *dest) {
		if (!readSafeCheck(dest)) return;

		uint64_t target = *dest;
		endianSwap(target);
		fwrite(&target, sizeof(uint64_t), 1, file_ptr);
	}
	

	void File::writeFloat32(float *dest) {
		if (!readSafeCheck(dest)) return;
		fwrite(dest, sizeof(float), 1, file_ptr);
	}

	void File::writeFloat32BE(float *dest) {
		if (!readSafeCheck(dest)) return;

		float target=*dest;
		endianSwap(*((unsigned int *) &target));
		fwrite(&target, sizeof(float), 1, file_ptr);
	}

	void File::writeFloat16(float *dest) {
		if (!readSafeCheck(dest)) return;

		unsigned short v = half_from_float(*((uint32_t *)dest));
		writeInt16(&v);
	}

	void File::writeFloat16BE(float *dest) {
		if (!readSafeCheck(dest)) return;

		unsigned short v = half_from_float(*((uint32_t *) dest));
		writeInt16BE(&v);
	}

	void File::writeNull(size_t size) {
		unsigned char zero=0;
		for (size_t i=0; i<size; i++) {
			fwrite(&zero, sizeof(char), 1, file_ptr);
		}
	}

	void File::writeText(const char *dest) {
		fprintf(file_ptr, dest);
	}

	size_t File::fixPadding(size_t multiple) {
		size_t address=getCurrentAddress();
		size_t extra=multiple - (address%multiple);

		if (extra == multiple) {
			extra = 0;
			return 0;
		}

		size_t zero=0;
		for (size_t c=0; c<extra; c++) fwrite(&zero, sizeof(char), 1, file_ptr);
		return extra;
	}

	size_t File::fixPaddingRead(size_t multiple) {
		size_t address=getCurrentAddress();
		size_t extra=multiple - (address%multiple);

		if (extra == multiple) {
			extra = 0;
			return 0;
		}
		goToAddress(address+extra);
		return extra;
	}

	size_t File::getFileSize() {
		if (!file_ptr) {
			return 0;
		}

		size_t address=getCurrentAddress();
		fseek(file_ptr, 0, SEEK_END);
		size_t sz=ftell(file_ptr);
		goToAddress(address);
		return sz;
	}

	bool File::readHeader(string verify_signature, int endianness)
	{
		if (!file_ptr)
		{
			return false;
		}

		size_t nbChar = verify_signature.size();
		if (nbChar > 8)					//security about the buffer of 8 bytes
		{
			nbChar = 8;					//we will only test on 8 chars.
			verify_signature = verify_signature.substr(0, nbChar);	//Note : we can do this because it's a copy. 
		}

		big_endian = false;
		memset(signature, 0, 8);
		read(signature, nbChar);		//before we only take flags with 4 bytes.

		if (!strcmp(signature, verify_signature.c_str())) 
		{
			if (endianness == -1)
			{
				unsigned short endian_flag = 0;
				readInt16(&endian_flag);

				if (endian_flag == 0xFEFF)
				{
					big_endian = true;
				}else if (endian_flag == 0xFFFE){
					big_endian = false;
				}
			}else {
				big_endian = (endianness == 1);
			}
			return true;
		}else {
			return false;
		}
	}

	void File::writeHeader(string new_signature, bool is_big_endian)
	{
		if (!file_ptr) {
			return;
		}

		big_endian = is_big_endian;

		memset(signature, 0, 8);
		strcpy(signature, new_signature.c_str());

		write(signature, 4);
		unsigned short endian_flag = (big_endian ? 0xFEFF : 0xFFFE);
		writeInt16(&endian_flag);
		writeNull(2);
	}

	size_t File::getCurrentAddress() {
		if (file_ptr) {
			return ftell(file_ptr)-global_offset;
		}
		else {
			return 0;
		}
	}

	void File::goToAddress(size_t address) {
		if (file_ptr) {
			fseek(file_ptr, address+global_offset, SEEK_SET);
		}
		else {
		}
	}

	void File::moveAddress(size_t address) {
		if (file_ptr) {
			fseek(file_ptr, address, SEEK_CUR);
		}
		else {
		}
	}

	void File::clone(string dest) {
		File file(dest, LIBXENOVERSE_FILE_WRITE_BINARY);
		if (file.valid()) {
			size_t sz=getFileSize();
			char *data=new char[sz];

			read(data, sz);
			file.write(data, sz);

			delete [] data;
			file.close();
		}
	}

	//-----------------------------------------------------------------------
	vector<string> File::split(const string& str, const string& delims, unsigned int maxSplits)
	{
		vector<string> ret;
		// Pre-allocate some space for performance
		ret.reserve(maxSplits ? maxSplits + 1 : 10);    // 10 is guessed capacity for most case

		unsigned int numSplits = 0;

		// Use STL methods 
		size_t start, pos;
		start = 0;
		do
		{
			pos = str.find_first_of(delims, start);
			if (pos == start)
			{
				// Do nothing
				start = pos + 1;
			}
			else if (pos == string::npos || (maxSplits && numSplits == maxSplits))
			{
				// Copy the rest of the string
				ret.push_back(str.substr(start));
				break;
			}
			else
			{
				// Copy up to delimiter
				ret.push_back(str.substr(start, pos - start));
				start = pos + 1;
			}
			// parse up to next real data
			start = str.find_first_not_of(delims, start);
			++numSplits;

		} while (pos != string::npos);



		return ret;
	}


	



	// trim from start
	std::string &ltrim(std::string &s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	// trim from end
	std::string &rtrim(std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	// trim from both ends
	std::string &trim(std::string &s) { return ltrim(rtrim(s)); }




	std::string FloatToString(float value)
	{
		char temp[32];
		std::string str;

		sprintf(temp, "%.9g", value);
		str = temp;

		if (str.find('.') == std::string::npos && str.find('e') == std::string::npos)
			str = str + ".0";

		return str;
	}

	float StringToFloat(string value)
	{
		if ((value == "+1.#INF") || (value == "1.#INF"))								//https://stackoverflow.com/questions/46694824/side-channel-resistant-math-functions-for-c
		{
			uint32_t tmp = 0x7f800000;
			return *((float*)((uint32_t*)&tmp));
		}else if (value == "-1.#INF"){
			uint32_t tmp = 0xff800000;
			return *((float*)((uint32_t*)&tmp));
		}else if ((value == "+1.#QNAN") || (value == "1.#QNAN")){
			uint32_t tmp = 0x7fC00000;
			return *((float*)((uint32_t*)&tmp));
		}else if (value == "-1.#QNAN"){
			uint32_t tmp = 0xffC00000;
			return *((float*)((uint32_t*)&tmp));
		}else if (value == "-1.#IND") {
			uint32_t tmp = 0xffc00000;
			return *((float*)((uint32_t*)&tmp));
		}

		string tmp = trim(value);

		float fval = 0;
		sscanf(tmp.c_str(), "%f", &fval);

		return fval;
	}


	bool File::compare(File *file) {
		if (!file) return false;
		if (getFileSize() != file->getFileSize()) return false;

		unsigned char source=0;
		unsigned char dest=0;

		goToAddress(0);
		file->goToAddress(0);

		while (!endOfFile()) {
			readUChar(&source);
			file->readUChar(&dest);

			if (source != dest) {
				return false;
			}
		}

		return true;
	}


	void File::createComparison(size_t sz) {
		comparison_bytes = new unsigned char[sz];
		comparison_bytes_min = new unsigned char[sz];
		comparison_bytes_max = new unsigned char[sz];
		comparison_size = sz;

		for (size_t i=0; i<comparison_size; i++) {
			comparison_bytes[i] = 0;
			comparison_bytes_min[i] = 255;
			comparison_bytes_max[i] = 0;
		}
	}

	void File::readComparison() {
		read(comparison_bytes, comparison_size);

		for (size_t i=0; i<comparison_size; i++) {
			if (comparison_bytes[i] < comparison_bytes_min[i]) comparison_bytes_min[i] = comparison_bytes[i];
			if (comparison_bytes[i] > comparison_bytes_max[i]) comparison_bytes_max[i] = comparison_bytes[i];
		}
	}

	void File::printComparisonResults() {
		for (size_t i=0; i<comparison_size; i++) {
			printf("Byte #%d Range: [%d, %d]\n", i, (int)comparison_bytes_min[i], (int)comparison_bytes_max[i]);
		}
		getchar();
	}

	void File::deleteComparison() {
		delete [] comparison_bytes;
		delete [] comparison_bytes_min;
		delete [] comparison_bytes_max;
	}

	bool fileCheck(string filename) {
		FILE *fp=fopen(filename.c_str(), "rb");

		if (fp) {
			fclose(fp);
			return true;
		}

		return false;
	}

	string extensionFromFilename(string filename, bool last_dot) {
		string name = filename;
		size_t sep = name.find_last_of("\\/");
		if (sep != std::string::npos) {
			name = name.substr(sep + 1, name.size() - sep - 1);
		}
	
		size_t dot = string::npos;
		if (last_dot) dot = name.find_last_of(".");
		else dot = name.find_first_of(".");

		if (dot != string::npos) {
			name = name.substr(dot+1, name.size()-dot-1);
		}
		else {
			name = "";
		}

		std::transform(name.begin(), name.end(), name.begin(), ::tolower);

		return name;
	}
	
	string nameFromFilename(string filename) {
		string name = filename;
		size_t sep = name.find_last_of("\\/");
		if (sep != std::string::npos) {
			name = name.substr(sep + 1, name.size() - sep - 1);
		}
		return name;
	}

	string nameFromFilenameNoExtension(string filename, bool last_dot) {
		string name = filename;
		size_t sep = name.find_last_of("\\/");
		if (sep != std::string::npos) {
			name = name.substr(sep + 1, name.size() - sep - 1);
		}
	
		size_t dot = string::npos;
		if (last_dot) dot = name.find_last_of(".");
		else dot = name.find_first_of(".");
		
		if (dot != string::npos) {
			name = name.substr(0, dot);
		}

		return name;
	}

	string filenameNoExtension(string filename) {
		string name = filename;

		size_t dot = string::npos;
		dot = name.find_last_of(".");

		if (dot != string::npos) {
			name = name.substr(0, dot);
		}

		return name;
	}

	string folderFromFilename(string filename) {
		size_t sep = filename.find_last_of("\\/");
		if (sep != std::string::npos) {
			return filename.substr(0, sep+1);
		}

		return "";
	}

	char *getBytesFrom(string filename, size_t &data_size) {
		File file(filename, LIBXENOVERSE_FILE_READ_BINARY);

		if (file.valid()) {
			data_size = file.getFileSize();
			char *data = new char[data_size];
			file.read(data, data_size);
			file.close();
			return data;
		}

		data_size = 0;
		return NULL;
	}

	bool writeBytesTo(string filename, char *data, size_t data_size) {
		File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
		if (file.valid()) {
			file.write(data, data_size);
			file.close();
			return true;
		}

		return false;
	}

	bool writeTextTo(string filename, char *data, size_t data_size) {
		File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
		if (file.valid()) {
			file.writeText(data);
			file.close();
			return true;
		}

		return false;
	}

	bool writeTextTo(string filename, const char *data, size_t data_size) {
		File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
		if (file.valid()) {
			file.writeText(data);
			file.close();
			return true;
		}

		return false;
	}

	std::vector<std::string> split(const std::string &text, char sep)
	{
		std::vector<std::string> tokens;
		std::size_t start = 0, end = 0;
		while ((end = text.find(sep, start)) != std::string::npos) {
			tokens.push_back(text.substr(start, end - start));
			start = end + 1;
		}
		tokens.push_back(text.substr(start));
		return tokens;
	}
	std::vector<std::string> multiSplit(const std::string &text, std::vector<char> sep)
	{
		std::vector<std::string> tokens;
		tokens.push_back(text);

		
		for (size_t i = 0, nb = sep.size(); i < nb; i++)
		{
			std::vector<std::string> tokens_b;

			for (size_t j = 0, nb2 = tokens.size(); j < nb2; j++)
			{
				std::vector<std::string> tokens_tmp = split(tokens.at(j), sep.at(i));

				for (size_t k = 0, nb3 = tokens_tmp.size(); k < nb3; k++)
					tokens_b.push_back(tokens_tmp.at(k));
			}

			tokens = tokens_b;
		}
		return tokens;
	}

	

	bool isNumber(const string& val)
	{
		std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > str(val);
		float tst;
		str >> tst;
		return !str.fail() && str.eof();
	}

	bool isIntNumber(const string& val)
	{
		std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > str(val);
		int tst;
		str >> tst;
		return !str.fail() && str.eof();
	}

	std::string toStringHexa(size_t val, unsigned short width, char fill, std::ios::fmtflags flags)
	{
		stringstream stream;
		stream.width(width);
		stream.fill(fill);
		if (flags)
			stream.setf(flags);
		stream << std::hex << val;
		return stream.str();
	}

	unsigned int parseHexaUnsignedInt(const std::string& val)
	{
		// Use istringstream for direct correspondence with toString
		std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > str;			//StringStream
		str << std::hex << val;

		unsigned int ret = 0;
		str >> ret;

		return ret;
	}
}