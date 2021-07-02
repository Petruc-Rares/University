package src.com.oop_pub.exceptions.ex4;

import java.util.EnumSet;

public abstract class LoggerBase {
    EnumSet<LogLevel> enumSet;
    LoggerBase nextLoggerBase;

    public LoggerBase(EnumSet<LogLevel> enumSet) {
        this.enumSet = enumSet;
    }

    public void setNext(LoggerBase loggerBase) {
        this.nextLoggerBase = loggerBase;
    }

    protected abstract void writeMessage(String message);

    public void message(String message, LogLevel severitate) {
        if (enumSet.contains(severitate)) {
            writeMessage(message);
        }
        if (nextLoggerBase != null) {
            nextLoggerBase.message(message, severitate);
        }
    }

}
