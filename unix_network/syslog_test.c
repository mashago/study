#include <stdio.h>
#include <syslog.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	/*
	 * syslog in fedora is rsyslog
	 * conf file is /etc/rsyslog.conf
	 * output log file is /var/log/message
	 */
	// void openlog(const char* ident, int options, int facility);
	// if ident is NULL, ident is program name
	openlog(NULL, LOG_NDELAY | LOG_PID, LOG_USER);

	// int syslog(int priority, const char *message, ...);
	syslog(LOG_INFO, "syslog_test:hello\n");

	closelog();

	return 0;
}
