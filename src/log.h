class Log{
	public:
		static void generateLog();
		void log();
	private:
		int logLevel;
		long time;
		long data;
		void getConfig();
	
};
