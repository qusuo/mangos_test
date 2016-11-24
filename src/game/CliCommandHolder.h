//CliCommandHolder.h
#ifndef CLICOMMANDHOLDER_H
#define CLICOMMANDHOLDER_H


struct CliCommandHolder
{
	typedef std::function<void(const char *)> Print;
	typedef std::function<void(bool)> CommandFinished;

	uint32 m_cliAccountId;                                  // 0 for console and real account id for RA/soap
	AccountTypes m_cliAccessLevel;
	std::vector<char> m_command;
	Print m_print;
	CommandFinished m_commandFinished;

	CliCommandHolder(uint32 accountId, AccountTypes cliAccessLevel, const char* command, Print print, CommandFinished commandFinished)
		: m_cliAccountId(accountId), m_cliAccessLevel(cliAccessLevel), m_command(strlen(command) + 1), m_print(print), m_commandFinished(commandFinished)
	{
		memcpy(&m_command[0], command, m_command.size() - 1);
	}
};

#endif // CLICOMMANDHOLDER_H
