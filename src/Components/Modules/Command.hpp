namespace Components
{
	class Command : public Component
	{
	public:
		class Params
		{
		public:
			Params() {};
			virtual ~Params() {};
			virtual char* get(size_t index) = 0;
			virtual size_t length() = 0;

			virtual std::string join(size_t startIndex);
			virtual char* operator[](size_t index);
		};

		class ClientParams : public Params
		{
		public:
			ClientParams(unsigned int id) : commandId(id) {};
			ClientParams(const ClientParams &obj) : commandId(obj.commandId) {};
			ClientParams() : ClientParams(*Game::cmd_id) {};

			char* get(size_t index) override;
			size_t length() override;

		private:
			unsigned int commandId;
		};

		class ServerParams : public Params
		{
		public:
			ServerParams(unsigned int id) : commandId(id) {};
			ServerParams(const ServerParams &obj) : commandId(obj.commandId) {};
			ServerParams() : ServerParams(*Game::cmd_id_sv) {};

			char* get(size_t index) override;
			size_t length() override;

		private:
			unsigned int commandId;
		};

		typedef void(Callback)(Command::Params* params);

		Command();
		~Command();

#if defined(DEBUG) || defined(FORCE_UNIT_TESTS)
		const char* getName() { return "Command"; };
#endif

		static Game::cmd_function_t* Allocate();

		static void Add(const char* name, Callback* callback);
		static void AddSV(const char* name, Callback* callback);
		static void AddRaw(const char* name, void(*callback)(), bool key = false);
		static void AddRawSV(const char* name, void(*callback)());
		static void Execute(std::string command, bool sync = true);

		static Game::cmd_function_t* Find(std::string command);

	private:
		static Utils::Memory::Allocator MemAllocator;
		static std::unordered_map<std::string, wink::slot<Callback>> FunctionMap;
		static std::unordered_map<std::string, wink::slot<Callback>> FunctionMapSV;

		static void MainCallback();
		static void MainCallbackSV();
	};
}
