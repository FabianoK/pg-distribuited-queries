#ifndef _LOG
#define _LOG 1

class Log{
	public:
		static void log(short, string);
		void log();
	private:
		int logLevel;
		long time;
		long data;
		void getConfig();
	
};

#endif
