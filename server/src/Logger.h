#ifndef P2P_RELAY__LOGGER_H
#define P2P_RELAY__LOGGER_H

#include <fstream>
#include <string>

// For easy conversion of various types to strings when logging messages
#define STR(x) std::to_string(x)

/*
 * An enumeration representing the log levels available. The entries
 * are in order of least verbose to most verbose logging.
 */
enum class LogLevel {
    None, Error, Info, Debug
};

/*
 * The Logger class provides a global interface for other files in
 * the p2p relay server to log messages to the console as they run.
 * Only one instance of this class exists globally and consumers may
 * obtain references to it whenever they like. Logging is performed in
 * stdout and can be viewed for a running container using the
 * 'docker logs' command.
 * 
 * The logger can be set to a specific log level. The default is Info.
 * When set to None, no messages are logged. When set to Debug, all
 * messages are logged. When a consumer logs a message, it will be logged
 * at a specific level and will only be logged if the logger is 
 * currently configured at the same level or at a MORE verbose level.
 * 
 * For unit testing of this feature, the P2P_RELAY_UNIT_TEST macro
 * is used to signify that logs should be saved locally in a buffer
 * as well as being written to stdout so they can be read by tests.
 */
class Logger {
 public:

    /*
     * Returns a reference to the single global logger instance.
     * The Logger may not be copied and so it must be received
     * in a reference variable that points to the same global object
     */
    static Logger &getGlobalLogger();

    /*
     * Sets the current log level for logger to one of the
     * entries in the enum LogLevel
     */
    void setLogLevel(LogLevel newLevel);
    
    /*
     * Returns the current log level setting of the global logger instance
     */
    const LogLevel getLogLevel();

    /*
     * Logs the given string at the 'Error' log level.
     */
    void error(const std::string &logString);

    /*
     * Logs the given string at the 'Info' log level.
     */
    void info(const std::string &logString);

    /*
     * Logs the given string at the 'Debug' log level.
     */
    void debug(const std::string &logString);

    #ifdef P2P_RELAY_UNIT_TEST

    /*
     * For unit test use only:
     * Returns the internal buffer holding a copy of
     * all the logged messages
     */
    const std::string getLogs();
    
    /*
     * For unit test use only:
     * Clears the internal buffer copy of logs to the empty string "" 
     */
    void clearLogs();

    #endif // P2P_RELAY_UNIT_TEST

 private:

    /*
     * Private constructor to force access through getGlobalLogger()
     */
    Logger();

    /*
     * Declare private copy constructor without implementation to ensure
     * no copies can be made of the single global logging instance
     */
    Logger(const Logger&);

    /*
     * Private pointer to the sole global logger instance.
     * Initially set to nullptr before first request for a reference.
     */
    static Logger *loggerInst;

    /*
     * The currently configured log level
     */
    LogLevel currentLogLevel;

    /*
     * The log file in use by the global logger instance
     */
    std::ofstream logOutputFile;

    /*
     * Log the given message string targeting the given log level
     */
    void log(LogLevel msgLogLevel, const std::string &logString);

    /*
     * For unit test use only:
     * Internal buffer holding a copy of all logs that have been output
     */
    #ifdef P2P_RELAY_UNIT_TEST
    std::string logs;
    #endif // P2P_RELAY_UNIT_TEST
};

#endif
