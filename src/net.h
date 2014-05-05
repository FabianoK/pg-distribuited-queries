class Net{
	public:
		int sockConnect();
		int sendMessage(char*);
		int recMessage();
	private:
		int sockFD;
};
